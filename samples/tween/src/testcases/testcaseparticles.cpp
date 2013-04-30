#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "testcase.h"

#include "../ui/sprite.h"
#include "../easeinfo.h"

#include "cpgf/tween/gtweenlist.h"
#include "cpgf/accessor/gaccessor.h"

#include "wx/log.h"

#if defined(_WIN32)
    #include <windows.h>
#endif
#include <GL/gl.h>

#include <vector>

using namespace std;
using namespace cpgf;

class TestCaseParticles : public TestCase
{
public:
	TestCaseParticles();

	virtual std::string getDescription();
	virtual void render(int viewWidth, int viewHeight);

protected:
	virtual void doReset();

private:
	void setRatio(float ratio);

private:
	vector<float> fromTo; // fromX, fromY, toX, toY
	vector<float> particles; // startX, startY, endX, endY
};

const int particleCountInRow = 100;
const int particleCountInColumn = 100;
const float xOffset = 0.005f;
const float yOffset = 0.0f;
const int duration = 2000;

TestCasePtr createTestCaseParticles()
{
	return TestCasePtr(new TestCaseParticles);
}

TestCaseParticles::TestCaseParticles()
{
}

std::string TestCaseParticles::getDescription()
{
	char buffer[100];
	itoa(particleCountInRow * particleCountInColumn, buffer, 10);
	return string("Animate ") + buffer + " particles at the same time.\n"
		"This test demonstrate how to use tween in CPU data cache friendly manner.\n"
		"\n"
		"This test maybe not real very CPU cache friendly,\n"
		"but it demonstrates the idea of how to change lots of properties at once."
	;
}

void TestCaseParticles::render(int viewWidth, int viewHeight)
{
	size_t count = this->particles.size();
	if(count == 0) {
		return;
	}

    glDisable(GL_LIGHTING);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertexPointer(2, GL_FLOAT, 0, &this->particles[0]);
	glDrawArrays(GL_LINES, 0, count / 2);
}

void TestCaseParticles::setRatio(float ratio)
{
	size_t count = this->particles.size();
	for(size_t i = 0; i < count; i += 4) {
		float x = this->fromTo[i] + (this->fromTo[i + 2] - this->fromTo[i]) * ratio;
		float y = this->fromTo[i + 1] + (this->fromTo[i + 3] - this->fromTo[i + 1]) * ratio;
		this->particles[i] = x;
		this->particles[i + 1] = y;
		this->particles[i + 2] = x + xOffset;
		this->particles[i + 3] = y + yOffset;
	}
}

void TestCaseParticles::doReset()
{
	const float marginX = 0.1f;
	const float marginY = 0.1f;
	const float spaceX = (2.0f - marginX * 2) / (particleCountInRow - 1);
	const float spaceY = (2.0f - marginY * 2) / (particleCountInColumn - 1);

	this->fromTo.clear();
	this->particles.clear();

	for(int c = 0; c < particleCountInColumn; ++c) {
		for(int r = 0; r < particleCountInRow; ++r) {
			float x = marginX + spaceX * r;
			float y = marginY + spaceY * c;
			
			this->fromTo.push_back(x);
			this->fromTo.push_back(y);
			this->fromTo.push_back(1.0f);
			this->fromTo.push_back(1.0f);

			this->particles.push_back(x);
			this->particles.push_back(y);
			this->particles.push_back(x + xOffset);
			this->particles.push_back(y + yOffset);
		}
	}

	GTween & tween = GTweenList::getInstance()->tween()
		// Passing 0 to createAccessor to disable getter.
		// We don't need getter since we passed 0.0f as "from" to target().
		.target(createAccessor(this, 0, &TestCaseParticles::setRatio), 0.0f, 1.0f)
	;

	this->setTweenable(&tween);
}

