package org.cpgf.metagen.metadata;


public class Operator extends CppInvokable {
	private String self;
	
	public Operator(String operator, CppType resultType) {
		super(EnumCategory.Operator, operator, resultType);
	}

	public String getOperator() {
		return this.getLiteralName();
	}

	public boolean isFunctor() {
		return this.getOperator().equals("()");
	}
	
	public boolean hasSelf() {
		return !this.isFunctor() && !this.isStatic();
	}

	@Override
	protected int getParameterPolicyRuleStartIndex() {
		return this.hasSelf() ? 1 : 0;
	}

	public String getSelf() {
		return self;
	}

	public void setSelf(String self) {
		this.self = self;
	}

}
