package com.cpgf.metagen.metadata;

public class TemplateInstance {
	private String fullType;
	private CppClass templateClass;
	private String mapName;
	
	public TemplateInstance(String fullType, CppClass templateClass, String mapName) {
		this.fullType = fullType;
		this.templateClass = templateClass;
		this.mapName = mapName;
		
		if(this.mapName == null || this.mapName.length() == 0) {
			this.mapName = this.fullType;
		}
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

	public String getMapName() {
		return mapName;
	}
}
