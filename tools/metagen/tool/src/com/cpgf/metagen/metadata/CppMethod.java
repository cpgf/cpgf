package com.cpgf.metagen.metadata;

public class CppMethod extends ParameteredItem {
	private CppType resultType;
	
	public CppMethod(String name, CppType resultType) {
		super(EnumCategory.Method, name);
		
		this.resultType = resultType;
	}

	public CppType getResultType() {
		return resultType;
	}

}
