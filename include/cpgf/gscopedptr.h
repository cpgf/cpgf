#ifndef __GSCOPEDPTR_H
#define __GSCOPEDPTR_H


#include <stddef.h>

namespace cpgf {


template <typename T>
class GScopedPointerDeleter_Delete
{
public:
	static inline void Delete(T * p) {
		delete p;
	}
};

template <typename T>
class GScopedPointerDeleter_DeleteArray
{
public:
	static inline void Delete(T * p) {
		delete[] p;
	}
};

template <typename T, typename Deleter = GScopedPointerDeleter_Delete<T> >
class GScopedPointer
{
private:
    typedef GScopedPointer<T, Deleter> ThisType;

public:
	GScopedPointer(): rawPointer(NULL) {
	}

	explicit GScopedPointer(T * p) : rawPointer(p) {
	}

	~GScopedPointer() {
		Deleter::Delete(this->rawPointer);
	}

	inline void reset(T * p = NULL) {
		if(p != this->rawPointer) {
			ThisType(p).swap(*this);
		}
	}

	inline T & operator * () const {
		return *this->rawPointer;
	}

	inline T * operator -> () const {
		return this->rawPointer;
	}

    inline bool operator ! () const {
		return this->rawPointer == NULL;
    }

	inline T * get() const {
		return this->rawPointer;
	}

	inline T * take() {
		T * p = this->rawPointer;
		
		this->rawPointer = NULL;
		
		return p;
	}

	inline operator bool() {
		return this->rawPointer != NULL;
	}

	inline operator bool() const {
		return this->rawPointer != NULL;
	}

	inline void swap(GScopedPointer & b) {
		T * temp = b.rawPointer;
		b.rawPointer = this->rawPointer;
		this->rawPointer = temp;
	}

private:
	GScopedPointer(const GScopedPointer &);
	GScopedPointer & operator = (const GScopedPointer &);

	void operator == (const GScopedPointer &) const;
	void operator != (const GScopedPointer &) const;

protected:
	T * rawPointer;
};


template <typename T, typename Deleter = GScopedPointerDeleter_DeleteArray<T> >
class GScopedArray : public GScopedPointer<T, Deleter>
{
public:
    explicit GScopedArray(T * p = NULL)
        : GScopedPointer<T, Deleter>(p) {
    }

    inline T & operator [] (int i) {
        return this->rawPointer[i];
    }

    inline const T & operator [] (int i) const {
        return this->rawPointer[i];
    }

private:
	GScopedArray(const GScopedArray &);
	GScopedArray & operator = (const GScopedArray &);

	void operator == (const GScopedArray &) const;
	void operator != (const GScopedArray &) const;
};



} // namespace cpgf



#endif
