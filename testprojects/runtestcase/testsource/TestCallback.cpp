/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>

#include "../../include/cpgf/testcase/TestCallback.h"

static TestCallback suite_TestCallback;

static CxxTest::List Tests_TestCallback = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestCallback( "../../include/cpgf/testcase/TestCallback.h", 425, "TestCallback", suite_TestCallback, Tests_TestCallback );

static class TestDescription_TestCallback_testSize : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCallback_testSize() : CxxTest::RealTestDescription( Tests_TestCallback, suiteDescription_TestCallback, 435, "testSize" ) {}
 void runTest() { suite_TestCallback.testSize(); }
} testDescription_TestCallback_testSize;

static class TestDescription_TestCallback_testMisc : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCallback_testMisc() : CxxTest::RealTestDescription( Tests_TestCallback, suiteDescription_TestCallback, 446, "testMisc" ) {}
 void runTest() { suite_TestCallback.testMisc(); }
} testDescription_TestCallback_testMisc;

static class TestDescription_TestCallback_testInherited : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCallback_testInherited() : CxxTest::RealTestDescription( Tests_TestCallback, suiteDescription_TestCallback, 516, "testInherited" ) {}
 void runTest() { suite_TestCallback.testInherited(); }
} testDescription_TestCallback_testInherited;

static class TestDescription_TestCallback_testBasicCallback : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCallback_testBasicCallback() : CxxTest::RealTestDescription( Tests_TestCallback, suiteDescription_TestCallback, 526, "testBasicCallback" ) {}
 void runTest() { suite_TestCallback.testBasicCallback(); }
} testDescription_TestCallback_testBasicCallback;

static class TestDescription_TestCallback_testGlobalCompare : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCallback_testGlobalCompare() : CxxTest::RealTestDescription( Tests_TestCallback, suiteDescription_TestCallback, 670, "testGlobalCompare" ) {}
 void runTest() { suite_TestCallback.testGlobalCompare(); }
} testDescription_TestCallback_testGlobalCompare;

static class TestDescription_TestCallback_testCallbackList : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCallback_testCallbackList() : CxxTest::RealTestDescription( Tests_TestCallback, suiteDescription_TestCallback, 766, "testCallbackList" ) {}
 void runTest() { suite_TestCallback.testCallbackList(); }
} testDescription_TestCallback_testCallbackList;

static class TestDescription_TestCallback_testTrackable : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCallback_testTrackable() : CxxTest::RealTestDescription( Tests_TestCallback, suiteDescription_TestCallback, 822, "testTrackable" ) {}
 void runTest() { suite_TestCallback.testTrackable(); }
} testDescription_TestCallback_testTrackable;

static class TestDescription_TestCallback_testRecursive : public CxxTest::RealTestDescription {
public:
 TestDescription_TestCallback_testRecursive() : CxxTest::RealTestDescription( Tests_TestCallback, suiteDescription_TestCallback, 867, "testRecursive" ) {}
 void runTest() { suite_TestCallback.testRecursive(); }
} testDescription_TestCallback_testRecursive;

