package com.cpgf.metagen;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.cpgf.metagen.metadata.CppClass;
import com.cpgf.metagen.metadata.DeferClass;
import com.cpgf.metagen.metadata.EnumVisibility;
import com.cpgf.metagen.metadata.Item;
import com.cpgf.metagen.metadata.Parameter;
import com.cpgf.metagen.metawriter.CodeWriter;
import com.cpgf.metagen.metawriter.CppWriter;

public class Util {
	private static int currentUniqueID = 0;

	public static int getUniqueID() {
		++currentUniqueID;
		return currentUniqueID;
	}
	
	public static void trace(String message) {
		System.out.println(message);
	}

	public static void error(String message) throws MetaException {
		throw new MetaException(message);
	}

	public static String getAttribute(Node node, String attributeName) {
		return node.getAttributes().getNamedItem(attributeName).getNodeValue();
	}
	
	public static Node getNode(Node node, String nodeName) {
		List<Node> childList = getChildNodesByName(node, nodeName);
		if(childList.size() > 0) {
			return childList.get(0);
		}
		else {
			return null;
		}
	}

	public static String getNodeText(Node node) {
		if(node == null) {
			return null;
		}

		String result = "";
		NodeList childList = node.getChildNodes();
		
		for(int i = 0; i < childList.getLength(); ++i) {
			Node child = childList.item(i);
			if(child.getNodeName().equals("#text")) {
				result = result + child.getNodeValue();
			}
			else {
				result = result + getNodeText(child);
			}
		}

		return result;
	}

	public static List<Node> getChildNodesByName(Node node, String name) {
		List<Node> result = new ArrayList<Node>();
		
		NodeList childList = node.getChildNodes();
		
		for(int i = 0; i < childList.getLength(); ++i) {
			Node child = childList.item(i);
			if(child.getNodeName().equals(name)) {
				result.add(child);
			}
		}

		return result;
	}
	
	public static boolean isValueYes(String value) {
		return ! value.toLowerCase().equals("no");
	}
	
	public static String getBaseName(String name) {
		Pattern pattern = Pattern.compile("^.*\\b(\\w+)$");
		Matcher matcher = pattern.matcher(name);
		if(matcher.matches()) {
			return matcher.group(1);
		}
		else {
			return name;
		}
	}
	
	public static <T extends Item> Item findItemByName(List<T> itemList, String name) {
		for(int i = 0; i < itemList.size(); ++i) {
			String itemName = itemList.get(i).getName();
			if(itemName != null && itemName.equals(name)) {
				return itemList.get(i);
			}
		}
		
		return null;
	}

	public static String readTextFromFile(String fileName) throws Exception {
	    BufferedReader bufferReader = new BufferedReader(new FileReader(fileName));
	    try {
	        StringBuilder stringBuilder = new StringBuilder();
	        boolean needNewLine = false;

	        while(true) {
		        String line = bufferReader.readLine();
		        if(line == null) {
		        	break;
		        }
	            if(needNewLine) {
	            	stringBuilder.append("\n");
	            }
	            needNewLine = true;
	            stringBuilder.append(line);
	        }

	        return stringBuilder.toString();
	    } finally {
	        bufferReader.close();
	    }
	}
	
	public static boolean writeTextToFile(String fileName, String content) throws Exception {
		File file = new File(fileName);

		if(file.exists()) {
			if(content.replaceAll("\\s+$", "").equals(readTextFromFile(fileName).replaceAll("\\s+$", ""))) {
				RunStats.countWrittenFile(false);
				return false;
			}
		}

	    BufferedWriter bufferWriter = new BufferedWriter(new FileWriter(file));
	    try {
	    	bufferWriter.write(content);
	    } finally {
	    	bufferWriter.close();
	    }

		RunStats.countWrittenFile(true);
		return true;
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
		String type = param.getType().getFullType();
		
		writer.out(type + (withName ? " " + param.getName() : ""));
	}

	public static String joinStringList(String delimiter, List<String> stringList) {
		String result = "";
		boolean isFirst = true;
		
		for(String s : stringList) {
			if(! isFirst) {
				result = result + delimiter;
			}
			isFirst = false;

			result = result + s;
		}
		
		return result;
	}

