#include "benchmark.h"

#include <chrono>
#include <sstream>
#include <iostream>

int64_t getNanoseconds()
{
	static auto start = std::chrono::high_resolution_clock::now();
	const auto now = std::chrono::high_resolution_clock::now();
	const std::chrono::nanoseconds diff = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start);
	return (uint64_t)diff.count();
}

std::string formatNanoseconds(const int64_t nanoseconds)
{
	const int64_t converted = nanoseconds / (1000LL * 1000LL);

	std::ostringstream stream;
	stream << converted << " ms";

	return stream.str();
}

BenchmarkTimer::BenchmarkTimer()
	: BenchmarkTimer("")
{
}

BenchmarkTimer::BenchmarkTimer(const std::string & message)
	:
		startNanoseconds(getNanoseconds()),
		endNanoseconds(getNanoseconds()),
		message(message)
{
}

BenchmarkTimer::~BenchmarkTimer()
{
	this->endNanoseconds = getNanoseconds();

	using namespace std;
	cout << this->message << " Time: " << formatNanoseconds(this->endNanoseconds - this->startNanoseconds) << endl;
}

