#include "cpgf/game/gcore.h"
#include "cpgf/game/gdebug.h"

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include<string>


#if defined(G_OS_WIN)
#include <windows.h>
#endif

#if defined(G_OPENGLES)
	#include <OpenGLES/ES1/gl.h>
#else
//	#include <gl/gl.h>
#endif


using namespace std;

namespace cpgf {


//#ifdef G_DEBUG


namespace _internal {

#if defined(G_OS_WIN)

	void DebugOutput(const char *message)
	{
		OutputDebugStringA(message);
	}
#else
	void DebugOutput(const char *message)
	{
	}
	
	
#endif

void DebugTrace(const char * sourceFile, int lineNumber, const char * message, ...)
{
	if(message == NULL) {
		return;
	}

	char buffer[4096];

	va_list args;
	va_start(args, message);
	vsprintf(buffer, message, args);

	int len = strlen(buffer);
	buffer[len++] = '\n';
	buffer[len++] = 0;
	DebugOutput(buffer);

	va_end (args);
}

bool DebugAssert(const char * sourceFile, int lineNumber, const char * expression, const char * message, ...)
{
	char buffer[4096];
	char buffer2[4096];

	va_list args;
	va_start(args, message);
	vsprintf(buffer, message, args);

	sprintf(buffer2, "exp = %s message = %s\n", expression, buffer);
	DebugOutput(buffer2);

	va_end (args);

	assert(false);

	return true;
}

string DebugFormatMatrixColumnMajor(float * matrix)
{
	string result;
	char buffer[200];

	for(int i = 0; i < 4; ++i) {
		for(int k = 0; k < 4; ++k) {
			sprintf(buffer, "%4.2f ", matrix[k * 4 + i]);
			result = result + buffer;
		}
		result = result + "\n";
	}

	return result;
}

void DebugOutputMatrixColumnMajor(float * matrix)
{
	string s = DebugFormatMatrixColumnMajor(matrix);
	DebugOutput(s.c_str());
}

void DebugCheckGL()
{
//	int e = glGetError();
//	debugAssert(e == GL_NO_ERROR, "OpenGL Error %X", e);
}


}

//#endif


} // namespace cpgf

