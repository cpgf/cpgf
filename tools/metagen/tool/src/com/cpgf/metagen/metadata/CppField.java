package com.cpgf.metagen.metadata;

public class CppField extends Item {
	private CppType type;
	private int bitField;

	public CppField(String name, CppType type) {
		super(EnumCategory.Field, name);
		
		this.type = type;
		this.type.setOwner(this);
	}

	public CppType getType() {
		return type;
	}
	
	public boolean isBitField() {
		return this.bitField > 0;
	}

	public int getBitField() {
		return bitField;
	}

	public void setBitField(int bitFields) {
		this.bitField = bitFields;
	}
	
}
