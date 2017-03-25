package org.cpgf.metagen.metadata;

public class EnumValue {
	private CppEnum owner;
	private String name;
	private String value;
	
	public EnumValue(CppEnum owner, String name, String value) {
		this.owner = owner;
		this.name = name;
		this.value = value;
	}

	public String getName() {
		return name;
	}
	
	public String getQualifiedName() {
		String n = this.owner.getFullNamespace();
		String enumName = this.getName();
		
		if(! this.owner.getPrimaryName().equals("")) {
			enumName = this.owner.getPrimaryName() + "::" + enumName;
		}
		
		if(! n.equals("")) {
			return n + "::" + enumName;
		}
		else {
			return enumName;
		}
	}

	public String getValue() {
		return value;
	}
	
}
