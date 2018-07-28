#include "cpgf/scriptbind/gscriptrunner.h"
#include "cpgf/private/gscriptrunner_p.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gmetaapi.h"

#include <fstream>
#include <vector>

using namespace std;


namespace cpgf {


GScriptRunnerImplement::GScriptRunnerImplement(IMetaService * service)
	: service(service)
{
	this->service->addReference();
}

GScriptRunnerImplement::~GScriptRunnerImplement()
{
}

GScriptRunner * GScriptRunnerImplement::createScriptRunner(GScriptRunnerImplement * implement)
{
	return new GScriptRunner(implement);
}


IMetaService * GScriptRunnerImplement::getService()
{
	this->service->addReference();
	return this->service.get();
}

IScriptObject * GScriptRunnerImplement::getScripeObject()
{
	this->scriptObject->addReference();
	return this->scriptObject.get();
}

void GScriptRunnerImplement::setScripeObject(IScriptObject * scriptObject)
{
	this->scriptObject.reset(scriptObject);
	this->scriptObject->addReference();
}

void GScriptRunnerImplement::finalize()
{
	this->scriptObject.reset();
	this->service.reset();
}


GScriptRunner::GScriptRunner(GScriptRunnerImplement * implement)
	: implement(implement)
{
}

GScriptRunner::~GScriptRunner()
{
}

void GScriptRunner::executeString(const char * code)
{
	this->implement->executeString(code);
}

bool GScriptRunner::executeFile(const char * fileName)
{
	ifstream stream;

	stream.open(fileName, ios::binary);
	if(! stream.good()) {
		return false;
	}
	stream.seekg(0, ios::end);
	long len = static_cast<long>(stream.tellg());
	stream.clear();
	stream.seekg(0, ios::beg);
	stream.clear();
	std::vector<char> buffer(len + 1);
	stream.read(buffer.data(), len);
	stream.close();
	buffer[len] = 0;

	this->implement->executeString(buffer.data());
	
	return true;
}

IScriptObject * GScriptRunner::getScripeObject()
{
	return this->implement->getScripeObject();
}

IMetaService * GScriptRunner::getService()
{
	return this->implement->getService();
}


} // namespace cpgf
