package com.cpgf.metagen.metawriter;

import java.util.HashMap;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.codewriter.CppWriter;
import com.cpgf.metagen.metadata.Constant;
import com.cpgf.metagen.metadata.Constructor;
import com.cpgf.metagen.metadata.CppClass;
import com.cpgf.metagen.metadata.CppEnum;
import com.cpgf.metagen.metadata.CppField;
import com.cpgf.metagen.metadata.CppMethod;
import com.cpgf.metagen.metadata.DeferClass;
import com.cpgf.metagen.metadata.EnumCategory;
import com.cpgf.metagen.metadata.EnumValue;
import com.cpgf.metagen.metadata.EnumVisibility;
import com.cpgf.metagen.metadata.Item;
import com.cpgf.metagen.metadata.Operator;
import com.cpgf.metagen.metadata.ParameteredItem;
import com.cpgf.metagen.metawriter.callback.IOutputCallback;
import com.cpgf.metagen.metawriter.callback.OutputCallbackData;

public class MetaClassWriter {
	private CppClass cppClass;
	private CppWriter codeWriter;
	private Config config;
	
	private String define;
	private String classType;

	private OutputCallbackData callbackData;
	
	public MetaClassWriter(Config config, CppWriter codeWriter, CppClass cppClass) {
		this.initialize(cppClass, codeWriter, config, "_d", "D::ClassType");
	}

	public MetaClassWriter(Config config, CppWriter codeWriter, CppClass cppClass, String define, String classType) {
		this.initialize(cppClass, codeWriter, config, define, classType);
	}
	
	private void initialize(CppClass cppClass, CppWriter codeWriter, Config config, String define, String classType) {
		this.cppClass = cppClass;
		this.codeWriter = codeWriter;
		this.config = config;
		this.define = define;
		this.classType = classType;
	}
	
	private String getScopePrefix() {
		return this.getScopePrefix(null);
	}
	
	private String getScopePrefix(String prefix) {
		if(prefix == null) {
			prefix = "";
		}

		if(this.cppClass.isGlobal()) {
			return "";
		}
		else {
			return prefix + this.classType + "::";
		}
	}

	private boolean canWrite(Item item) {
		return this.config.allowPublic && item.getVisibility() == EnumVisibility.Public
				|| this.config.allowProtected && item.getVisibility() == EnumVisibility.Protected
				|| this.config.allowPrivate && item.getVisibility() == EnumVisibility.Private
		;
	}

	private String getAction(String name) {
		return this.define + ".CPGF_MD_TEMPLATE " + name;
	}

	private String getReplace(String name) {
		return "replaceName(\"" + name + "\", _r)";
	}

	private void doCallback(Item item) {
		IOutputCallback callback = this.config.metaOutputCallback;

		if(callback != null) {
			this.callbackData = new OutputCallbackData();
			callback.outputCallback(item, this.callbackData);
		}
		else {
			if(this.callbackData == null) {
				this.callbackData = new OutputCallbackData();
			}
		}
	}

	private boolean skipItem() {
		return this.callbackData.isSkipBind();
	}

	private boolean allowedMetaData(EnumCategory category) {
		for(EnumCategory c : this.config.allowedMetaData) {
			if(c == category) {
				return true;
			}
		}
		
		return false;
	}
	
	private boolean shouldSkipItem(Item item)
	{
		return this.skipItem() || ! this.canWrite(item);
	}

	private boolean shouldSkipItem(ParameteredItem item)
	{
		return this.skipItem() || ! this.canWrite(item) || item.isTemplate();
	}
	
	public void write() {
		if(this.allowedMetaData(EnumCategory.Constructor)) {
			this.writeConstructor();
		}
		
		if(this.allowedMetaData(EnumCategory.Field)) {
			this.writeField();
		}
		
		if(this.allowedMetaData(EnumCategory.Method)) {
			this.writeMethod();
		}
		
		if(this.allowedMetaData(EnumCategory.Enum)) {
			this.writeEnum();
		}
		
		if(this.allowedMetaData(EnumCategory.Constant)) {
			this.writeConstant();
		}
		
		if(this.allowedMetaData(EnumCategory.Operator)) {
			this.writeOperator();
		}

		if(this.allowedMetaData(EnumCategory.Class)) {
			this.writeClass();
		}
	}
	
