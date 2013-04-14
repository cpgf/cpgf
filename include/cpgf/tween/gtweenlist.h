#ifndef __GTWEENLIST_H
#define __GTWEENLIST_H


#include "cpgf/tween/gtween.h"
#include "cpgf/gmemorypool.h"

#include <list>


namespace cpgf {


class GTweenList : public GTweenable
{
private:
	typedef GTweenable super;

protected:
	struct TweenData
	{
		GTweenNumber startTime;
		GTweenable * tween;
	};

	typedef std::list<TweenData> ListType;

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
		return this->to(duration).target(accessor, target);
	}

	template <typename AccessorType>
	GTween & from(GTweenNumber duration, const AccessorType & accessor, const typename AccessorType::ValueType & target) {
		return this->from(duration).target(accessor, target);
	}

	void clear();

public:
	virtual bool removeOf(const void * instance);

protected:
	virtual void performTime(GTweenNumber frameTime);

protected:
	void freeTween(GTweenable * tween);

private:
	ListType tweenList;
	GObjectPool<GTween> tweenPool;
};


} // namespace cpgf


#endif
