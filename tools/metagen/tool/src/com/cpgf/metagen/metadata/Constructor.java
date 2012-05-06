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
	
	public boolean isDefaultConstructor() {
		return ! this.hasParameter();
	}

	public boolean isCopyConstructor() {
		if(this.getParameterList().size() == 1 && this.getOwner() != null) {
			String paramType = this.getParameterList().get(0).getType().getFullType();
			return paramType.indexOf("const") >= 0
					&& paramType.indexOf(this.getOwner().getPrimaryName()) >= 0
					&& paramType.indexOf('&') > 0
				;
		}
		
		return false;
	}
}
