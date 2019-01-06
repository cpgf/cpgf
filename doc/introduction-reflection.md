

# cpgf reflection -- an open source library to add reflection to C++

cpgf reflection library adds powerful reflection feature to C++. The reflection is quite similar to Java reflection. We even support annotation.

The only feature that is not as good as Java reflection is, we have to build the meta data manually while Java compiler does it automatically. But with the powerful C++ templates, building meta data is just a piece of cake. The library will do most work for you, such as deducing meta type, deducing method parameters. This will not be a problem in the future. There is a tool metagen in the library, which had automatically generated meta data for library Box2D and SFML. The tool is under actively development.

## Language features that can be reflected
  * Member and static fields with any data type.
  * Global fields (global variable) with any data type.
  * Properties (fields with getter and setter). Can be either member or global.
  * Member and static functions with any parameters, return value, and calling convention.
  * Global functions.
  * Overloaded function.
  * Functions with variadic parameters.
  * Default arguments of functions.
  * Enumerators.
  * Constants in any data type.
  * Classes and nested inner classes.
  * Namespace simulation by using class.
  * Class constructors.
  * Operators. All overloadable operators, except new and delete, are supported. The functional of overloaded new and delete are already provided by meta class.
  * Templates. Template instances can be reflected as easily as classes.
  * Annotations. Any annotations, in key-value pairs, can be reflected. Value can be any type.
  * We can reflect everything in C++ same as we do in Java and C#.

## Key features
  * Building meta data doesn't require preprocessor or any external tool. Only using C++ templates, classes and functions.
  * True runtime reflection. Accessing fields and properties, calling methods, are truly runtime behavior, no template parameters are needed. All parameters and return values are passed via GVariant.
  * Automatically type conversion when getting/setting fields, invoking methods, etc.
  * Support multiple inheritance.
  * You don't pay for what you don't use. If you don't build the meta data, no any memory overhead. If you don't use the meta data, no any performance overhead. If you build and use the meta data, you get trivial memory and performance overhead beside very powerful reflection system.
  * Binary interface compatible. Can be used crossing program dynamic libraries.
  * Written in standard and portable C++, compiled on VC and GCC, Windows, Linux, and Mac (iOS, iPhone, iPad).

## We can do a lot with the reflection library
  * Easy script binding. Indeed there are Lua, Google V8 Javascript and Python binding engines in cpgf library, build upon the reflection library.
  * Easy serialization. We have a serialization library in cpgf.
  * Build GUI visual component editor for arbitrary GUI frameworks. If you ever used Borland VCL, you know how powerful the GUI editor is. Now we have the power in standard C++!
  * Create object from class name, without knowing the class type.
  * Discover the class hierarchy.
  * Get or set any object fields directly or via getter and setter (property) at runtime.
  * Invoke any functions at runtime. Function parameter and result types can be discovered at runtime.
  * Invoke any overloaded operators at runtime.
  * Discover enumerator types at runtime.
  * More. Waiting for you to discover.
