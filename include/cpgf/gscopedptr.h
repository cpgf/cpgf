#ifndef CPGF_GSCOPEDPTR_H
#define CPGF_GSCOPEDPTR_H


#include <stddef.h>

namespace cpgf {


template <typename T>
struct GScopedPointerDeleter_Delete
{
	static inline void Delete(T * p) {
		delete p;
	}
};

template <typename T>
struct GScopedPointerDeleter_DeleteArray
{
	static inline void Delete(T * p) {
		delete[] p;
	}
};

template <typename T>
struct GScopedPointerResetPredict
{
	static inline bool CanReset(T * pointerOfMine, T * pointerToReset) {
		return pointerOfMine != pointerToReset;
	}
};

template <typename T, typename Deleter = GScopedPointerDeleter_Delete<T>, typename ResetPredict = GScopedPointerResetPredict<T> >
class GScopedPointer
{
private:
    typedef GScopedPointer<T, Deleter, ResetPredict> ThisType;

public:
	GScopedPointer(): rawPointer(nullptr) {
	}

	explicit GScopedPointer(T * p) : rawPointer(p) {
	}

	~GScopedPointer() {
		T * p = this->rawPointer;
		this->rawPointer = nullptr;
		Deleter::Delete(p);
	}

	inline void reset(T * p = nullptr) {
		if(ResetPredict::CanReset(this->rawPointer, p)) {
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
		return this->rawPointer == nullptr;
	}

	inline T * get() const {
		return this->rawPointer;
	}

	inline T * take() {
		T * p = this->rawPointer;
		
		this->rawPointer = nullptr;
		
		return p;
	}

	inline operator bool() {
		return this->rawPointer != nullptr;
	}

	inline operator bool() const {
		return this->rawPointer != nullptr;
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

template <typename T, typename D, typename R>
inline void swap(GScopedPointer<T, D, R> & a, GScopedPointer<T, D, R> & b)
{
	a.swap(b);
}


template <typename T, typename Deleter = GScopedPointerDeleter_DeleteArray<T> >
class GScopedArray : public GScopedPointer<T, Deleter>
{
public:
    explicit GScopedArray(T * p = nullptr)
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
