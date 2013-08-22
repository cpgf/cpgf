#include "builderoperator.h"
#include "builderwriter.h"
#include "builderutil.h"
#include "codewriter/cppwriter.h"
#include "codewriter/codeblock.h"
#include "model/cppoperator.h"
#include "model/cppcontainer.h"
#include "model/cppclass.h"
#include "project.h"
#include "util.h"

#include "Poco/Format.h"

#include <map>

using namespace std;

namespace metagen {


BuilderOperator::BuilderOperator(const CppItem * cppItem)
	: super(cppItem)
{
	this->checkBuilderItemCategory(icOperator);
}

BuilderOperator::~BuilderOperator()
{
}

const CppOperator * BuilderOperator::getCppOperator() const
{
	return static_cast<const CppOperator *>(this->getCppItem());
}

void BuilderOperator::doWriteMetaData(BuilderWriter * writer)
{
	this->doWriteReflection(writer);
	if(this->shouldGenerateWrapper()) {
		this->doWriteWrapper(writer);
	}
}

size_t BuilderOperator::calculateReflectionParamCount() const
{
	const CppOperator * cppOperator = this->getCppOperator();
	size_t paramCount = cppOperator->getArity();

	if(! cppOperator->isStatic()) {
		++paramCount;
	}
	if(cppOperator->getOperatorName() == "->") {
		paramCount = 2;
	}

	return paramCount;
}

void BuilderOperator::doWriteReflection(BuilderWriter * writer)
{
	const CppOperator * cppOperator = this->getCppOperator();
	CodeBlock * codeBlock = writer->getParentReflectionCodeBlock(cppOperator);

	string text(writer->getReflectionAction("_operator"));

	const string op = cppOperator->getOperatorName();
	bool isIncOrDec = (op == "++" || op == "--");
	string opText;
	string self = "cpgf::GMetaSelf";

	if(cppOperator->isTypeConverter()) {
		text.append("<" + op + " (" + self + ")>(");
		opText = "mopHolder()";
	}
	else {
		text.append("<");

		string proto = "(*)(";
		bool hasSelf = ! cppOperator->isFunctor() && ! cppOperator->isStatic();
		if(hasSelf) {
			if(cppOperator->isConst()) {
				proto.append("const " + self + " &");
			}
			else {
				proto.append(self);
			}
		}
		if(isIncOrDec) {
		}
		else {
			if(cppOperator->getArity() > 0 && hasSelf) {
				proto.append(", ");
			}
			proto.append(cppOperator->getTextOfParamList(itoWithArgType));
		}
		text.append(cppOperator->getResultType().getQualifiedName(proto));
		text.append(")>(");
		if(cppOperator->isFunctor()) {
			opText = "mopHolder(mopHolder)";
		}
		else if(cppOperator->isArray()) {
			opText = "mopHolder[0]";
		}
		else {
			size_t realParamCount = this->calculateReflectionParamCount();
			if(realParamCount == 2) {
				if(isIncOrDec) {
					opText = "mopHolder" + op;
				}
				else {
					opText = "mopHolder " + op + " mopHolder";
				}
			}
			else if(realParamCount == 1) {
				opText = op + "mopHolder";
			}
			else {
			}
		}
	}

	if(op == ",") {
		opText = "(" + opText + ")"; // one more pair of brackets to avoid compile error
	}
	text.append(opText);
	text.append(getInvokablePolicyText(cppOperator, true));
	text.append(");");

	codeBlock->appendLine(text);
}

class OperatorNameMap
{
private:
	typedef map<string, string> MapType;

public:
	static OperatorNameMap * getNameMap() {
		static OperatorNameMap nameMap;
		return &nameMap;
	}

	string get(const CppOperator * op, int paramCount) const {
		MapType::const_iterator it = this->operatorNameMap.find(this->normalizeOperator(op->getOperatorName(), paramCount));
		if(it == this->operatorNameMap.end()) {
			it = this->operatorNameMap.find(this->normalizeOperator(op->getOperatorName(), -1));
		}
		return it->second;
	}
	
	string get(const CppOperator * op) const {
		return this->get(op, (int)op->getArity());
	}
	
private:
	OperatorNameMap() {
		this->initialize();
	}

	string normalizeOperator(string op, int parameterCount) const {
		if(parameterCount < 0) {
			return op;
		}
		else {
			return Poco::format("%s%s%d", op, string("_"), parameterCount);
		}
	}
	
