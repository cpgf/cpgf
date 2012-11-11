package org.cpgf.metagen.metadata;

import java.util.ArrayList;
import java.util.List;

import org.cpgf.metagen.cppparser.ParserUtil;


public class TemplateInstance {
	private String fullType;
	private CppClass templateClass;
	private String mapName;
	private List<String> arguments;
	
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
	
	public List<String> getArguments() {
		if(this.arguments == null) {
			this.arguments = new ArrayList<String>();
			ParserUtil.splitDelimitedString(this.arguments, this.getTemplateType(), ',', 0);
		}
		return this.arguments;
	}

	public String getMapName() {
		return mapName;
	}
}
