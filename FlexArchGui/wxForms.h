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
#include <wx/statline.h>
#include <wx/toolbar.h>
#include <wx/menu.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/dialog.h>
#include <wx/stattext.h>

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
			ID_MENU_ARCHIVE_OPEN = 1000,
			ID_MENU_ARCHIVE_SAVE,
			ID_MENU_ARCHIVE_CLOSE,
			ID_MENU_FILE_EXTRACT,
			ID_NEW_FILE,
			ID_NEW_DIRECTORY,
			ID_HUMANREADABLE_SIZE,
			ID_LOADED_PLUGINS,
			ID_ABOUT
		};

		wxStatusBar* m_statusBar;
		wxToolBar* m_toolBar;
		wxBitmapButton* m_bpButton_ArchiveOpen;
		wxBitmapButton* m_bpButton_ArchiveSave;
		wxBitmapButton* m_bpButton_ArchiveClose;
		wxStaticLine* m_staticline1;
		wxBitmapButton* m_bpButton_FileExtract;
		wxMenuBar* m_menubar;
		wxMenu* m_menu_archive;
		wxMenu* m_menu_file;
		wxMenu* m_menu_view;
		wxMenu* m_menu_help;
		wxTreeCtrl* m_treeCtrl_archiveData;
		wxListCtrl* m_listCtrl_archiveData;

		// Virtual event handlers, override them in your derived class
		virtual void FrameClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void ArchiveOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void ArchiveSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void ArchiveClose( wxCommandEvent& event ) { event.Skip(); }
		virtual void FileExtract( wxCommandEvent& event ) { event.Skip(); }
		virtual void ViewToggleHumanReadableSize( wxCommandEvent& event ) { event.Skip(); }
		virtual void HelpLoadedPlugings( wxCommandEvent& event ) { event.Skip(); }
		virtual void HelpAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void TreeSelectedNewItem( wxTreeEvent& event ) { event.Skip(); }
		virtual void FileListItemActivated( wxListEvent& event ) { event.Skip(); }


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
		wxListCtrl* m_listCtrl_PluginList;

		// Virtual event handlers, override them in your derived class
		virtual void DialogClose( wxCloseEvent& event ) { event.Skip(); }


	public:

		PluginListDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Loaded plugins"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,350 ), long style = wxDEFAULT_DIALOG_STYLE );

		~PluginListDialog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialog
///////////////////////////////////////////////////////////////////////////////
class AboutDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText2;

		// Virtual event handlers, override them in your derived class
		virtual void DialogClose( wxCloseEvent& event ) { event.Skip(); }


	public:

		AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 256,107 ), long style = wxDEFAULT_DIALOG_STYLE );

		~AboutDialog();

};

