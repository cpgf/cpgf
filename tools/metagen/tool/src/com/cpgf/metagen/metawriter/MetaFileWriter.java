package com.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.List;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.codewriter.CppWriter;
import com.cpgf.metagen.doxyxmlparser.FileInfo;
import com.cpgf.metagen.metadata.CppClass;
import com.cpgf.metagen.metadata.Item;
import com.cpgf.metagen.metadata.MetaInfo;
import com.cpgf.metagen.metadata.Parameter;
import com.cpgf.metagen.metadata.TemplateInstance;
import com.cpgf.metagen.metawriter.callback.IOutputCallback;
import com.cpgf.metagen.metawriter.callback.OutputCallbackData;

public class MetaFileWriter {
	private FileInfo fileInfo;
	private Config config;
	private MetaInfo metaInfo;
	private List<CppClass> classList;
	private String sourceFileName;
	private OutputCallbackData callbackData;

	public MetaFileWriter(Config config, MetaInfo metaInfo, String sourceFileName, FileInfo fileInfo) {
		this.metaInfo = metaInfo;
		this.sourceFileName = sourceFileName;
		this.fileInfo = fileInfo;
		this.config = config;

		this.classList = new ArrayList<CppClass>();
	}
	
	public void addClass(CppClass cppClass) {
		this.classList.add(cppClass);
	}

	private void doCallback(Item item) {
		IOutputCallback callback = this.config.metaOutputCallback;

		if(callback != null) {
			this.callbackData = new OutputCallbackData();
			callback.outputCallback(item, this.callbackData);
		}
		else {
			if(this.callbackData == null) {
				this.callbackData = new OutputCallbackData();
			}
		}
	}

	public void writeHeader() throws Exception {
		CppWriter codeWriter = new CppWriter();

		WriterUtil.writeCommentForAutoGeneration(codeWriter);	

		codeWriter.beginIncludeGuard(Util.normalizeSymbol(this.getDestFileName()) + "_H");

		codeWriter.include("cpgf/gmetadefine.h");
		codeWriter.include("cpgf/metadata/gnamereplacer.h");
		codeWriter.include("cpgf/metadata/gmetadataconfig.h");
		codeWriter.include("cpgf/metadata/private/gmetadata_header.h");
		codeWriter.include("cpgf/gmetapolicy.h");
		codeWriter.out("\n\n");
				
		for(String ns : this.fileInfo.getNamespaceList()) {
			codeWriter.useNamespace(ns);
		}
		
		codeWriter.out("\n\n");

		codeWriter.beginNamespace(this.config.cppNamespace);

		List<CppClass>  sortedClassList = Util.sortClassList(this.classList);
		for(CppClass cppClass : sortedClassList) {
			this.doCallback(cppClass);

			if(this.callbackData.isSkipBind()) {
				continue;
			}
			
			if(! cppClass.canGenerateMetaCode()) {
				continue;
			}

			MetaClassWriter writer = new MetaClassWriter(this.config, codeWriter, cppClass);
			
			String funcName = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);
			
			if(this.callbackData.getHeaderCode() != null) {
				codeWriter.out(this.callbackData.getHeaderCode() + "\n\n");
			}

			this.beginMetaFunction(codeWriter, funcName, cppClass);
			writer.write();
			this.endMetaFunction(codeWriter);
			
			codeWriter.out("\n\n");
		}

		codeWriter.endNamespace(this.config.cppNamespace);
		
		codeWriter.out("\n\n");
		codeWriter.include("cpgf/metadata/private/gmetadata_footer.h");
		
		codeWriter.endIncludeGuard();
		
