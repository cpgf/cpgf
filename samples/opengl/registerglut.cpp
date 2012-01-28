#define GLUT_DISABLE_ATEXIT_HACK

#if defined(_WIN32)
	#include <windows.h>
#endif
#include <gl/glut.h>

#include "cpgf/metadata/opengl/gmetadata_openglut.h"

#include "cpgf/gmetadefine.h"


using namespace cpgf;


void registerOpenGLUT(GMetaClass * metaClass)
{
	GDefineMetaClass<void> define = GDefineMetaClass<void>::fromMetaClass(metaClass);

	buildMetaData_open_glut(define);
}

