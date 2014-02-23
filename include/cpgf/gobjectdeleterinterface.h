#ifndef CPGF_GOBJECTDELETERINTERFACE_H
#define CPGF_GOBJECTDELETERINTERFACE_H


#include "cpgf/gapiutil.h"
#include "cpgf/gtypeutil.h"


namespace cpgf {

class GObjectDeleterInterface : public IObject
{
	G_INTERFACE_IMPL_OBJECT
	
public:
	GObjectDeleterInterface(const GTypeDeleterCallback & deleter, void * instance)
		: deleter(deleter), instance(instance)
	{
	}
	
	virtual ~GObjectDeleterInterface() {
		this->deleter(this->instance);
	}
	
private:
	GTypeDeleterCallback deleter;
	void * instance;
};

template <typename T>
IObject * createObjectDeleterInterface(T * instance)
{
	return new GObjectDeleterInterface(GTypeDeleter_Delete<T>(), instance);
}


} // namespace cpgf



#endif

