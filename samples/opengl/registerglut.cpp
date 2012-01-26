#define GLUT_DISABLE_ATEXIT_HACK

#include <windows.h>
#include <glut.h>

#include "cpgf/metadata/opengl/gmetadata_openglut.h"

#include "cpgf/gmetadefine.h"


using namespace cpgf;


void registerOpenGLUT(GMetaClass * metaClass)
{
	GDefineMetaClass<void> define = GDefineMetaClass<void>::fromMetaClass(metaClass);

	buildMetaData_open_glut(define);
}

