#ifndef __GCALLBACKLIST_H
#define __GCALLBACKLIST_H

#include "cpgf/gcallback.h"
#include "cpgf/gfunctiontraits.h"

#include <algorithm>


#define CB_PARAM_TYPEVALUE(N, P)		GPP_COMMA_IF(N) typename GArgumentTraits<P ## N>::Result  p ## N


#define CB_LIST_N(N) GPP_CONCAT(GCallbackList, N)

#define CB_DEF_LIST_N(N, PAR) \
	template < \
		typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT), \
		template<typename> class ConnectionT = GCallbackConnection, \
		template<typename T, typename = std::allocator<T> > class ListType = cpgf::GWiseList > \
	class CB_LIST_N(N) : public callback_internal::GCallbackListBase<GCallback<RT (GPP_REPEAT_PARAMS(N, PT))>, ConnectionT, ListType> \
	{ \
	public: \
		typedef GCallback<RT (GPP_REPEAT_PARAMS(N, PT))> CallbackType; \
	private: \
		typedef callback_internal::GCallbackListBase<CallbackType, ConnectionT, ListType> super; \
		typedef CB_LIST_N(N)<RT GPP_REPEAT_TAIL_PARAMS(N, PT), ConnectionT, ListType> ThisType; \
	public: \
		typedef typename super::DirectListType DirectListType; \
		typedef typename super::IteratorType IteratorType; \
		typedef typename super::ConnectionType ConnectionType; \
	public: \
		CB_LIST_N(N)() : super() { } \
		CB_LIST_N(N)(const CB_LIST_N(N) & other) : super(other) { } \
		CB_LIST_N(N) & operator = (const CB_LIST_N(N) & other) { \
			super::operator = (other); \
			return *this; \
		} \
		void dispatch(GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT) ) const { \
			if(this->directList.empty()) return; \
			this->canceled = false; \
			const IteratorType endIt = const_cast<ThisType *>(this)->directList.end(); \
			for(IteratorType it = const_cast<ThisType *>(this)->directList.begin(); it != endIt; ++it) { \
				const_cast<ThisType *>(this)->beforeCall(it); \
				it->getCallback().invoke(GPP_REPEAT_PARAMS(N, p) ); \
				const_cast<ThisType *>(this)->afterCall(it); \
				if(this->canceled) { break; } \
			} \
		} \
		void operator () (GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT) ) const { this->dispatch(GPP_REPEAT_PARAMS(N, p)); } \
	};

#define CB_DEF_FUNC_TRAITS(N, P) \
	template <typename Signature, template<typename> class ConnectionT, template<typename T, typename = std::allocator<T> > class ListType> \
	struct GCallbackListFunctionTraits <N, Signature, ConnectionT, ListType> { \
		typedef GFunctionTraits<Signature> TraitsType; \
		typedef GCallbackList ## N <typename TraitsType::ResultType GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename TraitsType::ArgList::Arg), ConnectionT, ListType> CallbackListNType; \
	};

namespace cpgf {

template <typename CallbackType>
class GCallbackConnection {
public:
	// all functions are required by callback list

	template <typename FT>
	explicit GCallbackConnection(const FT & func) : callback(func) {
	}

	explicit GCallbackConnection(CallbackType cb) {
		this->callback.takeOver(cb);
	}

	GCallbackConnection(const GCallbackConnection & other) : callback(other.callback) {
	}

	GCallbackConnection & operator = (GCallbackConnection other) {
		this->callback.takeOver(other.callback);

		return *this;
	}

	CallbackType & getCallback() const {
		return const_cast<CallbackType &>(this->callback);
	}

	void beforeInvoke() {
	}

	void afterInvoke() {
	}

	bool needRemove() const {
		return false;
	}

private:
	CallbackType callback;
};


template <typename CallbackType>
class GCallbackExtendedConnection {
public:
	template <typename FT>
	explicit GCallbackExtendedConnection(const FT & func) : callback(func), autoRemoveCalls(-1), tag(0){
	}

	explicit GCallbackExtendedConnection(CallbackType cb) : autoRemoveCalls(-1), tag(0) {
		this->callback.takeOver(cb);
	}

