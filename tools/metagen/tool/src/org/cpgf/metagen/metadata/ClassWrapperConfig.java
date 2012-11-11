package org.cpgf.metagen.metadata;

import org.cpgf.metagen.Config;

public class ClassWrapperConfig {
	private boolean wrapClass;
	private String wrapName;

	public boolean isWrapClass() {
		return wrapClass;
	}
	
	public void setWrapClass(boolean wrapClass) {
		this.wrapClass = wrapClass;
	}
	
	public String getWrapName() {
		return wrapName;
	}
	
	public void setWrapName(String wrapName) {
		this.wrapName = wrapName;
	}
	
	public String makeWrapName(Config config, CppClass cppClass) {
		String name = this.wrapName;
		if(name == null) {
			name = cppClass.getPrimaryName();
		}
		if(config.scriptClassWrapperPostfix != null) {
			name = name + config.scriptClassWrapperPostfix;
		}
		return name;
	}
}
