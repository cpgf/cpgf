package org.cpgf.metagen;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.CppInvokable;
import org.cpgf.metagen.metadata.DeferClass;
import org.cpgf.metagen.metadata.EnumVisibility;
import org.cpgf.metagen.metadata.Item;
import org.cpgf.metagen.metadata.Parameter;
import org.cpgf.metagen.metadata.TemplateInstance;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;


public class Util {
	private static Map<String, UniqueID> uniqueIDMap = new HashMap<String, UniqueID>();

	public static int getUniqueID(String scope) {
		if(scope == null || scope.length() == 0) {
			scope = "global";
		}
		if(! uniqueIDMap.containsKey(scope)) {
			uniqueIDMap.put(scope, new UniqueID());
		}
		return uniqueIDMap.get(scope).getNext();
	}
	
	public static void trace(String message) {
		System.out.println(message);
	}

	public static void error(String message) throws MetaException {
		throw new MetaException(message);
	}

	public static String quoteText(String text) {
		return "\"" + text + "\"";
	}
	
	public static String concatQualifiedName(String qualify, String name)
	{
		if(qualify == null || qualify.length() == 0) {
			return name;
		}
		else {
			return qualify + "::" + name;
		}
	}

	public static String getAttribute(Node node, String attributeName) {
		Node n = node.getAttributes().getNamedItem(attributeName);
		if(n != null) {
			return n.getNodeValue();
		}
		else {
			return null;
		}
	}

	public static int getIntAttribute(Node node, String attributeName, int defaultValue) {
		String value = getAttribute(node, attributeName);
		if(value == null) {
			return defaultValue;
		}
		return Integer.parseInt(value);
	}

