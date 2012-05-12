package com.cpgf.metagen.metadata;

import java.util.HashMap;
import java.util.Map;

import com.cpgf.metagen.cppparser.ParsedType;

public class TypeSolver {
	private MetaInfo metaInfo;
	private PredefinedClassTraitsList predefinedClassTraits;
	private Map<String, ParsedType> parsedTypeMap;
	private Map<String, ClassTraits> classTraitsMap;
	
	public TypeSolver(MetaInfo metaInfo, PredefinedClassTraitsList predefinedClassTraits) {
		this.metaInfo = metaInfo;
		this.predefinedClassTraits = predefinedClassTraits;
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
			traits = this.predefinedClassTraits.findTraits(qualifiedType);
		}

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
