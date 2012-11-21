package org.cpgf.metagen.metadata;

import java.util.List;

import org.cpgf.metagen.Util;
import org.cpgf.metagen.cppparser.ParserUtil;


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
	
	public boolean hasResult() {
		return ! this.resultType.isVoid();
	}

	@Override
	public void getPolicyRules(List<String> rules) {
		if(this.resultType != null) {
			getPolicyRuleForParameter(rules, this.resultType, -1);
		}
		
		int indexOffset = this.getParameterPolicyRuleStartIndex();
		
		for(int i = 0; i < this.getParameterList().size(); ++i) {
			getPolicyRuleForParameter(rules, this.getParameterList().get(i).getType(), i + indexOffset);
		}
	}
	
	protected int getParameterPolicyRuleStartIndex() {
		return 0;
	}

	public static void getPolicyRuleForParameter(List<String> rules, CppType type, int parameterIndex) {
		ClassTraits traits = type.getClassTraits();

		if(traits != null) {
			if(type.isConstValueReference()) {
				if(traits.isHasTypeConvertConstructor() && !traits.isCopyConstructorHidden()) {
					Util.addToList(rules, ParserUtil.composePolicyRuleForParameter("GMetaRuleCopyConstReference", parameterIndex));
				}
				
				return;
			}
			
			if(type.isPointer()) {
				return;
			}
			
			if(traits.isCopyConstructorHidden()) {
				Util.addToList(rules, ParserUtil.composePolicyRuleForParameter("GMetaRuleParamNoncopyable", parameterIndex));
			}
			
		}
	}

	@Override
	public void replaceInType(String pattern, String substitute)
	{
		super.replaceInType(pattern, substitute);

		if(this.resultType != null) {
			this.resultType.setLiteralType(this.resultType.getLiteralType().replaceAll(pattern, substitute));
		}
	}
	

}
