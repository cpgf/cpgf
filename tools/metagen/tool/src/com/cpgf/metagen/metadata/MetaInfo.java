package com.cpgf.metagen.metadata;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class MetaInfo {
    private List<CppClass> classList;
	private TypeSolver typeSolver;
    private List<CppClass> allClassList;

    public MetaInfo() {
        this.classList = new ArrayList<CppClass>();
		this.typeSolver = new TypeSolver(this);
    }

    public List<CppClass> getClassList() {
        return this.classList;
    }

    public List<CppClass> getAllClassList() {
        return this.allClassList;
    }

    public void fixup() {
    	this.doFixupGlobals();
    	this.doFixupBaseClasses();
    	this.doFixupInnerClasses();
    	this.doFixupOwnerClasses();
    	this.doBuildAllClassList();
    	
    	for(CppClass c : this.classList) {
    		if(! c.isGlobal()) {
    			c.resolveTypesForInnerClass();
    		}
    	}
    }
    
    private void doBuildAllClassList() {
    	this.allClassList = new ArrayList<CppClass>();
    	
    	for(CppClass c : this.classList) {
    		this.doBuildAllClassListForSingleClass(c);
    	}
    }

    private void doBuildAllClassListForSingleClass(CppClass cppClass) {
    	if(cppClass == null) {
    		return;
    	}

    	if(! cppClass.isGlobal()) {
    		this.allClassList.add(cppClass);
    	}

    	for(DeferClass c : cppClass.getClassList()) {
    		this.doBuildAllClassListForSingleClass(c.getCppClass());
    	}
    }

    private void doFixupGlobals() {
    	HashMap<String, CppClass> globalMap = new HashMap<String, CppClass>();

    	boolean finished = false;
    	while(! finished) {
    		finished = true;

    		for(int i = 0; i < this.classList.size(); ++i) {
    			CppClass c = this.classList.get(i);
    			if(! c.isGlobal()) {
    				continue;
    			}

    			this.doFixupGlobalItems(globalMap, c.getFieldList());
    			this.doFixupGlobalItems(globalMap, c.getMethodList());
    			this.doFixupGlobalItems(globalMap, c.getOperatorList());
    			this.doFixupGlobalItems(globalMap, c.getEnumList());
    			this.doFixupGlobalItems(globalMap, c.getConstantList());
    			this.doFixupGlobalItems(globalMap, c.getTypedefList());

    			this.classList.remove(i);
    			finished = false;
    			break;
    		}
    	}

    	for(CppClass c : globalMap.values()) {
    		this.classList.add(0, c);
    	}
    }

    private <T extends Item> void doFixupGlobalItems(HashMap<String, CppClass> globalMap, List<T> itemList) {
    	for(Item item : itemList) {
    		String location = item.getLocation();
    		if(! globalMap.containsKey(location)) {
    			CppClass c = new CppClass(null);
    			c.setLocation(location);
    			globalMap.put(location, c);
    		}
    		globalMap.get(location).addItem(item);
    	}
    }
    
    private void doFixupBaseClasses() {
    	for(CppClass c : this.classList) {
    		for(DeferClass deferClass : c.getBaseClassList()) {
    			deferClass.resolve(this.classList);
    		}
    	}
    }

    private void doFixupInnerClasses() {
    	for(CppClass c : this.classList) {
    		for(DeferClass deferClass : c.getClassList()) {
    			deferClass.resolve(this.classList);
    			deferClass.getCppClass().setInner(true);
    		}
    	}
    	
    	for(int i = this.classList.size() - 1; i >= 0; --i) {
    		if(this.classList.get(i).isInner()) {
    			this.classList.remove(i);
    		}
    	}
    }

    private void doFixupOwnerClasses() {
    	for(CppClass c : this.classList) {
    		c.fixupOwners();
    	}
    }

	public TypeSolver getTypeSolver() {
		return typeSolver;
	}

	public CppClass findClassByName(String name) {
		String re = "\\b" + name + "$";
		
		for(CppClass cppClass : this.allClassList) {
			if(cppClass.getQualifiedName().matches(re)) {
				return cppClass;
			}
		}
		
		return null;
	}
}
