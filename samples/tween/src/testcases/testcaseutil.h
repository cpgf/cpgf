#ifndef __TESTCASEUTIL_H
#define __TESTCASEUTIL_H

#include "cpgf/tween/gtweencommon.h"

#include <vector>


void calculateEaseChartPositions(std::vector<float> & positions, const cpgf::GTweenEaseType & ease, float start, float end);


#endif
