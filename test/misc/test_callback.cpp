#include "test_misc_common.h"

#include "cpgf/gcompiler.h"
#include "cpgf/gcallback.h"
#include "cpgf/gcallbacklist.h"

#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <functional>

#if defined(_MSC_VER)
	#define NON_INLINE __declspec(noinline)
	#define INT3 __asm int 3
#else
// gcc
	#define NON_INLINE __attribute__((noinline))
	#define INT3 asm("int $0x03")
#endif


#if defined(_MSC_VER)
// annoying warning "multiple copy constructors specified" in VC, generated from sample class CallbackSyntax copy constructor
#pragma warning(disable:4521)
#pragma warning(disable:4100)
#endif


using namespace cpgf;
using namespace std;

namespace Test_Callback { namespace {


class CallbackObject;

inline void testGlobalFunction() {
}

inline void callbackBindFirst(int a, int b) {
	GEQUAL(a, 1);
	GEQUAL(b, 2);
}

const int cvNone = 0;
const int cvConst = 1;
const int cvVolatile = 2;
const int cvConstVolatile = 3;

class CallbackSyntax {
public:
	CallbackSyntax() {
	}

	CallbackSyntax(const CallbackSyntax &) {
	}

	CallbackSyntax(const volatile CallbackSyntax & other) {
		if(&other) {}
	}

private:
	CallbackSyntax & operator = (const CallbackSyntax & /*other*/) {
		return *this;
	}

public:
	const int & intref_void() {
		return cvNone;
	}

	void void_void() {
	}

	void void_void_const() const {
	}

	void void_void_volatile() volatile {
	}

	void void_void_const_volatile() const volatile {
	}



	virtual void virtual_void_void() {
	}

	virtual void virtual_void_void_const() const {
	}

	virtual void virtual_void_void_volatile() volatile {
	}

	virtual void virtual_void_void_const_volatile() const volatile {
	}



	void overload_void_void() {
	}

	void overload_void_void() const {
	}
	
	void overload_void_void() volatile {
	}

	void overload_void_void() const volatile {
	}



	int operator () () {
		return cvNone;
	}

	int operator () () const {
		return cvConst;
	}

	int operator () () volatile {
		return cvVolatile;
	}

	int operator () () const volatile {
		return cvConstVolatile;
	}


	int int_void() {
		return cvNone;
	}

	int int_void_const() const {
		return cvConst;
	}

	int int_void_volatile() volatile {
		return cvVolatile;
	}

	int int_void_const_volatile() const volatile {
		return cvConstVolatile;
	}


	void operator () (int) {
	}

	void operator () (int) const {
	}

	void operator () (int) volatile {
	}

	void operator () (int) const volatile {
	}


	template <typename T>
	void overload_template_T(T) {
	}

	template <typename T>
	void overload_template_T(T) const {
	}

	template <typename T>
	void overload_template_T(T) volatile {
	}

	template <typename T>
	void overload_template_T(T) const volatile {
	}



	void G_API_CC stdcall_void_void(int a, std::string s) {
		GCHECK(a == 9 && s == "stdcall");
	}



	void G_API_CC overload_stdcall_void_void(int a, std::string s) {
		GCHECK(a == 9 && s == "stdcall");
	}

	void G_API_CC overload_stdcall_void_void(int a, std::string s) const {
		GCHECK(a == 9 && s == "stdcall");
	}

	void G_API_CC overload_stdcall_void_void(int a, std::string s) volatile {
		GCHECK(a == 9 && s == "stdcall");
	}

	void G_API_CC overload_stdcall_void_void(int a, std::string s) const volatile {
		GCHECK(a == 9 && s == "stdcall");
	}

	void overload_param(int) {
	}

	void overload_param(double) {
	}

	void overload_param(int, int, int = 0) {
	}


	int int_int_string(int a, std::string) const {
		return a + 1;
	}

private:
	char aaa[128];
};

class CallbackComparableObject {
public:
	void operator () () const {
	}

	void operator () () {
	}

