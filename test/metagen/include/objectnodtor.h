#ifndef __OBJECTNODTOR_H
#define __OBJECTNODTOR_H


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
