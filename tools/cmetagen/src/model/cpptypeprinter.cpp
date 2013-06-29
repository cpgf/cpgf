// Modified from TypePrinter.cpp in clang


#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "clang/AST/PrettyPrinter.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclObjC.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Type.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/SaveAndRestore.h"
#include "llvm/Support/raw_ostream.h"

//#if defined(_MSC_VER)
//#pragma warning(pop)
//#endif

using namespace clang;

namespace {
  /// \brief RAII object that enables printing of the ARC __strong lifetime
  /// qualifier.
  class IncludeStrongLifetimeRAII {
    PrintingPolicy &Policy;
    bool Old;
    
  public:
    explicit IncludeStrongLifetimeRAII(PrintingPolicy &Policy) 
      : Policy(Policy), Old(Policy.SuppressStrongLifetime) {
      Policy.SuppressStrongLifetime = false;
    }
    
    ~IncludeStrongLifetimeRAII() {
      Policy.SuppressStrongLifetime = Old;
    }
  };

  class ParamPolicyRAII {
    PrintingPolicy &Policy;
    bool Old;
    
  public:
    explicit ParamPolicyRAII(PrintingPolicy &Policy) 
      : Policy(Policy), Old(Policy.SuppressSpecifiers) {
      Policy.SuppressSpecifiers = false;
    }
    
    ~ParamPolicyRAII() {
      Policy.SuppressSpecifiers = Old;
    }
  };

  class ElaboratedTypePolicyRAII {
    PrintingPolicy &Policy;
    bool SuppressTagKeyword;
    bool SuppressScope;
    
  public:
    explicit ElaboratedTypePolicyRAII(PrintingPolicy &Policy) : Policy(Policy) {
      SuppressTagKeyword = Policy.SuppressTagKeyword;
      SuppressScope = Policy.SuppressScope;
      Policy.SuppressTagKeyword = true;
      Policy.SuppressScope = true;
    }
    
    ~ElaboratedTypePolicyRAII() {
      Policy.SuppressTagKeyword = SuppressTagKeyword;
      Policy.SuppressScope = SuppressScope;
    }
  };
  
  class CppTypePrinter {
    PrintingPolicy Policy;
    bool HasEmptyPlaceHolder;

  public:
    explicit CppTypePrinter(const PrintingPolicy &Policy)
      : Policy(Policy), HasEmptyPlaceHolder(false) { }

    void print(const Type *ty, Qualifiers qs, raw_ostream &OS,
               StringRef PlaceHolder);
    void print(QualType T, raw_ostream &OS, StringRef PlaceHolder);

    static bool canPrefixQualifiers(const Type *T, bool &NeedARCStrongQualifier);
    void spaceBeforePlaceHolder(raw_ostream &OS);
    void printTypeSpec(const NamedDecl *D, raw_ostream &OS);

    void printBefore(const Type *ty, Qualifiers qs, raw_ostream &OS);
    void printBefore(QualType T, raw_ostream &OS);
    void printAfter(const Type *ty, Qualifiers qs, raw_ostream &OS);
    void printAfter(QualType T, raw_ostream &OS);
    void AppendScope(DeclContext *DC, raw_ostream &OS);
    void printTag(TagDecl *T, raw_ostream &OS);
#define ABSTRACT_TYPE(CLASS, PARENT)
#define TYPE(CLASS, PARENT) \
    void print##CLASS##Before(const CLASS##Type *T, raw_ostream &OS); \
    void print##CLASS##After(const CLASS##Type *T, raw_ostream &OS);
#include "clang/AST/TypeNodes.def"
  };
}

static void AppendTypeQualList(raw_ostream &OS, unsigned TypeQuals) {
  bool appendSpace = false;
  if (TypeQuals & Qualifiers::Const) {
    OS << "const";
    appendSpace = true;
  }
  if (TypeQuals & Qualifiers::Volatile) {
    if (appendSpace) OS << ' ';
    OS << "volatile";
    appendSpace = true;
  }
  if (TypeQuals & Qualifiers::Restrict) {
    if (appendSpace) OS << ' ';
    OS << "restrict";
  }
}

void CppTypePrinter::spaceBeforePlaceHolder(raw_ostream &OS) {
  if (!HasEmptyPlaceHolder)
    OS << ' ';
}

void CppTypePrinter::print(QualType t, raw_ostream &OS, StringRef PlaceHolder) {
  SplitQualType split = t.split();
  print(split.Ty, split.Quals, OS, PlaceHolder);
}

void CppTypePrinter::print(const Type *T, Qualifiers Quals, raw_ostream &OS,
                        StringRef PlaceHolder) {
  if (!T) {
    OS << "NULL TYPE";
    return;
  }

  SaveAndRestore<bool> PHVal(HasEmptyPlaceHolder, PlaceHolder.empty());

  printBefore(T, Quals, OS);
  OS << PlaceHolder;
  printAfter(T, Quals, OS);
}

