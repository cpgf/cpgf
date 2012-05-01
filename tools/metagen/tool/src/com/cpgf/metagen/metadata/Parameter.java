package com.cpgf.metagen.metadata;

public class Parameter {
	private String name;
	private CppType type;
	private String defaultValue;

	public Parameter(String name, CppType type, String defaultValue) {
		this.name = name;
		this.type = type;
		this.defaultValue = defaultValue;
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

	public boolean hasDefaultValue() {
		return this.defaultValue != null;
	}
}
