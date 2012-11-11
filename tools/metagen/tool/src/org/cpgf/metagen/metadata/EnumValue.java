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
		if(! n.equals("")) {
			return n + "::" + this.getName();
		}
		else {
			return this.getName();
		}
	}

	public String getValue() {
		return value;
	}
	
}
