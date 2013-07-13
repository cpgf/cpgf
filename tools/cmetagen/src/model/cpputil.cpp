#include "cpputil.h"
#include "cpptype.h"

#include "Poco/RegularExpression.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "llvm/Support/raw_ostream.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/PrettyPrinter.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <vector>

using namespace llvm;
using namespace clang;
using namespace std;

namespace metagen {


string removeRecordWords(const string & text)
{
	static Poco::RegularExpression re("(struct|class|union)\\b\\s*");
	string result(text);
	re.subst(result, "");
	return result;
}

QualType stripType(const QualType & qualType)
{
	QualType qType = qualType;
	SplitQualType splitQualType = qType.split();
	const Type * t = splitQualType.Ty;

	for(;;) {
		if(t->isArrayType()) {
			qType = dyn_cast<ArrayType>(t->getCanonicalTypeInternal())->getElementType();
		}
		else if(t->isPointerType()) {
			qType = dyn_cast<PointerType>(t)->getPointeeType();
		}
		else if(t->isReferenceType()) {
			qType = dyn_cast<ReferenceType>(t)->getPointeeType();
		}
		else {
			break;
		}
		splitQualType = qType.split();
		t = splitQualType.Ty;
	}

	return qType;
}

void getNamedDeclNames(const NamedDecl * namedDecl,
	std::string & name,
	std::string & qualifiedName,
	std::string & qualifiedNameWithoutNamespace)
{
	name = namedDecl->getNameAsString();
	qualifiedName = name;
	qualifiedNameWithoutNamespace = name;

	const DeclContext * context = namedDecl->getDeclContext();
	if(context->isFunctionOrMethod()) {
		return;
	}

	vector<const DeclContext *> contextList;
	
	while(context != NULL && isa<NamedDecl>(context)) {
		contextList.push_back(context);
		context = context->getParent();
	}

	for(vector<const DeclContext *>::iterator it = contextList.begin(); it != contextList.end(); ++it) {
		const NamespaceDecl * namespaceDecl = dyn_cast<NamespaceDecl>(*it);
		if(namespaceDecl != NULL && ! namespaceDecl->isAnonymousNamespace()) {
			qualifiedName = namespaceDecl->getNameAsString() + "::" + qualifiedName;
		}
		else {
			const RecordDecl * recordDecl = dyn_cast<RecordDecl>(*it);
			if(recordDecl != NULL && ! recordDecl->isAnonymousStructOrUnion()) {
				string s(recordDecl->getNameAsString());
				qualifiedName = s + "::" + qualifiedName;
				qualifiedNameWithoutNamespace = s + "::" + qualifiedNameWithoutNamespace;
			}
		}
	}
}

std::string	 getNamedDeclOutputName(const NamedDecl * namedDecl)
{
	std::string name, qualifiedName, qualifiedNameWithoutNamespace;
	getNamedDeclNames(namedDecl, name, qualifiedName, qualifiedNameWithoutNamespace);
	return qualifiedNameWithoutNamespace;
}

std::string	 getNamedDeclQualifiedName(const clang::NamedDecl * namedDecl)
{
	std::string name, qualifiedName, qualifiedNameWithoutNamespace;
	getNamedDeclNames(namedDecl, name, qualifiedName, qualifiedNameWithoutNamespace);
	return qualifiedName;
}

std::string getTemplateSpecializationName(const TemplateSpecializationType * type)
{
	string qualifiedName;

	qualifiedName = type->getTemplateName().getAsTemplateDecl()->getQualifiedNameAsString();
	qualifiedName += "<";
	for(unsigned int i = 0; i < type->getNumArgs(); ++i) {
		if(i > 0) {
			qualifiedName += ", ";
		}
		qualifiedName += getTemplateArgumentName(type->getArg(i));
	}
	qualifiedName += " >";

	return qualifiedName;
}

string getTemplateArgumentName(const TemplateArgument & argument)
{
	string qualifiedName;

	switch(argument.getKind()) {
		case TemplateArgument::Null:
			qualifiedName = "NULL";
			break;

		case TemplateArgument::Type:
			qualifiedName = CppType(argument.getAsType()).getQualifiedName();
			break;

		case TemplateArgument::Declaration:
			qualifiedName = dyn_cast<NamedDecl>(argument.getAsDecl())->getQualifiedNameAsString();
			break;

		case TemplateArgument::Integral:
		case TemplateArgument::Expression:
			qualifiedName = exprToText(argument.getAsExpr());
			break;

		case TemplateArgument::Template:
			qualifiedName = argument.getAsTemplate().getAsTemplateDecl()->getQualifiedNameAsString();
			break;

		case TemplateArgument::TemplateExpansion:
			break;

		case TemplateArgument::Pack:
			break;

	}

	return qualifiedName;
}

std::string exprToText(const clang::Expr * expr)
{
	std::string text;
	raw_string_ostream stream(text);
	LangOptions langOptions;
	langOptions.CPlusPlus = 1;
	PrintingPolicy policy(langOptions);
	policy.SuppressSpecifiers = 0;
	expr->printPretty(stream, NULL, policy);
	return stream.str();
}

std::string namedDeclToText(const NamedDecl * namedDecl)
{
	std::string name, qualifiedName, qualifiedNameWithoutNamespace;
	getNamedDeclNames(namedDecl, name, qualifiedName, qualifiedNameWithoutNamespace);
	return qualifiedNameWithoutNamespace;
}

std::string declToText(const clang::Decl * decl)
{
	std::string text;
	raw_string_ostream stream(text);
	LangOptions langOptions;
	langOptions.CPlusPlus = 1;
	PrintingPolicy policy(langOptions);
	policy.SuppressSpecifiers =0;
	decl->print(stream, policy, NULL, false);
	return stream.str();

	//const NamedDecl * namedDecl = dyn_cast<NamedDecl>(decl);
	//if(namedDecl != NULL) {
	//	return namedDeclToText(namedDecl);
	//}
	//else {
	//	return "";
	//}
}


} // namespace metagen
