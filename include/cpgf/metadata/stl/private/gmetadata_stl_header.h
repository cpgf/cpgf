
#ifdef _MSC_VER
	#if _MSC_VER >= 1500
		#define QUIRK_CONST_ITERATOR() 1
		#define TEMPLATE
	#endif
#endif

#ifndef QUIRK_CONST_ITERATOR
	#define QUIRK_CONST_ITERATOR() 0
	#define TEMPLATE template
#endif


