namespace cpgf {

/**
The trackable object is used to automatically disconnect callback from callback list when the object is destroyed. <br />
Unlike other implementation such as boost Signals or libsigc++, you don't need to inherite your functor from the trackable. <br />
Instead what you need to do is to keep the trackable and functor's life time exactly the same. <br />
@code
class SomeObject {
public:
	void someFunc() { do something here }
	
	GCallbackTrackable trackable;
};
SomeObject obj = SomeObject();
SomeCallbackList.add(&obj, &SomeObject::someFunc, &SomeObject.trackable);
@endcode
When obj is destroyed or out of scope, it will be auto removed from SomeCallbackList.
*/
class GCallbackTrackable
{
public:
	/** Constructor */
	GCallbackTrackable();

	/** Copy constructor */
	GCallbackTrackable(const GCallbackTrackable & other);

	/** Destructor */
	~GCallbackTrackable();

	/** Assign operator */
	GCallbackTrackable & operator = (const GCallbackTrackable & other);
};



/**
The connection object is used to manage callback which added to callback list. <br />
The container returned by getDirectList() in callback list holds a list of connection object. <br />
Use GCallbackList<Params>::ConnectionType to access the instantialized type. <br />
To use tag or auto remove features, or want to write your own connection type, see GCallbackExtendedConnection. <br />
*/
template <typename CallbackType>
class GCallbackConnection
{
public:
	/** Construct an empty connection. */
	GCallbackConnection();

	/**
		Construct a connection from a callback. <br />
		Note: cb will be destroyed after connected. If it's not intended to destroy cb, use cb.copy() to make a copy.
	*/
	explicit GCallbackConnection(const CallbackType & cb);

	/**
		Get the callback.
	*/
	CallbackType * getCallback() const;
};



/**
This is an extended version of GCallbackConnection. It supports tag and auto remove features. <br />
The connection object is used to manage callback which added to callback list. <br />
The container returned by getDirectList() in callback list holds a list of connection object. <br />
Use GCallbackList<Params>::ConnectionType to access the instantialized type. <br />
*/
template <typename CallbackType>
class GCallbackExtendedConnection
{
public:
	/** Construct an empty connection. */
	GCallbackExtendedConnection();

	/**
		Construct a connection from a callback. <br />
		Note: cb will be destroyed after connected. If it's not intended to destroy cb, use cb.copy() to make a copy.
	*/
	explicit GCallbackExtendedConnection(const CallbackType & cb);

	/**
		Enable auto disconnect after first call of the callback.
	*/
	void autoRemoveAfterFirstCall();

	/**
		Enable auto disconnect after calls number call of the callback.
	*/
	void autoRemoveAfterCalls(int calls);

	/**
		Set a tag for the connection. Then you can control the callbacks by tag in callback list.
	*/
	void setTag(int tag);

	/**
		Get the tag.
	*/
	int getTag() const;

	/**
		Get the callback.
	*/
	CallbackType * getCallback() const;
};



/**
The callback list is useful to hold multiple callbacks. <br />
The callback list is the same concept of signal in other implementation, such as Qt, boost::Signals, and libsigc++. <br />
The template parameter 'Signature' is the function type, or a pointer type that points to a function (aka, function pointer). <br />
The template parameter 'ConnectionT' is the connection type, see GCallbackConnection and GCallbackExtendedConnection. The default value is GCallbackConnection. <br />
The template parameter 'ListType' is the internal list type to manage the callbacks. It must be STL list compatible. The default value is cpgf::GWiseList. <br />

About the Signature, you can read document of GCallback for more information. <br />

Callback list is copyable. Copying a callback list will cause all callbacks duplicated to another list. <br />
But keep in mind that copying a callback list is quite expensive (iterate over the internal list, duplicate each callback, insert to new list). <br />
So it will be better to avoid copying a callback list. <br />
*/
template <
	typename Signature,
	template<typename> class ConnectionT = GCallbackConnection,
	template<typename T, typename = std::allocator<T> > class ListType = cpgf::GWiseList >
class GCallbackList
{
public:
	/**
		Connection class type. <br />
		A connection object will be returned by "add" function.
	*/
	typedef typename super::GCallbackConnection ConnectionType;

	/**
		Callback type. <br />
		Can be used to declare new callbacks.
	*/
	typedef GCallback<RT, PT1, PT2 ... PTn > CallbackType;


	/**
		The internal list type. <br />
		The list can be accessed by calling getDirectList.
	*/
	typedef cpgf::GWiseList<GCallbackConnection> DirectListType;

