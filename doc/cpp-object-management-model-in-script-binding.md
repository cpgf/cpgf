# C++ object life management model in script binding

<!-- toc -->

- [Script frees the object](#script-frees-the-object)
  * [Return object as a copy](#return-object-as-a-copy)
  * [Return object as a raw pointer and use policy GMetaRuleTransferOwnership](#return-object-as-a-raw-pointer-and-use-policy-gmetaruletransferownership)
- [C++ frees the object](#c-frees-the-object)
- [C++ and script share the object ownership](#c-and-script-share-the-object-ownership)
- [If you are working on legacy code](#if-you-are-working-on-legacy-code)

<!-- tocstop -->

When returning a C++ object to script from a C++ function, either C++ or script needs to free the object when the object is not used by either C++ or script any more. cpgf library supports flexible object life management model that allows the user decides which side, C++, script, or both, to free the object.

## Script frees the object
In this model, C++ transfers the object ownership to script. Then C++ code doesn't use that object any more. Script will free the object usually in the garbage collection phase. There are two different ways to achieve this model, both has its disadvantage.

### Return object as a copy
```c++
MyObject getSomeObject() {
    return MyObject();
}
```
In native C++, the compiler will track the life of returned MyObject and free it properly, usuall this happens on the stack and no extra cost. In cpgf, the things become more complicated. cpgf has to duplicate the returned MyObject on the heap by invoking IMetaClass::cloneInstance, then hold the memory address as a vtShadow in GVariant. When the script frees the object, GVariant will call IMetaClass::destroyInstance to free the object.

The mechanism works quite well but the problem is that MyObject must be copied. It's not acceptable if there is non trivial copy constructor in MyObject.

### Return object as a raw pointer and use policy GMetaRuleTransferOwnership
```c++
MyObject * getSomeObject() {
    return new MyObject();
}

// reflect the method
metaDefine._method("getSomeObject", &getSomeObject, GMetaPolicyTransferResultOwnership());
```
In this model, the returned object will not be copied by cpgf, so there is no extra duplication cost. The problem is, same as returning as copy, talked above, when script frees the object, cpgf calls IMetaClass::destroyInstance to destroy the object. And IMetaClass::destroyInstance uses "delete MyObject" to free the object. If the object is not allocated by a normal "new", or it's allocated in a memory pool, deleting it will cause crash.

Usually this is not a serious problem and it rarely happens, but it's good to take aware of it.

## C++ frees the object
It's simple. If the returned object is a reference, or a pointer without policy GMetaRuleTransferOwnership, script doesn't free the object. It's the responsibility of C++ code to free it.
```c++
MyObject * getSomeObject() {
    return new MyObject();
}

MyObject myObject;
const MyObject & getSomeObjectByReference() {
    return myObject;
}
```

## C++ and script share the object ownership
Sometimes it's hard to decide which side free the object, in C++ or in script. Sometimes we may not want to specify GMetaRuleTransferOwnership (but it's good to use GMetaRuleTransferOwnership properly if you are building a meta data library, even you don't use that policy). That's to say, we want C++ and script share the ownership for an object.

To do it, in native C++, we use shared pointer, a kind of smart pointer. In cpgf, we also use shared pointer, exactly same as we did in native C++.
```c++
// C++11
std::shared_ptr<MYObject> getSomeObject() {
    return std::shared_ptr<MYObject>(new MyObject());
}

// Boost
boost::shared_ptr<MYObject> getSomeObject() {
    return boost::shared_ptr<MYObject>(new MyObject());
}
```
To let cpgf know std::shared_ptr and boost::shared_ptr are shared pointer, include meta traits header file before the meta reflection code for above methods.

To use C++11 shared pointer, include header "cpgf/metatraits/gmetasharedptrtraits_cpp11_shared_ptr.h"  
To use Boost shared pointer, include header "cpgf/metatraits/gmetasharedptrtraits_boost_shared_ptr.h"  
Script will hold the same shared pointer, exactly same as we did in native C++. Thus the life of MyObject will be managed by a shared pointer in both C++ and script.

The only disadvantage of this approach is, same as returning object as value, GVariant will duplicate a copy of shared pointer. This problem is usually quite trivial so you should not worry about it.

Note: if you forget to include the meta traits headers, the shared pointer still works in script. However, script can't use any member of the MyObject because it doesn't know the meta information for shared_ptr<MyObject>. After including the meta traits, script can get the proper meta information.

Currently cpgf supports shared pointers in C++11, Boost library, and cpgf library. If you have your own shared pointer, it's a breeze to implement your version of meta traits. If you want to do so, please read the source code in above meta traits headers, they are self explained even there is no comment in the code!

## If you are working on legacy code
Up to now, seems shared pointer is the best way to return an object from C++ to script. However, it you are working on legacy code base and you can't modify the code, it doesn't matter. You can still use the technical to return an object as a copied value, or a raw pointer (as long as you specify policy GMetaRuleTransferOwnership in reflection data), you legacy code base will still work quite well.

However, if you are designing new code from scratch and with cpgf in mind, you may rethink your C++ object life management model to get best performance and memory usage.
