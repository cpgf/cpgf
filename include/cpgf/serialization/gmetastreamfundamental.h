#ifndef CPGF_GMETASTREAMFUNDAMENTAL_H
#define CPGF_GMETASTREAMFUNDAMENTAL_H

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
		case GVariantType::vtBool:
			stream << fromVariant<typename TypeMap<bool>::Result>(value);
			break;

		case GVariantType::vtChar:
			stream << fromVariant<typename TypeMap<char>::Result>(value);
			break;

		case GVariantType::vtWchar:
			stream << fromVariant<typename TypeMap<wchar_t>::Result>(value);
			break;

		case GVariantType::vtSignedChar:
			stream << fromVariant<typename TypeMap<signed char>::Result>(value);
			break;

		case GVariantType::vtUnsignedChar:
			stream << fromVariant<typename TypeMap<unsigned char>::Result>(value);
			break;

		case GVariantType::vtSignedShort:
			stream << fromVariant<typename TypeMap<signed short>::Result>(value);
			break;

		case GVariantType::vtUnsignedShort:
			stream << fromVariant<typename TypeMap<unsigned short>::Result>(value);
			break;

		case GVariantType::vtSignedInt:
			stream << fromVariant<typename TypeMap<signed int>::Result>(value);
			break;

		case GVariantType::vtUnsignedInt:
			stream << fromVariant<typename TypeMap<unsigned int>::Result>(value);
			break;

		case GVariantType::vtSignedLong:
			stream << fromVariant<typename TypeMap<signed long>::Result>(value);
			break;

		case GVariantType::vtUnsignedLong:
			stream << fromVariant<typename TypeMap<unsigned long>::Result>(value);
			break;

		case GVariantType::vtSignedLongLong:
			stream << fromVariant<typename TypeMap<signed long long>::Result>(value);
			break;

		case GVariantType::vtUnsignedLongLong:
			stream << fromVariant<typename TypeMap<unsigned long long>::Result>(value);
			break;

		case GVariantType::vtFloat:
			stream << fromVariant<typename TypeMap<float>::Result>(value);
			break;

		case GVariantType::vtDouble:
			stream << fromVariant<typename TypeMap<double>::Result>(value);
			break;

		case GVariantType::vtLongDouble:
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
		case GVariantType::vtBool: {
			typename TypeMap<bool>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtChar: {
			typename TypeMap<char>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtWchar: {
			typename TypeMap<wchar_t>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtSignedChar: {
			typename TypeMap<signed char>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtUnsignedChar: {
			typename TypeMap<unsigned char>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtSignedShort: {
			typename TypeMap<signed short>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtUnsignedShort: {
			typename TypeMap<unsigned short>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtSignedInt: {
			typename TypeMap<signed int>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtUnsignedInt: {
			typename TypeMap<unsigned int>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtSignedLong: {
			typename TypeMap<signed long>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtUnsignedLong: {
			typename TypeMap<unsigned long>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtSignedLongLong: {
			typename TypeMap<signed long long>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtUnsignedLongLong: {
			typename TypeMap<unsigned long long>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtFloat: {
			typename TypeMap<float>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtDouble: {
			typename TypeMap<double>::Result value;
			stream >> value;
			return value;
		}

		case GVariantType::vtLongDouble: {
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
