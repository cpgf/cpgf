//#define NO_BOOST
//#define BOOST_SIGNAL

//#define NO_LIBSIGC
//#define NO_FD
//#define NO_SR


#if defined(_MSC_VER)
#pragma warning(push,0)
#pragma warning(disable:4503)
#endif

#include "cpgf/testcase/benchmark/benchmarkcallback.h"

#include <iostream>
#include <string>

#include "cpgf/gcallback.h"
#include "cpgf/gcallbacklist.h"

#ifndef NO_LIBSIGC
	#include "sigc++/functors/slot.h"
	#include "sigc++/signal.h"
#endif

#ifndef NO_BOOST
	#include "boost/signals2.hpp"
	#ifdef BOOST_SIGNAL
		#include "boost/signals.hpp"
	#endif

	#include "boost/bind.hpp"
#endif

#ifndef NO_FD
	// fast delegate
	// http://www.codeproject.com/KB/cpp/FastDelegate.aspx
	#include "FastDelegate.h"
	#include "FastDelegateBind.h"
#endif

#ifndef NO_SR
	// sr delegate
	// http://www.codeproject.com/KB/cpp/ImpossiblyFastCppDelegate.aspx?fid=199666&df=90&mpp=25&noise=3&sort=Position&view=Quick&fr=1#xx0xx
	#include "delegate.hpp"
#endif

using namespace std;
using namespace cpgf;

static clock_t previousClock;
static clock_t differentTick;

inline void beginClock() {
	previousClock = clock();
}

inline void endClock() {
	differentTick = (clock() - previousClock) * 1000 / CLOCKS_PER_SEC;
}

inline long getDifferentTick() {
	return differentTick;
}

template <typename Executer, typename Timing>
long executeBenchmarkHelp(const Executer & executer, int iterates, const Timing & t)
{
	Executer & e = const_cast<Executer &>(executer);

	beginClock();

	while(iterates > 0) {
		e.execute(iterates);

		--iterates;
	}

	endClock();

	t.timing(getDifferentTick());

	return getDifferentTick();
}

const unsigned int Iterate_1M = 1000 * 1000;
const unsigned int Iterate_10M = 10 * Iterate_1M;
const unsigned int Iterate_100M = 100 * Iterate_1M;
const unsigned int Iterate_1B = 1000 * Iterate_1M;

const unsigned int Iterates_Generations = 1;
const unsigned int Iterates_Slot = Iterate_100M;
const unsigned int Iterates_Signal = Iterate_10M;

const int Feature_InlineMember = 0;
const int Feature_NoninlineMember = 1;
const int Feature_VirtualMember = 2;
const int Feature_Global = 3;
const int Feature_Functor = 4;
const int Feature_SignalEmpty = 5;
const int Feature_SignalFiveSlots = 6;
const int Feature_SignalTenSlots = 7;
const int Feature_SignalFiftySlots = 8;
const int Feature_Count = 9;

const int Library_RawFunction = 0;
const int Library_CppCallback = 1;
const int Library_LibSigC = 2;
const int Library_BoostSignal2 = 3;
const int Library_Count = 4;

const int Library_FastDelegate = -1;
const int Library_SRDelegate = -2;

const string featureNames[] = {
	"inline member", "noninline member", "virtual member", "global", "functor",
	"empty signal", "5 slots signal", "10 slots signal", "50 slots signal"
};

const string libraryNames[] = {
	"raw function", "cppcallback", "libsigc++", "boost::signal2"
};

const string unreportLibraryNames[] = {
	"", "fast delegate", "sr delegate"
};

class BenchmarkItem
{
public:
	BenchmarkItem(int lib, int feature) : lib(lib), feature(feature) {
	}

	BenchmarkItem(const BenchmarkItem & other) : lib(other.lib), feature(other.feature) {
	}

	BenchmarkItem & operator = (const BenchmarkItem & other) {
		this->feature = other.feature;
		this->lib = other.lib;

		return *this;
	}

	bool operator < (const BenchmarkItem & other) const {
		if(this->lib == other.lib) {
			return this->feature < other.feature;
		}
		else {
			return this->lib < other.lib;
		}
	}

	int getLib() const {
		return this->lib;
	}

	int getFeature() const {
		return this->feature;
	}

private:
	int lib;
	int feature;
};

typedef long long DurationType;

class BenchmarkTime
{
public:
	BenchmarkTime() : duration(0) {
	}

	void timing(DurationType t) const {
		this->duration += t;
	}

