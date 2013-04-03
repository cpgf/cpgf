#include "cpgf/tween/gtweenlist.h"
#include "cpgf/gscopedptr.h"

// for test
#include "cpgf/accessor/gaccessor.h"
#include <iostream>
using namespace std;


namespace cpgf {


GTweenList * GTweenList::getInstance()
{
	static GScopedPointer<GTweenList> instance;

	if(! instance) {
		instance.reset(new GTweenList);
	}

	return instance.get();
}

GTweenList::GTweenList()
{
}

GTweenList::~GTweenList()
{
}

GTween * GTweenList::createTween()
{
	GTween * tween = this->tweenPool.allocate();
	this->tweenList.push_back(tween);
	return tween;
}

GTween * GTweenList::to(GTweenNumber duration)
{
	GTween * tween = this->createTween();
	tween->duration(duration);
	return tween;
}

GTween * GTweenList::from(GTweenNumber duration)
{
	GTween * tween = this->createTween();
	tween->duration(duration).backward(true);
	return tween;
}

void GTweenList::tick(GTweenNumber frameTime)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		(*it)->tick(frameTime);
		if((*it)->isCompleted()) {
			this->tweenPool.free(*it);
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}
}

void GTweenList::removeTweenOf(const void * instance)
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end();) {
		(*it)->removeOf(instance);
		if((*it)->isCompleted()) {
			this->tweenPool.free(*it);
			it = this->tweenList.erase(it);
		}
		else {
			++it;
		}
	}
}

void GTweenList::clear()
{
	for(ListType::iterator it = this->tweenList.begin(); it != this->tweenList.end(); ++it) {
		this->tweenPool.free(*it);
	}
	this->tweenList.clear();
}


} // namespace cpgf


using namespace cpgf;
void testTween()
{
	float x = 1.0;
	GTweenList::getInstance()->createTween()
		->tween(createInstanceAccessor(NULL, &x, &x), 10)
		.duration(9)
		.backward(true)
		.useFrames(true)
//		.delay(2)
		.repeat(2)
		.yoyo(true)
//		.repeatDelay(3)
	;
	for(int i = 0; i < 20; ++i) {
		GTweenList::getInstance()->tick(10);
		cout << i << " : " << x << endl;
	}
}

