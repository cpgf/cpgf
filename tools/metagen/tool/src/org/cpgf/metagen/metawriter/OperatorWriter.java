package org.cpgf.metagen.metawriter;

import java.util.List;
import java.util.ArrayList;

import org.cpgf.metagen.Util;
import org.cpgf.metagen.codewriter.CppWriter;
import org.cpgf.metagen.metadata.MetaInfo;
import org.cpgf.metagen.metadata.Operator;
import org.cpgf.metagen.metadata.CppClass;
import org.cpgf.metagen.metadata.Parameter;

public class OperatorWriter {
	private MetaInfo metaInfo;
	private Operator item;
	private int realParamCount;
	private boolean isIncOrDec;
	private boolean isNestedTemplateType;
	
	private List<List<Parameter> > templateDependentParameterLists;
	
	public OperatorWriter(MetaInfo metaInfo, Operator item) {
		this.metaInfo = metaInfo;
		this.item = item;
		
		this.initialize();
	}
	
	private void initialize() {
		boolean isStatic = item.isStatic();
		this.realParamCount = this.item.getParameterList().size();

		String op = item.getOperator();
		if(! isStatic) {
			++this.realParamCount;
		}
		
		if(op.equals("->")) {
			this.realParamCount = 2;
		}

		this.isIncOrDec = (op.equals("++") || op.equals("--"));
		
//		if(this.item.getOwner().isTemplate()) {
//			for(Parameter templateParam : this.item.getOwner().getTemplateParameterList()) {
//				for(Parameter param : this.item.getParameterList()) {
//					if(param.getType().getParsedType().matchSymbol(templateParam.getName())) {
//						if(this.templateDependentParameterList == null) {
//							this.templateDependentParameterList = new ArrayList<Parameter>();
//						}
//						this.templateDependentParameterList.add(templateParam);
//					}
//				}
//			}
//		}
		this.templateDependentParameterLists = new ArrayList<List<Parameter> >();
		CppClass owner = this.item.getOwner();
		while (owner != null) {
			if(owner.isTemplate()) {
				this.templateDependentParameterLists.add(owner.getTemplateParameterList());
				this.isNestedTemplateType = true;
			} else {
					this.templateDependentParameterLists.add(null);
			}
			owner = owner.getOwner();
		}

		owner = this.item.getOwner().getOwner();
		this.isNestedTemplateType = false;
		while (owner != null) {
			if(owner.isTemplate()) {
				this.isNestedTemplateType = true;
			}
			owner = owner.getOwner();
		}
	}
	
	public void writeReflectionCode(CppWriter codeWriter, String define) {
		String action = WriterUtil.getReflectionAction(define, "_operator");

		String op = item.getOperator();
		
		boolean isTypeConvert = this.item.isTypeConverter();
		String opText = "";
		
		String self = item.getSelf();
		if(self == null) {
			self = "cpgf::GMetaSelf";
		}
		
		if(isTypeConvert) {
			codeWriter.write(action + "<" + op + " (" + self + ")>(");
			opText = "H()";
		}
		else {
			codeWriter.write(action + "<" + item.getResultType().getLiteralType() + " (*)(");
			
			boolean hasSelf = item.hasSelf();

			if(hasSelf) {
				if(item.isConst()) {
					codeWriter.write("const " + self + " &");
				}
				else {
					codeWriter.write(self);
				}
			}

			if(this.isIncOrDec) {
			}
			else {
				if(item.hasParameter() && hasSelf) {
					codeWriter.write(", ");
				}
				WriterUtil.writeParamList(codeWriter, item.getParameterList(), false);
			}
			codeWriter.write(")>(");
			if(item.isFunctor()) {
				opText = "H(H)";
			}
			else if(op.equals("[]")) {
				opText = "H[0]";
			}
			else if(isTypeConvert && this.realParamCount == 1) { // type convert T()
				opText = "H()";
			}
			else {
				if(this.realParamCount == 2) {
					if(this.isIncOrDec) {
						opText = "H" + op;
					}
					else {
						opText = "H " + op + " H";
					}
				}
				else if(this.realParamCount == 1) {
					opText = op + "H";
				}
				else {
				}
			}
		}

		opText = opText.replaceAll("\\bH\\b", "mopHolder");
		if(op.equals(",")) {
			opText = "(" + opText + ")"; // one more pair of brackets to avoid compile error
		}
		codeWriter.write(opText);
		codeWriter.write(WriterUtil.getPolicyText(item) + ")");

		WriterUtil.writeDefaultParams(codeWriter, item.getParameterList());
	}
	