	void average(int count) const {
		this->duration /= count;
	}

	DurationType getDuration() const {
		return this->duration;
	}

private:
	mutable DurationType duration;
};

typedef GWiseMap<BenchmarkItem, BenchmarkTime> MapType;

MapType benchmarkMap;

const string & getLibName(int lib)
{
    if(lib < 0) {
        return unreportLibraryNames[-lib];
    }
    else {
        return libraryNames[lib];
    }
}

const string & getFeatureName(int feature)
{
	return featureNames[feature];
}

template <typename Executer>
void executeBenchmark(const Executer & executer, int iterates, int lib, int feature)
{
	cout << getLibName(lib) << " -- " << getFeatureName(feature)
		<< "\t\t\t"
		<< executeBenchmarkHelp(executer, iterates, benchmarkMap[BenchmarkItem(lib, feature)])
		<< endl;
}



#define DEF_EXECUTER_REF(NAME, OBJECT, EXECUTE) \
	struct NAME { \
		const OBJECT & obj; \
		NAME(const OBJECT & o) : obj(o) {} \
		void execute(int n) { EXECUTE; } \
	}

#define DEF_EXECUTER DEF_EXECUTER_REF

typedef GCallback<void (const ParamType &)> CallbackType1;
typedef GCallbackList<void (const ParamType &)> CallbackListType1;
//typedef GCallbackList<void (const ParamType &), GCallbackConnection, std::list> CallbackListType1;

#ifndef NO_LIBSIGC
	typedef sigc::slot1<void, const ParamType &> SigcType1;
	typedef sigc::signal<void, const ParamType &> SigcSignalType1;
#endif

#ifndef NO_BOOST
	#ifdef BOOST_SIGNAL
		typedef boost::signal<void (const ParamType &)> BoostSignalType1;
	#else
		typedef boost::signals2::slot<void (const ParamType &)> BoostSlotType1;
		typedef boost::signals2::signal<void (const ParamType &)> BoostSignalType1;
	#endif
#endif

#ifndef NO_FD
	typedef fastdelegate::FastDelegate1<const ParamType &> FastDelegateType1;
#endif

#ifndef NO_SR
	typedef srutil::delegate1<void, const ParamType &> SRDelegateType1;
#endif


int globalVar = 0;

void globalFunction1(const ParamType & n)
{
	globalVar += getValue(n);
}

void globalFunction2(const ParamType & n)
{
	globalVar += getValue(n);
}

void BenchmarkTestObject::noninlinedMember1(const ParamType & n) const
{
	globalVar += getValue(n);
}

void BenchmarkTestObject::noninlinedMember2(const ParamType & n) const
{
	globalVar += getValue(n);
}


DEF_EXECUTER(Execute_Raw_InlineMember1, BenchmarkTestObject, obj.inlinedMember1(n) );
DEF_EXECUTER(Execute_Raw_NoninlineMember1, BenchmarkTestObject, obj.noninlinedMember1(n) );
DEF_EXECUTER(Execute_Raw_VirtualMember1, BenchmarkTestObject, obj.virtualMember1(n) );
DEF_EXECUTER(Execute_Raw_Global1, BenchmarkTestObject, globalFunction1(n) );
DEF_EXECUTER(Execute_Raw_Functor1, BenchmarkTestObject, obj(n) );

DEF_EXECUTER(Execute_GCallback1, CallbackType1, obj(n) );
DEF_EXECUTER(Execute_GCallbackList1, CallbackListType1, obj(n) );

#ifndef NO_FD
	DEF_EXECUTER(Execute_FastDelegate, FastDelegateType1, obj(n) );
#endif

#ifndef NO_SR
	DEF_EXECUTER(Execute_SRDelegate, SRDelegateType1, obj(n) );
#endif

#ifndef NO_LIBSIGC
	DEF_EXECUTER(Execute_Sigc1, SigcType1, obj(n) );
	DEF_EXECUTER(Execute_SigcSignal1, SigcSignalType1, obj(n) );
#endif

#ifndef NO_BOOST
	DEF_EXECUTER(Execute_BoostSignal1, BoostSignalType1, obj(n) );
	#ifndef BOOST_SIGNAL
		DEF_EXECUTER(Execute_BoostSlot1, BoostSlotType1, obj(n) );
	#endif
#endif


