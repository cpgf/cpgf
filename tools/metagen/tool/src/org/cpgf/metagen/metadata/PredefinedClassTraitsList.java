package org.cpgf.metagen.metadata;

import java.util.ArrayList;
import java.util.List;

public class PredefinedClassTraitsList {
	private class TraitsItem {
		public String pattern;
		public ClassTraits traits;
	};
	
	private List<TraitsItem> traitsList;
	
	public PredefinedClassTraitsList() {
		this.traitsList = new ArrayList<PredefinedClassTraitsList.TraitsItem>();
	}
	
	public void addTraits(String pattern, ClassTraits traits) {
		TraitsItem item = new TraitsItem();
		item.pattern = pattern;
		item.traits = traits;
		this.traitsList.add(item);
	}
	
	public ClassTraits findTraits(String name) {
		for(TraitsItem item : this.traitsList) {
			if(name.matches(item.pattern)) {
				return item.traits;
			}
		}
		
		return null;
	}
	
	public ClassTraits findTraits(CppClass cppClass) {
		return this.findTraits(cppClass.getQualifiedName());
	}
}
