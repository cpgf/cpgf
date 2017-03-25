package org.cpgf.metagen.metadata;

import java.util.List;

public class CppField extends Item {
	private CppType type;
	private int bitField;

	public CppField(String name, CppType type) {
		super(EnumCategory.Field, name);
		
		this.type = type;
	}

	public CppType getType() {
		return type;
	}
	
	public boolean isBitField() {
		return this.bitField > 0;
	}

	public int getBitField() {
		return bitField;
	}

	public void setBitField(int bitFields) {
		this.bitField = bitFields;
	}

    @Override
    protected void doGetPolicyRules(List<String> rules) {
        CppType type = getType();
        ClassTraits traits = type.getClassTraits();
        if (traits.isCopyConstructorHidden()) {
            rules.add("GMetaRuleForbidWrite");
            rules.add("GMetaRuleForbidRead");
        }
    }
}
