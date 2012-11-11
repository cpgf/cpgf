package org.cpgf.metagen.metadata;

public class Typedef extends Item {
	private CppType type;

	public Typedef(String name, CppType type) {
		super(EnumCategory.Typedef, name);

		this.type = type;
	}

	public CppType getType() {
		return type;
	}

}
