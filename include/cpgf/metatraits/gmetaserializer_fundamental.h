#ifndef __GMETASERIALIZER_FUNDAMENTAL_H
#define __GMETASERIALIZER_FUNDAMENTAL_H

// Usually you don't need to include this file directly.
// It's primarily used by array traits.



namespace cpgf {

struct IMetaSerializer;

#define DEF(t) IMetaSerializer * metaTraitsCreateSerializer(const t &);

DEF(bool)
DEF(char)
DEF(wchar_t)
DEF(signed char)
DEF(unsigned char)
DEF(signed short)
DEF(unsigned short)
DEF(signed int)
DEF(unsigned int)
DEF(signed long)
DEF(unsigned long)
DEF(signed long long)
DEF(unsigned long long)
DEF(float)
DEF(double)
DEF(long double)


#undef DEF


} // namespace cpgf


#endif
