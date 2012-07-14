package com.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.List;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.codewriter.CppWriter;
import com.cpgf.metagen.metadata.ClassWrapperConfig;
import com.cpgf.metagen.metadata.CppClass;
import com.cpgf.metagen.metadata.CppMethod;

public class ClassWrapperWriter {
	private Config config;
	private ClassWrapperConfig wrapperConfig;
	private CppWriter codeWriter;
	private CppClass cppClass;
	private List<CppMethod> overrideMethodList;

	public ClassWrapperWriter(Config config, ClassWrapperConfig wrapperConfig, CppWriter codeWriter, CppClass cppClass) {
		this.config = config;
		this.cppClass = cppClass;
		this.codeWriter = codeWriter;
		this.wrapperConfig = wrapperConfig;
		
		this.buidOverrideMethodList();
	}

	private String getWrapperName() {
		return this.wrapperConfig.makeWrapName(this.config, this.cppClass);
	}
	
	private String getGuardName() {
		return "_cpgf_override_method_is_invoking_script";
	}
	
	private void buidOverrideMethodList() {
		this.overrideMethodList = new ArrayList<CppMethod>();
		for(CppMethod cppMethod : this.cppClass.getMethodList()) {
			if(cppMethod.isVirtual()) {
				this.overrideMethodList.add(cppMethod);
			}
		}
	}

	public void setCodeWriter(CppWriter codeWriter) {
		this.codeWriter = codeWriter;
	}

	public void writeClassWrapper() {
		this.codeWriter.write("class " + this.getWrapperName() + " : public " + this.cppClass.getLiteralName() + ", public cpgf::GScriptWrapper ");
		this.codeWriter.writeLine("{");
		this.codeWriter.writeLine("public:");
		this.codeWriter.incIndent();

		this.codeWriter.writeLine("static bool " + this.getGuardName() + "[" + this.overrideMethodList.size() + "];");

		for(int i = 0; i < this.overrideMethodList.size(); ++i) {
			this.codeWriter.writeLine("");
			CppMethod cppMethod = this.overrideMethodList.get(i);
			this.codeWriter.writeLine(Util.getInvokablePrototype(cppMethod, cppMethod.getLiteralName()));
			this.codeWriter.beginBlock();
				String paramText = Util.getParameterText(cppMethod.getParameterList(), false, true);
				String sentinel = this.getGuardName() + "[" + i + "]";
				this.codeWriter.writeLine("if(" + sentinel + ")");
				this.codeWriter.beginBlock();
					this.codeWriter.writeLine("cpgf::GScriptWrapperReentryGuard guard(&" + sentinel + ");");
					this.codeWriter.writeLine("cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction(\"" + cppMethod.getLiteralName() + "\"));");
					this.codeWriter.writeLine("if(func)");
					this.codeWriter.beginBlock();
						String invoke = "cpgf::invokeScriptFunction(func.get(), this";
						if(cppMethod.hasParameter()) {
							invoke = invoke + ", " + paramText;
						}
						invoke = invoke + ")";
						if(cppMethod.hasResult()) {
							invoke = "return cpgf::fromVariant<" + cppMethod.getResultType().getLiteralType() + " >(" + invoke + ".getValue())";
						}
						invoke = invoke + ";";
						this.codeWriter.writeLine(invoke);
						if(! cppMethod.hasResult()) {
							this.codeWriter.writeLine("return;");
						}
					this.codeWriter.endBlock();
				this.codeWriter.endBlock();
				if(cppMethod.isPureVirtual()) {
					this.codeWriter.writeLine("throw \"Abstract method\";");
				}
				else {
					invoke = this.cppClass.getLiteralName() + "::" + cppMethod.getLiteralName() + "(" + paramText + ");";
					if(cppMethod.hasResult()) {
						invoke = "return " + invoke;
					}
					this.codeWriter.writeLine(invoke);
				}
			this.codeWriter.endBlock("");
		}
		this.codeWriter.decIndent();
		this.codeWriter.writeLine("};");
	}
	
	public void writeCreation(String callFunc) {
		List<String> rules = new ArrayList<String>();
		this.cppClass.getPolicyRules(rules);

		String policy = "";
		if(rules != null && rules.size() > 0) {
			policy = "::Policy<MakePolicy<" + Util.joinStringList(", ", rules) + "> >";
		}
		
		String typeName = "GDefineMetaClass<" + this.getWrapperName() + ", " + this.cppClass.getLiteralName() + ">";

		this.codeWriter.write(typeName +  " _nd = " + typeName + policy + "::declare(\"" + this.getWrapperName() + "\");\n");
		
		this.codeWriter.write(callFunc + "(0, _nd, NULL);\n");
		this.codeWriter.write("_d._class(_nd);\n");
	}

	public void writeStaticInitializer(CppWriter codeWriter) {
		codeWriter.writeLine("bool " + this.getWrapperName() + "::" + this.getGuardName() + "[" + this.overrideMethodList.size() + "] = { false };");
	}
	
}
