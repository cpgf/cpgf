package org.cpgf.metagen.metawriter;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.codewriter.CppWriter;
import org.cpgf.metagen.metadata.CppClass;

public class MainSourceFileWriter extends CodeFileWriter {
	public MainSourceFileWriter(Config config) {
		super(config, null, null);
	}

	@Override
	public boolean shouldSkip() {
		return ! this.getConfig().autoRegisterToGlobal;
	}
	
	@Override
	protected String getOutputDirectory() {
		return this.getConfig().sourceOutput;
	}

	@Override
	protected String getOutputFileName() {
		return this.getConfig().mainSourceFile + this.getConfig().sourceExtension;
	}
	
	@Override
	protected void doWrite(CppWriter codeWriter) throws Exception {
		codeWriter.include(this.getConfig().metaHeaderPath + this.getConfig().mainSourceFile + ".h");
		codeWriter.include("cpgf/gmetadefine.h");
		codeWriter.include("cpgf/goutmain.h");

		codeWriter.writeLine("");
		codeWriter.writeLine("");
		
		codeWriter.useNamespace("cpgf");
		codeWriter.writeLine("");

		codeWriter.beginNamespace(this.getConfig().cppNamespace);
		
		codeWriter.beginNamespace("");

		codeWriter.writeLine("G_AUTO_RUN_BEFORE_MAIN()");

		codeWriter.beginBlock();

		CppClass global = new CppClass(null);
		WriterUtil.defineMetaClass(this.getConfig(), codeWriter, global, "_d", "define");

		codeWriter.writeLine(this.getMainFunctionName() + "(_d);");

		codeWriter.endBlock();
		codeWriter.writeLine("");

		codeWriter.endNamespace("");
		
		codeWriter.endNamespace(this.getConfig().cppNamespace);
	}

	private String getMainFunctionName() {
		return this.getConfig().metaClassMainRegisterPrefix + this.getConfig().projectID;
	}

}
