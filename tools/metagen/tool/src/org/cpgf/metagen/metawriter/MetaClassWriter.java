package org.cpgf.metagen.metawriter;

import java.util.HashMap;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.Util;
import org.cpgf.metagen.codewriter.CppWriter;
import org.cpgf.metagen.metadata.Constant;
import org.cpgf.metagen.metadata.Constructor;
import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.CppEnum;
import org.cpgf.metagen.metadata.CppField;
import org.cpgf.metagen.metadata.CppMethod;
import org.cpgf.metagen.metadata.DeferClass;
import org.cpgf.metagen.metadata.EnumCategory;
import org.cpgf.metagen.metadata.EnumValue;
import org.cpgf.metagen.metadata.Item;
import org.cpgf.metagen.metadata.MetaInfo;
import org.cpgf.metagen.metadata.Operator;
import org.cpgf.metagen.metadata.ParameteredItem;
import org.cpgf.metagen.metawriter.callback.OutputCallbackData;


public class MetaClassWriter {
	private CppClass cppClass;
	private CppWriter codeWriter;
	private Config config;
	private MetaInfo metaInfo;
	
	private String define;
	private String classType;

	private OutputCallbackData callbackData;
	
	public MetaClassWriter(Config config, MetaInfo metaInfo, CppWriter codeWriter, CppClass cppClass) {
		this.initialize(config, metaInfo, codeWriter, cppClass, "_d", "D::ClassType");
	}

	public MetaClassWriter(Config config, MetaInfo metaInfo, CppWriter codeWriter, CppClass cppClass, String define, String classType) {
		this.initialize(config, metaInfo, codeWriter, cppClass, define, classType);
	}

	private void initialize(Config config, MetaInfo metaInfo, CppWriter codeWriter, CppClass cppClass, String define, String classType) {
		this.cppClass = cppClass;
		this.codeWriter = codeWriter;
		this.config = config;
		this.metaInfo = metaInfo;
		this.define = define;
		this.classType = classType;
	}
	
	private String getUniqueText()
	{
		if(this.cppClass.isGlobal()) {
			return "" + Util.getUniqueID(null);
		}
		else {
			return "" + Util.getUniqueID(this.cppClass.getLocation() + this.cppClass.getFullQualifiedName());
		}
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

	private void doCallback(Item item) {
		this.callbackData = this.metaInfo.getCallbackClassMap().getData(item);
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
		return this.skipItem() || ! Util.allowMetaData(this.config, item);
	}

	private boolean shouldSkipItem(ParameteredItem item)
	{
		return this.skipItem() || ! Util.allowMetaData(this.config, item) || item.isTemplate();
	}
	
	public void write() {
		if(this.allowedMetaData(EnumCategory.Constructor)) {
			this.writeConstructors();
		}
		
		if(this.allowedMetaData(EnumCategory.Field)) {
			this.writeFields();
		}
		
		if(this.allowedMetaData(EnumCategory.Method)) {
			this.writeMethods();
		}
		
		if(this.allowedMetaData(EnumCategory.Enum)) {
			this.writeEnumerators();
		}
		
		if(this.allowedMetaData(EnumCategory.Constant)) {
			this.writeConstants();
		}
		
		if(this.allowedMetaData(EnumCategory.Operator)) {
			this.writeOperators();
		}

		if(this.allowedMetaData(EnumCategory.Class)) {
			this.writeClasses();
		}
	}
	
	private void writeConstructors() {
		if(this.cppClass.isGlobal()) {
			return;
		}
		if(this.cppClass.isAbstract()) {
			return;
		}
		
		this.writeConstructorsBind();
	}

	public void writeConstructorsBind() {
		String action = WriterUtil.getReflectionAction(this.define, "_constructor");

		for(Constructor item : this.cppClass.getConstructorList()) {
			this.doCallback(item);

			if(this.shouldSkipItem(item)) {
				continue;
			}
			
			this.codeWriter.write(action + "<void * (");
			WriterUtil.writeParamList(this.codeWriter, item.getParameterList(), false);
			this.codeWriter.write(")>(" + WriterUtil.getPolicyText(item, false) + ")");
			
			WriterUtil.writeDefaultParams(this.codeWriter, item.getParameterList());
		}
	}

	private void writeFields() {
		String prefix = this.getScopePrefix();
		String action = WriterUtil.getReflectionAction(this.define, "_field");

		for(CppField item : this.cppClass.getFieldList()) {
			String name = item.getPrimaryName();
			
			this.doCallback(item);
			
			if(this.shouldSkipItem(item)) {
				continue;
			}

			if(name.indexOf('@') >= 0 || name.equals("")) { // anonymous union
				continue;
			}
			
			if(item.isBitField()) {
				CppField field = (CppField)(item);
				if(WriterUtil.shouldGenerateBitfieldWrapper(this.config, field)) {
					this.codeWriter.writeLine(WriterUtil.getReflectionAction(this.define, "_property") + "(" + Util.quoteText(name)
							+ ", &" + WriterUtil.getBitfieldWrapperGetterName(field)
							+ ", &" + WriterUtil.getBitfieldWrapperSetterName(field)
							+ ", cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());"
							);
				}
			}
			else {
				this.codeWriter.write(action);
				this.codeWriter.write("(" + Util.quoteText(name) + ", ");
				this.codeWriter.writeLine("&" + prefix + name + WriterUtil.getPolicyText(item) + ");");
			}
		}
	}

	private void writeMethods() {
		String scopePrefix = this.getScopePrefix();

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
			
			WriterUtil.reflectMethod(this.codeWriter, this.define, scopePrefix, item, name, name, overload);
		}
	}