	GCallbackExtendedConnection(const GCallbackExtendedConnection & other) : callback(other.callback), autoRemoveCalls(other.autoRemoveCalls), tag(other.tag) {
	}

	GCallbackExtendedConnection & operator = (GCallbackExtendedConnection other) {
		this->callback.takeOver(other.callback);
		this->autoRemoveCalls = other.autoRemoveCalls;
		this->tag = other.tag;

		return *this;
	}

	CallbackType & getCallback() const {
		return const_cast<CallbackType &>(this->callback);
	}

	void beforeInvoke() {
	}

	void afterInvoke() {
		if(this->autoRemoveCalls > 0) {
			--this->autoRemoveCalls;
		}
	}

	bool needRemove() const {
		return this->autoRemoveCalls == 0;
	}

public:
	void autoRemoveAfterFirstCall() {
		this->autoRemoveCalls = 1;
	}

	void autoRemoveAfterCalls(int calls) {
		this->autoRemoveCalls = calls;
	}

	void setTag(int tag) {
		this->tag = tag;
	}

	int getTag() const {
		return this->tag;
	}

private:
	CallbackType callback;
	int autoRemoveCalls;
	int tag;
};


class GCallbackTrackable;

namespace callback_internal {

class GCallbackTrackManager
{
private:
	typedef std::pair<GCallbackTrackable *, const void *> PairType;
	typedef GWiseList<PairType> ListType;
	typedef ListType::iterator IteratorType;
	// use simple function pointer to avoid template parameter
	typedef void (* RemoveConnectionFunc)(void * param, const void * connection);

public:
	GCallbackTrackManager(RemoveConnectionFunc func, void * param) : func(func), param(param) {
	}

	GCallbackTrackManager(const GCallbackTrackManager & other) : itemList(other.itemList), func(other.func), param(other.param) {
	}

	~GCallbackTrackManager() {
		this->untrackAll();
	}

	GCallbackTrackManager & operator = (const GCallbackTrackManager & other) {
		if(this != &other) {
			this->itemList = other.itemList;
			this->func = other.func;
			this->param = other.param;
		}

		return *this;
	}

	inline void trackCallback(GCallbackTrackable * trackable, const void * connection);

	inline void trackableWillDestroy(GCallbackTrackable * trackable);
	inline void connectionWillDestroy(const void * connection);

protected:
	inline void untrackAll();

private:
	ListType itemList;
	RemoveConnectionFunc func;
	void * param;
};

} // namespace callback_internal


class GCallbackTrackable
{
private:
	typedef GWiseList<callback_internal::GCallbackTrackManager *> ListType;
	typedef ListType::iterator IteratorType;

public:
	GCallbackTrackable() {
	}

	GCallbackTrackable(const GCallbackTrackable & other) {
		(void)other;
	}

	~GCallbackTrackable() {
		this->notifyAll();
	}

	GCallbackTrackable & operator = (const GCallbackTrackable & other) {
		if(this != &other) {
			this->notifyAll();
		}

		return *this;
	}

protected:
	void willBeTracked(callback_internal::GCallbackTrackManager * trackManager) {
		if(std::find(this->trackManagerList.begin(), this->trackManagerList.end(), trackManager) == this->trackManagerList.end()) {
			this->trackManagerList.push_back(trackManager);
		}
	}

	void willBeUntracked(callback_internal::GCallbackTrackManager * trackManager) {
		this->trackManagerList.remove(trackManager);
	}

	void notifyAll() {
		for(IteratorType it = this->trackManagerList.begin(); it != this->trackManagerList.end(); ++it) {
			(*it)->trackableWillDestroy(this);
		}

		this->trackManagerList.clear();
	}

private:
	ListType trackManagerList;

