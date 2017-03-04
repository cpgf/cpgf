
#ifdef _MSC_VER
	#define CPGF_MD_TEMPLATE
#endif

#ifdef G_COMPILER_CPPBUILDER
	#define CPGF_MD_TEMPLATE
#endif

#ifndef CPGF_MD_TEMPLATE
	#define CPGF_MD_TEMPLATE template
#endif

#define M(Type, Name) define.CPGF_MD_TEMPLATE _method(# Name, &Type::Name);
#define GM(Name) define.CPGF_MD_TEMPLATE _method(# Name, &Name);

#define ENUM_VALUE(Type, Name) .CPGF_MD_TEMPLATE _element(# Name, Type::Name)
#define GENUM_VALUE(Name) .CPGF_MD_TEMPLATE _element(# Name, Name)
