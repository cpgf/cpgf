package com.cpgf.metagen.metadata;

public class Operator extends ParameteredItem {
	private CppType resultType;
	
	public Operator(String operator, CppType resultType) {
		super(EnumCategory.Operator, operator);
		
		this.resultType = resultType;
	}

	public String getOperator() {
		return super.getPrimaryName();
	}

	public CppType getResultType() {
		return resultType;
	}

	public void setResultType(CppType resultType) {
		this.resultType = resultType;
	}

}
