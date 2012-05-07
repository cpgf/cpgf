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
}
