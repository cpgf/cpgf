#ifndef CPGF_GCLASSUTIL_H
#define CPGF_GCLASSUTIL_H


#define GMAKE_NONCOPYABLE(cls) private: \
	cls(const cls &); \
	cls & operator = (const cls &);

#define GMAKE_FINAL(cls)  \
	class MakeFinal_ ## cls { \
		private: \
			~MakeFinal_ ## cls() {} \
			friend class cls; \
	}; 

#define GFINAL_BASE(cls) virtual public MakeFinal_ ## cls


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
