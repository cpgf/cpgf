#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <cstdint>
#include <string>

int64_t getNanoseconds();
std::string formatNanoseconds(const int64_t nanoseconds);

class BenchmarkTimer
{
public:
	BenchmarkTimer();
	explicit BenchmarkTimer(const std::string & message);
	~BenchmarkTimer();

private:
	int64_t startNanoseconds;
	int64_t endNanoseconds;

	std::string message;
};


#endif
