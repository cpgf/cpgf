
#ifdef _MSC_VER
	#define CPGF_MD_STL_TEMPLATE

	#if _MSC_VER >= 1500
		#define CPGF_MD_STL_QUIRK_CONST_ITERATOR() 1
	#endif
#endif

#ifdef G_COMPILER_CPPBUILDER
	#define CPGF_MD_STL_TEMPLATE
#endif

#ifndef CPGF_MD_STL_TEMPLATE
	#define CPGF_MD_STL_TEMPLATE template
#endif

#ifndef CPGF_MD_STL_QUIRK_CONST_ITERATOR
	#define CPGF_MD_STL_QUIRK_CONST_ITERATOR() 0
#endif


