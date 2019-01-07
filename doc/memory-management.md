<!--notoc-->

# cpgf library memory management guideline

The design goal of cpgf library memory management is to ease memory management and avoid memory leak.  
There is slight different rule between class and function based API (classes name starts with G) and interface based API (classes name starts with I).

Note here is only discussing the object that allocated on the heap, not any auto object that allocated on the stack.  
So the object dicussed here is passing by pointer, not by reference or by value.

## Memory management rules in class and function based API


  * Rule 1, every object has explicit, one, and only one owner.
  * Rule 2, the owner must free the object when the object is not used any more.
  * Rule 3, the owner can transfer the ownership to another, then the original owner loses the ownership and won't free the object.

## cpgf library uses different function name prefix to distinguish the ownership.

A function name prefixes with "create", or "gain", will transfer the ownership to the caller.  
So if you are invoking createScriptObject (in class GScriptObject and IScriptObject), you must free the returned object when it's not used.

A function name prefixes with "get", or any other non-"create" and non-"gain", will hold the ownership.  
So if you are calling any getMetaXXX functions in GMetaClass, you must NOT free the returned object.

To be exception safe, a scoped smart pointer, the one in cpgf called GScopedPointer, is your friend.

## Memory management rules in interface based API

  * Rule 1, every object uses reference count to manage the object life cycle.
  * Rule 2, there is no explicit owner for any object.
  * Rule 3, for any object returned by any API, after it is not used, releaseReference of the object must be called.

So no matter what's the functions named, releaseReference must be called.

To be exception safe, a scoped smart pointer GScopedInterface is provided.
