

# Using GVariant

## Overview

The idea of GVariant is inspired by the similar Variant in Windows COM technology.  
A GVariant is used to represent arbitrary type of value.  
Any value, such as but not limited to, integer, float, pointer, object, reference, can be stored in a GVariant.

The reflection and script binding libraries use GVariant extensively. GVariant is the very core concept in those libraries.

Please bear in mind, GVariant is used in runtime, so it doesn't remember any compile time information, except the fundamental types.  
So GVariant is not 100% type safe. To use it correctly, you must know the type it holds.

Also, GVariant is not only for reflection. It's an all purpose variant data type.

## Store value to GVariant

Any values can be converted to a GVariant implicitly, so you don't need to call GVariant constructor explicitly.

This is why the invoke function in GMetaMethod accepts only GVariant but any value can be passed directly.  
The prototype of "invoke" in GMetaMethod is,  
```c++

GVariant invoke(void * instance, const GVariant & p1, const GVariant & p2, ..., const GVariant & pN);
```
We can call it as method->invoke(instance, 5, "test", myObject).

## Retrieve value from GVariant

A GVariant can be converted back to a normal value with the function fromVariant.

Assume v is a GVariant.
```c++

int n = fromVariant<int>(v);
```
Convert v to integer.
```c++

string s = fromVariant<char *>(v);
```
Convert v to C string then assign to a STL string.
```c++

MyObject obj = fromVariant<MyObject>(v);
```
Convert v to MyObject.

If fromVariant finds the GVariant can't be converted to the desired type, an exception GVariantException will be thrown.

To test if a GVariant can be converted to a type, use canFromVariant.  
It has the same parameters as fromVariant, but returns a boolean to indicate if the GVariant can be casted.

## Store non-copyable object or reference to GVariant

Bye default, GVariant requires the value assigned to it copyable.  
If the value is a class object, but the copy constructor is private or not implemented, when trying to create GVariant from such value, compiling will fail.  
To store non-copyable object or reference to GVariant, use function createVariant.
```c++

template <bool CanShadow, typename T>
GVariant createVariant(const T & value, bool allowShadow = false);
```

The first template parameter, CanShadow, determines if the object copyable.  
If CanShadow is false, the object is not copyable.

So to store a non-copyable object, named A, to GVariant v, write like this,
```c++

GVariant v = createVariant<false>(A);
```

## Rules to cast from GVariant

### Rule 1, A GVariant with fundamental value can be casted to any other fundamental.
```c++

GVariant v;
v = 1; // integer
fromVariant<double>(v); // OK
```

### Rule 2, A GVariant with pointer value can be casted to any other pointer, even if the pointer dimension is different.
```c++

v = (char *)1;
fromVariant<int **>(v); // OK
```

### Rule 3, A GVariant with fundamental value or pointer value can be casted to an object that has constructor to accept the type implicitly.
```c++

v = "abc"; // C string, const char *
std::string s = fromVariant<std::string>(v); // OK, because std::string accepts "const char *".
```

### Rule 4, When casting a GVariant with object (T) or object reference (T &) to another object type (U), T and U must be the same type.

Otherwise, the memory may be corrupted.  
This is because of GVariant doesn't remember any type information, except the fundamental types.  
So when casting to type U, GVariant will always assume that it holds a buffer with sizeof(U) and try to cast the buffer to U.

### Rule 5, By default, casting a GVariant to reference-to-const (const T &) will not perform implicit type conversion.
```c++

std::string s = "blah";
v = s;
const std::string & rs = fromVariant<const std::string &>(v);
```

rs will become a reference to v.
The address of rs and v will be the same. &rs == &v
```c++

int n = fromVariant<int>(v);
```0

This doesn't work.  
rs will reference to invalid address because s is not std::string.  
And the cast will not convert C string to std::string.

To make fromVariant perform the implicit conversion, feed it with a second parameter.
```c++

int n = fromVariant<int>(v);
```1

This works as if it's a C++ assignment,
```c++

int n = fromVariant<int>(v);
```2

A temporary std::string will be create to hold s, then assign it to rs.

<WRAP tip>
How does VarantCastCopyConstRef work?

A usual fromVariant<const std::string &> without VarantCastCopyConstRef will return "const std::string &", but when VarantCastCopyConstRef is used, fromVariant will return std::string instead of a reference, which a temporary object is returned.

In pseudo code
```c++

int n = fromVariant<int>(v);
```3

</WRAP>

### Rule 6, Setting a reference to fundamental (such as int &, double &) to variant will cause the value is copied instead of reference.

That's to say, you can't convert such variant back to reference.
```c++

int n = fromVariant<int>(v);
```4

However, setting a reference to object (such as MyClass &) to variant will cause the variant references to the object.
```c++

int n = fromVariant<int>(v);
```5

In summary, be careful to use reference. Usually you should reference-to-const or pointer instead of reference.

### Rule 7, GVariant treats rvalue reference (int &&, MyClass &&, etc) same as lvalue reference.

## GVariant memory management and avoid memory leak

GVariant manages memory automatically.  
And usually a GVariant entity is created on the stack, and passed by value or const reference, then there is no possibility of memory leak.

However, the interface based APIs only accept GVariantData instead of GVariant.  
GVariantData is a POD struct and doesn't manage the memory.

So to avoid memory leak, never use GVariantData directly. You should use GVariant.refData() instead.  
For example, to invoke IMetaField::get to get the field value, we should write like this,
```c++

int n = fromVariant<int>(v);
```6
