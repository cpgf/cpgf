package com.cpgf.metagen.metadata;

import java.util.List;

import com.cpgf.metagen.metawriter.WriterUtil;

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
			WriterUtil.getPolicyRuleForParameter(rules, this.resultType, -1);
		}
		
		for(int i = 0; i < this.getParameterList().size(); ++i) {
			WriterUtil.getPolicyRuleForParameter(rules, this.getParameterList().get(i).getType(), i);
		}
	}


}
