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

public class ClassSourceFileWriter extends CodeFileWriter {
	private List<CppClass> masterClassList;
	private String sourceFileName;
	private String fileNameSuffix;
	private Map<CppClass, MetaClassCode> classCodeMap;

	public ClassSourceFileWriter(Config config, MetaInfo metaInfo, FileInfo fileInfo, String sourceFileName, String fileNameSuffix) {
		super(config, metaInfo, fileInfo);
		this.sourceFileName = sourceFileName;
		this.fileNameSuffix = fileNameSuffix;

		this.masterClassList = new ArrayList<CppClass>();
		
		this.classCodeMap = new HashMap<CppClass, MetaClassCode>();
	}

	public void addClass(CppClass cppClass) {
		this.masterClassList.add(cppClass);
	}
	
	@Override
	public boolean shouldSkip() {
		return ! this.getConfig().autoRegisterToGlobal;
	}
	
	@Override
	public void getCreationFunctionNames(List<String> creationFunctionNames) {
		List<CppClass>  sortedClassList = Util.sortClassList(this.masterClassList);
		for(CppClass cppClass : sortedClassList) {
			MetaClassCode classCode = this.getClassCode(cppClass);
			if(classCode.sourceCode.length() > 0) {
				creationFunctionNames.add(classCode.createFunctionName);
			}
		}
	}
	
	@Override
	protected String getOutputDirectory() {
		return this.getConfig().sourceOutput;
	}

	@Override
	protected String getOutputFileName() {
		return this.getDestFileName() + this.getConfig().sourceExtension;
	}
	
	@Override
	protected void doWrite(CppWriter codeWriter) throws Exception {
		if(this.getConfig().sourceHeaderCode != null) {
			codeWriter.write(this.getConfig().sourceHeaderCode);
			codeWriter.writeLine("");
		}
		if(this.getConfig().sourceHeaderReplacer != null) {
			String fileName = this.sourceFileName;
			fileName = fileName.replaceAll("\\\\", "/");
			fileName = Util.replaceStringWithArray(fileName, this.getConfig().sourceHeaderReplacer);
			codeWriter.include(fileName);
			codeWriter.writeLine("");
		}
		else {
			codeWriter.include(this.sourceFileName);
		}
		codeWriter.include(this.getConfig().metaHeaderPath + this.getDestFileName() + ".h");
		codeWriter.writeLine("");
		
		codeWriter.useNamespace("cpgf");
		codeWriter.writeLine("");

		codeWriter.beginNamespace(this.getConfig().cppNamespace);
		
		List<CppClass>  sortedClassList = Util.sortClassList(this.masterClassList);
		for(CppClass cppClass : sortedClassList) {
			MetaClassCode classCode = this.getClassCode(cppClass);
			if(classCode.sourceCode.length() > 0) {
				codeWriter.write(classCode.sourceCode);
				
				codeWriter.writeLine("");
				codeWriter.writeLine("");
			}
		}
		
		codeWriter.endNamespace(this.getConfig().cppNamespace);
	}

	private MetaClassCode getClassCode(CppClass cppClass) {
		MetaClassCode classCode = this.classCodeMap.get(cppClass);
		
		if(classCode == null) {
			classCode = (new MetaClassCodeGenerator(this.getConfig(), this.getMetaInfo(), cppClass, this.sourceFileName)).generateClassMetaCode();
			this.classCodeMap.put(cppClass, classCode);
		}
		
		return classCode;
	}
	
	private String getDestFileName()
	{
		return this.fileNameSuffix;
	}
}
