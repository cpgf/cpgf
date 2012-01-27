#if defined(_WIN32)
	#include <windows.h>
#endif
#include <gl/glu.h>

#include "cpgf/metadata/opengl/gmetadata_openglu.h"

#include "cpgf/gmetadefine.h"

using namespace cpgf;


void registerOpenGLU(GMetaClass * metaClass)
{
	GDefineMetaClass<void> define = GDefineMetaClass<void>::fromMetaClass(metaClass);

	buildMetaData_open_glu(define);
}

