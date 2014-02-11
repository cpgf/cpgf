/**
@mainpage C++ callback library
Documentation
*/

namespace cpgf {


/**
The callback object is useful to represent any functions. <br />
It can be used with any functions, include global function, member function, virtual member function, static member function, functor object (an object with () operator), etc. <br />
The callback is the same concept of slot in other implementation, such as Qt, boost::Signals, and libsigc++. <br />
The template parameter 'Signature' is the function type, or a pointer type that points to a function (aka, function pointer). <br />
GCallback<void ()> --> void foo(); // signature is a function type. <br />
GCallback<void (*)()> --> void foo(); // signature is a function pointer. <br />
GCallback<int (double, char)> --> int foo(double, char); <br />
GCallback<void __stdcall (int)> --> void foo(int n); // __stdcall is ignored, Note the type GCallback<void __stdcall (int)> is not equal to GCallback<void (int)>, though they behave exactly the same. <br />

The Signature can be in any format as long as it constructs a valid function type or function pointer type. <br />
The Signature can include calling convention and class qualifier. <br />
Any calling convention and class qualifier will be ignored. <br />
So for two Signatures, if they have same argument count and types and result type, even they have different calling convention and class type (so they ar different function signature), the callback will behave exactly the same. <br />
But always keeps in mind, different signatures, will produce different callback types, even if all callback types behave the same. <br />
So it's highly recommended that we should better stick to consistent signature format. For example, always using function type without any calling convention or class type should be the best. <br />

Please alwasy prefer function type to function pointer. If you don't want, please keep consistent. <br />
GCallback<Function type> is not equal to GCallback<Function pointer>. Though they behave exactly the same, they are different C++ types. <br />
Assigning a callback which is a type GCallback<Function type> to GCallback<Function pointer> will cause callback be treated as a common functor object, vice versa. <br />

For any functions that expect GCallback as parameter, global function and functor object can be used directly because it will be converted implicitly. <br />

Note: <br />
1, When passing functor object  (functor) to callback, if the object is volatile or const volatile, a volatile copy constructor must be specified as well as a const copy constructor. <br />
However, that will generator a "warning C4521:  multiple copy constructors specified" in VC. Seems it's a problem of VC (fine in GCC), so just use #pragma warning (disable : 4521)  to disable that warning. <br />
2, For the functor object passed to callback, it must either not define operator ==, or define operator == as public member. It mustn't declare the operator == in non-public section. <br />
3, For functor object, it must provide copy constructor. If it can't, use makeReference to get a reference. <br />
4, For functor object, the non-const operator function is always used if there is one. If there is no non-const version, the const version will be used.

How to compare two callbacks <br />
A callback is comparable only if the object or function is comparable. <br />
So, a callback with global function, or static memeber function, or normal member function, is always comparable. <br />
A callback with a functor object is only comparable when the functor is comparable (== operator is defined) <br />
For functor-only object which is only used to execute the () operator, an "always return true" == operator will be fine. <br />
@code
	bool operator == (const FunctorClass &) const {
		return true;
	}
@endcode

How to change the max parameter count? <br />
The max parameter count is defined by CB_MAX_ARITY. <br />
So to change it, define CB_MAX_ARITY with a new value (1~30) before include gcallback.h or gcallbacklist.h. <br />
If you need more than 30 parameters, you need to extend the macros in gpp.h which is quite tricky. <br />
<b> Note: usually you don't need to redefine CB_MAX_ARITY. It being 10 means you can use 0~10 parameters, not only 10 parameters. And 10 parameters are enough for most use.</b>
*/
template<typename Signature>
class GCallback
{
public:
	/** Construct an empty callback. Invoking an empty callback will cause nothing happen. */
	GCallback() : super(NULL) {}

	/** Construct a callback from a member function. <br />
		obj is the object, can be NULL and setObject later. <br />
		func is the address of the function. <br />
		Note: invoking a callback with NULL object will cause memory access violation. <br />
	*/
	template<typename OT, typename FT>	GCallback(OT * obj, const FT & func);

	/** Construct a callback from a global function or functor object. <br />
		func is the address of the function, or a reference (not address) of the functor object.
	*/
	template<typename FT> 				GCallback(FT & func);

	/** Construct a callback from a global function or functor object. <br />
		func is the address of the function, or a reference (not address) of the functor object.
	*/
	template<typename FT> 				GCallback(const FT & func);

	/** Construct a callback from a global function or functor object. <br />
		func is the address of the function, or a reference (not address) of the functor object.
	*/
	template<typename FT> 				GCallback(const volatile FT & func);

	/** Construct a callback from other callback. */
    GCallback(const ThisType & other);

	/** Assign a callback from other callback. */
	GCallback & operator = (const ThisType & other);

	/** Equal operator. */
	bool operator == (const ThisType & other) const;

	/** Not-equal operator. */
	bool operator != (const ThisType & other) const;

	/**
		Invoke the callback. <br />
		@code
		GCallback<int, double> cb = &foo;
		int a = cb.invoke(0.5); // same as int a = foo(0.5);
		@endcode
	*/
	RT invoke(PT1 p1, PT2 p2 ... PTn pn) const;

	/**
		Invoke the callback. <br />
		@code
		GCallback<int, double> cb = &foo;
		int a = cb(0.5); // same as int a = foo(0.5);
		@endcode
	*/
	RT operator () (PT1 p1, PT2 p2 ... PTn pn) const;

	/**
		Get the object that bound to a member function. <br />
		For non-member function, always return NULL.
	*/
	const void * getObject() const;

	/**
		Set the object to bind to a member function. <br />
		No effect for non-member function.
	*/
	void setObject(void * obj);

	/**
		Return true if no function is set to the callback. <br />
		Return false if there is function.
	*/
	bool empty() const;
};


/**
	Auxiliary function to make a callback. <br />
	It accepts an object pointer and a member function address. <br />
	Return a callback to wrap the member function. <br />
	
	There is another verison makeCallbackN which N is the argument count. <br />
	The N version is useful for overloaded function. <br />
*/
template <typename OT, typename FT>
GCallback<typename GFunctionTraits<FT>::FunctionType>
makeCallback(OT * obj, const FT & func) {
	return GCallback<typename GFunctionTraits<FT>::FunctionType>(obj, func);
}

/**
	Auxiliary function to make a callback. <br />
	It accepts a global or static function address. <br />
	Return a callback to wrap the function. <br />
	
	There is another verison makeCallbackN which N is the argument count. <br />
	The N version is useful for overloaded function. <br />
*/
template <typename FT>
GCallback<typename GFunctionTraits<FT>::FunctionType>
makeCallback(const FT & func) {
	return GCallback<typename GFunctionTraits<FT>::FunctionType>(func);
}


} // namespace cpgf


