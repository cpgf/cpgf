#ifndef __GMETASTREAMFUNDAMENTAL_H
#define __GMETASTREAMFUNDAMENTAL_H

#include "cpgf/gvariant.h"

#include <ostream>
#include <istream>


namespace cpgf {


namespace serialization_internal {

	typedef void (*FuncStreamWriteFundamental)(std::ostream & stream, const GVariant & value);
	typedef GVariant (*FuncStreamReadFundamental)(std::istream & stream, GVariantType vt);

} // namespace serialization_internal


template <template<typename> class TypeMap >
void streamWriteFundamental(std::ostream & stream, const GVariant & value)
{
	switch(value.getType()) {
		case vtBool:
			stream << fromVariant<typename TypeMap<bool>::Result>(value);
			break;

		case vtChar:
			stream << fromVariant<typename TypeMap<char>::Result>(value);
			break;

		case vtWchar:
			stream << fromVariant<typename TypeMap<wchar_t>::Result>(value);
			break;

		case vtSignedChar:
			stream << fromVariant<typename TypeMap<signed char>::Result>(value);
			break;

		case vtUnsignedChar:
			stream << fromVariant<typename TypeMap<unsigned char>::Result>(value);
			break;

		case vtSignedShort:
			stream << fromVariant<typename TypeMap<signed short>::Result>(value);
			break;

		case vtUnsignedShort:
			stream << fromVariant<typename TypeMap<unsigned short>::Result>(value);
			break;

		case vtSignedInt:
			stream << fromVariant<typename TypeMap<signed int>::Result>(value);
			break;

		case vtUnsignedInt:
			stream << fromVariant<typename TypeMap<unsigned int>::Result>(value);
			break;

		case vtSignedLong:
			stream << fromVariant<typename TypeMap<signed long>::Result>(value);
			break;

		case vtUnsignedLong:
			stream << fromVariant<typename TypeMap<unsigned long>::Result>(value);
			break;

		case vtSignedLongLong:
			stream << fromVariant<typename TypeMap<signed long long>::Result>(value);
			break;

		case vtUnsignedLongLong:
			stream << fromVariant<typename TypeMap<unsigned long long>::Result>(value);
			break;

		case vtFloat:
			stream << fromVariant<typename TypeMap<float>::Result>(value);
			break;

		case vtDouble:
			stream << fromVariant<typename TypeMap<double>::Result>(value);
			break;

		case vtLongDouble:
			stream << fromVariant<typename TypeMap<long double>::Result>(value);
			break;

		default:
			break;
	}
}

template <template<typename> class TypeMap >
GVariant streamReadFundamental(std::istream & stream, GVariantType vt)
{
	switch(vt) {
		case vtBool: {
			typename TypeMap<bool>::Result value;
			stream >> value;
			return value;
		}

		case vtChar: {
			typename TypeMap<char>::Result value;
			stream >> value;
			return value;
		}

		case vtWchar: {
			typename TypeMap<wchar_t>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedChar: {
			typename TypeMap<signed char>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedChar: {
			typename TypeMap<unsigned char>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedShort: {
			typename TypeMap<signed short>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedShort: {
			typename TypeMap<unsigned short>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedInt: {
			typename TypeMap<signed int>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedInt: {
			typename TypeMap<unsigned int>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedLong: {
			typename TypeMap<signed long>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedLong: {
			typename TypeMap<unsigned long>::Result value;
			stream >> value;
			return value;
		}

		case vtSignedLongLong: {
			typename TypeMap<signed long long>::Result value;
			stream >> value;
			return value;
		}

		case vtUnsignedLongLong: {
			typename TypeMap<unsigned long long>::Result value;
			stream >> value;
			return value;
		}

		case vtFloat: {
			typename TypeMap<float>::Result value;
			stream >> value;
			return value;
		}

		case vtDouble: {
			typename TypeMap<double>::Result value;
			stream >> value;
			return value;
		}

		case vtLongDouble: {
			typename TypeMap<long double>::Result value;
			stream >> value;
			return value;
		}

		default:
			return 0;
	}
}



} // namespace cpgf




#endif
