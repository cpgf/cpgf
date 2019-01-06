

# What cpgf is and is not

Since cpgf is a so comprehensive C++ library, it's not rare that the people misunderstand it and undervalue the library. In this document I will clarify what cpgf is and what cpgf is not.

## What cpgf is NOT

### cpgf is not a script binding tool like SWIG or toLua++

Though there is some tools inside cpgf, cpgf itself is not a tool. cpgf does much more than SWIG or toLua++. cpgf can not only generate meta data from C++ code, like SWIG and toLua++ does, but also support a lot of features that SWIG and toLua++ doesn't support, such serialization, general reflection, etc.

### cpgf is not a single script binding library like boost::Python or Luabind

cpgf covers almost all same features in boost::Python and Luabind, but it also does much much more than those two libraries. cpgf supports Lua, Javascript, and Python binding, also supports serialization.

## What cpgf IS

### Core cpgf is a C++ reflection library

The original idea when developing cpgf is simple -- enable the users to create reflection meta data for C++ objects manually. This can't be simple, just to reflect meta data and retrieve and use the data. That's all.

### cpgf was extended to much more than a reflection library

Upon the reflection system, cpgf supports script binding to Lua, Javascript, and Python. Also cpgf support serialization. The tool metagen can generate meta data automatically.

### cpgf can be extended infinitely

Unlike SWIG, toLua++, boost::Python, and Luabind, which is single purpose, there is infinite possibility to extend cpgf library. Such as, but not limited to, support new script binding, use the reflection system to implement property editor, etc.


