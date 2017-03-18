package org.cpgf.metagen.metadata;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.doxyxmlparser.FileMap;
import org.cpgf.metagen.metawriter.OperatorNameMap;
import org.cpgf.metagen.metawriter.OutputCallbackClassMap;


public class MetaInfo {
	private Config config;
    private List<CppClass> classList; // top level classes
	private TypeSolver typeSolver;
    private List<CppClass> allClassList; // top level + inner classes
    private List<TemplateInstance> templateInstanceList;
    private OutputCallbackClassMap callbackClassMap;
    private OperatorNameMap operatorNameMap;

    public MetaInfo(Config config) {
    	this.config = config;
    	
        this.classList = new ArrayList<CppClass>();
		this.typeSolver = new TypeSolver(this, this.config.classTraits);
		
		this.templateInstanceList = new ArrayList<TemplateInstance>();
		
		this.callbackClassMap = new OutputCallbackClassMap(this.config);
		
		this.operatorNameMap = new OperatorNameMap();
    }
    
    public Config getConfig() {
    	return this.config;
    }

    public List<CppClass> getClassList() {
        return this.classList;
    }

	public OutputCallbackClassMap getCallbackClassMap() {
		return callbackClassMap;
	}
	
	public OperatorNameMap getOperatorNameMap() {
		return this.operatorNameMap;
	}

    public void fixup(FileMap fileMap) {
    	this.doFixupGlobals();
    	this.doFixupBaseClasses();
    	this.doFixupInnerClasses();
    	this.doFixupOwnerClasses();
    	this.doBuildAllClassList();

    	Map<String, String> configTypeMap = null;
		if(config.parameterTypeReplacer != null) {
			configTypeMap = new HashMap<String, String>();
			for(int i = 0; i < config.parameterTypeReplacer.length ; i += 2) {
				configTypeMap.put(config.parameterTypeReplacer[i], config.parameterTypeReplacer[i + 1]);
			}
		}
		
    	for(CppClass c : this.classList) {
    		if(! c.isGlobal()) {
    			c.resolveTypesForClass();
    		}
    		if(configTypeMap != null) {
    			c.resolveParameterTypes(configTypeMap, null);
    		}
    	}
    	
    	this.doFixupTemplateInstances();
    	
    	this.callbackClassMap.build(this.getClassList(), fileMap);
    }
    
    private void doFixupTemplateInstances() {
    	String[] names = this.config.predefinedTemplateInstances;
    	for(int i = 0; i < names.length; i += 2) {
    		this.doFixupTemplateInstance(names[i], names[i + 1]);
    	}
    }
    
    private void doFixupTemplateInstance(String fullType, String mapName) {
    	int index = fullType.indexOf('<');
    	if(index <= 0) { // can't be first character too.
    		return;
    	}
    	
    	String name = fullType.substring(0, index).trim();
    	CppClass cppClass = this.typeSolver.getCppClass(name);
    	if(cppClass != null) {
    		this.templateInstanceList.add(new TemplateInstance(fullType, cppClass, mapName));
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
		String re = ".*\\b" + name + "$";
if(name.indexOf('{') >= 0) {
	name = name;
}
		for(CppClass cppClass : this.allClassList) {
			if(cppClass.getQualifiedName().matches(re)) {
				return cppClass;
			}
		}
		
		return null;
	}
	
	public List<TemplateInstance> findTemplateInstances(CppClass cppClass) {
		List<TemplateInstance> list = null;
		for(TemplateInstance templateInstance : this.templateInstanceList) {
			if(templateInstance.getTemplateClass() == cppClass) {
				if(list == null) {
					list = new ArrayList<TemplateInstance>();
				}
				list.add(templateInstance);
			}
		}

		return list;
	}
}
