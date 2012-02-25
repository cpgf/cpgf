#include "cpgf/gcompiler.h"
#include "cpgf/gmetadefine.h"

#if defined(_WIN32)
    #include <windows.h>
#endif

#ifdef G_OS_APPLE
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif

#include "cpgf/metadata/opengl/gmetadata_openglu.h"

using namespace cpgf;


void registerOpenGLU(GMetaClass * metaClass)
{
	GDefineMetaClass<void> define = GDefineMetaClass<void>::fromMetaClass(metaClass);

	buildMetaData_open_glu(define);
}

