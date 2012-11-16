package org.cpgf.metagen.metawriter;

import java.util.HashMap;
import java.util.Map;

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
	
	public String get(String op, int parameterCount) {
		String name = this.operatorNameMap.get(this.normalizeOperator(op, parameterCount));
		if(name == null) {
			name = this.operatorNameMap.get(this.normalizeOperator(op, -1));
		}
		return name;
	}
	
	public String get(String op) {
		return this.get(op, -1);
	}
	
	private void initOperatorNameMap() {
		this.set("+", "_opPlus");
		this.set("-", "_opMinus");
		this.set("*", "_opStar");
		this.set("/", "_opDiv");
		this.set("%", "_opMod");

		this.set("+=", "_opPlusAssign");
		this.set("-=", "_opMinusAssign");
		this.set("*=", "_opStarAssign");
		this.set("/=", "_opDivAssign");
		this.set("%=", "_opModAssign");

		this.set("=", "_opAssign");

		this.set("==", "_opEqual");
		this.set("!=", "_opNotEqual");

		this.set(">", "_opGreater");
		this.set("<", "_opLesser");
		this.set(">=", "_opGreaterEqual");
		this.set("<", "_opLesserEqual");
		
		this.set("&&", "_opAnd");
		this.set("||", "_opOr");
		this.set("!", "_opNot");

		this.set("&", "_opBitAnd");
		this.set("|", "_opBitOr");
		this.set("^", "_opBitXor");
		this.set("<<", "_opLeftShift");
		this.set(">>", "_opRightShift");
		this.set("~", "_opBitNot");

		this.set("&=", "_opBitAndEqual");
		this.set("|=", "_opBitOrEqual");
		this.set("^=", "_opBitXorEqual");
		this.set("<<=", "_opLeftShiftEqual");
		this.set(">>=", "_opRightShiftEqual");

		this.set("++", "_opInc");
		this.set("--", "_opDec");
		
		this.set(",", "_opComma");
		this.set("[]", "_opSubscript");
		this.set("->", "_opMember");
		this.set("()", "_opFunction");
	}

}
