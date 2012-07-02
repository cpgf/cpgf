#include <iostream>

using namespace std;

#define EXEC(n) cout << endl << "***** Tutorial " << # n << endl; run_ ## n();


void run_a01();

int main()
{
	EXEC(a01);

	return 0;
}
