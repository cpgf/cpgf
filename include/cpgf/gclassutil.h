#ifndef CPGF_GCLASSUTIL_H
#define CPGF_GCLASSUTIL_H


#define GMAKE_NONCOPYABLE(cls) private: \
	cls(const cls &); \
	cls & operator = (const cls &);


namespace cpgf {

class GNoncopyable
{
public:
	GNoncopyable() {}

private:
	GNoncopyable(const GNoncopyable &);
	GNoncopyable & operator = (const GNoncopyable &);
};

} // namespace cpgf


#endif