bool CppTypePrinter::canPrefixQualifiers(const Type *T,
                                      bool &NeedARCStrongQualifier) {
  // CanPrefixQualifiers - We prefer to print type qualifiers before the type,
  // so that we get "const int" instead of "int const", but we can't do this if
  // the type is complex.  For example if the type is "int*", we *must* print
  // "int * const", printing "const int *" is different.  Only do this when the
  // type expands to a simple string.
  bool CanPrefixQualifiers = false;
  NeedARCStrongQualifier = false;
  Type::TypeClass TC = T->getTypeClass();
  if (const AutoType *AT = dyn_cast<AutoType>(T))
    TC = AT->desugar()->getTypeClass();
  if (const SubstTemplateTypeParmType *Subst
                                      = dyn_cast<SubstTemplateTypeParmType>(T))
    TC = Subst->getReplacementType()->getTypeClass();
  
  switch (TC) {
    case Type::Builtin:
    case Type::Complex:
    case Type::UnresolvedUsing:
    case Type::Typedef:
    case Type::TypeOfExpr:
    case Type::TypeOf:
    case Type::Decltype:
    case Type::UnaryTransform:
    case Type::Record:
    case Type::Enum:
    case Type::Elaborated:
    case Type::TemplateTypeParm:
    case Type::SubstTemplateTypeParmPack:
    case Type::TemplateSpecialization:
    case Type::InjectedClassName:
    case Type::DependentName:
    case Type::DependentTemplateSpecialization:
    case Type::ObjCObject:
    case Type::ObjCInterface:
    case Type::Atomic:
      CanPrefixQualifiers = true;
      break;
      
    case Type::ObjCObjectPointer:
      CanPrefixQualifiers = T->isObjCIdType() || T->isObjCClassType() ||
        T->isObjCQualifiedIdType() || T->isObjCQualifiedClassType();
      break;
      
    case Type::ConstantArray:
    case Type::IncompleteArray:
    case Type::VariableArray:
    case Type::DependentSizedArray:
      NeedARCStrongQualifier = true;
      // Fall through
      
    case Type::Pointer:
    case Type::BlockPointer:
    case Type::LValueReference:
    case Type::RValueReference:
    case Type::MemberPointer:
    case Type::DependentSizedExtVector:
    case Type::Vector:
    case Type::ExtVector:
    case Type::FunctionProto:
    case Type::FunctionNoProto:
    case Type::Paren:
    case Type::Attributed:
    case Type::PackExpansion:
    case Type::SubstTemplateTypeParm:
    case Type::Auto:
      CanPrefixQualifiers = false;
      break;
  }

  return CanPrefixQualifiers;
}

void CppTypePrinter::printBefore(QualType T, raw_ostream &OS) {
  SplitQualType Split = T.split();

  // If we have cv1 T, where T is substituted for cv2 U, only print cv1 - cv2
  // at this level.
  Qualifiers Quals = Split.Quals;
  if (const SubstTemplateTypeParmType *Subst =
        dyn_cast<SubstTemplateTypeParmType>(Split.Ty))
    Quals -= QualType(Subst, 0).getQualifiers();

  printBefore(Split.Ty, Quals, OS);
}

/// \brief Prints the part of the type string before an identifier, e.g. for
/// "int foo[10]" it prints "int ".
void CppTypePrinter::printBefore(const Type *T,Qualifiers Quals, raw_ostream &OS) {
  if (Policy.SuppressSpecifiers && T->isSpecifierType())
    return;

  SaveAndRestore<bool> PrevPHIsEmpty(HasEmptyPlaceHolder);

  // Print qualifiers as appropriate.

  bool CanPrefixQualifiers = false;
  bool NeedARCStrongQualifier = false;
  CanPrefixQualifiers = canPrefixQualifiers(T, NeedARCStrongQualifier);

  if (CanPrefixQualifiers && !Quals.empty()) {
    if (NeedARCStrongQualifier) {
      IncludeStrongLifetimeRAII Strong(Policy);
      Quals.print(OS, Policy, /*appendSpaceIfNonEmpty=*/true);
    } else {
      Quals.print(OS, Policy, /*appendSpaceIfNonEmpty=*/true);
    }
  }

  bool hasAfterQuals = false;
  if (!CanPrefixQualifiers && !Quals.empty()) {
    hasAfterQuals = !Quals.isEmptyWhenPrinted(Policy);
    if (hasAfterQuals)
      HasEmptyPlaceHolder = false;
  }

  switch (T->getTypeClass()) {
#define ABSTRACT_TYPE(CLASS, PARENT)
#define TYPE(CLASS, PARENT) case Type::CLASS: \
    print##CLASS##Before(cast<CLASS##Type>(T), OS); \
    break;
#include "clang/AST/TypeNodes.def"
  }

  if (hasAfterQuals) {
    if (NeedARCStrongQualifier) {
      IncludeStrongLifetimeRAII Strong(Policy);
      Quals.print(OS, Policy, /*appendSpaceIfNonEmpty=*/!PrevPHIsEmpty.get());
    } else {
      Quals.print(OS, Policy, /*appendSpaceIfNonEmpty=*/!PrevPHIsEmpty.get());
    }
  }
}

void CppTypePrinter::printAfter(QualType t, raw_ostream &OS) {
  SplitQualType split = t.split();
  printAfter(split.Ty, split.Quals, OS);
}

/// \brief Prints the part of the type string after an identifier, e.g. for
/// "int foo[10]" it prints "[10]".
void CppTypePrinter::printAfter(const Type *T, Qualifiers Quals, raw_ostream &OS) {
  switch (T->getTypeClass()) {
#define ABSTRACT_TYPE(CLASS, PARENT)
#define TYPE(CLASS, PARENT) case Type::CLASS: \
    print##CLASS##After(cast<CLASS##Type>(T), OS); \
    break;
#include "clang/AST/TypeNodes.def"
  }
}

