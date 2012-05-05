package com.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.List;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.codewriter.CppWriter;
import com.cpgf.metagen.doxyxmlparser.FileInfo;
import com.cpgf.metagen.metadata.CppClass;

public class MetaFileWriter {
	private FileInfo fileInfo;
	private Config config;
	private List<CppClass> classList;
	private String sourceFileName;
	
	public MetaFileWriter(Config config, String sourceFileName, FileInfo fileInfo) {
		this.sourceFileName = sourceFileName;
		this.fileInfo = fileInfo;
		this.config = config;

		this.classList = new ArrayList<CppClass>();
	}
	
	public void addClass(CppClass cppClass) {
		this.classList.add(cppClass);
	}

	public void writeHeader() throws Exception {
		CppWriter codeWriter = new CppWriter();

		WriterUtil.writeCommentForAutoGeneration(codeWriter);	

		codeWriter.beginIncludeGuard(Util.normalizeSymbol(this.getDestFileName()) + "_H");

		codeWriter.include("cpgf/gmetadefine.h");
		codeWriter.include("cpgf/metadata/gnamereplacer.h");
		codeWriter.include("cpgf/metadata/gmetadataconfig.h");
		codeWriter.include("cpgf/metadata/private/gmetadata_header.h");
		codeWriter.out("\n\n");
				
		codeWriter.beginNamespace(this.config.cppNamespace);

		List<CppClass>  sortedClassList = Util.sortClassList(this.classList);
		for(CppClass cppClass : sortedClassList) {
			if(! cppClass.canGenerateMetaCode()) {
				continue;
			}
			
			MetaClassWriter writer = new MetaClassWriter(this.config, codeWriter, cppClass);
			
			String funcName = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);

			this.beginMetaFunction(codeWriter, funcName);
			writer.write();
			this.endMetaFunction(codeWriter);
			
			codeWriter.out("\n\n");
		}

		codeWriter.endNamespace(this.config.cppNamespace);
		
		for(String ns : this.fileInfo.getNamespaceList()) {
			codeWriter.useNamespace(ns);
		}
		
		codeWriter.out("\n\n");
		codeWriter.include("cpgf/metadata/private/gmetadata_footer.h");
		
		codeWriter.endIncludeGuard();
		
		Util.forceCreateDirectories(this.config.headerOutput);
		String outFileName = this.makeOutputFileName(this.config.headerExtension);
		Util.trace("Writing header " + outFileName);
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
		codeWriter.include("cpgf/gmetapolicy.h");
		codeWriter.out("\n");
		
		codeWriter.useNamespace("cpgf");
		codeWriter.out("\n");

		codeWriter.beginNamespace(this.config.cppNamespace);
		
		List<CppClass>  sortedClassList = Util.sortClassList(this.classList);
		for(CppClass cppClass : sortedClassList) {
			if(! cppClass.canGenerateMetaCode()) {
				continue;
			}
			
			if(cppClass.isTemplate()) {
				continue;
			}

			String funcName = this.createFunctionName(cppClass, this.config.metaClassCreatePrefix);
			createFunctionNames.add(funcName);

			codeWriter.out("GDefineMetaInfo " + funcName + "()\n");
			
			codeWriter.beginBlock();

			WriterUtil.createMetaClass(this.config, codeWriter, cppClass, "_d", cppClass.getPolicyRules());
			
			String callFunc = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);
			codeWriter.out(callFunc + "(0, _d, NULL, GMetaPolicyCopyAllConstReference());\n");
			codeWriter.out("return _d.getMetaInfo();\n");
			
			codeWriter.endBlock();

			codeWriter.out("\n\n");
		}
		
		codeWriter.endNamespace(this.config.cppNamespace);
		
		Util.forceCreateDirectories(this.config.sourceOutput);
		String outFileName = Util.concatFileName(this.config.sourceOutput, this.getDestFileName()) + this.config.sourceExtension;
		Util.trace("Writing source " + outFileName);
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

	private void beginMetaFunction(CppWriter codeWriter, String name) {
		codeWriter.out("template <typename D, typename Policy>\n");
		codeWriter.out("void " + name + "(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)\n");
		codeWriter.beginBlock();
		codeWriter.out("(void)config; (void)_d; (void)_r; (void)_d; (void)_p;\n");
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
