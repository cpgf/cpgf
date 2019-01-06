# Using GMetaType

GMetaType is one of the core data type in cpgf reflection system. GMetaType provides basic type information such as the base class name (if any), pointer and reference information, etc. The informatio provided by GMetaType is only guaranteed enough for reflection and script binding usage. It's not guaranteed to cover all C++ type information. We can't expect accurate information from GMetaType for "char *(*(* *foo [][8])())[]".

## Member functions

<code>bool isEmpty() const; </code>
Return true if the GMetaType is not initialized with any type. GMetaType constructed by the default constructor is empty.

<code>const GTypeInfo & getBaseType() const; </code>
Get the type information of underlying base type.
Base type is the type removed all const-volatile qualifiers and pointers and reference. However, currently an array type is considered of a base type, which is not quite accurate.
GTypeInfo is a simple wrapper of std::type_info.
Note: for interface based API such as IMetaClass, getBaseType always returns an empty GTypeInfo since GTypeInfo is not ABI portable which interface based API requires ABI portability.

<code>const char * getBaseName() const; </code>
Get the meta name of underlying base type.
Currently the fundamental types (int, double, etc) name can't be got through this function.
If you are using the interface based API, such as IMetaClass, the base name is already there. If you are using class based API, such as GMetaClass, you must call global function fixupMetaType to fetch the base name.
void fixupMetaType(GMetaType * type);
Give a type of Foo, or Foo *, or const Foo &, or const Foo * const * &, getBaseName returns "Foo".

<code>bool baseIsClass() const; </code>
Return true if the base type is a class.

<code>bool baseIsArray() const; </code>
Return true if the base type is an array.
	
<code>bool isFundamental() const; </code>
Return true if the type is fundamental. Note: here it's not base type but type.
	
<code>bool isFunction() const; </code>
Return true if the type is a function. Such as "void f()".

<code>bool isConstFunction() const; </code>
Return true if the type is a const function. Such as "void f() const".

<code>bool isVolatileFunction() const; </code>
Return true if the type is a volatile function. Such as "void f() volatile".

<code>bool isConstVolatileFunction() const; </code>
Return true if the type is a const volatile function. Such as "void f() const volatile".

<code>bool isConst() const; </code>
Return true if the type is const. Such as "const Foo".

<code>bool isVolatile() const; </code>
Return true if the type is volatile. Such as "volatile Foo".

<code>bool isConstVolatile() const; </code>
Return true if the type is const volatile. Such as "const volatile Foo".

<code>bool isPointerToConst() const; </code>
Return true if type is a pointer which points to a const. Such as "const Foo *".

<code>bool isPointerToVolatile() const; </code>
Return true if type is a pointer which points to a volatile. Such as "volatile Foo *".

<code>bool isPointerToConstVolatile() const; </code>
Return true if type is a pointer which points to a const volatile. Such as "const volatile Foo *".

<code>bool isReferenceToConst() const; </code>
Return true if type is a reference which references to a const. Such as "const Foo &".

<code>bool isReferenceToVolatile() const; </code>
Return true if type is a reference which references to a volatile. Such as "volatile Foo &".

<code>bool isReferenceToConstVolatile() const; </code>
Return true if type is a reference which references to a const volatile. Such as "const volatile Foo &".

<code>bool isPointer() const; </code>
Return true if type is a pointer. Such as "Foo *", "const Foo *", "const Foo * volatile * *".

<code>bool isReference() const; </code>
Return true if type is a reference. Such as "Foo &", "const Foo &", "const Foo * volatile * &".
isReference can overlap with isPointer. For example, "Foo * &" is a reference and is a pointer. If both isPointer and isReference return true, the type is a reference to pointer.
	
<code>bool isString() const; </code>
Return true if type is a "char *" or "const char *".
Note: GMetaType doesn't know std::string.

<code>bool isWideString() const; </code>
Return true if type is a "wchar_t *" or "const wchar_t *".
Note: GMetaType doesn't know std::wstring.

<code>unsigned int getPointerDimension() const; </code>
Get the count of pointers there. For example, "Foo" returns 0, "Foo *" return 1, "const Foo * const volatile **" return 3. Usually only 1 dimension makes sense.
	
<code>GVariantType getVariantType() const; </code>
Get type's variant type. This is useful if the type is a fundamental.