void CppTypePrinter::printBuiltinBefore(const BuiltinType *T, raw_ostream &OS) {
  OS << T->getName(Policy);
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printBuiltinAfter(const BuiltinType *T, raw_ostream &OS) { }

void CppTypePrinter::printComplexBefore(const ComplexType *T, raw_ostream &OS) {
  OS << "_Complex ";
  printBefore(T->getElementType(), OS);
}
void CppTypePrinter::printComplexAfter(const ComplexType *T, raw_ostream &OS) {
  printAfter(T->getElementType(), OS);
}

void CppTypePrinter::printPointerBefore(const PointerType *T, raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printBefore(T->getPointeeType(), OS);
  // Handle things like 'int (*A)[4];' correctly.
  // FIXME: this should include vectors, but vectors use attributes I guess.
  if (isa<ArrayType>(T->getPointeeType()))
    OS << '(';
  OS << '*';
}
void CppTypePrinter::printPointerAfter(const PointerType *T, raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  // Handle things like 'int (*A)[4];' correctly.
  // FIXME: this should include vectors, but vectors use attributes I guess.
  if (isa<ArrayType>(T->getPointeeType()))
    OS << ')';
  printAfter(T->getPointeeType(), OS);
}

void CppTypePrinter::printBlockPointerBefore(const BlockPointerType *T,
                                          raw_ostream &OS) {
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printBefore(T->getPointeeType(), OS);
  OS << '^';
}
void CppTypePrinter::printBlockPointerAfter(const BlockPointerType *T,
                                          raw_ostream &OS) {
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printAfter(T->getPointeeType(), OS);
}

void CppTypePrinter::printLValueReferenceBefore(const LValueReferenceType *T,
                                             raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printBefore(T->getPointeeTypeAsWritten(), OS);
  // Handle things like 'int (&A)[4];' correctly.
  // FIXME: this should include vectors, but vectors use attributes I guess.
  if (isa<ArrayType>(T->getPointeeTypeAsWritten()))
    OS << '(';
  OS << '&';
}
void CppTypePrinter::printLValueReferenceAfter(const LValueReferenceType *T,
                                            raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  // Handle things like 'int (&A)[4];' correctly.
  // FIXME: this should include vectors, but vectors use attributes I guess.
  if (isa<ArrayType>(T->getPointeeTypeAsWritten()))
    OS << ')';
  printAfter(T->getPointeeTypeAsWritten(), OS);
}

void CppTypePrinter::printRValueReferenceBefore(const RValueReferenceType *T,
                                             raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printBefore(T->getPointeeTypeAsWritten(), OS);
  // Handle things like 'int (&&A)[4];' correctly.
  // FIXME: this should include vectors, but vectors use attributes I guess.
  if (isa<ArrayType>(T->getPointeeTypeAsWritten()))
    OS << '(';
  OS << "&&";
}
void CppTypePrinter::printRValueReferenceAfter(const RValueReferenceType *T,
                                            raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  // Handle things like 'int (&&A)[4];' correctly.
  // FIXME: this should include vectors, but vectors use attributes I guess.
  if (isa<ArrayType>(T->getPointeeTypeAsWritten()))
    OS << ')';
  printAfter(T->getPointeeTypeAsWritten(), OS);
}

void CppTypePrinter::printMemberPointerBefore(const MemberPointerType *T, 
                                           raw_ostream &OS) { 
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printBefore(T->getPointeeType(), OS);
  // Handle things like 'int (Cls::*A)[4];' correctly.
  // FIXME: this should include vectors, but vectors use attributes I guess.
  if (isa<ArrayType>(T->getPointeeType()))
    OS << '(';

  PrintingPolicy InnerPolicy(Policy);
  InnerPolicy.SuppressTag = false;
  CppTypePrinter(InnerPolicy).print(QualType(T->getClass(), 0), OS, StringRef());

  OS << "::*";
}
void CppTypePrinter::printMemberPointerAfter(const MemberPointerType *T, 
                                          raw_ostream &OS) { 
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  // Handle things like 'int (Cls::*A)[4];' correctly.
  // FIXME: this should include vectors, but vectors use attributes I guess.
  if (isa<ArrayType>(T->getPointeeType()))
    OS << ')';
  printAfter(T->getPointeeType(), OS);
}

void CppTypePrinter::printConstantArrayBefore(const ConstantArrayType *T, 
                                           raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printBefore(T->getElementType(), OS);
}
void CppTypePrinter::printConstantArrayAfter(const ConstantArrayType *T, 
                                          raw_ostream &OS) {
  OS << '[' << T->getSize().getZExtValue() << ']';
  printAfter(T->getElementType(), OS);
}

void CppTypePrinter::printIncompleteArrayBefore(const IncompleteArrayType *T, 
                                             raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printBefore(T->getElementType(), OS);
}
void CppTypePrinter::printIncompleteArrayAfter(const IncompleteArrayType *T, 
                                            raw_ostream &OS) {
  OS << "[]";
  printAfter(T->getElementType(), OS);
}

void CppTypePrinter::printVariableArrayBefore(const VariableArrayType *T, 
                                           raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printBefore(T->getElementType(), OS);
}
void CppTypePrinter::printVariableArrayAfter(const VariableArrayType *T, 
                                          raw_ostream &OS) {
  OS << '[';
  if (T->getIndexTypeQualifiers().hasQualifiers()) {
    AppendTypeQualList(OS, T->getIndexTypeCVRQualifiers());
    OS << ' ';
  }

  if (T->getSizeModifier() == VariableArrayType::Static)
    OS << "static";
  else if (T->getSizeModifier() == VariableArrayType::Star)
    OS << '*';

  if (T->getSizeExpr())
    T->getSizeExpr()->printPretty(OS, 0, Policy);
  OS << ']';

  printAfter(T->getElementType(), OS);
}

void CppTypePrinter::printDependentSizedArrayBefore(
                                               const DependentSizedArrayType *T, 
                                               raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  printBefore(T->getElementType(), OS);
}
void CppTypePrinter::printDependentSizedArrayAfter(
                                               const DependentSizedArrayType *T, 
                                               raw_ostream &OS) {
  OS << '[';
  if (T->getSizeExpr())
    T->getSizeExpr()->printPretty(OS, 0, Policy);
  OS << ']';
  printAfter(T->getElementType(), OS);
}

void CppTypePrinter::printDependentSizedExtVectorBefore(
                                          const DependentSizedExtVectorType *T, 
                                          raw_ostream &OS) { 
  printBefore(T->getElementType(), OS);
}
void CppTypePrinter::printDependentSizedExtVectorAfter(
                                          const DependentSizedExtVectorType *T, 
                                          raw_ostream &OS) { 
  OS << " __attribute__((ext_vector_type(";
  if (T->getSizeExpr())
    T->getSizeExpr()->printPretty(OS, 0, Policy);
  OS << ")))";  
  printAfter(T->getElementType(), OS);
}

void CppTypePrinter::printVectorBefore(const VectorType *T, raw_ostream &OS) { 
  switch (T->getVectorKind()) {
  case VectorType::AltiVecPixel:
    OS << "__vector __pixel ";
    break;
  case VectorType::AltiVecBool:
    OS << "__vector __bool ";
    printBefore(T->getElementType(), OS);
    break;
  case VectorType::AltiVecVector:
    OS << "__vector ";
    printBefore(T->getElementType(), OS);
    break;
  case VectorType::NeonVector:
    OS << "__attribute__((neon_vector_type("
       << T->getNumElements() << "))) ";
    printBefore(T->getElementType(), OS);
    break;
  case VectorType::NeonPolyVector:
    OS << "__attribute__((neon_polyvector_type(" <<
          T->getNumElements() << "))) ";
    printBefore(T->getElementType(), OS);
    break;
  case VectorType::GenericVector: {
    // FIXME: We prefer to print the size directly here, but have no way
    // to get the size of the type.
    OS << "__attribute__((__vector_size__("
       << T->getNumElements()
       << " * sizeof(";
    print(T->getElementType(), OS, StringRef());
    OS << ")))) "; 
    printBefore(T->getElementType(), OS);
    break;
  }
  }
}
void CppTypePrinter::printVectorAfter(const VectorType *T, raw_ostream &OS) {
  printAfter(T->getElementType(), OS);
} 

void CppTypePrinter::printExtVectorBefore(const ExtVectorType *T,
                                       raw_ostream &OS) { 
  printBefore(T->getElementType(), OS);
}
void CppTypePrinter::printExtVectorAfter(const ExtVectorType *T, raw_ostream &OS) { 
  printAfter(T->getElementType(), OS);
  OS << " __attribute__((ext_vector_type(";
  OS << T->getNumElements();
  OS << ")))";
}

void CppTypePrinter::printFunctionProtoBefore(const FunctionProtoType *T, 
                                           raw_ostream &OS) {
  if (T->hasTrailingReturn()) {
    OS << "auto ";
    if (!HasEmptyPlaceHolder)
      OS << '(';
  } else {
    // If needed for precedence reasons, wrap the inner part in grouping parens.
    SaveAndRestore<bool> PrevPHIsEmpty(HasEmptyPlaceHolder, false);
    printBefore(T->getResultType(), OS);
    if (!PrevPHIsEmpty.get())
      OS << '(';
  }
}

void CppTypePrinter::printFunctionProtoAfter(const FunctionProtoType *T, 
                                          raw_ostream &OS) { 
  // If needed for precedence reasons, wrap the inner part in grouping parens.
  if (!HasEmptyPlaceHolder)
    OS << ')';
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);

  OS << '(';
  {
    ParamPolicyRAII ParamPolicy(Policy);
    for (unsigned i = 0, e = T->getNumArgs(); i != e; ++i) {
      if (i) OS << ", ";
      print(T->getArgType(i), OS, StringRef());
    }
  }
  
  if (T->isVariadic()) {
    if (T->getNumArgs())
      OS << ", ";
    OS << "...";
  } else if (T->getNumArgs() == 0 && !Policy.LangOpts.CPlusPlus) {
    // Do not emit int() if we have a proto, emit 'int(void)'.
    OS << "void";
  }
  
  OS << ')';

  FunctionType::ExtInfo Info = T->getExtInfo();
  switch(Info.getCC()) {
  case CC_Default: break;
  case CC_C:
    OS << " __attribute__((cdecl))";
    break;
  case CC_X86StdCall:
    OS << " __attribute__((stdcall))";
    break;
  case CC_X86FastCall:
    OS << " __attribute__((fastcall))";
    break;
  case CC_X86ThisCall:
    OS << " __attribute__((thiscall))";
    break;
  case CC_X86Pascal:
    OS << " __attribute__((pascal))";
    break;
  case CC_AAPCS:
    OS << " __attribute__((pcs(\"aapcs\")))";
    break;
  case CC_AAPCS_VFP:
    OS << " __attribute__((pcs(\"aapcs-vfp\")))";
    break;
  case CC_PnaclCall:
    OS << " __attribute__((pnaclcall))";
    break;
  case CC_IntelOclBicc:
    OS << " __attribute__((intel_ocl_bicc))";
    break;
  }
  if (Info.getNoReturn())
    OS << " __attribute__((noreturn))";
  if (Info.getRegParm())
    OS << " __attribute__((regparm ("
       << Info.getRegParm() << ")))";

  if (unsigned quals = T->getTypeQuals()) {
    OS << ' ';
    AppendTypeQualList(OS, quals);
  }

  switch (T->getRefQualifier()) {
  case RQ_None:
    break;
    
  case RQ_LValue:
    OS << " &";
    break;
    
  case RQ_RValue:
    OS << " &&";
    break;
  }
  T->printExceptionSpecification(OS, Policy);

  if (T->hasTrailingReturn()) {
    OS << " -> "; 
    print(T->getResultType(), OS, StringRef());
  } else
    printAfter(T->getResultType(), OS);
}

