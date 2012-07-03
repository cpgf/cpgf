#include <iostream>

using namespace std;

#define EXEC(n) void run_ ## n(); cout << endl << "***** Tutorial " << # n << endl; run_ ## n();


int main()
{
	EXEC(a01);
	EXEC(a02);
	EXEC(a03);

	return 0;
}
