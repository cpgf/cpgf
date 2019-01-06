

# Advanced usage of GVariant

This documentation will discuss the advanced usage of GVariant. If you don't know what GVariant is, please read the documentation "Using GVariant" first.

GVariant is designed to be as lightweight as possible, as timing and spacing efficient as possible. So when we assign any value to a variant, GVariant will
  * Storage the value in the variant if the value is a fundamental or a pointer
  * Or storage a void pointer to the value if the value is an object or object reference
That implies,
  * GVariant doesn't allocate any memory, or copy any object
  * GVariant doesn't hold any meta type information, except the fundamental types
The behaviors listed above is the default behaviors in GVariant, such as we do "myVariant = anotherValue". To be more powerful, GVariant can hold more information than default.

## Store a copy of object in GVariant

GVariant can allocate memory and copy an object to itself and GVariant will manage memory automatically.
```c++

template <bool Copyable, typename T>
GVariant createVariant(const T & value, bool copyObject = false)
```
The function createVariant can create a GVariant, with copying or not copying an object.

The template parameter "Copyable" indicates whether the type T is copyable. It's used to avoid compiling error.

The function parameter "copyObject" indicates if GVariant copies the object. If it's true, GVariant will allocate memory on the heap and copy the object there.

Examples:
```c++

// Same as v = myObject, don't copy the object
// v holds a pointer to myObject
// We can't get back the object from v after myObject is freed
// The variant type of v is vtObject
v = createVariant<false>(myObject);

// Same as v = myObject, don't copy the object
// v holds a pointer to myObject
// We can't get back the object from v after myObject is freed
// copyObject = true doesn't take effect
// The variant type of v is vtObject
v = createVariant<false>(myObject, true);

// Same as v = myObject, don't copy the object
// v holds a pointer to myObject
// We can't get back the object from v after myObject is freed
// The variant type of v is vtObject
v = createVariant<true>(myObject);
v = createVariant<true>(myObject, false);

// v copies myObject to internal buffer
// v doesn't point to myObject any more
// We can get back the object from v after myObject is freed
// The variant type of v is vtShadow
v = createVariant<true>(myObject, true);
```

## Store string in GVariant

Similar with storing object, storing a C string will cause GVariant points to the string, by default. Indeed GVariant doesn't distinguish a C string. When we assign a "char *" to a GVariant, the variant type is a pointer to vtChar.

There are function to create GVariant which allocate memory for the string.
```c++

void initializeVarString(GVariantData * data, const char * s);
GVariant createStringVariant(const char * s);
bool variantDataIsString(const GVariantData & v);
bool variantIsString(const GVariant & v);
```
createStringVariant will create a GVariant, which copy the string "s" on the heap. GVariant will track the memory automatically. The variant type is vtString.

variantIsString treats both vtString or pointer to vtChat as a string.

There are wide string version, the variant type is vtWideString.
```c++

void initializeVarWideString(GVariantData * data, const wchar_t * s);
GVariant createWideStringVariant(const wchar_t * s);
bool variantDataIsWideString(const GVariantData & v);
bool variantIsWideString(const GVariant & v);
```

## Store meta type in GVariant

By default GVariant doesn't hold any meta type information. There are functions to store meta type in GVariant. This kind of GVariant type is vtTypedVar.
```c++

GVariant createTypedVariant(const GVariant & value, const GMetaType & type);
template <typename T>
GVariant createTypedVariant(const T & value);
GVariant getVariantRealValue(const GVariant & value);
GMetaType getVariantRealMetaType(const GVariant & value);
```
The non-template version createTypedVariant creates a typed GVariant from another GVariant and a GMetaType. If "value" is already a typed variant, it's real value will be used. So it's guaranteed that the value in a typed variant is not a typed variant.

The template version createTypedVariant creates a typed GVariant from arbitrary value directly.

We can use getVariantRealValue to retrieve the real value from a GVariant. It returns the real value for typed GVariant, and return the variant itself if it's not typed variant.

We can use getVariantRealMetaType to retrieve the meta type from a GVariant. If the variant is not typed, GMetaType() will be returned.

The script binding library uses typed variant.
