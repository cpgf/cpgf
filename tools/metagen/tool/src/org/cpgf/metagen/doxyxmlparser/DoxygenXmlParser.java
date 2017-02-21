package org.cpgf.metagen.doxyxmlparser;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Stack;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.MetaException;
import org.cpgf.metagen.Util;
import org.cpgf.metagen.metadata.Constant;
import org.cpgf.metagen.metadata.Constructor;
import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.CppEnum;
import org.cpgf.metagen.metadata.CppField;
import org.cpgf.metagen.metadata.CppMethod;
import org.cpgf.metagen.metadata.CppType;
import org.cpgf.metagen.metadata.DeferClass;
import org.cpgf.metagen.metadata.Destructor;
import org.cpgf.metagen.metadata.EnumVisibility;
import org.cpgf.metagen.metadata.Item;
import org.cpgf.metagen.metadata.MetaInfo;
import org.cpgf.metagen.metadata.Operator;
import org.cpgf.metagen.metadata.Parameter;
import org.cpgf.metagen.metadata.ParameteredItem;
import org.cpgf.metagen.metadata.Typedef;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;


public class DoxygenXmlParser {
	private Config config;
	private MetaInfo metaInfo;
	private FileMap fileMap;
	private String xmlFileName;
	private String basePath;
	private File xmlFile;
	
	private Stack<CppClass> classStack;
	private List<String> namespaceStack;
	
	public DoxygenXmlParser(Config config, MetaInfo metaInfo, FileMap fileMap, String xmlFileName) {
		this.config = config;
		this.metaInfo = metaInfo;
		this.fileMap = fileMap;
		this.xmlFileName = xmlFileName;
		this.xmlFile = new File(this.xmlFileName);
		this.basePath = this.xmlFile.getParent();
		this.classStack = new Stack<CppClass>();
		this.namespaceStack = new ArrayList<String>();
		
		Util.trace("Parsing " + xmlFileName);
	}
	
	public void parseFile() throws Exception {
		DocumentBuilderFactory builderFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder xmlBuilder = builderFactory.newDocumentBuilder();
		Document xmlDocument = xmlBuilder.parse(this.xmlFile);
		
		this.doParaseXmlDocument(xmlDocument);
	}
	
	private void error(String message) throws MetaException {
		throw new MetaException(message);
	}
	
	private CppClass enterClass(String name) {
		CppClass cppClass = new CppClass(name);
		
		this.classStack.push(cppClass);
		this.metaInfo.getClassList().add(cppClass);
		
		return cppClass;
	}
	
	private void leaveClass() {
		this.classStack.pop();
	}
	
	private CppClass getCurrentClass() {
		return this.classStack.peek();
	}
	
	private void enterNamepsace(String namespace) {
		this.namespaceStack.add(namespace);
	}
	
	private void leaveNamespace() {
		this.namespaceStack.remove(this.namespaceStack.size() - 1);
	}
	
	private CppType getType(Node node) {
		String baseType = Util.getNodeText(Util.getNode(node, "type"));
		String array = Util.getNodeText(Util.getNode(node, "array"));

		return new CppType(this.metaInfo.getTypeSolver(), baseType, array);
	}
	
	private String getLocation(Node node) {
		Node child = Util.getNode(node, "location");
		if(child == null) {
			return "";
		}
		else {
			return Util.getAttribute(child, "file");
		}
	}

	private EnumVisibility getVisibility(Node node) {
		String v = Util.getAttribute(node, "prot");
		
		if(v.indexOf("public") >= 0) {
			return EnumVisibility.Public;
		}
		else if(v.indexOf("private") >= 0) {
			return EnumVisibility.Private;
		}
		else {
			return EnumVisibility.Protected;
		}
	}

	private void takeVisibility(Node node, Item item) {
		item.setVisibility(this.getVisibility(node));
	}

	private void takeLocation(Node node, Item item) {
		item.setLocation(this.getLocation(node));
	}

	private void resolveNamespace(Item item) {
		item.setNamespaces(this.namespaceStack);
	}

	private void doParaseXmlDocument(Document xmlDocument) throws Exception {
		NodeList nodeList = xmlDocument.getElementsByTagName("doxygen");
		if(nodeList.getLength() == 0) {
			nodeList = xmlDocument.getElementsByTagName("doxygenindex");
		}
		
		if(nodeList.getLength() == 0) {
			error("Invalid Doxygen XML format.");
		}

		Node root = nodeList.item(0);
		NodeList childList = root.getChildNodes();
		for(int i = 0; i < childList.getLength(); ++i) {
			Node child = childList.item(i);
			String nodeName = child.getNodeName();
			if(nodeName.equals("compound")) {
				this.doParseCompound(child);
			}
			else if(nodeName.equals("compounddef")) {
				this.doParseCompounddef(child);
			}
			else {
			}
		}
	}
	
