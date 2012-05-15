package com.cpgf.metagen.metadata;

public class TemplateInstance {
	private String fullType;
	private CppClass templateClass;
	
	public TemplateInstance(String fullType, CppClass templateClass) {
		this.fullType = fullType;
		this.templateClass = templateClass;
	}

	public String getFullType() {
		return fullType;
	}

	public CppClass getTemplateClass() {
		return templateClass;
	}

	public String getTemplateType() {
		return this.fullType.substring(this.fullType.indexOf('<') + 1, this.fullType.lastIndexOf('>'));
	}
}
