// wxFM.cpp

#include "StdAfx.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

// #include "../GUI/p7zip_32.xpm"

int Main1(int argc,TCHAR **argv);

/*
int Main1(int argc,TCHAR **argv)
{
	// FIXME
	Sleep(10000);
}
*/
	

#include "Windows/Registry.h"
using namespace NWindows;
using namespace NRegistry;


#include "Common/StringConvert.h"
#include "Windows/FileDir.h"
#include "Windows/Synchronization.h"

// FIXME #include "ExtractRes.h"
#include "../Explorer/MyMessages.h"

// FIXME #include "ExtractGUI.h"
// FIXME #include "UpdateGUI.h"
// FIXME #include "BenchmarkDialog.h"
#include "RegistryUtils.h"

using namespace NWindows;
using namespace NFile;

#include "../FileManager/ProgramLocation.h"

static LPCWSTR kHelpFileName = L"help/";

void ShowHelpWindow(HWND hwnd, LPCWSTR topicFile)
{
  UString path;
  if (!::GetProgramFolderPath(path))
    return;
  path += kHelpFileName;
  path += topicFile;
  printf("ShowHelpWindow(%p,%ls)=>%ls\n",hwnd,topicFile,(const wchar_t *)path);
  // HtmlHelp(hwnd, GetSystemString(path), HH_DISPLAY_TOPIC, NULL);
  wxString path2(path);
  wxLaunchDefaultBrowser(path2);
}

////////////////////////////// TRIES ///////////////////////////////////

// FIXME : duplicate

#if 0
static const TCHAR *kCUBasePath = TEXT("Software/7-ZIP");
static const WCHAR *kLangValueName = L"Lang";

void SaveRegLang(const UString &langFile)
{
  CKey key;
  key.Create(HKEY_CURRENT_USER, kCUBasePath);
  key.SetValue(kLangValueName, langFile);
}

void ReadRegLang(UString &langFile)
{
  langFile.Empty();
  CKey key;
  if (key.Open(HKEY_CURRENT_USER, kCUBasePath, KEY_READ) == ERROR_SUCCESS)
    key.QueryValue(kLangValueName, langFile);
}
#endif


//////////////////////////////////

#define NEED_NAME_WINDOWS_TO_UNIX
#include "myPrivate.h" // global_use_utf16_conversion

void mySplitCommandLineW(int numArguments, TCHAR  **arguments,UStringVector &parts) {

  parts.Clear();
  for(int ind=0;ind < numArguments; ind++) {
      UString tmp = arguments[ind];
      // tmp.Trim(); " " is a valid filename ...
      if (!tmp.IsEmpty()) {
        parts.Add(tmp);
      }
  }
}

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new frame type
#if 0
class MyFrame: public wxFrame
{
public:
    // ctor
    MyFrame(wxFrame *frame, const wxString& title, int x, int y, int w, int h);
    // virtual ~MyFrame();

    // operations
    void WriteText(const wxString& text) { m_txtctrl->WriteText(text); }
    
protected:
    // callbacks
    void OnWorkerEvent(wxCommandEvent& event);
private:
    // just some place to put our messages in
    wxTextCtrl *m_txtctrl;
    DECLARE_EVENT_TABLE()
};

enum {
    WORKER_EVENT=100    // this one gets sent from the worker thread
};
    
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(WORKER_EVENT, MyFrame::OnWorkerEvent)
    // EVT_IDLE(MyFrame::OnIdle)
END_EVENT_TABLE()

// My frame constructor
MyFrame::MyFrame(wxFrame *frame, const wxString& title,
                 int x, int y, int w, int h)
       : wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h))
{
	//FIXME this->SetIcon(wxICON(p7zip_32));
    
#if wxUSE_STATUSBAR
    CreateStatusBar(2);
#endif // wxUSE_STATUSBAR

    m_txtctrl = new wxTextCtrl(this, wxID_ANY, _T(""), wxPoint(0, 0), wxSize(0, 0), wxTE_MULTILINE | wxTE_READONLY);
}

void myCreateHandle(int n);
void MyFrame::OnWorkerEvent(wxCommandEvent& event)
{
	int n = event.GetInt();
	myCreateHandle(n);
}

wxWindow * g_window=0;
#endif


// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

time_t g_T0 = 0;

#if 0
class MyThread : public wxThread
{
	int _argc;
	TCHAR **_argv;
public:
	MyThread(int argc,TCHAR **argv): wxThread(),_argc(argc), _argv(argv) {}

	// thread execution starts here
	virtual void *Entry()
	{
		int ret = Main1(_argc,_argv);
		exit(ret);
	}
};
#endif

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // don't parse the command-line options !
    // : if ( !wxApp::OnInit() ) return false;

  { // define P7ZIP_HOME_DIR
    extern void my_windows_split_path(const AString &p_path, AString &dir , AString &base);
    static char p7zip_home_dir[MAX_PATH];

    UString fullPath;
    NDirectory::MyGetFullPathName(wxApp::argv[0], fullPath);
    AString afullPath = GetAnsiString(fullPath);

    AString dir,name;

    my_windows_split_path(afullPath,dir,name);

    const char *dir2 = nameWindowToUnix((const char *)dir);
    snprintf(p7zip_home_dir,sizeof(p7zip_home_dir),"P7ZIP_HOME_DIR=%s/",dir2);
    p7zip_home_dir[sizeof(p7zip_home_dir)-1] = 0;
    putenv(p7zip_home_dir);
    // DEBUG printf("putenv(%s)\n",p7zip_home_dir);
  }
  global_use_utf16_conversion = 1; // UNICODE !

  g_T0 = time(0);
  // DEBUG printf("MAIN Thread : 0x%lx\n",wxThread::GetCurrentId());

   // Create the main frame window