	bool operator == (const CallbackComparableObject &) const {
		return true;
	}
};

class CallbackNotComparableObject {
public:
	void operator () () {
	}
};

class CallbackNoncopyableObject {
public:
	CallbackNoncopyableObject() {
	}

	void operator () () const {
	}

	void operator () () {
	}

private:
	CallbackNoncopyableObject(const CallbackNoncopyableObject &);
	CallbackNoncopyableObject & operator = (const CallbackNoncopyableObject &);
};

class CallbackObject
{
public:
	CallbackObject() : nnn(16)
//		, trackable(GCallbackTrackable())
	{
	}

	CallbackObject(const CallbackObject & other) {
		*this = other;
	}

	virtual ~CallbackObject() {
	}

	CallbackObject & operator = (const CallbackObject & other) {
		this->nnn = other.nnn;
//		this->trackable = other.trackable;

		return *this;
	}

	static void callbackStatic(int) {
	}

	void operator () (int) const {
	}

	int callback0() {
		return 3;
	}

	void /*G_API_CC MingW gcc 4.4.0 bug on __stdcall, uncomment this will cause wrong binary code*/ callback1(int) const {
	}

	void callback2(int) const {
		GEQUAL(this->nnn, 16);
	}

	void callback2(int, int) {
	}

	virtual void callback3(int, int)  {
	}

	int nnn;
//	GCallbackTrackable trackable;
};


class CallbackBase
{
public:
	virtual void virFunc1(int a) = 0;
};

class CallbackSon1 : virtual public CallbackBase
{
public:
	virtual void ZvirFunc1(int) {
	}
};

class CallbackSon2 : virtual public CallbackBase
{
public:
	virtual void YvirFunc1(int) {
	}
};

class CallbackGrandson : public CallbackSon1, public CallbackSon2
{
public:
	virtual void virFunc1(int) {
	}

	void func2(int) const {
	}
};

	template<typename T> struct yyy {};

	GTEST(testMisc)
	{
		bool b;

		b = typeid(GCallback<void (int)>) == typeid(GCallback<void (*)(int)>);
		GCHECK(!b);

//		typedef GCallbackList<void (int), GCallbackExtendedConnection> CallbackListType;
		typedef GCallbackList<void (int)> CallbackListType;

		CallbackObject * cbObject;
		CallbackListType * cbList;

		cbObject = new CallbackObject;
		cbList = new CallbackListType;

//		CallbackListType::ConnectionType * connection;

		GCallback<void (int, int)> cb(cbObject, &CallbackObject::callback3);
		cb.invoke(6, 8);

//		GEQUAL(cb, (GCallback<void (CallbackObject::*)(int, int)>(cbObject, &CallbackObject::callback3)));

		GCallback<void (int, int)>(cbObject, &CallbackObject::callback3).invoke(66, 88);

		CallbackObject obj = CallbackObject();
		GCallback<void (int)> cb2 = GCallback<void (int)>(obj);
		cb2.invoke(66);

		GEQUAL(makeCallback(cbObject, &CallbackObject::callback0)(), 3);

#ifndef G_COMPILER_CPPBUILDER // C++ Builder can't compile it
		GCallback<void (int)>(std::bind1st(std::ptr_fun(&callbackBindFirst), 1))(2);
#endif

//		CallbackObject * tempObject = new CallbackObject;
//		connection = cbList->add(makeCallback1(tempObject, &CallbackObject::callback2));
//		cbList->track(&tempObject->trackable, connection);
//		GCHECK(cbList->find(makeCallback1(tempObject, &CallbackObject::callback2)) != NULL);
//		delete tempObject;

//		cbList->add(cbObject, &CallbackObject::callback1);
//		GCHECK(cbList->find(cbObject, &CallbackObject::callback1) != NULL);

//		cbList->add(&CallbackObject::callbackStatic);
//		GCHECK(cbList->find(&CallbackObject::callbackStatic) != NULL);

//		connection = cbList->add(CallbackObject());
//		connection->autoRemoveAfterFirstCall();
//		cbList->add(CallbackObject());

		cbList->dispatch(1999);

		cbList->dispatch(1998);

		cbList->clear();

//		cbList->add(makeCallback1(cbObject, &CallbackObject::callback2));

		cbList->dispatch(1997);

		cbList->clear();
		cbList->dispatch(1);

		delete cbList;
		delete cbObject;
	}

