#include "test_tween_common.h"
#include "cpgf/tween/gtween.h"
#include "cpgf/accessor/gaccessor.h"
#include "cpgf/gsharedptr.h"
#include "cpgf/tween/easing/linear.h"

#include <cstdarg>
#include <vector>

// for test
#include <iostream>


using namespace std;
using namespace cpgf;

namespace {
	int x = 1;
	
	const int END = 0;
}

typedef vector<int> ListType;
typedef GSharedPointer<GTween> Pointer;

Pointer createTween()
{
	Pointer tween(new GTween);
	
	x = 1;
	tween->tween(createAccessor(NULL, &x, &x), 5)
		.ease(LinearEase::ease())
		.duration(4)
		.useFrames(true)
	;
	
	return tween;
}

// [from, to]
int range(int from, int to)
{
	return from * 1000 + to;
}

void populate(ListType & valueList, int value)
{
	if(value < 1000) {
		valueList.push_back(value);
	}
	else {
		int from = value / 1000;
		int to = value % 1000;
		while(from != to) {
			valueList.push_back(from);
			if(from < to) {
				++from;
			}
			else {
				--from;
			}
		}
		valueList.push_back(to);
	}
}

void verify(GTween * tween, ...)
{
	ListType valueList;

	va_list v;
	va_start(v, tween);
	for(;;) {
		int value = va_arg(v, int);
		if(value == END) {
			break;
		}
		populate(valueList, value);
	}
	va_end(v);

	for(size_t i = 0; i < valueList.size(); ++i) {
		tween->tick(1);
		if(valueList[i] == x) {
		}
		else {
			GASSERT(false);
		}
	}
}

/*
GTEST(Fake)
{
	int x = 1;
	Pointer tween = createTween();
	tween->tween(createAccessor(NULL, &x, &x), 5)
		.duration(4)
//		.backward(true)
		.useFrames(true)
//		.delay(2)
		.repeat(2)
		.yoyo(true)
//		.repeatDelay(3)
	;
	for(int i = 0; i < 20; ++i) {
		tween->tick(10);
		cout << i << " : " << x << endl;
	}
}
*/

GTEST(Tween_default)
{
	Pointer tween = createTween();
	verify(tween.get(), range(1, 5), END);
}

GTEST(Tween_backward)
{
	Pointer tween = createTween();
	tween->backward(true);
	verify(tween.get(), range(5, 1), END);
}

GTEST(Tween_delay)
{
	Pointer tween = createTween();
	tween->delay(3);
	verify(tween.get(), 1, 1, 1, range(1, 5), END);
}

GTEST(Tween_immediateTick)
{
	Pointer tween = createTween();
	tween->immediateTick(true);
	verify(tween.get(), range(2, 5), END);
}

GTEST(Tween_repeat)
{
	Pointer tween = createTween();
	tween->repeat(2);
	verify(tween.get(), range(1, 5), range(1, 5), 5, 5, END);
}

GTEST(Tween_repeat_repeatDelay)
{
	Pointer tween = createTween();
	tween->repeat(2)
		.repeatDelay(2)
	;
	verify(tween.get(), range(1, 5), 5, 5, range(1, 5), 5, 5, END);
}

GTEST(Tween_repeat_yoyo)
{
	Pointer tween = createTween();
	tween->repeat(2)
		.yoyo(true)
	;
	verify(tween.get(), range(1, 5), range(5, 1), 1, 1, END);
}

GTEST(Tween_repeat_yoyo_immediateYoyo)
{
	Pointer tween = createTween();
	tween->repeat(2)
		.yoyo(true)
		.immediateYoyo(true)
	;
	verify(tween.get(), range(1, 5), range(4, 1), 1, 1, END);
}

GTEST(Tween_backward_delay)
{
	Pointer tween = createTween();
	tween->backward(true)
		.delay(2)
	;
	verify(tween.get(), 1, 1, range(5, 1), END);
}

GTEST(Tween_backward_immediateTick)
{
	Pointer tween = createTween();
	tween->backward(true)
		.immediateTick(true)
	;
	verify(tween.get(), range(4, 1), END);
}

GTEST(Tween_backward_delay_immediateTick)
{
	Pointer tween = createTween();
	tween->backward(true)
		.delay(2)
		.immediateTick(true)
	;
	verify(tween.get(), 1, 1, range(4, 1), END);
}

GTEST(Tween_backward_repeat)
{
	Pointer tween = createTween();
	tween->backward(true)
		.repeat(2)
	;
	verify(tween.get(), range(5, 1), range(5, 1), 1, 1, END);
}

GTEST(Tween_backward_repeat_repleatDelay)
{
	Pointer tween = createTween();
	tween->backward(true)
		.repeat(2)
		.repeatDelay(2)
	;
	verify(tween.get(), range(5, 1), 1, 1, range(5, 1), 1, 1, END);
}

GTEST(Tween_backward_repeat_yoyo)
{
	Pointer tween = createTween();
	tween->backward(true)
		.repeat(2)
		.yoyo(true)
	;
	verify(tween.get(), range(5, 1), range(1, 5), 5, 5, END);
}

GTEST(Tween_backward_repeat_yoyo_immediateYoyo)
{
	Pointer tween = createTween();
	tween->backward(true)
		.repeat(2)
		.yoyo(true)
		.immediateYoyo(true)
	;
	verify(tween.get(), range(5, 1), range(2, 5), 5, 5, END);
}

GTEST(Tween_delay_immediateTick)
{
	Pointer tween = createTween();
	tween->delay(3)
		.immediateTick(true)
	;
	verify(tween.get(), 1, 1, 1, range(2, 5), END);
}