	void set(string op, int parameterCount, string name) {
		this->operatorNameMap.insert(make_pair(this->normalizeOperator(op, parameterCount), name));
	}
	
	void set(string op, string name) {
		this->set(op, -1, name);
	}
	
	void initialize() {
		this->set("+", 1, "_opAdd");
		this->set("-", 1, "_opSub");
		this->set("*", 1, "_opMul");
		this->set("/", "_opDiv");
		this->set("%", "_opMod");

		this->set("+", 0, "_opPlus");
		this->set("-", 0, "_opMinus");
		
		this->set("+=", "_opAddAssign");
		this->set("-=", "_opSubAssign");
		this->set("*=", "_opMulAssign");
		this->set("/=", "_opDivAssign");
		this->set("%=", "_opModAssign");

		this->set("=", "_opAssign");

		this->set("==", "_opEqual");
		this->set("!=", "_opNotEqual");

		this->set(">", "_opGreater");
		this->set("<", "_opLess");
		this->set(">=", "_opGreaterEqual");
		this->set("<=", "_opLessEqual");
		
		this->set("&&", "_opAnd");
		this->set("||", "_opOr");
		this->set("!", "_opNot");

		this->set("&", 1, "_opBitAnd");
		this->set("|", "_opBitOr");
		this->set("^", "_opBitXor");
		this->set("<<", "_opLeftShift");
		this->set(">>", "_opRightShift");
		this->set("~", "_opBitNot");

		this->set("&=", "_opBitAndAssign");
		this->set("|=", "_opBitOrAssign");
		this->set("^=", "_opBitXorAssign");
		this->set("<<=", "_opLeftShiftAssign");
		this->set(">>=", "_opRightShiftAssign");

		this->set("++", 0, "_opInc");
		this->set("--", 0, "_opDec");
		this->set("++", 1, "_opIncSuffix");
		this->set("--", 1, "_opDecSuffix");
		
		this->set(",", "_opComma");
		this->set("()", "_opFunction");
		this->set("[]", 1, "_opArrayGet");
		this->set("[]", 2, "_opArraySet");

		this->set("&", 0, "_opAddress");
		this->set("*", 0, "_opDerefer");
	}

private:
	MapType operatorNameMap;
};

struct WriterParam
{
	BuilderWriter * writer;
	const CppOperator * cppOperator;
	string op;
	string operatorWrapperName;
	string arraySetterName;
	string selfParamName;
	string selfName;
	string self;
	string templateLine;
	bool isIncOrDec;
	string paramValuesText;
	size_t realParamCount;
	bool shouldWrapArraySetter;
};

namespace {

void writeOperator(const WriterParam * param)
{
	CodeBlock * codeBlock = param->writer->createOperatorWrapperCodeBlock(param->cppOperator);
	codeBlock->appendLine(param->templateLine);
	string s;
	s = param->operatorWrapperName + "(";
	s.append(param->self);
	if(param->cppOperator->getArity() > 0 && ! param->isIncOrDec) {
		s.append(", ");
		s.append(param->cppOperator->getTextOfParamList(itoWithArgType | itoWithArgName | itoWithDefaultValue));
	}
	s.append(")");
	s = param->cppOperator->getResultType().getQualifiedName(s);
	codeBlock->appendLine(s);

	CodeBlock * bodyBlock = codeBlock->appendBlock(cbsBracketAndIndent);
	string paramValuesText = param->cppOperator->getTextOfParamList(itoWithArgName);
	s = "";

	if(param->cppOperator->hasResult()) {
		s.append("return ");
	}

	if(param->cppOperator->isFunctor()) {
		s.append(Poco::format("(*%s)(%s)", param->selfName, param->paramValuesText));
	}
	else if(param->cppOperator->isArray()) {
		s.append(Poco::format("(*%s)[%s]", param->selfName, param->paramValuesText));
	}
	else {
		if(param->realParamCount == 2) {
			if(param->isIncOrDec) {
				s.append(Poco::format("(*%s)%s", param->selfName, param->op));
			}
			else {
				s.append(Poco::format("(*%s) %s %s", param->selfName, param->op, paramValuesText));
			}
		}
		else if(param->realParamCount == 1) {
				s.append(Poco::format("%s(*%s)", param->op, param->selfName));
		}
	}

	s.append(";");
	bodyBlock->appendLine(s);
}

void writeArraySetter(const WriterParam * param)
{
	if(param->shouldWrapArraySetter) {
		string s;
		CodeBlock * setterBlock = param->writer->createOperatorWrapperCodeBlock(param->cppOperator)->appendBlock();
		setterBlock->appendLine(param->templateLine);

		s = Poco::format("void %s(%s", param->arraySetterName, param->self);
		if(param->cppOperator->getArity() > 0) {
			s.append(", ");
			s.append(param->cppOperator->getTextOfParamList(itoWithArgType | itoWithArgName | itoWithDefaultValue));
		}
		s.append(Poco::format(", const %s & OpsEt_vALue)", param->cppOperator->getResultType().getNonReferenceType().getQualifiedName()));
		setterBlock->appendLine(s);

		CodeBlock * setterBodyBlock = setterBlock->appendBlock(cbsBracketAndIndent);
		s = Poco::format("(*%s)[%s] = OpsEt_vALue;", param->selfName, param->paramValuesText);
		setterBodyBlock->appendLine(s);
	}
}

void writeOperatorReflection(const WriterParam * param)
{
	CodeBlock * codeBlock = param->writer->getParentReflectionCodeBlock(param->cppOperator);

	std::string s = Poco::format("%s(\"%s\", &%s<%s >);",
		param->writer->getReflectionAction("_method"),
		OperatorNameMap::getNameMap()->get(param->cppOperator),
		param->operatorWrapperName,
		getReflectionClassName(param->cppOperator->getProject(), true)
	);

	codeBlock->appendLine(s);
}

void writeArraySetterReflection(const WriterParam * param)
{
	if(param->shouldWrapArraySetter) {
		CodeBlock * codeBlock = param->writer->getParentReflectionCodeBlock(param->cppOperator);

		std::string s = Poco::format("%s(\"%s\", &%s<%s >);",
			param->writer->getReflectionAction("_method"),
			OperatorNameMap::getNameMap()->get(param->cppOperator, 2),
			param->arraySetterName,
			getReflectionClassName(param->cppOperator->getProject(), true)
		);

		codeBlock->appendLine(s);
	}
}

}

void BuilderOperator::doWriteWrapper(BuilderWriter * writer)
{
	WriterParam param;
	param.writer = writer;
	const CppOperator * cppOperator = this->getCppOperator();
	param.cppOperator = cppOperator;
	string operatorWrapperPrefix = "oPeRat0rWrapPer_" + normalizeSymbolName(cppOperator->getParent()->getQualifiedName());
	param.operatorWrapperName = Poco::format("%s%s_%i",
		operatorWrapperPrefix,
		OperatorNameMap::getNameMap()->get(cppOperator),
		cppOperator->getIndexInCategory()
	);
	param.selfParamName = "TsE1f";
	param.selfName = "sE1F";
	param.shouldWrapArraySetter = false;

	param.op = cppOperator->getOperatorName();
	param.isIncOrDec = (param.op == "++" || param.op == "--");

	param.templateLine = "template <typename " + param.selfParamName;

	if(cppOperator->getParent()->isClass()) {
		const CppClass * cppClass = static_cast<const CppClass *>(cppOperator->getParent());
		if(cppClass->isChainedTemplate()) {
			param.templateLine.append(", ");
			param.templateLine.append(cppClass->getTextOfChainedTemplateParamList(itoWithArgType | itoWithArgName | itoWithDefaultValue));
		}
	}
	param.templateLine.append(">");

	if(cppOperator->isConst()) {
		param.self.append("const ");
	}
	param.self.append(param.selfParamName + " * " + param.selfName);

	param.paramValuesText = cppOperator->getTextOfParamList(itoWithArgName);
	param.realParamCount = this->calculateReflectionParamCount();

	if(cppOperator->isArray()
		&& cppOperator->getResultType().isReference()
		&& ! cppOperator->getResultType().isReferenceToConst()
	) {
		param.shouldWrapArraySetter = true;
		param.arraySetterName = Poco::format("%s%s_s%i",
			operatorWrapperPrefix,
			OperatorNameMap::getNameMap()->get(cppOperator, 2),
			cppOperator->getIndexInCategory()
		);
	}

	writeOperator(&param);
	writeArraySetter(&param);

	writeOperatorReflection(&param);
	writeArraySetterReflection(&param);
}

bool BuilderOperator::shouldGenerateWrapper() const
{
	const CppOperator * cppOperator = this->getCppOperator();
	if(! this->getProject()->shouldWrapOperator()) {
		return false;
	}
	else if(cppOperator->isTypeConverter()) {
		return false;
	}
	else if(cppOperator->getOperatorName() == "->") {
		return false;
	}
	else {
		return true;
	}
}


} // namespace metagen