	GTEST(testInherited)
	{
		typedef GCallback<void (int)> VirFunc1Type;
		CallbackGrandson grandson;

		VirFunc1Type cbVir;
		cbVir = VirFunc1Type(&grandson, &CallbackGrandson::virFunc1);
//		cbVir = VirFunc1Type(&grandson, &CallbackGrandson::func2);
		cbVir(1);
	}

	GTEST(testBasicCallback)
	{
//		GCallback<int (*)(int)> emptyCB;
//		emptyCB(5);

		CallbackSyntax obj;
		const CallbackSyntax constObj;
		volatile CallbackSyntax volatileObj;
		const volatile CallbackSyntax constVolatileObj;

		makeCallback(&obj, &CallbackSyntax::intref_void)();

		makeCallback(&obj, &CallbackSyntax::void_void)();
		makeCallback(&obj, &CallbackSyntax::void_void_const)();
		makeCallback(&obj, &CallbackSyntax::void_void_volatile)();
		makeCallback(&obj, &CallbackSyntax::void_void_const_volatile)();


		GEQUAL((GCallback<int ()>(&obj, &CallbackSyntax::int_void))(), cvNone);
		GEQUAL((GCallback<int ()>(&obj, &CallbackSyntax::int_void_const))(), cvConst);
		GEQUAL((GCallback<int ()>(&obj, &CallbackSyntax::int_void_volatile))(), cvVolatile);
		GEQUAL((GCallback<int ()>(&obj, &CallbackSyntax::int_void_const_volatile))(), cvConstVolatile);

		GEQUAL(makeCallback(&obj, &CallbackSyntax::int_void)(), cvNone);
		GEQUAL(makeCallback(&obj, &CallbackSyntax::int_void_const)(), cvConst);
		GEQUAL(makeCallback(&obj, &CallbackSyntax::int_void_volatile)(), cvVolatile);
		GEQUAL(makeCallback(&obj, &CallbackSyntax::int_void_const_volatile)(), cvConstVolatile);


		makeCallback(&constObj, &CallbackSyntax::void_void_const)();
		makeCallback(&constObj, &CallbackSyntax::void_void_const_volatile)();

		GCallback<void ()>(&obj, &CallbackSyntax::void_void)();
		GCallback<void ()>(&obj, &CallbackSyntax::void_void_const)();
		GCallback<void ()>(&obj, &CallbackSyntax::void_void_volatile)();
		GCallback<void ()>(&obj, &CallbackSyntax::void_void_const_volatile)();

		GCallback<void ()>(&constObj, &CallbackSyntax::void_void_const)();
		GCallback<void ()>(&constObj, &CallbackSyntax::void_void_const_volatile)();



		makeCallback(&obj, &CallbackSyntax::virtual_void_void)();
		makeCallback(&obj, &CallbackSyntax::virtual_void_void_const)();
		makeCallback(&obj, &CallbackSyntax::virtual_void_void_volatile)();
		makeCallback(&obj, &CallbackSyntax::virtual_void_void_const_volatile)();

		makeCallback(&constObj, &CallbackSyntax::virtual_void_void_const)();
		makeCallback(&constObj, &CallbackSyntax::virtual_void_void_const_volatile)();
		makeCallback(&volatileObj, &CallbackSyntax::virtual_void_void_volatile)();
		makeCallback(&constVolatileObj, &CallbackSyntax::virtual_void_void_const_volatile)();

		GCallback<void ()>(&obj, &CallbackSyntax::virtual_void_void)();
		GCallback<void ()>(&obj, &CallbackSyntax::virtual_void_void_const)();
		GCallback<void ()>(&obj, &CallbackSyntax::virtual_void_void_volatile)();
		GCallback<void ()>(&obj, &CallbackSyntax::virtual_void_void_const_volatile)();

		GCallback<void ()>(&constObj, &CallbackSyntax::virtual_void_void_const)();
		GCallback<void ()>(&constObj, &CallbackSyntax::virtual_void_void_const_volatile)();



		makeCallback(&obj, (void (CallbackSyntax::*)())&CallbackSyntax::overload_void_void)();
		makeCallback(&constObj, (void (CallbackSyntax::*)() const)&CallbackSyntax::overload_void_void)();
		makeCallback(&obj, (void (CallbackSyntax::*)() volatile)&CallbackSyntax::overload_void_void)();
		makeCallback(&obj, (void (CallbackSyntax::*)() const volatile)&CallbackSyntax::overload_void_void)();
		makeCallback(&constObj, (void (CallbackSyntax::*)() const volatile)&CallbackSyntax::overload_void_void)();

		GCallback<void ()>(&obj, (void (CallbackSyntax::*)())&CallbackSyntax::overload_void_void)();
		GCallback<void ()>(&constObj, (void (CallbackSyntax::*)() const)&CallbackSyntax::overload_void_void)();
		GCallback<void ()>(&obj, (void (CallbackSyntax::*)() volatile)&CallbackSyntax::overload_void_void)();
		GCallback<void ()>(&obj, (void (CallbackSyntax::*)() const volatile)&CallbackSyntax::overload_void_void)();
		GCallback<void ()>(&constObj, (void (CallbackSyntax::*)() const volatile)&CallbackSyntax::overload_void_void)();


		delete new GCallback<int ()>(CallbackSyntax());

		GEQUAL((GCallback<int ()>(obj))(), cvNone);
		GEQUAL((GCallback<int ()>(constObj))(), cvNone);
		GEQUAL((GCallback<int ()>(volatileObj))(), cvVolatile);
		GEQUAL((GCallback<int ()>(constVolatileObj))(), cvVolatile);


		(GCallback<int ()>(std::ref(obj)))();
		(GCallback<int ()>(std::ref(constObj)))();
		(GCallback<int ()>(std::ref(volatileObj)))();
		(GCallback<int ()>(std::ref(constVolatileObj)))();

#if 0
		GCallback<int ()> cb1(obj);
		cb1 = std::ref(cb1);
		cb1 = GCallback<int ()>(std::ref(cb1));
		cb1 = GCallback<int ()>(std::ref(std::ref(cb1)));
		cb1 = GCallback<int ()>(std::ref(std::cref(std::ref(cb1))));
		cb1();
#endif

		(GCallback<void (int)>(obj))(5);
		(GCallback<void (int)>(constObj))(5);
		(GCallback<void (int)>(volatileObj))(5);
		(GCallback<void (int)>(constVolatileObj))(5);


		(GCallback<void (int)>(std::ref(obj)))(5);
		(GCallback<void (int)>(std::ref(constObj)))(5);
		(GCallback<void (int)>(std::ref(volatileObj)))(5);
		(GCallback<void (int)>(std::ref(constVolatileObj)))(5);



		makeCallback(&obj, (void (CallbackSyntax::*)(int))&CallbackSyntax::overload_template_T<int>)(1999);
		makeCallback(&constObj, (void (CallbackSyntax::*)(int) const)&CallbackSyntax::overload_template_T<int>)(1999);
		makeCallback(&volatileObj, (void (CallbackSyntax::*)(int) volatile)&CallbackSyntax::overload_template_T<int>)(1999);
		makeCallback(&constVolatileObj, (void (CallbackSyntax::*)(int) const volatile)&CallbackSyntax::overload_template_T<int>)(1999);



		GEQUAL(6, (makeCallback(&obj, &CallbackSyntax::int_int_string)(5, "normal object")));
		GEQUAL(2000, (makeCallback(&constObj, &CallbackSyntax::int_int_string)(1999, "const object")));


		GCallback<void (int, std::string)>(&obj, &CallbackSyntax::stdcall_void_void)(9, "stdcall");
		GCallback<void (int, std::string)>(&obj, (void (G_API_CC CallbackSyntax::*)(int, std::string))&CallbackSyntax::overload_stdcall_void_void)(9, "stdcall");
		GCallback<void (int, std::string)>(&obj, (void (G_API_CC CallbackSyntax::*)(int, std::string) const)&CallbackSyntax::overload_stdcall_void_void)(9, "stdcall");
		GCallback<void (int, std::string)>(&obj, (void (G_API_CC CallbackSyntax::*)(int, std::string) volatile)&CallbackSyntax::overload_stdcall_void_void)(9, "stdcall");
		GCallback<void (int, std::string)>(&obj, (void (G_API_CC CallbackSyntax::*)(int, std::string) const volatile)&CallbackSyntax::overload_stdcall_void_void)(9, "stdcall");


		makeCallback(&obj, (void (CallbackSyntax::*)(int))&CallbackSyntax::overload_param)(9);
		makeCallback(&obj, (void (CallbackSyntax::*)(double))&CallbackSyntax::overload_param)(9.0);
		makeCallback(&obj, (void (CallbackSyntax::*)(int, int, int))&CallbackSyntax::overload_param)(9, 10, 11);

		CallbackNoncopyableObject noncopyable;
//		(GCallback<void ()>(noncopyable))(); // won't compile
		(GCallback<void ()>(std::ref(noncopyable)))();


		makeCallback(&testGlobalFunction)();
		(GCallback<void ()>(&testGlobalFunction))();

		void (*globalFunctionPtr)() = testGlobalFunction;
		makeCallback(globalFunctionPtr)();
		(GCallback<void ()>(globalFunctionPtr))();
	}

