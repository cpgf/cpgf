#ifndef CPGF_CONFIG_H
#define CPGF_CONFIG_H


#ifndef ENABLE_LUA
	#define ENABLE_LUA 0
#endif

#ifndef ENABLE_V8
	#define ENABLE_V8 0
#endif

#ifndef ENABLE_PYTHON
	#define ENABLE_PYTHON 0
#endif

#ifndef ENABLE_SPIDERMONKEY
	#define ENABLE_SPIDERMONKEY 0
#endif

#define HAS_SCRIPT_ENGINE (ENABLE_LUA || ENABLE_V8 || ENABLE_PYTHON || ENABLE_SPIDERMONKEY)


#endif
