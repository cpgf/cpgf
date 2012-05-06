package com.cpgf.metagen.metadata;

public class CppType {
	private String baseType;
	private Item owner;

	public CppType(String baseType) {
		this.baseType = baseType;
	}

	public CppType(String baseType, String array) {
		this.baseType = baseType + (array == null ? "" : array);
	}
	
	public String getFullType() {
		return this.baseType;
	}
	
	public Item getOwner() {
		return owner;
	}

	public void setOwner(Item owner) {
		this.owner = owner;
	}

}
