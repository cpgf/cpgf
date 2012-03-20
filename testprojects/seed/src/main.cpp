#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include <iostream>
#include <string>

#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/metatraits/gmetaserializer_string.h"
#include "cpgf/gmetaextendtype.h"

#include "unittestbase.h"

using namespace std;
using namespace cpgf;

void testSer();

void testBoost();

namespace cpgf {
namespace ttt {

template <typename T>
void test(T &)
{
	GMetaExtendType type;
	type = createMetaExtendType<T>(GExtendTypeCreateFlag_Converter | GExtendTypeCreateFlag_Serializer);
	cout << "Array size: " << type.getArraySize() << endl;
	cout << "Converter: " << type.getConverter() << endl;
	cout << "Serializer: " << type.getSerializer() << endl;
}

}
}


using namespace cpgf;
using namespace cpgf::ttt;

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;

//	testBoost();
	string s;
	test(s);

	UnitTest::RunAllTests();
	testSer();

//	_getch();
	printf("\n");

	return 0;
}

