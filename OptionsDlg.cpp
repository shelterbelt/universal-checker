// OptionsDlg.cpp : Implementation File
//

#include "stdafx.h"
#include "UniCheck.h"
#include "OptionsDlg.h"

#include "AddFilesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg Dialog


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg Construction/Destruction

COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_szProgramPathControlText = _T("");
	m_nMaxFilesToAddAtOneTime = 0;
	m_szHeaderPathControlText = _T("");
	m_szSourcePathControlText = _T("");
	m_szViewerPathControlText = _T("");
	m_szFilesetNameControlText = _T("");
	m_szFilesetFilePathControlText = _T("");
	//}}AFX_DATA_INIT
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Control(pDX, IDC_OPTIONS_MAXFILESPIN, m_wndMaxFilesAdjustmentCtrl);
	DDX_Text(pDX, IDC_OPTIONS_PROGAMPATH, m_szProgramPathControlText);
	DDX_Text(pDX, IDC_OPTIONS_MAXFILESALLOWED, m_nMaxFilesToAddAtOneTime);
	DDV_MinMaxInt(pDX, m_nMaxFilesToAddAtOneTime, UNICHECK_REG_MAXFILEFLOOR, UNICHECK_REG_MAXFILECEILING);
	DDX_Text(pDX, IDC_OPTIONS_HEADERPATH, m_szHeaderPathControlText);
	DDX_Text(pDX, IDC_OPTIONS_SOURCEPATH, m_szSourcePathControlText);
	DDX_Text(pDX, IDC_OPTIONS_VIEWERPATH, m_szViewerPathControlText);
	DDX_Text(pDX, IDC_OPTIONS_FILESETFILENAME, m_szFilesetNameControlText);
	DDX_Text(pDX, IDC_OPTIONS_FILESETPATH, m_szFilesetFilePathControlText);
	//}}AFX_DATA_MAP
}

BOOL COptionsDlg::OnInitDialog() 
{
	// Execute the parent class implementation of the method to complete standard behavior.
	CDialog::OnInitDialog();
	
	// Even though the interface gives the user the flexibility to adjust how many files can
	// be added at one time, the limit of 200 files is put in place to prevent the user
	// from overrunning the file name buffer or copying gig.s of source files to the temp.
	// directories.

	// Set the bounds of the max. files spin control.
	m_wndMaxFilesAdjustmentCtrl.SetRange(UNICHECK_REG_MAXFILEFLOOR, UNICHECK_REG_MAXFILECEILING);
	
	// Return TRUE unless you set the focus to a control.
	// EXCEPTION: OCX Property Pages should return FALSE.
	return TRUE;
}

BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(IDC_OPTIONS_BROWSEBTN, OnOptionsBrowsebtn)
	ON_BN_CLICKED(IDC_OPTIONS_BROWSEHDRBTN, OnOptionsBrowsehdrbtn)
	ON_BN_CLICKED(IDC_OPTIONS_BROWSESRCBTN, OnOptionsBrowsesrcbtn)
	ON_BN_CLICKED(IDC_OPTIONS_BROWSEVWRBTN, OnOptionsBrowsevwrbtn)
	ON_BN_CLICKED(IDC_OPTIONS_BROWSEFSPBTN, OnOptionsBrowsefspbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg Member Access Methods

/* GetProgramPath - Return the Value of m_szProgramPathControlText Member Variable */
CString COptionsDlg::GetProgramPath()
{
	return m_szProgramPathControlText;
}

/* SetProgramPath - Assign a Value to m_szProgramPathControlText Member Variable */
void COptionsDlg::SetProgramPath(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Verify the path really exists.
	if ( !pTheApp->VerifyThatFilePathExists(szPath) )
	{
		// If the path does not exist, set the path to NULL.
		m_szProgramPathControlText = NULL_TEXT;
	}
	else
	{
		// If the path does exist, load the selected path to a class member.
		m_szProgramPathControlText = szPath;
	}
}

/* GetHeaderPath - Return the Value of m_szHeaderPathControlText Member Variable */
CString COptionsDlg::GetHeaderPath()
{
	return m_szHeaderPathControlText;
}

/* SetHeaderPath - Assign a Value to m_szHeaderPathControlText Member Variable */
void COptionsDlg::SetHeaderPath(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Verify the path really exists.
	if ( !pTheApp->VerifyThatFilePathExists(szPath) )
	{
		// If the path does not exist, set the path to NULL.
		m_szHeaderPathControlText = NULL_TEXT;
	}
	else
	{
		// If the path does exist, load the selected path to a class member.
		m_szHeaderPathControlText = szPath;
	}
}

/* GetSourcePath - Return the Value of m_szSourcePathControlText Member Variable */
CString COptionsDlg::GetSourcePath()
{
	return m_szSourcePathControlText;
}

/* SetSourcePath - Assign a Value to m_szSourcePathControlText Member Variable */
void COptionsDlg::SetSourcePath(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Verify the path really exists.
	if ( !pTheApp->VerifyThatFilePathExists(szPath) )
	{
		// If the path does not exist, set the path to NULL.
		m_szSourcePathControlText = NULL_TEXT;
	}
	else
	{
		// If the path does exist, load the selected path to a class member.
		m_szSourcePathControlText = szPath;
	}
}

/* GetFilesetFileName - Return the Value of m_szFilesetNameControlText Member Variable */
CString COptionsDlg::GetFilesetFileName()
{
	return m_szFilesetNameControlText;
}

/* SetFilesetFileName - Assign a Value to m_szFilesetNameControlText Member Variable */
void COptionsDlg::SetFilesetFileName(CString szFile)
{
	m_szFilesetNameControlText = szFile;
}

/* GetFilesetFilePath - Return the Value of m_szFilesetFilePathControlText Member Variable */
CString COptionsDlg::GetFilesetFilePath()
{
	return m_szFilesetFilePathControlText;
}

/* SetFilesetFilePath - Assign a Value to m_szFilesetFilePathControlText Member Variable */
void COptionsDlg::SetFilesetFilePath(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Verify the path really exists.
	if ( !pTheApp->VerifyThatFilePathExists(szPath) )
	{
		// If the path does not exist, set the path to NULL.
		m_szFilesetFilePathControlText = NULL_TEXT;
	}
	else
	{
		// If the path does exist, load the selected path to a class member.
		m_szFilesetFilePathControlText = szPath;
	}
}

/* GetViewerPath - Return the Value of m_szViewerPathControlText Member Variable */
CString COptionsDlg::GetViewerPath()
{
	return m_szViewerPathControlText;
}

/* SetViewerPath - Assign a Value to m_szViewerPathControlText Member Variable */
void COptionsDlg::SetViewerPath(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Verify the path really exists.
	if ( !pTheApp->VerifyThatFilePathExists(szPath) )
	{
		// If the path does not exist, set the path to NULL.
		m_szViewerPathControlText = NULL_TEXT;
	}
	else
	{
		// If the path does exist, load the selected path to a class member.
		m_szViewerPathControlText = szPath;
	}
}

/* GetMaxFile - Return the Value of m_nMaxFilesToAddAtOneTime Member Variable */
int COptionsDlg::GetMaxFile()
{
	return m_nMaxFilesToAddAtOneTime;
}

/* SetMaxFile - Assign a Value to m_nMaxFilesToAddAtOneTime Member Variable */
void COptionsDlg::SetMaxFile(int nMaxNumber)
{
	m_nMaxFilesToAddAtOneTime = nMaxNumber;
}


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg Message Handlers

/* OnOK - OK Button Handler */
void COptionsDlg::OnOK() 
{
	/* Local Variable Declarations and Initialization */

	BOOL bValidated = TRUE;				// Flag which Indicates if the Dialog Data Is Well Formed

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Call DDX to refresh all the form control variables with the user entered values.
	UpdateData(TRUE);


	// Verify that all form elements contain a valid value.
	if ( (m_szProgramPathControlText.IsEmpty()) || 
		 (m_szHeaderPathControlText.IsEmpty()) ||
		 (m_szSourcePathControlText.IsEmpty()) || 
		 (m_szViewerPathControlText.IsEmpty()) || 
		 (m_szFilesetNameControlText.IsEmpty()) || 
		 (m_szFilesetFilePathControlText.IsEmpty()) || 
		 (m_nMaxFilesToAddAtOneTime < UNICHECK_REG_MAXFILEFLOOR) || 
		 (m_nMaxFilesToAddAtOneTime > UNICHECK_REG_MAXFILECEILING) )
	{
		// If any element does not have a valid value, signal that the data did not validate.
		bValidated = FALSE;
	}

	// If the data validates okay, call the parent implementation of the method to complete standard functionality.
	if ( bValidated )
	{
		CDialog::OnOK();
	}
	else
	{
		// If the data did not validate, display an error to the user.
		if ( pTheApp != NULL )
		{
			pTheApp->DisplayMessageToUser(NULL, UNICHECK_MESSAGE_FTDEFXVALIDATE, NULL_TEXT, TRUE);
		}
	}
}

/* OnOptionsBrowsebtn - Browse for Program Path Button Handler */
void COptionsDlg::OnOptionsBrowsebtn() 
{
	/* Local Variable Declarations and Initialization */

	CString szSelectedPath;		// The Path Selected by the User;


	// Call DDX to preserve any modified data on the form.
	UpdateData(TRUE);

	// Prompt the user to select a new value for the path.
	szSelectedPath = BrowseForPath();

	// If the user actually selected a value, assign the value to the form control.
	if ( !szSelectedPath.IsEmpty() )
	{
		m_szProgramPathControlText = szSelectedPath;
	}

	// Call DDX to refresh the dialog controls with the new value of the class member variables.
	UpdateData(FALSE);
}

/* OnOptionsBrowsehdrbtn - Browse for Header Path Button Handler */
void COptionsDlg::OnOptionsBrowsehdrbtn() 
{
	/* Local Variable Declarations and Initialization */

	CString szSelectedPath;		// The Path Selected by the User;


	// Call DDX to preserve any modified data on the form.
	UpdateData(TRUE);


	// Display a warning to the user stating the danger of pointing this setting at 
	// a directory that is not empty.
	MessageBox(UNICHECK_MESSAGE_TEMPFLDRWARN, UNICHECK_TEXT_WARNINGTITLE, MB_OK|MB_ICONWARNING);


	// Prompt the user to select a new value for the path.
	szSelectedPath = BrowseForPath();

	// If the user actually selected a value, assign the value to the form control.
	if ( !szSelectedPath.IsEmpty() )
	{
		m_szHeaderPathControlText = szSelectedPath;
	}

	// Call DDX to refresh the dialog controls with the new value of the class member variables.
	UpdateData(FALSE);
}

/* OnOptionsBrowsesrcbtn - Browse for Source Path Button Handler */
void COptionsDlg::OnOptionsBrowsesrcbtn() 
{
	/* Local Variable Declarations and Initialization */

	CString szSelectedPath;		// The Path Selected by the User;


	// Call DDX to preserve any modified data on the form.
	UpdateData(TRUE);


	// Display a warning to the user stating the danger of pointing this setting at 
	// a directory that is not empty.
	MessageBox(UNICHECK_MESSAGE_TEMPFLDRWARN, UNICHECK_TEXT_WARNINGTITLE, MB_OK|MB_ICONWARNING);


	// Prompt the user to select a new value for the path.
	szSelectedPath = BrowseForPath();

	// If the user actually selected a value, assign the value to the form control.
	if ( !szSelectedPath.IsEmpty() )
	{
		m_szSourcePathControlText = szSelectedPath;
	}

	// Call DDX to refresh the dialog controls with the new value of the class member variables.
	UpdateData(FALSE);
}

/* OnOptionsBrowsefspbtn - Browse for Fileset File Path Button Handler */
void COptionsDlg::OnOptionsBrowsefspbtn() 
{
	/* Local Variable Declarations and Initialization */

	CString szSelectedPath;		// The Path Selected by the User;


	// Call DDX to preserve any modified data on the form.
	UpdateData(TRUE);

	// Prompt the user to select a new value for the path.
	szSelectedPath = BrowseForPath();

	// If the user actually selected a value, assign the value to the form control.
	if ( !szSelectedPath.IsEmpty() )
	{
		m_szFilesetFilePathControlText = szSelectedPath;
	}

	// Call DDX to refresh the dialog controls with the new value of the class member variables.
	UpdateData(FALSE);
}

/* OnOptionsBrowsevwrbtn - Browse for View Results Executable Button Handler */
void COptionsDlg::OnOptionsBrowsevwrbtn() 
{
	/* Local Variable Declarations and Initialization */

	int nDialogResult = IDCANCEL;	// Windows Shell Select File Dialog Return Value

	CString szViewerPath;			// Path to the Results Viewer Program Selected in the Dialog

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get references to the application instance, document, and owner.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Call DDX to preserve any modified data on the form.
	UpdateData(TRUE);

	// Set the extension filters for the "Select File" dialog.
	static char BASED_CODE szFilter[] = UNICHECK_EXECDLG_FILTER;
 
	// Create the "Select Program" dialog.
	CAddFilesDialog wndSelectProgDialog(TRUE, NULL, NULL, OFN_EXPLORER|OFN_HIDEREADONLY, szFilter, this->GetWindow(GW_OWNER));

	// Load the dialog title to the dialog instance.
	wndSelectProgDialog.m_ofn.lpstrTitle = UNICHECK_TEXT_SELVIEWER;

	// Display the "Select File" dialog.
	nDialogResult = wndSelectProgDialog.DoModal();

	// If the user selected a file and confirmed the select (pressed OK instead of Cancel), load the file.
	if (nDialogResult == IDOK)
	{
		// Get the appropriate data about the file from the dialog.
		szViewerPath = wndSelectProgDialog.GetPathName();

		// Assign the selected path to the dialog class member.
		SetViewerPath(szViewerPath);
	}

	// Call DDX to refresh the dialog controls with the new value of the class member variables.
	UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg Utility Methods

/* BrowseForPath - Select a Path Using a Dialog */
CString COptionsDlg::BrowseForPath()
{
	/* Local Variable Declarations and Initialization */

	int nResult = IDCANCEL;		// Flag Identifying How the User Exited the Dialog

	TCHAR szPath[MAX_PATH];		// Directory Path from Dialog

	CString szValidatedPath;	// The Fully Processed Path Selected by the User
	CString szCaption;			// Dialog Title

	HWND hWnd = NULL;			// Handle to the Dialog Window

	BROWSEINFO dsBI;			// Windows Shell Browse Structure

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get references to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Memset data structures.
	ZeroMemory(&dsBI, sizeof(BROWSEINFO));


	// Get a handle to the dialog window for Windows shell APIs.
	hWnd = GetSafeHwnd();

	// Verify that dialog window handle is valid before displaying the Windows Shell Select Directory dialog.
	if (hWnd != NULL)
	{
		// Set the title of the Windows Shell Select Directory dialog.
		szCaption = UNICHECK_TEXT_SELPATH;

		// Load the parameters to the Windows Shell Select Directory dialog.
		dsBI.hwndOwner = hWnd;
		dsBI.pszDisplayName = szCaption.GetBufferSetLength(szCaption.GetLength());
		dsBI.lpszTitle = szCaption.GetBufferSetLength(szCaption.GetLength());
		dsBI.ulFlags = BIF_VALIDATE;

		// Display the Windows Shell Select Directory dialog.
		LPITEMIDLIST lpItem = SHBrowseForFolder(&dsBI);	// Returns results from the dialog.

		// Convert the path to a string and obtain the dialog result (OK or Cancel).
		nResult = SHGetPathFromIDList(lpItem, szPath);

		// If the user pressed OK on the Select Directory dialog, assign the selected path to 
		// the class member variable.
		if (nResult == IDOK)
		{
			// Verify the path really exists and assign the selected, processed path to the return variable.
			if ( pTheApp != NULL )
			{
				szValidatedPath = pTheApp->ValidatePath(szPath);
			}
		}	
	}

	return szValidatedPath;
}
