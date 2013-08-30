#include "cpputil.h"
#include "cpptype.h"
#include "cppclass.h"

#include "cpgf/gassert.h"

#include "Poco/RegularExpression.h"
#include "Poco/NumberParser.h"

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "llvm/Support/raw_ostream.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/AST/ASTContext.h"
#include "clang/Lex/Lexer.h"

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
	QualType qType = qualType->getCanonicalTypeInternal();
	SplitQualType splitQualType = qType.split();
	const Type * t = splitQualType.Ty;

	for(;;) {
		if(t->isArrayType()) {
			qType = dyn_cast<ArrayType>(t)->getElementType();
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

std::string getTemplateSpecializationName(const clang::ASTContext * astContext, const TemplateSpecializationType * type)
{
	string qualifiedName;

	qualifiedName = type->getTemplateName().getAsTemplateDecl()->getQualifiedNameAsString();
	qualifiedName += "<";
	for(unsigned int i = 0; i < type->getNumArgs(); ++i) {
		if(i > 0) {
			qualifiedName += ", ";
		}
		qualifiedName += getTemplateArgumentName(astContext, type->getArg(i));
	}
	qualifiedName += " >";

	return qualifiedName;
}

string getTemplateArgumentName(const clang::ASTContext * astContext, const TemplateArgument & argument)
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
			qualifiedName = exprToText(astContext, argument.getAsExpr());
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

std::string exprToText(const clang::ASTContext * astContext, const clang::Expr * expr)
{
	std::string text;
	raw_string_ostream stream(text);
	const LangOptions & langOptions = astContext->getLangOpts();
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

std::string declToText(const clang::ASTContext * astContext, const clang::Decl * decl)
{
	std::string text;
	raw_string_ostream stream(text);
	const LangOptions & langOptions = astContext->getLangOpts();
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

std::string getSourceText(const clang::ASTContext * astContext,
	const clang::SourceLocation & start,
	const clang::SourceLocation & end)
{
	const clang::SourceManager & sourceManager = astContext->getSourceManager();
	clang::SourceLocation e(clang::Lexer::getLocForEndOfToken(end, 0, sourceManager, astContext->getLangOpts()));
	return std::string(sourceManager.getCharacterData(start),
		sourceManager.getCharacterData(e) - sourceManager.getCharacterData(start));
}

const CppClass * findOutterTemplateByDepth(const CppClass * cppClass, int depth)
{
	const CppClass * currentClass = cppClass;
	while(currentClass != NULL) {
		if(currentClass->getTemplateDepth() == depth) {
			return currentClass;
		}
		currentClass = dynamic_cast<const CppClass *>(currentClass->getParent());
	}
	return NULL;
}

/*
namespace NS {

template <typename T, typename X>
struct A
{
	template <typename U>
	struct Y {
	};

	struct YY {
		template <typename U>
		struct B {
			void bb(Y<T> *, U *, X);
			void bb(int, int, int);
		};
	};
};

}
Will generate first "bb" as,
    _d.CPGF_MD_TEMPLATE _method("bb", (void (NS::A<T, X >::YY::B<U >::*)(Y<type-parameter-0-0> *, type-parameter-1-0 *, type-parameter-0-1))(&D_d::ClassType::bb));
type-parameter-DD-II is the place holder for type name such as T, X.
DD is the depth, which the most outter template is 0, the inner one is increased by 1
II is the type index, from left to right
*/
std::string doFixIllFormedTemplatePlaceHolder(const CppClass * ownerClass, const std::string & text)
{
	static Poco::RegularExpression re("\\btype-parameter-(\\d+)-(\\d+)\\b");
	string result = text;
	size_t position = 0;
	for(;;) {
		Poco::RegularExpression::MatchVec matches;
		int matchCount = re.match(result, position, matches, 0);
		if(matchCount == 3) {
			position = matches[0].offset + matches[0].length;
			
			int depth = Poco::NumberParser::parse(result.substr(matches[1].offset, matches[1].length));
			int index = Poco::NumberParser::parse(result.substr(matches[2].offset, matches[2].length));

			const CppClass * templateClass = findOutterTemplateByDepth(ownerClass, depth);
			GASSERT(templateClass != NULL);

			if(index < templateClass->getTemplateParamCount()) {
				string name = templateClass->getTemplateParamName(index);
				result = result.substr(0, matches[0].offset) + name + result.substr(matches[0].offset + matches[0].length);
				position = matches[0].offset + name.length();
			}
		}
		else {
			break;
		}
	}
	return result;
}

std::string fixIllFormedTemplates(const CppClass * ownerClass, const std::string & text)
{
	if(ownerClass == NULL || ! ownerClass->isChainedTemplate()) {
		return text;
	}

	string result = doFixIllFormedTemplatePlaceHolder(ownerClass, text);

	return result;
}


} // namespace metagen
