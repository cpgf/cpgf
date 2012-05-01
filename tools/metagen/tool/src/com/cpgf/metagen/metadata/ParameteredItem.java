package com.cpgf.metagen.metadata;

import java.util.ArrayList;
import java.util.List;

public class ParameteredItem extends Item {
	private List<Parameter> parameterList;
	private List<Parameter> templateParameterList;
	private boolean isVirtual;
	private boolean isPureVirtual;

	public ParameteredItem(EnumCategory category, String name) {
		super(category, name);
		
		this.parameterList = new ArrayList<Parameter>();
		this.templateParameterList = new ArrayList<Parameter>();
	}
	
	public boolean hasParameter() {
		return this.parameterList.size() > 0;
	}

	public List<Parameter> getParameterList() {
		return parameterList;
	}

	public List<Parameter> getTemplateParameterList() {
		return templateParameterList;
	}

	public boolean isVirtual() {
		return isVirtual || this.isPureVirtual();
	}

	public void setVirtual(boolean isVirtual) {
		this.isVirtual = isVirtual;
	}

	public boolean isPureVirtual() {
		return isPureVirtual;
	}

	public void setPureVirtual(boolean isPureVirtual) {
		this.isPureVirtual = isPureVirtual;
	}

	public boolean isTemplate() {
		return this.templateParameterList.size() > 0;
	}

}
