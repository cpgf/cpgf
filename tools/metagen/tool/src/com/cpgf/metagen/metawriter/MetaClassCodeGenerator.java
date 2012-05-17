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
import com.cpgf.metagen.metawriter.callback.IOutputCallback;
import com.cpgf.metagen.metawriter.callback.OutputCallbackData;

public class MetaClassCodeGenerator {
	private Config config;
	private MetaInfo metaInfo;
	private CppClass cppClass;
	private OutputCallbackData callbackData;
	private MetaClassCode classCode;
	private String sourceFileName;
	
	private String extraHeaderCodeInClass;

	public MetaClassCodeGenerator(Config config, MetaInfo metaInfo, CppClass cppClass, String sourceFileName) {
		this.config = config;
		this.metaInfo = metaInfo;
		this.cppClass = cppClass;
		this.sourceFileName = sourceFileName;
		this.classCode = new MetaClassCode();
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
		
		if(invokable.getResultType() != null) {
			result = result + invokable.getResultType().getLiteralType();
		}
		else {
			result = result + "void";
		}
		
		result = result + " " + name + "(";
		
		boolean first = true;
		
		if(! invokable.getOwner().isGlobal()) {
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
				result = result + "IScriptFunction * scriptFunction" + callbackIndex;
			}
			else {
				result = result + param.getType().getLiteralType() + " " + param.getName();
			}
		}
		
		result = result + ")";

		return result;
	}
	
	private void doGenerateClassCallbackCode(CppInvokable invokable, int index) {
		String uniqueName = invokable.getOwner().getPrimaryName();
		if(invokable.isMethod()) {
			uniqueName = uniqueName + "_" + invokable.getPrimaryName();
		}
		else if(invokable.isConstructor()) {
			uniqueName = uniqueName + "_Constructor";
		}
		uniqueName = uniqueName + "_" + index;
		
		String wrapperName = "callbackWrapper_" + uniqueName;
		String callbackName = "callback_" + uniqueName;
		
		String wrapperPrototype = this.doGenerateCallbackWrapperPrototype(invokable, wrapperName);
		
		this.classCode.headerCode = this.classCode.headerCode + wrapperPrototype + ";\n"; 
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

		MetaClassWriter classWriter = new MetaClassWriter(this.config, codeWriter, cppClass);
		
		String funcName = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);

		if(this.callbackData.getHeaderCode() != null) {
			codeWriter.write(this.callbackData.getHeaderCode() + "\n\n");
		}

		this.beginMetaFunction(codeWriter, funcName, cppClass);
		classWriter.write();
		
		codeWriter.writeLines(this.extraHeaderCodeInClass);
		
		this.endMetaFunction(codeWriter);
		
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

		codeWriter.write("GDefineMetaInfo " + funcName + "()\n");
		
		codeWriter.beginBlock();

		String callFunc = this.createFunctionName(cppClass, this.config.metaClassFunctionPrefix);
		WriterUtil.createMetaClass(codeWriter, cppClass, callFunc, templateInstanceList);
		
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

		// must process header first because the JS script will be applied
		this.generateClassReflectionHeaderCode();
		this.generateClassReflectionSourceCode();
		
		MetaClassCode tempCode = new MetaClassCode(this.classCode);
		this.classCode.headerCode = "";
		this.classCode.sourceCode = "";

//		this.generateClassCallbackCode();

		this.classCode.headerCode = this.appendText(this.classCode.headerCode, tempCode.headerCode);
		this.classCode.sourceCode = this.appendText(this.classCode.sourceCode, tempCode.sourceCode);
	}
	
	public MetaClassCode generateClassMetaCode() {
		this.extraHeaderCodeInClass = "";

		this.doGenerateClassMetaCode();

		return this.classCode;
	}

}
