

# Inherit and extend C++ class from script in cpgf library

cpgf library provides unified method to extend C++ class from Lua, Google V8 Javascript, and Python, even if the scripts don't support inheritance or they implement inheritance in completely different way.

## Override C++ functions from script

Usually the purpose to inherit or extend C++ class is to override C++ function. In cpgf, we can override C++ functions on a class, or on an object instance.  
If we override on a class, all existing or new created object instances, will be affected.  
If we override on an object instance, only that instance is affected. The other instances with the same class won't be affected.

A normal C++ class can't be override by script, we must create wrapper for it.

Assume we have a C++ class
```c++
class MyClass
{
public:
    virtual int getValue(int n) {
        return n + m;
    }
    
    int m;
};
```

Then the wrapper class should look like
```c++
class MyClassWrapper : public MyClass, public cpgf::GScriptWrapper
{
public:
    virtual int getValue(int n) {
        GScopedInterface<IScriptFunction> func(this->getScriptFunction("getValue"));
        if(func) {
            return fromVariant<int>(invokeScriptFunction(func.get(), this, n).getValue());
        }
        else {
            return MyClass::getValue(n);
        }
    }
};
```

The class GScriptWrapper is a library class declared in "cpgf/scriptbind/gscriptwrapper.h", it provides the override storage and the function "getScriptFunction" to retrieve the override script function. You class wrapper must inherit from GScriptWrapper.

After declared the wrapper class, we override all functions that we want to override from script in the wrapper class. In the above sample code, we want the function "getValue" overridable from script, so we just override it in the wrapper class.

In each override functions in the wrapper class, firstly we call this->getScriptFunction to see if the function is overrided by the script, if so, we call the script function, otherwise, we just call base class' function. If the function in base class is an abstract virtual function , we should throw exception or report error is the function is not overrided by the script.

Invoking the overrided script function is quite intuitive, the only thing we should notice is that there is always an extra parameter, "this", as the first parameter. That's to say, we have to pass "this" pointer explicitly.

Now we must build meta data for the wrapper class. The script will use the wrapper class to achieve the override ability.

After built the meta data and reflected "getValue" and "m", now we can override "getValue" from the script. Below we only use Javascript syntax as sample.
```javascript
// This is javascript code
function overrideGetValue(me, n) {
    return me.m * 2 + n;
}

var notOverridedObject = new MyClassWrapper();
notOverridedObject.getValue(5); // execute notOverridedObject.m + 5

// Override getValue on an object
var object = new MyClassWrapper();
object.getValue = overrideGetValue;
object.getValue(5); // execute object.m * 2 + 5

// Override getValue on the class
MyClassWrapper.getValue = overrideGetValue;
var anotherObject = new MyClassWrapper();
anotherObject.getValue(5); // execute anotherObject.m * 2 + 5
// Indeed from now on all notOverridedObject, object,
// and anotherObject, will execute m * 2 + 5 because they
// have the same class.
```

## Extend C++ class from script

The sample code in above section looks perfect, but the problem is, after run "MyClassWrapper.getValue = overrideGetValue;", all object instances will share the same overrided function. This is a big problem. Ideally, we may want to create new class MyClassWrapperA and extend from MyClassWrapper, then override getValue with getValueA, then create another new class MyClassWrapperB and extend from MyClassWrapper, then override getValue with getValueB. The two new classes won't conflict with each other.

To solve the problem, cpgf library provides a powerful function cloneClass in the core service. cloneClass is a function that is called from the script. Its prototype is, in Javascript syntax,
```javascript
NewMetaClass cloneClass(MetaClass);
```
cloneClass will create a class same as MetaClass, but they have independent overrided function storage. So overriding a function on NewMetaClass will not affect MetaClass.

To use cloneClass, we have to bind the core service into the script engine.
There is a function in GScriptBind and IScriptBind
```c++
virtual void bindCoreService(const char * name) = 0;
```

Calling it will bind the core service into the namespace "name". So if call
```c++
myScriptBind->bindCoreService("cpgf");
```

In script, we can extend a class as,
```javascript
var NewClass = cpgf.cloneClass(MyClassWrapper);
// Now we can use NewClass as if it's MyClassWrapper
NewClass.getValue = overrideGetValue;
var object = new NewClass();
object.getValue(5); // execute object.m * 2 + 5

var anotherObject = new MyClassWrapper();
// execute anotherObject.m + 5 because MyClassWrapper.getValue is not changed
anotherObject.getValue(5);
```
