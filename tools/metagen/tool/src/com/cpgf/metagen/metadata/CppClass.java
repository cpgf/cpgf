package com.cpgf.metagen.metadata;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.cpgf.metagen.Util;

public class CppClass extends ParameteredItem {
	private List<Constructor> constructorList;
	private Destructor destructor;
	private List<Constant> constantList;
	private List<CppField> fieldList;
	private List<CppMethod> methodList;
	private List<CppEnum> enumList;
	private List<Operator> operatorList;
	private List<DeferClass> classList;
	private List<DeferClass> baseClassList;
	private List<Typedef> typedefList;
	
	private boolean isInner;
	
	private ClassTraits traits;
	
	public CppClass(String name) {
		super(EnumCategory.Class, name);

		this.constructorList = new ArrayList<Constructor>();
		this.constantList = new ArrayList<Constant>();
		this.fieldList = new ArrayList<CppField>();
		this.methodList = new ArrayList<CppMethod>();
		this.enumList = new ArrayList<CppEnum>();
		this.operatorList = new ArrayList<Operator>();
		this.typedefList = new ArrayList<Typedef>();
		this.classList = new ArrayList<DeferClass>();
		this.baseClassList = new ArrayList<DeferClass>();
	}

	public List<Constructor> getConstructorList() {
		return constructorList;
	}

	public List<Constant> getConstantList() {
		return constantList;
	}

	public Destructor getDestructor() {
		return destructor;
	}

	public void setDestructor(Destructor destructor) {
		this.destructor = destructor;
	}

	public List<CppField> getFieldList() {
		return fieldList;
	}

	public List<CppMethod> getMethodList() {
		return methodList;
	}

	public List<CppEnum> getEnumList() {
		return enumList;
	}

	public List<Operator> getOperatorList() {
		return operatorList;
	}

	public List<Typedef> getTypedefList() {
		return typedefList;
	}

	public List<DeferClass> getClassList() {
		return classList;
	}
	
	public boolean isGlobal() {
		return this.getPrimaryName().equals("");
	}

	public List<DeferClass> getBaseClassList() {
		return baseClassList;
	}
	
	public void addItem(Item item) {
		switch(item.getCategory()) {
		case Constructor:
			this.getConstructorList().add((Constructor)item);
			break;

		case Class:
			this.getClassList().add(new DeferClass((CppClass)item));
			break;
			
		case Constant:
			this.getConstantList().add((Constant)item);
			break;
			
		case Enum:
			this.getEnumList().add((CppEnum)item);
			break;
			
		case Field:
			this.getFieldList().add((CppField)item);
			break;
			
		case Method:
			this.getMethodList().add((CppMethod)item);
			break;
			
		case Operator:
			this.getOperatorList().add((Operator)item);
			break;
			
		default:
			assert false;
		}
	}

	public boolean isInner() {
		return isInner;
	}

	public void setInner(boolean isInner) {
		this.isInner = isInner;
	}
	
	public void fixupOwners() {
		if(this.destructor != null) {
			this.destructor.setOwner(this);
		}
		
		this.doFixupOwners(this.constructorList);
		this.doFixupOwners(this.fieldList);
		this.doFixupOwners(this.methodList);
		this.doFixupOwners(this.enumList);
		this.doFixupOwners(this.operatorList);
		this.doFixupOwners(this.constantList);
		this.doFixupOwners(this.typedefList);

		for(DeferClass deferClass : this.classList) {
			deferClass.getCppClass().setOwner(this);
		}
	}

	private <T extends Item> void doFixupOwners(List<T> itemList) {
		for(Item item : itemList) {
			item.setOwner(this);
		}
	}
	
	public boolean isAbstract() {
		return this.getTraits().isAbstract();
	}
	
	public boolean canGenerateMetaCode() {
		if(this.isTemplate()) {
			return false;
		}
		return this.constructorList.size() > 0
				|| this.fieldList.size() > 0
				|| this.methodList.size() > 0
				|| this.enumList.size() > 0
				|| this.operatorList.size() > 0
				|| this.constantList.size() > 0
			;
	}
	
	@Override
	public void getPolicyRules(List<String> rules) {
		this.getTraits().getRules(rules);
	}