	public static void defineMetaClass(Config config, CppWriter codeWriter, CppClass cppClass, String varName, String action) {
		List<String> rules = cppClass.getPolicyRules();
		
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
			String typeName = "GDefineMetaClass<" + cppClass.getName();
			for(DeferClass deferClass : cppClass.getBaseClassList()) {
				if(deferClass.getCppClass().getVisibility() == EnumVisibility.Public) {
					typeName = typeName + ", " + getBaseName(deferClass.getCppClass().getName());
				}
			}
			
			typeName = typeName + ">";
			String policy = "";
			if(rules != null && rules.size() > 0) {
				policy = "::Policy<MakePolicy<" + joinStringList(", ", rules) + "> >";
			}
			if(config.metaNamespace != null) {
				codeWriter.out("GDefineMetaNamespace _ns = GDefineMetaNamespace::" + action + "(" + namespace + ");\n");
				codeWriter.out(typeName +  " " + varName + " = " + typeName + policy + "::declare(\"" + getBaseName(cppClass.getName()) + "\");\n");
				codeWriter.out("_ns._class(" + varName + ");\n");
			}
			else {
				codeWriter.out(typeName +  " " + varName + " = " + typeName + policy + "::" + action + "(\"" + getBaseName(cppClass.getName()) + "\");\n");
			}
		}
	}

	public static void writeAutoComment(CodeWriter writer) {
		writer.out("// Auto generated file, don't modify.\n");
		writer.out("\n");
	}

	public static String normalizeSymbol(String s) {
		return s.replaceAll("\\.", "_");
	}
	
	public static String getBaseFileName(String fileName) {
		fileName = fileName.replaceAll("\\\\", "/");
		int index = fileName.lastIndexOf('/');
		if(index >= 0) {
			fileName = fileName.substring(index + 1);
		}
		index = fileName.lastIndexOf('.');
		if(index >= 0) {
			fileName = fileName.substring(0, index);
		}
		
		return fileName;
	}
	
	public static List<CppClass> sortClassList(List<CppClass> classList) {
		List<CppClass> newList = new ArrayList<CppClass>();
		newList.addAll(classList);
		Collections.sort(newList, new Comparator<CppClass>() {

			@Override
			public int compare(CppClass o1, CppClass o2) {
				if(o1.getName() == null) {
					return -1;
				}
				if(o2.getName() == null) {
					return 1;
				}
				return o1.getName().compareTo(o2.getName());
			}
		}
		);
		return newList;
	}

	public static List<String> sortStringList(List<String> stringList) {
		List<String> newList = new ArrayList<String>();
		newList.addAll(stringList);
		Collections.sort(newList);
		return newList;
	}

	public static void createMetaClass(Config config, CodeWriter codeWriter, CppClass cppClass, String varName, List<String> rules) {
		if(cppClass.isGlobal()) {
			codeWriter.out("GDefineMetaGlobalDangle " + varName + " = GDefineMetaGlobalDangle::dangle();\n");
		}
		else {
			String typeName = "GDefineMetaClass<" + cppClass.getName();
			for(DeferClass deferClass : cppClass.getBaseClassList()) {
				if(deferClass.getCppClass().getVisibility() == EnumVisibility.Public) {
					typeName = typeName + ", " + getBaseName(deferClass.getCppClass().getName());
				}
			}
			
			typeName = typeName + ">";
			String policy = "";
			if(rules != null && rules.size() > 0) {
				policy = "::Policy<MakePolicy<" + joinStringList(", ", rules) + "> >";
			}
			codeWriter.out(typeName +  " " + varName + " = " + typeName + policy + "::declare(\"" + getBaseName(cppClass.getName()) + "\");\n");
		}
	}
	
	public static String upcaseFirst(String s)
	{
		if(s.length() <= 1) {
			return s.toUpperCase();
		}
		else {
			return s.substring(0, 1).toUpperCase() + s.substring(1);
		}
	}
	
	public static String concatFileName(String path, String fileName) {
		return new File(path, fileName).getAbsolutePath();
	}
	
	public static void forceCreateDirectories(String path) {
		(new File(path, "")).mkdirs();
	}

}
