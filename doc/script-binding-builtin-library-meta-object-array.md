

# Use built-in object array in cpgf script binding

## Overview

The built-in library builtin.collections.objectarray provides an object array class to ease script programming.

An object array manages a set of homogeneous elements in a block of continuous memory. All elements must be of the same class type.

The object array is introduced to simulate C++ array "MyClass arr[10]". With object array, we can create and manipulate C++ array in script languages.

To load the core library, in script, call "cpgf.import("cpgf", "builtin.collections.objectarray")".

Note:
  * To be consistent, all below sample script code is written in JavaScript.
  * All sample code assumes that cpgf core service is reflected in namespace "cpgf".

## Create an object array

Though we can allocate an object array, there is global function createObjectArray to create an object array.
```javascript

var myObjectArray = cpgf.createObjectArray(MyCppClass);
```
createObjectArray accepts one parameter, which is a C++ class that's bound to the script. createObjectArray returns an instance of the object array.

## Class GMetaObjectArray

Class GMetaObjectArray has a constructor, with the same signature as function createObjectArray, below code allocate an object array.
```javascript

var myObjectArray = new cpgf.GMetaObjectArray(MyCppClass);
```

Below is the C++ declaration of GMetaObjectArray
```c++

class GMetaObjectArray
{
public:
    explicit GMetaObjectArray(IMetaClass * metaClass);

    GVariant getItem(uint32_t index) const;
    void setItem(uint32_t index, const GVariant & value, const GMetaVariadicParam * moreValues = NULL);
	
    uint32_t getCount() const;
};
```

GMetaObjectArray has only three functions.

**getItem**

Get an object at index. The returned value is the address of the object, so any changes on the returned object will be applied into the array.

If the index is out of bounds, null value is returned.

Because getItem returns the object address, arr.getItem(10) is equivalent to &arr[10] in C++.

**setItem**

Set an object at index. We can pass more than one objects to the function.

If index is beyond the end of the array, the array will be expanded and all new allocated elements are the default value of the object (object constructed with default constructor).

**getCount**

Get the amount of the objects.
