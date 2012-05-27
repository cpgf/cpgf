#ifndef __GPYTHONBIND_H
#define __GPYTHONBIND_H


#include "cpgf/scriptbind/gscriptbind.h"

#include "python.h"


namespace cpgf {

GScriptObject * createPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config);
IScriptObject * createPythonScriptInterface(IMetaService * service, PyObject * object, const GScriptConfig & config);


} // namespace cpgf




#endif
