#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "testcase.h"

#include "sprite.h"
#include "easeinfo.h"

#include "cpgf/tween/gtweenlist.h"
#include "cpgf/accessor/gaccessor.h"

#if defined(_WIN32)
    #include <windows.h>
#endif
#include <GL/gl.h>

using namespace cpgf;

class TestCaseAnimation : public TestCase
{
public:
	TestCaseAnimation();

	virtual void render(int viewWidth, int viewHeight);
	virtual void setEase(int easeIndex);

private:
	void reset();

private:
	Sprite sprite;
};

const int startX = 150;
const int endX = 350;
const int startY = endX;
const int endY = startX;
const int duration = 2000;

TestCasePtr createTestCaseAnimation()
{
	return TestCasePtr(new TestCaseAnimation);
}

TestCaseAnimation::TestCaseAnimation()
{
	this->reset();
}

void TestCaseAnimation::render(int viewWidth, int viewHeight)
{
    glEnable(GL_LIGHTING);

	this->sprite.render(viewWidth, viewHeight);
}

void TestCaseAnimation::setEase(int easeIndex)
{
	GTweenList::getInstance()->clear();
	this->reset();

	GTweenEaseType ease = getEase(easeIndex)->ease;
	GTween & tween = GTweenList::getInstance()->to(duration)
		.ease(ease)
		.tween(createAccessor(&this->sprite, &Sprite::getX, &Sprite::setX), endX)
		.tween(createAccessor(&this->sprite, &Sprite::getY, &Sprite::setY), endY)
	;
}

void TestCaseAnimation::reset()
{
	this->sprite.setX(startX);
	this->sprite.setY(startY);
	this->sprite.setZ(0);
	this->sprite.setAlpha(1.0f);
	this->sprite.setSize(60);
	this->sprite.setColor(0x7777ff);
}