	public static int getIntAttribute(Node node, String attributeName) {
		return getIntAttribute(node, attributeName, 0);
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
	
	public static <T extends Item> Item findItemByName(List<T> itemList, String name) {
		for(int i = 0; i < itemList.size(); ++i) {
			String itemName = itemList.get(i).getLiteralName();
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
				trace("Skip " + fileName);
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

		trace("Written " + fileName);
		RunStats.countWrittenFile(true);
		return true;
	}
	
	public static void deleteFile(String fileName) {
		if((new File(fileName)).delete()) {
			trace("Deleted " + fileName);
			RunStats.countDeletedFile();
		}
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

	public static String normalizeSymbol(String s) {
		s = s.replaceAll("::", "_");
		s = s.replaceAll("-", "_");
		s = s.replaceAll("\\.", "_");
		return s;
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
				return o1.getQualifiedName().compareTo(o2.getQualifiedName());
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

	public static String generateBaseClassList(CppClass cppClass, TemplateInstance templateInstance) {
		String typeName = "";

		for(DeferClass deferClass : cppClass.getBaseClassList()) {
			if(deferClass.getVisibility() == EnumVisibility.Public) {
				String baseName = deferClass.getName();
				
				if(templateInstance != null && cppClass.isTemplate()) {
					// check if the class is inherited from a template parameter
					int index = -1;
					for(Parameter templateParameter : cppClass.getTemplateParameterList()) {
						++index;
						if(baseName.equals(templateParameter.getName())) {
							if(templateInstance.getArguments().size() > index) {
								baseName = templateInstance.getArguments().get(index);
							}
							else {
								baseName = templateParameter.getDefaultValue();
							}
							break;
						}
					}
				}
				
				typeName = typeName + ", " + baseName;
			}
		}
		
		return typeName;
	}

	public static String generateBaseClassList(CppClass cppClass) {
		return generateBaseClassList(cppClass, null);
	}

	public static <T> void swapListItems(List<T> list, int a, int b) {
		T temp = list.get(a);
		list.set(a, list.get(b));
		list.set(b, temp);
	}

	public static <T> int findInArray(T[] array, T item) {
		for(int i = 0; i < array.length; ++i) {
			if(array[i].equals(item)) {
				return i;
			}
		}

		return -1;
	}

	public static <T> void addToList(List<T> list, T item) {
		if(item == null) {
			return;
		}

		for(T d : list) {
			if(d.equals(item)) {
				return;
			}
		}

		list.add(item);
	}
	
	public static String selectString(String a, String b) {
		return a != null ? a : b;
	}

	public static String getItemBaseName(String name) {
		if(name == null) {
			return "";
		}
		if(name.indexOf('@') >= 0) {
			return "";
		}
		
		Pattern pattern = Pattern.compile("^.*\\b(\\w+)$");
		Matcher matcher = pattern.matcher(name);
		if(matcher.matches()) {
			return matcher.group(1);
		}
		else {
			return name;
		}
	}

	public static String getParameterText(List<Parameter> parameterList, boolean withType, boolean withName, boolean withDefaultValue) {
		String result = "";
	    int counter = 0;	
		for(Parameter param : parameterList) {
			if(result.length() > 0) {
				result = result + ", ";
			}
			if(withType) {
				result = result + param.getType().getLiteralType();
			}
			if(withName && param.getName() != null) {
				if(withType) {
					result = result + " ";
				}
				String name = param.getName();
				if ("" == name) {
                    name = "__arg"+(counter++);
				}
				result = result + name;
			} else if (withName) {
				if(withType) {
					result = result + " ";
				}
				result = result + "__arg"+(counter++);
			}
			if(withDefaultValue && param.hasDefaultValue()) {
				result = result + " = " + param.getDefaultValue();
			}
		}
		
		return result;
	}

	public static String getParameterText(List<Parameter> parameterList, boolean withType, boolean withName) {
		return getParameterText(parameterList, withType, withName, false);
		
	}
	
	public static String getInvokablePrototype(CppInvokable invokable, String name, boolean withDefaultValue) {
		if(name == null) {
			name = invokable.getPrimaryName();
		}
		
		String result = "";
		
		if(invokable.getResultType() != null) {
			result = invokable.getResultType().getLiteralType();
		}
		else {
			result = "void";
		}
		
		result = result + " " + name + "(" + getParameterText(invokable.getParameterList(), true, true, withDefaultValue) + ")";
		
		return result;
	}
	
	public static String getInvokablePrototype(CppInvokable invokable, String name) {
		return getInvokablePrototype(invokable, name, true);
	}
	
	private static class RegExpFlags {
		public boolean replaceAll;
	};
	
	public static Pattern parseRegExp(String s, RegExpFlags outFlags)
	{
		String re;
		int flags = 0;

		outFlags.replaceAll = false;
		Matcher matcher = Pattern.compile("^[/!](.*)[/!]([imsg]*)$").matcher(s);

		if(matcher.matches()) {
			re = matcher.group(1);
			String flagText = matcher.group(2);
			if(flagText.indexOf('i') >= 0) {
				flags |= Pattern.CASE_INSENSITIVE;
			}
			if(flagText.indexOf('m') >= 0) {
				flags |= Pattern.MULTILINE;
			}
			if(flagText.indexOf('s') >= 0) {
				flags |= Pattern.DOTALL;
			}
			if(flagText.indexOf('g') >= 0) {
				outFlags.replaceAll = true;
			}
		}
		else {
			re = s;
		}
		
		return Pattern.compile(re, flags);
	}
	
	public static String replaceStringWithArray(String s, String[] replacers) throws MetaException {
		if(replacers != null) {
			if(replacers.length %2 != 0) {
				error("Replacer must be in even count. In format of [ RegExp, replacer, RegExp, replacer, ... ]");
			}
			
			for(int i = 0; i < replacers.length; i += 2) {
				s = replaceStringWithRegExp(s, replacers[i], replacers[i + 1]);
			}
		}
		
		return s;
	}
	
	public static String replaceStringWithRegExp(String s, String re, String replacer) {
		RegExpFlags flags = new RegExpFlags();
		Pattern pattern = parseRegExp(re, flags);
		Matcher matcher = pattern.matcher(s);
		if(flags.replaceAll) {
			return matcher.replaceAll(replacer);
		}
		else {
			return matcher.replaceFirst(replacer);
		}
	}

	public static boolean allowMetaData(Config config, Item item) {
		return config.allowPublic && item.getVisibility() == EnumVisibility.Public
				|| config.allowProtected && item.getVisibility() == EnumVisibility.Protected
				|| config.allowPrivate && item.getVisibility() == EnumVisibility.Private
		;
	}
	
}

class UniqueID {
	private int currentValue;

	public UniqueID() {
		this.currentValue = -1;
	}
	
	public int getNext() {
		++this.currentValue;
		return this.currentValue;
	}
}
