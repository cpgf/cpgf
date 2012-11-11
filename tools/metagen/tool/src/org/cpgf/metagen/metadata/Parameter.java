package org.cpgf.metagen.metadata;

import org.cpgf.metagen.MetaException;
import org.cpgf.metagen.Util;
import org.cpgf.metagen.cppparser.ParserUtil;

public class Parameter {
	private String name;
	private CppType type;
	private String defaultValue;
	private ParameteredItem owner;
	private CppInvokable callback; // used for callback in script binding

	public Parameter(String name, CppType type, String defaultValue, ParameteredItem owner) {
		this.name = name;
		this.type = type;
		this.defaultValue = defaultValue;
		this.owner = owner;
	}

	public String getName() {
		return name;
	}

	public CppType getType() {
		return type;
	}
	
	public String getDefaultValue() {
		return defaultValue;
	}
	
	public void setDefaultValue(String defaultValue) {
		this.defaultValue = defaultValue;
	}

	public void clearDefaultValue() {
		this.defaultValue = null;
	}

	public boolean hasDefaultValue() {
		return this.defaultValue != null;
	}

	public ParameteredItem getOwner() {
		return owner;
	}

	public void setOwner(ParameteredItem owner) {
		this.owner = owner;
	}

	public CppInvokable getCallback() {
		return callback;
	}

	public void setCallback(String functionType) throws MetaException {
		this.callback = null;
		if(functionType != null && functionType.length() > 0) {
			this.callback = ParserUtil.parseFunctionType(functionType);
			if(this.callback == null) {
				Util.error("Function type is incorrect -- " + functionType);
			}
		}
	}

}
