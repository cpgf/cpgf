# cpgf library

cpgf library is a cross platform C++ library for callback, reflection, serialization and script binding.

It's written in standard C++ and doesn't require any preprocessor or tools.

## This project is NOT continued, it will be replaced by substitute projects

There will be substitute projects under development.  
If you are looking for C++ reflection library, please [check metapp](https://github.com/wqking/metapp), that's from the same developer of cpgf.  
If you are looking for the callback list feature in cpgf, please [check eventpp](https://github.com/wqking/eventpp), it's from the same develop of cpgf too.  
A metapp based Lua binding project is under development as well.  

### The problems in cpgf that it can't be continued

**1, The project is too ambitious and is too large.**  

Indeed cpgf is not a single library, it contains many libraries, such as,  

- Callback and callback list
- Variant
- Reflection
- Serialization
- Lua script binding
- Python script binding
- V8 script binding
- SpiderMonkey script binding
- Meta data for various libraries
- Meta data generating tool
- Tweening library.

Each component is already a very large library by itself. Putting them to a giant cpgf is not manageable even for big software companies.

**2, I was too conservative on new technology.**  

At the time I started the cpgf project, the latest C++ standard was C++03. And C++11 was already drafted, and was partially usable in the compilers. Especially, TR1 was there already.  
I chose to stick to C++03, because I wanted my libraries be able to be used in as many as compilers.  
That's another big mistake. The different between C++11 and pre-C++11 is nothing like C++03 and pre-03, or between C++14 and C++11. C++11 is a new era of C++.  

### What will be in the substitute projects

1. Each project will be small and only focus on one feature. For example, [metapp](https://github.com/wqking/metapp) will only focus on reflection feature, there will be no serialization, no script binding, under metapp.  
2. All projects will be using at least C++11. Though C++11 is already old, I feel it's enough for most projects.  


## Documentations  
[Documentations](doc/index.md)

## Version
1.6.1
		
## Libraries

* cpgf reflection -- a library to add reflection feature to C++.

* cpgf serialization -- a library to save C++ data and objects to persistent storage
	and then load back. It's based on the reflection library.

* cpgf script binding -- a script binding engine to bind Lua (5.3, 5.2 and 5.1),
	Google V8 JavaScript, Python (2.7.3), and Mozilla SpiderMonkey JavaScript to C++.
	It's based on the reflection library.

* cpgf callback -- a callback library to implement signal/slot or callback in C++.
		It's the primary library for other libraries.

* cpgf meta data collection -- a library with built-in meta data support.
		Now there are meta data for C++ STL, OpenGL, OpenGLU, OpenGLUT,
		Box2D physical engine, SFML graphics and multimedia engine and Irrlicht
		3D render engine.

* cpgf meta data generator tool -- a tool writen in Java to automatically generate
		meta data from Doxygen XML files.

* cpgf tweening -- a library to implement ease animation in C++.
	
## License
Apache License, Version 2.0
You may obtain a copy of the License at
[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

## Author and owner
Wang Qi, in Beijing, China

## Learning the library
Though there are some documentations on the official website, they are far from good.
You may understand that developers like coding much more than documenting.

The fastest way to get start with the library is to read the sample code
in the folder "samples".
The samples cover most features so it's	a good kick off.

If you want to read more sample code, read the code in the folder "test".
They are for unit test. They are much more difficult to read and
use a lot of macros.
However, they cover much more functions than the sample code.

## A sister project using cpgf
There is a 2D game engine from the cpgf author,

[https://github.com/wqking/gincu](https://github.com/wqking/gincu)

The game engine uses cpgf callback, tweening and script binding extensively.
	
## Using the library
* In the project setting, add an include path pointing to "cpgf/include".
* Link the source code or link the library in your project.	
* All classes, functions and APIs are in the namespace "cpgf".

## Link the source code
To using the library, the easiest way is to just link all .cpp source code
under the src and sub folder.
However, a build script is also provide to build the code as static library.

## Build the library
The library uses CMake as the build system.
A makefile is provided to wrap the CMake build.
To build the library, open command line terminal, go to folder "build",

The library requires C++ exception and RTTI are enabled.

```	
make PLATFORM
```
		
PLATFORM can be one of

	nmake mingw linux vc05 vc08 vc10 cb_mingw cb_nmake cb_linux xcode
		
For detailed PLATFORM information, run "make" without parameter to see the help.
	
If you are using MSVC, run "nmake" instead of "make".

To build with Lua binding, you must change the variable HAS_LUA and LUA_DIR
in build.config.txt to point to the source code of Lua.
	
To build with Google V8 Javascript binding, you must change the variable HAS_V8 and V8_DIR
in build.config.txt to point to the source code of V8.

To build with Python binding, you must change the variable HAS_PYTHON and PYTHON_DIR
in build.config.txt to point to the source code of Python.
	
To build with Mozilla SpiderMonkey binding, you must change the variable HAS_SPIDERMONKEY and SPIDERMONKEY_DIR
in build.config.txt to point to the source code of Mozilla SpiderMonkey.

To build the unit test and sample code, feed a second parameter

```	
	make PLATFORM TARGET=TheTarget
```

	TheTarget can be,
	TARGET=lib                   Build the library. This is the default if TARGET is omitted.
	TARGET=tutorials             Build the tutorials.
	TARGET=tests                 Build the unit test.
	TARGET=samplereflection      Build the reflection sample application.
	TARGET=sampleserialization   Build the serialization sample application.
	TARGET=samplelua             Build the Lua binding sample application.
	TARGET=sampleopengl          Build the Opengl binding sample application.
	TARGET=samplebox2d           Build the Box2D binding sample application.
	TARGET=samplesfml            Build the SFML binding sample application.
	TARGET=sampleirrlicht        Build the Irrlicht binding sample application.

## Compatibility -- Tested compilers and OSes
* Linux (Ubuntu 18.04), GCC 7.3.0
* Linux (Ubuntu 18.04, LLVM Clang 5.0.2
* The below cases were based on the old 1.5 version. Since version 1.6, C++11 compiler is required, and VC 2015 and GCC 4.9.1 were tested.
* Windows XP, Microsoft Visual C++ 2008 Express and Microsoft Visual C++ 2010 Professional
* Windows XP, MingW GCC 3.4.2, 4.4.0 and 4.5.2
* Windows XP, Embarcadero C++ Builder 2010
* Linux (Ubuntu 11.04 in VirtualBox), GCC 4.5.2
* Linux (Ubuntu 11.04 in VirtualBox), Intel C++ Compiler Composer XE
* Mac 10.6.8, GCC 4.4.0
	
## Required third party library -- None for the major libraries
* For Lua binding, Lua library is required.
* For Google V8 binding, V8 library is required.
* For Python binding, Python library is required.
* For Mozilla SpiderMonkey, Mozilla SpiderMonkey is required.
* If you want to run the unit test, UnitTest++ is required.
	But usually you don't need to do that.

## Contribute to the library
If you like the library and want to contribute to it, here are some most
desired work to do
* Help with the documentation. I'm not a native English speaker, and
	I also don't like to write documentation. 
* Test the library on more compilers and platforms.

## What does cpgf mean?
The original cpgf means Cross Platform Game Framework.
It was intended	to be game framework that can work on PC and smart phone.
But later the author decided to add more fun but non-game features to the library.
So now the library is neither for game nor a framework.
It's a cross platform library.

