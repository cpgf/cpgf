package org.cpgf.metagen.cppparser;

public enum EnumCompoundType {
	None,
	NoCV,
	Const,
	Volatile,
	ConstVolatile;
	
	public static EnumCompoundType add(EnumCompoundType ct, EnumCompoundType add) {
		if(ct == None || ct == NoCV) {
			ct = add;
		}

		if(add != None && ct != add) {
			ct = ConstVolatile;
		}

		return ct;
	}
}
