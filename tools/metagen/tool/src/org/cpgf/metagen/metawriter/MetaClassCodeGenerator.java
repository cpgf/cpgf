package org.cpgf.metagen.metawriter;

import java.util.List;

import org.cpgf.metagen.Config;
import org.cpgf.metagen.Util;
import org.cpgf.metagen.codewriter.CppWriter;
import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.CppField;
import org.cpgf.metagen.metadata.CppInvokable;
import org.cpgf.metagen.metadata.DeferClass;
import org.cpgf.metagen.metadata.Item;
import org.cpgf.metagen.metadata.MetaInfo;
import org.cpgf.metagen.metadata.Operator;
import org.cpgf.metagen.metadata.Parameter;
import org.cpgf.metagen.metadata.TemplateInstance;
import org.cpgf.metagen.metawriter.callback.OutputCallbackData;


public class MetaClassCodeGenerator {
	private Config config;
	private MetaInfo metaInfo;
	private CppClass cppClass;
	private OutputCallbackData callbackData;
	private MetaClassCode classCode;
	private String sourceFileName;
	
	private String extraHeaderCodeInClass;
	
	private ClassWrapperWriter wrapperWriter;

	public MetaClassCodeGenerator(Config config, MetaInfo metaInfo, CppClass cppClass, String sourceFileName) {
		this.config = config;
		this.metaInfo = metaInfo;
		this.cppClass = cppClass;
		this.sourceFileName = sourceFileName;
		this.classCode = new MetaClassCode();
	}

	private void doCallback(Item item) {
		this.callbackData = this.metaInfo.getCallbackClassMap().getData(item);
	}

	private String createFunctionName(String cppClassName, boolean isGlobal, String prefix) {
		return WriterUtil.createFunctionName(cppClassName, sourceFileName, isGlobal, prefix);
	}

	private String createFunctionName(CppClass cppClass, String prefix) {
		return this.createFunctionName(cppClass.getPrimaryName(), cppClass.isGlobal(), prefix);
	}

	private void beginMetaFunction(CppWriter codeWriter, String name, CppClass cppClass) {
		codeWriter.write("template <typename D");
		if(cppClass.isTemplate()) {
			for(Parameter param : cppClass.getTemplateParameterList()) {
				codeWriter.write(", " + param.getType().getLiteralType() + " " + param.getName());
			}
		}
		codeWriter.writeLine(">");
		codeWriter.writeLine("void " + name + "(const cpgf::GMetaDataConfigFlags & config, D _d)");
		codeWriter.beginBlock();
		codeWriter.writeLine("(void)config; (void)_d; (void)_d;");
		codeWriter.useNamespace("cpgf");
		codeWriter.writeLine("");
	}

	private void endMetaFunction(CppWriter codeWriter) {
		codeWriter.endBlock();
	}
	
	private String appendText(String text, String append) {
		if(text.length() > 0 && append.length() > 0) {
			text = text + "\n\n";
		}
		return text + append;
	}

	private String doGenerateCallbackWrapperPrototype(CppInvokable invokable, String name) {
		String result = "";
		
		if(invokable.isConstructor()) {
			result = "void *";
		}
		else if(invokable.getResultType() != null) {
			result = result + invokable.getResultType().getLiteralType();
		}
		else {
			result = result + "void";
		}
		
		result = result + " " + name + "(";
		
		boolean first = true;
		
		if(! invokable.getOwner().isGlobal() && invokable.isMethod()) {
			result = result + invokable.getOwner().getLiteralName() + " * self";
			first = false;
		}
		
		int callbackIndex = -1;
		for(Parameter param : invokable.getParameterList()) {
			if(first) {
				first = false;
			}
			else {
				result = result + ", ";
			}

			if(param.getCallback() != null) {
				++callbackIndex;
				result = result + "cpgf::IScriptFunction * scriptFunction" + callbackIndex;
			}
			else {
				result = result + param.getType().getLiteralType() + " " + param.getName();
			}
		}
		
		result = result + ")";

		return result;
	}
	
	private String doGenerateCallbackReflect(CppInvokable invokable, String name) {
		String result = "";
		
		if(invokable.isConstructor()) {
			result = result + "_d.CPGF_MD_TEMPLATE _constructor(&" + name + ");";
		}
		
		return result;
	}
	
	private String getClassCallbackUniqueName(CppInvokable invokable, int index) {
		String uniqueName = invokable.getOwner().getPrimaryName();
		if(invokable.isMethod()) {
			uniqueName = uniqueName + "_" + invokable.getPrimaryName();
		}
		else if(invokable.isConstructor()) {
			uniqueName = uniqueName + "_Constructor";
		}
		uniqueName = uniqueName + "_" + index;
		
		return uniqueName;
	}
	
