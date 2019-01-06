# Using class GScriptValue

## Overview

GScriptValue is the core type used to exchange data with script binding engine. A GScriptValue can represent a meta class, an object, a string, a fundamental value, and so on. GScriptValue is copyable and is safe to pass around.

## Construct GScriptValue
```c++

GScriptValue();
GScriptValue(const GScriptValue & other);
GScriptValue & operator = (const GScriptValue & other);
```

GScriptValue can be default constructed and the value is a null pointer which is not quite meaningful. GScriptValue can be copied and assigned.

## Construct GScriptValue with given value

GScriptValue utilizes "named constructor idiom" to construct from given values. Below lists a bunch of static functions in GScriptValue, which accept values and return a GScriptValue.
```c++

static GScriptValue fromNull();
```
Construct a GScriptValue with null pointer.
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```
Construct a GScriptValue with primary value. A primary value is a boolean, integer, float point, string, or wide string. For example, "GScriptValue::fromPrimary(5)" will construct a GScriptValue with value 5.
```c++

static GScriptValue fromClass(IMetaClass * metaClass);
```
Construct a GScriptValue with a meta class. Setting the GScriptValue to script engine will cause the meta class is bound to the script engine.

After a meta class is set to the script engine, the script can create objects from the class.

In Lua, the script can always invoke the class name to construct new instance.  
a = MyClass()  
a is constructed with the default constructor.  
b = MyClass(5)  
b is constructed with the constructor that accepts an integer.

In Google V8 and Mozilla SpiderMonkey JavaScript, Python, the keyword "new" must be used to invoke the constructor.  
a = new MyClass()  
a is constructed with the default constructor.  
b = new MyClass(5)  
b is constructed with the constructor that accepts an integer.
```c++

static GScriptValue fromObject(const GVariant & instance,
    IMetaClass * metaClass, bool transferOwnership);
```
Construct a GScriptValue with an object.  
Parameter "instance" is the object address. It can be a pointer, or a GVariant created by function createVariant(const T & value, bool copyObject). We can set copyObject to true so the "instance" can hold and manage the object lifetime.  
Parameter "metaClass" is the meta class of the object.  
Parameter "transferOwnership", if it's true, after setting to script engine, the script engine will free the object. If it's false, the script engine will not free the object.
```c++

static GScriptValue fromMethod(void * instance, IMetaMethod * method);
```
Construct a GScriptValue with a method.  
Parameter "instance" is the object address points to the method object. If method is a global function, pass NULL to instance.  
Parameter "method" is the meta method.

Note: only set a meta method to script engine to bind method in global scope. You don't need to bind member function for class. Setting a meta class to script engine will do it for you.
```c++

static GScriptValue fromOverloadedMethods(IMetaList * methods);
```
Construct a GScriptValue with a bunch of overloaded methods.  
Parameter "methods" is a meta list holding the methods.  
To create a IMetaList, call "IMetaList * createMetaList();" in gmetaapi.h
```c++

static GScriptValue fromEnum(IMetaEnum * metaEnum);
```
Construct a GScriptValue with a C++ Enum.
```c++

static GScriptValue fromRaw(const GVariant & raw);
```
Construct a GScriptValue with a raw value.  
A raw value is not recognized by the script engine.  
A raw value can be passed around within the script, and between script and C++.
```c++

static GScriptValue fromAccessible(void * instance, IMetaAccessible * accessible);
```
Construct a GScriptValue with a property or field.  
Parameter "instance" is the object address points to the property or field object. If property of field is global, pass NULL to instance.  
Parameter "accessible" is the meta property or field.

**Caveats for setting an accessible to script engine**
  * The method bindAccessible may be not supported by all script engines. Though it's supported now.
  * Lua binding performance may be slow. Binding an accessible to a script object may slow down the accessing to that script object. So if performance is important for you, don't bind accessible to global script object. The other script engines don't have this impact.
  * The syntax is weird in Python binding. When accessing a bound accessible, use obj.__get__(0).accessbileName rather than obj.accessbileName.
  * You should better not set an accessible to script engine when possible. Use object property instead.
```c++

static GScriptValue fromNull();
```0
Construct a GScriptValue with a script object.  
Note: a GScriptValue with script object value can't be set to the script engine.
```c++

static GScriptValue fromNull();
```1
Construct a GScriptValue with a script function.  
Note: a GScriptValue with script function value can't be set to the script engine.
```c++

static GScriptValue fromNull();
```2
Construct a GScriptValue with a script array.  
Note: a GScriptValue with script function value can't be set to the script engine.


## Get the underlying value from GScriptValue

On contrary to the construct functions, there are a bunch of functions to convert a GScriptValue to underlying value.
```c++

static GScriptValue fromNull();
```3
Get the null value. It always returns (void *)0.  
If the value is not a null, return (void *)0. Thus this function can't fail.
```c++

static GScriptValue fromNull();
```4
Get the primary value.  
If the value is not a fundamental, return GVariant().
```c++

static GScriptValue fromNull();
```5
Get the meta class.  
If the value is not a meta class, return NULL.  
The caller must call releaseReference on the returned value.
```c++

static GScriptValue fromNull();
```6
Get the object. Returns the object address in GVariant.  
If the value is not an object, return GVariant().
outMetaClass and outTransferOwnership are used to receive the meta class and transferOwnership property. They can be NULL if the information is not needed.
The caller must call releaseReference on (*outMetaClass) if outMetaClass is not NULL.

Note: Don't free the returned object.
```c++

static GScriptValue fromNull();
```7
Similar as toObject, but this function returns the object address pointer.  
If the value is not an object, return NULL.
```c++

static GScriptValue fromNull();
```8
Get the method.  
outInstance is used to receive the instance. It can be NULL if the instance is not needed.  
If the value is not a method, return NULL.  
The caller must call releaseReference on the returned value.
```c++

static GScriptValue fromNull();
```9
Get the overloaded methods.  
If the value is not overloaded methods, return NULL.  
The caller must call releaseReference on the returned value.
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```0
Get the meta enum.  
If the value is not meta enum, return NULL.  
The caller must call releaseReference on the returned value.
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```1
Get the raw value.  
If the value is not raw value, return GVariant().
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```2
Get the property or field.  
outInstance is used to receive the instance. It can be NULL if the instance is not needed.  
If the value is not a property or field, return NULL.  
The caller must call releaseReference on the returned value.
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```3
Get the script object.  
If the value is not script object, return NULL.  
The caller must call releaseReference on the returned value.
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```4
Get the script function.  
If the value is not script function, return NULL.  
The caller must call releaseReference on the returned value.
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```5
Get the script array.  
If the value is not script arrayreturn NULL.  
The caller must call releaseReference on the returned value.

## Detect the type of a GScriptValue
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```6

Above lists the isXXX function to check if the GScriptValue is the type of XXX.
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```7
Get the type.
```c++

static GScriptValue fromPrimary(const GVariant & primary);
```8
