#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <cstdio>
#include <cinttypes>
#include <FlexArchBase/plugin.h>
#include <FlexArchBase/FlexArchInternals.h>
#include "wxForms.h"
#include <algorithm>
#include <vector>
#include <iomanip>

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

#ifndef _WIN32
#define _stricmp(a, b) strcasecmp(a, b)
#endif

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

class ArchiveContents : public wxTreeItemData
{
private:
    void SetValues(uint64_t id, const char* name, uint64_t flags, uint64_t size, uint64_t size_in_archive, time_t creation_date, time_t modification_date, ArchiveContents* parent, wxTreeItemId * associated_tree_item = NULL)
    {
        this->id = id;
        this->name = wxString::FromUTF8(name);
        this->flags = flags;
        this->size = size;
        this->size_in_archive = size_in_archive;
        this->creation_date = creation_date;
        this->modification_date = modification_date;
        this->parent = parent;
        this->associated_tree_item = associated_tree_item;
    }

public:
    uint64_t id;             
    wxString name;        
    uint64_t flags;        
    uint64_t size;           
    uint64_t size_in_archive;
    time_t creation_date;    
    time_t modification_date;

    wxTreeItemId associated_tree_item;

    wxVector<ArchiveContents*> children;
    ArchiveContents* parent;

    bool sorted;

    ArchiveContents(uint64_t id, const char *name, uint64_t flags, uint64_t size, uint64_t size_in_archive, time_t creation_date, time_t modification_date, ArchiveContents* parent)
    {
        SetValues(id, name, flags, size, size_in_archive, creation_date, modification_date, parent);

        sorted = false;
    }

    ArchiveContents() : 
        ArchiveContents(0, "", 0, 0, 0, 0, 0, 0)
    {
        ;
    }

    ArchiveContents(const ArchiveContents &orig) :
        ArchiveContents(orig.id, orig.name, orig.flags, orig.size, orig.size_in_archive, orig.creation_date, orig.modification_date, orig.parent)
    {
        ;
    }

    ArchiveContents(archive_entry* entry, ArchiveContents* parent) :
        ArchiveContents(entry->id, entry->super_name ? entry->super_name : entry->name, entry->flags, entry->size, entry->size_in_archive, entry->creation_date, entry->modification_date, parent)
    {
        ;
    }

    ~ArchiveContents()
    {
        ;
    }

    ArchiveContents& operator=(const ArchiveContents& other)
    {
        if (this != &other) // not a self-assignment
        {         
            SetValues(other.id, other.name, other.flags, other.size, other.size_in_archive, other.creation_date, other.modification_date, other.parent);
        }
        return *this;
    }
};

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

        m_listCtrl_PluginList->SetColumnWidth(0, 150);
        m_listCtrl_PluginList->SetColumnWidth(1, 320);

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

class AboutDialogImplementation : public AboutDialog
{
private:
    wxWindow* m_parent;

    void DialogClose(wxCloseEvent& event)
    {
        m_parent->Enable(true);
        event.Skip();
    }

public:
    AboutDialogImplementation(wxWindow* parent) : AboutDialog(parent)
    {
        m_parent = parent;

        m_parent->Enable(false);
    }

    ~AboutDialogImplementation()
    {
    }
};

class MainFrameImplementation : public MainFrame
{
private:
    enum columns
    {
        COLUMN_NAME = 0,
        COLUMN_SIZE = 1,
        COLUMN_MOD_DATE = 2,
    };

    opened_archive m_archive;
    bool m_archive_opened;
    ArchiveContents *m_archive_data;
    bool m_human_readable_size;
    enum columns m_sort_by;
    bool m_sort_direction_asc;