void CppTypePrinter::printFunctionNoProtoBefore(const FunctionNoProtoType *T, 
                                             raw_ostream &OS) { 
  // If needed for precedence reasons, wrap the inner part in grouping parens.
  SaveAndRestore<bool> PrevPHIsEmpty(HasEmptyPlaceHolder, false);
  printBefore(T->getResultType(), OS);
  if (!PrevPHIsEmpty.get())
    OS << '(';
}
void CppTypePrinter::printFunctionNoProtoAfter(const FunctionNoProtoType *T, 
                                            raw_ostream &OS) {
  // If needed for precedence reasons, wrap the inner part in grouping parens.
  if (!HasEmptyPlaceHolder)
    OS << ')';
  SaveAndRestore<bool> NonEmptyPH(HasEmptyPlaceHolder, false);
  
  OS << "()";
  if (T->getNoReturnAttr())
    OS << " __attribute__((noreturn))";
  printAfter(T->getResultType(), OS);
}

void CppTypePrinter::printTypeSpec(const NamedDecl *D, raw_ostream &OS) {
  IdentifierInfo *II = D->getIdentifier();
  OS << II->getName();
  spaceBeforePlaceHolder(OS);
}

void CppTypePrinter::printUnresolvedUsingBefore(const UnresolvedUsingType *T,
                                             raw_ostream &OS) {
  printTypeSpec(T->getDecl(), OS);
}
void CppTypePrinter::printUnresolvedUsingAfter(const UnresolvedUsingType *T,
                                             raw_ostream &OS) { }

