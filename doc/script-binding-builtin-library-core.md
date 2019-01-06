

# Use built-in library builtin.core in cpgf script binding

## Overview

The built-in library builtin.core provides several core functions to ease script programming.

To load the core library, in script, call "cpgf.import("cpgf", "builtin.core")".

There are two global functions in the core library, cloneClass and cast.

Note:
  * To be consistent, all below sample script code is written in JavaScript.
  * All sample code assumes that cpgf core service is reflected in namespace "cpgf".


## Function cloneClass -- extend C++ code efficiently

The prototype is
```javascript
NewCppClass = cpgf.cloneClass(CppClass);
```
cloneClass takes one parameter, which is a C++ class. It returns another C++ class which is exactly same the parameter.

Sample code
```javascript
var MyNewCppClass = cpgf.cloneClass(MyCppClass);
var aNewInstance = new MyNewCppClass();
```

How can that be useful? That's useful to override C++ virtual function from script. In above code, if override the method foo in MyNewCppClass, such as MyNewCppClass.foo = function() { blah }, only MyNewCppClass is overrided, MyCppClass is not affected.

## Function cast -- Script version dynamic_cast

The prototype is
```javascript
newInstance = cpgf.cast(instance, CppClassToCastTo);
```

Assume we have C++ code as below
```c++
class B {
public:
  virtual ~B();
  void foo();
};

class D : public B {
public:
  void bar();
};

B * createD() {
  return new D();
}
```

Then in script if we write below code,
```javascript
var a = createD();
a.foo(); // OK
a.bar(); // ERROR. Can't find method bar.
```

The above C++ code is very common which return a base class pointer from a function. cpgf script binding engine is to mimic C++ behavior maximally. Since C++ is strong type and you can't get information in class D from pointer to B, the same is valid in cpgf script binding.

In C++, to use the return value of createD as D, we will use dynamic_cast and static_cast, in script, we use "cast".

cpgf.cast takes two parameters, the first parameter is an object instance, the second is the C++ class we want to cast to. The return value is the casted object instance, or null if the casting failed.

If the second parameter is omitted or it's null, cpgf will try to find the closest derived class to cast to. That's to say, cpgf will try to cast the object to the most appropriate class.

**Note**: the function cast uses C++ RTTI to achieve the function, that's to say, to use "cast", RTTI must be enabled in the compiler settings, and object class (it's B in the sample) must have virtual functions (which will have RTTI information).

So we can rewrite above script code
```javascript
var a = createD();
a = cpgf.cast(a, D);
// or just write as
// a = cpgf.cast(a);
// cpgf will cast a to D
a.foo(); // OK
a.bar(); // OK, works!
```
