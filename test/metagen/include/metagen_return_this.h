#ifndef CPGF_METAGEN_RETURN_THIS_H
#define CPGF_METAGEN_RETURN_THIS_H


class MetagenReturnThis
{
public:
	MetagenReturnThis() : magic(0x1b9df282) {}
	~MetagenReturnThis() {
		this->magic = 0;
	}

	bool check() const {
		return this->magic == 0x1b9df282;
	}

	const MetagenReturnThis & selfRef() const {
		return *this;
	}

	MetagenReturnThis * selfPointer() {
		return this;
	}

private:
	int magic;
};



#endif