void runBenchmarkSlot()
{
	BenchmarkTestObjectExtend bto;

	const int IterateCount = Iterates_Slot;


	executeBenchmark(Execute_Raw_InlineMember1(bto), IterateCount, Library_RawFunction, Feature_InlineMember);
	executeBenchmark(Execute_Raw_NoninlineMember1(bto), IterateCount, Library_RawFunction, Feature_NoninlineMember);
	executeBenchmark(Execute_Raw_VirtualMember1(bto), IterateCount, Library_RawFunction, Feature_VirtualMember);
	executeBenchmark(Execute_Raw_Global1(bto), IterateCount, Library_RawFunction, Feature_Global);
	executeBenchmark(Execute_Raw_Functor1(bto), IterateCount, Library_RawFunction, Feature_Functor);

	executeBenchmark(Execute_GCallback1(makeCallback(&bto, &BenchmarkTestObjectExtend::inlinedMember1)), IterateCount, Library_CppCallback, Feature_InlineMember);
	executeBenchmark(Execute_GCallback1(makeCallback(&bto, &BenchmarkTestObjectExtend::noninlinedMember1)), IterateCount, Library_CppCallback, Feature_NoninlineMember);
	executeBenchmark(Execute_GCallback1(makeCallback(&bto, &BenchmarkTestObjectExtend::virtualMember1)), IterateCount, Library_CppCallback, Feature_VirtualMember);
	executeBenchmark(Execute_GCallback1(makeCallback(&globalFunction1)), IterateCount, Library_CppCallback, Feature_Global);
	executeBenchmark(Execute_GCallback1(BenchmarkTestObject()), IterateCount, Library_CppCallback, Feature_Functor);

#ifndef NO_LIBSIGC
	executeBenchmark(Execute_Sigc1(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::inlinedMember1)), IterateCount, Library_LibSigC, Feature_InlineMember);
	executeBenchmark(Execute_Sigc1(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::noninlinedMember1)), IterateCount, Library_LibSigC, Feature_NoninlineMember);
	executeBenchmark(Execute_Sigc1(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::virtualMember1)), IterateCount, Library_LibSigC, Feature_VirtualMember);
	executeBenchmark(Execute_Sigc1(sigc::ptr_fun(&globalFunction1)), IterateCount, Library_LibSigC, Feature_Global);
	executeBenchmark(Execute_Sigc1(BenchmarkTestObject()), IterateCount, Library_LibSigC, Feature_Functor);
#endif

#ifndef NO_BOOST
#ifndef BOOST_SIGNAL
	executeBenchmark(Execute_BoostSlot1(boost::bind(&BenchmarkTestObjectExtend::inlinedMember1, &bto, _1)), IterateCount, Library_BoostSignal2, Feature_InlineMember);
	executeBenchmark(Execute_BoostSlot1(boost::bind(&BenchmarkTestObjectExtend::noninlinedMember1, &bto, _1)), IterateCount, Library_BoostSignal2, Feature_NoninlineMember);
	executeBenchmark(Execute_BoostSlot1(boost::bind(&BenchmarkTestObjectExtend::virtualMember1, &bto, _1)), IterateCount, Library_BoostSignal2, Feature_VirtualMember);
	executeBenchmark(Execute_BoostSlot1(&globalFunction1), IterateCount, Library_BoostSignal2, Feature_Global);
	executeBenchmark(Execute_BoostSlot1(BenchmarkTestObject()), IterateCount, Library_BoostSignal2, Feature_Functor);
#endif
#endif

#ifndef NO_FD
	executeBenchmark(Execute_FastDelegate(fastdelegate::MakeDelegate(&bto, &BenchmarkTestObjectExtend::inlinedMember1)), IterateCount, Library_FastDelegate, Feature_InlineMember);
	executeBenchmark(Execute_FastDelegate(fastdelegate::MakeDelegate(&bto, &BenchmarkTestObjectExtend::noninlinedMember1)), IterateCount, Library_FastDelegate, Feature_NoninlineMember);
	executeBenchmark(Execute_FastDelegate(fastdelegate::MakeDelegate(&bto, &BenchmarkTestObjectExtend::virtualMember1)), IterateCount, Library_FastDelegate, Feature_VirtualMember);
#endif

