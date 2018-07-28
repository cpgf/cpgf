#ifndef CPGF_GPYTHONBIND_H
#define CPGF_GPYTHONBIND_H


#include "cpgf/scriptbind/gscriptbind.h"

#include "Python.h"

#include <memory>

namespace cpgf {

GScriptObject * createPythonScriptObject(IMetaService * service, PyObject * object);
IScriptObject * createPythonScriptInterface(IMetaService * service, PyObject * object);

template <typename T>
struct GPythonScopedPointerDeleter
{
	inline void operator() (T * p) const {
		Py_XDECREF(p);
	}
};

typedef std::unique_ptr<PyObject, GPythonScopedPointerDeleter<PyObject> > GPythonScopedPointer;



} // namespace cpgf




#endif
