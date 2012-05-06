package com.cpgf.metagen.metadata;

public class Typedef extends Item {
	private CppType type;

	public Typedef(String name, CppType type) {
		super(EnumCategory.Typedef, name);
		
		this.type = type;
		this.type.setOwner(this);
	}

	public CppType getType() {
		return type;
	}

}
