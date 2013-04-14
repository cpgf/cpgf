#include "testcaseutil.h"

#include "cpgf/tween/gtween.h"
#include "cpgf/accessor/gaccessor.h"


using namespace cpgf;

void calculateEaseChartPositions(std::vector<float> & positions, const GTweenEaseType & ease, float start, float end)
{
	const size_t steps = 1000;

	const float deltaX = (end - start) / steps;

	float x = start;
	float y = start;

	GTween tweenY;
	tweenY.target(createAccessor(NULL, &y, &y), end)
		.ease(ease)
		.duration((float)steps)
		.useFrames(true)
	;
	for(size_t i = 0; i < steps; ++i) {
		tweenY.tick(1);
		positions.push_back(x);
		positions.push_back(y);
		x += deltaX;
	}

	float maxValue = 0.0f;
	float minValue = 100000.0f;
	size_t valueCount = positions.size();
	for(size_t i = 0; i < valueCount; ++i) {
		float value = positions[i];
		if(value > maxValue) {
			maxValue = value;
		}
		if(value < minValue) {
			minValue = value;
		}
	}
	if(minValue < start) {
		maxValue += start - minValue;
		for(size_t i = 0; i < valueCount; ++i) {
			float value = positions[i];
			value = value + (start - minValue);
			positions[i] = value;
		}
	}
	if(maxValue > end || minValue < start) {
		float scale = (end - start) / (maxValue - minValue);
		for(size_t i = 0; i < valueCount; ++i) {
			float value = positions[i];
			value = start + (value - start) * scale;
			positions[i] = value;
		}
	}
}

