#ifndef CPGF_GAPIUTIL_H
#define CPGF_GAPIUTIL_H


#include "cpgf/gapi.h"

#include <string>
#include <memory>

#define G_INTERFACE_IMPL_OBJECT_DERIVED \
protected: \
	virtual uint32_t G_API_CC addReference() { return this->ginterface_implObject.addReference(); } \
	virtual uint32_t G_API_CC releaseReference() { return this->ginterface_implObject.releaseReference(this); }

#define G_INTERFACE_IMPL_OBJECT_DERIVED_OVERRIDE \
protected: \
	virtual uint32_t G_API_CC addReference() override { return this->ginterface_implObject.addReference(); } \
	virtual uint32_t G_API_CC releaseReference() override { return this->ginterface_implObject.releaseReference(this); }

#define G_INTERFACE_IMPL_OBJECT \
protected: \
	cpgf::GImplObject ginterface_implObject; \
	G_INTERFACE_IMPL_OBJECT_DERIVED

#define G_INTERFACE_IMPL_OBJECT_OVERRIDE \
protected: \
	cpgf::GImplObject ginterface_implObject; \
	G_INTERFACE_IMPL_OBJECT_DERIVED_OVERRIDE

#define G_INTERFACE_IMPL_EXTENDOBJECT_DERIVED \
protected: \
	virtual int32_t G_API_CC getErrorCode() { return this->ginterface_implExtendObject.getErrorCode(); } \
	virtual const char * G_API_CC getErrorMessage() { return this->ginterface_implExtendObject.getErrorMessage(); }

#define G_INTERFACE_IMPL_EXTENDOBJECT_DERIVED_OVERRIDE \
protected: \
	virtual int32_t G_API_CC getErrorCode() override { return this->ginterface_implExtendObject.getErrorCode(); } \
	virtual const char * G_API_CC getErrorMessage() override { return this->ginterface_implExtendObject.getErrorMessage(); }

#define G_INTERFACE_IMPL_EXTENDOBJECT \
protected: \
	cpgf::GImplExtendObject ginterface_implExtendObject; \
	G_INTERFACE_IMPL_EXTENDOBJECT_DERIVED

#define G_INTERFACE_IMPL_EXTENDOBJECT_OVERRIDE \
protected: \
	cpgf::GImplExtendObject ginterface_implExtendObject; \
	G_INTERFACE_IMPL_EXTENDOBJECT_DERIVED_OVERRIDE

namespace cpgf {


class GImplObject
{
public:
	GImplObject() : referenceCount(1) {
	}

	virtual ~GImplObject() {
	}

	uint32_t addReference() {
		return ++this->referenceCount;
	}

	template <typename T>
	uint32_t releaseReference(T * p)
	{
		if(this->referenceCount > 0) {
			--this->referenceCount;
		}

		unsigned int refCount = this->referenceCount;

		if(this->referenceCount == 0) {
			delete p;
		}

		return refCount;
	}

private:
	unsigned int referenceCount;
};

class GImplExtendObject : public GImplObject
{
private:
	struct ExtendObjectErrorInfo {
		int32_t errorCode;
		std::string message;
	};

public:
	GImplExtendObject();

	void clearError();
	void handleError(int errorCode, const char * message);

	int32_t getErrorCode();
	const char * getErrorMessage();

private:
	std::unique_ptr<ExtendObjectErrorInfo> errorInfo;
};

class GImplMemoryAllocator : public GImplObject, public IMemoryAllocator
{
	G_INTERFACE_IMPL_OBJECT_OVERRIDE

protected:
	virtual void * G_API_CC allocate(uint32_t size) override;
	virtual void G_API_CC free(void * p) override;
	virtual void * G_API_CC reallocate(void * p, uint32_t size) override;
};


} // namespace cpgf



#endif
