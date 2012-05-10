package com.cpgf.metagen.metadata;

import java.util.HashMap;
import java.util.Map;

import com.cpgf.metagen.cppparser.ParsedType;

public class TypeSolver {
	private MetaInfo metaInfo;
	private Map<String, ParsedType> parsedTypeMap;
	private Map<String, ClassTraits> classTraitsMap;
	
	public TypeSolver(MetaInfo metaInfo) {
		this.metaInfo = metaInfo;
		this.parsedTypeMap = new HashMap<String, ParsedType>();
		this.classTraitsMap = new HashMap<String, ClassTraits>();
	}

	public ParsedType getParsedType(String literalType) {
		ParsedType type = this.parsedTypeMap.get(literalType);

		if(type == null) {
			type = new ParsedType(literalType);
			this.parsedTypeMap.put(literalType, type);
		}

		return type;
	}

	public ClassTraits getClassTraits(String qualifiedType) {
		ClassTraits traits = this.classTraitsMap.get(qualifiedType);

		if(traits == null) {
			CppClass cppClass = this.metaInfo.findClassByName(qualifiedType);
			if(cppClass == null) {
				traits = new ClassTraits();
			}
			else {
				traits = cppClass.getTraits();
			}
			this.classTraitsMap.put(qualifiedType, traits);
		}

		return traits;
	}
}
