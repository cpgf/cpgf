package com.cpgf.metagen.cppparser;

public class TypeToken {
	private String token;
	private EnumTypeTokenKind kind;
	
	public TypeToken(String token) {
		this.token = token;
		this.kind = ParserUtil.getTypeTokenKind(token);
	}

	public String getToken() {
		return token;
	}

	public EnumTypeTokenKind getKind() {
		return kind;
	}

	public boolean isPointerOrReference() {
		return this.getKind() == EnumTypeTokenKind.Pointer
			|| this.getKind() == EnumTypeTokenKind.Reference
			|| this.getKind() == EnumTypeTokenKind.RValueReference;
	}

	public boolean isConstOrVolatile() {
		return this.getKind() == EnumTypeTokenKind.Const
			|| this.getKind() == EnumTypeTokenKind.Volatile;
	}

}
