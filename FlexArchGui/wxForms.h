///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bmpbuttn.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/toolbar.h>
#include <wx/menu.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame
{
	private:

	protected:
		enum
		{
			ID_SAVE = 1000,
			ID_CLOSE,
			ID_EXIT,
			ID_LOADED_PLUGINS,
			ID_ABOUT
		};

		wxStatusBar* m_statusBar;
		wxToolBar* m_toolBar;
		wxBitmapButton* m_bpButton_open;
		wxBitmapButton* m_bpButton_save;
		wxBitmapButton* m_bpButton_close;
		wxMenuBar* m_menubar;
		wxMenu* file;
		wxMenu* help;
		wxListCtrl* m_listCtrl_archiveData;

	public:

		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("FlexArch"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 936,675 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MainFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PluginListDialog
///////////////////////////////////////////////////////////////////////////////
class PluginListDialog : public wxDialog
{
	private:

	protected:
		wxListBox* m_listBox_pluginList;

	public:

		PluginListDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 297,224 ), long style = wxDEFAULT_DIALOG_STYLE );

		~PluginListDialog();

};

