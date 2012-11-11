package org.cpgf.metagen.metadata;

public class CppMethod extends CppInvokable {
	public CppMethod(String name, CppType resultType) {
		super(EnumCategory.Method, name, resultType);
	}

}
