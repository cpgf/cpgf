package com.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.List;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.codewriter.CppWriter;
import com.cpgf.metagen.metadata.ClassWrapperConfig;
import com.cpgf.metagen.metadata.Constructor;
import com.cpgf.metagen.metadata.CppClass;
import com.cpgf.metagen.metadata.CppMethod;

public class ClassWrapperWriter {
	private Config config;
	private ClassWrapperConfig wrapperConfig;
	private CppClass cppClass;
	private List<CppMethod> overrideMethodList;

	public ClassWrapperWriter(Config config, ClassWrapperConfig wrapperConfig, CppClass cppClass) {
		this.config = config;
		this.cppClass = cppClass;
		this.wrapperConfig = wrapperConfig;
		
		this.buidOverrideMethodList();
	}

	public String getWrapperName() {
		return this.wrapperConfig.makeWrapName(this.config, this.cppClass);
	}
	
	private String getGuardName() {
		return "_cpgf_override_method_is_in_script";
	}
	
	private void buidOverrideMethodList() {
		this.overrideMethodList = new ArrayList<CppMethod>();
		for(CppMethod cppMethod : this.cppClass.getMethodList()) {
			if(cppMethod.isVirtual()) {
				this.overrideMethodList.add(cppMethod);
			}
		}
	}

	private void doWriteConstructor(CppWriter codeWriter, Constructor ctor) {
		codeWriter.writeLine(this.getWrapperName() + "(" + Util.getParameterText(ctor.getParameterList(), true, true, true) + ")");
		codeWriter.incIndent();
		codeWriter.writeLine(": " + this.cppClass.getLiteralName() + "(" + Util.getParameterText(ctor.getParameterList(), false, true) + ") {}");
		codeWriter.decIndent();
	}
	
	private void doWriteOverrideMethod(CppWriter codeWriter, CppMethod cppMethod, int order) {
		String prototype = Util.getInvokablePrototype(cppMethod, cppMethod.getLiteralName());
		if(cppMethod.isConst()) {
			prototype = prototype + " const";
		}
		codeWriter.writeLine(prototype);
		codeWriter.beginBlock();
			String paramText = Util.getParameterText(cppMethod.getParameterList(), false, true);
			String sentinel = this.getGuardName() + "[" + order + "]";
			codeWriter.writeLine("if(! " + sentinel + ")");
			codeWriter.beginBlock();
				codeWriter.writeLine("cpgf::GScriptWrapperReentryGuard guard(&" + sentinel + ");");
				codeWriter.writeLine("cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction(\"" + cppMethod.getLiteralName() + "\"));");
				codeWriter.writeLine("if(func)");
				codeWriter.beginBlock();
					String invoke = "cpgf::invokeScriptFunction(func.get(), this";
					if(cppMethod.hasParameter()) {
						invoke = invoke + ", " + paramText;
					}
					invoke = invoke + ")";
					if(cppMethod.hasResult()) {
						invoke = "return cpgf::fromVariant<" + cppMethod.getResultType().getLiteralType() + " >(" + invoke + ".getValue())";
					}
					invoke = invoke + ";";
					codeWriter.writeLine(invoke);
					if(! cppMethod.hasResult()) {
						codeWriter.writeLine("return;");
					}
				codeWriter.endBlock();
			codeWriter.endBlock();
			if(cppMethod.isPureVirtual()) {
				codeWriter.writeLine("throw \"Abstract method\";");
			}
			else {
				invoke = this.cppClass.getLiteralName() + "::" + cppMethod.getLiteralName() + "(" + paramText + ");";
				if(cppMethod.hasResult()) {
					invoke = "return " + invoke;
				}
				codeWriter.writeLine(invoke);
			}
		codeWriter.endBlock("");
	}
	
	public void writeClassWrapper(CppWriter codeWriter) {
		codeWriter.write("class " + this.getWrapperName() + " : public " + this.cppClass.getLiteralName() + ", public cpgf::GScriptWrapper ");
		codeWriter.writeLine("{");
		codeWriter.writeLine("public:");
		codeWriter.incIndent();

		codeWriter.writeLine("static bool " + this.getGuardName() + "[" + this.overrideMethodList.size() + "];");
		
		for(Constructor ctor : this.cppClass.getConstructorList()) {
			codeWriter.writeLine("");
			this.doWriteConstructor(codeWriter, ctor);
		}

		for(int i = 0; i < this.overrideMethodList.size(); ++i) {
			codeWriter.writeLine("");
			CppMethod cppMethod = this.overrideMethodList.get(i);
			this.doWriteOverrideMethod(codeWriter, cppMethod, i);
		}
		codeWriter.decIndent();
		codeWriter.writeLine("};");
	}

	public void writeCreation(CppWriter codeWriter, String callFunc) {
		List<String> rules = new ArrayList<String>();
		this.cppClass.getPolicyRules(rules);

		String policy = "";
		if(rules != null && rules.size() > 0) {
			policy = "::Policy<MakePolicy<" + Util.joinStringList(", ", rules) + "> >";
		}
		
		String typeName = "GDefineMetaClass<" + this.getWrapperName() + ", " + this.cppClass.getLiteralName() + ">";

		codeWriter.writeLine(typeName +  " _nd = " + typeName + policy + "::declare(\"" + this.getWrapperName() + "\");");
		
		codeWriter.writeLine(callFunc + "(0, _nd);");
		codeWriter.writeLine("_d._class(_nd);");
	}

	public void writeStaticInitializer(CppWriter codeWriter) {
		codeWriter.writeLine("bool " + this.getWrapperName() + "::" + this.getGuardName() + "[" + this.overrideMethodList.size() + "] = { false };");
	}
	
}
