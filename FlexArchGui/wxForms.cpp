///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.0.0-0-g0efcecf)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxForms.h"

#include "res/books.png.h"
#include "res/cross_mark.png.h"
#include "res/download.png.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	m_toolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_bpButton_ArchiveOpen = new wxBitmapButton( m_toolBar, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 32,32 ), wxBU_AUTODRAW|wxBORDER_NONE );

	m_bpButton_ArchiveOpen->SetBitmap( books_png_to_wx_bitmap() );
	m_toolBar->AddControl( m_bpButton_ArchiveOpen );
	m_bpButton_ArchiveSave = new wxBitmapButton( m_toolBar, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 32,32 ), wxBU_AUTODRAW|wxBORDER_NONE );

	m_bpButton_ArchiveSave->SetBitmap( download_png_to_wx_bitmap() );
	m_toolBar->AddControl( m_bpButton_ArchiveSave );
	m_bpButton_ArchiveClose = new wxBitmapButton( m_toolBar, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 32,-1 ), wxBU_AUTODRAW|wxBORDER_NONE );

	m_bpButton_ArchiveClose->SetBitmap( cross_mark_png_to_wx_bitmap() );
	m_toolBar->AddControl( m_bpButton_ArchiveClose );
	m_staticline1 = new wxStaticLine( m_toolBar, wxID_ANY, wxDefaultPosition, wxSize( -1,30 ), wxLI_VERTICAL );
	m_toolBar->AddControl( m_staticline1 );
	m_bpButton4 = new wxBitmapButton( m_toolBar, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
	m_toolBar->AddControl( m_bpButton4 );
	m_toolBar->Realize();

	m_menubar = new wxMenuBar( 0 );
	archive = new wxMenu();
	wxMenuItem* open;
	open = new wxMenuItem( archive, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	open->SetBitmaps( books_png_to_wx_bitmap() );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	open->SetBitmap( books_png_to_wx_bitmap() );
	#endif
	archive->Append( open );

	wxMenuItem* save;
	save = new wxMenuItem( archive, ID_SAVE, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	save->SetBitmaps( download_png_to_wx_bitmap() );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	save->SetBitmap( download_png_to_wx_bitmap() );
	#endif
	archive->Append( save );

	wxMenuItem* close;
	close = new wxMenuItem( archive, ID_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	close->SetBitmaps( cross_mark_png_to_wx_bitmap() );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	close->SetBitmap( cross_mark_png_to_wx_bitmap() );
	#endif
	archive->Append( close );

	m_menubar->Append( archive, wxT("Archive") );

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

	// Connect Events
	m_bpButton_ArchiveOpen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveOpen ), NULL, this );
	m_bpButton_ArchiveSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveSave ), NULL, this );
	m_bpButton_ArchiveClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveClose ), NULL, this );
	archive->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ArchiveOpen ), this, open->GetId());
	archive->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ArchiveSave ), this, save->GetId());
	archive->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ArchiveClose ), this, close->GetId());
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	m_bpButton_ArchiveOpen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveOpen ), NULL, this );
	m_bpButton_ArchiveSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveSave ), NULL, this );
	m_bpButton_ArchiveClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveClose ), NULL, this );

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