    //helpers
    void UpdateGuiForArchiveState()
    {
        if (m_archive_opened)
        {
            m_bpButton_ArchiveSave->Enable(true);
            m_bpButton_ArchiveClose->Enable(true);
            m_bpButton_FileExtract->Enable(true);
            m_menu_archive->Enable(ID_MENU_ARCHIVE_SAVE, true); 
            m_menu_archive->Enable(ID_MENU_ARCHIVE_CLOSE, true);
            m_menu_file->Enable(ID_MENU_FILE_EXTRACT, true);
        }
        else
        {
            m_bpButton_ArchiveSave->Enable(false);
            m_bpButton_ArchiveClose->Enable(false);
            m_bpButton_FileExtract->Enable(false);
            m_menu_archive->Enable(ID_MENU_ARCHIVE_SAVE, false);
            m_menu_archive->Enable(ID_MENU_ARCHIVE_CLOSE, false);
            m_menu_file->Enable(ID_MENU_FILE_EXTRACT, false);
        }
    }
    
    static ArchiveContents *FindItemInTree(ArchiveContents *tree, uint64_t target_id)
    {
        if (tree->id == target_id)
        {
            return tree;
        }

        ArchiveContents* result = NULL;

        for (auto i = tree->children.begin(); i < tree->children.end(); ++i)
        {
            if (result = FindItemInTree(*i, target_id))
            {
                return result;
            }
        }
        return NULL;
    }

    static void FLEXARCH_CALL_TYPE archive_enumerate(archive_handle archive, void* context, archive_entry* entry, uint8_t last_item)
    {
        ArchiveContents* tree = (ArchiveContents*)context;
        ArchiveContents* parent;

        parent = FindItemInTree(tree, entry->parent);
        assert(parent != NULL);

        parent->children.push_back(new ArchiveContents(entry, parent));
    }

    void ViewArchiveFiles()
    {
        if (!m_archive_opened)
        {
            return;
        }

        InitTreeCtrlWithArchiveData(m_treeCtrl_archiveData->GetRootItem(), m_archive_data);
    }

    void InitTreeCtrlWithArchiveData(wxTreeItemId tree_parent, ArchiveContents *archive_parent)
    {
        wxTreeItemId new_id;
        if (archive_parent->id == 0)
        {
            new_id = m_treeCtrl_archiveData->AddRoot(archive_parent->name, -1, -1, archive_parent);
        }
        else
        {
            new_id = m_treeCtrl_archiveData->AppendItem(tree_parent, archive_parent->name, -1, -1, archive_parent);
        }

        archive_parent->associated_tree_item = new_id;

        if (!archive_parent->sorted)
        {
            std::sort(archive_parent->children.begin(), archive_parent->children.end(), 
                [](ArchiveContents* a, ArchiveContents* b) -> bool
                {
                    if ((a->flags & FA_ENTRY_IS_DIRECTORY) == (b->flags & FA_ENTRY_IS_DIRECTORY)) return a->name < b->name;
                    if (a->flags & FA_ENTRY_IS_DIRECTORY) return true;
                    return false;
                });
            archive_parent->sorted = true;
        }

        std::for_each(archive_parent->children.begin(), archive_parent->children.end(), [this,new_id](ArchiveContents *child) {
            InitTreeCtrlWithArchiveData(new_id, child);
            });
    }

    void ListViewSort()
    {
        struct sort_info_temp_t
        {
            int sort_by;
            bool sort_direction_asc;
        }sort_info_temp = { m_sort_by , m_sort_direction_asc };

        //m_listCtrl_archiveData->RemoveSortIndicator();
        m_listCtrl_archiveData->ShowSortIndicator(m_sort_by, m_sort_direction_asc);

        m_listCtrl_archiveData->SortItems(
            [](wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData) -> int
            { 
                ArchiveContents* a = (ArchiveContents*)item1;
                ArchiveContents* b = (ArchiveContents*)item2;
                sort_info_temp_t* sort_type = (sort_info_temp_t*)sortData;

                //special cases
                if (!a || !b) return 0;
                if ((b->parent && b->parent->parent) && (a->id == b->parent->parent->id)) return -1;
                if ((a->parent->parent) && (b->id == a->parent->parent->id)) return 1;

                //directories first always
                if ((a->flags & FA_ENTRY_IS_DIRECTORY) == (b->flags & FA_ENTRY_IS_DIRECTORY))
                {
                    //actual sort compare
                    int r;
                    switch (sort_type->sort_by)
                    {
                    case COLUMN_NAME:
                        r = _stricmp(a->name, b->name);
                        break;

                    case COLUMN_SIZE:
                        r = a->size - b->size;
                        break;

                    case COLUMN_MOD_DATE:
                        r = a->modification_date - b->modification_date;
                        break;

                    default:
                        r = 0;
                    }
                    return sort_type->sort_direction_asc ? r : -r;
                }
                if (a->flags & FA_ENTRY_IS_DIRECTORY) return -1;
                return 1;
            }, (wxIntPtr)&sort_info_temp);
    }