	friend class callback_internal::GCallbackTrackManager;
};


namespace callback_internal {

void GCallbackTrackManager::trackCallback(GCallbackTrackable * trackable, const void * connection) {
	trackable->willBeTracked(this);
	this->itemList.push_back(std::make_pair(trackable, connection));
}

void GCallbackTrackManager::untrackAll() {
	for(IteratorType it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		it->first->willBeUntracked(this);
	}
}

void GCallbackTrackManager::trackableWillDestroy(GCallbackTrackable * trackable) {
	for(IteratorType it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		if(it->first == trackable) {
			this->func(this->param, it->second);
			it->first->willBeUntracked(this);
			this->itemList.erase(it);
		}
	}
}

void GCallbackTrackManager::connectionWillDestroy(const void * connection) {
	for(IteratorType it = this->itemList.begin(); it != this->itemList.end(); ++it) {
		if(it->second == connection) {
			it->first->willBeUntracked(this);
			this->itemList.erase(it);
		}
	}
}


template <typename CallbackT, template<typename> class ConnectionT, template<typename T, typename = std::allocator<T> > class ListType>
class GCallbackListBase
{
protected:
	typedef CallbackT CallbackType;
	typedef ConnectionT<CallbackType> ConnectionType;
	typedef ListType<ConnectionType> DirectListType;
	typedef typename DirectListType::iterator IteratorType;
	typedef typename DirectListType::const_iterator ConstIteratorType;

private:
	typedef GCallbackListBase<CallbackType, ConnectionT, ListType> ThisType;

public:
	GCallbackListBase() : canceled(false), tracker(NULL) {
	}

	GCallbackListBase(const GCallbackListBase & other) : directList(other.directList), canceled(other.canceled), tracker(other.tracker ? new callback_internal::GCallbackTrackManager(*other.tracker) : NULL) {
	}

	~GCallbackListBase() {
		this->clear();

		delete tracker;
	}

	GCallbackListBase & operator = (GCallbackListBase other) {
		this->swap(other);

		return *this;
	}

	void swap(GCallbackListBase & other) {
		this->directList.swap(other.directList);
		std::swap(this->canceled, other.canceled);
		std::swap(this->tracker, other.tracker);
	}

	template <typename FT>
	ConnectionType * add(const FT & func) {
		return this->add<FT>(func, NULL);
	}

	template <typename FT>
	ConnectionType * add(const FT & func, GCallbackTrackable * trackable) {
		return this->checkAddFunction(func, trackable);
	}

	ConnectionType * add(const CallbackType & cb) {
		return this->add(cb, NULL);
	}

	ConnectionType * add(const CallbackType & cb, GCallbackTrackable * trackable) {
		return this->checkAddCallback(cb, trackable);
	}

	template<typename OT, typename FT>
	ConnectionType * add(OT * instance, const FT & func) {
		return this->add<OT, FT>(instance, func, NULL);
	}

	template<typename OT, typename FT>
	ConnectionType * add(OT * instance, const FT & func, GCallbackTrackable * trackable) {
		return this->checkAddCallback(CallbackType(instance, func), trackable);
	}

	bool remove(const CallbackType & cb) {
		return checkRemove(doFindCallback(cb));
	}

	template<typename OT, typename FT>
	bool remove(OT * instance, const FT & func) {
		return checkRemove(doFindCallback(CallbackType(instance, func)));
	}

	bool remove(const ConnectionType & connection) {
		return checkRemove(doFindCallback(connection));
	}

	bool remove(int tag) {
		return checkRemove(doFindCallback(tag));
	}

	ConnectionType * find(const CallbackType & cb) const {
		return checkFind(const_cast<ThisType *>(this)->doFindCallback(cb));
	}

	template<typename OT, typename FT>
	ConnectionType * find(OT * instance, const FT & func) const {
		return checkFind(const_cast<ThisType *>(this)->doFindCallback(CallbackType(instance, func)));
	}

	ConnectionType * find(int tag) const {
		return checkFind(const_cast<ThisType *>(this)->doFindCallback(tag));
	}

	void clear() {
		if(this->tracker != NULL) {
			for(IteratorType it = directList.begin(); it != directList.end(); ++it) {
				this->freeConnection(*it);
			}
		}
		directList.clear();
	}

	void track(GCallbackTrackable * trackable, const void * connection) {
		if(connection != NULL) {
			if(this->tracker == NULL) {
				this->tracker = new callback_internal::GCallbackTrackManager(removeConnectionFunc, this);
			}
			this->tracker->trackCallback(trackable, connection);
		}
	}

	void cancel() const {
		this->canceled = true;
	}

	bool empty() const {
		return this->directList.empty();
	}

	// following advanced functions can access the internal data
	DirectListType & getDirectList() {
		return this->directList;
	}

