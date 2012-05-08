package com.cpgf.metagen.metadata;

public class Operator extends CppInvokable {
	public Operator(String operator, CppType resultType) {
		super(EnumCategory.Operator, operator, resultType);
	}

	public String getOperator() {
		return this.getLiteralName();
	}

}
