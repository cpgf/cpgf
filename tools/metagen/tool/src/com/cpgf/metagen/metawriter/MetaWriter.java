package com.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.doxyxmlparser.FileMap;
import com.cpgf.metagen.metadata.CppClass;

public class MetaWriter {
	private List<MetaFileWriter> fileWriterList;
	private List<CppClass> classList;
	private Config config;
	private FileMap fileMap;
	
	public MetaWriter(List<CppClass> classList, FileMap fileMap, Config config) {
		this.classList = classList;
		this.fileMap = fileMap;
		this.config = config;

		this.fileWriterList = new ArrayList<MetaFileWriter>();
	}

	public void write() throws Exception
	{
		List<String> createFunctionNames = new ArrayList<String>();

		Util.trace("Building file information.");

		this.buildFileWriterList();

		Util.trace("Writing files.");

		for(MetaFileWriter fileWriter : this.fileWriterList) {
			fileWriter.writeHeader();
			fileWriter.writeSource(createFunctionNames);
		}

		this.createMainHeader(createFunctionNames);
		this.createMainSource(createFunctionNames);
	}

	private String getMainFileName() {
		return this.config.mainSourceFile;
	}

	private String getMainFunctionName() {
		return this.config.metaClassMainRegisterPrefix + this.config.projectID;
	}

	private void createMainHeader(List<String> createFunctionNames) throws Exception {
		if(! this.config.autoRegisterToGlobal) {
			return;
		}
		
		String outFileName = Util.concatFileName(this.config.headerOutput, this.getMainFileName()) + ".h";
		CppWriter cw = new CppWriter();

		Util.writeAutoComment(cw);	

		cw.beginIncludeGuard(Util.normalizeSymbol(this.getMainFileName()) + "_H");

		cw.include("cpgf/gmetadefine.h");

		cw.out("\n\n");
		
		cw.useNamespace("cpgf");
		cw.out("\n");

		cw.beginNamespace(this.config.cppNamespace);
		
		List<String> sortedCreateFunctionNames = Util.sortStringList(createFunctionNames);

		for(String funcName : sortedCreateFunctionNames) {
			cw.out("GDefineMetaInfo " + funcName + "();\n");
		}
		
		cw.out("\n\n");

		cw.out("template <typename Meta>\n");
		cw.out("void " + this.getMainFunctionName() + "(Meta _d)\n");

		cw.beginBlock();

		cw.out("_d\n");

		cw.incIndent();

		for(String funcName : sortedCreateFunctionNames) {
			cw.out("._class(" + funcName + "())\n");
		}
		cw.decIndent();
		cw.out(";\n");

		cw.endBlock();
		
		cw.out("\n");
		
		cw.endNamespace(this.config.cppNamespace);

		cw.endIncludeGuard();	

		Util.writeTextToFile(outFileName, cw.getText());
	}

	private void createMainSource(List<String> createFunctionNames) throws Exception {
		if(! this.config.autoRegisterToGlobal) {
			return;
		}
		
		String outFileName = Util.concatFileName(this.config.sourceOutput, this.getMainFileName()) + ".cpp";
		CppWriter cw = new CppWriter();

		Util.writeAutoComment(cw);	

		cw.include(this.config.metaHeaderPath + this.getMainFileName() + ".h");
		cw.include("cpgf/gmetadefine.h");
		cw.include("cpgf/goutmain.h");

		cw.out("\n\n");
		
		cw.useNamespace("cpgf");
		cw.out("\n");

		cw.beginNamespace(this.config.cppNamespace);
		
		cw.beginNamespace("");

		cw.out("G_AUTO_RUN_BEFORE_MAIN()\n");

		cw.beginBlock();

		CppClass global = new CppClass(null);
		Util.defineMetaClass(this.config, cw, global, "_d", "define");

		cw.out(this.getMainFunctionName() + "(_d);\n");

		cw.endBlock();
		cw.out("\n");

		cw.endNamespace("");
		
		cw.endNamespace(this.config.cppNamespace);

		Util.writeTextToFile(outFileName, cw.getText());
	}

	private void buildFileWriterList()
	{
		HashMap<String, MetaFileWriter> fm = new HashMap<String, MetaFileWriter>();

		for(CppClass item : this.classList) {
			String location = item.getLocation();
			String key = location.toLowerCase();

			if(! fm.containsKey(key)) {
				MetaFileWriter fileWriter = new MetaFileWriter(
					location,
					this.fileMap.getFileMap().get(location),
					this.config
				);
				fm.put(key, fileWriter);
				this.fileWriterList.add(fileWriter);
			}
			fm.get(key).addClass(item);
		}
	}


}
