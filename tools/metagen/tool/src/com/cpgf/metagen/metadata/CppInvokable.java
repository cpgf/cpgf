package com.cpgf.metagen.metadata;

import java.util.List;

import com.cpgf.metagen.cppparser.ParserUtil;

public class CppInvokable extends ParameteredItem {
	private CppType resultType;

	public CppInvokable(EnumCategory category, String name, CppType resultType) {
		super(category, name);
		
		this.resultType = resultType;
	}

	public CppType getResultType() {
		return resultType;
	}

	public void setResultType(CppType resultType) {
		this.resultType = resultType;
	}

	@Override
	public void getPolicyRules(List<String> rules) {
		if(this.resultType != null) {
			ParserUtil.getPolicyRuleForParameter(rules, this.resultType, -1);
		}
		
		for(int i = 0; i < this.getParameterList().size(); ++i) {
			ParserUtil.getPolicyRuleForParameter(rules, this.getParameterList().get(i).getType(), i);
		}
	}


}
