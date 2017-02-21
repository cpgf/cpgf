#ifndef CPGF_METAGEN_TEMPLATE_CLASSES_H
#define CPGF_METAGEN_TEMPLATE_CLASSES_H


template <typename T, typename U, int N>
struct TemplateClassA
{
public:
	template <typename I>
	struct TemplateClassInnerA
	{
	};

	struct Shared
	{
	};

//	void a(TemplateClassInnerA<T> *) {}
	void a(int) {}
	void b(Shared *) {}
	void b(void *) {}
};

struct TemplateBase
{
};

namespace NS1 {
namespace NS2 {

template <typename T>
struct TemplateClassB : public T
{
public:
	struct Shared
	{
	};

	void b(Shared *) {}
	void b(void *) {}
};

}
}


#endif

