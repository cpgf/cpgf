package org.cpgf.metagen.metawriter;

import java.util.HashMap;
import java.util.Map;

import org.cpgf.metagen.metadata.Operator;

public class OperatorNameMap {
	private Map<String, String> operatorNameMap;
	
	public OperatorNameMap() {
		this.operatorNameMap = new HashMap<String, String>();
		
		this.initOperatorNameMap();
	}

	private String normalizeOperator(String op, int parameterCount) {
		if(parameterCount < 0) {
			return op;
		}
		else {
			return op + "_" + parameterCount;
		}
	}
	
	public void set(String op, int parameterCount, String name) {
		this.operatorNameMap.put(this.normalizeOperator(op, parameterCount), name);
	}
	
	public void set(String op, String name) {
		this.set(op, -1, name);
	}
	
	public String get(Operator op, int paramCount) {
		String name = this.operatorNameMap.get(this.normalizeOperator(op.getOperator(), paramCount));
		if(name == null) {
			name = this.operatorNameMap.get(this.normalizeOperator(op.getOperator(), -1));
		}
		return name;
	}
	
	public String get(Operator op) {
		return this.get(op, op.getParameterCount());
	}
	
	private void initOperatorNameMap() {
		this.set("+", 1, "_opAdd");
		this.set("-", 1, "_opSub");
		this.set("*", 1, "_opMul");
		this.set("/", "_opDiv");
		this.set("%", "_opMod");

		this.set("+", 0, "_opPlus");
		this.set("-", 0, "_opMinus");
		
		this.set("+=", "_opAddAssign");
		this.set("-=", "_opSubAssign");
		this.set("*=", "_opMulAssign");
		this.set("/=", "_opDivAssign");
		this.set("%=", "_opModAssign");

		this.set("=", "_opAssign");

		this.set("==", "_opEqual");
		this.set("!=", "_opNotEqual");

		this.set(">", "_opGreater");
		this.set("<", "_opLess");
		this.set(">=", "_opGreaterEqual");
		this.set("<=", "_opLessEqual");
		
		this.set("&&", "_opAnd");
		this.set("||", "_opOr");
		this.set("!", "_opNot");

		this.set("&", 1, "_opBitAnd");
		this.set("|", "_opBitOr");
		this.set("^", "_opBitXor");
		this.set("<<", "_opLeftShift");
		this.set(">>", "_opRightShift");
		this.set("~", "_opBitNot");

		this.set("&=", "_opBitAndAssign");
		this.set("|=", "_opBitOrAssign");
		this.set("^=", "_opBitXorAssign");
		this.set("<<=", "_opLeftShiftAssign");
		this.set(">>=", "_opRightShiftAssign");

		this.set("++", 0, "_opInc");
		this.set("--", 0, "_opDec");
		this.set("++", 1, "_opIncSuffix");
		this.set("--", 1, "_opDecSuffix");
		
		this.set(",", "_opComma");
		this.set("()", "_opFunction");
		this.set("[]", 1, "_opArrayGet");
		this.set("[]", 2, "_opArraySet");

		this.set("&", 0, "_opAddress");
		this.set("*", 0, "_opDerefer");
	}

}
