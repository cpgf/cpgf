package org.cpgf.metagen;

import java.util.ArrayList;
import java.util.List;

import org.cpgf.metagen.doxyxmlparser.DoxygenXmlParser;
import org.cpgf.metagen.doxyxmlparser.FileMap;
import org.cpgf.metagen.metadata.MetaInfo;
import org.cpgf.metagen.metawriter.MetaWriter;


public class MetagenMain {
	private List<String> xmlFileNameList;
	private List<String> configFileNameList;

	private void usage() {
		System.out.println("Usage:");
		System.out.println("  megagen --xml Xml1 [Xml2 Xml3...] --config Config1 [Config2 Config3...]");
		System.exit(1);
	}

	private void error(String message) {
		System.out.println(message);
		System.exit(1);
	}

	private void parseCommandLine(String[] args) throws Exception {
		this.xmlFileNameList = new ArrayList<String>();
		this.configFileNameList = new ArrayList<String>();
		
		if(args.length == 0) {
			this.usage();
		}

		List<String> currentList = null;
		for(String arg : args) {
			if(arg.indexOf('-') == 0) {
				if(arg.equals("--xml") || arg.equals("-xml")) {
					currentList = this.xmlFileNameList;
				}
				else if(arg.equals("--config") || arg.equals("-config")) {
					currentList = this.configFileNameList;
				}
			}
			else {
				if(currentList == null) {
					error("Need option before " + arg);
				}
				else {
					currentList.add(arg);
				}
			}
		}
		
		if(this.xmlFileNameList.size() == 0) {
			error("No XML file is specified.");
		}

		if(this.configFileNameList.size() == 0) {
			error("No config file is specified.");
		}
	}

	public void run(String[] args) throws Exception {
		this.parseCommandLine(args);

		Config config = new Config();

		List<JavascriptConfigLoader> configLoaderList = new ArrayList<JavascriptConfigLoader>();
		try {
			for(String configFileName : this.configFileNameList) {
				JavascriptConfigLoader configLoader = new JavascriptConfigLoader(config);
				configLoaderList.add(configLoader);
				configLoader.load(configFileName);
			}

			MetaInfo metaInfo = new MetaInfo(config);
			FileMap fileMap = new FileMap();

			for(String xmlFileName : this.xmlFileNameList) {
				(new DoxygenXmlParser(config, metaInfo, fileMap, xmlFileName)).parseFile();
			}

			metaInfo.fixup(fileMap);

			MetaWriter metaWriter = new MetaWriter(config, metaInfo, fileMap);
			metaWriter.write();

			RunStats.report();
		}
		finally {
			for(JavascriptConfigLoader configLoader : configLoaderList) {
				configLoader.free();
			}
		}
	}
	
}
