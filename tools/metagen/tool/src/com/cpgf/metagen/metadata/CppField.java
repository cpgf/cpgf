package com.cpgf.metagen.metadata;

public class CppField extends Item {
	private CppType type;

	public CppField(String name, CppType type) {
		super(EnumCategory.Field, name);
		
		this.type = type;
	}

	public CppType getType() {
		return type;
	}
	
}