	/**
		The iterator type to go over DirectListType.
	*/
	typedef typename DirectListType::iterator IteratorType;
	
public:
	/**
		Construct a callback list.
	*/
	GCallbackList();

	/**
		Construct a callback list from another list.
	*/
	GCallbackList(const GCallbackList & other);
	
	/**
		Assign operator.
	*/
	GCallbackList & operator = (const GCallbackList & other);
	
	/**
		Invoke the callbacks. <br />
		Doesn't support return value. <br />
		If return value does matter, use getDirectList to access the internal list and call each callbacks manually.
	*/
	void dispatch(PT1 p1, PT2 p2 ... PTn pn) const;
	
	/**
		Invoke the callbacks. Same as dispatch.
	*/
	void operator () (PT1 p1, PT2 p2 ... PTn pn) const;

	/**
		Add a global function or functor object to list. <br />
		Return a connection which can be used to control the callback such as set tag. <br />
		Usually you don't need to retain the connection. <br />
		This function is introduced to optimize to avoid unnecessary object copy.
	*/
	template <typename FT>
	ConnectionType * add(const FT & func);

	/**
		Add a global function or functor object to list. <br />
		Return a connection which can be used to control the callback such as set tag. <br />
		Usually you don't need to retain the connection. <br />
		This function is introduced to optimize to avoid unnecessary object copy.
	*/
	template <typename FT>
	ConnectionType * add(const FT & func, GCallbackTrackable * trackable);

	/**
		Add a callback to list. <br />
		Return a connection which can be used to control the callback such as set tag. <br />
		Usually you don't need to retain the connection. <br />
		Note: global function or functor object can be added directly using this function without converting to callback because it will be converted implicitly.
	*/
	ConnectionType * add(const CallbackType & cb);

	/**
		Add a callback to list and track the connection. <br />
		Return a connection which can be used to control the callback such as set tag. <br />
		Usually you don't need to retain the connection. <br />
		Note: global function or functor object can be added directly using this function without converting to callback because it will be converted implicitly.
	*/
	ConnectionType * add(const CallbackType & cb, GCallbackTrackable * trackable);

	/**
		Add a member function to list. <br />
		Return a connection which can be used to control the callback such as set tag. <br />
		Usually you don't need to retain the connection.
	*/
	template<typename OT, typename FT>
	ConnectionType * add(OT * obj, const FT & func);

	/**
		Add a member function to list and track the connection. <br />
		Return a connection which can be used to control the callback such as set tag. <br />
		Usually you don't need to retain the connection.
	*/
	template<typename OT, typename FT>
	ConnectionType * add(OT * obj, const FT & func, GCallbackTrackable * trackable);

	/**
		Remove first found callback from the list. <br />
		Return true if success. <br />
		Return false if not found.
	*/
	bool remove(const CallbackType & cb);

	/**
		Remove first found callback which match the member function from the list. <br />
		Return true if success. <br />
		Return false if not found.
	*/
	template<typename OT, typename FT>
	bool remove(OT * obj, const FT & func);

	/**
		Remove first found callback that owned by the connection from the list. <br />
		Return true if success. <br />
		Return false if not found.
	*/
	bool remove(ConnectionType * connection);

	/**
		Remove first found callback that has the same tag from the list. <br />
		Return true if success. <br />
		Return false if not found.
	*/
	bool remove(int tag);

	/**
		Find a connection from a callback. <br />
		Return NULL if not found.
	*/
	ConnectionType * find(const CallbackType & cb) const;

	/**
		Find a connection from a member function. <br />
		Return NULL if not found.
	*/
	template<typename OT, typename FT>
	ConnectionType * find(OT * obj, const FT & func) const;

	/**
		Find a connection from a tag. <br />
		Return NULL if not found.
	*/
	ConnectionType * find(int tag) const;

	/**
		Remove all callbacks.
	*/
	void clear();

	/**
		Track a connection with a trackable object.
	*/
	void track(GCallbackTrackable * trackable, void * connection);

	/**
		Cancel current dispatch. <br />
		If this function is called during dispatch, all later callbacks will not be executed.
	*/
	void cancel() const;

	/**
		Return true if there is no callbacks in the list.
	*/
	bool empty() const;

	/**
		Return the internal list.
	*/
	DirectListType & getDirectList();

	/**
		Return the internal list, const version.
	*/
	const DirectListType & getDirectList() const;
};



}
