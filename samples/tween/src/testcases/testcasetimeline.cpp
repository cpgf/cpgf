#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "testcase.h"

#include "sprite.h"
#include "easeinfo.h"

#include "cpgf/tween/gtimeline.h"
#include "cpgf/accessor/gaccessor.h"

#include "cpgf/tween/easing/linear.h"
#include "cpgf/tween/easing/back.h"
#include "cpgf/tween/easing/bounce.h"
#include "cpgf/tween/easing/circle.h"
#include "cpgf/tween/easing/cubic.h"
#include "cpgf/tween/easing/elastic.h"
#include "cpgf/tween/easing/exponent.h"
#include "cpgf/tween/easing/quad.h"
#include "cpgf/tween/easing/quart.h"
#include "cpgf/tween/easing/quint.h"
#include "cpgf/tween/easing/sin.h"

#include "wx/log.h"

#if defined(_WIN32)
    #include <windows.h>
#endif
#include <GL/gl.h>

using namespace cpgf;

const int SpriteCount = 3;

class TestCaseTimeline : public TestCase
{
public:
	TestCaseTimeline();

	virtual void render(int viewWidth, int viewHeight);
	virtual void setEase(int easeIndex);
	virtual void reset();
	virtual bool shouldShowEaseButtons() {
		return false;
	}

private:
	Sprite sprites[SpriteCount];
};

const int startX = 150;
const int endX = SpriteBoardSize - startX;
const int startY = endX;
const int endY = startY;
const int size = 30;
const int distanceX = (endX - startX) / (SpriteCount - 1);
const int distanceY = (endY - startY) / (SpriteCount - 1);
const int duration = 1000;

namespace {
const unsigned int colors[] = {
	0x7777ff, 0xff7777, 0x77ff77
};

}

TestCasePtr createTestCaseTimeline()
{
	return TestCasePtr(new TestCaseTimeline);
}

TestCaseTimeline::TestCaseTimeline()
{
	this->reset();
}

void TestCaseTimeline::render(int viewWidth, int viewHeight)
{
    glEnable(GL_LIGHTING);

    for(int i = 0; i < SpriteCount; ++i) {
		this->sprites[i].render(viewWidth, viewHeight);
    }
}

void TestCaseTimeline::setEase(int easeIndex)
{
}

struct OnComplete
{
	OnComplete() {}
	void operator() () const {
		wxLogDebug("Timeline complete");
	}
};

void TestCaseTimeline::reset()
{
	for(int i = 0; i < SpriteCount; ++i) {
		this->sprites[i].setShape(Sprite::ssBox);
		this->sprites[i].setX(startX + size * i);
		this->sprites[i].setY(startY);
		this->sprites[i].setZ(0);
		this->sprites[i].setAlpha(1.0f);
		this->sprites[i].setSize(size);
		this->sprites[i].setColor(colors[i]);
		this->sprites[i].setRotate(0);
	}
	this->sprites[1].setSize(size / 2);

	GTweenList::getInstance()->clear();
	
	GTimeline & timeline = GTweenList::getInstance()->createTimeline()
		.onComplete(OnComplete())
		.repeat(1)
		.yoyo(true)
		.backward(true)
	;

	timeline.append(
		timeline.to(duration)
			.ease(ElasticEase::easeIn())
			.relative(createAccessor(&this->sprites[0], &Sprite::getX, &Sprite::setX), -distanceX)
	);
	GTimeline & timeline2 = timeline.createTimeline();
	timeline2.append(
		timeline2.to(duration)
			.ease(QuintEase::easeIn())
			.target(createAccessor(&this->sprites[1], &Sprite::getSize, &Sprite::setSize), size)
	);
	timeline2.append(
		timeline2.to(duration)
			.ease(SinEase::easeIn())
			.target(createAccessor(&this->sprites[1], &Sprite::getRotate, &Sprite::setRotate), 360)
	);
	timeline.append(
		timeline2
	);
	timeline.append(
		timeline.to(duration)
			.ease(CubicEase::easeIn())
			.relative(createAccessor(&this->sprites[2], &Sprite::getX, &Sprite::setX), distanceX)
	);

	GTweenNumber t = timeline.append(
		timeline.to(duration)
			.ease(BounceEase::easeOut())
			.relative(createAccessor(&this->sprites[0], &Sprite::getY, &Sprite::setY), -100)
	);
	timeline.setAt(t,
		timeline.to(duration)
			.ease(BounceEase::easeOut())
			.relative(createAccessor(&this->sprites[1], &Sprite::getY, &Sprite::setY), 100)
	);
	timeline.setAt(t,
		timeline.to(duration)
			.ease(BounceEase::easeOut())
			.relative(createAccessor(&this->sprites[2], &Sprite::getY, &Sprite::setY), -100)
	);

	timeline.immediateTick();
}

