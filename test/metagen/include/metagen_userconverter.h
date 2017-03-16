#ifndef CPGF_METAGEN_USERCONVERTER_H
#define CPGF_METAGEN_USERCONVERTER_H


class TestUserConverterData
{
public:
	TestUserConverterData() {}

	// Must make it explicit, otherwise, GVariant can convert int to TestUserConverterData automatically,
	// then "QERR(mtest.testUserConverterGetDoubleValue(5))" won't pass.
	explicit TestUserConverterData(int n) : value(n) {
	}

public:
	int value;
};

inline int testUserConverterGetDoubleValue(const TestUserConverterData & data)
{
	return data.value * 2;
}


#endif