	private void writeConstructor() {
		String action = this.getAction("_constructor");

		if(this.cppClass.isGlobal()) {
			return;
		}
		if(this.cppClass.isAbstract()) {
			return;
		}

		for(Constructor item : this.cppClass.getConstructorList()) {
			this.doCallback(item);

			if(this.shouldSkipItem(item)) {
				continue;
			}
			
			this.codeWriter.out(action + "<void * (");
			WriterUtil.writeParamList(this.codeWriter, item.getParameterList(), false);
			this.codeWriter.out(")>(" + WriterUtil.getPolicyText(item, false) + ")");
			
			WriterUtil.writeDefaultParams(this.codeWriter, item.getParameterList());
		}
	}

	private void writeField() {
		String prefix = this.getScopePrefix();
		String action = this.getAction("_field");

		for(CppField item : this.cppClass.getFieldList()) {
			String name = item.getPrimaryName();
			
			this.doCallback(item);
			
			if(this.shouldSkipItem(item)) {
				continue;
			}

			if(name.indexOf('@') >= 0 || name.equals("")) { // anonymous union
				continue;
			}
			
			this.codeWriter.out(action);
			this.codeWriter.out("(" + this.getReplace(name) + ", ");
			this.codeWriter.out("&" + prefix + name + WriterUtil.getPolicyText(item) + ");\n");
		}
	}

	private void writeMethod() {
		String prefix = this.getScopePrefix();
		String action = this.getAction("_method");

		HashMap<String, Integer> methodOverload = new HashMap<String, Integer>();

		for(CppMethod item : this.cppClass.getMethodList()) {
			String name = item.getPrimaryName();
			Integer count = methodOverload.get(name);
			if(count == null) {
				count = new Integer(0);
			}
			count = new Integer(count.intValue() + 1);
			methodOverload.put(name, count);
		}

		for(CppMethod item : this.cppClass.getMethodList()) {
			String name = item.getPrimaryName();
			Integer overloadCount = methodOverload.get(name);
			boolean overload = (overloadCount != null && overloadCount.intValue() > 1);

			this.doCallback(item);
			
			if(this.shouldSkipItem(item)) {
				continue;
			}
			
			overload = overload || this.cppClass.isGlobal();

			this.codeWriter.out(action);
			this.codeWriter.out("(" + this.getReplace(name) + ", ");
			if(overload) {
				String typePrefix = prefix;
				if(item.isStatic()) {
					typePrefix = "";
				}
				this.codeWriter.out("(" + item.getResultType().getLiteralType() + " (" + typePrefix + "*) (");
				WriterUtil.writeParamList(this.codeWriter, item.getParameterList(), false);
				this.codeWriter.out(")");
				if(!item.isStatic() && item.isConst()) {
					this.codeWriter.out(" const");
				}
				this.codeWriter.out(")");
			}
			this.codeWriter.out("&" + prefix + name + WriterUtil.getPolicyText(item) + ")");

			WriterUtil.writeDefaultParams(this.codeWriter, item.getParameterList());
		}
	}

	private void writeEnum() {
		String typePrefix = this.getScopePrefix("typename ");
		String prefix = this.getScopePrefix();
		String action = this.getAction("_enum");

		for(CppEnum item : this.cppClass.getEnumList()) {
			String name = item.getPrimaryName();
			
			this.doCallback(item);

			if(this.shouldSkipItem(item)) {
				continue;
			}

			String typeName = typePrefix + name;

			if(name.indexOf('@') >= 0 || name.equals("")) {
				name = "GlobalEnum_"  + this.config.projectID + "_" + Util.getUniqueID();
				typeName = "long long";
			}

			this.codeWriter.out(action + "<" + typeName + ">(" + this.getReplace(name) + ")\n");
			this.codeWriter.incIndent();
				for(EnumValue value : item.getValueList()) {
					this.codeWriter.out("._element(" + this.getReplace(value.getName()) + ", " + prefix + value.getQualifiedName() + ")\n");
				}
			this.codeWriter.decIndent();
			this.codeWriter.out(";\n");
		}
	}

