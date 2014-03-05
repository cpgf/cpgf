package org.cpgf.metagen.metawriter;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.Util;
import org.cpgf.metagen.codewriter.CppWriter;
import org.cpgf.metagen.metadata.ClassTraits;
import org.cpgf.metagen.metadata.ClassWrapperConfig;
import org.cpgf.metagen.metadata.Constructor;
import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.DeferClass;
import org.cpgf.metagen.metadata.CppMethod;


public class ClassWrapperWriter {
	private Config config;
	private ClassWrapperConfig wrapperConfig;
	private CppClass cppClass;
	private Map<String, CppMethod> overrideMethods;

	public ClassWrapperWriter(Config config, ClassWrapperConfig wrapperConfig, CppClass cppClass) {
		this.config = config;
		this.cppClass = cppClass;
		this.wrapperConfig = wrapperConfig;

		this.buidOverrideMethodList();
	}

	public String getWrapperName() {
		return this.wrapperConfig.makeWrapName(this.config, this.cppClass);
	}

	private void buidOverrideMethodList() {
		this.overrideMethods = new HashMap<String, CppMethod>();
		addOverrideMethodsToListRecursively(this.cppClass, this.overrideMethods);
	}

	private void addOverrideMethodsToListRecursively(CppClass item, Map<String, CppMethod> result) {
		for(DeferClass deferClass : item.getBaseClassList()) {
			if(deferClass.getCppClass() != null) {
				addOverrideMethodsToListRecursively(deferClass.getCppClass(), result);
			}
		}
		for(CppMethod cppMethod : item.getMethodList()) {
			String prototype = getMethodPrototypeKey(cppMethod);
			if(cppMethod.isVirtual() || (result.containsKey(prototype) && !cppMethod.isStatic())) {
				result.put(prototype, cppMethod);
			}
		}
	}

	private String getMethodPrototypeKey(CppMethod cppMethod) {
		String result = "";

		if(cppMethod.getResultType() != null) {
			result = cppMethod.getResultType().getLiteralType();
		} else {
			result = "void";
		}

		result = result + " " +  cppMethod.getLiteralName() + "(" + Util.getParameterText(cppMethod.getParameterList(), true, false, false) + ")";

		if(cppMethod.isConst()) {
			result += " const";
		}

		return result.replace(" ", "");
	}

	private void doWriteConstructor(CppWriter codeWriter, Constructor ctor) {
		codeWriter.writeLine(this.getWrapperName() + "(" + Util.getParameterText(ctor.getParameterList(), true, true, true) + ")");
		codeWriter.incIndent();
		codeWriter.writeLine(": " + this.cppClass.getLiteralName() + "(" + Util.getParameterText(ctor.getParameterList(), false, true) + ") {}");
		codeWriter.decIndent();
	}

	private void doWriteOverrideMethod(CppWriter codeWriter, CppMethod cppMethod) {
		String prototype = Util.getInvokablePrototype(cppMethod, cppMethod.getLiteralName());
		if(cppMethod.isConst()) {
			prototype = prototype + " const";
		}
		String paramText = Util.getParameterText(cppMethod.getParameterList(), false, true);
		codeWriter.writeLine(prototype);
		codeWriter.beginBlock();
			codeWriter.writeLine("cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction(\"" + cppMethod.getLiteralName() + "\"));");
			codeWriter.writeLine("if(func)");
			codeWriter.beginBlock();
				String invoke = "cpgf::invokeScriptFunction(func.get(), this";
				if(cppMethod.hasParameter()) {
					invoke = invoke + ", " + paramText;
				}
				invoke = invoke + ")";
				if(cppMethod.hasResult()) {
					invoke = "return cpgf::fromVariant<" + cppMethod.getResultType().getLiteralType() + " >(" + invoke + ")";
				}
				invoke = invoke + ";";
				codeWriter.writeLine(invoke);
				if(! cppMethod.hasResult()) {
					codeWriter.writeLine("return;");
				}
			codeWriter.endBlock();
			if(cppMethod.isPureVirtual()) {
				if(cppMethod.isPureVirtual()) {
					codeWriter.writeLine("throw \"Abstract method\";");
				}
			}
			else {
				invoke = this.cppClass.getLiteralName() + "::" + cppMethod.getLiteralName() + "(" + paramText + ");";
				if(cppMethod.hasResult()) {
					invoke = "return " + invoke;
				}
				codeWriter.writeLine(invoke);
			}
		codeWriter.endBlock("");

		prototype = Util.getInvokablePrototype(cppMethod, WriterUtil.getMethodSuperName(cppMethod));
		if(cppMethod.isConst()) {
			prototype = prototype + " const";
		}
		codeWriter.writeLine(prototype);
		codeWriter.beginBlock();
			if(cppMethod.isPureVirtual()) {
				if(cppMethod.isPureVirtual()) {
					codeWriter.writeLine("throw \"Abstract method\";");
				}
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

	public void writeSuperMethodBind(CppWriter codeWriter) {
		for(CppMethod cppMethod : this.overrideMethods.values()) {
			if (cppMethod.isProtected()) {
                String name = cppMethod.getPrimaryName();
                WriterUtil.reflectMethod(codeWriter, "_d", "D::ClassType::", cppMethod, name, name, true);
			}
		}
		for(CppMethod cppMethod : this.overrideMethods.values()) {
			if (!cppMethod.isPrivate()) {
				String name = WriterUtil.getMethodSuperName(cppMethod);
				WriterUtil.reflectMethod(codeWriter, "_d", "D::ClassType::", cppMethod, name, name, true);
			}
		}
	}

	public void writeClassWrapper(CppWriter codeWriter) {
		codeWriter.write("class " + this.getWrapperName() + " : public " + this.cppClass.getLiteralName() + ", public cpgf::GScriptWrapper ");
		codeWriter.writeLine("{");
		codeWriter.writeLine("public:");
		codeWriter.incIndent();

		for(Constructor ctor : this.cppClass.getConstructorList()) {
		    if (ctor.isPublic()) {
			    codeWriter.writeLine("");
			    this.doWriteConstructor(codeWriter, ctor);
            }
		}

		for(CppMethod cppMethod : this.overrideMethods.values()) {
			if (!cppMethod.isPrivate()) {
			    codeWriter.writeLine("");
			    this.doWriteOverrideMethod(codeWriter, cppMethod);
			}
		}

		codeWriter.decIndent();
		codeWriter.writeLine("};");
	}

	public void writeCreation(CppWriter codeWriter, String callFunc) {
		ClassTraits newTraits = new ClassTraits();
		this.cppClass.getTraits().assignTo(newTraits);
		newTraits.setAbstract(false);
		List<String> rules = new ArrayList<String>();
		newTraits.getRules(rules);

		String policy = "";
		if(rules != null && rules.size() > 0) {
			policy = "::Policy<MakePolicy<" + Util.joinStringList(", ", rules) + "> >";
		}

		String typeName = "GDefineMetaClass<" + this.getWrapperName() + ", " + this.cppClass.getLiteralName() + ">";

		codeWriter.writeLine(typeName +  " _nd = " + typeName + policy + "::declare(\"" + this.getWrapperName() + "\");");

		codeWriter.writeLine(callFunc + "(0, _nd);");
		codeWriter.writeLine("_d._class(_nd);");
	}

}
