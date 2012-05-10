package com.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.List;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.codewriter.CodeWriter;
import com.cpgf.metagen.codewriter.CppWriter;
import com.cpgf.metagen.metadata.CppClass;
import com.cpgf.metagen.metadata.Parameter;

public class WriterUtil {

	public static void writeCommentForAutoGeneration(CodeWriter writer) {
		writer.out("// Auto generated file, don't modify.\n");
		writer.out("\n");
	}

	public static void writeParamList(CodeWriter writer, List<Parameter> paramList, boolean withName) {
		boolean comma = false;
		for(Parameter p : paramList) {
			if(comma) {
				writer.out(", ");
			}
			writeParam(writer, p, withName);
			comma = true;
		}
	}

	public static void writeDefaultParams(CodeWriter writer, List<Parameter> paramList) {
		int index = paramList.size() - 1;

		if(index >= 0 && paramList.get(index).hasDefaultValue()) {
			writer.out("\n");
			writer.incIndent();

			while(index >= 0 && paramList.get(index).hasDefaultValue()) {
				writer.out("._default(copyVariantFromCopyable(" + paramList.get(index).getDefaultValue() + "))\n");
				--index;
			}

			writer.decIndent();
		}
		writer.out(";\n");
	}

	public static void writeParam(CodeWriter writer, Parameter param, boolean withName) {
		String type = param.getType().getLiteralType();
		
		writer.out(type + (withName ? " " + param.getName() : ""));
	}

	public static void defineMetaClass(Config config, CppWriter codeWriter, CppClass cppClass, String varName, String action) {
		List<String> rules = new ArrayList<String>();
		cppClass.getPolicyRules(rules);
		
		String namespace = "\"" + ((config.metaNamespace != null) ? config.metaNamespace : "") + "\"";
		
		if(cppClass.isGlobal()) {
			if(config.metaNamespace != null) {
				codeWriter.out("GDefineMetaNamespace " + varName + " = GDefineMetaNamespace::" + action + "(" + namespace + ");\n");
			}
			else {
				codeWriter.out("GDefineMetaGlobal " + varName + ";\n");
			}
		}
		else {
			String typeName = "GDefineMetaClass<" + cppClass.getQualifiedName();

			typeName = typeName + Util.generateBaseClassList(cppClass.getBaseClassList());
			
			typeName = typeName + ">";
			String policy = "";
			if(rules != null && rules.size() > 0) {
				policy = "::Policy<MakePolicy<" + Util.joinStringList(", ", rules) + "> >";
			}
			if(config.metaNamespace != null) {
				codeWriter.out("GDefineMetaNamespace _ns = GDefineMetaNamespace::" + action + "(" + namespace + ");\n");
				codeWriter.out(typeName +  " " + varName + " = " + typeName + policy + "::declare(\"" + cppClass.getPrimaryName() + "\");\n");
				codeWriter.out("_ns._class(" + varName + ");\n");
			}
			else {
				codeWriter.out(typeName +  " " + varName + " = " + typeName + policy + "::" + action + "(\"" + cppClass.getPrimaryName() + "\");\n");
			}
		}
	}

	public static void createMetaClass(Config config, CodeWriter codeWriter, CppClass cppClass, String varName) {
		List<String> rules = new ArrayList<String>();
		cppClass.getPolicyRules(rules);
		
		if(cppClass.isGlobal()) {
			codeWriter.out("GDefineMetaGlobalDangle " + varName + " = GDefineMetaGlobalDangle::dangle();\n");
		}
		else {
			String typeName = "GDefineMetaClass<" + cppClass.getLiteralName();
			
			typeName = typeName + Util.generateBaseClassList(cppClass.getBaseClassList());
			
			typeName = typeName + ">";
			String policy = "";
			if(rules != null && rules.size() > 0) {
				policy = "::Policy<MakePolicy<" + Util.joinStringList(", ", rules) + "> >";
			}
			codeWriter.out(typeName +  " " + varName + " = " + typeName + policy + "::declare(\"" + cppClass.getPrimaryName() + "\");\n");
		}
	}
	
}
