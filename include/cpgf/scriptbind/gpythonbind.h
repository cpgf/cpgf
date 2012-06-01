#ifndef __GPYTHONBIND_H
#define __GPYTHONBIND_H


#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gscopedptr.h"

#include "Python.h"


namespace cpgf {

GScriptObject * createPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config);
IScriptObject * createPythonScriptInterface(IMetaService * service, PyObject * object, const GScriptConfig & config);


template <typename T>
struct GPythonScopedPointerDeleter
{
	static inline void Delete(T * p) {
		Py_XDECREF(p);
	}
};

typedef GScopedPointer<PyObject, GPythonScopedPointerDeleter<PyObject> > GPythonScopedPointer;



} // namespace cpgf




#endif
