

# Built-in meta data for STL in C++ cpgf library

## Overview

The meta data for STL is a collection of meta data to represent for C++ STL library.  
Currently there are meta data for iterator, vector, list and deque.  
More meta data will be added in the future.

Note: due to the variations of STL implementation, the meta data may be not compilable for certain STL porting.  
If so, a report with detail to me is welcome.

Note: due to the very heavy use of template in both cpgf and STL, compiling speed is quite slow.

## APIs to build meta data
The meta data for each STL template is in its own header file.  
The header files are in folder include/cpgf/metadata/stl.  
gmetadata_deque.h  
gmetadata_iterator.h  
gmetadata_list.h  
gmetadata_vector.h

## The APIs prototype
```c++

template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_constIterator(const GMetaDataConfigFlags & config, MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);

template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_constIterator(MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);

template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_iterator(const GMetaDataConfigFlags & config, MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);

template <typename MetaDefine, typename Policy>
MetaDefine buildMetaData_iterator(MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);


template <typename MetaDefine, typename Policy>
void buildMetaData_vector(const GMetaDataConfigFlags & config, MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);

template <typename MetaDefine, typename Policy>
void buildMetaData_vector(MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);


template <typename MetaDefine, typename Policy>
void buildMetaData_list(const GMetaDataConfigFlags & config, MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);

template <typename MetaDefine, typename Policy>
void buildMetaData_list(MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);


template <typename MetaDefine, typename Policy>
void buildMetaData_deque(const GMetaDataConfigFlags & config, MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);

template <typename MetaDefine, typename Policy>
void buildMetaData_deque(MetaDefine define,
    const Policy & policy, const GMetaDataNameReplacer * replacer = NULL);
```

The parameters:

**config**: The config flags to control how to build meta data.  
Now it can be only either 0, or mdcScriptable.  
If it's mdcScriptable, the meta data will contain auxiliary functions which can be used by script.  
For example, C++ iterator can be only increased by ++ operator which is not possible in script. A function "inc" is provided that can be used in script.  
Note: the counterpart API that doesn't have config, will always build meta data as if mdcScriptable is provided.

Here is the auxiliary functions list, assume "it" is an iterator:  
++it --> it.inc()  
it++ --> it.inc()  
--it --> it.dec()  
it-- --> it.dec()  
it + n --> it.skip(n)  
*it --> it.value()  
*it = newValue --> it.set(newValue) %%//%% only available for non-const iterator  
Examples:  
myVector.begin().inc().inc().value() will return the third value in the vector.

**define**: a meta define that represents the meta class.  
It can be obtained by GDefineMetaClass<T>::define or GDefineMetaClass<T>::declare.

**policy**: the policy that controlling how the STL functions accept parameters.  
cgpf provides GMetaDataPolicyDeduce to auto deduce the policy. But it only counts std::string specially.

**replacer**: a mechanism that allows to change the default function name in the meta data.  
If replacer is NULL, the meta name will keep same as in STL.  
If replacer is not NULL, the meta name will be replaced if the name appears in replacer.  
For example, in replacer, if "end" is replaced with "_end", the meta name of any "end" will become "_end".  
If you are binding meta data to Lua, you should ways replace "end" with "_end" because "end" is key word in Lua.  
A function initializeLuaReplacer can do it for you.  
void initializeLuaReplacer(GMetaDataNameReplacer * replacer);

Examples to build meta data for STL.
```c++

GMetaDataNameReplacer replacer;
initializeLuaReplacer(&replacer);
GDefineMetaClass<vector<int> > define = GDefineMetaClass<vector<int> >::declare("MyVector");
buildMetaData_vector(define, typename GMetaDataPolicyDeduce<typename vector<int>::value_type>::Result(), &replacer);
```

Now the meta class in "define" contains the meta data for vector<int>, named MyVector.  
If the code doesn't compile (C++ Builder will fail), change typename GMetaDataPolicyDeduce<typename vector<int>::value_type>::Result() to a temporary variable.
```c++

typename GMetaDataPolicyDeduce<typename vector<int>::value_type>::Result p;
buildMetaData_vector(define, p, &replacer);
```
