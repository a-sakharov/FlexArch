#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <stdio.h>
#include <FlexArchBase/plugin.h>
#include <FlexArchBase/FlexArchInternals.h>
#include "wxForms.h"

#ifdef _WIN32

#ifdef _DEBUG
#pragma comment(lib, "wxmsw32ud_core.lib")
#pragma comment(lib, "wxbase32ud.lib")
#pragma comment(lib, "wxtiffd.lib")
#pragma comment(lib, "wxjpegd.lib")
#pragma comment(lib, "wxpngd.lib")
#pragma comment(lib, "wxzlibd.lib")
#pragma comment(lib, "wxregexud.lib")
#pragma comment(lib, "wxexpatd.lib")
#else
#pragma comment(lib, "wxmsw32u_core.lib")
#pragma comment(lib, "wxbase32u.lib")
#pragma comment(lib, "wxtiff.lib")
#pragma comment(lib, "wxjpeg.lib")
#pragma comment(lib, "wxpng.lib")
#pragma comment(lib, "wxzlib.lib")
#pragma comment(lib, "wxregexu.lib")
#pragma comment(lib, "wxexpat.lib")
#endif

#endif


class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    MainFrame* frame = new MainFrame(NULL);
    frame->Show(true);
    return true;
}