    void ListViewUpdate()
    {
        if (!m_archive_opened)
        {
            return;
        }

        ArchiveContents* item = (ArchiveContents*)m_treeCtrl_archiveData->GetItemData(m_treeCtrl_archiveData->GetSelection());

        if ((item->flags & FA_ENTRY_IS_DIRECTORY) == 0)
        {
            item = item->parent;
        }

        m_listCtrl_archiveData->ClearAll();

        m_listCtrl_archiveData->AppendColumn("Name", wxLIST_FORMAT_LEFT, 200);
        m_listCtrl_archiveData->AppendColumn("Size", wxLIST_FORMAT_RIGHT, 80);
        m_listCtrl_archiveData->AppendColumn("Modified", wxLIST_FORMAT_LEFT, 120);
        int i;
        for (i = 0; i < item->children.size(); ++i)
        {
            char time_buf[128];
            std::tm* tm = std::localtime(&item->modification_date);
            strftime(time_buf, sizeof(time_buf), "%F %T", tm);

            char size_buf[128];
            if (m_human_readable_size)
            {
                FlexArch_FormatSizeHumanly(size_buf, sizeof(size_buf), item->children[i]->size);
            }
            else
            {
                uint64_t size_temp = item->children[i]->size;
                std::string size_str_temp;

                do
                {
                    snprintf(size_buf, sizeof(size_buf), "%.3" PRIu64 " ", size_temp % 1000);
                    size_temp /= 1000;
                    size_str_temp = size_buf + size_str_temp;
                } while (size_temp);

                if (size_str_temp[size_str_temp.length() - 1] == ' ')
                {
                    size_str_temp = size_str_temp.substr(0, size_str_temp.length() - 1);
                }
                while (size_str_temp.length() > 1 && size_str_temp[0] == '0') size_str_temp = size_str_temp.substr(1, size_str_temp.length() - 1);
                strncpy(size_buf, size_str_temp.c_str(), sizeof(size_buf));
            }

            m_listCtrl_archiveData->InsertItem(i, "");
            m_listCtrl_archiveData->SetItem(i, COLUMN_NAME, item->children[i]->name, (item->children[i]->flags & FA_ENTRY_IS_DIRECTORY) ? 1 : 0);
            m_listCtrl_archiveData->SetItem(i, COLUMN_SIZE, size_buf);
            m_listCtrl_archiveData->SetItem(i, COLUMN_MOD_DATE, time_buf);
            m_listCtrl_archiveData->SetItemPtrData(i, (wxUIntPtr)item->children[i]);
        }

        if (item->parent)
        {
            m_listCtrl_archiveData->InsertItem(i, "");
            m_listCtrl_archiveData->SetItem(i, COLUMN_NAME, "..", (FA_ENTRY_IS_DIRECTORY & FA_ENTRY_IS_DIRECTORY) ? 1 : 0);
            m_listCtrl_archiveData->SetItemPtrData(i, (wxUIntPtr)item->parent);
        }

        ListViewSort();
    }

    //different events
    void FrameClose(wxCloseEvent& event)
    {
        wxCommandEvent dummy;
        ArchiveClose((wxCommandEvent&)dummy);
        if (!dummy.GetSkipped())
        {
            event.Skip();
        }
    }

    void TreeSelectedNewItem(wxTreeEvent& event)
    {
        ListViewUpdate();
    }

    void FileListItemActivated(wxListEvent& event)
    {
        ArchiveContents* item_activated = (ArchiveContents*)event.GetItem().GetData();

        if (item_activated && item_activated->flags & FA_ENTRY_IS_DIRECTORY)
        {
            m_treeCtrl_archiveData->SelectItem(item_activated->associated_tree_item, true);
        }

        event.Skip();
    }

