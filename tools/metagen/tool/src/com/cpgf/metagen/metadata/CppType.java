package com.cpgf.metagen.metadata;

public class CppType {
	private String baseType;
	private String array;

	public CppType(String baseType) {
		this.baseType = baseType;
	}
	
	public CppType(String baseType, String array) {
		this.baseType = baseType;
		this.array = array;
	}
	
	public String getFullType() {
		return this.baseType + (this.array == null ? "" : this.array);
	}
	
	public String toString() {
		return this.getFullType();
	}
	
}
