#include <string>

using namespace std;

#define MYDEF 1
#define MYDEF2
#define MYFUNC(n) n
#define MYFUNC2() 5

namespace NNN {

namespace MMM {

struct CLASS {};

class BaseA
{
public:
	int ddd();
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
	virtual ~MyClass();

	virtual int abc(const int * volatile p, int (MyClass::*xxx)(int, double), int def = 0x38);
	void overloadFunc(int n);
	int overloadFunc(void * p);

	bool operator <= (const MyClass & other);
	int operator [] (int n) const;
	operator std::string () const;
	CLASS operator ++ ();
	CLASS operator ++ (int);
	CLASS operator -- ();
	CLASS operator -- (int);
	CLASS operator ! () const;
	CLASS operator ~ () const;
	
	template <typename T>
	void templateFunc(T * a);

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
	enum {DYY = 5, DPP = 6};

	typedef ABC DEF;
};

MyClass * myGlobalFunc(int nnn = 5);


}


NNN::MyClass * myGlobalFuncNot();
