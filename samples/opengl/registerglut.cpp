#define GLUT_DISABLE_ATEXIT_HACK

#include "cpgf/gcompiler.h"
#include "cpgf/gmetadefine.h"

#if defined(_WIN32)
    #include <windows.h>
#endif

#ifdef G_OS_APPLE
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "cpgf/metadata/opengl/gmetadata_openglut.h"


using namespace cpgf;


void registerOpenGLUT(GMetaClass * metaClass)
{
	GDefineMetaClass<void> define = GDefineMetaClass<void>::fromMetaClass(metaClass);

	buildMetaData_open_glut_constants(define);
}