	GTEST(testGlobalCompare)
	{
		GCallback<void ()> cb1, cb2;
		void (*functionPtr)() = testGlobalFunction;

		cb1 = makeCallback(&testGlobalFunction);
		cb2 = (GCallback<void ()>(&testGlobalFunction));
		GCHECK(cb1 == cb2 && cb2 == cb1);

		cb1 = makeCallback(functionPtr);
		cb2 = (GCallback<void ()>(functionPtr));
		GCHECK(cb1 == cb2 && cb2 == cb1);

		cb2 = cb1;
		GCHECK(cb1 == cb2 && cb2 == cb1);

		cb2 = GCallback<void ()>(cb1);
        GCHECK(cb1 == cb2 && cb2 == cb1);

		CallbackComparableObject cobj1;
		CallbackComparableObject cobj2 = cobj1;
		cb1 = GCallback<void ()>(cobj1);
		cb2 = GCallback<void ()>(cobj2);
		GCHECK(cb1 == cb2 && cb2 == cb1);
		cb2 = cobj2;
		GCHECK(cb1 == cb2 && cb2 == cb1);
		cb1 = GCallback<void ()>(std::ref(std::ref(cobj1)));
		cb2 = GCallback<void ()>(std::ref(std::cref(std::ref(cobj2))));
		GCHECK(cb1 == cb2 && cb2 == cb1);

#ifndef G_COMPILER_CPPBUILDER // C++ Builder can't compile it
		cb2 = GCallback<void ()>(std::cref(std::ref(std::cref(cobj2))));
		GCHECK(cb1 == cb2 && cb2 == cb1);
#endif

//		cb1 = GCallback<void ()>(std::ref(std::cref(std::ref(std::cref(cb2)))));
//		GCHECK(cb1 == cb2 && cb2 == cb1);

		CallbackNotComparableObject nobj1;
		CallbackNotComparableObject nobj2;
		cb1 = GCallback<void ()>(nobj1);
		cb2 = GCallback<void ()>(nobj2);
		GCHECK(cb1 != cb2 && cb2 != cb1);
		cb1 = GCallback<void ()>(std::ref(nobj1));
		cb2 = GCallback<void ()>(std::ref(nobj2));
		GCHECK(cb1 != cb2 && cb2 != cb1);
		cb1 = GCallback<void ()>(std::ref(nobj1));
		cb2 = GCallback<void ()>(std::ref(nobj1));
		GCHECK(cb1 == cb2 && cb2 == cb1);
	}

