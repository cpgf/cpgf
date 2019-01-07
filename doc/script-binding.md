# Binding Lua, Google V8 Javascript, Python and Mozilla SpiderMonkey engine to C++

<!-- toc -->

- [Overview](#overview)
- [Get start](#get-start)
- [Global script object VS. nested script object](#global-script-object-vs-nested-script-object)
- [Script object API](#script-object-api)
- [Utility API](#utility-api)
- [Passing script function and object to C++](#passing-script-function-and-object-to-c)

<!-- tocstop -->

## Overview

In this documentation, the phrase "script engine" represents any supported script engine, which is Lua, Google V8 JavaScript, Python or Mozilla SpiderMonkey.

The binding engine uses reflection meta data as the source. It doesn't require any external tools.

What can the binding engine do
  * Bind C++ class to the script engine. Then script engine can create instance, invoke class member function, get/set class field or property, and more.
  * Bind C++ enumerator type to script engine. Then script engine can access the enum value by name.
  * Bind C++ functions (global or member) to the script engine.
  * Bind C++ values to script engine variable.
  * Bind C++ operators to Lua and Python (Lua and Python only). The overloaded operators in C++ are bound to Lua and Python together with the class.
  * Invoke script engine functions from C++.
  * Get and set script engine value from C++.
  * Transfer binary data between C++ and script.
  * Pass script function and object to C++. 

## Get start

To binding Lua, #include "cpgf/scriptbind/gluabind.h"

For V8, #include "cpgf/scriptbind/gv8bind.h"

For Python, #include "cpgf/scriptbind/gpythonbind.h"

For Mozilla SpiderMonkey, #include "cpgf/scriptbind/gspidermonkeybind.h"

First, we need a global script object, GScriptObject or IScriptObject (the interface version).

Lua version:
```c++
GScriptObject * createLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config);
IScriptObject * createLuaScriptInterface(IMetaService * service, lua_State * L, const GScriptConfig & config);
```

The first parameter is a IMetaService. You can call createDefaultMetaService() or createMetaService() to create the service.  
The second parameter is the Lua state.  
The third one is a config. Just call GScriptConfig() to construct default one.

V8 version:
```c++
GScriptObject * createV8ScriptObject(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config);
IScriptObject * createV8ScriptInterface(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config);
```

The first parameter is a IMetaService. You can call createDefaultMetaService() or createMetaService() to create the service.  
The second parameter is the V8 global object.  
The third one is a config. Just call GScriptConfig() to construct default one.

Python version:
```c++
GScriptObject * createPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config);
IScriptObject * createPythonScriptInterface(IMetaService * service, PyObject * object, const GScriptConfig & config);
```

The first parameter is a IMetaService. You can call createDefaultMetaService() or createMetaService() to create the service.  
The second parameter is the Python object. You can use the object <nowiki>__main__</nowiki> as the root object.  
The third one is a config. Just call GScriptConfig() to construct default one.

Mozilla SpiderMonkey version:
```c++
JSObject * createSpiderMonkeyGlobaObject(JSContext * jsContext);
GScriptObject * createSpiderMonkeyScriptObject(IMetaService * service, JSContext * jsContext, JSObject  * jsObject, const GScriptConfig & config);
IScriptObject * createSpiderMonkeyScriptInterface(IMetaService * service, JSContext *jsContext, JSObject  * jsObject, const GScriptConfig & config);
```

The first parameter is a IMetaService. You can call createDefaultMetaService() or createMetaService() to create the service.  
The second parameter is the JSContext.  
The third parameter is the global object. You can call createSpiderMonkeyGlobaObject to create the global object.  
The fourth one is a config. Just call GScriptConfig() to construct default one.

The script object acts as a global scope. All meta data bound to this script object will be bound into Lua/V8 global space.  
New named scope can be create by calling GScriptObject::createScriptObject(TheName).  
Then all meta data bound to TheName can be accessed via TheName.  
Such as TheName.MyClass, etc.

After getting the GScriptObject object, we get everything ready to bind the meta data to the script engine.

## Global script object VS. nested script object

The APIs shows above create global script object. A global script object represent the whole scripting environment, it's the root object of every other objects. Anything that is set to a global object is accessed by the name directly. For example, if we have set a variable "myObj" to global object, in script we can write "myObj.foo = bar" to use myObj".

A nested script object is a kind of object that embeded in another object. It can be created by GScriptObject::createScriptObject. Assume we created a nested object named "myNested", and then set a variable "myOtherObj" to "myNested", then in script we can access myOtherObj as "myNested.myOtherObj.foo = bar".

## Script object API
```c++
GScriptValue getValue(const char * name);
```
Get a GScriptValue from the script object.  
"name" is the property name in the script object.
```c++
void setValue(const char * name, const GScriptValue & value);
```
Set a GScriptValue to the script object.  
"name" is the property name in the script object.  
"value" is the script value to be set.  
If value is a meta data (meta class, meta enum, meta method, meta property and field), the meta data is bound to the script object.  
If value is not a meta data, it's underlying value is set to the script object.
```c++
virtual GScriptValue createScriptObject(const char * name);
```
Create a new script object in current script object.  
The new script object can be used to bind meta data.  
On failure the function will fail and return GScriptValue().  
To get the IScriptObject pointer, call toScriptObject on the returned value.
```c++
virtual GVariant invoke(const char * name, const GVariant * params, size_t paramCount) = 0;
```
Invoke a function named "name" in the script.  
This can be any function, such as a function written in script, or a C++ function bound to the script.  
The parameters are passed as a pointer to a GVariant.
```c++
virtual GVariant invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount) = 0;
```
Same as "invoke", but the parameters are passed as a pointer to a GVariant pointer.
```c++
virtual void assignValue(const char * fromName, const char * toName) = 0;
```
Assign a script value named "fromName" to another script value named "toName".
```c++
virtual void bindCoreService(const char * name) = 0;
```
Bind core service to the script engine in namespace "name".  
The core service is a set of utility functions and objects to be used by the script.  
Currently there is only core service function, cloneClass.
```javascript
MetaClass cloneClass(MetaClass);
```
cloneClass recieves a meta class as parameter and return a clone of the meta class.  
cloneClass is used to inherit C++ class from script. For more information, please see the documentation of "inherit C++ class from script".

## Utility API

To easy the use, there are some utility APIs in gscriptbindutil.h
```c++
GScriptValue scriptGetValue(GScriptObject * scriptObject, const char * name);
GScriptValue scriptGetValue(IScriptObject * scriptObject, const char * name);
void scriptSetValue(GScriptObject * scriptObject, const char * name, const GScriptValue & value);
void scriptSetValue(IScriptObject * scriptObject, const char * name, const GScriptValue & value);
```

Set or get GScriptValue from a script object. When using the interfaced based API (IScriptObject), we should always use these function to set or get the values.
```c++
GVariant invokeScriptFunction(GScriptObject * scriptObject, const char * name, GVariant P1, GVariant P2, ..., GVariant PN);
GVariant invokeScriptFunction(IScriptObject * scriptObject, const char * name, GVariant P1, GVariant P2, ..., GVariant PN);
```

Invoke a script function.  
Example,
```c++
GVariant result = invokeScriptFunction(binding, "funcAdd", 8, 2);
```
```c++
IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject);
```
Wrap GScriptObject object to IScriptObject.  
Note: the returned interface will not own the scriptObject. The caller should delete scriptObject.
```c++
IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject, bool freeObject);
```
If freeObject is true, the returned interface will free the script object.
```c++
void injectMetaClassToScript(IScriptObject * scriptObject, IMetaClass * metaClass, void * instance);
```
Inject a class to the script. It will bind all methods, enumerators, nested class, in metaClass, to scriptObject in global scope.  
"instance" is the object if metaClass is not global.  
The function is usually only useful to bind the whole global meta class.  
So if metaClass is a top level global class, calling
```c++
injectMetaClassToScript(myScriptObject, globalClass, NULL);
```
will bind everything in the meta system to the script.

## Passing script function and object to C++

The script function and object here are the function and object defined in the script, not exposed by C++.  
When the script is calling a C++ function, or setting a property, a script function will be wrapped in a IScriptFunction interface, and a script object will be wrapped in a IScriptObject interface.  
Then the C++ code can use the interfaces to access the script function and object.

Notes:
  - If the script interface is passed as method parameter, don't release it. The caller will release it.
  - Don't pass script interface to field. You should use property with getter and setter to set the data. In the setter, addReference must be called on the interface to retain it. Otherwise, the interface will be released by the script.
