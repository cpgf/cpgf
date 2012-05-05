package com.cpgf.metagen.metadata;

public class Constructor extends ParameteredItem {
	private boolean isExplicit;

	public Constructor() {
		super(EnumCategory.Constructor, "");
	}

	public boolean isExplicit() {
		return isExplicit;
	}

	public void setExplicit(boolean isExplicit) {
		this.isExplicit = isExplicit;
	}
}
