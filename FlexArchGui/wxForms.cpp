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
#include "res/file.png.h"
#include "res/inbox.png.h"
#include "res/open_folder.png.h"

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
	m_bpButton_FileExtract = new wxBitmapButton( m_toolBar, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );

	m_bpButton_FileExtract->SetBitmap( inbox_png_to_wx_bitmap() );
	m_toolBar->AddControl( m_bpButton_FileExtract );
	m_toolBar->Realize();

	m_menubar = new wxMenuBar( 0 );
	m_menu_archive = new wxMenu();
	wxMenuItem* m_menu_archive_open;
	m_menu_archive_open = new wxMenuItem( m_menu_archive, ID_MENU_ARCHIVE_OPEN, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menu_archive_open->SetBitmaps( books_png_to_wx_bitmap() );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menu_archive_open->SetBitmap( books_png_to_wx_bitmap() );
	#endif
	m_menu_archive->Append( m_menu_archive_open );

	wxMenuItem* m_menu_archive_save;
	m_menu_archive_save = new wxMenuItem( m_menu_archive, ID_MENU_ARCHIVE_SAVE, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menu_archive_save->SetBitmaps( download_png_to_wx_bitmap() );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menu_archive_save->SetBitmap( download_png_to_wx_bitmap() );
	#endif
	m_menu_archive->Append( m_menu_archive_save );

	wxMenuItem* m_menu_archive_close;
	m_menu_archive_close = new wxMenuItem( m_menu_archive, ID_MENU_ARCHIVE_CLOSE, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menu_archive_close->SetBitmaps( cross_mark_png_to_wx_bitmap() );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menu_archive_close->SetBitmap( cross_mark_png_to_wx_bitmap() );
	#endif
	m_menu_archive->Append( m_menu_archive_close );

	m_menubar->Append( m_menu_archive, wxT("Archive") );

	m_menu_file = new wxMenu();
	wxMenuItem* m_menu_file_extract;
	m_menu_file_extract = new wxMenuItem( m_menu_file, ID_MENU_FILE_EXTRACT, wxString( wxT("Extract") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menu_file_extract->SetBitmaps( inbox_png_to_wx_bitmap() );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menu_file_extract->SetBitmap( inbox_png_to_wx_bitmap() );
	#endif
	m_menu_file->Append( m_menu_file_extract );

	wxMenuItem* m_menu_file_newFile;
	m_menu_file_newFile = new wxMenuItem( m_menu_file, ID_NEW_FILE, wxString( wxT("New file") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menu_file_newFile->SetBitmaps( file_png_to_wx_bitmap() );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menu_file_newFile->SetBitmap( file_png_to_wx_bitmap() );
	#endif
	m_menu_file->Append( m_menu_file_newFile );

	wxMenuItem* m_menu_file_newDirectory;
	m_menu_file_newDirectory = new wxMenuItem( m_menu_file, ID_NEW_DIRECTORY, wxString( wxT("New directory") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	m_menu_file_newDirectory->SetBitmaps( open_folder_png_to_wx_bitmap() );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	m_menu_file_newDirectory->SetBitmap( open_folder_png_to_wx_bitmap() );
	#endif
	m_menu_file->Append( m_menu_file_newDirectory );

	m_menubar->Append( m_menu_file, wxT("File") );

	m_menu_view = new wxMenu();
	wxMenuItem* m_menu_human_readable_size;
	m_menu_human_readable_size = new wxMenuItem( m_menu_view, ID_HUMANREADABLE_SIZE, wxString( wxT("Human-readable size") ) , wxEmptyString, wxITEM_CHECK );
	m_menu_view->Append( m_menu_human_readable_size );

	m_menubar->Append( m_menu_view, wxT("View") );

	m_menu_help = new wxMenu();
	wxMenuItem* m_menu_help_loadedPlugins;
	m_menu_help_loadedPlugins = new wxMenuItem( m_menu_help, ID_LOADED_PLUGINS, wxString( wxT("Loaded plugins") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_help->Append( m_menu_help_loadedPlugins );

	wxMenuItem* m_menu_help_about;
	m_menu_help_about = new wxMenuItem( m_menu_help, ID_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu_help->Append( m_menu_help_about );

	m_menubar->Append( m_menu_help, wxT("Help") );

	this->SetMenuBar( m_menubar );

	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxHORIZONTAL );

	m_treeCtrl_archiveData = new wxTreeCtrl( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), wxTR_DEFAULT_STYLE|wxBORDER_NONE|wxHSCROLL|wxVSCROLL );
	bMainSizer->Add( m_treeCtrl_archiveData, 0, wxALL|wxEXPAND, 5 );

	m_listCtrl_archiveData = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bMainSizer->Add( m_listCtrl_archiveData, 1, wxEXPAND, 5 );


	this->SetSizer( bMainSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::FrameClose ) );
	m_bpButton_ArchiveOpen->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveOpen ), NULL, this );
	m_bpButton_ArchiveSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveSave ), NULL, this );
	m_bpButton_ArchiveClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveClose ), NULL, this );
	m_bpButton_FileExtract->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::FileExtract ), NULL, this );
	m_menu_archive->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ArchiveOpen ), this, m_menu_archive_open->GetId());
	m_menu_archive->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ArchiveSave ), this, m_menu_archive_save->GetId());
	m_menu_archive->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ArchiveClose ), this, m_menu_archive_close->GetId());
	m_menu_file->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::FileExtract ), this, m_menu_file_extract->GetId());
	m_menu_view->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::ViewToggleHumanReadableSize ), this, m_menu_human_readable_size->GetId());
	m_menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::HelpLoadedPlugings ), this, m_menu_help_loadedPlugins->GetId());
	m_menu_help->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::HelpAbout ), this, m_menu_help_about->GetId());
	m_treeCtrl_archiveData->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrame::TreeSelectedNewItem ), NULL, this );
	m_listCtrl_archiveData->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::FileListItemActivated ), NULL, this );
}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::FrameClose ) );
	m_bpButton_ArchiveOpen->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveOpen ), NULL, this );
	m_bpButton_ArchiveSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveSave ), NULL, this );
	m_bpButton_ArchiveClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::ArchiveClose ), NULL, this );
	m_bpButton_FileExtract->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::FileExtract ), NULL, this );
	m_treeCtrl_archiveData->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainFrame::TreeSelectedNewItem ), NULL, this );
	m_listCtrl_archiveData->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrame::FileListItemActivated ), NULL, this );

}

PluginListDialog::PluginListDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bMainSizer;
	bMainSizer = new wxBoxSizer( wxVERTICAL );

	m_listCtrl_PluginList = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	bMainSizer->Add( m_listCtrl_PluginList, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bMainSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PluginListDialog::DialogClose ) );
}

PluginListDialog::~PluginListDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( PluginListDialog::DialogClose ) );

}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxT("FlexArch"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText1->Wrap( -1 );
	bSizer3->Add( m_staticText1, 0, wxALL|wxEXPAND, 5 );

	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText2->Wrap( -1 );
	bSizer3->Add( m_staticText2, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AboutDialog::DialogClose ) );
}

AboutDialog::~AboutDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( AboutDialog::DialogClose ) );

}
