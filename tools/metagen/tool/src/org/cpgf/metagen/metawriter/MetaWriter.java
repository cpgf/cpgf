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

		for(CppClass item : this.classList) {
			String location = item.getLocation();
			String key = location.toLowerCase();

			FileInfo fileInfo = this.fileMap.getFileMap().get(location);
			if(fileInfo == null) {
				continue;
			}

			if(this.metaInfo.getCallbackClassMap().getData(item).isInSeparatedFile()) {
				ClassSourceFileWriter sourceFileWriter = new ClassSourceFileWriter(this.config, this.metaInfo, fileInfo, location, this.makeTargetFileName(location, item));
				sourceFileWriter.addClass(item);
				this.fileWriterList.add(sourceFileWriter);

				ClassHeaderFileWriter headerFileWriter = new ClassHeaderFileWriter(this.config, this.metaInfo, fileInfo, location, this.makeTargetFileName(location, item));
				headerFileWriter.addClass(item);
				this.fileWriterList.add(headerFileWriter);
			}
			else {
				if(! locationSourceFileWriterMap.containsKey(key)) {
					ClassSourceFileWriter sourceFileWriter = new ClassSourceFileWriter(this.config, this.metaInfo, fileInfo, location, this.makeTargetFileName(location));
					locationSourceFileWriterMap.put(key, sourceFileWriter);
					this.fileWriterList.add(sourceFileWriter);
				}
				locationSourceFileWriterMap.get(key).addClass(item);
				
				if(! locationHeaderFileWriterMap.containsKey(key)) {
					ClassHeaderFileWriter headerFileWriter = new ClassHeaderFileWriter(this.config, this.metaInfo, fileInfo, location, this.makeTargetFileName(location));
					locationHeaderFileWriterMap.put(key, headerFileWriter);
					this.fileWriterList.add(headerFileWriter);
				}
				locationHeaderFileWriterMap.get(key).addClass(item);
			}
		}
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
		return this.config.sourceFilePrefix + Util.getBaseFileName(sourceFileName);
	}

	private String makeTargetFileName(String sourceFileName, CppClass cppClass)
	{
		String suffix = "_";
		if(cppClass.isGlobal()) {
			suffix = suffix + "global";
		}
		else {
			suffix = suffix + cppClass.getPrimaryName();
		}
		return this.config.sourceFilePrefix + Util.getBaseFileName(sourceFileName) + suffix;
	}
}
