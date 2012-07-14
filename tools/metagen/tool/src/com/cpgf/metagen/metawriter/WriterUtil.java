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
		writer.write("// Auto generated file, don't modify.\n");
		writer.write("\n");
	}

	public static void writeParamList(CodeWriter writer, List<Parameter> paramList, boolean withName) {
		writer.write(Util.getParameterText(paramList, true, withName));
	}

	public static void writeDefaultParams(CodeWriter writer, List<Parameter> paramList) {
		int index = paramList.size() - 1;

		if(index >= 0 && paramList.get(index).hasDefaultValue()) {
			writer.write("\n");
			writer.incIndent();

			while(index >= 0 && paramList.get(index).hasDefaultValue()) {
				String value = paramList.get(index).getDefaultValue();
				if(value.equals("NULL") || value.equals("nullptr")) {
					value = "(" + paramList.get(index).getType().getLiteralType() + ")" + value;
				}
				writer.write("._default(copyVariantFromCopyable(" + value + "))\n");
				--index;
			}

			writer.decIndent();
		}
		writer.write(";\n");
	}

	public static void defineMetaClass(Config config, CppWriter codeWriter, CppClass cppClass, String varName, String action) {
		List<String> rules = new ArrayList<String>();
		cppClass.getPolicyRules(rules);
		
		String namespace = "\"" + ((config.metaNamespace != null) ? config.metaNamespace : "") + "\"";
		
		if(cppClass.isGlobal()) {
			if(config.metaNamespace != null) {
				codeWriter.write("GDefineMetaNamespace " + varName + " = GDefineMetaNamespace::" + action + "(" + namespace + ");\n");
			}
			else {
				codeWriter.write("GDefineMetaGlobal " + varName + ";\n");
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

			codeWriter.write(typeName +  " " + varName + " = " + typeName + policy + "::" + action + "(\"" + cppClass.getPrimaryName() + "\");\n");
		}
	}

	private static String normalizeClassName(String name)
	{
		name = name.replaceAll("::", "_");
		name = name.replaceAll(":", "_");
		name = name.replaceAll("<", "_");
		name = name.replaceAll(">", "");
		name = name.replaceAll("\\s", "");

		return name;
	}
	
	public static void createMetaClass(CppWriter codeWriter, CppClass cppClass, String callFunc, List<TemplateInstance> templateInstanceList) {
		List<String> rules = new ArrayList<String>();
		cppClass.getPolicyRules(rules);
		
		if(cppClass.isGlobal()) {
			codeWriter.write("GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();\n");
			
			codeWriter.write(callFunc + "(0, _d, NULL);\n");
		}
		else {
			String policy = "";
			if(rules != null && rules.size() > 0) {
				policy = "::Policy<MakePolicy<" + Util.joinStringList(", ", rules) + "> >";
			}

			codeWriter.write("GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();\n");
			if(cppClass.isTemplate()) {
				for(TemplateInstance templateInstance : templateInstanceList) {
					codeWriter.beginBlock();
					
					String typeName = "GDefineMetaClass<" + templateInstance.getFullType();
					typeName = typeName + Util.generateBaseClassList(cppClass.getBaseClassList());
					typeName = typeName + " >";

					codeWriter.write(typeName +  " _nd = " + typeName + policy + "::declare(\"" + normalizeClassName(templateInstance.getMapName()) + "\");\n");
					
					codeWriter.write(callFunc + "<" + typeName + ", " + templateInstance.getTemplateType() + " >(0, _nd, NULL);\n");
					codeWriter.write("_d._class(_nd);\n");

					codeWriter.endBlock();
				}
			}
			else {
				codeWriter.beginBlock();

				String typeName = "GDefineMetaClass<" + cppClass.getLiteralName();
				typeName = typeName + Util.generateBaseClassList(cppClass.getBaseClassList());
				typeName = typeName + ">";
				codeWriter.write(typeName +  " _nd = " + typeName + policy + "::declare(\"" + cppClass.getPrimaryName() + "\");\n");
				
				codeWriter.write(callFunc + "(0, _nd, NULL);\n");
				codeWriter.write("_d._class(_nd);\n");

				codeWriter.endBlock();
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
