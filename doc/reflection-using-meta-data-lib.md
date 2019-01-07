# cpgf reflection -- using meta data

<!-- toc -->

- [Using reflection through library](#using-reflection-through-library)
  * [Retrieve meta class](#retrieve-meta-class)
  * [Retrieve meta data](#retrieve-meta-data)
- [Using meta data](#using-meta-data)
  * [Using meta item](#using-meta-item)
  * [Using meta field](#using-meta-field)
  * [Using meta property](#using-meta-property)
  * [Using meta method](#using-meta-method)
  * [Using meta constructor](#using-meta-constructor)
  * [Using meta operator](#using-meta-operator)
  * [Using meta annotation](#using-meta-annotation)
  * [Using meta annotation value](#using-meta-annotation-value)
  * [Using meta enumerators](#using-meta-enumerators)
  * [Using variadic parameters in meta methods, constructors and operators](#using-variadic-parameters-in-meta-methods-constructors-and-operators)

<!-- tocstop -->

## Using reflection through library

### Retrieve meta class

To retrieve any meta data, first we need to get the desired meta class.

To get the global meta class, call getGlobalMetaClass().  
To get a certain meta class, call findMetaClass("TheMetaClassName").

Both functions return either NULL for not found or a pointer of const GMetaClass *.

### Retrieve meta data

Any meta data can be retrieved through GMetaClass.

GMetaClass exposes a lot of functions to access the meta data, but fortunately we don't need to memorize each function. Most functions have the similar naming convention.  
Assume MMM is the meta name, it can be Constructor, Field, Property, Method, Operator, Enum, Class.  
Then for most meta data, we have these kind of functions,
```c++
const GMetaMMM * getMMM(const char * name) const;
const GMetaMMM * getMMMInHierarchy(const char * name, void ** outInstance) const;
size_t getMMMCount() const;
const GMetaMMM * getMMMAt(size_t index) const;
```

**getMMM** -- get the meta data by name in current meta class. Return the meta data, or NULL if not found. If the meta data with the same name is overloaded, only the first meta data is returned.

**getMMMInHierarchy** -- find the meta data in the whole inheritance hierarchy. That means, if the meta data is not found in current meta class, all parent meta classes will be checked recursively.  
The second parameter, can be NULL, or the address of an instance pointer. If it's not NULL, on return the function will convert the pointer to the object in which the meta data is in.  
The function will perform an action as,  
```c++
*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```
Usually the casted pointer is same as the original pointer, but under virtual inheritance, they can be different.

**getMMMCount** -- get how many meta data there is.

**getMMMAt** -- get the meta data at certain index. With this function and getMMMCount, we can get all of the meta data.

Constructors are special because they can't be inherited and have no name. So there is no getConstructor and getConstructorInHierarchy. But there is a function getConstructorByParamCount to get a constructor quickly.
```c++
const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```

Here is a list of all functions in GMetaClass that can retrieve meta data:
```c++
const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
size_t getConstructorCount() const;
const GMetaConstructor * getConstructorAt(size_t index) const;

const GMetaField * getFieldInHierarchy(const char * name, void ** outInstance) const;
const GMetaField * getField(const char * name) const;
size_t getFieldCount() const;
const GMetaField * getFieldAt(size_t index) const;

const GMetaProperty * getPropertyInHierarchy(const char * name, void ** outInstance) const;
const GMetaProperty * getProperty(const char * name) const;
size_t getPropertyCount() const;
const GMetaProperty * getPropertyAt(size_t index) const;

const GMetaMethod * getMethodInHierarchy(const char * name, void ** outInstance) const;
const GMetaMethod * getMethod(const char * name) const;
size_t getMethodCount() const;
const GMetaMethod * getMethodAt(size_t index) const;

const GMetaOperator * getOperatorInHierarchy(GMetaOpType op, void ** outInstance) const;
const GMetaOperator * getOperator(GMetaOpType op) const;
size_t getOperatorCount() const;
const GMetaOperator * getOperatorAt(size_t index) const;

const GMetaEnum * getEnumInHierarchy(const char * name, void ** outInstance) const;
const GMetaEnum * getEnum(const char * name) const;
size_t getEnumCount() const;
const GMetaEnum * getEnumAt(size_t index) const;

const GMetaClass * getClassInHierarchy(const char * name, void ** outInstance) const;
const GMetaClass * getClass(const char * name) const;
size_t getClassCount() const;
const GMetaClass * getClassAt(size_t index) const;

size_t getMetaCount() const;
const GMetaItem * getMetaAt(size_t index) const;
```

The last two functions, getMetaCount and getMetaAt, can be used to retrieve arbitrary meta data.

## Using meta data

### Using meta item

Class GMetaItem is the base class for all meta class.  
It covers the common aspects of the meta data.
```c++
bool isStatic() const;
```
Return true if the field, property, or method is global (not class member).  
Otherwise, return false.  
For member only meta data, such as constructor, operator, this function always returns false.
```c++
GMetaCategory getCategory() const;
```
Return the category of the meta data.

Categories list:
```c++
enum GMetaCategory {
    mcatField = 0,
    mcatProperty = 1,
    mcatMethod = 2,
    mcatEnum = 3,
    mcatOperator = 4,
    mcatConstructor = 5,
    mcatClass = 6,
    mcatAnnotation = 7,
    mcatFundamental = 8,
};
```
```c++
const GMetaItem * getOwnerItem() const;
```
Return the outter meta class that owns the item.
For members in class A, the owner item is the meta class of A.  
For members in top level global namespace (the meta class returned by getGlobalMetaClass()), the owner item is the global meta class.  
Calling this function on getGlobalMetaClass() will always return NULL.
```c++
virtual const GMetaType & getItemType() const;
```
Return the meta type of the item.  
For field and property, it's the field and property variable type.  
For method, it's the function type.
```c++
const std::string & getName() const;
```
Return the item name. The name is the one used to register the meta data.
```c++
const std::string & getQualifiedName() const;
```
Return the full qualified name. It's all of the owner items, recursively, names joined with ".".  
If a name looks like ClassOne.ClassTwo.myData, it means the item name is myData, it's in ClassTwo, and ClassTwo is an inner class of ClassOne.
```c++
std::string makeQualifiedName(const char * delimiter) const;
```
If you are not satisfied with the "." delimiter returned by getQualifiedName, use makeQualifiedName to make your own full qualified name.  
delimiter can be any string.
```c++
const GMetaAnnotation * getAnnotation(const char * name) const;
```
Get the annotation with "name" that bound to the item.  
Return NULL if there is no annotation with "name".
```c++
size_t getAnnotationCount() const;
```
Return annotation count.
```c++
const GMetaAnnotation * getAnnotationAt(size_t index) const;
```
Return annotation at certain index.

### Using meta field

Class GMetaField exposes several functions to set/get field, and acquire field information.
```c++
virtual bool canGet() const;
```
Check if the field value can be got.  
A field can't be got if it's forbidden by the policy.
```c++
virtual bool canSet() const;
```
Check if the field value can be set.  
A field can't be set if it's forbidden by the policy.
```c++
virtual GVariant get(void * instance) const;
```
Get the field value on the object "instance". A GVariant that represents the value will be returned.
```c++
virtual void set(void * instance, const GVariant & v) const;
```
Set the field value on the object "instance". A GVariant that represents the value is passed as the second parameter.
```c++
virtual size_t getSize() const;
```
Get the field memory size. It's as if performing a sizeof on the field.
```c++
void * getAddress(void * instance) const;
```
Get the field address. It's as if performing a "&" operator on the field.

### Using meta property

Using property is almost same as using field, except that property doesn't support getAddress.
```c++
virtual bool canGet() const;
```
Check if the property value can be got.  
A property can't be got if it's forbidden by the policy, or the getter is absence (NULL).
```c++
virtual bool canSet() const;
```
Check if the property value can be set.  
A property can't be set if it's forbidden by the policy, or the setter is absence (NULL).
```c++
virtual GVariant get(void * instance) const;
```
Get the property value on the object "instance". A GVariant that represents the value will be returned.
```c++
virtual void set(void * instance, const GVariant & v) const;
```
Set the property value on the object "instance". A GVariant that represents the value is passed as the second parameter.
```c++
virtual size_t getSize() const;
```
Get the property memory size. It's as if performing a sizeof on the property.

### Using meta method

Class GMetaMethod exposes several functions to invoke method, and acquire field information.
```c++
virtual GMetaType getParamType(size_t index) const;
```
Acquire parameter type at index.
```c++
virtual size_t getParamCount() const;
```
Get parameter count.
```c++
virtual size_t getDefaultParamCount() const;
```
Get the default parameter count.  
For instance, if we have a meta method "int abc(int a, int b = 5, int c = 6);", getDefaultParamCount() will return 2 for it.
```c++
virtual bool hasResult() const;
```
Check whether the method has result. For functions that return "void", this function return false, otherwise, it returns true.
```c++
virtual GMetaType getResultType() const;
```
Acquire result type information.
```c++
virtual bool isVariadic() const;
```
Check whether the method can accept variadic parameters.
```c++
GVariant invoke(void * instance, const GVariant & p1);
GVariant invoke(void * instance, const GVariant & p1, const GVariant & p2);
GVariant invoke(void * instance, const GVariant & p1, const GVariant & p2, ..., const GVariant & pN);
```
A group of overloaded functions to invoke the method with different amount of parameters, on the object "instance".
```c++
virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const;
```
Invoke the method, and pass the parameters in an array.
```c++
virtual bool checkParam(const GVariant & param, size_t paramIndex) const;
```
Check if a parameter can be converted to the method parameter.

### Using meta constructor

Using constructor is almost same as using method, except that its return value is always a pointer of "void *".
```c++
virtual GMetaType getParamType(size_t index) const;
```
Acquire parameter type at index.
```c++
virtual size_t getParamCount() const;
```
Get parameter count.
```c++
virtual size_t getDefaultParamCount() const;
```
Get the default parameter count.  
For instance, if we have a meta method "int abc(int a, int b = 5, int c = 6);", getDefaultParamCount() will return 2 for it.
```c++
virtual bool hasResult() const;
```
Always return true;
```c++
virtual GMetaType getResultType() const;
```
Return the type information of the class it will construct.
```c++
virtual bool isVariadic() const;
```
Check whether the constructor can accept variadic parameters.
```c++
void * invoke(const GVariant & p1);
void * invoke(const GVariant & p1, const GVariant & p2);
void * invoke(const GVariant & p1, const GVariant & p2, ..., const GVariant & pN);
```
A group of overloaded functions to invoke the method with different amount of parameters.  
Unlike GMetaMethod, these functions here always return void *, which is the address of the new instance.
```c++
virtual GVariant execute(const GVariant * params, size_t paramCount) const;
```
Invoke the constructor, and pass the parameters in an array.
```c++
virtual bool checkParam(const GVariant & param, size_t paramIndex) const;
```
Check if a parameter can be converted to the constructor parameter.

### Using meta operator
```c++
Using operator is almost same as using method, except that its parameter count is determined by the operator itself.
```
```c++
virtual GMetaType getParamType(size_t index) const;
```
Acquire parameter type at index.
```c++
virtual size_t getParamCount() const;
```
Get parameter count.
```c++
virtual size_t getDefaultParamCount() const;
```
Get the default parameter count.  
For instance, if we have a meta method "int abc(int a, int b = 5, int c = 6);", getDefaultParamCount() will return 2 for it.
```c++
virtual bool hasResult() const;
```
Check whether the operator has result. For operators that return "void", this function return false, otherwise, it returns true.
```c++
virtual GMetaType getResultType() const;
```
Acquire result type information.
```c++
virtual bool isVariadic() const;
```
Check whether the operator can accept variadic parameters.
```c++
GVariant invokeUnary(const GVariant & p0) const;
```
Invoke the operator with one parameter. The operator should be a unary operator.
```c++
GVariant invokeBinary(const GVariant & p0, const GVariant & p1) const;
```
Invoke the operator with two parameter. The operator should be a binary operator.
```c++
GVariant invokeFunctor(const GVariant & instance, const GVariant & p1, const GVariant & p2, ..., const GVariant & pMax) const;
```
Invoke the functor operator.
```c++
virtual GVariant execute(const GVariant * params, size_t paramCount) const;
```
Invoke the operator, and pass the parameters in an array.
```c++
virtual bool checkParam(const GVariant & param, size_t paramIndex) const;
```
Check if a parameter can be converted to the operator parameter.

### Using meta annotation
```c++
const GMetaItem * getMetaItem() const;
```
Get the meta item that the annotation is bound to.
```c++
const GAnnotationValue * getValue(const char * name) const;
```
Get the annotation value of the key with the "name". Return NULL if "name" doesn't exist.
```c++
size_t getCount() const;
```
Get how many annotation values there are.
```c++
const char * getNameAt(size_t index) const;
```
Get the annotation name at certain index.
```c++
const GAnnotationValue * getValueAt(size_t index) const;
```
Get the annotation value at certain index.

### Using meta annotation value

Annotation value is not meta data.  
It's the value type of annotation.
```c++
const GVariant * getVariant() const;
```
Get the value as a GVariant;
```c++
bool canToString() const;
```
Check if the value can be converted to a string.
```c++
bool canToWideString() const;
```
Check if the value can be converted to a wide string.
```c++
bool canToInt() const;
```
Check if the value can be converted to a integer.
```c++
const char * toString() const;
```
Convert the value to a string.
```c++
const wchar_t * toWideString() const;
```
Convert the value to a wide string.
```c++
int toInt() const;
```
Convert the value to an integer.
```c++
bool toBoolean() const;
```
Convert the value to a boolean.
```c++
template <typename T>
T toObject() const;
```
Convert the value to a specified type.


### Using meta enumerators
```c++
virtual size_t getTypeSize() const;
```
Get the memory size of the enumerator type. It's as if performing a sizeof on the enumerator.
```c++
size_t getCount() const;
```
Get the element count of the enumerator;
```c++
const char * getKey(size_t index) const;
```
Get the enumerator element key name at index.
```c++
GVariant getValue(size_t index) const;
```
Get the enumerator element value at index.  
It's safe to convert the result value to an integer.
```c++
int findKey(const char * key) const;
```
Get the index of a key name.  
Return -1 if not found.
```c++
virtual void * createInstance() const;
virtual void * createInplace(void * placement) const;
virtual void * cloneInstance(void * instance) const;
virtual void * cloneInplace(void * instance, void * placement) const;
virtual void destroyInstance(void * instance) const;
```


### Using variadic parameters in meta methods, constructors and operators

Any meta invokable -- methods, constructors, or operators -- with only one parameter of type GMetaVariadicParam * or const GMetaVariadicParam *, is treated as variadic invokable.  
Thus any number of parameters can be passed to the invokable.

A variadic meta method,
```c++
int sum(const cpgf::GMetaVariadicParam * params);
```

In meaning it's same as C++ variadic function,
```c++
int sum(...);
```

Calling the meta version sum,
```c++
method->invoke(obj, 1, 3, 5, 7, 9);
```

is quite similar with C++ variadic function,
```c++
obj->sum(1, 3, 5, 7, 9);
```

GMetaVariadicParam is a structure,
```c++
struct GMetaVariadicParam
{
    GVariant const * const * params;
    size_t paramCount;
};
```

params is a pointer that points to an array of GVariant pointers, each GVariant pointer points to a parameter.  
paramCount is the parameter count.

A sample implementation of the variadic method "sum",
```c++
int sum(const cpgf::GMetaVariadicParam * params)
{
    int total = 0;
    for(size_t i = 0; i < params->paramCount; ++i) {
        total += cpgf::fromVariant<int>(*(params->params[i]));
    }

    return total;
}
```