	private void writeConstant() {
		String action = this.getAction("_enum");

		if(this.cppClass.getConstantList().size() == 0) {
			return;
		}

		this.codeWriter.out(action + "<long long>(" + this.getReplace("GlobalDefine_" + this.config.projectID + "_" + Util.getUniqueID()) + ")\n");
		this.codeWriter.incIndent();

		for(Constant item : this.cppClass.getConstantList()) {
			this.doCallback(item);
			
			if(this.shouldSkipItem(item)) {
				continue;
			}
			
			String value = item.getValue();
			if(value == null || value.equals("")) {
				continue;
			}
			
			this.codeWriter.out("._element(" + this.getReplace(item.getPrimaryName()) + ", " + item.getPrimaryName() + ")\n");
		}
		
		this.codeWriter.decIndent();
		this.codeWriter.out(";\n");
	}

	private void writeOperator() {
		String action = this.getAction("_operator");

		for(Operator item : this.cppClass.getOperatorList()) {
			this.doCallback(item);
			
			if(this.shouldSkipItem(item)) {
				continue;
			}

			boolean isStatic = item.isStatic();
			String op = item.getOperator();
			
			int realParamCount = item.getParameterList().size();
			if(! isStatic) {
				++realParamCount;
			}
			
			if(op.equals("->")) {
				realParamCount = 2;
			}

			boolean isTypeConvert = op.matches(".*\\w+.*"); // && realParamCount == 1; // type convert T()
			String opText = "";
			
			if(isTypeConvert) {
				this.codeWriter.out(action + "<" + op + " (GMetaSelf)>(");
				opText = "H()";
			}
			else {
				this.codeWriter.out(action + "<" + item.getResultType().getLiteralType() + " (*)(");
				
				boolean isFunctor = item.isFunctor();
				boolean hasSelf = item.hasSelf();

				if(hasSelf) {
					if(item.isConst()) {
						this.codeWriter.out("const cpgf::GMetaSelf &");
					}
					else {
						this.codeWriter.out("cpgf::GMetaSelf");
					}
				}

				if(op.equals("++") || op.equals("--")) {
				}
				else {
					if(item.hasParameter() && hasSelf) {
						this.codeWriter.out(", ");
					}
					WriterUtil.writeParamList(this.codeWriter, item.getParameterList(), false);
				}
				this.codeWriter.out(")>(");
				if(isFunctor) {
					opText = "H(H)";
				}
				else if(op.equals("[]")) {
					opText = "H[0]";
				}
				else if(op.matches("\\w") && realParamCount == 1) { // type convert T()
					opText = "H()";
				}
				else {
					if(realParamCount == 2) {
						if(op.equals("++") || op.equals("--")) {
							opText = "H" + op;
						}
						else {
							opText = "H " + op + " H";
						}
					}
					else if(realParamCount == 1) {
						opText = op + "H";
					}
					else {
					}
				}
			}

			opText = opText.replaceAll("\\bH\\b", "mopHolder");
			this.codeWriter.out(opText);
			this.codeWriter.out(WriterUtil.getPolicyText(item) + ")");

			WriterUtil.writeDefaultParams(this.codeWriter, item.getParameterList());
		}
	}

	private void writeClass() {
		String action = this.getAction("_class");

		for(DeferClass deferClass : this.cppClass.getClassList()) {
			CppClass item = deferClass.getCppClass();
			this.doCallback(item);
			
			if(this.shouldSkipItem(item)) {
				continue;
			}
			
			this.codeWriter.beginBlock();
			
			WriterUtil.defineMetaClass(this.config, this.codeWriter, item, "_nd", "declare");
			MetaClassWriter writer = new MetaClassWriter(
				this.config,
				this.codeWriter,
				item,
				"_nd",
				item.getLiteralName()
			);
			writer.write();
			this.codeWriter.out(action + "(_nd);\n");
			
			this.codeWriter.endBlock();
		}
	}

}
