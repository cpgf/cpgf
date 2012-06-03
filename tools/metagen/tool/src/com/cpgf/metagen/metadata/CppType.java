package com.cpgf.metagen.metadata;

import java.util.Map;

import com.cpgf.metagen.cppparser.EnumCompoundType;
import com.cpgf.metagen.cppparser.ParsedType;

public class CppType {
	private Item owner;
	TypeSolver typeSolver;
	private ParsedType parsedType;

	public CppType(TypeSolver typeSolver, String baseType) {
		this.typeSolver = typeSolver;
		if(typeSolver != null) {
			this.parsedType = typeSolver.getParsedType(baseType);
		}
		else {
			this.parsedType = new ParsedType(baseType);
		}
	}

	public CppType(TypeSolver typeSolver, String baseType, String array) {
		this.typeSolver = typeSolver;
		if(typeSolver != null) {
			this.parsedType = typeSolver.getParsedType(baseType + (array == null ? "" : array));
		}
		else {
			this.parsedType = new ParsedType(baseType);
		}
	}

	public String getLiteralType() {
		return this.parsedType.getLiteralType();
	}
	
	public void setLiteralType(String literalType) {
		this.parsedType.setLiteralType(literalType);
	}

	// use to identify the type
	public String getQualifiedBaseType() {
		return this.parsedType.getBaseType();
	}

	public ClassTraits getClassTraits() {
		return this.typeSolver.getClassTraits(this.getQualifiedBaseType());
	}

	public Item getOwner() {
		return owner;
	}

	public void setOwner(Item owner) {
		this.owner = owner;
	}

	public boolean isConstValueReference() {
		return ! this.parsedType.isPointer()
			&& (this.parsedType.getReference() == EnumCompoundType.Const);
	}

	public ParsedType getParsedType() {
		return parsedType;
	}

	public void replaceToken(Map<String, String> map)
	{
		this.parsedType.replaceToken(map);
	}

}
