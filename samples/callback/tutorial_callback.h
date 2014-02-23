#ifndef CPGF_TUTORIAL_CALLBACK_H
#define CPGF_TUTORIAL_CALLBACK_H

/**
How to read the tutorial
Tutorial 1 -- get start using callback (slot in Qt, boost::signals, and libsigc++)
Tutorial 2 -- get start using callback list (signal in Qt, boost::signals, and libsigc++)
Tutorial 3 -- auto disconnection
Tutorial 4 -- access the internal list and make advanced change to callback list

How to read the tutorial:
Each tutorial example is a class which name starts with Tutorial_.
Each class' constructor is the entry function of the tutorial.
All other functions in the class are accessorial.
*/

#include "cpgf/gcallbacklist.h"

/**
The namespace is cpgf.
*/
using namespace cpgf; // for tutorial only, usually don't do this in head file.

#include <string>
#include <iostream>
using namespace std; // for tutorial only again


// A global function to test using global function as callback
static int globalFunction(const string & s) {
	cout << "globalFunction: " << s << endl;
	return 5;
}

// A functor class to test using functor as callback
class SampleFunctor {
public:
	int operator () (const string & s) const {
		cout << "SampleFunctor: " << s << endl;
		return 0;
	}

	/* Define the == operator to always return true to make the functor comparable.
	   Otherwise, GCallback<int, const string &>(SampleFunctor()) will be not equal to another GCallback<int, const string &>(SampleFunctor())
	*/
	bool operator == (const SampleFunctor &) const {
		return true;
	}
};

// A normal class to test using member function as callback
class TutorialTestObject
{
public:
	// virtual or non-virtual doesn't matter for callback
	virtual int __stdcall objectFunction(const string & s) {
		cout << "objectFunction: " << s << endl;
		return 1;
	}

	static int staticFunction(const string & s) {
		cout << "staticFunction: " << s << endl;
		return 2;
	}
};

/**
Get start using callback (slot).
A single callback can be used where C style raw function pointer is used.
You can pass a callback, store it, assign new function to it, and invoke it.
*/
class Tutorial_GetStart_Callback
{
public:

	Tutorial_GetStart_Callback() {
		TutorialTestObject test;

		GCallback<int (const string &)> cb;

		cb = SampleFunctor();
		cout << cb("aaa") << endl;

		cb = GCallback<int (const string &)>(&test, &TutorialTestObject::objectFunction);
		// same as above line but simpler
		cb = makeCallback(&test, &TutorialTestObject::objectFunction);

		cout << cb("bbb") << endl;

		cb = &TutorialTestObject::staticFunction;
		cout << cb("ccc") << endl;

		cb = &globalFunction;
		cout << cb("ddd") << endl;
	}
};


/**
Get start using callback list (signal).
*/
class Tutorial_GetStart_CallbackList
{
public:

	Tutorial_GetStart_CallbackList() {
		TutorialTestObject test;

		GCallbackList<int (const string &)> cbList;

		cbList.add(SampleFunctor()); // the functor will be converted to callback implicitly
		cbList.add(&test, &TutorialTestObject::objectFunction);
		cbList.add(&TutorialTestObject::staticFunction); // the function will be converted to callback implicitly
		cbList.add(&globalFunction); // the function will be converted to callback implicitly

		cbList.dispatch("dispatch"); // call all callbacks

		// This will only succeed if an "always return true" == operator is defined for the SampleFunctor.
		// If no the == operator, to remove the functor, we need to either retain the reference to the functor object or add a special tag to it.
		cbList.remove(SampleFunctor());

		// Below will always work
		cbList.remove(&test, &TutorialTestObject::objectFunction);
		cbList.remove(&TutorialTestObject::staticFunction);
		cbList.remove(&globalFunction);

		// Now the list should be empty.
		cbList.dispatch("dispatch after remove. Should not happen."); // do nothing and can't see the message
	}
};

class SampleTrackable
{
public:
	void callback1() const {
		cout << "SampleTrackable callback1" << std::endl;
	}

public:
	GCallbackTrackable trackable;
};


/**
Auto disconnection.
Learn how to make a callback auto disconnect from callback list when the host object is destroyed.
*/
class Tutorial_AutoDisconnection
{
public:
	Tutorial_AutoDisconnection() {
		GCallbackList<void ()> cbList;

		// Scoped context
		{
			SampleTrackable obj;

			// The last parameter is the key for auto disconnection
			cbList.add(&obj, &SampleTrackable::callback1, &obj.trackable);

			cbList();
		}

		// Now obj is freed out of scope and the callback is auto removed from cbList

		cbList(); // does nothing
	}
};


/**
Access the internal list and make advanced change to callback list.
This is very advanced topic and usually you don't need to read this.
*/
class Tutorial_InternalList
{
public:
	Tutorial_InternalList() {
		// Define a type for callback list so we don't need to input the template parameters every time.
		// Good practice for callback list and callback.
		// To use the tag and auto remove feature in connection, GCallbackExtendedConnection must be specified as the second parameter of callback list.
		typedef GCallbackList<int (const string &), GCallbackExtendedConnection> CallbackListType;

		TutorialTestObject test;
		CallbackListType cbList;

		cbList.add(SampleFunctor());
		cbList.add(&test, &TutorialTestObject::objectFunction);
		cbList.add(&TutorialTestObject::staticFunction);
		cbList.add(&globalFunction);

		// Get the internal list. Don't copy it, just reference to it.
		CallbackListType::DirectListType & internalList = cbList.getDirectList();

		{
			CallbackListType::ConnectionType & connection = *internalList.begin();
			connection.autoRemoveAfterFirstCall();
		}

		// After the dispath, the first callback, SampleFunctor, will be auto removed due to autoRemoveAfterFirstCall.
		cbList("first dispatch");

		// This dispatch will not invoke SampleFunctor
		cbList("second dispatch");

		// Invoke each callback manually and output the return values.
		for(CallbackListType::IteratorType it = internalList.begin(); it != internalList.end(); ++it) {
			cout << it->getCallback().invoke("internal invoke") << endl;
		}

		// Reverse the callbacks order, fun but not very useful
		internalList.reverse();
		cbList("reversed");

		// Control the connections
		for(CallbackListType::IteratorType it = internalList.begin(); it != internalList.end(); ++it) {
			CallbackListType::ConnectionType & connection = *it;
			connection.setTag(10);
		}

		// Remove all callbacks one by one
		for(CallbackListType::IteratorType it = internalList.begin(); it != internalList.end(); ++it) {
			CallbackListType::ConnectionType & connection = *it;

			// Warning: Don't remove the iterator from internal list directly, otherwise the auto disconnection will take no effect and cause memory leak.
			// We should always remove callbacks from the callback list.
			cbList.remove(connection);
		}

		// We have removed all connections.
		// Below invoke will do nothing.
		cbList("should not happen");
	}
};


#define RUN_TUTORIAL(t) \
	cout << endl << # t << endl; \
	t();

// call this function to execute all tutorials
inline void runTutorials()
{
	RUN_TUTORIAL(Tutorial_GetStart_Callback);
	RUN_TUTORIAL(Tutorial_GetStart_CallbackList);
	RUN_TUTORIAL(Tutorial_AutoDisconnection);
	RUN_TUTORIAL(Tutorial_InternalList);
}



#endif
