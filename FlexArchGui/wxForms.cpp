///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxForms.h"

#include "res/open.png.h"
#include "res/open_current.png.h"
#include "res/open_focus.png.h"
#include "res/open_pressed.png.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	m_toolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_bpButton_open = new wxBitmapButton( m_toolBar, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 30,30 ), wxBU_AUTODRAW|wxBORDER_NONE );

	m_bpButton_open->SetBitmap( open_png_to_wx_bitmap() );
	m_bpButton_open->SetBitmapPressed( open_pressed_png_to_wx_bitmap() );
	m_bpButton_open->SetBitmapFocus( open_focus_png_to_wx_bitmap() );
	m_bpButton_open->SetBitmapCurrent( open_current_png_to_wx_bitmap() );
	m_toolBar->AddControl( m_bpButton_open );
	m_bpButton_save = new wxBitmapButton( m_toolBar, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	m_toolBar->AddControl( m_bpButton_save );
	m_bpButton_close = new wxBitmapButton( m_toolBar, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	m_bpButton_close->SetBitmap( wxNullBitmap );
	m_toolBar->AddControl( m_bpButton_close );
	m_toolBar->Realize();

	m_menubar = new wxMenuBar( 0 );
	file = new wxMenu();
	wxMenuItem* open;
	open = new wxMenuItem( file, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( open );

	wxMenuItem* save;
	save = new wxMenuItem( file, ID_SAVE, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( save );

	wxMenuItem* close;
	close = new wxMenuItem( file, ID_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( close );

	file->AppendSeparator();

	wxMenuItem* exit;
	exit = new wxMenuItem( file, ID_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	file->Append( exit );

	m_menubar->Append( file, wxT("File") );

	help = new wxMenu();
	wxMenuItem* loadedPlugins;
	loadedPlugins = new wxMenuItem( help, ID_LOADED_PLUGINS, wxString( wxT("Loaded plugins") ) , wxEmptyString, wxITEM_NORMAL );
	help->Append( loadedPlugins );

	wxMenuItem* about;
	about = new wxMenuItem( help, ID_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	help->Append( about );

	m_menubar->Append( help, wxT("Help") );

	this->SetMenuBar( m_menubar );

	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );

	m_listCtrl_archiveData = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_ICON );
	bMainSizer->Add( m_listCtrl_archiveData, 1, wxEXPAND, 5 );


	this->SetSizer( bMainSizer );
	this->Layout();

	this->Centre( wxBOTH );
}

MainFrame::~MainFrame()
{
}

PluginListDialog::PluginListDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );

	m_listBox_pluginList = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	bMainSizer->Add( m_listBox_pluginList, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bMainSizer );
	this->Layout();

	this->Centre( wxBOTH );
}

PluginListDialog::~PluginListDialog()
{
}
