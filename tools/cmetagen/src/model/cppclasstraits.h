#ifndef CPGF_CPPCLASSTRAITS_H
#define CPGF_CPPCLASSTRAITS_H

#include "cpgf/gflags.h"

namespace metagen {

class CppPolicy;

class CppClassTraits
{
private:
	enum TraitsFlag {
		tfDestructorHidden = 1 << 0,
		tfDefaultConstructorHidden = 1 << 1,
		tfCopyConstructorHidden = 1 << 2,
		tfHasTypeConvertConstructor = 1 << 3
	};

public:	
	bool isDestructorHidden() const;
	void setDestructorHidden(bool value);
	
	bool isDefaultConstructorHidden() const;
	void setDefaultConstructorHidden(bool value);
	
	bool isCopyConstructorHidden() const;
	void setCopyConstructorHidden(bool value);
	
	bool isHasTypeConvertConstructor() const;
	void setHasTypeConvertConstructor(bool value);

private:
	cpgf::GFlags<TraitsFlag> flags;
};


} // namespace metagen


#endif