	final private static String CallbackWrapperPrefix = "callbackWrapper_";
	private String doGenerateCallbackWrapperImplementation(CppInvokable invokable, int index) {
		String result = "";

		String uniqueName = this.getClassCallbackUniqueName(invokable, index);
		int callbackIndex = -1;
		
		for(Parameter param : invokable.getParameterList()) {
			if(param.getCallback() == null) {
				continue;
			}
			++callbackIndex;
			String callbackName = "callback_" + uniqueName + "_p" + callbackIndex;
result = result + "static IScriptFunction * xxx = NULL;\n"; //temp			
			result = result + Util.getInvokablePrototype(param.getCallback(), callbackName) + "\n";
			result = result + "{\n";
			String body = Util.getParameterText(param.getCallback().getParameterList(), false, true);
			if(body.length() > 0) {
				body = ", " + body;
			}
			body = "    invokeScriptFunction(" + "xxx" + body + ");\n"; 
			result = result + body;
			result = result + "}\n";
			result = result + "\n";

			String wrapperName = CallbackWrapperPrefix + uniqueName;
			String wrapperPrototype = this.doGenerateCallbackWrapperPrototype(invokable, wrapperName);
			result = result + wrapperPrototype + "\n";
			result = result + "{\n";
			result = result + "xxx = scriptFunction0; \n";
			result = result + "";
			result = result + "}\n";
			result = result + "\n";
		}
		
		return result;
	}
	
	private void doGenerateClassCallbackCode(CppInvokable invokable, int index) {
		String uniqueName = this.getClassCallbackUniqueName(invokable, index);
		
		String wrapperName = CallbackWrapperPrefix + uniqueName;
		String wrapperPrototype = this.doGenerateCallbackWrapperPrototype(invokable, wrapperName);

		this.classCode.headerCode = this.appendText(this.classCode.headerCode, wrapperPrototype + ";\n");
		
		this.extraHeaderCodeInClass = this.extraHeaderCodeInClass + this.doGenerateCallbackReflect(invokable, wrapperName) + "\n";
		
		this.classCode.sourceCode = this.appendText(this.classCode.sourceCode, this.doGenerateCallbackWrapperImplementation(invokable, index));
	}
	
	private void generateClassCallbackCode() {
		List<CppInvokable> invokableList = this.cppClass.getAllInvokables();
		
		int index = -1;
		
		for(CppInvokable invokable : invokableList) {
			if(! invokable.hasCallbackParameter()) {
				continue;
			}
			
			++index;
			this.doGenerateClassCallbackCode(invokable, index);
		}
	}
	
	private void generateBitfieldsWrapperFunctions(CppClass cls) {
		List<CppField> fieldList = cls.getFieldList();
		CppWriter codeWriter = new CppWriter();
		for(CppField field : fieldList) {
			if(! WriterUtil.shouldGenerateBitfieldWrapper(this.metaInfo, field)) {
				continue;
			}
			
			// getter
			codeWriter.write("inline " + field.getType().getLiteralType() + " ");
			codeWriter.write(WriterUtil.getBitfieldWrapperGetterName(field) + "(" + field.getOwner().getQualifiedName() + " * self) ");
			codeWriter.beginBlock();
			codeWriter.writeLine("return self->" + field.getLiteralName() + ";");
			codeWriter.endBlock();
			
			codeWriter.writeLine("");

			// setter
			codeWriter.write("inline void ");
			codeWriter.write(WriterUtil.getBitfieldWrapperSetterName(field) + "(" + field.getOwner().getQualifiedName() + " * self, ");
			codeWriter.write(field.getType().getLiteralType() + " value) ");
			codeWriter.beginBlock();
			codeWriter.writeLine("self->" + field.getLiteralName() + " = value;");
			codeWriter.endBlock();
		}
		this.classCode.headerCode = this.appendText(this.classCode.headerCode, codeWriter.getText());
		
		for(DeferClass innerClass : cls.getClassList()) {
			generateBitfieldsWrapperFunctions(innerClass.getCppClass());
		}
	}
	
	private void generateOperatorWrapperFunctions(CppClass cls) {
		List<Operator> operatorList = cls.getOperatorList();
		CppWriter codeWriter = new CppWriter();
		for(Operator item : operatorList) {
			if(! WriterUtil.shouldGenerateOperatorWrapper(this.metaInfo, item)) {
				continue;
			}
			
			OperatorWriter opWriter = new OperatorWriter(this.metaInfo, item);
			opWriter.writeNamedWrapperFunctionCode(codeWriter);
		}
		this.classCode.headerCode = this.appendText(this.classCode.headerCode, codeWriter.getText());
		
		for(DeferClass innerClass : cls.getClassList()) {
			generateOperatorWrapperFunctions(innerClass.getCppClass());
		}
	}
	
