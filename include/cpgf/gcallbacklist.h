#ifndef GCALLBACKLIST_H
#define GCALLBACKLIST_H

#include "gcallback.h"

#include <deque>

namespace cpgf {

template <typename Signature>
class GCallbackList
{
private:
	typedef GCallback<Signature> CallbackType;
	typedef std::deque<CallbackType> CallbackList;
	
public:
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
	void dispatch(Parameters && ... args) const
	{
		const int count = this->getCount();
		if(count > 0) {
			int index = 0;
			auto it = this->callbackList.begin();
			while(index < count) {
				if(it->empty()) {
					it = this->callbackList.erase(it);
				}
				else {
					it->invoke(std::forward<Parameters>(args)...);
					++it;
				}
				++index;
			}
		}
	}

	// duplicate code from emit to avoid forwarding arguments.
	template <typename... Parameters>
	void operator() (Parameters && ... args) const
	{
		const int count = this->getCount();
		if(count > 0) {
			int index = 0;
			auto it = this->callbackList.begin();
			while(index < count) {
				if(it->empty()) {
					it = this->callbackList.erase(it);
				}
				else {
					it->invoke(std::forward<Parameters>(args)...);
					++it;
				}
				++index;
			}
		}
	}

private:
	mutable CallbackList callbackList;
};


} //namespace cpgf


#endif
