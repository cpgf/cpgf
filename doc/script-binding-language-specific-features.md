# Script language specific features in cpgf script binding
<!--begintoc-->
* [Overview](#a2_1)
* [Lua - Use dot (.) to access member functions and fields](#a2_2)
* [Lua - C++ operator overloading](#a2_3)
* [Python - C++ operator overloading](#a2_4)
* [Lua and Python - assign C++ method to script variable](#a2_5)
* [JavaScript - enumerate C++ object methods and properties](#a2_6)
* [JavaScript - use instanceof on C++ object](#a2_7)
* [JavaScript - use C++ object as __proto__](#a2_8)
<!--endtoc-->

<a id="a2_1"></a>
## Overview

cpgf script binding engine supports three script languages, Lua, JavaScript (Google V8), and Python. All languages work on the same meta data in the similar way, so we can have best portability between languages. However, to maximize the power of each script language, some language specified features are supported.

This document describes the script language specified features. Please note these features are only supported by a single language so they are not portable.

<a id="a2_2"></a>
## Lua - Use dot (.) to access member functions and fields

In current cpgf implementation of Lua script binding, it requires using dot (.) rather than colon (:) to access member functions and fields ([Read hear to learn the difference between dot and colon in Lua](https://coronalabs.com/blog/2015/12/01/tutorial-understanding-the-colon-vs-dot-operator/)).  

The advantages are:
  - Better performance. The C++ code doesn't need to track the "self" each time on function calling because the object instance is already stored in the method itself.
  - We can assign the member function to variable and call the variable later. For example: a = myObj.someFunc; a(blah blah);

The disadvantage is, it's against Lua paradigm and may be confusing. Assume you have some classes in pure Lua and some other classes in C++, then sometimes you need to use colon to access Lua objects and sometimes you need to use dot to access C++ objects.

I have very little experience on Lua programming, any your thoughts on this issue are welcome.

<a id="a2_3"></a>
## Lua - C++ operator overloading

cpgf can bind C++ operator to Lua and Python, but not JavaScript. This is useful to use mathematical object such as vector or matrix in Lua and Python. Lua and Python supports different sets of operator overloading, so we split operator overloading to two sections.

The operators that supported by cpgf Lua binding engine are,
  * +, binary addition, a + b
  * -, binary subtraction, a - b
  * *, binary multiplication, a * b
  * /, binary division, a / b
  * %, binary modulo, a % b
  * ##### , binary equal, a b
  * <, binary less than, a < b
  * <=, binary less than or equal than, a <= b
  * -, unary minus, -a
  * (), function object invoking, a(p1, p2, .. pn)

<a id="a2_4"></a>
## Python - C++ operator overloading

The operators that supported by cpgf Python binding engine are,
  * +, binary addition, a + b
  * -, binary subtraction, a - b
  * *, binary multiplication, a * b
  * /, binary division, a / b
  * %, binary modulo, a % b
  * +, unary plus, +a
  * -, unary minus, -a
  * ~, unary bitwise not, ~a
  * <<, binary bitwise left shift, a << b
  * >>, binary bitwise right shift, a >> b
  * &, binary bitwise and, a & b
  * |, binary bitwise or, a | b
  * ^, binary bitwise xor, a ^ b
  * +=, -=, *=, /=, %=, <<=, >>=, &=, |=, ^=, the inplace version of corresponding operators

<a id="a2_5"></a>
## Lua and Python - assign C++ method to script variable

Assume we have C++ code
```c++
class MyClass {
public:
    void foo(int a, const char * b);
};
```

In script, we can have the code
```lua
obj = MyClass(); -- construct an object
f = obj.foo; -- assign object.foo to a variable
f(1, "abc"); -- same as calling obj.foo(1, "abc");
```
Above code works for Lua and Python, but not JavaScript, because the "this" pointer may be wrong in JavaScript.

<a id="a2_6"></a>
## JavaScript - enumerate C++ object methods and properties

Assume we have C++ code
```c++
class MyClass {
public:
    void foo();
    int a;
};
```
In JavaScript
```javascript
obj = new MyClass();
for(p in obj) {
}
```
In the body of the for loop, p will be obj.foo, then obj.a, in turn.

<a id="a2_7"></a>
## JavaScript - use instanceof on C++ object

If we have two C++ class, B and D, D is inherited from B, then in JavaScript
```javascript
obj = new D();
b = (obj instanceof D); // true
b = (obj instanceof B); // true
```

<a id="a2_8"></a>
## JavaScript - use C++ object as __proto__

If we have a C++ class MyClass, in JavaScript
```javascript
obj = new MyClass();
b = {};
b.__proto__ = obj;
```
After above code, b will function same as obj.
