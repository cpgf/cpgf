#ifndef CPGF_PANELEASE_H
#define CPGF_PANELEASE_H

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include "cpgf/gcallbacklist.h"

#include "wx/panel.h"
#include "wx/sizer.h"
#include "wx/button.h"

class PanelEase : public wxPanel
{
private:
	typedef wxPanel super;
	typedef cpgf::GCallback<void (int)> CallbackType;
	typedef cpgf::GCallbackList<void (int)> CallbackListType;
	
public:
	explicit PanelEase(wxWindow * parent);
	~PanelEase();

	void addCallback(const CallbackType & callback);

	int getEaseIndex() const { return this->easeIndex; }
	
private:
	void initEaseButtons(wxSizer * sizer);
	void onEaseButtonClicked(wxCommandEvent& event);
	
private:
	CallbackListType callbackList;
	int easeIndex;
};


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
