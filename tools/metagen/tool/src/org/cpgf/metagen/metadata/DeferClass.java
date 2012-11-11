package org.cpgf.metagen.metadata;

import java.util.List;

import org.cpgf.metagen.Util;


public class DeferClass {
	private String name;
	private EnumVisibility visibility;
	private CppClass cppClass;
	
	public DeferClass(String name, EnumVisibility visibility) {
		this.name = name;
		this.visibility = visibility;
	}
	
	public DeferClass(CppClass cppClass) {
		this.cppClass = cppClass;
		this.visibility = EnumVisibility.Public;
	}
	
	public void resolve(List<CppClass> classList) {
		if(this.cppClass == null) {
			this.cppClass = (CppClass)(Util.findItemByName(classList, this.name));
		}

		if(this.cppClass != null) {
			this.cppClass.setVisibility(this.getVisibility());
		}
	}

	public CppClass getCppClass() {
		return cppClass;
	}

	public EnumVisibility getVisibility() {
		return visibility;
	}

	public String getName() {
		return name;
	}

}
