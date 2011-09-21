#ifndef __GEVENT_H
#define __GEVENT_H

#include <queue>
#include <map>

#include "cpgf/gcallbacklist.h"
#include "cpgf/gcontainer.h"
#include "cpgf/gtypetraits.h"


namespace cpgf {


int registerEventMessage();
int registerEventMessage(const char * eventName);


class GEvent;

template <typename EventIndexType, typename EventType>
class GEventDispatcherBase;

struct IGEventReceiver
{
	virtual void processEvent(GEvent * e) = 0;
};


template <typename T, typename F>
class EventFunctorMember
{
private:
	typedef typename RemoveConst<typename RemoveReference<typename GFunctionTraits<F>::ArgList::Arg0>::Result>::Result EventType;

public:
	EventFunctorMember(T * obj, const F & func) : obj(obj), func(func) {
	}

	void operator() (const GEvent & e) const {
		(this->obj->*func)(static_cast<const EventType &>(e));
	}

private:
	T * obj;
	F func;
};

template <typename F, typename E>
class EventFunctorGlobal
{
private:
	typedef typename RemoveConst<typename RemoveReference<E>::Result>::Result EventType;
public:
	EventFunctorGlobal(const F & func) : func(func) {
	}

	void operator() (const GEvent & e) const {
		(*&const_cast<typename RemoveConst<F>::Result &>(func))(static_cast<const EventType &>(e));
	}

private:
	F func;
};

template <typename T, typename F>
EventFunctorMember<T, F> makeEventListener(T * obj, const F & f)
{
	return EventFunctorMember<T, F>(obj, f);
}

template <typename F>
EventFunctorGlobal<F, typename GFunctionTraits<F>::ArgList::Arg0> makeEventListener(const F & f)
{
	return EventFunctorGlobal<F, typename GFunctionTraits<F>::ArgList::Arg0>(f);
}

template <typename A, typename F>
EventFunctorGlobal<F, A> makeEventFunctorListener(const F & f)
{
	return EventFunctorGlobal<F, A>(f);
}


template <typename EventIndexType, typename EventType>
class GEventDispatcherBase
{
public:
	typedef GCallback<void (const EventType &)> CallbackType;
	typedef GCallbackList<void (const EventType &)> CallbackListType;

private:
	typedef GWiseMap<EventIndexType, CallbackListType *> MapType;
	typedef typename CallbackListType::ConnectionType ConnectionType;
	typedef GEventDispatcherBase<EventIndexType, EventType> DispatcherType;

private:
	class EventFinalizer {
	public:
		EventFinalizer(const EventType & e, const DispatcherType * dispatcher, CallbackListType * callbackList) : e(const_cast<EventType *>(&e)) {
			this->e->setDispatcher(const_cast<DispatcherType * >(dispatcher), callbackList);
		}

		~EventFinalizer() {
			this->e->setDispatcher(NULL, NULL);
		}

	private:
		EventType * e;
	};

public:
	GEventDispatcherBase() {
	}

	GEventDispatcherBase(const GEventDispatcherBase & other) : eventMap(other.eventMap) {
	}

	virtual ~GEventDispatcherBase() {
		this->clearListeners();
	}

	GEventDispatcherBase & operator = (const GEventDispatcherBase & other) {
		this->eventMap = other.eventMap;
	}

	ConnectionType * addListener(const EventIndexType & type, const CallbackType & listener, GCallbackTrackable * trackable = NULL) {
		CallbackListType * cbList = this->getCallbackList(type);
		return cbList->add(listener, trackable);
	}

	ConnectionType * addListenerAtHead(const EventIndexType & type, const CallbackType & listener, GCallbackTrackable * trackable = NULL) {
		CallbackListType * cbList = this->getCallbackList(type);
		cbList->getDirectList().push_front(typename CallbackListType::ConnectionType(listener, trackable));
		ConnectionType * connection = &cbList->getDirectList().front();
		cbList->track(trackable, connection);
		return connection;
	}

	void removeListener(const EventIndexType & type, const CallbackType & listener) {
		typename MapType::iterator it = this->eventMap.find(type);
		if(it != this->eventMap.end()) {
			it->second->remove(listener);

			if(it->second->empty()) {
				delete it->second;
				this->eventMap.erase(it);
			}
		}
	}

	void sendEvent(const EventType & e) const {
		EventIndexType type = e.getMessage();
		typename MapType::const_iterator it = this->eventMap.find(type);

		if(it != this->eventMap.end()) {
			EventFinalizer f(e, this, it->second);

			it->second->dispatch(e);
		}
	}

	bool hasEvent(const EventIndexType & type) const {
		CallbackListType * cbList = this->findCallbackList(type);

		return cbList != NULL && ! cbList->empty();
	}

