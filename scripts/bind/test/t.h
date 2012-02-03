#include <string>

using namespace std;

#define MYDEF 1
#define MYDEF2
#define MYFUNC(n) n
#define MYFUNC2() 5

namespace NNN {

namespace MMM {

#define CLASS int

class BaseA
{
public:
	int ddd() { return 0; }
};

void globalInNested() {}

}

using namespace MMM;

class BaseB
{
};

class MyClass : public BaseA, private BaseB
{
public:
	MyClass() {};
	explicit MyClass(int n) {};
	virtual ~MyClass() {};

	virtual int abc(const int * volatile p, int (MyClass::*xxx)(int, double), int def = 0x38) { return 0; };
	void overloadFunc(int n) {};
	int overloadFunc(void * p) { return 0; };

	bool operator <= (const MyClass & other) { return true; };
	int operator [] (int n) const {return 0;};
	operator std::string () const {return "";};
	CLASS operator ++ () { return 0; }
	CLASS operator ++ (int) { return 0; }
	CLASS operator -- () { return 0; }
	CLASS operator -- (int) { return 0; }
	CLASS operator ! () const { return 0; }
	CLASS operator ~ () const { return 0; }
	
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

	typedef ABC DEF;
};

MyClass * myGlobalFunc(int nnn = 5) {return 0;}


}


NNN::MyClass * myGlobalFuncNot() {return 0;}
