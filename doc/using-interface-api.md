

# Using interface based API

## What is interface based API

The idea of interface based API is inspired by Windows Component Object Model.  
All APIs are exposed in interface form, which is a cluster of abstract virtual functions.

## Why and when to use interface based API

The most important difference between interface based API and class-function based API is, interface based API is binary compatible and class-function based API is not binary compatible.  
That's to say, if we need to invoke API cross dynamic libraries or modules, we should use interface based API.  
If we use class-function based API cross dynamic libraries, we must ensure all libraries are compiled with same compiler and with same compiling options, which is not realistic.

If we want to use the API in other languages such as C or Delphi, only interface based API can be used.  
Consider Microsoft MFC and COM. We can use COM in C or Delphi, but we can't use MFC in C or Delphi.

## Supporting cross dynamic library is important for reflection and script binding

Let's imagine we want to create a GUI visual editor. We may want to extend the editor by allowing new components be added via new dynamic library, without re-distribute the editor itself.  
Also we can see DLLs (the dynamic libraries) are how important in Windows development, COM development and .Net development.

## Obtain meta service interface for reflection

Include header file cpgf/gmetaapi.h, call createDefaultMetaService to create a default meta service.
```c++

IMetaService * createDefaultMetaService();
```

There two other functions related to the service.
```c++

IMetaModule * getMetaModule();
IMetaService * createMetaService(IMetaModule * primaryModule);
```

getMetaModule returns a module.  
A module is an executable, or a dynamic library.  
An IMetaModule represents the meta information of current module.  
Each module will have one and only one IMetaModule interface.

An IMetaService represents all meta information of the application, include the executable, any dynamic libraries.  
An application should have one and only one IMetaService, no matter how many modules there are.

## Obtain root interface for Lua binding

Include header file cpgf/scriptbind/gluabind.h, call createLuaScriptObject to get an IScriptObject interface.
```c++

IScriptObject * createLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config);
```

## Memory management in interface based API

Very like Windows Component Object Model, cpgf interface based API utilizes reference count based memory management.  
Two functions from the root interface IObject give the memory management,
```c++

virtual uint32_t G_API_CC addReference() = 0;
virtual uint32_t G_API_CC releaseReference() = 0;
```

## Memory management rules:
  - In-parameters -- the interface passed to another function -- should no reference count change (no add/release).
  - Out-parameters -- returned by another function from either the result or the parameter -- should be released by the caller.
  - Never try to free the interface memory by calling C++ delete or C free function. You can only call addReference or releaseReference. 

The easiest way to hold an interface is to store it into a scoped pointer. 、、
GScopedInterface (in gapi.h) 、、
Then you don't need to call releaseReference manually, GScopedInterface will do it for you. 、、
GScopedInterface is the scoped pointer (or auto pointer) for interface.

## Learning interface based API

I won't repeat each interface and each function in the document because it's not necessary. 、、
The interface based API is quite similar with class-function based API.

Here only lists the different aspects between the two set of API. 、、
Interface will refer to interface based API, and CF will refer to class-function based API.

Aspect -- Class/Interface prefix  
Interface: Prefix is I. Example: IMetaMethod.  
CF: Prefix is G. Example: GMetaMethod.

Aspect -- Calling convention  
Interface: G_API_CC. Which is stdcall on Windows, cdecl on other platform, by default.  
CF: cdecl.

Aspect -- Meta value  
Interface: GVariantData.  
Example:
```c++

void G_API_CC get(GVariantData * outResult, void * instance); // in IMetaField
```
CF: GVariant.  
Example:
```c++

GVariant get(void * instance); // in GMetaField
```

Aspect -- Meta type  
Interface: GMetaTypeData  
Example:  
```c++

void G_API_CC getParamType(GMetaTypeData * outType, uint32_t index); // in IMetaMethod
```
CF: GMetaType  
Example:  
```c++

GMetaType getParamType(size_t index); // in GMetaMethod
```

Aspect -- how functions return aggregate data (GVariant/GVariantData, GMetaType/GMetaTypeData).  
Interface: Return in first parameter which is a pointer to the data.  
Example:  
```c++

void G_API_CC getParamType(GMetaTypeData * outType, uint32_t index); // in IMetaMethod
```
CF: Return as the C++ function return value.  
Example:  
```c++

GMetaType getParamType(size_t index); // in GMetaMethod
```

## Comparison of class-function based API and interface based API

^Feature 	^Class-function based API 	^Interface based API^
|Binary compatible 	|No 	|Yes|
|Memory management 	|Raw memory management. Using new and delete. 	|Reference count. Using addReference and releaseReference.|
|Variant type in API 	|GVariant 	|GVariantData|
|Meta type in API 	|GMetaType 	|GMetaTypeData|
|Scoped pointer 	|GScopedPointer (in gscopedptr.h) 	|GScopedInterface (in gapi.h)|
|Error handling 	|Throw exception 	|Error code |