	private void generateClassReflectionHeaderCode() {
		CppWriter codeWriter = new CppWriter();

		MetaClassWriter classWriter = new MetaClassWriter(this.config, this.metaInfo, codeWriter, this.cppClass);
		
		String funcName = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);

		if(this.callbackData.getHeaderCode() != null) {
			codeWriter.write(this.callbackData.getHeaderCode() + "\n\n");
		}

		this.beginMetaFunction(codeWriter, funcName, cppClass);
		classWriter.write();
		
		codeWriter.writeMultipleLines(this.extraHeaderCodeInClass);

		this.endMetaFunction(codeWriter);
		
		if(this.callbackData.wrapClass()) {
			codeWriter.writeLine("");
			codeWriter.writeLine("");
			
			this.writeClassWrapper(codeWriter);
		}
		
		this.classCode.headerCode = this.appendText(this.classCode.headerCode, codeWriter.getText());
	}
	
	private void writeClassWrapper(CppWriter codeWriter) {
		this.wrapperWriter = new ClassWrapperWriter(this.config, this.callbackData.getWrapperConfig(), this.cppClass);
		this.wrapperWriter.writeClassWrapper(codeWriter);
		
		codeWriter.writeLine("");
		codeWriter.writeLine("");

		MetaClassWriter classWriter = new MetaClassWriter(this.config, this.metaInfo, codeWriter, this.cppClass);
		
		String funcName = this.createFunctionName(this.wrapperWriter.getWrapperName(), false, this.config.metaClassFunctionPrefix);
		
		this.beginMetaFunction(codeWriter, funcName, cppClass);

		if(this.cppClass.isAbstract()) {
			classWriter.writeConstructorsBind();
		}

		codeWriter.writeLine("");
		
		this.wrapperWriter.writeSuperMethodBind(codeWriter);
		codeWriter.writeLine("");

		String callFuncName = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);
		codeWriter.write(callFuncName + "<D");
		if(cppClass.isTemplate()) {
			for(Parameter param : cppClass.getTemplateParameterList()) {
				codeWriter.write(", " + param.getName());
			}
		}
		codeWriter.writeLine(">(config, _d);");

		this.endMetaFunction(codeWriter);
	}
	
	private void generateClassReflectionSourceCode() {
		List<TemplateInstance> templateInstanceList = null;
		
		if(cppClass.isTemplate()) {
//			templateInstanceList = this.metaInfo.findTemplateInstances(cppClass);
			if(templateInstanceList == null) {
				return;
			}
		}
		
		CppWriter codeWriter = new CppWriter();

		String funcName = this.createFunctionName(cppClass, this.config.metaClassCreatePrefix);
		this.classCode.createFunctionName = funcName;

		if(this.callbackData.getSourceCode() != null) {
			codeWriter.write(this.callbackData.getSourceCode() + "\n\n");
		}

		codeWriter.writeLine("GDefineMetaInfo " + funcName + "()");

		codeWriter.beginBlock();

		String callFunc = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);

		WriterUtil.createMetaClass(codeWriter, cppClass, callFunc, templateInstanceList);

		if(this.wrapperWriter != null) {
			codeWriter.beginBlock();
			String wrapperFuncName = this.createFunctionName(this.wrapperWriter.getWrapperName(), false, this.config.metaClassFunctionPrefix);
			this.wrapperWriter.writeCreation(codeWriter, wrapperFuncName);
			codeWriter.endBlock();
		}

		codeWriter.writeLine("return _d.getMetaInfo();");
		
		codeWriter.endBlock();

		this.classCode.sourceCode = this.appendText(this.classCode.sourceCode, codeWriter.getText());
	}
	
	private void doGenerateClassMetaCode() {
		this.doCallback(cppClass);

		if(this.callbackData.isSkipBind()) {
			return;
		}
		
		if(! this.cppClass.canGenerateMetaCode()) {
			return;
		}

		if(this.config.wrapCallback) {
			this.generateClassCallbackCode();
		}
		
		this.generateBitfieldsWrapperFunctions(this.cppClass);
		this.generateOperatorWrapperFunctions(this.cppClass);

		this.generateClassReflectionHeaderCode();
		this.generateClassReflectionSourceCode();
	}
	
	public MetaClassCode generateClassMetaCode() {
		this.extraHeaderCodeInClass = "";

		this.doGenerateClassMetaCode();

		return this.classCode;
	}

}
