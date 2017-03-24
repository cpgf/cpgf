package org.cpgf.metagen.metadata;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.cpgf.metagen.Util;


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
	
	private String fullQualifiedName;
	
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

	public String getFullQualifiedName() {
		if(this.fullQualifiedName == null) {
			this.fullQualifiedName = this.getPrimaryName();
			if(this.fullQualifiedName.equals("")) {
				return this.fullQualifiedName;
			}

			if(this.isTemplate()) {
				this.fullQualifiedName = this.fullQualifiedName + "<" + Util.getParameterText(this.getTemplateParameterList(), false, true) + " >";
			}

			CppClass parent = this.getOwner();
			if(parent == null) {
				if(! this.getNamespace().equals("")) {
					this.fullQualifiedName = this.getNamespace() + "::" + this.fullQualifiedName;
				}
			}
			else {
				while(parent != null) {
					String parentQualifiedname = parent.getFullQualifiedName();
					if(parentQualifiedname == null || parentQualifiedname.equals("")) {
						break;
					}
					this.fullQualifiedName = parentQualifiedname + "::" + this.fullQualifiedName;
					parent = parent.getOwner();
				}
			}
			
//			if(this.getOwner() != null && this.getOwner().isTemplate()) {
//				if(this.isTemplate()) {
//					this.fullQualifiedName = "template " + this.fullQualifiedName;
//				}
//				else {
//					this.fullQualifiedName = "typename " + this.fullQualifiedName;
//				}
//			}
		}

		return this.fullQualifiedName;
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
	
	public List<CppInvokable> getAllInvokables() {
		List<CppInvokable> list = new ArrayList<CppInvokable>();

		list.addAll(this.constructorList);
		list.addAll(this.methodList);
		list.addAll(this.operatorList);

		return list;
	}

	public void getAllItems(List<Item> list) {
		list.addAll(this.constructorList);
		if(this.destructor != null) {
			list.add(this.destructor);
		}
		list.addAll(this.constantList);
		list.addAll(this.fieldList);
		list.addAll(this.methodList);
		list.addAll(this.enumList);
		list.addAll(this.operatorList);
		list.addAll(this.typedefList);

		for(DeferClass deferClass : this.classList) {
			list.add(deferClass.getCppClass());
			deferClass.getCppClass().getAllItems(list);
		}
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
			
		case Typedef:
			this.getTypedefList().add((Typedef)item);
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

		for(DeferClass deferClass : this.classList) {
			deferClass.getCppClass().fixupOwners();
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
		if(! this.isGlobal()) {
			return true;
		}
		else {
			return this.constructorList.size() > 0
					|| this.fieldList.size() > 0
					|| this.methodList.size() > 0
					|| this.enumList.size() > 0
					|| this.operatorList.size() > 0
					|| this.constantList.size() > 0
				;
		}
	}
	
	@Override
	protected void doGetPolicyRules(List<String> rules) {
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
			if(c.isPublic() && c.isTypeConverter() && ! c.isExplicit()) {
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
	
	public void resolveTypesForClass() {
		Map<String, String> typeMap = new HashMap<String, String>();
		Map<String, String> valueMap = new HashMap<String, String>();
		this.doResolveTypesForClass(typeMap, valueMap);
	}
	
	public void resolveParameterTypes(Map<String, String> typeMap, Map<String, String> valueMap) {
		this.doResolveTypesForInner(typeMap, valueMap, this.getConstructorList());
		this.doResolveTypesForInner(typeMap, valueMap, this.getMethodList());
		this.doResolveTypesForInner(typeMap, valueMap, this.getOperatorList());
	}
	
	private void doResolveTypesForClass(Map<String, String> typeMap, Map<String, String> valueMap) {
		this.doBuildInnerTypeMap(typeMap, null);
		this.doBuildInnerTypeMap(typeMap);
		this.doBuildInnerValueMap(typeMap, valueMap);
		
		this.resolveParameterTypes(typeMap, valueMap);

		for(DeferClass deferClass : this.getClassList()) {
			deferClass.getCppClass().doResolveTypesForClass(typeMap, valueMap);
		}
	}

	private <T extends CppInvokable> void doResolveTypesForInner(Map<String, String> typeMap, Map<String, String> valueMap, List<T> list) {
		for(T item : list) {
			this.doResolveTypeForInvokable(typeMap, valueMap, item);
		}
	}
	
	private void doResolveTypeForInvokable(Map<String, String> typeMap, Map<String, String> valueMap, CppInvokable item) {
		this.doResolveType(typeMap, item.getResultType());
		for(Parameter param : item.getParameterList()) {
			this.doResolveType(typeMap, param.getType());
			
			if(valueMap != null && param.hasDefaultValue()) {
				if(valueMap.containsKey(param.getDefaultValue())) {
					param.setDefaultValue(valueMap.get(param.getDefaultValue()));
				}
			}
		}
	}
	
	private void doResolveType(Map<String, String> typeMap, CppType type) {
		if(type != null && typeMap != null) {
			type.replaceToken(typeMap);
		}
	}
	
	private void doBuildInnerTypeMap(Map<String, String> typeMap, String outterName) {
		if(outterName != null) {
			outterName = outterName + "::" + this.getPrimaryName();
			typeMap.put(this.getPrimaryName(), outterName);
		}
		else {
			outterName = this.getPrimaryName();
		}
		
		for(DeferClass deferClass : this.getClassList()) {
			deferClass.getCppClass().doBuildInnerTypeMap(typeMap, outterName);
		}
	}
	
	private String normalizeOutterName(String outterName)
	{
		if(this.isTemplate()) {
			outterName = outterName + "<";
			boolean first = true;
			for(Parameter param : this.getTemplateParameterList()) {
				if(!first) {
					outterName = outterName + ", ";
				}
				first = false;
				outterName = outterName + param.getName();
			}
			outterName = outterName + " >";
		}

		return outterName;
	}
	
	private void doBuildInnerTypeMap(Map<String, String> typeMap) {
		String outterName = Util.getItemBaseName(this.getPrimaryName());
		outterName = Util.selectString(typeMap.get(outterName), outterName);
		
		outterName = this.normalizeOutterName(outterName);
		
		for(DeferClass deferClass : this.getClassList()) {
			deferClass.getCppClass().doBuildInnerTypeMap(typeMap);
		}
		
		String prefix;
		for(DeferClass deferClass : this.getClassList()) {
			String name = deferClass.getCppClass().getPrimaryName();
			prefix = "";
			if(this.isTemplate()) {
				if(deferClass.getCppClass().isTemplate()) {
					prefix = "template ";
				}
				else {
					prefix = "typename ";
				}
			}
			typeMap.put(name, prefix + outterName + "::" + name);
		}
		
		for(Typedef typedef : this.getTypedefList()) {
			String name = typedef.getPrimaryName();
			prefix = "";
			if(this.isTemplate()) {
				prefix = "typename ";
			}
			typeMap.put(name, prefix + outterName + "::" + name);
		}
		
		for(CppEnum cppEnum : this.getEnumList()) {
			String name = cppEnum.getPrimaryName();
			prefix = "";
			if(this.isTemplate()) {
				prefix = "typename ";
			}
			typeMap.put(name, prefix + outterName + "::" + name);
		}
	}
	
	private void doBuildInnerValueMap(Map<String, String> typeMap, Map<String, String> valueMap) {
		String outterName = Util.getItemBaseName(this.getPrimaryName());
		outterName = Util.selectString(typeMap.get(outterName), outterName);
		
		outterName = this.normalizeOutterName(outterName);
		
		for(DeferClass deferClass : this.getClassList()) {
			deferClass.getCppClass().doBuildInnerValueMap(typeMap, valueMap);
		}
		
		for(CppEnum cppEnum : this.getEnumList()) {
			for(EnumValue value : cppEnum.getValueList()) {
				String name = value.getName();
				valueMap.put(name, outterName + "::" + name);
			}
		}
	}
	
}
