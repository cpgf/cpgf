package com.cpgf.metagen.metawriter;

import java.util.ArrayList;
import java.util.List;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.codewriter.CodeWriter;
import com.cpgf.metagen.codewriter.CppWriter;
import com.cpgf.metagen.metadata.CppClass;
import com.cpgf.metagen.metadata.Item;
import com.cpgf.metagen.metadata.Parameter;
import com.cpgf.metagen.metadata.TemplateInstance;

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
				String value = paramList.get(index).getDefaultValue();
				if(value.equals("NULL") || value.equals("nullptr")) {
					value = "(" + paramList.get(index).getType().getLiteralType() + ")" + value;
				}
				writer.out("._default(copyVariantFromCopyable(" + value + "))\n");
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

			codeWriter.out(typeName +  " " + varName + " = " + typeName + policy + "::" + action + "(\"" + cppClass.getPrimaryName() + "\");\n");
		}
	}

	public static void createMetaClass(CppWriter codeWriter, CppClass cppClass, String varName, String callFunc, List<TemplateInstance> templateInstanceList) {
		List<String> rules = new ArrayList<String>();
		cppClass.getPolicyRules(rules);
		
		if(cppClass.isGlobal()) {
			codeWriter.out("GDefineMetaGlobalDangle " + varName + " = GDefineMetaGlobalDangle::dangle();\n");
			
			codeWriter.out(callFunc + "(0, _d, NULL);\n");
			codeWriter.out("return _d.getMetaInfo();\n");
		}
		else {
			if(cppClass.isTemplate()) {
				codeWriter.out("GDefineMetaGlobalDangle " + varName + " = GDefineMetaGlobalDangle::dangle();\n");

				for(TemplateInstance templateInstance : templateInstanceList) {
					codeWriter.beginBlock();
					
					String typeName = "GDefineMetaClass<" + templateInstance.getFullType();
					
					typeName = typeName + Util.generateBaseClassList(cppClass.getBaseClassList());
					
					typeName = typeName + " >";
					String policy = "";
					if(rules != null && rules.size() > 0) {
						policy = "::Policy<MakePolicy<" + Util.joinStringList(", ", rules) + "> >";
					}
					codeWriter.out(typeName +  " _nd = " + typeName + policy + "::declare(\"" + templateInstance.getFullType() + "\");\n");
					
					codeWriter.out(callFunc + "<" + typeName + ", " + templateInstance.getTemplateType() + " >(0, _nd, NULL);\n");
					codeWriter.out(varName + "._class(_nd);\n");

					codeWriter.endBlock();
				}
				
				codeWriter.out("return _d.getMetaInfo();\n");
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
				
				codeWriter.out(callFunc + "(0, _d, NULL);\n");
				codeWriter.out("return _d.getMetaInfo();\n");
			}
		}
	}
	
	public static String getPolicyText(Item item) {
		return getPolicyText(item, true);
	}

	public static String getPolicyText(Item item, boolean prefixWithComma) {
		List<String> rules = new ArrayList<String>();
		
		item.getPolicyRules(rules);
		
		if(rules.size() > 0) {
			String rulesText = "";
			for(String s : rules) {
				if(rulesText.length() > 0) {
					rulesText = rulesText + ", ";
				}
				rulesText = rulesText + "cpgf::" + s;
			}
			return (prefixWithComma ? ", " : "")
					+ "cpgf::MakePolicy<"
					+ rulesText
					+ " >()";
		}
		else {
			return "";
		}
	}
	
}
