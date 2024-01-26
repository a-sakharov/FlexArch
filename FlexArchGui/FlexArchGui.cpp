#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/listctrl.h>
#include <stdio.h>
#include <FlexArchBase/plugin.h>
#include <FlexArchBase/FlexArchInternals.h>
#include "wxForms.h"
#include <algorithm>
#include <vector>

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

class ArchiveContents : public wxTreeItemData
{
private:
    void SetValues(uint64_t id, const char* name, uint64_t flags, uint64_t size, uint64_t size_in_archive, time_t creation_date, time_t modification_date)
    {
        this->id = id;
        this->name = wxString::FromUTF8(name);
        this->flags = flags;
        this->size = size;
        this->size_in_archive = size_in_archive;
        this->creation_date = creation_date;
        this->modification_date = modification_date;
    }

public:
    uint64_t id;             
    wxString name;        
    uint64_t flags;        
    uint64_t size;           
    uint64_t size_in_archive;
    time_t creation_date;    
    time_t modification_date;

    wxVector<ArchiveContents*> children;

    bool sorted;

    ArchiveContents(uint64_t id, const char *name, uint64_t flags, uint64_t size, uint64_t size_in_archive, time_t creation_date, time_t modification_date)
    {
        SetValues(id, name, flags, size, size_in_archive, creation_date, modification_date);

        sorted = false;
    }

    ArchiveContents() : 
        ArchiveContents(0, "", 0, 0, 0, 0, 0)
    {
        ;
    }

    ArchiveContents(const ArchiveContents &orig) :
        ArchiveContents(orig.id, orig.name, orig.flags, orig.size, orig.size_in_archive, orig.creation_date, orig.modification_date)
    {
        ;
    }

    ArchiveContents(archive_entry* entry) :
        ArchiveContents(entry->id, entry->super_name ? entry->super_name : entry->name, entry->flags, entry->size, entry->size_in_archive, entry->creation_date, entry->modification_date) 
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
            SetValues(other.id, other.name, other.flags, other.size, other.size_in_archive, other.creation_date, other.modification_date);
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
    opened_archive m_archive;
    wxString m_archive_name;
    bool m_archive_opened;
    ArchiveContents *m_archive_data;

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
#if 1
        std::for_each(tree->children.begin(), tree->children.end(), [&result, target_id](ArchiveContents *child) {
                result = FindItemInTree(child, target_id);
                if (result)
                {
                    return result;
                }
            });
#else
        for (ArchiveContents child: tree->children)
        {
            result = FindItemInTree(&child, target_id);
            if (result)
            {
                return result;
            }
        }
#endif

        return result;
    }

    static void FLEXARCH_CALL_TYPE archive_enumerate(archive_handle archive, void* context, archive_entry* entry, uint8_t last_item)
    {
        ArchiveContents* tree = (ArchiveContents*)context;
        ArchiveContents* parent;

        parent = FindItemInTree(tree, entry->parent);
        assert(parent != NULL);

        parent->children.push_back(new ArchiveContents(entry));
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

        if (!archive_parent->sorted)
        {
            std::sort(archive_parent->children.begin(), archive_parent->children.end(), [](ArchiveContents* a, ArchiveContents* b) { 
                
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

        m_archive_name = wxFileName(openFileDialog.GetPath()).GetFullName();
        m_archive_data = new ArchiveContents(0, m_archive_name, 0, 0, 0, 0, 0);
        m_archive.used_plugin.Archive_EnumerateEntries(m_archive.handle, (void*)m_archive_data, archive_enumerate);

        m_archive_opened = true;
        UpdateGuiForArchiveState();
        ViewArchiveFiles();
    }

    void ArchiveSave(wxCommandEvent& event)
    {
        event.Skip();
    }

    void ArchiveClose(wxCommandEvent& event)
    {
        if (!m_archive_opened)
        {
            return;
        }

        if (m_archive.have_unsaved_changes)
        {
            //ask if changes should be saved
            //save changes if should
        }

        m_archive.used_plugin.Archive_Close(m_archive.handle);
        m_archive_name = "";
        m_archive_opened = false;
        UpdateGuiForArchiveState();

        m_treeCtrl_archiveData->DeleteAllItems();
        m_listCtrl_archiveData->DeleteAllItems();
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
        AboutDialogImplementation* ab = new AboutDialogImplementation(this);

        ab->Show();
    }

    void TreeSelectedNewItem(wxTreeEvent& event) 
    { 
        ArchiveContents* item = (ArchiveContents * )m_treeCtrl_archiveData->GetItemData(event.GetItem());
        
        m_listCtrl_archiveData->ClearAll();

        m_listCtrl_archiveData->AppendColumn("");
        m_listCtrl_archiveData->AppendColumn("Name");
        m_listCtrl_archiveData->AppendColumn("Size");
        
        wxImageList *images = new wxImageList(32, 32, true, 2);
        
        images->Add(m_menu_file->GetMenuItems()[1]->GetBitmap());
        images->Add(m_menu_file->GetMenuItems()[2]->GetBitmap());

        m_listCtrl_archiveData->AssignImageList(images, wxIMAGE_LIST_SMALL);
        
        for (int i = 0; i < item->children.size(); ++i)
        {
            m_listCtrl_archiveData->InsertItem(i, "");
            m_listCtrl_archiveData->SetItem(i, 0, "", item->children[i]->flags & FA_ENTRY_IS_DIRECTORY ? 1 : 0);
            m_listCtrl_archiveData->SetItem(i, 1, item->children[i]->name);
            m_listCtrl_archiveData->SetItem(i, 2, std::to_string(item->children[i]->size));
        }

    }

public:
    MainFrameImplementation(wxWindow* parent) : MainFrame(parent)
    {
        m_archive_opened = false;
        UpdateGuiForArchiveState();
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

