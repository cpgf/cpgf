package com.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.List;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.doxyxmlparser.FileInfo;
import com.cpgf.metagen.metadata.CppClass;

public class MetaFileWriter {
	private FileInfo fileInfo;
	private Config config;
	private List<CppClass> classList;
	private String sourceFileName;
	
	public MetaFileWriter(String sourceFileName, FileInfo fileInfo, Config config) {
		this.sourceFileName = sourceFileName;
		this.fileInfo = fileInfo;
		this.config = config;

		this.classList = new ArrayList<CppClass>();
	}
	
	public void addClass(CppClass cppClass) {
		this.classList.add(cppClass);
	}

	public void writeHeader() throws Exception {
		CppWriter cw = new CppWriter();

		Util.writeAutoComment(cw);	

		cw.beginIncludeGuard(Util.normalizeSymbol(this.getDestFileName()) + "_H");

		cw.include("cpgf/gmetadefine.h");
		cw.include("cpgf/metadata/gnamereplacer.h");
		cw.include("cpgf/metadata/gmetadataconfig.h");
		cw.include("cpgf/metadata/private/gmetadata_header.h");
		cw.out("\n\n");
				
		cw.beginNamespace(this.config.cppNamespace);

		List<CppClass>  sortedClassList = Util.sortClassList(this.classList);
		for(CppClass cppClass : sortedClassList) {
			MetaClassWriter writer = new MetaClassWriter(
				cppClass,
				cw,
				this.config
			);
			
			String funcName = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);

			this.beginMetaFunction(cw, funcName);
			writer.write();
			this.endMetaFunction(cw);
			
			cw.out("\n\n");
		}

		cw.endNamespace(this.config.cppNamespace);
		
		for(String ns : this.fileInfo.getNamespaceList()) {
			cw.useNamespace(ns);
		}
		
		cw.out("\n\n");
		cw.include("cpgf/metadata/private/gmetadata_footer.h");
		
		cw.endIncludeGuard();
		
		Util.forceCreateDirectories(this.config.headerOutput);
		String outFileName = this.makeOutputFileName(this.config.headerExtension);
		Util.trace("Writing header " + outFileName);
		Util.writeTextToFile(outFileName, cw.getText());
	}

	public void writeSource(List<String> createFunctionNames) throws Exception { 
		if(! this.config.autoRegisterToGlobal) {
			return;
		}
		
		CppWriter cw = new CppWriter();

		Util.writeAutoComment(cw);	

		if(this.config.sourceHeaderCode != null) {
			cw.out(this.config.sourceHeaderCode);
			cw.out("\n");
		}
		if(this.config.sourceHeaderReplacer != null) {
			String fileName = this.sourceFileName;
			fileName.replaceAll("\\\\", "/");
			fileName = this.config.sourceHeaderReplacer.replaceSourceHeader(fileName);
			cw.include(fileName);
			cw.out("\n");
		}
		else {
			cw.include(this.sourceFileName);
		}
		cw.include(this.config.metaHeaderPath + this.getDestFileName() + ".h");
		cw.out("\n");
		cw.include("cpgf/gmetapolicy.h");
		cw.out("\n");
		
		cw.useNamespace("cpgf");
		cw.out("\n");

		cw.beginNamespace(this.config.cppNamespace);
		
		List<CppClass>  sortedClassList = Util.sortClassList(this.classList);
		for(CppClass cppClass : sortedClassList) {
			if(cppClass.isTemplate()) {
				continue;
			}

			String funcName = this.createFunctionName(cppClass, this.config.metaClassCreatePrefix);
			createFunctionNames.add(funcName);

			cw.out("GDefineMetaInfo " + funcName + "()\n");
			
			cw.beginBlock();
			
			Util.createMetaClass(this.config, cw, cppClass, "_d", cppClass.getPolicyRules());
			
			String callFunc = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);
			cw.out(callFunc + "(0, _d, NULL, GMetaPolicyCopyAllConstReference());\n");
//			cw.out(callFunc + "(0, _d, NULL, GMetaPolicyDefault());\n");
			cw.out("return _d.getMetaInfo();\n");
			
			cw.endBlock();

			cw.out("\n\n");
		}
		
		cw.endNamespace(this.config.cppNamespace);
		
		Util.forceCreateDirectories(this.config.sourceOutput);
		String outFileName = Util.concatFileName(this.config.sourceOutput, this.getDestFileName()) + this.config.sourceExtension;
		Util.trace("Writing source " + outFileName);
		Util.writeTextToFile(outFileName, cw.getText());
	}

	private String createFunctionName(CppClass cppClass, String prefix) {
		String className = this.getGlobalPostfix();
		if(! cppClass.isGlobal()) {
			className = cppClass.getName();
		}
		className = Util.getBaseName(className);
		className = Util.upcaseFirst(className);
			
		return prefix + className;
	}

	private void beginMetaFunction(CppWriter cw, String name) {
		cw.out("template <typename D, typename Policy>\n");
		cw.out("void " + name + "(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)\n");
		cw.beginBlock();
		cw.out("(void)config; (void)_d; (void)_r; (void)_d; (void)_p;\n");
		cw.useNamespace("cpgf");
		cw.out("\n");
	}

	private void endMetaFunction(CppWriter cw) {
		cw.endBlock();
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
