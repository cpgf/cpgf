#ifndef CPGF_PANELCANVAS_H
#define CPGF_PANELCANVAS_H

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "cpgf/gcallbacklist.h"

#include "wx/glcanvas.h"

#include <vector>

class PanelCanvas : public wxGLCanvas
{
private:
	typedef wxGLCanvas super;
	typedef cpgf::GCallback<void (int, int)> CallbackType;
	typedef cpgf::GCallbackList<void (int, int)> CallbackListType;

public:
	explicit PanelCanvas(wxWindow * parent);
	~PanelCanvas();
	
	void addRender(const CallbackType & render);

private:
	void connectPanelEvents();
	void disconnectPanelEvents();

	void onPaint(wxPaintEvent & e);
	void onEraseBackground(wxEraseEvent & e);
	
	void doPaint();

private:
	wxGLContext * context;
	CallbackListType renderList;
};


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
