#include "test_variant_common.h"

#include "cpgf/gvariant.h"
#include "cpgf/gapiutil.h"
#include "cpgf/gscopedinterface.h"

#define CLASS TestClass_Variant_Interface_ReferenceCount

using namespace std;
using namespace cpgf;

namespace Test_Variant_Interface_ReferenceCount { namespace {

int objectCount = 0;

class CLASS : public IObject
{
	G_INTERFACE_IMPL_OBJECT

public:
	CLASS() {
		++objectCount;
	}

	virtual ~CLASS() {
		--objectCount;
	}

	uint32_t getReferenceCount() {
		uint32_t referenceCount = this->addReference();
		this->releaseReference();
		return referenceCount - 1;
	}
};

GTEST(TestVariant_Interface_MustBeAddedReferenceByNewVariant)
{
	GEQUAL(0, objectCount);
	{
		CLASS * object = new CLASS;
		GScopedInterface<IObject> intf(object);

		GEQUAL(1, objectCount);
		GEQUAL(1, object->getReferenceCount());

		{
			GVariant v(intf.get());
			GEQUAL(2, object->getReferenceCount());
		}
		GEQUAL(1, object->getReferenceCount());
	}
	GEQUAL(0, objectCount);
}

GTEST(TestVariant_Interface_MustBeAddedReferenceByCopyVariant)
{
	GEQUAL(0, objectCount);
	{
		CLASS * object = new CLASS;
		GScopedInterface<IObject> intf(object);

		GEQUAL(1, objectCount);
		GEQUAL(1, object->getReferenceCount());

		{
			GVariant v1(intf.get());
			GEQUAL(2, object->getReferenceCount());

			GVariant v2(v1);
			GEQUAL(3, object->getReferenceCount());

			GVariant v3;
			v3 = v2;
			GEQUAL(4, object->getReferenceCount());
			
			v1 = "abc";

			GEQUAL(3, object->getReferenceCount());
		}
		GEQUAL(1, object->getReferenceCount());
	}
	GEQUAL(0, objectCount);
}

GTEST(TestVariant_Interface_MoveAndRvalueReference)
{
	GEQUAL(0, objectCount);
	{
		CLASS * object = new CLASS;
		GScopedInterface<IObject> intf(object);

		GEQUAL(1, objectCount);
		GEQUAL(1, object->getReferenceCount());

		{
			GVariant v1(intf.get());
			GEQUAL(2, object->getReferenceCount());

			GVariant v2(std::move(v1));
			GEQUAL(2, object->getReferenceCount());
		}
		GEQUAL(1, object->getReferenceCount());
	}
	GEQUAL(0, objectCount);
}


} }
