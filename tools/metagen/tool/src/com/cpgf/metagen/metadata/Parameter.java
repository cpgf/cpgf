package com.cpgf.metagen.metadata;

public class Parameter {
	private String name;
	private CppType type;
	private String defaultValue;
	private ParameteredItem owner;

	public Parameter(String name, CppType type, String defaultValue, ParameteredItem owner) {
		this.name = name;
		this.type = type;
		this.defaultValue = defaultValue;
		this.owner = owner;
		this.type.setOwner(this.owner);
	}

	public String getName() {
		return name;
	}

	public CppType getType() {
		return type;
	}
	
	public String getDefaultValue() {
		return defaultValue;
	}
	
	public void setDefaultValue(String defaultValue) {
		this.defaultValue = defaultValue;
	}

	public void clearDefaultValue() {
		this.defaultValue = null;
	}

	public boolean hasDefaultValue() {
		return this.defaultValue != null;
	}

	public ParameteredItem getOwner() {
		return owner;
	}

	public void setOwner(ParameteredItem owner) {
		this.owner = owner;
	}

}
