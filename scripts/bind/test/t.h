
#define MYDEF 1
#define MYDEF2
#define MYFUNC(n) n
#define MYFUNC2() 5

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

template <typename MMM, typename EEE = void>
class MyClass : public BaseA, private BaseB
{
public:
	explicit MyClass(int n);

	virtual int abc(const int * volatile p, int (MyClass::*xxx)(int, double), int def = 0x38);
	void overloadFunc(int n);
	int overloadFunc(void * p);

	bool operator <= (const MyClass & other);

	template <typename TT, int NN>
	void tempFunc(TT * aa);

	MyClass * imPublic;
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
