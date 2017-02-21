#ifndef CPGF_CPPSOURCEFILE_H
#define CPGF_CPPSOURCEFILE_H

#include <string>
#include <vector>

namespace metagen {

class CppSourceFile
{
public:
	explicit CppSourceFile(const std::string & fileName);
	~CppSourceFile();

	/*	Ideally the below functions should return const std::string & rather than const char *.
		However, these functions will be reflected to be used in the script.
		std::string is handled by meta traits, but seems the meta tratis doesn't work correctly
		in VC 2008. Not sure if it's a bug in VC 2008.
		For example, below code (it has memory leak, only for test purpose)
			#include "cpgf/metatraits/gmetaconverter_string.h"
			#include "cpgf/gmetaextendtype.h"
			#include <string>
			#include <iostream>
			using namespace cpgf;
			using namespace std;

			void test()
			{
				GMetaTraitsParam param;
				string * s = 0;
				IMetaConverter * a = createConverterFromMetaTraits(param, s);
				IMetaConverter * b = createMetaExtendType<string>(GExtendTypeCreateFlag_Converter).getConverter();

				cout << "a: " << a << " b: " << b << endl;
			}
		If the code is in a simple file, both a and b point to some valid interfaces.
		However, if the code is in a meta data file with some complicated cpgf headers included,
		a points to a valid interface, but b is NULL, because the traits for std::string doesn't effect.
		Very annoying.
	*/
//	const std::string & getFileName() const;
//	const std::string & getBaseFileName() const;
	const char * getFileName() const;
	const char * getBaseFileName() const;

	void setSkipBind(bool skip);
	bool shouldSkipBind() const;

	void addInclude(const char * include);
	const std::vector<std::string> & getIncludeList() const;

	void addMetaInclude(const char * include);
	const std::vector<std::string> & getMetaIncludeList() const;

private:
	std::string fileName;
	std::string baseName;
	bool skipBind;
	std::vector<std::string> includeList;
	std::vector<std::string> metaIncludeList;
};


} // namespace metagen


#endif
