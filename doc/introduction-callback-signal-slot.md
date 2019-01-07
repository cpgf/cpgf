<!--notoc-->

# cpgf callback -- an open source library for C++ callback, signal, slot, delegate, observer patten, event system

cpgf callback is a callback system for C++. It's written heavily using C++ templates and preprocess macros.
## With this library, C++ programmers can easily do:

  * Define a callback object (also called slot in Qt, boost::Signals and libsigc++) to hold any functions and functor object with any parameters and return type.
  * Pass and store the callback object anywhere for any later invoke.
  * Define a callback list (also called signal in Qt, boost::Signals and libsigc++) to hold any number of callbacks together.
  * Call multiple callbacks at once via callback list.

## Features:

  * Pure C++ template code. No extra preprocessor or meta object compiler is required.
  * Support arbitrary  number of parameters.
  * Support any type of functions. Static (global) function, member function (virtual, const, volatile, const volatile), functor object ( () operator).
  * Support any calling conventions. cdecl, stdcall, fastcall, etc.
  * Comparable. Callbacks can be compared via == operator.
  * Auto disconnection management. A connection can be auto disconnected when host object is destroyed.  Use GCallbackTrackable. Even better, your object doesn't need to inherit from GCallbackTrackable. This feature was not tested thoroughly.
  * Customized connection type to make any extension. For instance, GCallbackExtendedConnection supports tag and auto remove features.
  * Fast and compact memory usage.
  * Better handling empty callback (no function is bound). Unlike boost::function and boost::signal (which either forbid empty signal or throw exception when calling an empty function), also unlike libsigc++ (silent for empty slot but cause compile error "invalid value-initialization of reference types" when the function type returns reference in GCC), cpgf callback keeps silent on empty callback and no compile error.
  * Same idea as signals and slots. GCallback is the slot and GCallbackList is the signal.
  * Quite easy use of syntax. Most time you can forget the template <> and use normal function address as the callback (implicitly converting).
  * Can work with const, volatile and const volatile member functions.
  * Can be invoked recursively.
  * Disconnect/delete immediately, no lazy deletion.
  * Can be used cross exe and dll, or cross different modules.
  * A number of auxiliary  functions to ease the creating of callbacks (makeCallback and makeCallbackN).
  * Easy to use override functions with different count of parameters (use makeCallbackN).
  * Header only. No library or source code to link with. Just include and start using it.
  * Not thread safe for now.
  * Test compiled on MS VC 2008 Express, GCC 4.4.0 and GCC 4.5.2.

 
## Here is a piece of code that the callback looks like
```c++
GCallback<void (int)> cb = someObj;
cb.invoke(66);

TestObject test;

GCallbackList<int (const string &)> cbList;

cbList.add(SampleFunctor()); // functor object
cbList.add(&test, &TestObject::objectFunction); // a member function
cbList.add(&TestObject::staticFunction); // static class function
cbList.add(&globalFunction); // global function

cbList.dispatch("dispatch"); // call all callbacks
```


## How may the callback library help your development?

  * It helps to loose couple your code. A module may only emit certain events (callbacks) and doesn't need to care about who will response them. Another module may only response to certain events without caring who emits them.
  * It helps to implement Observer pattern.
  * It's quite easy to develop an event system basing on the callback library.
  * Raw C style function pointer callbacks can be replaced with the library and get benefits of type safe.


## The callback library is the very core system in cpgf library

The callback library takes a very important role among the whole cpgf library. Callback is used to implement meta methods, meta constructors, meta operators, and meta property. We can proudly say, if there is no callback system, there is no cpgf library.

What does that mean? That means as long as cpgf, a library with tens and thousands lines of complicated C++ code, can work well with callback, you can adopt the callback library in your project well.
