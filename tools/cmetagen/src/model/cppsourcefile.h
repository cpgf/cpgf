#ifndef __CPPSOURCEFILE_H
#define __CPPSOURCEFILE_H

#include <string>


namespace metagen {

class CppSourceFile
{
public:
	explicit CppSourceFile(const std::string & fileName);
	~CppSourceFile();

	void setSkipBind(bool skip);
	bool shouldSkipBind() const;

	const std::string & getFileName() const;
	std::string getBaseFileName() const;

private:
	std::string fileName;
	bool skipBind;
};


} // namespace metagen


#endif
