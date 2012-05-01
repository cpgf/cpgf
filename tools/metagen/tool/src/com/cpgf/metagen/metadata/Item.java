package com.cpgf.metagen.metadata;


public class Item {
	private String name;
	private EnumCategory category;
	private EnumVisibility visibility;
	private String location;
	private String namespace;
	private boolean isStatic;
	private boolean isConst;
	private CppClass owner;
	
	protected Item(EnumCategory category, String name) {
		this.category = category;
		this.name = name;
	}

	public String getName() {
		return name;
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
	
	public String getLocation() {
		return location;
	}
	
	public void setLocation(String location) {
		this.location = location;
	}

	public String getNamespace() {
		return namespace;
	}

	public void setNamespace(String namespace) {
		this.namespace = namespace;
		if(this.namespace != null) {
			this.name = this.namespace + "::" + name;
		}
	}

	public boolean isStatic() {
		return isStatic;
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
	}

	public boolean isConstructor() { return this.getCategory() == EnumCategory.Constructor; }
	public boolean isDestructor() { return this.getCategory() == EnumCategory.Destructor; }
	public boolean isClass() { return this.getCategory() == EnumCategory.Class; }
	public boolean isConstant() { return this.getCategory() == EnumCategory.Constant; }
	public boolean isEnum() { return this.getCategory() == EnumCategory.Enum; }
	public boolean isField() { return this.getCategory() == EnumCategory.Field; }
	public boolean isMethod() { return this.getCategory() == EnumCategory.Method; }
	public boolean isOperator() { return this.getCategory() == EnumCategory.Operator; }

}
