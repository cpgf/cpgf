#ifndef CPGF_GSCRIPTRUNNER_H
#define CPGF_GSCRIPTRUNNER_H

#include <string>
#include <memory>

namespace cpgf {


struct IMetaService;
struct IScriptObject;

class GScriptRunnerImplement;

class GScriptRunner
{
protected:
	GScriptRunner(GScriptRunnerImplement * implement);

public:
	~GScriptRunner();

public:
	void executeString(const char * code);
	bool executeFile(const char * fileName);

	IScriptObject * getScripeObject();

	IMetaService * getService();

private:
	std::unique_ptr<GScriptRunnerImplement> implement;

private:
	friend class GScriptRunnerImplement;
};



} // namespace cpgf



#endif
