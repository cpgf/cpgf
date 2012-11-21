package org.cpgf.metagen.metadata;

import java.util.Map;

import org.cpgf.metagen.cppparser.EnumCompoundType;
import org.cpgf.metagen.cppparser.ParsedType;


public class CppType {
	private TypeSolver typeSolver;
	private ParsedType parsedType;

	public CppType(TypeSolver typeSolver, String literalType) {
		this.typeSolver = typeSolver;
		if(typeSolver != null) {
			this.parsedType = typeSolver.getParsedType(literalType);
		}
		else {
			this.parsedType = new ParsedType(literalType);
		}
	}

	public CppType(TypeSolver typeSolver, String literalType, String array) {
		this.typeSolver = typeSolver;
		if(typeSolver != null) {
			this.parsedType = typeSolver.getParsedType(literalType + (array == null ? "" : array));
		}
		else {
			this.parsedType = new ParsedType(literalType + (array == null ? "" : array));
		}
	}
	
	public ParsedType getParsedType() {
		return this.parsedType;
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

	public boolean isVoid() {
		return this.parsedType.isVoid();
	}

	public boolean isPointer() {
		return this.parsedType.isPointer();
	}

	public boolean isEmpty() {
		return this.parsedType.isEmpty();
	}
	
	public boolean isConstValueReference() {
		return ! this.parsedType.isPointer()
			&& (this.parsedType.getReference() == EnumCompoundType.Const);
	}

	public boolean isNonConstValueReference() {
		return ! this.parsedType.isPointer()
			&& (this.parsedType.getReference() == EnumCompoundType.NoCV);
	}

	public void replaceToken(Map<String, String> map)
	{
		if(this.parsedType != null) {
			this.parsedType.replaceToken(map);
		}
		else {
			if(map.containsKey(this.getLiteralType())) {
				this.setLiteralType(map.get(this.getLiteralType()));
			}
		}
	}

}
