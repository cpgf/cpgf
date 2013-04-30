#include "test_tween_common.h"
#include "cpgf/tween/gtween.h"
#include "cpgf/accessor/gaccessor.h"
#include "cpgf/gsharedptr.h"
#include "cpgf/tween/easing/linear.h"

#include <cstdarg>
#include <vector>


using namespace std;
using namespace cpgf;

namespace {

int x = 1;
	
const int END = 0;


typedef vector<int> ListType;
typedef GSharedPointer<GTween> Pointer;

Pointer createTween()
{
	Pointer tween(new GTween);
	
	x = 1;
	tween->target(createAccessor(NULL, &x, &x), 5)
		.ease(LinearEase::ease())
		.duration(4)
		.useFrames(true)
	;
	
	return tween;
}

Pointer createRelativeTween()
{
	Pointer tween(new GTween);
	
	x = 1;
	tween->relative(createAccessor(NULL, &x, &x), 4)
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

GTEST(Tween_default)
{
	Pointer tween = createTween();
	verify(tween.get(), range(2, 5), 5, END);
	
	tween = createRelativeTween();
	verify(tween.get(), range(2, 5), 5, END);
}

GTEST(Tween_backward)
{
	Pointer tween = createTween();
	tween->backward(true);
	verify(tween.get(), range(4, 1), END);

	tween = createRelativeTween();
	tween->backward(true);
	verify(tween.get(), range(4, 1), END);
}

GTEST(Tween_delay)
{
	Pointer tween = createTween();
	tween->delay(3);
	verify(tween.get(), 1, 1, 1, range(2, 5), END);

	tween = createRelativeTween();
	tween->delay(3);
	verify(tween.get(), 1, 1, 1, range(2, 5), END);
}

GTEST(Tween_repeat)
{
	Pointer tween = createTween();
	tween->repeat(1);
	verify(tween.get(), range(2, 5), range(2, 5), 5, 5, END);

	tween = createRelativeTween();
	tween->repeat(1);
	verify(tween.get(), range(2, 5), range(2, 5), 5, 5, END);
}

GTEST(Tween_repeat_repeatDelay)
{
	Pointer tween = createTween();
	tween->repeat(2)
		.repeatDelay(2)
	;
	verify(tween.get(), range(2, 5), 5, 5, range(2, 5), 5, 5, END);

	tween = createRelativeTween();
	tween->repeat(2)
		.repeatDelay(2)
	;
	verify(tween.get(), range(2, 5), 5, 5, range(2, 5), 5, 5, END);
}

GTEST(Tween_repeat_yoyo)
{
	Pointer tween = createTween();
	tween->repeat(1)
		.yoyo(true)
	;
	verify(tween.get(), range(2, 5), range(4, 1), 1, 1, END);

	tween = createRelativeTween();
	tween->repeat(1)
		.yoyo(true)
	;
	verify(tween.get(), range(2, 5), range(4, 1), 1, 1, END);
}

GTEST(Tween_repeat_infinitely)
{
	Pointer tween = createTween();
	tween->repeat(-1);
	verify(tween.get(), range(2, 5), range(2, 5), range(2, 5), range(2, 5), range(2, 5), range(2, 5), END);

	tween = createRelativeTween();
	tween->repeat(-1);
	verify(tween.get(), range(2, 5), range(2, 5), range(2, 5), range(2, 5), range(2, 5), range(2, 5), END);
}

GTEST(Tween_backward_delay)
{
	Pointer tween = createTween();
	tween->backward(true)
		.delay(2)
	;
	verify(tween.get(), 1, 1, range(4, 1), END);

	tween = createRelativeTween();
	tween->backward(true)
		.delay(2)
	;
	verify(tween.get(), 1, 1, range(4, 1), END);
}

GTEST(Tween_backward_repeat)
{
	Pointer tween = createTween();
	tween->backward(true)
		.repeat(1)
	;
	verify(tween.get(), range(4, 1), range(4, 1), 1, 1, END);

	tween = createRelativeTween();
	tween->backward(true)
		.repeat(1)
	;
	verify(tween.get(), range(4, 1), range(4, 1), 1, 1, END);
}

GTEST(Tween_backward_repeat_repleatDelay)
{
	Pointer tween = createTween();
	tween->backward(true)
		.repeat(1)
		.repeatDelay(2)
	;
	verify(tween.get(), range(4, 1), 1, 1, range(4, 1), 1, 1, END);

	tween = createRelativeTween();
	tween->backward(true)
		.repeat(1)
		.repeatDelay(2)
	;
	verify(tween.get(), range(4, 1), 1, 1, range(4, 1), 1, 1, END);
}

GTEST(Tween_backward_repeat_yoyo)
{
	Pointer tween = createTween();
	tween->backward(true)
		.repeat(1)
		.yoyo(true)
	;
	verify(tween.get(), range(4, 1), range(2, 5), 5, 5, END);

	tween = createRelativeTween();
	tween->backward(true)
		.repeat(1)
		.yoyo(true)
	;
	verify(tween.get(), range(4, 1), range(2, 5), 5, 5, END);
}

GTEST(Tween_parameters)
{
	GTween tween;
	GCHECK(! tween.isBackward());
	GCHECK(! tween.isCompleted());
	GCHECK(! tween.isPaused());
	GCHECK(! tween.isRunning());
	GCHECK(! tween.isUseFrames());
	GCHECK(! tween.isYoyo());
	GCHECK(! tween.isRepeat());
	GCHECK(! tween.isRepeatInfinitely());
	GEQUAL(0, tween.getRepeatCount());
}


}
