#ifndef CPGF_METAGEN_METHOD_OVERLOAD_BY_FUNDAMENTAL_H
#define CPGF_METAGEN_METHOD_OVERLOAD_BY_FUNDAMENTAL_H

#include <string>

class MetagenMethodOverloadByFundamental
{
public:
	std::string overload_Boolean_Int(bool value) {
		return value ? "true" : "false";
	}
	
	std::string overload_Boolean_Int(int) {
		return "int";
	}

	
	std::string overload_Boolean_Real(bool value) {
		return value ? "true" : "false";
	}
	
	std::string overload_Boolean_Real(double) {
		return "real";
	}

	
	std::string overload_Boolean_Int_Real(bool value) {
		return value ? "true" : "false";
	}
	
	std::string overload_Boolean_Int_Real(int) {
		return "int";
	}

	std::string overload_Boolean_Int_Real(double) {
		return "real";
	}

};

inline std::string global_overload_Boolean_Int(bool value)
{
	return value ? "true" : "false";
}

inline std::string global_overload_Boolean_Int(int)
{
	return "int";
}

#endif
