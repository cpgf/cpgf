#ifndef CPGF_METAGEN_USERCONVERTER_H
#define CPGF_METAGEN_USERCONVERTER_H


class TestUserConverterData
{
public:
	TestUserConverterData() {}

	TestUserConverterData(int n) : value(n) {
	}

public:
	int value;
};

inline int testUserConverterGetDoubleValue(const TestUserConverterData & data)
{
	return data.value * 2;
}


#endif