void CppTypePrinter::printTypedefBefore(const TypedefType *T, raw_ostream &OS) { 
  printTypeSpec(T->getDecl(), OS);
}
void CppTypePrinter::printTypedefAfter(const TypedefType *T, raw_ostream &OS) { } 

void CppTypePrinter::printTypeOfExprBefore(const TypeOfExprType *T,
                                        raw_ostream &OS) {
  OS << "typeof ";
  T->getUnderlyingExpr()->printPretty(OS, 0, Policy);
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printTypeOfExprAfter(const TypeOfExprType *T,
                                       raw_ostream &OS) { }

void CppTypePrinter::printTypeOfBefore(const TypeOfType *T, raw_ostream &OS) { 
  OS << "typeof(";
  print(T->getUnderlyingType(), OS, StringRef());
  OS << ')';
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printTypeOfAfter(const TypeOfType *T, raw_ostream &OS) { } 

void CppTypePrinter::printDecltypeBefore(const DecltypeType *T, raw_ostream &OS) { 
  OS << "decltype(";
  T->getUnderlyingExpr()->printPretty(OS, 0, Policy);
  OS << ')';
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printDecltypeAfter(const DecltypeType *T, raw_ostream &OS) { } 

void CppTypePrinter::printUnaryTransformBefore(const UnaryTransformType *T,
                                            raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);

  switch (T->getUTTKind()) {
    case UnaryTransformType::EnumUnderlyingType:
      OS << "__underlying_type(";
      print(T->getBaseType(), OS, StringRef());
      OS << ')';
      spaceBeforePlaceHolder(OS);
      return;
  }

  printBefore(T->getBaseType(), OS);
}
void CppTypePrinter::printUnaryTransformAfter(const UnaryTransformType *T,
                                           raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);

  switch (T->getUTTKind()) {
    case UnaryTransformType::EnumUnderlyingType:
      return;
  }

  printAfter(T->getBaseType(), OS);
}

void CppTypePrinter::printAutoBefore(const AutoType *T, raw_ostream &OS) { 
  // If the type has been deduced, do not print 'auto'.
  if (!T->getDeducedType().isNull()) {
    printBefore(T->getDeducedType(), OS);
  } else {
    OS << (T->isDecltypeAuto() ? "decltype(auto)" : "auto");
    spaceBeforePlaceHolder(OS);
  }
}
void CppTypePrinter::printAutoAfter(const AutoType *T, raw_ostream &OS) { 
  // If the type has been deduced, do not print 'auto'.
  if (!T->getDeducedType().isNull())
    printAfter(T->getDeducedType(), OS);
}

void CppTypePrinter::printAtomicBefore(const AtomicType *T, raw_ostream &OS) {
  IncludeStrongLifetimeRAII Strong(Policy);

  OS << "_Atomic(";
  print(T->getValueType(), OS, StringRef());
  OS << ')';
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printAtomicAfter(const AtomicType *T, raw_ostream &OS) { }

/// Appends the given scope to the end of a string.
void CppTypePrinter::AppendScope(DeclContext *DC, raw_ostream &OS) {
  if (DC->isTranslationUnit()) return;
  if (DC->isFunctionOrMethod()) return;
  AppendScope(DC->getParent(), OS);

  if (NamespaceDecl *NS = dyn_cast<NamespaceDecl>(DC)) {
    if (Policy.SuppressUnwrittenScope && 
        (NS->isAnonymousNamespace() || NS->isInline()))
      return;
    if (NS->getIdentifier())
      OS << NS->getName() << "::";
    else
      OS << "<anonymous>::";
  } else if (ClassTemplateSpecializationDecl *Spec
               = dyn_cast<ClassTemplateSpecializationDecl>(DC)) {
    IncludeStrongLifetimeRAII Strong(Policy);
    OS << Spec->getIdentifier()->getName();
    const TemplateArgumentList &TemplateArgs = Spec->getTemplateArgs();
    TemplateSpecializationType::PrintTemplateArgumentList(OS,
                                            TemplateArgs.data(),
                                            TemplateArgs.size(),
                                            Policy);
    OS << "::";
  } else if (TagDecl *Tag = dyn_cast<TagDecl>(DC)) {
    if (TypedefNameDecl *Typedef = Tag->getTypedefNameForAnonDecl())
      OS << Typedef->getIdentifier()->getName() << "::";
    else if (Tag->getIdentifier())
      OS << Tag->getIdentifier()->getName() << "::";
    else
      return;
  }
}

void CppTypePrinter::printTag(TagDecl *D, raw_ostream &OS) {
  if (Policy.SuppressTag)
    return;

  bool HasKindDecoration = false;

  // bool SuppressTagKeyword
  //   = Policy.LangOpts.CPlusPlus || Policy.SuppressTagKeyword;

  // We don't print tags unless this is an elaborated type.
  // In C, we just assume every RecordType is an elaborated type.
  if (!(Policy.LangOpts.CPlusPlus || Policy.SuppressTagKeyword ||
        D->getTypedefNameForAnonDecl())) {
    HasKindDecoration = true;
    OS << D->getKindName();
    OS << ' ';
  }

  // Compute the full nested-name-specifier for this type.
  // In C, this will always be empty except when the type
  // being printed is anonymous within other Record.
  if (!Policy.SuppressScope)
    AppendScope(D->getDeclContext(), OS);

  if (const IdentifierInfo *II = D->getIdentifier())
    OS << II->getName();
  else if (TypedefNameDecl *Typedef = D->getTypedefNameForAnonDecl()) {
    assert(Typedef->getIdentifier() && "Typedef without identifier?");
    OS << Typedef->getIdentifier()->getName();
  } else {
    // Make an unambiguous representation for anonymous types, e.g.
    //   <anonymous enum at /usr/include/string.h:120:9>
    
    if (isa<CXXRecordDecl>(D) && cast<CXXRecordDecl>(D)->isLambda()) {
      OS << "<lambda";
      HasKindDecoration = true;
    } else {
      OS << "<anonymous";
    }
    
    if (Policy.AnonymousTagLocations) {
      // Suppress the redundant tag keyword if we just printed one.
      // We don't have to worry about ElaboratedTypes here because you can't
      // refer to an anonymous type with one.
      if (!HasKindDecoration)
        OS << " " << D->getKindName();

      PresumedLoc PLoc = D->getASTContext().getSourceManager().getPresumedLoc(
          D->getLocation());
      if (PLoc.isValid()) {
        OS << " at " << PLoc.getFilename()
           << ':' << PLoc.getLine()
           << ':' << PLoc.getColumn();
      }
    }
    
    OS << '>';
  }

  // If this is a class template specialization, print the template
  // arguments.
  if (ClassTemplateSpecializationDecl *Spec
        = dyn_cast<ClassTemplateSpecializationDecl>(D)) {
    const TemplateArgument *Args;
    unsigned NumArgs;
    if (TypeSourceInfo *TAW = Spec->getTypeAsWritten()) {
      const TemplateSpecializationType *TST =
        cast<TemplateSpecializationType>(TAW->getType());
      Args = TST->getArgs();
      NumArgs = TST->getNumArgs();
    } else {
      const TemplateArgumentList &TemplateArgs = Spec->getTemplateArgs();
      Args = TemplateArgs.data();
      NumArgs = TemplateArgs.size();
    }
    IncludeStrongLifetimeRAII Strong(Policy);
    TemplateSpecializationType::PrintTemplateArgumentList(OS,
                                                          Args, NumArgs,
                                                          Policy);
  }

  spaceBeforePlaceHolder(OS);
}

void CppTypePrinter::printRecordBefore(const RecordType *T, raw_ostream &OS) {
  printTag(T->getDecl(), OS);
}
void CppTypePrinter::printRecordAfter(const RecordType *T, raw_ostream &OS) { }

void CppTypePrinter::printEnumBefore(const EnumType *T, raw_ostream &OS) { 
  printTag(T->getDecl(), OS);
}
void CppTypePrinter::printEnumAfter(const EnumType *T, raw_ostream &OS) { }

void CppTypePrinter::printTemplateTypeParmBefore(const TemplateTypeParmType *T, 
                                              raw_ostream &OS) { 
  if (IdentifierInfo *Id = T->getIdentifier())
    OS << Id->getName();
  else
    OS << "type-parameter-" << T->getDepth() << '-' << T->getIndex();
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printTemplateTypeParmAfter(const TemplateTypeParmType *T, 
                                             raw_ostream &OS) { } 

void CppTypePrinter::printSubstTemplateTypeParmBefore(
                                             const SubstTemplateTypeParmType *T, 
                                             raw_ostream &OS) { 
  IncludeStrongLifetimeRAII Strong(Policy);
  printBefore(T->getReplacementType(), OS);
}
void CppTypePrinter::printSubstTemplateTypeParmAfter(
                                             const SubstTemplateTypeParmType *T, 
                                             raw_ostream &OS) { 
  IncludeStrongLifetimeRAII Strong(Policy);
  printAfter(T->getReplacementType(), OS);
}

void CppTypePrinter::printSubstTemplateTypeParmPackBefore(
                                        const SubstTemplateTypeParmPackType *T, 
                                        raw_ostream &OS) { 
  IncludeStrongLifetimeRAII Strong(Policy);
  printTemplateTypeParmBefore(T->getReplacedParameter(), OS);
}
void CppTypePrinter::printSubstTemplateTypeParmPackAfter(
                                        const SubstTemplateTypeParmPackType *T, 
                                        raw_ostream &OS) { 
  IncludeStrongLifetimeRAII Strong(Policy);
  printTemplateTypeParmAfter(T->getReplacedParameter(), OS);
}

void CppTypePrinter::printTemplateSpecializationBefore(
                                            const TemplateSpecializationType *T, 
                                            raw_ostream &OS) { 
  IncludeStrongLifetimeRAII Strong(Policy);
  T->getTemplateName().print(OS, Policy);
  
  TemplateSpecializationType::PrintTemplateArgumentList(OS,
                                                        T->getArgs(), 
                                                        T->getNumArgs(), 
                                                        Policy);
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printTemplateSpecializationAfter(
                                            const TemplateSpecializationType *T, 
                                            raw_ostream &OS) { } 

void CppTypePrinter::printInjectedClassNameBefore(const InjectedClassNameType *T,
                                               raw_ostream &OS) {
  printTemplateSpecializationBefore(T->getInjectedTST(), OS);
}
void CppTypePrinter::printInjectedClassNameAfter(const InjectedClassNameType *T,
                                               raw_ostream &OS) { }

void CppTypePrinter::printElaboratedBefore(const ElaboratedType *T,
                                        raw_ostream &OS) {
  OS << TypeWithKeyword::getKeywordName(T->getKeyword());
  if (T->getKeyword() != ETK_None)
    OS << " ";
  NestedNameSpecifier* Qualifier = T->getQualifier();
  if (Qualifier)
    Qualifier->print(OS, Policy);
  
  ElaboratedTypePolicyRAII PolicyRAII(Policy);
  printBefore(T->getNamedType(), OS);
}
void CppTypePrinter::printElaboratedAfter(const ElaboratedType *T,
                                        raw_ostream &OS) {
  ElaboratedTypePolicyRAII PolicyRAII(Policy);
  printAfter(T->getNamedType(), OS);
}

void CppTypePrinter::printParenBefore(const ParenType *T, raw_ostream &OS) {
  if (!HasEmptyPlaceHolder && !isa<FunctionType>(T->getInnerType())) {
    printBefore(T->getInnerType(), OS);
    OS << '(';
  } else
    printBefore(T->getInnerType(), OS);
}
void CppTypePrinter::printParenAfter(const ParenType *T, raw_ostream &OS) {
  if (!HasEmptyPlaceHolder && !isa<FunctionType>(T->getInnerType())) {
    OS << ')';
    printAfter(T->getInnerType(), OS);
  } else
    printAfter(T->getInnerType(), OS);
}

void CppTypePrinter::printDependentNameBefore(const DependentNameType *T,
                                           raw_ostream &OS) { 
  OS << TypeWithKeyword::getKeywordName(T->getKeyword());
  if (T->getKeyword() != ETK_None)
    OS << " ";
  
  T->getQualifier()->print(OS, Policy);
  
  OS << T->getIdentifier()->getName();
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printDependentNameAfter(const DependentNameType *T,
                                          raw_ostream &OS) { } 

void CppTypePrinter::printDependentTemplateSpecializationBefore(
        const DependentTemplateSpecializationType *T, raw_ostream &OS) { 
  IncludeStrongLifetimeRAII Strong(Policy);
  
  OS << TypeWithKeyword::getKeywordName(T->getKeyword());
  if (T->getKeyword() != ETK_None)
    OS << " ";
  
  if (T->getQualifier())
    T->getQualifier()->print(OS, Policy);    
  OS << T->getIdentifier()->getName();
  TemplateSpecializationType::PrintTemplateArgumentList(OS,
                                                        T->getArgs(),
                                                        T->getNumArgs(),
                                                        Policy);
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printDependentTemplateSpecializationAfter(
        const DependentTemplateSpecializationType *T, raw_ostream &OS) { } 

void CppTypePrinter::printPackExpansionBefore(const PackExpansionType *T, 
                                           raw_ostream &OS) {
  printBefore(T->getPattern(), OS);
}
void CppTypePrinter::printPackExpansionAfter(const PackExpansionType *T, 
                                          raw_ostream &OS) {
  printAfter(T->getPattern(), OS);
  OS << "...";
}

void CppTypePrinter::printAttributedBefore(const AttributedType *T,
                                        raw_ostream &OS) {
  // Prefer the macro forms of the GC and ownership qualifiers.
  if (T->getAttrKind() == AttributedType::attr_objc_gc ||
      T->getAttrKind() == AttributedType::attr_objc_ownership)
    return printBefore(T->getEquivalentType(), OS);

  printBefore(T->getModifiedType(), OS);
}

void CppTypePrinter::printAttributedAfter(const AttributedType *T,
                                       raw_ostream &OS) {
  // Prefer the macro forms of the GC and ownership qualifiers.
  if (T->getAttrKind() == AttributedType::attr_objc_gc ||
      T->getAttrKind() == AttributedType::attr_objc_ownership)
    return printAfter(T->getEquivalentType(), OS);

  // TODO: not all attributes are GCC-style attributes.
  OS << " __attribute__((";
  switch (T->getAttrKind()) {
  case AttributedType::attr_address_space:
    OS << "address_space(";
    OS << T->getEquivalentType().getAddressSpace();
    OS << ')';
    break;

  case AttributedType::attr_vector_size: {
    OS << "__vector_size__(";
    if (const VectorType *vector =T->getEquivalentType()->getAs<VectorType>()) {
      OS << vector->getNumElements();
      OS << " * sizeof(";
      print(vector->getElementType(), OS, StringRef());
      OS << ')';
    }
    OS << ')';
    break;
  }

  case AttributedType::attr_neon_vector_type:
  case AttributedType::attr_neon_polyvector_type: {
    if (T->getAttrKind() == AttributedType::attr_neon_vector_type)
      OS << "neon_vector_type(";
    else
      OS << "neon_polyvector_type(";
    const VectorType *vector = T->getEquivalentType()->getAs<VectorType>();
    OS << vector->getNumElements();
    OS << ')';
    break;
  }

  case AttributedType::attr_regparm: {
    OS << "regparm(";
    QualType t = T->getEquivalentType();
    while (!t->isFunctionType())
      t = t->getPointeeType();
    OS << t->getAs<FunctionType>()->getRegParmType();
    OS << ')';
    break;
  }

  case AttributedType::attr_objc_gc: {
    OS << "objc_gc(";

    QualType tmp = T->getEquivalentType();
    while (tmp.getObjCGCAttr() == Qualifiers::GCNone) {
      QualType next = tmp->getPointeeType();
      if (next == tmp) break;
      tmp = next;
    }

    if (tmp.isObjCGCWeak())
      OS << "weak";
    else
      OS << "strong";
    OS << ')';
    break;
  }

  case AttributedType::attr_objc_ownership:
    OS << "objc_ownership(";
    switch (T->getEquivalentType().getObjCLifetime()) {
    case Qualifiers::OCL_None: llvm_unreachable("no ownership!");
    case Qualifiers::OCL_ExplicitNone: OS << "none"; break;
    case Qualifiers::OCL_Strong: OS << "strong"; break;
    case Qualifiers::OCL_Weak: OS << "weak"; break;
    case Qualifiers::OCL_Autoreleasing: OS << "autoreleasing"; break;
    }
    OS << ')';
    break;

  case AttributedType::attr_noreturn: OS << "noreturn"; break;
  case AttributedType::attr_cdecl: OS << "cdecl"; break;
  case AttributedType::attr_fastcall: OS << "fastcall"; break;
  case AttributedType::attr_stdcall: OS << "stdcall"; break;
  case AttributedType::attr_thiscall: OS << "thiscall"; break;
  case AttributedType::attr_pascal: OS << "pascal"; break;
  case AttributedType::attr_pcs: {
    OS << "pcs(";
   QualType t = T->getEquivalentType();
   while (!t->isFunctionType())
     t = t->getPointeeType();
   OS << (t->getAs<FunctionType>()->getCallConv() == CC_AAPCS ?
         "\"aapcs\"" : "\"aapcs-vfp\"");
   OS << ')';
   break;
  }
  case AttributedType::attr_pnaclcall: OS << "pnaclcall"; break;
  case AttributedType::attr_inteloclbicc: OS << "inteloclbicc"; break;
  }
  OS << "))";
}

void CppTypePrinter::printObjCInterfaceBefore(const ObjCInterfaceType *T, 
                                           raw_ostream &OS) { 
  OS << T->getDecl()->getName();
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printObjCInterfaceAfter(const ObjCInterfaceType *T, 
                                          raw_ostream &OS) { } 

void CppTypePrinter::printObjCObjectBefore(const ObjCObjectType *T,
                                        raw_ostream &OS) {
  if (T->qual_empty())
    return printBefore(T->getBaseType(), OS);

  print(T->getBaseType(), OS, StringRef());
  OS << '<';
  bool isFirst = true;
  for (ObjCObjectType::qual_iterator
         I = T->qual_begin(), E = T->qual_end(); I != E; ++I) {
    if (isFirst)
      isFirst = false;
    else
      OS << ',';
    OS << (*I)->getName();
  }
  OS << '>';
  spaceBeforePlaceHolder(OS);
}
void CppTypePrinter::printObjCObjectAfter(const ObjCObjectType *T,
                                        raw_ostream &OS) {
  if (T->qual_empty())
    return printAfter(T->getBaseType(), OS);
}

void CppTypePrinter::printObjCObjectPointerBefore(const ObjCObjectPointerType *T, 
                                               raw_ostream &OS) {
  T->getPointeeType().getLocalQualifiers().print(OS, Policy,
                                                /*appendSpaceIfNonEmpty=*/true);

  if (T->isObjCIdType() || T->isObjCQualifiedIdType())
    OS << "id";
  else if (T->isObjCClassType() || T->isObjCQualifiedClassType())
    OS << "Class";
  else if (T->isObjCSelType())
    OS << "SEL";
  else
    OS << T->getInterfaceDecl()->getName();
  
  if (!T->qual_empty()) {
    OS << '<';
    for (ObjCObjectPointerType::qual_iterator I = T->qual_begin(), 
                                              E = T->qual_end();
         I != E; ++I) {
      OS << (*I)->getName();
      if (I+1 != E)
        OS << ',';
    }
    OS << '>';
  }
  
  if (!T->isObjCIdType() && !T->isObjCQualifiedIdType()) {
    OS << " *"; // Don't forget the implicit pointer.
  } else {
    spaceBeforePlaceHolder(OS);
  }
}
void CppTypePrinter::printObjCObjectPointerAfter(const ObjCObjectPointerType *T, 
                                              raw_ostream &OS) { }


std::string cppPrintQualType(const QualType & qualType, const std::string &name)
{
	std::string text;

	llvm::raw_string_ostream stream(text);
	LangOptions langOptions;
	langOptions.CPlusPlus = 1;
	CppTypePrinter(PrintingPolicy(langOptions)).print(qualType, stream, name);

	return stream.str();
}
