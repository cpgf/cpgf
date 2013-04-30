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

class TestCaseBasic : public TestCase
{
public:
	TestCaseBasic();

	virtual std::string getDescription();
	virtual void render(int viewWidth, int viewHeight);
	virtual bool shouldShowEaseButtons() {
		return false;
	}
	virtual bool shouldShowCommandButtons() {
		return true;
	}
	virtual bool shouldShowPauseResumeButtons() {
		return false;
	}

protected:
	virtual void doReset();

private:
	Sprite sprite;
	Sprite target;
};

const int startX = 150;
const int endX = SpriteBoardSize - startX;
const int startY = endX;
const int endY = startX;

TestCasePtr createTestCaseBasic()
{
	return TestCasePtr(new TestCaseBasic);
}

TestCaseBasic::TestCaseBasic()
{
}

std::string TestCaseBasic::getDescription()
{
	return "Very basic animation to show certain tween parameters.\n"
		"The ease and tween parameters can NOT be changed.\n"
		"\n"
		"The code in this test case is very straightforward, unlike the other test cases."
	;
}

void TestCaseBasic::render(int viewWidth, int viewHeight)
{
    glEnable(GL_LIGHTING);

	this->sprite.render(viewWidth, viewHeight);
	this->target.render(viewWidth, viewHeight);
}

void TestCaseBasic::doReset()
{
	this->sprite.setX(startX);
	this->sprite.setY(startY);
	this->sprite.setZ(0);
	this->sprite.setAlpha(1.0f);
	this->sprite.setSize(60);
	this->sprite.setColor(0x7777ff);
	this->sprite.setRotate(0);

	this->target.setX(endX);
	this->target.setY(endY);
	this->target.setZ(0);
	this->target.setAlpha(0.1f);
	this->target.setSize(this->sprite.getSize());
	this->target.setColor(0x7777ff);

	/*GTween & tween = */ GTweenList::getInstance()->tween()
		.target(createAccessor(&this->sprite, &Sprite::getX, &Sprite::setX), endX)
		.target(createAccessor(&this->sprite, &Sprite::getY, &Sprite::setY), endY)
		.target(createAccessor(&this->sprite, &Sprite::getRotate, &Sprite::setRotate), 360)
		.duration(2000)
		.repeat(tweenRepeatInfinitely)
		.backward(true)
		.yoyo(true)
	;
}

