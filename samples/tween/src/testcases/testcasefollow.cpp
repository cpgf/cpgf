#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "testcase.h"

#include "../ui/sprite.h"
#include "../easeinfo.h"

#include "cpgf/tween/gtweenlist.h"
#include "cpgf/accessor/gaccessor.h"

#if defined(_WIN32)
    #include <windows.h>
#endif
#include <GL/gl.h>

using namespace cpgf;

class TestCaseFollow : public TestCase
{
public:
	TestCaseFollow();

	virtual std::string getDescription();
	virtual void render(int viewWidth, int viewHeight);

protected:
	virtual void doReset();

private:
	Sprite sprite;
	Sprite target;
	bool inited;
};

const int startX = 150;
const int endX = SpriteBoardSize - startX;
const int startY = endX;
const int endY = startX;

TestCasePtr createTestCaseFollow()
{
	return TestCasePtr(new TestCaseFollow);
}

TestCaseFollow::TestCaseFollow() : inited(false)
{
}

std::string TestCaseFollow::getDescription()
{
	return "Play tweening animation with moving target.\n"
		"The ease and tween parameters can be changed in the left panel."
	;
}

void TestCaseFollow::render(int viewWidth, int viewHeight)
{
    glEnable(GL_LIGHTING);

	this->sprite.render(viewWidth, viewHeight);
	this->target.render(viewWidth, viewHeight);
}

void TestCaseFollow::doReset()
{
	this->sprite.setX(startX);
	this->sprite.setY(startY);
	this->sprite.setZ(0);
	this->sprite.setAlpha(1.0f);
	this->sprite.setSize(60);
	this->sprite.setColor(0x7777ff);

	this->target.setX(startX);
	this->target.setY(endY);
	this->target.setZ(0);
	this->target.setAlpha(1.0f);
	this->target.setSize(this->sprite.getSize());
	this->target.setColor(0xff7777);

	GTween & tween = GTweenList::getInstance()->tween()
		.duration(this->getDuration())
		.follow(createAccessor(&this->sprite, &Sprite::getX, &Sprite::setX), createGetter(&this->target, &Sprite::getX))
		.follow(createAccessor(&this->sprite, &Sprite::getY, &Sprite::setY), createGetter(&this->target, &Sprite::getY))
	;

	if(! inited) {
		inited = true;
	}
	else {
		GTweenList::getInstance()->tween()
			.duration(this->getDuration())
			.target(createAccessor(&this->target, &Sprite::getX, &Sprite::setX), endX)
			.useFrames(this->useFrames())
		;
	}

	this->setTweenable(&tween);
}

