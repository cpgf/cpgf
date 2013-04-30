#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "testcase.h"

#include "../ui/sprite.h"
#include "../easeinfo.h"
#include "testcaseutil.h"

#include "cpgf/tween/gtweenlist.h"
#include "cpgf/accessor/gaccessor.h"

#if defined(_WIN32)
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>

#include <vector>

using namespace cpgf;
using namespace std;

class TestCaseAllCharts : public TestCase
{
private:
	typedef vector<float> EasePositions;
	typedef vector<EasePositions> AllEasePositions;

public:
	TestCaseAllCharts();

	virtual std::string getDescription();
	virtual void render(int viewWidth, int viewHeight);
	virtual void setEase(int easeIndex);
	virtual bool shouldShowEaseButtons() {
		return false;
	}
	virtual bool shouldShowCommandButtons() {
		return false;
	}

private:
	void populatePositions();

private:
	AllEasePositions positions;
};

TestCasePtr createTestCaseAllCharts()
{
	return TestCasePtr(new TestCaseAllCharts);
}

const int cellCountPerLine = 6;
const float cellSpace = 0.01f;
const float cellMargin = cellSpace;
const float cellSize = (2.0f - (cellCountPerLine - 1) * cellSpace - cellMargin * 2) / cellCountPerLine;

TestCaseAllCharts::TestCaseAllCharts()
	: positions(getEaseCount())
{
	this->populatePositions();
}

void drawString(const char * str)
{
	glColor4f(0, 1.0f, 0, 0.5f);
	glPushAttrib(GL_LIGHTING_BIT + GL_CURRENT_BIT);
	glDisable(GL_LIGHTING);
	glRasterPos3f(0.01f, cellSize / 2, 0.0f);
	int i = 0;
	while(str[i]) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
		++i;
	}
	glEnable(GL_LIGHTING);
	glPopAttrib();
}

std::string TestCaseAllCharts::getDescription()
{
	return "Display charts for all easing function visually.";
}

void TestCaseAllCharts::render(int viewWidth, int viewHeight)
{
    glDisable(GL_LIGHTING);

	const int easeCount = getEaseCount();
	int easeIndex = 0;
	for(int i = 0; i < cellCountPerLine; ++i) {
		for(int k = 0; k < cellCountPerLine; ++k) {
			easeIndex = i * cellCountPerLine + k;
			if(easeIndex >= easeCount) {
				break;
			}

			glPushMatrix();

			glTranslatef(0, 2.0f - (cellSize + cellSpace), 0);
			glTranslatef(k * (cellSize + cellSpace) + cellMargin, -i * (cellSize + cellSpace), 0);

			glBegin(GL_LINE_LOOP);
			glColor3f(0, 0, 1.0f);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, cellSize);
			glVertex2f(cellSize, cellSize);
			glVertex2f(cellSize, 0.0f);
			glEnd();

			glColor3f(0.8f, 0.8f, 0.8f);
			glVertexPointer(2, GL_FLOAT, 0, &this->positions[easeIndex][0]);
			glDrawArrays(GL_LINE_STRIP, 0, this->positions[easeIndex].size() / 2);

			const char * name = getEase(easeIndex)->name;
			drawString(name);

			glPopMatrix();
		}
		if(easeIndex >= easeCount) {
			break;
		}
	}
}

void TestCaseAllCharts::setEase(int easeIndex)
{
}

void TestCaseAllCharts::populatePositions()
{
	const int easeCount = getEaseCount();

	for(int i = 0; i < easeCount; ++i) {
		EasePositions & pos = this->positions[i];
		pos.clear();
		calculateEaseChartPositions(pos, getEase(i)->ease, 0.005f, cellSize - 0.005f);
	}
}

