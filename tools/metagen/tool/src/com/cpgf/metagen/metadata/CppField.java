package com.cpgf.metagen.metadata;

public class CppField extends Item {
	private CppType type;

	public CppField(String name, CppType type) {
		super(EnumCategory.Field, name);
		
		this.type = type;
		this.type.setOwner(this);
	}

	public CppType getType() {
		return type;
	}
	
}
