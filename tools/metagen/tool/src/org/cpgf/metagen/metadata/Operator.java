package org.cpgf.metagen.metadata;


public class Operator extends CppInvokable {
	private String self;
	private boolean isWrapping;
	
	public Operator(String operator, CppType resultType) {
		super(EnumCategory.Operator, operator, resultType);
	}

	public String getOperator() {
		return this.getLiteralName();
	}

	public boolean isFunctor() {
		return this.getOperator().equals("()");
	}
	
	public boolean isArray() {
		return this.getOperator().equals("[]");
	}
	
	public boolean isTypeConverter() {
		return this.getOperator().matches(".*\\w+.*");
	}
	
	public boolean hasSelf() {
		return !this.isFunctor() && !this.isStatic();
	}

	@Override
	protected int getParameterPolicyRuleStartIndex() {
		if(this.hasSelf()) {
			return 1;
		}
		else {
			// When isWrapping, for functor, we must start from 1 because the first parameter is the explicit this.
			if(this.isWrapping && this.isFunctor()) {
				return 1;
			}
			return 0;
		}
	}

	public String getSelf() {
		return self;
	}

	public void setSelf(String self) {
		this.self = self;
	}

	public void setIsWrapping(boolean isWrapping) {
		this.isWrapping = isWrapping;
	}
}
