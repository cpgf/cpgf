#ifndef __METAGEN_RETURN_THIS_H
#define __METAGEN_RETURN_THIS_H


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

	MetagenReturnThis & selfRef() {
		return *this;
	}

	MetagenReturnThis * selfPointer() {
		return this;
	}

private:
	int magic;
};



#endif
