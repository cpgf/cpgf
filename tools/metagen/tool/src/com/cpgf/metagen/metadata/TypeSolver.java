package com.cpgf.metagen.metadata;

import java.util.HashMap;
import java.util.Map;

import com.cpgf.metagen.cppparser.ParsedType;

public class TypeSolver {
	private Map<String, ParsedType> cache;
	
	public TypeSolver() {
		this.cache = new HashMap<String, ParsedType>();
	}

	public ParsedType getParsedType(String literalType) {
		ParsedType type = this.cache.get(literalType);

		if(type == null) {
			type = new ParsedType(literalType);
			this.cache.put(literalType, type);
		}

		return type;
	}

}