	private void writeSelf(CppWriter codeWriter, boolean includeName) {
		if(this.item.isConst()) {
			codeWriter.write("const ");
		}
		if (this.isNestedTemplateType) {
			codeWriter.write("typename ");
		}
		codeWriter.write( this.item.getOwner().getFullQualifiedName());
		codeWriter.write(" *");
		if(includeName) {
			codeWriter.write(" self");
		}
	}
	
	private boolean shouldGenerateArraySetter() {
		return this.item.isArray() && this.item.getResultType().isNonConstValueReference();
	}

	public void writeNamedWrapperFunctionCode(CppWriter codeWriter) {
		String op = this.item.getOperator();

		for (List<Parameter> parameters : this.templateDependentParameterLists) {
			if(parameters != null) {
				codeWriter.write("template <");
				codeWriter.write(Util.getParameterText(parameters, true, true));
				codeWriter.writeLine(">");
			}
		}
		codeWriter.write("inline " + this.item.getResultType().getLiteralType() + " ");
		codeWriter.write(WriterUtil.getOperatorWraperName(this.metaInfo, this.item) + "(");
		this.writeSelf(codeWriter, true);
		if(this.item.hasParameter() && ! this.isIncOrDec) {
			codeWriter.write(", ");
			WriterUtil.writeParamList(codeWriter, this.item.getParameterList(), true);
		}
		codeWriter.write(") ");
		
		codeWriter.beginBlock();
		if(this.item.hasResult()) {
			codeWriter.write("return ");
		}
		if(this.item.isFunctor()) {
			codeWriter.write("(*self)(");
			codeWriter.write(Util.getParameterText(this.item.getParameterList(), false, true));
			codeWriter.write(")");
		}
		else if(this.item.isArray()) {
			codeWriter.write("(*self)[");
			codeWriter.write(Util.getParameterText(this.item.getParameterList(), false, true));
			codeWriter.write("]");
		}
		else if(op.matches("\\w") && this.realParamCount == 1) { // type convert T()
		}
		else {
			if(this.realParamCount == 2) {
				if(this.isIncOrDec) {
					codeWriter.write("(*self)" + op);
				}
				else {
					codeWriter.write("(*self) " + op + " ");
					codeWriter.write(Util.getParameterText(this.item.getParameterList(), false, true));
				}
			}
			else if(this.realParamCount == 1) {
				codeWriter.write(op + "(*self)");
			}
			else {
			}
		}
		codeWriter.writeLine(";");

		codeWriter.endBlock();
		
		if(this.shouldGenerateArraySetter()) {
			for (List<Parameter> parameters : this.templateDependentParameterLists) {
				if(parameters != null) {
					codeWriter.write("template <");
					codeWriter.write(Util.getParameterText(parameters, true, true));
					codeWriter.writeLine(">");
				}
			}
			codeWriter.write("inline void ");
			codeWriter.write(WriterUtil.getOperatorWraperNamePrefix(this.metaInfo, this.item) + this.metaInfo.getOperatorNameMap().get(this.item, 2) + "(");
			this.writeSelf(codeWriter, true);
			codeWriter.write(", ");
			WriterUtil.writeParamList(codeWriter, this.item.getParameterList(), true);
			codeWriter.write(", ");
			codeWriter.write(this.getArraySetterValueType() + " OpsEt_vALue");
			codeWriter.write(") ");
			
			codeWriter.beginBlock();
			codeWriter.write("(*self)[");
			codeWriter.write(Util.getParameterText(this.item.getParameterList(), false, true));
			codeWriter.writeLine("] = OpsEt_vALue;");
			codeWriter.endBlock();
		}
	}
	
