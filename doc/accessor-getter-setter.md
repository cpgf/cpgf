

# cpgf Accessor library -- generic getter and setter for data accessing

## Overview

Accessor is a generic template based mechanism to get or set data via data address or functions. The library includes getter, setter and accessor. An accessor is composed by both getter and setter.

Accessors are widely used in cpgf property reflection, tween and timeline library.

Header files

Getter  
#include "cpgf/accessor/ggetter.h"

Setter  
#include "cpgf/accessor/gsetter.h"

Accessor  
#include "cpgf/accessor/gaccessor.h"

The namespace is cpgf.

## Getter
```c++
template <typename RawGetter, typename Policy = GMetaPolicyDefault>
class GGetter
{
public:
  GGetter(const void * instance, RawGetterPassType getter);
  GGetter(const GGetter & other);
  GGetter & operator = (const GGetter & other);
  T get() const;
  T operator() () const;
  operator T () const;
  void * getAddress() const;
  const void * getInstance() const;
  void setInstance(const void * newInstance);
};
```
RawGetter is the raw type of the getter which can be a pointer to a variable, a pointer to a class member data, or a pointer to a function.  
Policy can be GMetaRuleForbidRead or GMetaRuleExplicitThis. GMetaRuleForbidRead means the data is not readable and getting from the getter will cause an exception. GMetaRuleExplicitThis is used when the raw getter is a function. In the case, the object instance will be passed as the first parameter of the raw getter function.

The constructor is "GGetter(const void * instance, RawGetterPassType getter)". The first parameter is the instance. If the getter is a global variable or global function, instance must be NULL.

To obtain the value from the getter, use one of below functions or operators,
```c++
T get() const;
  T operator() () const;
  operator T () const;
```
Type T is deduced from getter type. Usually T is a reference to the value.

Function getAddress() is used to obtain the variable address of the getter. If the getter is a function, getAddress() returns NULL.

It's cumbersome to specify RawGetter for each getter, so some auxiliary function are provided to create getter.
```c++
template <typename RawGetter, typename Policy>
GGetter<RawGetter, Policy> createGetter(const void * instance,
    const RawGetter & getter, Policy policy);

template <typename RawGetter>
GGetter<RawGetter, GMetaPolicyDefault> createGetter(const void * instance,
    const RawGetter & getter);
```
The only difference between the two createGetter functions is that the first form uses an explicit policy while the second uses the default policy.

Some examples,

Use non-member variable as getter,
```c++
int x;
  GGetter<int *> getter = createGetter(NULL, &x);
  x = 5;
  cout << getter->get(); // print 5
  cout << getter(); // print 5
  cout << (int)getter; // print 5
  x = 6;
  cout << getter->get(); // print 6
```

Use member variable as getter,
```c++
class C
  {
  public:
    int x;
  };
  C instance;
  GGetter<int C::*> getter = createGetter(&instance, &C::x);
```

Use member function as getter,
```c++
class C
  {
  public:
    int getX() const;
  };
  C instance;
  GGetter<int (C::*)() const> getter = createGetter(&instance, &C::getX);
```

## Setter
```c++
template <typename RawSetter, typename Policy = GMetaPolicyDefault>
class GSetter
{
  GSetter(void * instance, RawSetterPassType setter);
  GSetter(const GSetter & other);
  GSetter & operator = (const GSetter & other);
  void set(T value) const;
  void operator() (T value) const;
  GSetter & operator = (T value);
  void * getAddress() const;
  void * getInstance() const;
  void setInstance(void * newInstance);
};

template <typename RawSetter, typename Policy>
GSetter<RawSetter, Policy> createSetter(void * instance,
    const RawSetter & setter, Policy policy);

template <typename RawSetter>
GSetter<RawSetter, GMetaPolicyDefault> createSetter(void * instance,
    const RawSetter & setter);
```

Setter is very similar with getter, except that a setter is used to store value to a variable while a getter is used to retrieve value from a variable.

To set the value with the setter, use one of below functions or operators,
```c++
void set(T value) const;
  void operator() (T value) const;
  GSetter & operator = (T value);
```
Type T is deduced from getter type. Usually T is a reference to the value.

## Accessor

An accessor is a combination of a getter and setter. So an accessor can be used to either retrieve data from a variable or save data to a variable.
```c++
GAccessor(const void * instance, RawGetter rawGetter, RawSetter rawSetter);
  GAccessor(const GAccessor & other);
  GAccessor & operator = (const GAccessor & other);
  
  bool canRead() const;
  bool canWrite() const;

  T get() const;
  T operator() () const;
  operator T () const;
  
  void set(T value) const;
  void operator() (T value) const;
  GAccessor & operator = (T value);

  void * getAddress() const;
  const void * getInstance() const;
  void setInstance(const void * newInstance);
```
