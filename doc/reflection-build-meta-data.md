# cpgf reflection -- build meta data
<!--begintoc-->
* [Overview](#a2_1)
* [Define meta classes](#a2_2)
  * [Overview](#a3_1)
  * [Define a meta class](#a3_2)
  * [Lazy define a meta class](#a3_3)
  * [Add meta data to global scope](#a3_4)
  * [Build dangling meta class](#a3_5)
  * [GDefineMetaInfo -- non template representation of meta define class](#a3_6)
  * [Reflect templates](#a3_7)
* [Build meta data](#a2_3)
  * [Reflect field](#a3_8)
  * [Reflect property](#a3_9)
  * [Reflect method](#a3_10)
  * [Reflect constructor](#a3_11)
  * [Reflect operator](#a3_12)
  * [Reflect annotation](#a3_13)
  * [Reflect enumerators](#a3_14)
  * [Reflect nested class](#a3_15)
  * [Reflect parameter default values](#a3_16)
<!--endtoc-->


<a id="a2_1"></a>
## Overview

Meta data is the core concept in the whole cpgf library. All the main features, such as reflection, script binding, serialization, are built upon meta data.

Below is a piece of sample code to define all kind of meta data. The full compilable code can be found in file samples/reflection/overview.cpp
```c++
void myMetaBuild()
{
    using namespace cpgf;

    // define meta class
    GDefineMetaClass<MyClass, MyFirstBaseClass, MySecondBaseClass> // which class to define (MyClass), and give the base classes
    	::define("MyClass_TheNameCanBeDiffWithClassName") // the class name

        // constructors
    	._constructor<void * ()>()
    	._constructor<void * (int)>()
    	._constructor<void * (int, const std::string &)>(GMetaPolicyCopyAllConstReference())

    	// fields. All class data members and global values are fields.
    	._field("width", &MyClass::width) // reflect a normal field
    	._field("nocopy", &MyClass::nocopy, GMetaPolicyNoncopyable()) // reflect a field with policy

    	// properties. Special fields with getter and setter functions.
    	._property("name", &MyClass::getName, 0) // a property has only getter, no setter
    	._property("data", &MyClass::getData, &MyClass::data) // a property has both getter and setter.
    	// Same as fields, we can also reflect a property with policy

    	// methods. All class function members and global functions are methods.
    	._method("getWidth", &MyClass::getWidth) // reflect a normal method
    	._method("incWidth", &MyClass::incWidth, GMetaPolicyCopyAllConstReference()) // reflect a method with policy
    		._annotation("attr") // the method incWidth has an annotation
    			._element("what", "ok")

    	// operator
    	._operator<MyClass (const GMetaSelf &, int)>(mopHolder + mopHolder)

    	// enumerator
    	._enum<MyClass::WindowStyle>("WindowStyle")
    		._element("ws0", MyClass::ws0)
    		._element("ws1", MyClass::ws1)
    		._element("ws2", MyClass::ws2)
    		._element("ws3", MyClass::ws3)

    	// nested class
    	._class(
    		GDefineMetaClass<MyClass::AnotherInner>
    			::declare("AnotherInner")
    			._field("y", &MyClass::AnotherInner::y)
    	)

    	// Now let's define some annotations.
    	// The meta class for MyClass has two annotations.
    	// First's name is attribute, and has three elements.
    	// Second's name is style.
    	._annotation("attribute")
    		._element("name", "What's my name") // element name, value is an Ansi string
    		._element("cat", mcatClass) // element cat, value is int
    		._element("dog", TestData(mcatClass, "A cute dog")) // element dog, value is TestData
    	._annotation("style")
    		._element("fit", L"fit me") // element fit, value is a Wide string.
    ;

    // define global meta data

    GDefineMetaGlobal()
    	._field("myStats", &myStats)
    	._method("myAddNumber", &myAddNumber)
    ;

    // define meta data for template
    GDefineMetaClass<MyTemplate<char, int> >::lazy("MyTemplate_char_int", &lazyDefineClass<MyTemplate<char, int> >);
}

int main()
{
    myMetaBuild();

    // now meta data is ready to use

    return 0;
}
```

<a id="a2_2"></a>
## Define meta classes

<a id="a3_1"></a>
### Overview

Meta class represents a collection of meta information about a certain class. All meta data, like meta fields, meta properties, meta methods, meta enumerators, meta operators, meta constructor, meta annotation, must be added to a meta class. For global fields and methods, they must be added to the global meta class.

To build the meta data, include the header file gmetadefine.h.

Meta data is built with only template functions, no any macros, no any preprocessors.

<a id="a3_2"></a>
### Define a meta class

Assume we want to define meta classes for below classes
```c++
class Animal
{
};

class Cat : public Animal
{
};
```

Now let's build meta class for Animal.
```c++
GDefineMetaClass<Animal>
    ::define("Animal")
    ._field(blah blah)
    ._method(blah blah)
;
```

GDefineMetaClass is the template to define a meta class.  
It accepts one or more template parameters.  
The first template parameter is always the class itself. The other template parameters are the class' super classes (aka, parent classes).

The function "define" is used to define a top level class.  
Top level class means the class is in the global or a namespace scope. It's not nested in another class.

Now let's build meta class for Cat.
```c++
GDefineMetaClass<Cat, Animal>
    ::define("Cat")
    ._field(blah blah)
    ._method(blah blah)
;
```

This time there is the second template parameter, Animal, which is the base class, aka, super class.  
The library supports multiple inheritance. If Cat is also derived from Biological, just add Biological to GDefineMetaClass.

The base classes are not necessary reflected. When a non-reflected base class is retrieving for meta class by calling GMetaClass::getBaseClass, it always return NULL. However, calling GMetaClass::getBaseCount to get base class count will include all non-reflected base classes.

GDefineMetaClass::declare  
The function "declare" behaves similar as "define". "define" will add the meta class to global scope, while "declare" not.

**GDefineMetaClass::define VS GDefineMetaClass::declare**  
GDefineMetaClass::define = GDefineMetaClass::declare + add to global  
GDefineMetaClass::declare will create a meta class, and GDefineMetaClass owns the meta class. That means GDefineMetaClass will free the meta class in its destructor.  
If the meta class is added to another class (the global class, or any other class), GDefineMetaClass will give up the ownership.

If you want to reflect class to global scope, just use GDefineMetaClass::define.  
If you want to reflect a nested class, or a temporary meta class (not want to add to global scope), use GDefineMetaClass::declare.

There are two functions in GDefineMetaClass to get the meta class:  
```c++
GMetaClass * getMetaClass() const;
```
Get the meta class, GDefineMetaClass still owns the meta class
```c++
GMetaClass * takeMetaClass();
```
Get the meta class, GDefineMetaClass will transfer the ownership to the caller. It's the caller's responsibility to free the meta class if it's not added to other class.

<a id="a3_3"></a>
### Lazy define a meta class
```c++
void lazyDefineClass(GDefineMetaClass<Animal> define)
{
    define
    	._field(blah blah)
    	._method(blah blah)
    ;
}

GDefineMetaClass<Animal>::lazy("Animal", &lazyDefineClass);
```

GDefineMetaClass::define will cause all meta data is built immediately.  
GDefineMetaClass::lazy will delay the building until the meta data is really used.  
"lazy" is similar with "define", except it requires a second parameter, which is a function that does the real reflecting.  
The function prototype is  
void func(GDefineMetaClass<T> define)  
Which T is the class to build for, here is Animal.

GDefineMetaClass::lazyDeclare "declare" a meta class with lazy build.  
<a id="a3_4"></a>
### Add meta data to global scope
```c++
GDefineMetaGlobal()
    ._field(blah blah)
    ._method(blah blah)
;
```

GDefineMetaGlobal indicates that any reflection added to it are reflected to global scope.  
The global scoped is treated as a meta class, but it contains all global meta data for global variables, methods, classes, etc.

When any meta class is defined by GDefineMetaClass::define, it's add to global scope automatically.

<a id="a3_5"></a>
### Build dangling meta class
```c++
GDefineMetaGlobalDangle myGlobalDangleMeta = GDefineMetaGlobalDangle::dangle()()
    ._field(blah blah)
    ._method(blah blah)
;
GDefineMetaGlobal()
    ._class(myGlobalDangleMeta)
;

GDefineMetaDangle<MyClass> myClassDangle = GDefineMetaDangle<MyClass>::dangle()
;

GDefineMetaDangle<MyClass>::define("MyClass")
    ._class(myClassDangle)
;
```

A dangling meta class is similar as a normal meta class.  
When a dangling meta class is added to other meta class, it will be not added as nested class, but all of its members will be extracted directly to the outter class.  
In above code sample, the field and method added to myGlobalDangleMeta will be added to global directly.  
That's to say, a dangling meta class is a virtual container rather than a meta class.

**How is dangling meta class useful?**  
It's useful to split the meta data building to different translate units (cpp files).  
With the help of the non-template GDefineMetaInfo, it's possible to split meta data for a class, or for global, to different cpp files.  
Splitting meta data to smaller cpp files can help to reduce the compile time, compile memory usage, and help to make compiler happy (some compilers may crash with cpp file containing a lot of meta data).

<a id="a3_6"></a>
### GDefineMetaInfo -- non template representation of meta define class

GDefineMetaInfo can be obtained by calling getMetaInfo() on GDefineMetaClass and GDefineMetaDangle.  
GDefineMetaInfo is a non-template class.  
The non-template feature is vital because then it's possible to return GDefineMetaInfo from a function defined in a source file.  
In contrary, GDefineMetaClass and GDefineMetaDangle must be returned from functions defined in header file.

Below is pseudo code to demonstrate how to use GDefineMetaInfo.
```c++
// a.cpp
GDefineMetaInfo buildMetaDataForMyClass()
{
    GDefineMetaDangle<MyClass> define = GDefineMetaDangle<MyClass>::dangle();
    define
    	._constructor<blah blah>()
    	._method(blah blah);
    	._field(blah blah);
    ;
    return define.getMetaInfo();
}

// b.cpp
start code block // auto register or function, whatever
GDefineMetaClass<MyClass> define = GDefineMetaClass<MyClass>::define();
define
    ._class(buildMetaDataForMyClass())
;
```


<a id="a3_7"></a>
### Reflect templates
```c++
template <typename T>
class MyTemplateBase
{
public:
    virtual ~MyTemplateBase();

    int getDouble() const;
};

template <typename T, typename P>
class MyTemplate : public MyTemplateBase<T>
{
public:
    int getSize() const;
};

template <typename ClassType>
void lazyDefineClass(cpgf::GDefineMetaClass<ClassType> define)
{
    define
    	._method("getSize", &ClassType::getSize)
    	._method("getDouble", &ClassType::getDouble)
    ;
}

// instantiation
GDefineMetaClass<MyTemplate<char, int> >::lazy("MyTemplate_char_int", &lazyDefineClass<MyTemplate<char, int> >);
// another instantiation
GDefineMetaClass<MyTemplate<double, short> >::lazy("MyTemplate_double_short", &lazyDefineClass<MyTemplate<double, short> >);
```

Though we can treat a template instantiation as a normal class and use GDefineMetaClass<T>::define to reflect the meta data, it will be better to use the "lazy" trick in the above sample code.  
Then the lazy define function (here is lazyDefineClass) can be reused for every template instantiations.  
Note unlike lazy reflection class which lazy function is a normal function, the lazy function for template is a template function.

<a id="a2_3"></a>
## Build meta data

<a id="a3_8"></a>
### Reflect field

A field is either a member data or a global variable.  
Getting and setting a field will be applied directly to the field without any setter or getter function.  
To use setter and getter functions, use Property.
```c++
GDefineMetaClass<MyClass>
    ::define("MyClass")

    ._field("width", &MyClass::width) // reflect a normal field
    ._field("nocopy", &MyClass::nocopy, GMetaPolicyNoncopyable()) // reflect a field with policy
```

_field takes two or three parameters.  
The first parameter is the name. The named is used to find the meta data later.  
The second parameter is the field address.  
The third parameter is optional. It's the policy.

<a id="a3_9"></a>
### Reflect property

A property is just a member data or a global variable, but can be accessed with or without getter or setter functions.
```c++
GDefineMetaClass<MyClass>
    ::define("MyClass")

    ._property("name", &MyClass::getName, 0) // a property has only getter, no setter
    ._property("data", &MyClass::getData, &MyClass::data, MyPolicy()) // a property has both getter and setter, and has a policy.
```

._property takes three or four parameters.  
The first parameter is the name. The named is used to find the meta data later.  
The second parameter is getter address.  
The third parameter is setter address.  
The fourth parameter is optional. It's the policy.

Here "getter" and "setter" are not necessary functions, they can be the data address directly.

<a id="a3_10"></a>
### Reflect method

A method is a function.  
It can take any parameters. If any parameter is an object, that object is not necessary to be reflected.  
It can be any function, such as member function, virtual function, static member function, global function.  
It can also has any calling convention, such as cdecl, stdcall, fastcall.  
For member function, it can be const, volatile, and const volatile.
```c++
GDefineMetaClass<MyClass>
    ::define("MyClass")

    ._method("getWidth", &MyClass::getWidth) // reflect a normal method
    ._method("incWidth", &MyClass::incWidth, GMetaPolicyCopyAllConstReference()) // reflect a method with policy
    ._method("overloadFunction", (void (MyClass::*)())&MyClass::overloadFunction)
    ._method("overloadFunction", (void (MyClass::*)(int))&MyClass::overloadFunction)
    ._method("overloadFunction", (void (MyClass::*)() const)&MyClass::overloadFunction)
```

._method takes two or three parameters.  
The first parameter is the name. The named is used to find the meta data later.  
The second parameter is method address.  
The third parameter is optional. It's the policy.

In the code snippet, "overloadFunction" is a group of C++ overload functions. There are three overload functions, the prototypes are "void ()", "void (int)", and "void () const".
To reflect overloaded functions, just cast the function pointer to appropriate type.

In the code snippet, all functions are reflected to the same name as the function name. We can reflect the meta data to any name, not necessary same as the name in C++.

<a id="a3_11"></a>
### Reflect constructor

A constructor is a C++ class constructor.  
It can take any parameters, same as method.
```c++
GDefineMetaClass<MyClass>
    ::define("MyClass")

    ._constructor<void * ()>()
    ._constructor<void * (int)>()
    ._constructor<void * (int, const std::string &)>(GMetaPolicyCopyAllConstReference())
```

._constructor is a template function.  
It accepts one template parameter, and one optional policy function parameter.  
The template parameter is the function type. The function type must always return void *.

<a id="a3_12"></a>
### Reflect operator

An operator is a C++ class operator that be overloaded.  
It can take any parameters, same as method.
```c++
GDefineMetaClass<MyClass>
    ::define("MyClass")

    ._operator<MyClass (const GMetaSelf &, int)>(mopHolder + mopHolder) // +
    ._operator<MyClass (const GMetaSelf &, int)>(mopHolder - mopHolder) // -
```

_operator is a template function.  
It accepts one template parameter, one operator function parameter, and one optional policy function parameter.  
The template parameter is the operator function type.  
The first function parameter is the result to apply the same operator on mopHolder.  
For example, if it's operator +, then the parameter is (mopHolder + mopHolder)  
For most operators, it's quite intuitive.  
Here are some operators that are not that intuitive:  
Type conversion, such like "int()", etc. The parameter is mopHolder().  
Functor, such like "void (int)". The parameter is mopHolder(mopHolder);

About GMetaSelf. GMetaSelf indicates that parameter is the object itself. So that parameter can be passed as both object reference and object pointer.

<a id="a3_13"></a>
### Reflect annotation

An annotation is a kind of special meta data that's added by programmer rather than code.  
All other non-annotation meta data, such as class, method, field, property, constructor, operator, can have annotations.
```c++
GDefineMetaClass<MyClass>
    ::define("MyClass")

    ._annotation("attribute")
    	._element("name", "What's my name") // element name, value is an Ansi string
    	._element("cat", mcatClass) // element cat, value is int
    	._element("dog", TestData(mcatClass, "A cute dog")) // element dog, value is TestData
    ._annotation("style")
    	._element("fit", L"fit me") // element fit, value is a Wide string.

    ._method("incWidth", &MyClass::incWidth, GMetaPolicyCopyAllConstReference()) // reflect a method with policy
    	._annotation("attr") // the method incWidth has an annotation
    		._element("what", "ok")
```

_nnotation takes one parameter, the annotation name.  
To add elements, just call _element on the object. The first parameter is the value name, the second parameter is the value.

In above code, the annotation "attribute" and "style" are added to the meta class, and the annotation "attr" is added to meta method "incWidth".

<a id="a3_14"></a>
### Reflect enumerators

Note: enumerator here, doesn't only limit to C++ enum, but also can be any constants, such as string, pointer, object.  
It's because now meta enum uses a GVariant to hold the value.  
If the enum value is object, be sure it's not freed after the meta data is built.  
Use the function copyVariantFromCopyable in GVariant.h to make the GVariant holds a copy of object.
```c++
GDefineMetaClass<MyClass>
    ::define("MyClass")

    ._enum<MyClass::WindowStyle>("WindowStyle")
    	._element("ws0", MyClass::ws0)
    	._element("ws1", MyClass::ws1)
    	._element("ws2", MyClass::ws2)
    	._element("ws3", MyClass::ws3)
```

<a id="a3_15"></a>
### Reflect nested class
```c++
class MyClass {
public:
    class InnerClass { // this is nested class
    };
};

GDefineMetaClass<MyClass>
    ::define("MyClass")

    ._class(
    	GDefineMetaClass<MyClass::InnerClass>
    		::declare("InnerClass")
    		._field("whatever", &MyClass::InnerClass::whatever)
    )
```

<a id="a3_16"></a>
### Reflect parameter default values

Like the native C++, an invokable, such as constructor, method, operator functor, can have default value in meta data.  
Assume we give the below function default values to parameter obj, s, and i
```c++
bool myFunc(int type, int i, std::string s, TestObject obj);

GDefineMetaClass<MyClass>
    ::define("MyClass")

    ._method("myFunc", &myFunc)
    	._default(copyVariantFromCopyable(TestObject(8)))
    	._default(copyVariantFromCopyable(string("abc")))
    	._default(5)
    ;
```

Then the meta method myFunc is same as a C++ function with the prototype
```c++
bool myFunc(int type, int i = 5, std::string s = "abc", TestObject obj = TestObject(8));
```

The meta function "_default" must be called in the order from right most parameter to left. The first "_default" is always given to the right first parameter, the second is given to right second parameter, and so on.  
When any default parameter values are omitted when calling the meta method, cpgf library will supply the parameter with the default value.  
Note: If the default value is object, be sure it's not freed after the meta data is built.  
Use the function copyVariantFromCopyable in GVariant.h to make the GVariant holds a copy of object.  
Note again: the meta default value is implemeted in no relation to C++ default parameters. So the C++ function may have no default value, such as the example above shows.
