# cgpf library portability and compatibility -- tested supported C++ compilers and platforms

Below table shows the tested platforms and C++ compilers that cpgf works on.  
In summary, if you are working on Microsoft Windows, Linux, Mac (iOS, iPhone, iPad), and using the compilers MS VC 2008 or newer, GCC 3.4.2 or newer, cpgf library will work for you.  
And your target platform is not limited to Windows, Linux or Mac. It can be mobile phones, embeded deviced, game consoles, as long as it has a compiler supported in the list.

From version 1.2.3, cpgf also supports Embarcadero C++ Builder (tested with version 2010).  
^Platform 	^Compiler 	^Target 	^Result 	^Comment 	^
|Microsoft Windows 	|Microsoft Visual C++ 2008 Express 	|32 bit 	|Compile clean 	|Though the target is 32 bit, "-Wp64" is enabled to check 64 bit portability. 	|
|Microsoft Windows 	|Microsoft Visual C++ 2010 Professional 	|32 bit 	|Compile clean 	|Though the target is 32 bit, "-Wp64" is enabled to check 64 bit portability. 	|
|MinGW 	|gcc 4.4.0, 4.5.2, 4.6.1 	|32 bit 	|Compile clean 	| 	|
|MinGW 	|gcc 3.4.2 	|32 bit 	|A lot of useless compile warnings 	|The test case "test_reflection_class.cpp" will trigger an internal bug in g++. So that file is excluded from the test. 	|
|Linux 	|gcc 4.5.2 	|32 bit 	|Compile clean 	|The Linux version is Ubuntu 11.04 running in VirtualBox 	|
|Apple Mac 10.6.8 	|gcc 4.4.0 	|32 bit 	|Compile clean 	| 	|
|Apple Mac 10.6.8 	|Darwin gcc 4.2.1 	|32 bit 	|A lot of useless compile warnings 	| 	|
|Apple Mac 10.6.8 	|Xcode 4.0.1 	|32 bit 	|Compile clean 	|The compiler is Xcode default one. 	|
|Apple Mac 10.6.8 	|Xcode 4.0.1 	|64 bit 	|Compile clean 	|Failed link because Lua and V8 were compiled in 32 bit. The library itself should work well on 64 bit target. 	|
|Linux 	|Intel C++ Compiler Composer XE 12.1.2, free version 	|32 bit 	|Issues some no hurt warnings 	|The Linux version is Ubuntu 11.04 running in VirtualBox. Unit test test_callback is failed to compile due to compiler internal error. 	|
|Windows XP 	|Embarcadero C++ Builder 	|32 bit 	|Only compiled library and partial unit tests 	|Note: script binding library can be compiled but doesn't link, because it's not trivial work to get C++ Buidler work with Lua and Google V8 library. Some unit tests will cause "E2491 Maximum VIRDEF count exceeded" error, or internal error. That's because of that the compiler is buggy and lame. 	|

## Notes:

**"Compile clean" means succeeding the compiling and linking, no errors or warnings after enabling all warnings.**  
Google V8 v8.h will introduce warnings but that's not counted to cpgf.

**The working compilers and platforms are not limited to the list in the table.**  
The table only shows the compilers and platforms that had been tested.  
The other not too old compilers in GCC, VC family should work well too. Not too old means not as old as VC 6 or GCC 2.9.5.  
**Intel C++ Compiler is super slow to compile cpgf.**  
It uses more than 2 hours to compile the unit tests while GCC and VC only need 10~20 minutes.  
Not sure it's because that the compiler is quite slow to compile template code, or because it's run in Ubuntu which is running in Virtualbox (but GCC in Ubuntu + Virtualbox is quite faster).

**C++ Builder is not fully tested.**  
Seems C++ Builder is quite lame and doesn't have the ability to compile quite complicated source file.  
For some unit test, the compiler always throws errors, such like "E2491 Maximum VIRDEF count exceeded", or internal error. All are the compiler's internal limitation.  
So some unit test for reflection, such as method or operator, most tests are removed in C++ Builder.  
Also script binding is not tested in C++ Builder, because it will be hard to build Lua and V8 library using C++ Builder.  
In brief, only 40% of the total unit tests are used to test cpgf in C++ Builder.  
So if you want to use cpgf in C++ Builder, take your own risk.  
**Not all compilers are tested for compatibility in every cpgf release.**  
Due to the frequently library releasing, and the huge work/time on testing all compilers listed above, in each release, not all compilers are tested.  
So if you find any supported compiler failed to compile the library, please forgive it kindly and notify me for the issue.  
Up to now, it's only guaranteed that GCC 4.6 and VC 2008 are used to test every release.

**Your help is wanted.**  
If you approve that cpgf library works on other compilers or platforms, please let me know!  
Especially non-VC and non-gcc compilers are interested, such as Intel compiler.  
Also non-Windows, non-Linux and non-Mac platforms are interested, such as mobile phones, game consoles (Wii, WiiU, NDS, 3DS, XBox 360, PS3, PSP), or embeded system.  
Also more 64 bit tests are interested.

