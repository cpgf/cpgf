#include "panelcanvas.h"

#include "wx/dcbuffer.h"
#include "wx/glcanvas.h"
#include "wx/sizer.h"

#include "GL/glut.h"


PanelCanvas::PanelCanvas(wxWindow * parent)
	: super(parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, wxGLCanvasName, wxNullPalette)
{
//	this->SetBackgroundStyle(wxBG_STYLE_PAINT);

	this->context = new wxGLContext(this);

	this->connectPanelEvents();
}

PanelCanvas::~PanelCanvas()
{
	delete this->context;
	this->disconnectPanelEvents();
}

void PanelCanvas::addRender(const CallbackType & render)
{
	this->renderList.add(render);
}

void PanelCanvas::connectPanelEvents()
{
	this->Bind(wxEVT_PAINT, &PanelCanvas::onPaint, this);
	this->Bind(wxEVT_ERASE_BACKGROUND, &PanelCanvas::onEraseBackground, this);
}

void PanelCanvas::disconnectPanelEvents()
{
	this->Unbind(wxEVT_PAINT, &PanelCanvas::onPaint, this);
	this->Unbind(wxEVT_ERASE_BACKGROUND, &PanelCanvas::onEraseBackground, this);
}

void PanelCanvas::onPaint(wxPaintEvent & e)
{
	this->doPaint();
}

void PanelCanvas::onEraseBackground(wxEraseEvent & e)
{
	e.Skip();
}

namespace {
	bool initedGL = false;
}

void initGL()
{
	char * s = (char *)"\0";
	int n = 0;
	glutInit(&n, &s);

	static const GLfloat light0_ambient[4]   = { 0.6f, 0.6f, 0.6f, 1.0f };
    static const GLfloat light0_diffuse[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	static const GLfloat light0_specular[4]   = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat light0_pos[4]   = { 1.0f, 1.0f, 0.0f, 1.0f };

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_DITHER);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnableClientState(GL_VERTEX_ARRAY);
}

void checkGL()
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		error = error;
	}
}

void PanelCanvas::doPaint()
{
    wxPaintDC(this);
    SetCurrent(*context);

	if(! initedGL) {
		initedGL = true;
		initGL();
	}

    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	int viewWidth;
	int viewHeight;
	this->GetSize(&viewWidth, &viewHeight);
	if(viewWidth > viewHeight) {
		viewWidth = viewHeight;
	}
	else {
		viewHeight = viewWidth;
	}
    glViewport(0, 0, viewWidth, viewHeight);
	glPushMatrix();
	glTranslatef(-1.0f, -1.0f, 0);

	this->renderList.dispatch(viewWidth, viewHeight);

	glPopMatrix();

    glFlush();
    SwapBuffers();
}