	typedef GCallbackList<int (int)> CBListType;
	typedef GCallback<int (int)> CBType;
	class CallbackTestList
	{
	public:
		enum { paramValue = 18 };

		CallbackTestList(CBListType * cbList) : cbList(cbList) {
		}

		int operator () (int n) const {
			GCHECK(n == paramValue);

			return 1;
		}

		int callback1(int n) {
			GCHECK(n == paramValue);

			return 1;
		}

		int callback2(int n) const {
			GCHECK(n == paramValue);

			return 2;
		}

		static int callback3(int n) {
			GCHECK(n == paramValue);

			return 3;
		}

	private:
		CBListType * cbList;
	};
/*
    GTEST(testCallbackList)
    {
		CBListType * cbList = new CBListType;

		CallbackTestList * obj = new CallbackTestList(cbList);
		const CallbackTestList * constObj = new CallbackTestList(cbList);
		volatile CallbackTestList * volatileObj = new CallbackTestList(cbList);
		const volatile CallbackTestList * constVolatileObj = new CallbackTestList(cbList);

		CBType cb = CBType(obj, &CallbackTestList::callback1);
		cbList->add(cb);
		cb(CallbackTestList::paramValue);
		cbList->add(obj, &CallbackTestList::callback2);
		cbList->add(&CallbackTestList::callback3);
		cbList->add(constObj, &CallbackTestList::callback2);
		cbList->add(CallbackTestList(cbList));
		cbList->dispatch(CallbackTestList::paramValue);

		GCHECK(cbList->remove(obj, &CallbackTestList::callback2));
		GCHECK(cbList->remove(constObj, &CallbackTestList::callback2));
		cbList->dispatch(CallbackTestList::paramValue);

		CBListType copiedList0 = *cbList;
		CBListType copiedList;

		copiedList = copiedList0;

		delete obj;

        delete constObj;
        delete volatileObj;
		delete constVolatileObj;
		delete cbList;

		copiedList(CallbackTestList::paramValue);
	}
*/

/* disable trackable since our new GCallbackList abandoned it.
	class CallbackTestTrackable
	{
	public:
		void callback1() {
		}

		void callback2() {
		}

	public:
		GCallbackTrackable trackable;
	};

	GTEST(testTrackable)
	{
		GCallbackList<void ()> cbList, cbList2;

		CallbackTestTrackable * permanent;

		permanent = new CallbackTestTrackable;

		cbList.add(permanent, &CallbackTestTrackable::callback1, &permanent->trackable);

		CallbackTestTrackable trackable1;

		{
			CallbackTestTrackable temp1;
			temp1 = CallbackTestTrackable();
			cbList.add(&temp1, &CallbackTestTrackable::callback1, &temp1.trackable);
			cbList.add(&temp1, &CallbackTestTrackable::callback2, &temp1.trackable);

			cbList2.add(&temp1, &CallbackTestTrackable::callback1, &temp1.trackable);

			trackable1 = temp1;

			CallbackTestTrackable temp2 = temp1;
		}

		cbList.dispatch();
		cbList2.dispatch();

		delete permanent;
	}
*/

	class CallbackTestRecursive
	{
	public:
		typedef GCallback<int (int, const void *)> RecursiveCallbackType;
		int recursive1(int n, const void * p) {
			if(n <= 1) {
				return 1;
			}
			const RecursiveCallbackType * cb = (const RecursiveCallbackType *)p;
			return n + (cb ? (*cb)(n - 1, cb) : recursive1(n - 1, cb));
		}
	};

	GTEST(testRecursive)
	{
		const int iterateCount = 10;

		CallbackTestRecursive obj;

		CallbackTestRecursive::RecursiveCallbackType cb(&obj, &CallbackTestRecursive::recursive1);

		GEQUAL((obj.recursive1(iterateCount, NULL)), (cb(iterateCount, &cb)));
	}


// it's ok as long as below code won't cause compiler failure
inline void callback1() {
}

inline void callback2(int) {
}

inline void callback3(int) {
}

inline void callCB1(const GCallback<void ()> & cb) {
	cb();
}

inline void callCB1(const GCallback<void (int)> & cb) {
	cb(1);
}

inline void callCB1(const GCallback<void (int, int)> & cb) {
	cb(2, 3);
}

inline void doTestCompile() {
	callCB1(makeCallback(&callback2));
}


} }

