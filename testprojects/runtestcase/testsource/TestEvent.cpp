/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>

#include "../../include/cpgf/testcase/TestEvent.h"

static TestEvent suite_TestEvent;

static CxxTest::List Tests_TestEvent = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestEvent( "../../include/cpgf/testcase/TestEvent.h", 70, "TestEvent", suite_TestEvent, Tests_TestEvent );

static class TestDescription_TestEvent_test000 : public CxxTest::RealTestDescription {
public:
 TestDescription_TestEvent_test000() : CxxTest::RealTestDescription( Tests_TestEvent, suiteDescription_TestEvent, 82, "test000" ) {}
 void runTest() { suite_TestEvent.test000(); }
} testDescription_TestEvent_test000;

static class TestDescription_TestEvent_test1 : public CxxTest::RealTestDescription {
public:
 TestDescription_TestEvent_test1() : CxxTest::RealTestDescription( Tests_TestEvent, suiteDescription_TestEvent, 84, "test1" ) {}
 void runTest() { suite_TestEvent.test1(); }
} testDescription_TestEvent_test1;

