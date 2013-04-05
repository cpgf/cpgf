#ifndef __GTWEENLIST_H
#define __GTWEENLIST_H


#include "cpgf/tween/gtween.h"
#include "cpgf/gmemorypool.h"

#include <list>


namespace cpgf {


class GTweenList
{
private:
	typedef std::list<GTween *> ListType;

public:
	static GTweenList * getInstance();

public:
	GTweenList();
	~GTweenList();

	// Return pointer instead of reference because reference maybe wrongly copied.
	GTween & createTween();
	
	GTween & to(GTweenNumber duration);
	GTween & from(GTweenNumber duration);

	template <typename AccessorType>
	GTween & to(GTweenNumber duration, const AccessorType & accessor, const typename AccessorType::ValueType & target) {
		return this->to(duration).tween(accessor, target);
	}

	template <typename AccessorType>
	GTween & from(GTweenNumber duration, const AccessorType & accessor, const typename AccessorType::ValueType & target) {
		return this->from(duration).tween(accessor, target);
	}

	void tick(GTweenNumber frameTime);

	void removeTweenOf(const void * instance);
	void clear();

private:
	ListType tweenList;
	GObjectPool<GTween> tweenPool;
};


} // namespace cpgf


#endif
