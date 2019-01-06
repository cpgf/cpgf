# cpgf reflection -- using meta data

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

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```0
Return the full qualified name. It's all of the owner items, recursively, names joined with ".".  
If a name looks like ClassOne.ClassTwo.myData, it means the item name is myData, it's in ClassTwo, and ClassTwo is an inner class of ClassOne.
```c++

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```1
If you are not satisfied with the "." delimiter returned by getQualifiedName, use makeQualifiedName to make your own full qualified name.  
delimiter can be any string.
```c++

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```2
Get the annotation with "name" that bound to the item.  
Return NULL if there is no annotation with "name".
```c++

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```3
Return annotation count.
```c++

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```4
Return annotation at certain index.

### Using meta field

Class GMetaField exposes several functions to set/get field, and acquire field information.
```c++

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```5
Check if the field value can be got.  
A field can't be got if it's forbidden by the policy.
```c++

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```6
Check if the field value can be set.  
A field can't be set if it's forbidden by the policy.
```c++

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```7
Get the field value on the object "instance". A GVariant that represents the value will be returned.
```c++

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```8
Set the field value on the object "instance". A GVariant that represents the value is passed as the second parameter.
```c++

*outInstance = static_cast<ClassThatFoundTheMetaData *>(*outInstance). \\
```9
Get the field memory size. It's as if performing a sizeof on the field.
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```0
Get the field address. It's as if performing a "&" operator on the field.

### Using meta property

Using property is almost same as using field, except that property doesn't support getAddress.
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```1
Check if the property value can be got.  
A property can't be got if it's forbidden by the policy, or the getter is absence (NULL).
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```2
Check if the property value can be set.  
A property can't be set if it's forbidden by the policy, or the setter is absence (NULL).
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```3
Get the property value on the object "instance". A GVariant that represents the value will be returned.
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```4
Set the property value on the object "instance". A GVariant that represents the value is passed as the second parameter.
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```5
Get the property memory size. It's as if performing a sizeof on the property.

### Using meta method

Class GMetaMethod exposes several functions to invoke method, and acquire field information.
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```6
Acquire parameter type at index.
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```7
Get parameter count.
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```8
Get the default parameter count.  
For instance, if we have a meta method "int abc(int a, int b = 5, int c = 6);", getDefaultParamCount() will return 2 for it.
```c++

const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
```9
Check whether the method has result. For functions that return "void", this function return false, otherwise, it returns true.
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
```0
Acquire result type information.
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
```1
Check whether the method can accept variadic parameters.
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
```2
A group of overloaded functions to invoke the method with different amount of parameters, on the object "instance".
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
```3
Invoke the method, and pass the parameters in an array.
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
```4
Check if a parameter can be converted to the method parameter.

### Using meta constructor

Using constructor is almost same as using method, except that its return value is always a pointer of "void *".
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
```5
Acquire parameter type at index.
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
```6
Get parameter count.
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
```7
Get the default parameter count.  
For instance, if we have a meta method "int abc(int a, int b = 5, int c = 6);", getDefaultParamCount() will return 2 for it.
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
```8
Always return true;
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
```9
Return the type information of the class it will construct.
```c++

bool isStatic() const;
```0
Check whether the constructor can accept variadic parameters.
```c++

bool isStatic() const;
```1
A group of overloaded functions to invoke the method with different amount of parameters.  
Unlike GMetaMethod, these functions here always return void *, which is the address of the new instance.
```c++

bool isStatic() const;
```2
Invoke the constructor, and pass the parameters in an array.
```c++

bool isStatic() const;
```3
Check if a parameter can be converted to the constructor parameter.

### Using meta operator
```c++

bool isStatic() const;
```4
```c++

bool isStatic() const;
```5
Acquire parameter type at index.
```c++

bool isStatic() const;
```6
Get parameter count.
```c++

bool isStatic() const;
```7
Get the default parameter count.  
For instance, if we have a meta method "int abc(int a, int b = 5, int c = 6);", getDefaultParamCount() will return 2 for it.
```c++

bool isStatic() const;
```8
Check whether the operator has result. For operators that return "void", this function return false, otherwise, it returns true.
```c++

bool isStatic() const;
```9
Acquire result type information.
```c++

GMetaCategory getCategory() const;
```0
Check whether the operator can accept variadic parameters.
```c++

GMetaCategory getCategory() const;
```1
Invoke the operator with one parameter. The operator should be a unary operator.
```c++

GMetaCategory getCategory() const;
```2
Invoke the operator with two parameter. The operator should be a binary operator.
```c++

GMetaCategory getCategory() const;
```3
Invoke the functor operator.
```c++

GMetaCategory getCategory() const;
```4
Invoke the operator, and pass the parameters in an array.
```c++

GMetaCategory getCategory() const;
```5
Check if a parameter can be converted to the operator parameter.

### Using meta annotation
```c++

GMetaCategory getCategory() const;
```6
Get the meta item that the annotation is bound to.
```c++

GMetaCategory getCategory() const;
```7
Get the annotation value of the key with the "name". Return NULL if "name" doesn't exist.
```c++

GMetaCategory getCategory() const;
```8
Get how many annotation values there are.
```c++

GMetaCategory getCategory() const;
```9
Get the annotation name at certain index.
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
```0
Get the annotation value at certain index.

### Using meta annotation value

Annotation value is not meta data.  
It's the value type of annotation.
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
```1
Get the value as a GVariant;
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
```2
Check if the value can be converted to a string.
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
```3
Check if the value can be converted to a wide string.
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
```4
Check if the value can be converted to a integer.
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
```5
Convert the value to a string.
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
```6
Convert the value to a wide string.
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
```7
Convert the value to an integer.
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
```8
Convert the value to a boolean.
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
```9
Convert the value to a specified type.


### Using meta enumerators
```c++

const GMetaItem * getOwnerItem() const;
```0
Get the memory size of the enumerator type. It's as if performing a sizeof on the enumerator.
```c++

const GMetaItem * getOwnerItem() const;
```1
Get the element count of the enumerator;
```c++

const GMetaItem * getOwnerItem() const;
```2
Get the enumerator element key name at index.
```c++

const GMetaItem * getOwnerItem() const;
```3
Get the enumerator element value at index.  
It's safe to convert the result value to an integer.
```c++

const GMetaItem * getOwnerItem() const;
```4
Get the index of a key name.  
Return -1 if not found.
```c++

const GMetaItem * getOwnerItem() const;
```5


### Using variadic parameters in meta methods, constructors and operators

Any meta invokable -- methods, constructors, or operators -- with only one parameter of type GMetaVariadicParam * or const GMetaVariadicParam *, is treated as variadic invokable.  
Thus any number of parameters can be passed to the invokable.

A variadic meta method,
```c++

const GMetaItem * getOwnerItem() const;
```6

In meaning it's same as C++ variadic function,
```c++

const GMetaItem * getOwnerItem() const;
```7

Calling the meta version sum,
```c++

const GMetaItem * getOwnerItem() const;
```8

is quite similar with C++ variadic function,
```c++

const GMetaItem * getOwnerItem() const;
```9

GMetaVariadicParam is a structure,
```c++

virtual const GMetaType & getItemType() const;
```0

params is a pointer that points to an array of GVariant pointers, each GVariant pointer points to a parameter.  
paramCount is the parameter count.

A sample implementation of the variadic method "sum",
```c++

virtual const GMetaType & getItemType() const;
```1
