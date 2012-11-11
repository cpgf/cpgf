package org.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.Util;
import org.cpgf.metagen.codewriter.CppWriter;
import org.cpgf.metagen.doxyxmlparser.FileInfo;
import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.MetaInfo;


public class MetaFileWriter {
	private FileInfo fileInfo;
	private Config config;
	private MetaInfo metaInfo;
	private List<CppClass> masterClassList;
	private String sourceFileName;
	private Map<CppClass, MetaClassCode> classCodeMap;

	public MetaFileWriter(Config config, MetaInfo metaInfo, String sourceFileName, FileInfo fileInfo) {
		this.config = config;
		this.metaInfo = metaInfo;
		this.sourceFileName = sourceFileName;
		this.fileInfo = fileInfo;

		this.masterClassList = new ArrayList<CppClass>();
		
		this.classCodeMap = new HashMap<CppClass, MetaClassCode>();
	}
	
	public void addClass(CppClass cppClass) {
		this.masterClassList.add(cppClass);
	}
	
	private boolean shouldSplitFile() {
		for(CppClass cppClass : this.masterClassList) {
			if(this.metaInfo.getCallbackClassMap().getData(cppClass).isInSeparatedFile()) {
				return true;
			}
		}
		return false;
	}
	
	private List<List<CppClass> > buildSpittedClassList() {
		List<List<CppClass> > result = new ArrayList<List<CppClass> >();
		result.add(new ArrayList<CppClass>());
		
		for(CppClass cppClass : this.masterClassList) {
			if(this.metaInfo.getCallbackClassMap().getData(cppClass).isInSeparatedFile()) {
				List<CppClass> classList = new ArrayList<CppClass>();
				classList.add(cppClass);
				result.add(classList);
			}
			else {
				result.get(0).add(cppClass);
			}
		}

		if(result.get(0).size() == 0) {
			result.remove(0);
		}

		return result;
	}

	private MetaClassCode getClassCode(CppClass cppClass) {
		MetaClassCode classCode = this.classCodeMap.get(cppClass);
		
		if(classCode == null) {
			classCode = (new MetaClassCodeGenerator(this.config, this.metaInfo, cppClass, this.sourceFileName)).generateClassMetaCode();
			this.classCodeMap.put(cppClass, classCode);
		}
		
		return classCode;
	}
	
	private boolean shouldWrapClass(List<CppClass> classList) {
		for(CppClass cppClass : classList) {
			if(this.metaInfo.getCallbackClassMap().getData(cppClass).wrapClass()) {
				return true;
			}
		}
		return false;
	}
	
	private void doWriteHeader(List<CppClass> classList, String outputFileName) throws Exception {
		CppWriter codeWriter = new CppWriter();

		WriterUtil.writeCommentForAutoGeneration(codeWriter);	

		codeWriter.beginIncludeGuard(Util.normalizeSymbol(outputFileName) + "_H");

		if(this.config.headerHeaderCode != null) {
			codeWriter.write(this.config.headerHeaderCode);
			codeWriter.writeLine("");
		}
		codeWriter.include("cpgf/gmetadefine.h");
		codeWriter.include("cpgf/metadata/gmetadataconfig.h");
		codeWriter.include("cpgf/metadata/private/gmetadata_header.h");
		codeWriter.include("cpgf/gmetapolicy.h");
		if(this.config.scriptable) {
			codeWriter.include("cpgf/scriptbind/gscriptbindapi.h");
		}
		if(this.shouldWrapClass(classList)) {
			codeWriter.include("cpgf/scriptbind/gscriptbindutil.h");
			codeWriter.include("cpgf/scriptbind/gscriptwrapper.h");
		}
		codeWriter.writeLine("");
		codeWriter.writeLine("");
				
		for(String ns : this.fileInfo.getNamespaceList()) {
			codeWriter.useNamespace(ns);
		}
		
		codeWriter.writeLine("");
		codeWriter.writeLine("");

		codeWriter.beginNamespace(this.config.cppNamespace);

		List<CppClass>  sortedClassList = Util.sortClassList(classList);
		for(CppClass cppClass : sortedClassList) {
			MetaClassCode classCode = this.getClassCode(cppClass);
			if(classCode.headerCode.length() > 0) {
				codeWriter.write(this.getClassCode(cppClass).headerCode);
			
				codeWriter.writeLine("");
				codeWriter.writeLine("");
			}
		}

		codeWriter.endNamespace(this.config.cppNamespace);
		
		codeWriter.writeLine("");
		codeWriter.writeLine("");
		codeWriter.include("cpgf/metadata/private/gmetadata_footer.h");
		
		codeWriter.endIncludeGuard();
		
		Util.forceCreateDirectories(this.config.headerOutput);
		String outFileName = Util.concatFileName(this.config.headerOutput, outputFileName) + this.config.headerExtension;
		Util.writeTextToFile(outFileName, codeWriter.getText());
	}