	private String getArraySetterValueType() {
		String typename = "";
		if(this.item.getOwner().isTemplate() || this.isNestedTemplateType) {
			typename = "typename ";
		}
		return "const " + typename + "cpgf::RemoveReference<" + this.item.getResultType().getLiteralType() + " >::Result &";
	}

	public void writeNamedWrapperReflectionCode(CppWriter codeWriter, String define) {
		String methodName = WriterUtil.getOperatorWraperName(this.metaInfo, this.item);
		for (List<Parameter> parameters : this.templateDependentParameterLists) {
			if(parameters != null) {
				methodName = methodName + "<"
					+ Util.getParameterText(parameters, false, true)
					+ ">"
				;
			}
		}
		String reflectionName = this.metaInfo.getOperatorNameMap().get(this.item);

		String action = WriterUtil.getReflectionAction(define, "_method");

		codeWriter.write(action);
		codeWriter.write("(" + Util.quoteText(reflectionName) + ", ");
		codeWriter.write("(" + this.item.getResultType().getLiteralType() + " (*) (");
		this.writeSelf(codeWriter, false);
		if(this.item.hasParameter() && ! this.isIncOrDec) {
			codeWriter.write(", ");
			WriterUtil.writeParamList(codeWriter, this.item.getParameterList(), false);
		}
		codeWriter.write(")");
		codeWriter.write(")");
		codeWriter.write("&" + methodName);
		item.setIsWrapping(true);
		String ruleText = WriterUtil.getPolicyRulesText(this.item);
		item.setIsWrapping(false);
		if(ruleText.length() > 0) {
			ruleText = ruleText + ", ";
		}
		ruleText = ruleText + "cpgf::GMetaRuleExplicitThis";
		codeWriter.write(", cpgf::MakePolicy<" + ruleText + " >())");

		WriterUtil.writeDefaultParams(codeWriter, this.item.getParameterList());
		
		if(this.shouldGenerateArraySetter()) {
			this.doWriteNamedWrapperReflectionCodeForArraySetter(codeWriter, define);
		}
	}
	
	private void doWriteNamedWrapperReflectionCodeForArraySetter(CppWriter codeWriter, String define) {
		String reflectionName = this.metaInfo.getOperatorNameMap().get(this.item, 2);
		String methodName = WriterUtil.getOperatorWraperNamePrefix(this.metaInfo, this.item) + reflectionName;
		for (List<Parameter> parameters : this.templateDependentParameterLists) {
			if(parameters != null) {
				methodName = methodName + "<"
					+ Util.getParameterText(parameters, false, true)
					+ ">"
				;
			}
		}
		String action = WriterUtil.getReflectionAction(define, "_method");

		codeWriter.write(action);
		codeWriter.write("(" + Util.quoteText(reflectionName) + ", ");
		codeWriter.write("(void (*) (");
		this.writeSelf(codeWriter, false);
		codeWriter.write(", ");
		WriterUtil.writeParamList(codeWriter, this.item.getParameterList(), false);
		codeWriter.write(", ");
		codeWriter.write(this.getArraySetterValueType());
		codeWriter.write(")");
		codeWriter.write(")");
		codeWriter.write("&" + methodName);
		item.setIsWrapping(true);
		String ruleText = WriterUtil.getPolicyRulesText(this.item);
		item.setIsWrapping(false);
		if(ruleText.length() > 0) {
			ruleText = ruleText + ", ";
		}
		ruleText = ruleText + "cpgf::GMetaRuleExplicitThis";
		codeWriter.write(", cpgf::MakePolicy<" + ruleText + " >())");

		WriterUtil.writeDefaultParams(codeWriter, this.item.getParameterList());
	}
	
}
