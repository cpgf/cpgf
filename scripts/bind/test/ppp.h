
#define MYDEF_PPP 1
#define MYDEF2_PPP
#define MYFUNC_PPP(n) n
#define MYFUNC2_PPP() 5

namespace NNN {

namespace MMM {

class BaseA_PPP
{
};

}

using namespace MMM;

class BaseB_PPP
{
};

template <typename MMM, typename EEE = void>
class MyClass_PPP : public BaseA_PPP, private BaseB_PPP
{
public:
	explicit MyClass_PPP(int n);

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
	class InnerClass_PPP
	{
	public:
		int value;
	};

	enum ABC { a=1, b};
	enum {YY = 5, PP = 6};

	typedef ABC DEF;
};

MyClass * myGlobalFunc_PPP(int nnn = 5);


}


MyClass * myGlobalFuncNot_PPP();