	const DirectListType & getDirectList() const {
		return this->directList;
	}

protected:
	template <typename FT>
	ConnectionType * checkAddFunction(const FT & cb, GCallbackTrackable * trackable) {
		this->directList.push_back(ConnectionType(cb));
		return this->doTrack(&this->directList.back(), trackable);
	}

	ConnectionType * checkAddCallback(const CallbackType & cb, GCallbackTrackable * trackable) {
		this->directList.push_back(ConnectionType(cb));
		return this->doTrack(&this->directList.back(), trackable);
	}

	bool checkRemove(const IteratorType & it) {
		return this->freeIterator(it);
	}

	ConnectionType * checkFind(const IteratorType & it) const {
		if(it != const_cast<ThisType *>(this)->directList.end()) {
			return &(*it);
		}
		else {
			return NULL;
		}
	}

	ConnectionType * doTrack(ConnectionType * connection, GCallbackTrackable * trackable) {
		if(trackable != NULL) {
			this->track(trackable, connection);
		}

		return connection;
	}

	void freeConnection(const ConnectionType & connection) const {
		if(this->tracker != NULL) {
			this->tracker->connectionWillDestroy(&connection);
		}
	}

	bool freeIterator(const IteratorType & it) const {
		if(it != directList.end()) {
			this->freeConnection(*it);
			directList.erase(it);

			return true;
		}
		else {
			return false;
		}
	}

	void beforeCall(const IteratorType & it) const {
		it->beforeInvoke();
	}

	void afterCall(const IteratorType & it) const {
		it->afterInvoke();
		if(it->needRemove()) {
			this->freeIterator(it);
		}
	}

	IteratorType doFindCallback(const CallbackType & cb) {
		for(IteratorType it = directList.begin(); it != directList.end(); ++it) {
			if(it->getCallback() == cb) {
				return it;
			}
		}

		return directList.end();
	}

	IteratorType doFindCallback(const ConnectionType & connection) {
		for(IteratorType it = directList.begin(); it != directList.end(); ++it) {
			if(&*it == &connection) {
				return it;
			}
		}

		return directList.end();
	}

	IteratorType doFindCallback(int tag) {
		for(IteratorType it = directList.begin(); it != directList.end(); ++it) {
			if(it->tag == tag) {
				return it;
			}
		}

		return directList.end();
	}

private:
	static void removeConnectionFunc(void * param, const void * connection) {
		GCallbackListBase<CallbackType, ConnectionT, ListType> * instance = static_cast<GCallbackListBase<CallbackType, ConnectionT, ListType> * >(param);
		for(IteratorType it = instance->directList.begin(); it != instance->directList.end(); ++it) {
			if(&(*it) == connection) {
				instance->directList.erase(it);
			}
		}
	}

protected:
	mutable DirectListType directList;
	volatile mutable bool canceled;
	callback_internal::GCallbackTrackManager * tracker;
};


GPP_REPEAT_2(CB_MAX_ARITY, CB_DEF_LIST_N, GPP_EMPTY)

template <int arity, typename FT, template<typename> class ConnectionT, template<typename T, typename = std::allocator<T> > class ListType>
class GCallbackListFunctionTraits;

GPP_REPEAT_2(CB_MAX_ARITY, CB_DEF_FUNC_TRAITS, GPP_EMPTY)

} // namespace callback_internal


template <
	typename Signature,
	template<typename> class ConnectionT = GCallbackConnection,
	template<typename T, typename = std::allocator<T> > class ListType = cpgf::GWiseList >
class GCallbackList : public callback_internal::GCallbackListFunctionTraits<GFunctionTraits<Signature>::Arity, Signature, ConnectionT, ListType>::CallbackListNType {
private:
	typedef typename callback_internal::GCallbackListFunctionTraits<GFunctionTraits<Signature>::Arity, Signature, ConnectionT, ListType>::CallbackListNType super;

public:
	typedef typename super::CallbackType CallbackType;
	typedef typename super::DirectListType DirectListType;
	typedef typename super::IteratorType IteratorType;
	typedef typename super::ConnectionType ConnectionType;
};


} //namespace cpgf


#undef CB_PARAM_TYPEVALUE
#undef CB_DEF_FUNC_TRAITS
#undef CB_DEF_LIST_N
#undef CB_LIST_N


#endif
