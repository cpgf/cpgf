package org.cpgf.metagen.metadata;

import java.util.List;

import org.cpgf.metagen.Util;
import org.cpgf.metagen.cppparser.ParserUtil;


public class CppInvokable extends ParameteredItem {
	private CppType resultType;
	private boolean transferResultOwnership;
	private boolean discardResultOwnership;
	private boolean useFullPrototype;

	public CppInvokable(EnumCategory category, String name, CppType resultType) {
		super(category, name);

		this.resultType = resultType;
		this.transferResultOwnership = false;
		this.discardResultOwnership = false;
	}

	public CppType getResultType() {
		return resultType;
	}

	public void setResultType(CppType resultType) {
		this.resultType = resultType;
	}

	public boolean getTransferResultOwnership() {
		return transferResultOwnership;
	}

	public void setTransferResultOwnership(boolean transferResultOwnership) {
		this.transferResultOwnership = transferResultOwnership;
	}

	public boolean getDiscardResultOwnership() {
		return discardResultOwnership;
	}

	public void setDiscardResultOwnership(boolean discardResultOwnership) {
		this.discardResultOwnership = discardResultOwnership;
	}

	public boolean getUseFullPrototype() {
		return useFullPrototype;
	}

	public void setUseFullPrototype(boolean useFullPrototype) {
		this.useFullPrototype = useFullPrototype;
	}

	public boolean hasResult() {
		return ! this.resultType.isVoid();
	}

	@Override
	protected void doGetPolicyRules(List<String> rules) {
		if(this.resultType != null) {
			if(getTransferResultOwnership()) {
				rules.add(ParserUtil.composePolicyRuleForParameter("GMetaRuleTransferOwnership", -1));
			}
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
