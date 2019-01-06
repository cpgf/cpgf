

# Import libraries from script at runtime

Beside binding every bit of meta data to script engine at startup, cpgf script engine supports importing libraries at runtime, on demand.

This is similar as "require" in Lua, "import" in Python.

To using the importing mechanism, GScriptObject::bindScriptCoreService must be called to bind the core services.

Assume we use bindScriptCoreService to bind the service to namespace "cpgf", to import a library from script, just write the script code as,
```javascript

cpgf.import(Namespace, Libraryname);
```
import is the function to load the library. Note: to avoid name conflicting, there is another alias _import. So cpgf.import is same as cpgf._import.  
Namespace is a string of the namespace that the library is loaded into. If it's null, the library is loaded to the same namespace as the core service.  
Libraryname is a string of the library name you want to load.

## Library name convention

Library name can be any string. However, cpgf supports Java style dotted library name.

For example, both "Namespace1.Namespace2.LibraryName1" and "Namespace1.Namespace2.LibraryName2" are valid library name. Also if you are loading "Namespace1.Namespace2" or "Namespace1.Namespace2.*", the script engine will load all libraries under the same namespace.

## Builtin libraries

The long term goal is to support loading libraries from external shared libraries (.DLLs and .SOs). However, currently cpgf only supports builtin libraries.

The builtin libraries:

  * builtin.core -- support cloneClass and cast.
  * builtin.collections.bytearray -- byte array.
  * builtin.collections.objectarray -- object array.
  * builtin.debug -- debug trace, and int 3 break point.

## Examples for importing libraries

All the sample code below are written in Javascript.
```javascript

cpgf.import(null, "builtin.core");
```
Load core library to namespace "cpgf". Then to use function cast, we should write cpgf.cast(blah).
```javascript

cpgf.import("foo", "builtin.core");
```
Load core library to namespace "foo". Then to use function cast, we should write foo.cast(blah).
```javascript

cpgf.import("foo.bar", "builtin.core");
```
Load core library to namespace "foo.bar". Then to use function cast, we should write foo.bar.cast(blah).
```javascript

cpgf.import(null, "builtin.collections");
```
Load all libraries under namespace builtin.collections to namespace "cpgf". That is equivalent to
```javascript

cpgf.import(null, "builtin.collections.bytearray");
cpgf.import(null, "builtin.collections.objectarray");
```
```javascript

cpgf.import(null, "builtin");
```
Load all builtin libraries to namespace "cpgf". That is equivalent to
```javascript

cpgf.import(null, "builtin.core");
cpgf.import(null, "builtin.collections.bytearray");
cpgf.import(null, "builtin.collections.objectarray");
cpgf.import(null, "builtin.debug");
```

