# cpgf library -- general frequently asked questions

<!--begintoc-->
* [General](#a2_1)
  * [What's the exception safety in cpgf library?](#a3_1)
  * [What's the thread safety in cpgf library?](#a3_2)
  * [Some of the library document is bad, I want to help to rephrase it, how can I help?](#a3_3)
* [Reflection](#a2_2)
  * [When I build meta data for method, why the compiler issues warning saying "returning reference to temporary object"?](#a3_4)
  * [cpgf doesn't support reflecting meta data for namespace. I need namespace, please!](#a3_5)
  * [The compile time is super slow, how to boost it?](#a3_6)
  * [Should I reflect all class members even if I don't need all of them?](#a3_7)
* [Script binding (Lua, Google V8 Javascript and Python)](#a2_3)
  * [Why can 0 (zero) be passed as pointer in Javascript and Python, but not in Lua](#a3_8)
  * [When should I use bindMethod and bindMethodList?](#a3_9)
  * [Why the Google V8 binding is based on an old version V8 (v3.4.5)?](#a3_10)
<!--endtoc-->

<a id="a2_1"></a>
## General

<a id="a3_1"></a>
### What's the exception safety in cpgf library?

The library provides basic exception safety. That's to say, when exception occurs, no memory leak, and the object is not garbled. However, it's hard to guarantee the exception safety. So if you find anything that's not exception safety, let me know.

<a id="a3_2"></a>
### What's the thread safety in cpgf library?

The library is not safe for writing at the same time in multiple thread.

Due to the static objects and lazy meta data building, before the static objects intializes and lazy occurs. reading from multiple thread is not safe.

When all static objects initializes and lazy occurs, reading is safe in multiple thread.

For example, if you get a meta method in one thread, calling the method in several threads at the same time should be safe.

However, the library is not tested in multiple threads environment yet, so take your own risk.

<a id="a3_3"></a>
### Some of the library document is bad, I want to help to rephrase it, how can I help?

First, any intention of helping the library is appreciated.

Second, about the documentation. If you want to help in documentation randomly, just send me the revised documentation by mail. If you want to help continuously, I can create an editor account for you.

<a id="a2_2"></a>
## Reflection

<a id="a3_4"></a>
### When I build meta data for method, why the compiler issues warning saying "returning reference to temporary object"?

This is because either the parameter, or the return value, is a reference-to-const to an object which can be constructed implicitly from a fundamental (int, char, etc) type or pointer or reference to a fundamental type.

For example, if we have this method,
```c++
void setName(const std::string & name);
```
std::string can be constructed implicitly from const char *.

When this kind of method is reflected, GVariant will try to return reference to local temporary object.

That warning is dangerous, it may crash your program. To solve this issue, use policy GMetaPolicyCopyAllConstReference. That policy will force the parameter or return value being passed by value instead of reference.
```c++
._method("setName", &setName, GMetaPolicyCopyAllConstReference())
```


<a id="a3_5"></a>
### cpgf doesn't support reflecting meta data for namespace. I need namespace, please!

It's true that cpgf doesn't support reflecting namespace. There are two reasons for it.
  - Reflecting namespace will cause using meta data much more complicated.
  - Namespace in C++ is less important. It's mainly used to avoid naming clash.

However, we can simulate namespace reflection in cpgf perfectly.

There is a typedef in gmetadefine.h
```c++
typedef GDefineMetaClass<void> GDefineMetaNamespace;
```

GDefineMetaNamespace acts as a virtual meta class. A GDefineMetaNamespace can hold any meta data. A GDefineMetaNamespace can be added to global, or even another meta class (in C++ we can't have namespace in a class, but in cpgf we can).

The OpenGL and Box2D sample code in cpgf package demonstrate the use of namespace well.

<a id="a3_6"></a>
### The compile time is super slow, how to boost it?

Due to the heavy and deep recursive use of C++ template technology, the compile for meta data reflection is quite slow and consuming huge memory. It's quite possible that the compiler eats gigabytes memory to compile a single .cpp file which contains a lot of reflection templates.

Currently there is no good way to optimize it from the library side. The best way to boost the compile is to split the meta data reflection to several smaller .cpp file.

cpgf has very good support to split the reflection. To do it, GDefineMetaInfo is your good friend. There is good demonstration in the Box2D meta data in the library package. The tool metagen uses GDefineMetaInfo extensively.

Also be sure you have enough RAM memory. Usually 2G RAM is the minimum requirement. 4G RAM is preferred. Of course this depends on the scale of your project and which compiler you use.

Just to clarify, compiling code that reflects meta data is slow, but compiling code that uses meta data is not slow because there are very few templates involved.

<a id="a3_7"></a>
### Should I reflect all class members even if I don't need all of them?
No, you don't.

The philosophy in cpgf library is "you don't pay for what you don't use". You only need to reflect the class members that you need. If there are 10 functions in a class and you only need to access one function via reflection, you only need to reflect that one function. Another example, if you only need to construct objects from class name, you only need to reflect the constructors and don't reflect all other members.

<a id="a2_3"></a>
## Script binding (Lua, Google V8 Javascript and Python)

<a id="a3_8"></a>
### Why can 0 (zero) be passed as pointer in Javascript and Python, but not in Lua

Assume we have a C++ method "void foo(MyClass * p);" In JS and Python script, calling it by "foo(0)" works, but in Lua it will cause GVariant fail cast error. Why?

To be more compatible with C++ behavior, the integer 0 is special treated when converting to pointer. An integer 0 is converted to (T *)0 automatically. (Note any integer other than 0 is not converted to pointer automatically)

In Google V8 Javascript and Python's C/C++ interface, there is integer type. So cpgf knows 0 is an integer. However, there is no integer type in Lua, so 0 is passed as a float number to cpgf script binding engine. A float can't be converted to a pointer.

To avoid any cross script clash, we'd better always use null (in JS), nil (in Lua), and None (in Python) to represent NULL pointer rather than 0.

<a id="a3_9"></a>
### When should I use bindMethod and bindMethodList?

If you are only binding classes, never you need to call bindMethod or bindMethodList.  
cpgf will bind all class members automatically.

bindMethod and bindMethodList are useful if you want to bind methods to global scope under current script object.

<a id="a3_10"></a>
### Why the Google V8 binding is based on an old version V8 (v3.4.5)?

First, the V8 version used in cpgf is not very old. It should be already stable version.  
Second, the reason cpgf doesn't use an up to date V8 version is that, V8 is always using some exotic building system, from Scons to gyp, rather than makefile or CMake, which is widely accepted by C++ world.

It's very hard to get those weird building system working correctly under MS VC and MinGW, both of which are used to develop cpgf. You may check the user comments on V8 web page [here](*code.google.com/p/v8/wiki/BuildingOnWindows) and [here](*code.google.com/p/v8/wiki/BuildingWithGYP) to see how other V8 users said.

The total time I wasted on building V8 is much longer than total time I spent on building wxWidgets and Qt. I would like to invest my time on more valuable works, rather than building a third party library.

So let's stick with current V8 version 3.4.5, until the future V8 is not compatible with it.
