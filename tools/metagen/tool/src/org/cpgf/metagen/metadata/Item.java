package org.cpgf.metagen.metadata;

import java.util.ArrayList;
import java.util.List;

import org.cpgf.metagen.Util;



public class Item {
	private String literalName;
	private String primaryName;
	private String qualifiedName;
	private EnumCategory category;
	private EnumVisibility visibility;
	private String location;
	private List<String> namespaces;
	private boolean isStatic;
	private boolean isConst;
	private CppClass owner;
	private List<String> presetPolicyRules;

	protected Item(EnumCategory category, String name) {
		if(name == null) {
			name = "";
		}

		this.category = category;
		this.literalName = name;
		this.primaryName = Util.getItemBaseName(name);
		
		this.namespaces = new ArrayList<String>();
		this.presetPolicyRules = new ArrayList<String>();
	}

	public String getPrimaryName() {
		return primaryName;
	}

	public String getLiteralName() {
		return literalName;
	}

	public String getQualifiedName() {
		if(this.qualifiedName == null) {
			CppClass parent = this.getOwner();
			if(parent == null || parent.isGlobal()) {
				this.qualifiedName = Util.concatQualifiedName(this.getNamespace(), this.getPrimaryName());
			}
			else {
				this.qualifiedName = Util.concatQualifiedName(parent.getQualifiedName(), this.getPrimaryName());
			}
		}

		return this.qualifiedName;
	}
	
	public String getPrefixName() {
		String prefix = this.getFullNamespace();
		if(prefix.equals("")) {
			return this.getPrimaryName();
		}
		else {
			if(this.getPrimaryName().equals("")) {
				return prefix;
			}
			else {
				return prefix + "::" + this.getPrimaryName();
			}
		}
	}
	
	public String getFullNamespace() {
		String namespace = "";
		for(String ns : this.namespaces) {
			if(namespace.length() > 0) {
				namespace = namespace + "::";
			}
			namespace = namespace + ns;
		}
		return namespace;
	}
	
	final public void addPolicyRule(String rule) {
		this.presetPolicyRules.add(rule);
	}
	
	final public void getPolicyRules(List<String> rules) {
		this.doGetPolicyRules(rules);
		for(int i = 0; i < this.presetPolicyRules.size(); ++i) {
			rules.add(this.presetPolicyRules.get(i));
		}
	}

	protected void doGetPolicyRules(List<String> rules) {
	}

	public EnumCategory getCategory() {
		return category;
	}

	public EnumVisibility getVisibility() {
		return visibility;
	}
	
	public void setVisibility(EnumVisibility visibility) {
		this.visibility = visibility;
	}
	
	public boolean isPublic() {
		return this.visibility == EnumVisibility.Public;
	}

	public boolean isProtected() {
		return this.visibility == EnumVisibility.Protected;
	}

	public boolean isPrivate() {
		return this.visibility == EnumVisibility.Private;
	}

	public String getLocation() {
		return location;
	}

	public void setLocation(String location) {
		this.location = location;
	}

	public String getNamespace() {
		return this.namespaces.size() > 0 ? this.namespaces.get(this.namespaces.size() - 1) : "";
	}

	public void setNamespaces(List<String> namespaces) {
		this.namespaces.clear();
		this.namespaces.addAll(namespaces);
	}

	public boolean isStatic() {
		return isStatic || this.owner.isGlobal();
	}

	public void setStatic(boolean isStatic) {
		this.isStatic = isStatic;
	}

	public boolean isConst() {
		return isConst;
	}

	public void setConst(boolean isConst) {
		this.isConst = isConst;
	}

	public CppClass getOwner() {
		return owner;
	}

	public void setOwner(CppClass owner) {
		this.owner = owner;
		this.qualifiedName = null;
	}
	
	public CppClass getRootOwner() {
		CppClass owner = this.owner;

		while(owner.getOwner() != null) {
			owner = owner.getOwner();
		}

		return owner;
	}

	public boolean isConstructor() {
		return this.getCategory() == EnumCategory.Constructor;
	}
	
	public boolean isDestructor() {
		return this.getCategory() == EnumCategory.Destructor;
	}
	
	public boolean isClass() {
		return this.getCategory() == EnumCategory.Class;
	}

	public boolean isTypedef() {
		return this.getCategory() == EnumCategory.Typedef;
	}
	
	public boolean isConstant() {
		return this.getCategory() == EnumCategory.Constant;
	}
	
	public boolean isEnum() {
		return this.getCategory() == EnumCategory.Enum;
	}
	
	public boolean isField() {
		return this.getCategory() == EnumCategory.Field;
	}
	
	public boolean isMethod() {
		return this.getCategory() == EnumCategory.Method;
	}
	
	public boolean isOperator() {
		return this.getCategory() == EnumCategory.Operator;
	}

	public void replaceInType(String pattern, String substitute)
	{
	}

}