#ifndef NO_SR
	executeBenchmark(Execute_SRDelegate(SRDelegateType1::from_const_method<BenchmarkTestObject, &BenchmarkTestObjectExtend::inlinedMember1>(&bto)), IterateCount, Library_SRDelegate, Feature_InlineMember);
	executeBenchmark(Execute_SRDelegate(SRDelegateType1::from_const_method<BenchmarkTestObject, &BenchmarkTestObjectExtend::noninlinedMember1>(&bto)), IterateCount, Library_SRDelegate, Feature_NoninlineMember);
	executeBenchmark(Execute_SRDelegate(SRDelegateType1::from_const_method<BenchmarkTestObject, &BenchmarkTestObject::virtualMember1>(&bto)), IterateCount, Library_SRDelegate, Feature_VirtualMember);
	executeBenchmark(Execute_SRDelegate(SRDelegateType1::from_function<&globalFunction1>()), IterateCount, Library_SRDelegate, Feature_Global);
#endif
}

void runBenchmarkSignal()
{
	BenchmarkTestObjectExtend bto;

	const int IterateCount = Iterates_Signal;

	CallbackListType1 callbackList1;

#ifndef NO_LIBSIGC
	SigcSignalType1 sigcSignal1;
#endif

#ifndef NO_BOOST
	BoostSignalType1 boostSignal1;
#endif

	executeBenchmark(Execute_GCallbackList1(callbackList1), IterateCount, Library_CppCallback, Feature_SignalEmpty);

#ifndef NO_LIBSIGC
	executeBenchmark(Execute_SigcSignal1(sigcSignal1), IterateCount, Library_LibSigC, Feature_SignalEmpty);
#endif

#ifndef NO_BOOST
	executeBenchmark(Execute_BoostSignal1(boostSignal1), IterateCount, Library_BoostSignal2, Feature_SignalEmpty);
#endif

	callbackList1.add(&bto, &BenchmarkTestObjectExtend::inlinedMember1);
	callbackList1.add(&bto, &BenchmarkTestObjectExtend::noninlinedMember1);
	callbackList1.add(&bto, &BenchmarkTestObjectExtend::virtualMember1);
	callbackList1.add(&globalFunction1);
	callbackList1.add(BenchmarkTestObject());
	executeBenchmark(Execute_GCallbackList1(callbackList1), IterateCount, Library_CppCallback, Feature_SignalFiveSlots);

	callbackList1.add(&bto, &BenchmarkTestObjectExtend::inlinedMember2);
	callbackList1.add(&bto, &BenchmarkTestObjectExtend::noninlinedMember2);
	callbackList1.add(&bto, &BenchmarkTestObjectExtend::virtualMember2);
	callbackList1.add(&globalFunction2);
	callbackList1.add(BenchmarkTestObject2());
	executeBenchmark(Execute_GCallbackList1(callbackList1), IterateCount, Library_CppCallback, Feature_SignalTenSlots);

	for(int n = 0; n < 4; ++n) {
		callbackList1.add(&bto, &BenchmarkTestObjectExtend::inlinedMember1);
		callbackList1.add(&bto, &BenchmarkTestObjectExtend::noninlinedMember1);
		callbackList1.add(&bto, &BenchmarkTestObjectExtend::virtualMember1);
		callbackList1.add(&globalFunction1);
		callbackList1.add(BenchmarkTestObject());
		callbackList1.add(&bto, &BenchmarkTestObjectExtend::inlinedMember2);
		callbackList1.add(&bto, &BenchmarkTestObjectExtend::noninlinedMember2);
		callbackList1.add(&bto, &BenchmarkTestObjectExtend::virtualMember2);
		callbackList1.add(&globalFunction2);
		callbackList1.add(BenchmarkTestObject2());
	}
	executeBenchmark(Execute_GCallbackList1(callbackList1), IterateCount, Library_CppCallback, Feature_SignalFiftySlots);

#ifndef NO_LIBSIGC
	sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::inlinedMember1));
	sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::noninlinedMember1));
	sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::virtualMember1));
	sigcSignal1.connect(sigc::ptr_fun(&globalFunction1));
	sigcSignal1.connect(BenchmarkTestObject());
	executeBenchmark(Execute_SigcSignal1(sigcSignal1), IterateCount, Library_LibSigC, Feature_SignalFiveSlots);

	sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::inlinedMember2));
	sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::noninlinedMember2));
	sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::virtualMember2));
	sigcSignal1.connect(sigc::ptr_fun(&globalFunction2));
	sigcSignal1.connect(BenchmarkTestObject2());
	executeBenchmark(Execute_SigcSignal1(sigcSignal1), IterateCount, Library_LibSigC, Feature_SignalTenSlots);

	for(int n = 0; n < 4; ++n) {
		sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::inlinedMember1));
		sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::noninlinedMember1));
		sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::virtualMember1));
		sigcSignal1.connect(sigc::ptr_fun(&globalFunction1));
		sigcSignal1.connect(BenchmarkTestObject());
		sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::inlinedMember2));
		sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::noninlinedMember2));
		sigcSignal1.connect(sigc::mem_fun(&bto, &BenchmarkTestObjectExtend::virtualMember2));
		sigcSignal1.connect(sigc::ptr_fun(&globalFunction2));
		sigcSignal1.connect(BenchmarkTestObject2());
	}
	executeBenchmark(Execute_SigcSignal1(sigcSignal1), IterateCount, Library_LibSigC, Feature_SignalFiftySlots);
