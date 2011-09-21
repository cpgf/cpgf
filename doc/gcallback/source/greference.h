namespace cpgf {

/**
A wrapper class to be used to pass reference to other functions. <br />
With this wrapper, only the wrapper will be copied by the functions and the data will keep uncopied. <br />
It's useful to wrap non-copyable object to pass to functions that need to copy the parameters. <br />

Usually you don't need to use this class directly. <br />
The global functions makeReference (to make a normal reference) and makeConstReference (to make a constant reference) are the easy way to use the reference wrapper.

This class is the same concept of boost::ref library, though it works slightly different than boost::ref.
*/
template <typename T>
class GReference
{
public:
	/**
		The type of the data that the reference holds.
	*/
	typedef T Type;

public:
	/**
		Construct a wrapper object to reference to data.
	*/
	explicit GReference(T & data);

	/**
		Construct a wrapper object from another wrapper object.
	*/
	GReference(const GReference & other);

	/**
		Construct a wrapper object from another wrapper object.
	*/
	GReference(const volatile GReference & other);

	/**
		Copy operator. Assign a wrapper object from another wrapper object.
	*/
	GReference & operator = (const GReference & other);

	/**
		Get the data address. <br />
		This gives a way to access the data directly. <br />
		@code
			GReference<SomeType> foo(SomeData);
		@endcode
		&foo will represent SomeData's address. <br />
		And *&foo will represent SomeData's reference. <br />
	*/
	T * operator & () const;

	/**
		Convert operator. Convert the wrapper object to T&
	*/
	operator T& () const;

private:
	bool operator == (const GReference & other);

private:
	T * dataAddress;
};

/**
Wrap data to a reference wrapper object. <br />
There are overloaded functions to deal with wrapping reference to reference.
*/
template <typename T>
inline GReference<T> makeReference(T & data);


/**
Wrap constant data to a reference wrapper object. <br />
There are overloaded functions to deal with wrapping reference to reference.
*/
template <typename T>
inline GReference<const T> makeConstReference(const T & data);


}
