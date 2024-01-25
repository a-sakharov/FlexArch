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

class PluginListDialogImplementation : public PluginListDialog
{
private:
    wxWindow* m_parent;

    void DialogClose(wxCloseEvent& event)
    {
        m_parent->Enable(true);
        event.Skip();
    }

public:
    PluginListDialogImplementation(wxWindow* parent) : PluginListDialog(parent)
    {
        m_parent = parent;

        m_parent->Enable(false);
        
        m_listCtrl_PluginList->InsertColumn(0, "Name");
        m_listCtrl_PluginList->InsertColumn(1, "Description");

        for (size_t i = 0; i < LoadedPluginsCount; ++i)
        {
            m_listCtrl_PluginList->InsertItem(i, "");
            m_listCtrl_PluginList->SetItem(i, 0, LoadedPlugins[i].Plugin_GetName());
            m_listCtrl_PluginList->SetItem(i, 1, LoadedPlugins[i].Plugin_Description());
        }
    }

    ~PluginListDialogImplementation()
    {
    }
};

class MainFrameImplementation : public MainFrame
{
private:
    void ArchiveOpen(wxCommandEvent& event)
    {
        event.Skip();
    }

    void ArchiveSave(wxCommandEvent& event)
    {
        event.Skip();
    }

    void ArchiveClose(wxCommandEvent& event)
    {
        event.Skip();
    }

    void FileExtract(wxCommandEvent& event)
    {
        event.Skip();
    }

    void HelpLoadedPlugings(wxCommandEvent& event)
    {
        PluginListDialogImplementation* pl = new PluginListDialogImplementation(this);

        pl->Show();
    }

    void HelpAbout(wxCommandEvent& event)
    {
        event.Skip();
    }

public:
    MainFrameImplementation(wxWindow* parent) : MainFrame(parent)
    {

    }

    ~MainFrameImplementation()
    {
    }
};


bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    FlexArch_CollectPlugins();

    MainFrameImplementation* frame = new MainFrameImplementation(NULL);
    frame->Show(true);
    return true;
}

