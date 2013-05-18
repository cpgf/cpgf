package org.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.List;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.Util;
import org.cpgf.metagen.codewriter.CppWriter;
import org.cpgf.metagen.doxyxmlparser.FileInfo;
import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.MetaInfo;
import org.cpgf.metagen.metadata.TemplateInstance;

public class TemplateInstanceSourceFileWriter extends CodeFileWriter {
	private TemplateInstance templateInstance;
	private String sourceFileName;
	private String targetFileName;

	public TemplateInstanceSourceFileWriter(Config config, MetaInfo metaInfo, FileInfo fileInfo, String sourceFileName, TemplateInstance templateInstance) {
		super(config, metaInfo, fileInfo);
		this.sourceFileName = sourceFileName;

		this.templateInstance = templateInstance;
		this.targetFileName = this.getConfig().sourceFilePrefix + Util.getBaseFileName(this.sourceFileName) + "_" + WriterUtil.normalizeClassName(this.templateInstance.getMapName());
	}
	
	private String createFunctionName() {
		String funcName = this.getConfig().metaClassCreatePrefix + Util.upcaseFirst(this.templateInstance.getTemplateClass().getPrimaryName());
		funcName = funcName + "_TemplateInstance_" + WriterUtil.normalizeClassName(this.templateInstance.getMapName());
		return funcName;
	}

	@Override
	public boolean shouldSkip() {
		return ! this.getConfig().generateRegisterHeader;
	}
	
	@Override
	protected void doGetCreationFunctionNames(List<String> creationFunctionNames) {
		creationFunctionNames.add(this.createFunctionName());
	}
	
	@Override
	protected String getOutputDirectory() {
		return this.getConfig().sourceOutput;
	}

	@Override
	protected String getOutputFileName() {
		return this.targetFileName + this.getConfig().sourceExtension;
	}
	
	@Override
	protected void doWrite(CppWriter codeWriter) throws Exception {
		if(this.getConfig().sourceHeaderCode != null) {
			codeWriter.write(this.getConfig().sourceHeaderCode);
			codeWriter.writeLine("");
		}

		codeWriter.include(WriterUtil.formatSourceIncludeHeader(this.getConfig(), this.sourceFileName));
		codeWriter.writeLine("");
		
		codeWriter.include(this.getConfig().metaHeaderPath + this.getConfig().sourceFilePrefix + Util.getBaseFileName(this.sourceFileName) + ".h");
		codeWriter.writeLine("");
		
		codeWriter.useNamespace("cpgf");
		codeWriter.writeLine("");
		
		CppClass cppClass = this.templateInstance.getTemplateClass();

		codeWriter.beginNamespace(this.getConfig().cppNamespace);
		
		String funcName = this.createFunctionName();

		codeWriter.writeLine("GDefineMetaInfo " + funcName + "()");

		codeWriter.beginBlock();

		String callFunc = WriterUtil.createFunctionName(cppClass.getPrimaryName(), this.sourceFileName, cppClass.isGlobal(), this.getConfig().metaClassFunctionPrefix);

		List<TemplateInstance> templateInstanceList = new ArrayList<TemplateInstance>();
		templateInstanceList.add(this.templateInstance);
		WriterUtil.createMetaClass(codeWriter, cppClass, callFunc, templateInstanceList);

		codeWriter.writeLine("return _d.getMetaInfo();");
		
		codeWriter.endBlock();
		
		codeWriter.endNamespace(this.getConfig().cppNamespace);
	}

}