	private void doParseCompound(Node node) throws Exception {
		String refid = Util.getAttribute(node, "refid");

		String fileName = (new File(this.basePath, refid + ".xml")).getAbsolutePath();
		if(fileName != null && fileName.length() > 0) {
			DoxygenXmlParser parser = new DoxygenXmlParser(this.config, this.metaInfo, this.fileMap, fileName);
			parser.parseFile();
		}
	}

	private void doParseCompounddef(Node node) {
		String kind = Util.getAttribute(node, "kind");
		String location = this.getLocation(node);

		if(kind.equals("class")) {
			this.doParseClass(node, location);
		}
		else if(kind.equals("union")) {
			// we can't handle union for now because union will cause compile error in type traits
			this.doParseClass(node, location);
		}
		else if(kind.equals("struct")) {
			this.doParseClass(node, location);
		}
		else if(kind.equals("file")) {
			this.fileMap.addLocation(location, node);
			this.doParseDefFile(node, location);
		}
		else if(kind.equals("namespace")) {
			this.doParseNamespace(node, location);
		}
	}
	
	private void doParseClass(Node node, String location) {
		String className = Util.getNodeText(Util.getNode(node, "compoundname"));
		CppClass cppClass = this.enterClass(className);
		cppClass.setLocation(location);

		try {
			this.doParseBaseClasses(node);
			this.doParseInnerClasses(node);
			this.doParseAllSectionDef(node, location);
			this.doParseTemplateParams(node, cppClass);

			this.resolveNamespace(cppClass);
		}
		finally {
			this.leaveClass();
		}
	}

	private void doParseDefFile(Node node, String location) {
		this.enterClass(null).setLocation(location);
		try {
			this.doParseAllSectionDef(node, location);
		}
		finally {
			this.leaveClass();
		}
	}

	private void doParseNamespace(Node node, String location) {
		this.enterNamepsace(Util.getNodeText(Util.getNode(node, "compoundname")));
		try {
			this.doParseDefFile(node, location);
		}
		finally {
			this.leaveNamespace();
		}
	}
	
	private void doParseBaseClasses(Node node) {
		for(Node child : Util.getChildNodesByName(node, "basecompoundref")) {
			DeferClass baseClass = new DeferClass(Util.getNodeText(child), this.getVisibility(child));
			this.getCurrentClass().getBaseClassList().add(baseClass);
	   	}
	}
	
	private void doParseInnerClasses(Node node) {
		for(Node child : Util.getChildNodesByName(node, "innerclass")) {
			DeferClass baseClass = new DeferClass(Util.getNodeText(child), this.getVisibility(child));
			this.getCurrentClass().getClassList().add(baseClass);
	   	}
	}
	
	private void doParseAllSectionDef(Node node, String location) {
		for(Node child : Util.getChildNodesByName(node, "sectiondef")) {
			this.doParseSectionDef(child, location);
		}
	}

	private void doParseSectionDef(Node node, String location) {
		for(Node child : Util.getChildNodesByName(node, "memberdef")) {
			String kind = Util.getAttribute(child, "kind");
			String name = Util.getNodeText(Util.getNode(child, "name"));
			
			Item item = null;

			if(kind.equals("function")) {
				item = this.doParseMethod(child, name);
			}
			else if(kind.equals("variable")) {
				item = this.doParseField(child, name);
			}
			else if(kind.equals("enum")) {
				item = this.doParseEnum(child, name);
			}
			else if(kind.equals("define")) {
				item = this.doParseConstant(child, name);
			}
			else if(kind.equals("typedef")) {
				item = this.doParseTypedef(child, name);
			}
			else if(kind.equals("enumvalue")) {
			}
			
			if(item != null) {
				this.takeVisibility(child, item);
				this.takeLocation(child, item);
				this.resolveNamespace(item);
			}
		}
	}

