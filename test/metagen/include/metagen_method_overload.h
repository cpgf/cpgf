#ifndef CPGF_METAGEN_METHOD_OVERLOAD_H
#define CPGF_METAGEN_METHOD_OVERLOAD_H


class MetagenMethodOverload
{
public:
	MetagenMethodOverload() : value(0) {}

	const MetagenMethodOverload * makeConst() {
		return this;
	}

	int testOverloadByConst_ConstFirst() const {
		return 1;
	}

	int testOverloadByConst_ConstFirst() {
		return 2;
	}

	int testOverloadByConst_NonConstFirst() {
		return 3;
	}

	int testOverloadByConst_NonConstFirst() const {
		return 4;
	}

public:
	int value;
};



#endif
