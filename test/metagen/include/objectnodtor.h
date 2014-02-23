#ifndef CPGF_OBJECTNODTOR_H
#define CPGF_OBJECTNODTOR_H


class ObjectNoDestructor
{
public:
	ObjectNoDestructor() {
	}
	
	static void free(ObjectNoDestructor * obj) {
		delete obj;
	}
	
private:
	~ObjectNoDestructor() {}
};



#endif
