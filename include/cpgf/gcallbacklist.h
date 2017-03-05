#ifndef CPGF_GCALLBACKLIST_H
#define CPGF_GCALLBACKLIST_H

#include "cpgf/gcallback.h"

#include <deque>

namespace cpgf {

template <typename Signature>
class GCallbackList
{
private:
	typedef GCallback<Signature> CallbackType;
	typedef std::deque<CallbackType> CallbackListType;
	
public:
	GCallbackList() : callbackList() {
	}
	
	GCallbackList(const GCallbackList & other) : callbackList(other.callbackList) {
	}
	
	GCallbackList(GCallbackList && other) : callbackList(std::move(other.callbackList)) {
	}
	
	GCallbackList & operator = (GCallbackList other) {
		this->swap(other);
	}
	
	GCallbackList & operator = (GCallbackList && other) {
		this->swap(std::move(other));
	}
	
	int getCount() const {
		return (int)this->callbackList.size();
	}
	
	bool empty() const {
		return this->callbackList.empty();
	}
	
	void add(const CallbackType & callback)
	{
		this->callbackList.push_back(callback);
	}
	
	void remove(const CallbackType & callback)
	{
		const int count = this->getCount();
		for(int i = 0; i < count; ++i) {
			if(this->callbackList[i] == callback) {
				this->callbackList[i].clear();
			}
		}
	}
	
	void clear()
	{
		const int count = this->getCount();
		for(int i = 0; i < count; ++i) {
			this->callbackList[i].clear();
		}
	}

	template <typename... Parameters>
	void operator() (Parameters && ... args) const
	{
		int count = this->getCount();
		if(count > 0) {
			int index = 0;
			while(index < count) {
				auto & callback = this->callbackList[index];
				if(callback.empty()) {
					--count;
					this->callbackList.erase(this->callbackList.begin() + index);
				}
				else {
					++index;
					callback.invoke(std::forward<Parameters>(args)...);
				}
			}
		}
	}
	
	template <typename... Parameters>
	void dispatch(Parameters && ... args) const
	{
		(*this)(std::forward<Parameters>(args)...);
	}
	
	void swap(GCallbackList & other) noexcept
	{
		using namespace std;
		swap(this->callbackList, other.callbackList);
	}
	
	friend void swap(GCallbackList & a, GCallbackList & b)
	{
		a.swap(b);
	}

private:
	mutable CallbackListType callbackList;
};


} //namespace cpgf


#endif
