<!--notoc-->

# cpgf script binding -- an open source library to connect Lua, Google V8 Javascript and Python to C++

cpgf script binding library is a very easy to use script binding engine to bind Lua, Google V8 Javascript and Python to C++.  
The library uses reflection meta data extensively.

cpgf script binding is licensed under Apache License, Version 2.0

The supported Lua library is version 5.2 and 5.1  
The supported Google V8 Javascript library is Version 3.4.5  
The supported Python is version 2.7.3.

## Key features

  * Bind C++ class to Lua and Google V8 Javascript. Then Lua and Javascript can create instance, invoke class member function, get/set class field or property, and more.
  * Bind C++ enum type to Lua and V8 Javascript. Then Lua and Javascript can access the enum value by name.
  * Bind C++ functions (global or member) to Lua, V8, and Python.
  * Bind C++ values to Lua, V8 and Python variable.
  * Bind C++ operators to Lua (Lua only). The overloaded operators in C++ are bound to Lua together with the class.
  * Invoke Lua, V8 Javascript and Python functions from C++.
  * Get and set Lua, V8 Javascript and Python value from C++.

## Different script language, same API interface!

Though there is very big difference in internal implementation and public API among Lua, Google V8 Javascript and Python, cpgf makes the binding exactly the same.  
We use same data, the reflection meta data, to drive the binding.  
We use same API, only the interface of classes GScriptObject and IScriptObject, to make the binding.

What does that mean?  
It means write your C++ code once, binding to any script languages -- now supported Lua, V8 Javascript and Python, on any platforms -- Windows, Linux, Mac (iOS, iPhone, iPad), VC, gcc, XCode.

Are you a game developer? Try this library. Use Lua, Javascript and Python to make your game AI and logic fast and easily.  
Are you a desktop application developer? Try this library. Use Lua, Javascript and Python to extend your application fast and easily.  
Are you a web developign team? Try this libray. Connect your great web experience to desktop fast and easily.