#endif

#ifndef NO_BOOST
	boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::inlinedMember1, &bto, _1));
	boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::noninlinedMember1, &bto, _1));
	boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::virtualMember1, &bto, _1));
	boostSignal1.connect(&globalFunction1);
	boostSignal1.connect(BenchmarkTestObject());
	executeBenchmark(Execute_BoostSignal1(boostSignal1), IterateCount, Library_BoostSignal2, Feature_SignalFiveSlots);

	boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::inlinedMember2, &bto, _1));
	boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::noninlinedMember2, &bto, _1));
	boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::virtualMember2, &bto, _1));
	boostSignal1.connect(&globalFunction2);
	boostSignal1.connect(BenchmarkTestObject2());
	executeBenchmark(Execute_BoostSignal1(boostSignal1), IterateCount, Library_BoostSignal2, Feature_SignalTenSlots);

	for(int n = 0; n < 4; ++n) {
		boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::inlinedMember1, &bto, _1));
		boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::noninlinedMember1, &bto, _1));
		boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::virtualMember1, &bto, _1));
		boostSignal1.connect(&globalFunction1);
		boostSignal1.connect(BenchmarkTestObject());
		boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::inlinedMember2, &bto, _1));
		boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::noninlinedMember2, &bto, _1));
		boostSignal1.connect(boost::bind(&BenchmarkTestObjectExtend::virtualMember2, &bto, _1));
		boostSignal1.connect(&globalFunction2);
		boostSignal1.connect(BenchmarkTestObject2());
	}
	executeBenchmark(Execute_BoostSignal1(boostSignal1), IterateCount, Library_BoostSignal2, Feature_SignalFiftySlots);
#endif
}

bool outputResultByLibraryAndFeature(int lib, int feature, bool html)
{
	bool found = false;

	for(MapType::iterator it = benchmarkMap.begin(); it != benchmarkMap.end(); ++it) {
		if(it->first.getLib() == lib && it->first.getFeature() == feature) {
			found = true;

			if(html) {
				cout << "<td>" << it->second.getDuration() << "</td>" << endl;
			}
			else {
				cout << getLibName(lib) << "\t\t";
				cout << it->second.getDuration() << endl;
			}
		}
	}

	if(!found) {
		if(html) {
			cout << "<td>N/A</td>" << endl;
		}
		else {
			cout << getLibName(lib) << "\t\t" << "N/A" << endl;
		}
	}

	return found;
}

bool outputResultByFeature(int feature)
{
	bool found = false;

	cout << getFeatureName(feature) << endl;

	for(int i = 0; i < Library_Count; i++) {
		found = outputResultByLibraryAndFeature(i, feature, false) || found;
	}

	for(int i = 0; i < Library_Count; i++) {
		outputResultByLibraryAndFeature(i, feature, true);
	}

	return found;
}

void outputResult()
{
	cout << endl << endl;
	cout << "Report:" << endl;
	cout << endl;

	for(int i = 0; i < Feature_Count; i++) {
		if(outputResultByFeature(i)) {
			cout << endl;
		}
	}

	DurationType total = 0;
	for(MapType::iterator it = benchmarkMap.begin(); it != benchmarkMap.end(); ++it) {
		total += it->second.getDuration();
	}
	cout << endl << "Total time: " << total << endl;
}

void runBenchmarkCallback()
{
	for(unsigned int i = 0; i < Iterates_Generations; i++) {
		cout << "Generation: " << i << endl;

		runBenchmarkSlot();
		runBenchmarkSignal();

		cout << endl;
	}

	if(Iterates_Generations > 1) {
		for(MapType::iterator it = benchmarkMap.begin(); it != benchmarkMap.end(); ++it) {
			it->second.average(Iterates_Generations);
		}
	}

	outputResult();
}


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

