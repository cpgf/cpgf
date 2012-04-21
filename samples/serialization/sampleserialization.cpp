#include <iostream>

using namespace std;

void check(bool b)
{
	if(! b) {
		cout << "Check error!" << endl;
		throw "Something wrong!";
	}
}


void testSimpleValue();
void testObject();


int main(int /*argc*/, char * /*argv*/[])
{
	testSimpleValue();
	
	cout << endl;

	testObject();

	return 0;
}