	private Item doParseMethod(Node node, String name) {
		if(! this.getCurrentClass().isGlobal()) {
			if(name.indexOf('~') >= 0 && ! name.matches("operator\\s*~")) {
				Destructor destructor = new Destructor();
				this.getCurrentClass().setDestructor(destructor);

				return destructor;
			}
			
			if(this.getCurrentClass().getPrimaryName().equals(name)) { // constructor
				Constructor constructor = new Constructor();
				this.doParseParams(node, constructor);
				this.doParseTemplateParams(node, constructor);
				this.getCurrentClass().getConstructorList().add(constructor);
				constructor.setExplicit(Util.isValueYes(Util.getAttribute(node, "explicit")));

				return constructor;
			}
		}

		Pattern pattern = Pattern.compile("^.*\\boperator(.*)$");
		Matcher matcher = pattern.matcher(name);

		if(matcher.matches()) { // operator
			String op = matcher.group(1);
			Operator operator = new Operator(
					op, 
					new CppType(this.metaInfo.getTypeSolver(), Util.getNodeText(Util.getNode(node, "type")))
			);

			operator.setStatic(Util.isValueYes(Util.getAttribute(node, "static")));
			operator.setConst(Util.isValueYes(Util.getAttribute(node, "const")));

			if(operator.getResultType().isEmpty()) { // type convertion operator, T()
				operator.setResultType(new CppType(this.metaInfo.getTypeSolver(), operator.getOperator()));
			}
			this.doParseParams(node, operator);
			this.doParseTemplateParams(node, operator);
			this.getCurrentClass().getOperatorList().add(operator);

			return operator;
		}

		// method
		CppMethod method = new CppMethod(
			name,
			new CppType(this.metaInfo.getTypeSolver(), Util.getNodeText(Util.getNode(node, "type")))
		);
		method.setStatic(Util.isValueYes(Util.getAttribute(node, "static")));
		method.setConst(Util.isValueYes(Util.getAttribute(node, "const")));
		method.setVirtual(Util.getAttribute(node, "virt").equals("virtual"));
		method.setPureVirtual(Util.getAttribute(node, "virt").equals("pure-virtual"));
		
		this.doParseParams(node, method);
		this.doParseTemplateParams(node, method);
		this.getCurrentClass().getMethodList().add(method);

		return method;
	}

	private void doParseParams(Node node, ParameteredItem item) {
		for(Node child : Util.getChildNodesByName(node, "param")) {
			Parameter param = new Parameter(
					Util.getNodeText(Util.getNode(child, "declname")),
					this.getType(child),
					Util.getNodeText(Util.getNode(child, "defval")),
					item
			);
			if(! param.getType().isVoid()) {
				item.getParameterList().add(param);
			}
		}
	}
	
	private void doParseTemplateParams(Node node, ParameteredItem item) {
		node = Util.getNode(node, "templateparamlist");
		if(node == null) {
			return;
		}
		
		item.setTemplate(true);

		for(Node child : Util.getChildNodesByName(node, "param")) {
			String baseType = Util.getNodeText(Util.getNode(child, "type"));
			String array = Util.getNodeText(Util.getNode(child, "array"));
			String name = Util.getNodeText(Util.getNode(child, "declname"));
			
			if(name == null) {
				name = baseType;
				name = name.replaceAll("\\btypename\\b", "");
				name = name.replaceAll("\\bclass\\b", "");
				name = name.trim();
				if(baseType.matches("\\btypename\\b.*")) {
					baseType = "typename";
				}
				else if(baseType.matches("\\bclass\\b.*")) {
					baseType = "class";
				}
			}

			Parameter param = new Parameter(
					name,
					new CppType(this.metaInfo.getTypeSolver(), baseType, array),
					Util.getNodeText(Util.getNode(child, "defval")),
					item
				);
			item.getTemplateParameterList().add(param);
		}
	}
	
	private Item doParseField(Node node, String name) {
		CppField field = new CppField(name, this.getType(node));
		
		field.setStatic(Util.isValueYes(Util.getAttribute(node, "static")));
		
		Node bitFieldNode = Util.getNode(node, "bitfield");
		if(bitFieldNode != null) {
			field.setBitField(Integer.parseInt(Util.getNodeText(bitFieldNode).trim()));
		}

		this.getCurrentClass().getFieldList().add(field);
		
		return field;
	}
	
	private Item doParseEnum(Node node, String name) {
		CppEnum cppEnum = new CppEnum(name);
		this.getCurrentClass().getEnumList().add(cppEnum);
		for(Node child : Util.getChildNodesByName(node, "enumvalue")) {
			cppEnum.addValue(
					Util.getNodeText(Util.getNode(child, "name")),
					Util.getNodeText(Util.getNode(child, "initializer"))
			);
		}

		return cppEnum;
	}

	private Item doParseConstant(Node node, String name) {
		if(Util.getNode(node, "param") != null) {
			return null;
		}

		Constant constant = new Constant(
				Util.getNodeText(Util.getNode(node, "name")),
				Util.getNodeText(Util.getNode(node, "initializer"))
		);

		this.getCurrentClass().getConstantList().add(constant);

		return constant;
	}

	private Item doParseTypedef(Node node, String name) {
		Typedef typedef = new Typedef(name, this.getType(node));
		
		this.getCurrentClass().getTypedefList().add(typedef);
		
		return typedef;
	}
	
}
