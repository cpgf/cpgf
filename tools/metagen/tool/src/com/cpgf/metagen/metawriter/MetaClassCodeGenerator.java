package com.cpgf.metagen.metawriter;

import java.util.List;

import com.cpgf.metagen.Config;
import com.cpgf.metagen.Util;
import com.cpgf.metagen.codewriter.CppWriter;
import com.cpgf.metagen.metadata.CppClass;
import com.cpgf.metagen.metadata.CppInvokable;
import com.cpgf.metagen.metadata.Item;
import com.cpgf.metagen.metadata.MetaInfo;
import com.cpgf.metagen.metadata.Parameter;
import com.cpgf.metagen.metadata.TemplateInstance;
import com.cpgf.metagen.metawriter.callback.OutputCallbackData;

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

	private String getGlobalPostfix() {
		String g = "global_" + this.getFileNameSymbol();

		return g.toLowerCase();
	}

	private String getFileNameSymbol()
	{
		String s = Util.getBaseFileName(this.sourceFileName);

		return Util.normalizeSymbol(s);
	}

	private String createFunctionName(CppClass cppClass, String prefix) {
		String className = this.getGlobalPostfix();
		if(! cppClass.isGlobal()) {
			className = cppClass.getPrimaryName();
		}
		className = Util.upcaseFirst(className);
			
		return prefix + className;
	}

	private void beginMetaFunction(CppWriter codeWriter, String name, CppClass cppClass) {
		codeWriter.write("template <typename D");
		if(cppClass.isTemplate()) {
			for(Parameter param : cppClass.getTemplateParameterList()) {
				codeWriter.write(", typename " + param.getName());
			}
		}
		codeWriter.write(">\n");
		codeWriter.write("void " + name + "(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)\n");
		codeWriter.beginBlock();
		codeWriter.write("(void)config; (void)_d; (void)_r; (void)_d;\n");
		codeWriter.useNamespace("cpgf");
		codeWriter.write("\n");
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
	
	private void generateClassReflectionHeaderCode() {
		CppWriter codeWriter = new CppWriter();

		MetaClassWriter classWriter = new MetaClassWriter(this.config, this.metaInfo, codeWriter, cppClass);
		
		String funcName = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);

		if(this.callbackData.getHeaderCode() != null) {
			codeWriter.write(this.callbackData.getHeaderCode() + "\n\n");
		}

		this.beginMetaFunction(codeWriter, funcName, cppClass);
		classWriter.write();
		
		codeWriter.writeMultipleLines(this.extraHeaderCodeInClass);

		this.endMetaFunction(codeWriter);
		
		if(this.callbackData.wrapClass()) {
			codeWriter.write("\n\n");
			this.wrapperWriter = new ClassWrapperWriter(this.config, this.callbackData.getWrapperConfig(), codeWriter, this.cppClass);
			this.wrapperWriter.writeClassWrapper();
		}
		
		this.classCode.headerCode = this.appendText(this.classCode.headerCode, codeWriter.getText());
	}
	
	private void generateClassReflectionSourceCode() {
		List<TemplateInstance> templateInstanceList = null;
		
		if(cppClass.isTemplate()) {
			templateInstanceList = this.metaInfo.findTemplateInstances(cppClass);
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

		if(this.wrapperWriter != null) {
			this.wrapperWriter.writeStaticInitializer(codeWriter);
			codeWriter.writeLine("");
		}

		codeWriter.write("GDefineMetaInfo " + funcName + "()\n");

		codeWriter.beginBlock();

		String callFunc = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);

		WriterUtil.createMetaClass(codeWriter, cppClass, callFunc, templateInstanceList);

		if(this.wrapperWriter != null) {
			codeWriter.beginBlock();
			this.wrapperWriter.setCodeWriter(codeWriter);
			this.wrapperWriter.writeCreation(callFunc);
			codeWriter.endBlock();
		}

		codeWriter.write("return _d.getMetaInfo();\n");
		
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

		if(this.config.scriptable) {
			this.generateClassCallbackCode();
		}

		this.generateClassReflectionHeaderCode();
		this.generateClassReflectionSourceCode();
	}
	
	public MetaClassCode generateClassMetaCode() {
		this.extraHeaderCodeInClass = "";

		this.doGenerateClassMetaCode();

		return this.classCode;
	}

}
