
#define MYDEF 1

namespace NNN {

namespace MMM {

class BaseA
{
};

}

using namespace MMM;

class BaseB
{
};

class MyClass : public BaseA, private BaseB
{
public:
	explicit MyClass(int n);

	int abc(const int * volatile p, int (MyClass::*xxx)(int, double), int def = 0x38);

	bool operator <= (const MyClass & other);

private:
	MyClass * imPrivate;

public:
	class InnerClass
	{
	public:
		int value;
	};

	enum ABC { a=1, b};
	enum {YY = 5, PP = 6};

	typedef ABC DEF;
};

MyClass * myGlobalFunc(int nnn = 5);


}


MyClass * myGlobalFuncNot();