	public void writeHeader() throws Exception {
		if(this.shouldSplitFile()) {
			List<List<CppClass> > splittedClassList = this.buildSpittedClassList();
			for(int i = 0; i < splittedClassList.size(); ++i) {
				this.doWriteHeader(splittedClassList.get(i), this.getDestFileName(i));
			}
		}
		else {
			this.doWriteHeader(this.masterClassList, this.getDestFileName());
		}
	}

	private void doWriteSource(List<String> createFunctionNames, List<CppClass> classList, String outputFileName) throws Exception { 
		if(! this.config.autoRegisterToGlobal) {
			return;
		}
		
		CppWriter codeWriter = new CppWriter();

		WriterUtil.writeCommentForAutoGeneration(codeWriter);	

		if(this.config.sourceHeaderCode != null) {
			codeWriter.write(this.config.sourceHeaderCode);
			codeWriter.writeLine("");
		}
		if(this.config.sourceHeaderReplacer != null) {
			String fileName = this.sourceFileName;
			fileName = fileName.replaceAll("\\\\", "/");
			fileName = Util.replaceStringWithArray(fileName, this.config.sourceHeaderReplacer);
			codeWriter.include(fileName);
			codeWriter.writeLine("");
		}
		else {
			codeWriter.include(this.sourceFileName);
		}
		codeWriter.include(this.config.metaHeaderPath + outputFileName + ".h");
		codeWriter.writeLine("");
		
		codeWriter.useNamespace("cpgf");
		codeWriter.writeLine("");

		codeWriter.beginNamespace(this.config.cppNamespace);
		
		List<CppClass>  sortedClassList = Util.sortClassList(classList);
		for(CppClass cppClass : sortedClassList) {
			MetaClassCode classCode = this.getClassCode(cppClass);
			if(classCode.sourceCode.length() > 0) {
				codeWriter.write(classCode.sourceCode);
				
				createFunctionNames.add(classCode.createFunctionName);

				codeWriter.writeLine("");
				codeWriter.writeLine("");
			}
		}
		
		codeWriter.endNamespace(this.config.cppNamespace);
		
		Util.forceCreateDirectories(this.config.sourceOutput);
		String outFileName = Util.concatFileName(this.config.sourceOutput, outputFileName) + this.config.sourceExtension;
		Util.writeTextToFile(outFileName, codeWriter.getText());
	}

	public void writeSource(List<String> createFunctionNames) throws Exception {
		if(this.shouldSplitFile()) {
			List<List<CppClass> > splittedClassList = this.buildSpittedClassList();
			for(int i = 0; i < splittedClassList.size(); ++i) {
				this.doWriteSource(createFunctionNames, splittedClassList.get(i), this.getDestFileName(i));
			}
		}
		else {
			this.doWriteSource(createFunctionNames, this.masterClassList, this.getDestFileName());
		}
	}

	private String getDestFileName()
	{
		return this.config.sourceFilePrefix + Util.getBaseFileName(this.sourceFileName);
	}

	private String getDestFileName(int index)
	{
		if(index == 0) {
			return this.getDestFileName();
		}
		else {
			return this.getDestFileName() + index;
		}
	}

}
