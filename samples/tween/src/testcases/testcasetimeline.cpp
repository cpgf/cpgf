#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "testcase.h"

#include "../ui/sprite.h"
#include "../easeinfo.h"

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
#include "cpgf/tween/easing/sine.h"

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

	virtual std::string getDescription();
	virtual void render(int viewWidth, int viewHeight);
	virtual bool shouldShowEaseButtons() {
		return false;
	}
	virtual bool shouldShowCommandButtons() {
		return true;
	}

protected:
	virtual void doReset();

private:
	Sprite sprites[SpriteCount];
};

const int startX = 200;
const int endX = SpriteBoardSize - startX;
const int startY = SpriteBoardSize - startX;
const int endY = startY;
const int size = 50;
const int distanceX = 100;
const int distanceY = 100;

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
}

std::string TestCaseTimeline::getDescription()
{
	return "Play animations in a timeline.\n"
		"Several tweens and nested timelines are on the master timeline."
	;
}

void TestCaseTimeline::render(int viewWidth, int viewHeight)
{
    glEnable(GL_LIGHTING);

    for(int i = 0; i < SpriteCount; ++i) {
		this->sprites[i].render(viewWidth, viewHeight);
    }
}

void TestCaseTimeline::doReset()
{
	for(int i = 0; i < SpriteCount; ++i) {
		this->sprites[i].setShape(Sprite::ssTeapot);
		this->sprites[i].setX(startX + size * 1.5 * i);
		this->sprites[i].setY(startY);
		this->sprites[i].setZ(0);
		this->sprites[i].setAlpha(1.0f);
		this->sprites[i].setSize(size);
		this->sprites[i].setColor(colors[i]);
		this->sprites[i].setRotate(0);
	}
	this->sprites[1].setSize(size / 2);

	GTimeline & timeline = GTweenList::getInstance()->timeline();
	GTweenNumber t;

	timeline.append(
		timeline.tween()
			.duration(this->getDuration())
			.ease(ElasticEase::easeIn())
			.relative(createAccessor(&this->sprites[0], &Sprite::getX, &Sprite::setX), -distanceX)
	);
	GTimeline & timeline2 = timeline.timeline();

	timeline2.append(
		timeline2.tween()
			.duration(this->getDuration())
			.ease(SineEase::easeIn())
			.target(createAccessor(&this->sprites[1], &Sprite::getRotate, &Sprite::setRotate), 360)
	);
	timeline2.prepend(
		timeline2.tween()
			.duration(this->getDuration())
			.ease(QuintEase::easeIn())
			.target(createAccessor(&this->sprites[1], &Sprite::getSize, &Sprite::setSize), size)
	);
	t = timeline.append(
		timeline.tween()
			.duration(this->getDuration())
			.ease(CubicEase::easeIn())
			.relative(createAccessor(&this->sprites[2], &Sprite::getX, &Sprite::setX), distanceX)
	);
	timeline.insert(
		t,
		timeline2
	);

	t = timeline.append(
		timeline.tween()
			.duration(this->getDuration())
			.ease(BounceEase::easeOut())
			.relative(createAccessor(&this->sprites[0], &Sprite::getY, &Sprite::setY), -100)
	);
	timeline.setAt(t,
		timeline.tween()
			.duration(this->getDuration())
			.ease(BounceEase::easeOut())
			.relative(createAccessor(&this->sprites[1], &Sprite::getY, &Sprite::setY), 100)
	);
	timeline.setAt(t,
		timeline.tween()
			.duration(this->getDuration())
			.ease(BounceEase::easeOut())
			.relative(createAccessor(&this->sprites[2], &Sprite::getY, &Sprite::setY), -100)
	);

	this->setTweenable(&timeline);
}

