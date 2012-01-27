#if defined(_WIN32)
	#include <windows.h>
#endif
#include <gl/gl.h>

#include "cpgf/metadata/opengl/gmetadata_opengl.h"

#include "cpgf/gmetadefine.h"

using namespace cpgf;


void registerOpenGL(GMetaClass * metaClass)
{
	GDefineMetaClass<void> define = GDefineMetaClass<void>::fromMetaClass(metaClass);

	buildMetaData_open_gl(define);
}

