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
import org.cpgf.metagen.metawriter.callback.OutputCallbackData;

public class ClassHeaderFileWriter extends CodeFileWriter {
	private List<CppClass> masterClassList;
	private String sourceFileName;
	private String targetFileName;
	private Map<CppClass, MetaClassCode> classCodeMap;

	public ClassHeaderFileWriter(Config config, MetaInfo metaInfo, FileInfo fileInfo, String sourceFileName, String targetFileName) {
		super(config, metaInfo, fileInfo);
		this.sourceFileName = sourceFileName;
		this.targetFileName = targetFileName;

		this.masterClassList = new ArrayList<CppClass>();
		
		this.classCodeMap = new HashMap<CppClass, MetaClassCode>();
	}

	public void addClass(CppClass cppClass) {
		this.masterClassList.add(cppClass);
	}
	
	@Override
	protected String getOutputDirectory() {
		return this.getConfig().headerOutput;
	}

	@Override
	protected String getOutputFileName() {
		return this.targetFileName + this.getConfig().headerExtension;
	}
	
	@Override
	protected void doWrite(CppWriter codeWriter) throws Exception {
		codeWriter.beginIncludeGuard(Util.normalizeSymbol(this.targetFileName) + "_H");

		if(this.getConfig().headerHeaderCode != null) {
			codeWriter.write(this.getConfig().headerHeaderCode);
			codeWriter.writeLine("");
		}
		codeWriter.include("cpgf/gmetadefine.h");
		codeWriter.include("cpgf/metadata/gmetadataconfig.h");
		codeWriter.include("cpgf/metadata/private/gmetadata_header.h");
		codeWriter.include("cpgf/gmetapolicy.h");
		if(this.getConfig().wrapCallback) {
			codeWriter.include("cpgf/scriptbind/gscriptbindapi.h");
		}
		if(this.shouldWrapClass(this.masterClassList)) {
			codeWriter.include("cpgf/scriptbind/gscriptbindutil.h");
			codeWriter.include("cpgf/scriptbind/gscriptwrapper.h");
			codeWriter.include("cpgf/gscopedinterface.h");
		}
		List<String> customizedHeaderIncludeList = new ArrayList<String>();
		for(CppClass cppClass : this.masterClassList) {
			OutputCallbackData callbackData = this.getMetaInfo().getCallbackClassMap().getData(cppClass);
			if(callbackData != null && callbackData.getHeaderIncludeList() != null) {
				for(String include : callbackData.getHeaderIncludeList()) {
					if(customizedHeaderIncludeList.indexOf(include) < 0) {
						customizedHeaderIncludeList.add(include);
					}
				}
			}
		}
		for(String include : customizedHeaderIncludeList) {
			codeWriter.include(include);
		}
		codeWriter.writeLine("");
		codeWriter.writeLine("");
				
		for(String ns : this.getFileInfo().getNamespaceList()) {
			codeWriter.useNamespace(ns);
		}
		
		codeWriter.writeLine("");
		codeWriter.writeLine("");

		codeWriter.beginNamespace(this.getConfig().cppNamespace);

		List<CppClass>  sortedClassList = Util.sortClassList(this.masterClassList);
		for(CppClass cppClass : sortedClassList) {
			MetaClassCode classCode = this.getClassCode(cppClass);
			if(classCode.headerCode.length() > 0) {
				codeWriter.write(this.getClassCode(cppClass).headerCode);
			
				codeWriter.writeLine("");
				codeWriter.writeLine("");
			}
		}

		codeWriter.endNamespace(this.getConfig().cppNamespace);
		
		codeWriter.writeLine("");
		codeWriter.writeLine("");
		codeWriter.include("cpgf/metadata/private/gmetadata_footer.h");
		
		codeWriter.endIncludeGuard();
	}

	private MetaClassCode getClassCode(CppClass cppClass) {
		MetaClassCode classCode = this.classCodeMap.get(cppClass);
		
		if(classCode == null) {
			classCode = (new MetaClassCodeGenerator(this.getConfig(), this.getMetaInfo(), cppClass, this.sourceFileName)).generateClassMetaCode();
			this.classCodeMap.put(cppClass, classCode);
		}
		
		return classCode;
	}
	
	private boolean shouldWrapClass(List<CppClass> classList) {
		for(CppClass cppClass : classList) {
			if(this.getMetaInfo().getCallbackClassMap().getData(cppClass).wrapClass()) {
				return true;
			}
		}
		return false;
	}
}