		Util.forceCreateDirectories(this.config.headerOutput);
		String outFileName = this.makeOutputFileName(this.config.headerExtension);
		Util.writeTextToFile(outFileName, codeWriter.getText());
	}

	public void writeSource(List<String> createFunctionNames) throws Exception { 
		if(! this.config.autoRegisterToGlobal) {
			return;
		}
		
		CppWriter codeWriter = new CppWriter();

		WriterUtil.writeCommentForAutoGeneration(codeWriter);	

		if(this.config.sourceHeaderCode != null) {
			codeWriter.out(this.config.sourceHeaderCode);
			codeWriter.out("\n");
		}
		if(this.config.sourceHeaderReplacer != null) {
			String fileName = this.sourceFileName;
			fileName.replaceAll("\\\\", "/");
			fileName = this.config.sourceHeaderReplacer.replaceSourceHeader(fileName);
			codeWriter.include(fileName);
			codeWriter.out("\n");
		}
		else {
			codeWriter.include(this.sourceFileName);
		}
		codeWriter.include(this.config.metaHeaderPath + this.getDestFileName() + ".h");
		codeWriter.out("\n");
		
		codeWriter.useNamespace("cpgf");
		codeWriter.out("\n");

		codeWriter.beginNamespace(this.config.cppNamespace);
		
		List<CppClass>  sortedClassList = Util.sortClassList(this.classList);
		for(CppClass cppClass : sortedClassList) {
			this.doCallback(cppClass);

			if(this.callbackData.isSkipBind()) {
				continue;
			}
			
			if(! cppClass.canGenerateMetaCode()) {
				continue;
			}
			
			List<TemplateInstance> templateInstanceList = null;
			
			if(cppClass.isTemplate()) {
				templateInstanceList = this.metaInfo.findTemplateInstances(cppClass);
				if(templateInstanceList == null) {
					continue;
				}
			}

			String funcName = this.createFunctionName(cppClass, this.config.metaClassCreatePrefix);
			createFunctionNames.add(funcName);

			if(this.callbackData.getSourceCode() != null) {
				codeWriter.out(this.callbackData.getSourceCode() + "\n\n");
			}

			codeWriter.out("GDefineMetaInfo " + funcName + "()\n");
			
			codeWriter.beginBlock();

			String callFunc = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);
			WriterUtil.createMetaClass(codeWriter, cppClass, "_d", callFunc, templateInstanceList);
			
			codeWriter.endBlock();

			codeWriter.out("\n\n");
		}
		
		codeWriter.endNamespace(this.config.cppNamespace);
		
		Util.forceCreateDirectories(this.config.sourceOutput);
		String outFileName = Util.concatFileName(this.config.sourceOutput, this.getDestFileName()) + this.config.sourceExtension;
		Util.writeTextToFile(outFileName, codeWriter.getText());
	}

	private String createFunctionName(CppClass cppClass, String prefix) {
		String className = this.getGlobalPostfix();
		if(! cppClass.isGlobal()) {
			className = cppClass.getPrimaryName();
		}
		className = Util.upcaseFirst(className);
			
		return prefix + className;
	}

	private void beginMetaFunction(CppWriter codeWriter, String name, CppClass cppClass) {
		codeWriter.out("template <typename D");
		if(cppClass.isTemplate()) {
			for(Parameter param : cppClass.getTemplateParameterList()) {
				codeWriter.out(", typename " + param.getName());
			}
		}
		codeWriter.out(">\n");
		codeWriter.out("void " + name + "(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)\n");
		codeWriter.beginBlock();
		codeWriter.out("(void)config; (void)_d; (void)_r; (void)_d;\n");
		codeWriter.useNamespace("cpgf");
		codeWriter.out("\n");
	}

	private void endMetaFunction(CppWriter codeWriter) {
		codeWriter.endBlock();
	}

	private String makeOutputFileName(String extension) {
		return Util.concatFileName(this.config.headerOutput, this.getDestFileName()) + extension;
	}

	private String getGlobalPostfix() {
		String g = "global_" + this.getFileNameSymbol();

		return g.toLowerCase();
	}

	private String getFileNameSymbol()
	{
		String s = Util.getBaseFileName(this.sourceFileName);

		return Util.normalizeSymbol(s);
	}

	private String getBaseFileName()
	{
		return Util.getBaseFileName(this.sourceFileName);
	}

	private String getDestFileName()
	{
		return this.config.sourceFilePrefix + this.getBaseFileName();
	}

}
