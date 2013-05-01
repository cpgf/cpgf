#include "wx/app.h"

#include "ui/mainframe.h"


class TweenApp: public wxApp
{
    virtual bool OnInit();
};

bool TweenApp::OnInit()
{
    MainFrame *frame = new MainFrame;
    frame->Show(true);
    SetTopWindow(frame);
    return true;
}

IMPLEMENT_APP(TweenApp)
