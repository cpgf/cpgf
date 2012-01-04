#include <stdio.h>
//#include <tchar.h>
//#include <conio.h>

#include "cpgf/metadata/stl/gmetadata_vector.h"
#include "cpgf/gmetadefine.h"

#include <vector>

using namespace cpgf;
using namespace std;

typedef vector<int> VectorType;

int main(int argc, char * argv[])
{
	GDefineMetaClass<VectorType> define = GDefineMetaClass<VectorType>::declare("vector_int");
	bindMetaData_vector(define);

//	_getch();

	return 0;
}