	private void writeEnumerators() {
		String typePrefix = this.getScopePrefix("typename ");
		String prefix = this.getScopePrefix();
		String action = WriterUtil.getReflectionAction(this.define, "_enum");

		for(CppEnum item : this.cppClass.getEnumList()) {
			String name = item.getPrimaryName();
			
			this.doCallback(item);

			if(this.shouldSkipItem(item)) {
				continue;
			}

			String typeName = typePrefix + name;

			if(name.indexOf('@') >= 0 || name.equals("")) {
				name = "GlobalEnum_"  + this.config.projectID + "_" + this.getUniqueText();
				typeName = "long long";
			}

			this.codeWriter.writeLine(action + "<" + typeName + ">(" + Util.quoteText(name) + ")");
			this.codeWriter.incIndent();
				for(EnumValue value : item.getValueList()) {
					this.codeWriter.writeLine("._element(" + Util.quoteText(value.getName()) + ", " + prefix + value.getQualifiedName() + ")");
				}
			this.codeWriter.decIndent();
			this.codeWriter.writeLine(";");
		}
	}

	private void writeConstants() {
		String action = WriterUtil.getReflectionAction(this.define, "_enum");

		boolean haveItems = false;

		for(Constant item : this.cppClass.getConstantList()) {
			this.doCallback(item);
			
			if(this.shouldSkipItem(item)) {
				continue;
			}

			if (!haveItems) {
		        this.codeWriter.writeLine(action + "<long long>(" + Util.quoteText("GlobalDefine_" + this.config.projectID + "_" + this.getUniqueText()) + ")");
        		this.codeWriter.incIndent();
                haveItems = true;
			}
			
			String value = item.getValue();
			if(value == null || value.equals("")) {
				continue;
			}
			
			this.codeWriter.writeLine("._element(" + Util.quoteText(item.getPrimaryName()) + ", " + item.getPrimaryName() + ")");
		}
	    if (haveItems) {
		    this.codeWriter.decIndent();
		    this.codeWriter.writeLine(";");
        }
	}

	private void writeOperators() {
		for(Operator item : this.cppClass.getOperatorList()) {
			this.doCallback(item);
			
			if(this.shouldSkipItem(item)) {
				continue;
			}
			
			OperatorWriter opWriter = new OperatorWriter(this.metaInfo, item);
			opWriter.writeReflectionCode(this.codeWriter, this.define);
			
			if(WriterUtil.shouldGenerateOperatorWrapper(this.metaInfo, item)) {
				opWriter.writeNamedWrapperReflectionCode(this.codeWriter, define);
			}
		}
	}

	private void writeClasses() {
		String action = WriterUtil.getReflectionAction(this.define, "_class");

		for(DeferClass deferClass : this.cppClass.getClassList()) {
			CppClass item = deferClass.getCppClass();
			this.doCallback(item);
			
			if(this.shouldSkipItem(item)) {
				continue;
			}
			
			this.codeWriter.beginBlock();
			
			WriterUtil.defineMetaClass(this.config, this.codeWriter, item, "_nd", "declare");
			MetaClassWriter classWriter = new MetaClassWriter(
				this.config,
				this.metaInfo,
				this.codeWriter,
				item,
				"_nd",
				item.getFullQualifiedName()
			);
			classWriter.write();
			this.codeWriter.writeLine(action + "(_nd);");
			
			this.codeWriter.endBlock();
		}
	}

}
