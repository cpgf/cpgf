package org.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.Util;
import org.cpgf.metagen.doxyxmlparser.FileInfo;
import org.cpgf.metagen.doxyxmlparser.FileMap;
import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.MetaInfo;
import org.cpgf.metagen.metadata.TemplateInstance;


public class MetaWriter {
	private List<CodeFileWriter> fileWriterList;
	private MetaInfo metaInfo;
	private List<CppClass> classList;
	private Config config;
	private FileMap fileMap;

	public MetaWriter(Config config, MetaInfo metaInfo, FileMap fileMap) {
		this.metaInfo = metaInfo;
		this.classList = this.metaInfo.getClassList();
		this.fileMap = fileMap;
		this.config = config;

		this.fileWriterList = new ArrayList<CodeFileWriter>();
	}

	public void write() throws Exception
	{
		Util.trace("Building file information.");

		this.buildFileWriterList();
		this.buildMainFileWriterList();

		Util.trace("Writing files.");

		for(CodeFileWriter fileWriter : this.fileWriterList) {
			fileWriter.write();
		}
	}

	private void buildFileWriterList()
	{
		HashMap<String, ClassSourceFileWriter> locationSourceFileWriterMap = new HashMap<String, ClassSourceFileWriter>();
		HashMap<String, ClassHeaderFileWriter> locationHeaderFileWriterMap = new HashMap<String, ClassHeaderFileWriter>();

		for(CppClass cppClass : this.classList) {
			String location = cppClass.getLocation();
			String key = location.toLowerCase();

			FileInfo fileInfo = this.fileMap.getFileMap().get(location);
			if(fileInfo == null) {
				continue;
			}
			
			if(cppClass.isTemplate()) {
				List<TemplateInstance> templateInstanceList = this.metaInfo.findTemplateInstances(cppClass);
				if(templateInstanceList != null) {
					for(TemplateInstance templateInstance : templateInstanceList) {
						TemplateInstanceSourceFileWriter templateInstanceSourceFileWriter = new TemplateInstanceSourceFileWriter(this.config, this.metaInfo, fileInfo, location, templateInstance); 
						this.fileWriterList.add(templateInstanceSourceFileWriter);
					}
				}
			}
			
			if(this.metaInfo.getCallbackClassMap().getData(cppClass).isInSeparatedFile()) {
				ClassSourceFileWriter sourceFileWriter = this.createAndAddSourceFileWriter(fileInfo, location, cppClass);
				sourceFileWriter.addClass(cppClass);

				ClassHeaderFileWriter headerFileWriter = this.createAndAddHeaderFileWriter(fileInfo, location, cppClass);
				headerFileWriter.addClass(cppClass);
			}
			else {
				if(! locationSourceFileWriterMap.containsKey(key)) {
					ClassSourceFileWriter sourceFileWriter = this.createAndAddSourceFileWriter(fileInfo, location, null);
					locationSourceFileWriterMap.put(key, sourceFileWriter);
				}
				locationSourceFileWriterMap.get(key).addClass(cppClass);
				
				if(! locationHeaderFileWriterMap.containsKey(key)) {
					ClassHeaderFileWriter headerFileWriter = this.createAndAddHeaderFileWriter(fileInfo, location, null);
					locationHeaderFileWriterMap.put(key, headerFileWriter);
				}
				locationHeaderFileWriterMap.get(key).addClass(cppClass);
			}
		}
	}
	
	private ClassSourceFileWriter createAndAddSourceFileWriter(FileInfo fileInfo, String location, CppClass cppClass) {
		ClassSourceFileWriter sourceFileWriter = new ClassSourceFileWriter(this.config, this.metaInfo, fileInfo, location, this.makeTargetFileName(location, cppClass));
		this.fileWriterList.add(sourceFileWriter);
		
		return sourceFileWriter; 
	}
	
	private ClassHeaderFileWriter createAndAddHeaderFileWriter(FileInfo fileInfo, String location, CppClass cppClass) {
		ClassHeaderFileWriter headerFileWriter = new ClassHeaderFileWriter(this.config, this.metaInfo, fileInfo, location, this.makeTargetFileName(location, cppClass));
		this.fileWriterList.add(headerFileWriter);
		
		return headerFileWriter;
	}
	
	private void buildMainFileWriterList()
	{
		List<String> creationFunctionNames = new ArrayList<String>();

		for(CodeFileWriter fileWriter : this.fileWriterList) {
			fileWriter.getCreationFunctionNames(creationFunctionNames);
		}
		
		this.fileWriterList.add(new MainHeaderFileWriter(this.config, creationFunctionNames));
		this.fileWriterList.add(new MainSourceFileWriter(this.config));
	}

	private String makeTargetFileName(String sourceFileName)
	{
		return Util.makeTargetFileName(this.config, sourceFileName);
	}

	private String makeTargetFileName(String sourceFileName, CppClass cppClass)
	{
        return Util.makeTargetFileName(this.config, sourceFileName, cppClass);
	}
}
