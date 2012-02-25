#include "cpgf/gcompiler.h"
#include "cpgf/gmetadefine.h"

#if defined(_WIN32)
    #include <windows.h>
#endif

#ifdef G_OS_APPLE
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include "cpgf/metadata/opengl/gmetadata_opengl.h"

using namespace cpgf;


void registerOpenGL(GMetaClass * metaClass)
{
	GDefineMetaClass<void> define = GDefineMetaClass<void>::fromMetaClass(metaClass);

	buildMetaData_open_gl(define);
}

