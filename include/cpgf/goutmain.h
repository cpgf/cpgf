#ifndef CPGF_GOUTMAIN_H
#define CPGF_GOUTMAIN_H

#include "cpgf/gpp.h"


#define G_RUN_BEFORE_MAIN(...) \
	namespace { \
	static struct GPP_CONCAT(I_rUnBeforemAin_, __LINE__) { GPP_CONCAT(I_rUnBeforemAin_, __LINE__)() {__VA_ARGS__ ;} } GPP_CONCAT(v_rUnBeforemAin_, __LINE__); \
	}

#define G_AUTO_RUN_BEFORE_MAIN(...) I_G_AUTO_RUN_BEFORE_MAIN(__VA_ARGS__ ## __LINE__)

#define I_G_AUTO_RUN_BEFORE_MAIN(ln) \
	void GPP_CONCAT(I_AutorUnBeforemAin_, ln)(); \
	G_RUN_BEFORE_MAIN(GPP_CONCAT(I_AutorUnBeforemAin_, ln)()); \
	void GPP_CONCAT(I_AutorUnBeforemAin_, ln)()



#endif
