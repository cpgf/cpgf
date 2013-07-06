#include "builderoperator.h"
#include "builderfilewriter.h"
#include "codewriter/cppwriter.h"
#include "model/cppoperator.h"
#include "config.h"

#include "Poco/Format.h"

#include <map>

using namespace std;


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

void BuilderOperator::doWriteMetaData(BuilderFileWriter * writer)
{
	this->doWriteReflection(writer);
	if(this->getConfig()->shouldWrapOperator()) {
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

void BuilderOperator::doWriteReflection(BuilderFileWriter * writer)
{
	const CppOperator * cppOperator = this->getCppOperator();
	CodeBlock * codeBlock = writer->getReflectionCodeBlock(cppOperator);

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
			proto.append(cppOperator->getTextOfParamList(itoWithType));
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
	text.append(this->getPolicyText());
	text.append(");");

	codeBlock->addLine(text);
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

void BuilderOperator::doWriteWrapper(BuilderFileWriter * writer)
{
}