    void FileListColumnClicked(wxListEvent& event)
    { 
        if (event.GetColumn() == m_sort_by)
        {
            m_sort_direction_asc = !m_sort_direction_asc;
        }
        else
        {
            m_sort_by = (columns)event.GetColumn();
        }

        ListViewSort();
        event.Skip(); 
    }

    //menus
    ///archive
    void ArchiveOpen(wxCommandEvent& event)
    {
        if (m_archive_opened)
        {
            int user_chouce = wxMessageBox("Archive already opened.\nClose archive?", "Caution", wxOK | wxCANCEL);

            if (user_chouce == wxCANCEL)
            {
                return;
            }
            ArchiveClose(event);
        }

        wxFileDialog openFileDialog(this);

        if (openFileDialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        if (!FlexArch_TryOpenArchive(&m_archive, openFileDialog.GetPath().c_str().AsChar()))
        {
            wxMessageBox("Can not open archive " + openFileDialog.GetPath(), "Error");
            return;
        }

        m_archive_data = new ArchiveContents(0, wxFileName(openFileDialog.GetPath()).GetFullName(), FA_ENTRY_IS_DIRECTORY, 0, 0, 0, 0, 0);
        m_archive.used_plugin.Archive_EnumerateEntries(m_archive.handle, (void*)m_archive_data, archive_enumerate);

        m_archive_opened = true;
        UpdateGuiForArchiveState();
        ViewArchiveFiles();
    }

    void ArchiveSave(wxCommandEvent& event)
    {
        if (!m_archive_opened)
        {
            event.Skip();
        }
        m_archive.used_plugin.Archive_Save(m_archive.handle);
    }

    void ArchiveClose(wxCommandEvent& event)
    {
        if (!m_archive_opened)
        {
            return;
        }

        if (m_archive.have_unsaved_changes)
        {
            int user_chouce = wxMessageBox("Archive have unsaved changes\nSave changes?", "Caution", wxYES_NO | wxID_CANCEL);
            if (user_chouce == wxID_CANCEL)
            {
                event.Skip();
                return;
            }

            if (user_chouce == wxYES)
            {
                m_archive.used_plugin.Archive_Save(m_archive.handle);
            }
        }

        m_archive.used_plugin.Archive_Close(m_archive.handle);
        m_archive_opened = false;
        UpdateGuiForArchiveState();

        m_treeCtrl_archiveData->DeleteAllItems();
        m_listCtrl_archiveData->DeleteAllItems();
    }
    ///file
    void FileExtract(wxCommandEvent& event)
    {
        event.Skip();
    }
    ///view
    void ViewToggleHumanReadableSize(wxCommandEvent& event)
    {
        m_human_readable_size = m_menu_view->IsChecked(ID_HUMANREADABLE_SIZE);
        ListViewUpdate();
        event.Skip();
    }
    ///Help
    void HelpLoadedPlugings(wxCommandEvent& event)
    {
        PluginListDialogImplementation* pl = new PluginListDialogImplementation(this);

        pl->Show();
    }

    void HelpAbout(wxCommandEvent& event)
    {
        AboutDialogImplementation* ab = new AboutDialogImplementation(this);

        ab->Show();
    }

public:
    MainFrameImplementation(wxWindow* parent) : MainFrame(parent)
    {
        m_archive_opened = false;
        UpdateGuiForArchiveState();

        wxImageList* images = new wxImageList(16, 16, true, 2);

        wxBitmap file_bmp = m_menu_file->GetMenuItems()[1]->GetBitmap();   //fixme
        wxBitmap folder_bmp = m_menu_file->GetMenuItems()[2]->GetBitmap(); //fixme

        wxBitmap::Rescale(folder_bmp, wxSize(16, 16));
        wxBitmap::Rescale(file_bmp, wxSize(16, 16));

        images->Add(file_bmp);
        images->Add(folder_bmp);

        m_listCtrl_archiveData->AssignImageList(images, wxIMAGE_LIST_SMALL);

        m_human_readable_size = false;
        m_archive_opened = false;

        m_sort_by = COLUMN_NAME;
        m_sort_direction_asc = true;
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

