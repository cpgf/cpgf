# Useful technologies, tips, tricks and caveats for cpgf script binding

<!-- toc -->

- [Overview](#overview)
- [Use non-const reference as function in/out parameter](#use-non-const-reference-as-function-inout-parameter)
- [Function returning reference to fundamental will be converted to value](#function-returning-reference-to-fundamental-will-be-converted-to-value)
- [Use C++ operator overloading in script language](#use-c-operator-overloading-in-script-language)
- [Use C++ bit fields in script language](#use-c-bit-fields-in-script-language)
- [Be careful that in script variable is assigned by reference, not by value](#be-careful-that-in-script-variable-is-assigned-by-reference-not-by-value)

<!-- tocstop -->

## Overview

This documentation discusses several technologies and caveats which is not obvious to know when using cpgf script binding.

After reading this documentation, we can use a lot of C/C++ only features, such as reference, operator overloading, bit field, in any script languages supported by cpgf script binding engine.

## Use non-const reference as function in/out parameter

It's not rare in C++ we have below code
```c++
bool foo(MyClass & a)
{
    a = MyClass(blah, blah);
    return true;
}

char * bar(int & b)
{
    b = 5;
    return "OK";
}

// Call the functions.
MyClass a;
foo(a);

int b = 0;
bar(b);
```
The function parameter is used to return value.

In cpgf script binding, we can invoke those kind of functions from script, with or without tweaks.

For function foo, we can call it in quite nature way from script
```javascript
var a = new MyClass();
foo(a);
```

For function bar, there is no way to pass reference to fundamental types (here is int) around the script, we need to use byte array to simulate it.
```javascript
// Don't forget to import byte array
cpgf.import(null, "builtin.collections.bytearray");

var b = cpgf.createByteArray();

// set initial value, same as "int b = 0" in C++
b.writeInt32(0);

// getPointer() will return the address of b
bar(b.getPointer());

// rewind the position
b.setPosition(0);

// get the value returned from bar
var bValue = b.readInt32();

// bValue is 5
```

## Function returning reference to fundamental will be converted to value

If a function returns a reference to fundamental type such as int, the result is converted to the value itself than a reference. This because there is no way to manipulate reference to fundamental in script. So we can't change the returned value.

C++
```c++
static int value = 0;
int & getA()
{
    return value;
}

int & a = getA();

// here a == 0 and value == 0

a = 5;
// here a == 5 and value == 5
```

Script
```javascript
var a = getA();
// here a == 0 and value == 0

a = 5;
// here a == 5 and value Still == 0
```

If the return type is a reference to class, the address will be returned. So changing any class member will affect the original value.

C++
```c++
static MyClass value; // value.n == 0
MyClass & getM()
{
    return value;
}

MyClass & a = getM();
a.n = 5;
// here a.n == 5 and value.n == 5
```

Script
```javascript
var a = getM();
a.n = 5;
// here a.n == 5 and value.n == 5
```

## Use C++ operator overloading in script language

Though cpgf Lua binding supports operator overloading, it's not recommended to use operator overloading in script because not all script languages support it. Instead that, we can use the wrapper functions to access those operator, thus we can even use operator overloading in JavaScript.

The tool metagen can generate wrappers for operator overloading perfectly.

For more information, please read the document about "Use C++ operator wrapper functions from script". There is a full page document for this topic.

## Use C++ bit fields in script language

cpgf can't reflect meta data for bit fields. However, if we use metagen, metagen can generate wrapper code to reflect bit fields as properties. So in script, we can use bit fields as if they are common member data. Indeed we can forget the difference between bit field and member data.

If we don't use metagen, we can reflect bit fields as property manually. We only need to add setter and getter function to access the bit field.

## Be careful that in script variable is assigned by reference, not by value

If we have below C++ code
```c++
class A {
public:
    void setValue(int n);
};

const A & getA();
```

In C++, if we wrote
```c++
A a = getA();
a.setValue(0);
```
The code works.

But if we wrote in JavaScript
```javascript
var a = getA();
a.setValue(0);
```
The code will cause runtime error because we can't call a non-const function setValue on a const object. This is because getA returns a "const A &", so "a" is a reference to const! The C++ version works because when assigning to "a", the whole class A is copied, but in script, it's just a reference.

To make the code works in script, we need to create new A and copy return value of getA.
```javascript
var a = new A(getA());
a.setValue(0);
```
That works!