#if 0
    MyFrame *frame = new MyFrame((wxFrame *)NULL, _T("7-zip Main Window"), 50, 50, 450, 340);
   // Don't Show the frame !
   frame->Show(true); // FIXME

    SetTopWindow(frame);

    g_window = frame;
#endif

    /*
    MyThread *thread = new MyThread(wxApp::argc,wxApp::argv);
    thread->Create(); //  != wxTHREAD_NO_ERROR
    thread->Run();
    */

    wxInitAllImageHandlers();

    Main1(wxApp::argc,wxApp::argv);

  // success: wxApp::OnRun() will be called which will enter the main message
  // loop and the application will run. If we returned false here, the
  // application would exit immediately.
    return true;
}

DWORD WINAPI GetTickCount(VOID) {
	static wxStopWatch sw;
	return sw.Time();
}

//////////////////////////////////////////

#if 0
#include "resource.h"
#include "ExtractRes.h"

static CStringTable g_stringTable[] =
{
  /* resource.rc */	  
  /***************/
	{ IDS_OPEN_TYPE_ALL_FILES, L"All Files" },
	{ IDS_METHOD_STORE, L"Store" },
	{ IDS_METHOD_NORMAL, L"Normal" },
	{ IDS_METHOD_MAXIMUM, L"Maximum" },
	{ IDS_METHOD_FAST, L"Fast" },
	{ IDS_METHOD_FASTEST, L"Fastest" },
	{ IDS_METHOD_ULTRA, L"Ultra" },
	{ IDS_COMPRESS_NON_SOLID, L"Non-solid" },
	{ IDS_COMPRESS_SOLID, L"Solid" },

	{ IDS_COMPRESS_UPDATE_MODE_ADD, L"Add and replace files" },
	{ IDS_COMPRESS_UPDATE_MODE_UPDATE, L"Update and add files" },
	{ IDS_COMPRESS_UPDATE_MODE_FRESH, L"Freshen existing files" },
	{ IDS_COMPRESS_UPDATE_MODE_SYNCHRONIZE, L"Synchronize files" },
	{ IDS_COMPRESS_SET_ARCHIVE_DIALOG_TITLE, L"Browse" },
	{ IDS_COMPRESS_INCORRECT_VOLUME_SIZE, L"Incorrect volume size" },
	{ IDS_COMPRESS_SPLIT_CONFIRM_MESSAGE, L"Specified volume size: {0} bytes.\nAre you sure you want to split archive into such volumes?" },

	{ IDS_PASSWORD_USE_ASCII, L"Use only English letters, numbers and special characters (!, #, $, ...) for password." },
	{ IDS_PASSWORD_PASSWORDS_DO_NOT_MATCH, L"Passwords do not match" },
	{ IDS_PASSWORD_IS_TOO_LONG, L"Password is too long" },

	{ IDS_PROGRESS_COMPRESSING, L"Compressing" },
	{ IDS_PROGRESS_TESTING, L"Testing" },
	{ IDS_MESSAGE_NO_ERRORS, L"There are no errors" },
	{ IDS_FILES_COLON, L"Files:" },
	{ IDS_FOLDERS_COLON, L"Folders:" },
	{ IDS_SIZE_COLON, L"Size:" },
	{ IDS_COMPRESSED_COLON, L"Compressed size:" },
	{ IDS_ARCHIVES_COLON, L"Archives:" },

  /* Extract.rc */	  
  /**************/
	{ IDS_CANNOT_CREATE_FOLDER , L"Cannot create folder '{0}'"},
	{ IDS_OPEN_IS_NOT_SUPORTED_ARCHIVE, L"File is not supported archive."},

	{ IDS_MESSAGES_DIALOG_EXTRACT_MESSAGE_CRC , L"CRC failed in '{0}'. File is broken."},
	{ IDS_MESSAGES_DIALOG_EXTRACT_MESSAGE_DATA_ERROR , L"Data error in '{0}'. File is broken"},
	{ IDS_MESSAGES_DIALOG_EXTRACT_MESSAGE_UNSUPPORTED_METHOD , L"Unsupported compression method for '{0}'."},
	{ IDS_MESSAGES_DIALOG_EXTRACT_MESSAGE_CRC_ENCRYPTED , L"CRC failed in encrypted file '{0}'. Wrong password?"},
	{ IDS_MESSAGES_DIALOG_EXTRACT_MESSAGE_DATA_ERROR_ENCRYPTED , L"Data error in encrypted file '{0}'. Wrong password?"},

	{ IDS_EXTRACT_SET_FOLDER , L"Specify a location for extracted files."},
	{ IDS_MESSAGES_DIALOG_EXTRACT_MESSAGE_CANNOT_OPEN_FILE, L"Can not open output file '{0}'."},
	{ IDS_PROGRESS_EXTRACTING, L"Extracting" },

	{ IDS_CANT_OPEN_ARCHIVE , L"Can not open file '{0}' as archive"},
	{ IDS_CANT_OPEN_ENCRYPTED_ARCHIVE , L"Can not open encrypted archive '{0}'. Wrong password?"},

	{ 0 , 0 }
};

REGISTER_STRINGTABLE(g_stringTable)
#endif


///////////////////////////////////////////

namespace NFsFolder {

	// FIXME
	bool wxw_CopyFile(LPCWSTR existingFile, LPCWSTR newFile, bool overwrite)
	{
		return wxCopyFile(wxString(existingFile), wxString(newFile), overwrite);
	}
}