	void clearListeners() {
		for(typename MapType::iterator it = this->eventMap.begin(); it != this->eventMap.end(); ++it) {
			delete it->second;
		}

		this->eventMap.clear();
	}

	CallbackListType * findCallbackList(const EventIndexType & type) const {
		typename MapType::const_iterator it = this->eventMap.find(type);

		if(it == this->eventMap.end()) {
			return NULL;
		}
		else {
			return it->second;
		}
	}

	CallbackListType * getCallbackList(const EventIndexType & type) {
		typename MapType::iterator it = this->eventMap.find(type);

		if(it == this->eventMap.end()) {
			return this->eventMap.insert(type, new CallbackListType())->second;
		}
		else {
			return it->second;
		}
	}

private:
	MapType eventMap;
};


class GEvent
{
protected:
	typedef GEventDispatcherBase<int, GEvent> DispatcherType;

	static const int FlagPreventedDefault = 1 << 0;
	static const int FlagDispatchStoppable = 1 << 1;

public:
	explicit GEvent(int message)
		: message(message), source(NULL), flags(0), timeStamp(0), receiver(NULL), dispatcher(NULL), callbackList(NULL)
	{
	}

	GEvent(int message, const void * source)
		: message(message), source(const_cast<void *>(source)), flags(0), timeStamp(0), receiver(NULL), dispatcher(NULL), callbackList(NULL)
	{
	}

	GEvent(const GEvent & other)
		: message(other.message), source(other.source), flags(other.flags), timeStamp(other.timeStamp), receiver(other.receiver), dispatcher(other.dispatcher), callbackList(other.callbackList)
	{
	}

	virtual ~GEvent() {
	}

	GEvent & operator = (GEvent other) {
		this->swap(other);

		return *this;
	}

	void swap(GEvent & other) {
		using std::swap;

		swap(this->message, other.message);
		swap(this->source, other.source);
		swap(this->flags, other.flags);
		swap(this->timeStamp, other.timeStamp);
		swap(this->receiver, other.receiver);
		swap(this->dispatcher, other.dispatcher);
		swap(this->callbackList, other.callbackList);
	}

	int getMessage() const {
		return this->message;
	}

	long getTimeStamp() const {
		return this->timeStamp;
	}

	void setTimeStamp(long timeStamp) {
		this->timeStamp = timeStamp;
	}

	void * getSource() const {
		return this->source;
	}

	void stopDispatch() const {
		if(this-> getDispatchStoppable() && this->callbackList != NULL) {
			this->callbackList->cancel();
		}
	}

	void setDispatchStoppable(bool stoppable) {
        this->maskFlag(FlagDispatchStoppable, stoppable);
    }

	virtual bool getDispatchStoppable() const {
        return this->hasFlag(FlagDispatchStoppable);
    }

	void preventDefault() const {
		this->maskFlag(FlagPreventedDefault, true);
	}

	bool isPreventedDefault() const {
		return this->hasFlag(FlagPreventedDefault);
	}

	void setReceiver(IGEventReceiver * receiver) {
		this->receiver = receiver;
	}

	IGEventReceiver * getReceiver() const {
		return this->receiver;
	}

protected:
	void setDispatcher(DispatcherType * dispatcher, DispatcherType::CallbackListType * callbackList) const {
		this->dispatcher = dispatcher;
		this->callbackList = callbackList;
	}

private:
	void maskFlag(int flag, bool setFlag) const {
		if(setFlag) {
			this->flags |= flag;
		}
		else {
			this->flags &= ~flag;
		}
	}

	bool hasFlag(int flag) const {
		return (this->flags & flag) != 0;
	}

protected:
	int message;
	void * source;
	mutable int flags;
	long timeStamp;
	IGEventReceiver * receiver;

	mutable DispatcherType * dispatcher;
	mutable DispatcherType::CallbackListType * callbackList;

    template <typename EventIndexType, typename EventType>
    friend class GEventDispatcherBase;
};


class GEventDispatcher : public GEventDispatcherBase<int, GEvent>
{
};


typedef GCallback<void (const GEvent &)> GEventListener;


class GEventQueue : public GEventDispatcher
{
private:
	typedef GEventDispatcher super;

public:
	GEventQueue() : super() {
	}

	virtual ~GEventQueue() {
		this->clearEvents();
	}

	template <typename ET>
	void postEvent(const ET & e) {
		eventQueue.push(new ET(e));
	}

	GEvent * peekNextEvent() {
		return this->getOrRemoveNextEvent(false);
	}

	GEvent * getNextEvent() {
		return this->getOrRemoveNextEvent(true);
	}

	bool processNextEvent();
	void processEvents();

	void clearEvents();

protected:
	GEvent * getOrRemoveNextEvent(bool remove);
	void doProcessEvent(GEvent * e);

private:
	std::queue<GEvent *> eventQueue;
};



} // namespace cpgf


#endif

