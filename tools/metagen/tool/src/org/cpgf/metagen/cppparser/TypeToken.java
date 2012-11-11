package org.cpgf.metagen.cppparser;

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
	
	public void setToken(String token) {
		this.token = token;
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

	public boolean isLeftBracket() {
		return this.getKind() == EnumTypeTokenKind.LeftAngle
			|| this.getKind() == EnumTypeTokenKind.LeftParenthesis
			|| this.getKind() == EnumTypeTokenKind.LeftSquare;
	}

	public boolean isRightBracket() {
		return this.getKind() == EnumTypeTokenKind.RightAngle
			|| this.getKind() == EnumTypeTokenKind.RightParenthesis
			|| this.getKind() == EnumTypeTokenKind.RightSquare;
	}

	public boolean isLiteralText() {
		return this.getKind() == EnumTypeTokenKind.Const
			|| this.getKind() == EnumTypeTokenKind.Volatile
			|| this.getKind() == EnumTypeTokenKind.Symbol;
	}

}