	private boolean checkAbstract() {
		for(CppMethod method : this.methodList) {
			if(method.isPureVirtual()) {
				return true;
			}
		}
		
		return false;
	}
	
	private boolean checkDestructorHidden() {
		if(this.getDestructor() != null && this.getDestructor().getVisibility() != EnumVisibility.Public) {
			return true;
		}
		else {
			return false;
		}
	}

	private boolean checkDefaultConstructorHidden() {
		boolean hasDefaultCtor = false;
		boolean hasNonDefaultCtor = false;
		boolean hasNonPublicDefaultCtor = false;
		for(Constructor c : this.getConstructorList()) {
			if(c.isDefaultConstructor()) {
				hasDefaultCtor = true;
				if(c.getVisibility() != EnumVisibility.Public) {
					hasNonPublicDefaultCtor = true;
				}
			}
			else {
				hasNonDefaultCtor = true;
			}
		}

		if(hasNonPublicDefaultCtor || (!hasDefaultCtor && hasNonDefaultCtor)) {
			return true;
		}
		else {
			return false;
		}
	}
	
	private boolean checkCopyConstructorHidden() {
		for(Constructor c : this.getConstructorList()) {
			if(c.isCopyConstructor()) {
				if(c.getVisibility() != EnumVisibility.Public) {
					return true;
				}
			}
		}
		
		for(DeferClass deferClass : this.getBaseClassList()) {
			if(deferClass.getCppClass() != null) {
				if(deferClass.getCppClass().checkCopyConstructorHidden()) {
					return true;
				}
			}
		}
		
		return false;
	}
	
	private boolean checkHasTypeConverterConstructor() {
		for(Constructor c : this.getConstructorList()) {
			if(c.isTypeConverter()) {
				return true;
			}
		}
		
		return false;
	}
	
	private void loadTraits(ClassTraits traits) {
		traits.setAbstract(this.checkAbstract());
		traits.setDestructorHidden(this.checkDestructorHidden());
		traits.setDefaultConstructorHidden(this.checkDefaultConstructorHidden());
		traits.setCopyConstructorHidden(this.checkCopyConstructorHidden());
		traits.setHasTypeConvertConstructor(this.checkHasTypeConverterConstructor());
	}

	public ClassTraits getTraits() {
		if(this.traits == null) {
			ClassTraits t = new ClassTraits();
			this.loadTraits(t);
			this.traits = t;
		}
		return this.traits;
	}
	
	public void resolveTypesForInnerClass() {
		Map<String, String> innerClassMap = new HashMap<String, String>();
		
		this.doBuildInnerClassMap(innerClassMap, null);
		
		this.doResolveTypesForInner(innerClassMap, this.getConstructorList());
		this.doResolveTypesForInner(innerClassMap, this.getMethodList());
		this.doResolveTypesForInner(innerClassMap, this.getOperatorList());

		for(DeferClass deferClass : this.getClassList()) {
			deferClass.getCppClass().resolveTypesForInnerClass();
		}
	}
	
	private <T extends CppInvokable> void doResolveTypesForInner(Map<String, String> innerClassMap, List<T> list) {
		for(T item : list) {
			this.doResolveTypeForInvokable(innerClassMap, item);
		}
	}
	
	private void doResolveTypeForInvokable(Map<String, String> innerClassMap, CppInvokable item) {
		this.doResolveType(innerClassMap, item.getResultType());
		for(Parameter param : item.getParameterList()) {
			this.doResolveType(innerClassMap, param.getType());
		}
	}
	
	private void doResolveType(Map<String, String> innerClassMap, CppType type) {
		if(type != null) {
			if(innerClassMap.containsKey(type.getLiteralType())) {
				type.setLiteralType(innerClassMap.get(type.getLiteralType()));
			}
		}
	}
	
	private void doBuildInnerClassMap(Map<String, String> innerClassMap, String outterName) {
		if(outterName != null) {
			outterName = outterName + "::" + this.getPrimaryName();
			innerClassMap.put(this.getPrimaryName(), outterName);
		}
		else {
			outterName = this.getPrimaryName();
		}
		
		for(DeferClass deferClass : this.getClassList()) {
			deferClass.getCppClass().doBuildInnerClassMap(innerClassMap, outterName);
		}
	}
	
}
