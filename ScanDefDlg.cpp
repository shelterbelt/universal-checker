// ScanDefDlg.cpp : Implementation File
//
/*
 * Copyright (c) 2004 Mark Johnson
 */

#include "stdafx.h"
#include "UniCheck.h"
#include "ScanDefDlg.h"

#include "AddFilesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScanDefDlg Dialog


/////////////////////////////////////////////////////////////////////////////
// CScanDefDlg Construction/Destruction

CScanDefDlg::CScanDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanDefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScanDefDlg)
	m_bCommandPromptCheckBox = FALSE;
	m_bEndSpaceCheckBox = FALSE;
	m_bSelectedFileCheckBox = FALSE;
	m_bFileSetCheckBox = FALSE;
	m_bTempFoldersCheckBox = FALSE;
	m_szScanTitleControlText = _T("");
	m_szExecutableControlText = _T("");
	m_szParameterControlText = _T("");
	m_szParameterPathControlText = _T("");
	m_szResultsFileControlText = _T("");
	m_szResultsFilePathControlText = _T("");
	m_bUseParameterPathCheckBox = FALSE;
	//}}AFX_DATA_INIT

	m_uScanType = NULL;
	m_uParameterEditMode = UNICHECK_PARAMMODE_ADD;
	m_uParamIDSeed = NULL;
}

void CScanDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanDefDlg)
	DDX_Control(pDX, IDC_SCAN_ADJPARAMUP, m_wndAdjustParamUpButton);
	DDX_Control(pDX, IDC_SCAN_ADJPARAMDOWN, m_wndAdjustParamDownButton);
	DDX_Control(pDX, IDC_SCAN_PARAMSLISTBOX, m_wndParametersListBox);
	DDX_Control(pDX, IDC_SCAN_PARAMETER, m_wndParameterEditControl);
	DDX_Control(pDX, IDC_SCAN_FILESETCHKBX, m_wndFileSetCheckBox);
	DDX_Control(pDX, IDC_SCAN_ENDSPCHKBX, m_wndEndSpaceCheckBox);
	DDX_Control(pDX, IDC_SCAN_CMDCHKBX, m_wndCommandPromptCheckBox);
	DDX_Control(pDX, IDC_SCAN_USEPARAMPATHCHKBX, m_wndUseParameterPathCheckBox);
	DDX_Control(pDX, IDC_SCAN_TEMPFLDRSCHKBX, m_wndTempFoldersCheckBox);
	DDX_Control(pDX, IDC_SCAN_SELFILECHKBX, m_wndSelectedFileCheckBox);
	DDX_Control(pDX, IDC_SCAN_BROWSEEXEBTN, m_wndBrowseExeButton);
	DDX_Control(pDX, IDC_SCAN_RESULTSFILEPATH, m_wndResultsFileEditControl);
	DDX_Control(pDX, IDC_SCAN_PARAMPATH, m_wndParameterPathEditControl);
	DDX_Control(pDX, IDC_SCAN_EXECUTABLE, m_wndExecutableEditControl);
	DDX_Control(pDX, IDC_SCAN_PARMADDUPDTBTN, m_wndAddUpdateParameterButton);
	DDX_Control(pDX, IDC_SCAN_EDITBTN, m_wndEditParameterButton);
	DDX_Control(pDX, IDC_SCAN_DELETEBTN, m_wndDeleteParameterButton);
	DDX_Control(pDX, IDC_SCAN_BROWSEPARBTN, m_wndBrowseParameterPathButton);
	DDX_Check(pDX, IDC_SCAN_CMDCHKBX, m_bCommandPromptCheckBox);
	DDX_Check(pDX, IDC_SCAN_ENDSPCHKBX, m_bEndSpaceCheckBox);
	DDX_Check(pDX, IDC_SCAN_SELFILECHKBX, m_bSelectedFileCheckBox);
	DDX_Check(pDX, IDC_SCAN_FILESETCHKBX, m_bFileSetCheckBox);
	DDX_Check(pDX, IDC_SCAN_TEMPFLDRSCHKBX, m_bTempFoldersCheckBox);
	DDX_Text(pDX, IDC_SCAN_TITLE, m_szScanTitleControlText);
	DDX_Text(pDX, IDC_SCAN_EXECUTABLE, m_szExecutableControlText);
	DDX_Text(pDX, IDC_SCAN_PARAMETER, m_szParameterControlText);
	DDX_Text(pDX, IDC_SCAN_PARAMPATH, m_szParameterPathControlText);
	DDX_Text(pDX, IDC_SCAN_RESULTSFILE, m_szResultsFileControlText);
	DDX_Text(pDX, IDC_SCAN_RESULTSFILEPATH, m_szResultsFilePathControlText);
	DDX_Check(pDX, IDC_SCAN_USEPARAMPATHCHKBX, m_bUseParameterPathCheckBox);
	//}}AFX_DATA_MAP
}

BOOL CScanDefDlg::OnInitDialog() 
{
	/* Local Variable Declarations and Initialization */

	UINT uScanType = NULL;		// Scan Type for the Scan to Display

	HICON hIcon = NULL;			// Handle to an Icon File Used to Load to Form Buttons

	// Execute the parent class implementation of the method to complete standard behavior.
	CDialog::OnInitDialog();


	/* Set the initial state of the dialog controls. */

	// Ensure the parameter path control is enabled/disabled appropriately.
	OnScanUseparampathchkbx();

	// Enable/Disable the parameter edit, delete and adjust position buttons are enable/disabled appropriately.
	OnSelchangeScanParamslistbox();

	// Set the arrow icons for the adjust parameter position buttons.
	hIcon = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_UPARROW));
	m_wndAdjustParamUpButton.SetIcon(hIcon);
	hIcon = LoadIcon(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_DOWNARROW));
	m_wndAdjustParamDownButton.SetIcon(hIcon);

	// Get the loaded scan type to determine if a new scan is being added or an existing scan is being viewed.
	uScanType = GetScanType();

	// If the Unicode scan definition is being displayed, disable all necessary form controls to 
	// prevent the user from adjusting settings she/he shouldn't.
	if ( uScanType == UNICHECK_SCANTYPE_UNICODE )
	{
		m_wndBrowseExeButton.EnableWindow(FALSE);
		m_wndBrowseParameterPathButton.EnableWindow(FALSE);
		m_wndAddUpdateParameterButton.EnableWindow(FALSE);
		m_wndDeleteParameterButton.EnableWindow(FALSE);
		m_wndEditParameterButton.EnableWindow(FALSE);
		m_wndCommandPromptCheckBox.EnableWindow(FALSE);
		m_wndEndSpaceCheckBox.EnableWindow(FALSE);
		m_wndFileSetCheckBox.EnableWindow(FALSE);
		m_wndParameterEditControl.EnableWindow(FALSE);
		m_wndParametersListBox.EnableWindow(FALSE);
		m_wndSelectedFileCheckBox.EnableWindow(FALSE);
		m_wndTempFoldersCheckBox.EnableWindow(FALSE);
		m_wndUseParameterPathCheckBox.EnableWindow(FALSE);
	}

	
	// If no scan type is loaded (i.e. a new scan is being added), generate a new scan type.
	if ( uScanType == NULL )
	{
		uScanType = GenerateScanType();

		// If the generated scan type is a valid value, load the generated scan type value to 
		// a class member.
		if ( uScanType != NULL )
		{
			SetScanType(uScanType);
		}
	}


	// Load the parameters list box for the loaded scan if an installed scan's settings are being displayed.
	LoadParametersForDisplay(&m_wndParametersListBox);


	// Return TRUE unless you set the focus to a control.
	// EXCEPTION: OCX Property Pages should return FALSE.
	return TRUE;  
}

BEGIN_MESSAGE_MAP(CScanDefDlg, CDialog)
	//{{AFX_MSG_MAP(CScanDefDlg)
	ON_BN_CLICKED(IDC_SCAN_BROWSEEXEBTN, OnScanBrowseexebtn)
	ON_BN_CLICKED(IDC_SCAN_BROWSERESBTN, OnScanBrowseresbtn)
	ON_BN_CLICKED(IDC_SCAN_BROWSEPARBTN, OnScanBrowseparbtn)
	ON_BN_CLICKED(IDC_SCAN_USEPARAMPATHCHKBX, OnScanUseparampathchkbx)
	ON_BN_CLICKED(IDC_SCAN_PARMADDUPDTBTN, OnScanParmaddupdtbtn)
	ON_BN_CLICKED(IDC_SCAN_EDITBTN, OnScanEditbtn)
	ON_BN_CLICKED(IDC_SCAN_DELETEBTN, OnScanDeletebtn)
	ON_LBN_SELCHANGE(IDC_SCAN_PARAMSLISTBOX, OnSelchangeScanParamslistbox)
	ON_BN_CLICKED(IDC_SCAN_ADJPARAMUP, OnScanAdjparamup)
	ON_BN_CLICKED(IDC_SCAN_ADJPARAMDOWN, OnScanAdjparamdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScanDefDlg Member Access Methods

/* GetParameterMode - Return the Value of m_uParameterEditMode Member Variable */
UINT CScanDefDlg::GetParameterMode()
{
	return m_uParameterEditMode;
}

/* SetParameterMode - Assign a Value to m_uParameterEditMode Member Variable */
void CScanDefDlg::SetParameterMode(UINT uMode)
{
	m_uParameterEditMode = uMode;
}

/* GetScanType - Return the Value of m_uScanType Member Variable */
UINT CScanDefDlg::GetScanType()
{
	return m_uScanType;
}

/* SetScanType - Assign a Value to m_uScanType Member Variable */
void CScanDefDlg::SetScanType(UINT uType)
{
	m_uScanType = uType;
}

/* GetScanTitle - Return the Value of m_szScanTitleControlText Member Variable */
CString CScanDefDlg::GetScanTitle()
{
	return m_szScanTitleControlText;
}

/* SetScanTitle - Assign a Value to m_szScanTitleControlText Member Variable */
void CScanDefDlg::SetScanTitle(CString szTitle)
{
	m_szScanTitleControlText = szTitle;
}

/* GetExecutable - Return the Value of m_szExecutableControlText Member Variable */
CString CScanDefDlg::GetExecutable()
{
	return m_szExecutableControlText;
}

/* SetExecutable - Assign a Value to m_szProgramPathControlText Member Variable */
void CScanDefDlg::SetExecutable(CString szExecutable)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get references to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Verify the path really exists.
	if ( pTheApp != NULL )
	{
		if ( !pTheApp->VerifyThatFilePathExists(szExecutable) )
		{
			// If the path does not exist, set the path to NULL.
			m_szExecutableControlText = NULL_TEXT;
		}
		else
		{
			// If the path does exist, load the selected path to a class member.
			m_szExecutableControlText = szExecutable;
		}
	}
}

/* GetResultsFile - Return the Value of m_szResultsFileControlText Member Variable */
CString CScanDefDlg::GetResultsFile()
{
	return m_szResultsFileControlText;
}

/* SetResultsFile - Assign a Value to m_szResultsFileControlText Member Variable */
void CScanDefDlg::SetResultsFile(CString szFile)
{
	m_szResultsFileControlText = szFile;
}

/* GetResultsFilePath - Return the Value of m_szResultsFilePathControlText Member Variable */
CString CScanDefDlg::GetResultsFilePath()
{
	return m_szResultsFilePathControlText;
}

/* SetResultsFilePath - Assign a Value to m_szResultsFilePathControlText Member Variable */
void CScanDefDlg::SetResultsFilePath(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get references to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Verify the path really exists and is well formed.
	if ( pTheApp != NULL )
	{
		m_szResultsFilePathControlText = pTheApp->ValidatePath(szPath);
	}
}

/* GetParameterID - Return the Value of m_uCurrentParameterID Member Variable */
UINT CScanDefDlg::GetParameterID()
{
	return m_uCurrentParameterID;
}

/* SetParameterID - Assign a Value to m_uCurrentParameterID Member Variable */
void CScanDefDlg::SetParameterID(UINT uID)
{
	m_uCurrentParameterID = uID;
}

/* GetParameterEditorText - Return the Value of m_szParameterControlText Member Variable */
CString CScanDefDlg::GetParameterEditorText()
{
	return m_szParameterControlText;
}

/* SetParameterEditorText - Assign a Value to m_szParameterControlText Member Variable */
void CScanDefDlg::SetParameterEditorText(CString szParameterValue)
{
	m_szParameterControlText = szParameterValue;
}

/* GetUseParameterPathCheck - Return the Value of m_bUseParameterPathCheckBox Member Variable */
BOOL CScanDefDlg::GetUseParameterPathCheck()
{
	return m_bUseParameterPathCheckBox;
}

/* SetUseParameterPathCheck - Assign a Value to m_bUseParameterPathCheckBox Member Variable */
void CScanDefDlg::SetUseParameterPathCheck(BOOL bCheckedState)
{
	m_bUseParameterPathCheckBox = bCheckedState;
}

/* GetParameterPathEditorText - Return the Value of m_szParameterPathControlText Member Variable */
CString CScanDefDlg::GetParameterPathEditorText()
{
	return m_szParameterPathControlText;
}

/* SetParameterPathEditorText - Assign a Value to m_szParameterPathControlText Member Variable */
void CScanDefDlg::SetParameterPathEditorText(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get references to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Verify the path really exists and is well formed.
	if ( pTheApp != NULL )
	{
		m_szParameterPathControlText = pTheApp->ValidatePath(szPath);
	}
}

/* GetEndSpaceCheck - Return the Value of m_bEndSpaceCheckBox Member Variable */
BOOL CScanDefDlg::GetEndSpaceCheck()
{
	return m_bEndSpaceCheckBox;
}

/* SetEndSpaceCheck - Assign a Value to m_bEndSpaceCheckBox Member Variable */
void CScanDefDlg::SetEndSpaceCheck(BOOL bCheckState)
{
	m_bEndSpaceCheckBox = bCheckState;
}

/* GetCommandPromptCheck - Return the Value of m_bCommandPromptCheckBox Member Variable */
BOOL CScanDefDlg::GetCommandPromptCheck()
{
	return m_bCommandPromptCheckBox;
}

/* SetCommandPromptCheck - Assign a Value to m_bCommandPromptCheckBox Member Variable */
void CScanDefDlg::SetCommandPromptCheck(BOOL bCheckState)
{
	m_bCommandPromptCheckBox = bCheckState;
}

/* GetFileSetCheck - Return the Value of m_bFileSetCheckBox Member Variable */
BOOL CScanDefDlg::GetFileSetCheck()
{
	return m_bFileSetCheckBox;
}

/* SetFileSetCheck - Assign a Value to m_bFileSetCheckBox Member Variable */
void CScanDefDlg::SetFileSetCheck(BOOL bCheckState)
{
	m_bFileSetCheckBox = bCheckState;
}

/* GetTempFoldersCheck - Return the Value of m_bTempFoldersCheckBox Member Variable */
BOOL CScanDefDlg::GetTempFoldersCheck()
{
	return m_bTempFoldersCheckBox;
}

/* SetTempFoldersCheck - Assign a Value to m_bTempFoldersCheckBox Member Variable */
void CScanDefDlg::SetTempFoldersCheck(BOOL bCheckState)
{
	m_bTempFoldersCheckBox = bCheckState;
}

/* GetSelectedFileCheck - Return the Value of m_bSelectedFileCheckBox Member Variable */
BOOL CScanDefDlg::GetSelectedFileCheck()
{
	return m_bSelectedFileCheckBox;
}

/* SetSelectedFileCheck - Assign a Value to m_bSelectedFileCheckBox Member Variable */
void CScanDefDlg::SetSelectedFileCheck(BOOL bCheckState)
{
	m_bSelectedFileCheckBox = bCheckState;
}

/* GetParamIDSeed - Return the Value of m_uParamIDSeed Member Variable */
UINT CScanDefDlg::GetParamIDSeed()
{
	return m_uParamIDSeed;
}

/* SetParamIDSeed - Assign a Value to m_uParamIDSeed Member Variable */
void CScanDefDlg::SetParamIDSeed(UINT uLastIDUsed)
{
	m_uParamIDSeed = uLastIDUsed;
}


/////////////////////////////////////////////////////////////////////////////
// CScanDefDlg Message Handlers

/* OnOK - OK Button Handler */
void CScanDefDlg::OnOK() 
{
	/* Local Variable Declarations and Initialization */

	BOOL bValidated = TRUE;				// Flag which Indicates if the Dialog Data Is Well Formed

	DSSCAN dsScan;						// The Scan to Install

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get references to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Call DDX to load the form data from the controls to class members.
	UpdateData(TRUE);


	/* Validate the form data. */

	// Verify that the title has a valid value.
	if ( m_szScanTitleControlText.IsEmpty() )
	{
		// If the element does not have a valid value, signal that the data did not validate.
		bValidated = FALSE;

		// If the data did not validate, display an error to the user.
		if ( pTheApp != NULL )
		{
			pTheApp->DisplayMessageToUser(NULL, UNICHECK_MESSAGE_SDEFXVALTITLE, NULL_TEXT, TRUE);
		}
	}

	// Verify that the scan engine has a valid value.
	if ( m_szExecutableControlText.IsEmpty() )
	{
		// If the element does not have a valid value, signal that the data did not validate.
		bValidated = FALSE;

		// If the data did not validate, display an error to the user.
		if ( pTheApp != NULL )
		{
			pTheApp->DisplayMessageToUser(NULL, UNICHECK_MESSAGE_SDEFXVALENG, NULL_TEXT, TRUE);
		}
	}

	// Verify that the results file has a valid value.
	if ( m_szResultsFileControlText.IsEmpty() )
	{
		// If the element does not have a valid value, signal that the data did not validate.
		bValidated = FALSE;

		// If the data did not validate, display an error to the user.
		if ( pTheApp != NULL )
		{
			pTheApp->DisplayMessageToUser(NULL, UNICHECK_MESSAGE_SDEFXVALRFILE, NULL_TEXT, TRUE);
		}
	}

	// Verify that the results file path has a valid value.
	if ( m_szResultsFilePathControlText.IsEmpty() )
	{
		// If the element does not have a valid value, signal that the data did not validate.
		bValidated = FALSE;

		// If the data did not validate, display an error to the user.
		if ( pTheApp != NULL )
		{
			pTheApp->DisplayMessageToUser(NULL, UNICHECK_MESSAGE_SDEFXVALRFPATH, NULL_TEXT, TRUE);
		}
	}


	// If the data validates okay, attempt to install the scan and call the parent implementation of 
	// the method to complete standard functionality.
	if ( bValidated )
	{
		// Load the form data to a scan structure.
		LoadScanStructure(&dsScan);

		// Install the scan.
		if ( pTheApp != NULL )
		{
			if ( !pTheApp->InstallScan(&dsScan) )
			{
				// If the scan failed to install, display an error message to the user.
				if ( pTheApp != NULL )
				{
					pTheApp->DisplayMessageToUser(NULL, UNICHECK_MESSAGE_XINSTSCAN, NULL_TEXT, TRUE);
				}
			}
		}

		// Execute the parent class implementation of the method to complete standard behavior.
		CDialog::OnOK();
	}
}

/* OnScanBrowseexebtn - Browse for Scan Executable Button Handler */
void CScanDefDlg::OnScanBrowseexebtn() 
{
	/* Local Variable Declarations and Initialization */

	CString szSelectedFile;		// The File Selected by the User;


	// Call DDX to preserve any modified data on the form.
	UpdateData(TRUE);

	// Prompt the user to select a new value for the executable.
	szSelectedFile = BrowseForFile(UNICHECK_TEXT_SELSCANEXE, UNICHECK_EXECDLG_FILTER);

	// If the user actually selected a value, assign the value to the form control.
	if ( !szSelectedFile.IsEmpty() )
	{
		// Assign the selected value selected by the user to form control.
		m_szExecutableControlText = szSelectedFile;
	}

	// Call DDX to update the new parameter values to the edit form controls.
	UpdateData(FALSE);
}

/* OnScanBrowseresbtn - Browse for Results File Path Button Handler */
void CScanDefDlg::OnScanBrowseresbtn() 
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
		// Assign the selected value selected by the user to form control.
		m_szResultsFilePathControlText = szSelectedPath;
	}

	// Call DDX to update the new parameter values to the edit form controls.
	UpdateData(FALSE);
}

/* OnScanBrowseparbtn - Browse for Parameter Path Button Handler */
void CScanDefDlg::OnScanBrowseparbtn() 
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
		// Assign the selected value selected by the user to form control.
		m_szParameterPathControlText = szSelectedPath;
	}

	// Call DDX to update the new parameter values to the edit form controls.
	UpdateData(FALSE);
}

/* OnScanUseparampathchkbx - Use Parameter Path Check Box Handler */
void CScanDefDlg::OnScanUseparampathchkbx() 
{
	// Determine the state of the check box.
	if ( m_wndUseParameterPathCheckBox.GetCheck() == BST_CHECKED )
	{
		/* The check box is selected. */

		// Enable the browse button for the parameter path value.
		m_wndBrowseParameterPathButton.EnableWindow(TRUE);
	}
	else
	{
		/* The check box is not selected. */

		// Call DDX to preserve any modified data on the form.
		UpdateData(TRUE);

		// Clear the parameter path edit control of any value.
		m_szParameterPathControlText.Empty();

		// Call DDX to update the new parameter values to the edit form controls.
		UpdateData(FALSE);

		// Disable the browse button for the parameter path value.
		m_wndBrowseParameterPathButton.EnableWindow(FALSE);
	}
}

/* OnSelchangeScanParamslistbox - Change Selection in Parameters List Box Handler */
void CScanDefDlg::OnSelchangeScanParamslistbox() 
{
	/* Local Variable Declarations and Initialization */

	int nParamIndex = UNICHECK_RETURN_NOINDEX;		// The Index of an Item in the List Box


	/* Determine the scan to delete. */

	// Get the index of the item being deleted.
	nParamIndex = m_wndParametersListBox.GetCurSel();
	
	// Verify that a scan is selected.
	if ( nParamIndex > UNICHECK_RETURN_NOINDEX )
	{
		// If a parameter is selected, enable the edit, delete, and adjust parameter buttons.
		m_wndEditParameterButton.EnableWindow(TRUE);
		m_wndDeleteParameterButton.EnableWindow(TRUE);

		// Only update the adjust up botton if the selected parameter is not the top item in the list.
		if ( nParamIndex == 0)
		{
			m_wndAdjustParamUpButton.EnableWindow(FALSE);
		}
		else
		{
			m_wndAdjustParamUpButton.EnableWindow(TRUE);
		}

		// Only update the adjust up botton if the selected parameter is not the bottom item in the list.
		if ( nParamIndex == (m_wndParametersListBox.GetCount() - 1) )
		{
			m_wndAdjustParamDownButton.EnableWindow(FALSE);
		}
		else
		{
			m_wndAdjustParamDownButton.EnableWindow(TRUE);
		}

	}
	else
	{
		// If no parameter is selected, disable the edit, delete, and adjust parameter buttons.
		m_wndEditParameterButton.EnableWindow(FALSE);
		m_wndDeleteParameterButton.EnableWindow(FALSE);
		m_wndAdjustParamUpButton.EnableWindow(FALSE);
		m_wndAdjustParamDownButton.EnableWindow(FALSE);
	}


	/* Toggle the parameters edits out of Update mode. */
	
	// Call DDX to preserve any data entered into non-parameters form controls.
	UpdateData(TRUE);

	// Clear the parameters edits of any values.
	SetParameterEditorText(NULL_TEXT);
	SetParameterPathEditorText(NULL_TEXT);
	SetUseParameterPathCheck(FALSE);
	SetEndSpaceCheck(FALSE);
	SetParameterID(NULL);

	// Call DDX to refresh the form controls with the cleared data.
	UpdateData(FALSE);

	// Reset the form to add mode and update the Add/Update button text.
	SetParameterMode(UNICHECK_PARAMMODE_ADD);
	m_wndAddUpdateParameterButton.SetWindowText(UNICHECK_TEXT_ADDPARAMBTN);
}

/* OnScanParmaddupdtbtn - Add/Update Button Handler */
void CScanDefDlg::OnScanParmaddupdtbtn() 
{
	/* Local Variable Declarations and Initialization */

	int nIndex = UNICHECK_RETURN_NOINDEX;				// The Index of the Parameter in the Parameters List Box

	UINT uParameterEditMode = UNICHECK_PARAMMODE_ADD;	// Flag Specifying whether the Values in the Parameter Edits should be Added or Updated

	BOOL bValidated = TRUE;								// Flag which Indicates if the Dialog Data Is Well Formed

	POSITION parameterPosition = NULL;					// Position of the Parameter in the Parameters List Box

	DSPARAMETER dsParam;								// Structure Holding the Attributes of the Scan Parameter

	CUniCheckApp* pTheApp = NULL;						// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Clear the parameter structure to ensure clean data.
	if ( pTheApp != NULL )
	{
		pTheApp->ClearParameterStructure(&dsParam);
	}


	// Call DDX to populate the form control values to the associated variables.
	UpdateData(TRUE);


	/* Validate the form data. */

	// Verify that the parameter value field has a valid value.
	if ( m_szParameterControlText.IsEmpty() )
	{
		// If the element does not have a valid value, signal that the data did not validate.
		bValidated = FALSE;

		// If the data did not validate, display an error to the user.
		if ( pTheApp != NULL )
		{
			pTheApp->DisplayMessageToUser(NULL, UNICHECK_MESSAGE_SDEFXVALPARAM, NULL_TEXT, TRUE);
		}
	}

	// If the Uses Parameter Path flag is set, verify that the parameter value field has a valid value.
	if ( m_bUseParameterPathCheckBox )
	{
		if ( m_szParameterPathControlText.IsEmpty() )
		{
			// If the element does not have a valid value, signal that the data did not validate.
			bValidated = FALSE;

			// If the data did not validate, display an error to the user.
			if ( pTheApp != NULL )
			{
				pTheApp->DisplayMessageToUser(NULL, UNICHECK_MESSAGE_SDEFXVALPPATH, NULL_TEXT, TRUE);
			}
		}
	}


	// If the data validates okay, add the parameter to the scan data.
	if ( bValidated )
	{
		// Determine the parameter editing mode.
		uParameterEditMode = GetParameterMode();

		switch ( uParameterEditMode )
		{
		// The parameter edits values should be added as a new parameter.
		case UNICHECK_PARAMMODE_ADD:

			// Load the parameter edit values to a parameter structure.
			dsParam.uParameterID = GenerateParameterID();
			dsParam.szParameter = GetParameterEditorText();
			dsParam.szParameterPath = GetParameterPathEditorText();
			dsParam.bUsesParameterPath = GetUseParameterPathCheck();
			dsParam.bAppendEndSpace = GetEndSpaceCheck();

			// Add the parameter structure to the parameters list.
			m_ParameterList.AddTail(dsParam);

			// Add the parameter to the list box.
			nIndex = m_wndParametersListBox.AddString(dsParam.szParameter);

			// If the parameter was added to the list box, load the ID to the item data.
			if ( nIndex > UNICHECK_RETURN_NOINDEX )
			{
				m_wndParametersListBox.SetItemData(nIndex, dsParam.uParameterID);
			}

			break;

		// The parameter edits values should be used to update an existing parameter.
		case UNICHECK_PARAMMODE_EDIT:

			// Load the parameter edit values to a parameter structure.
			dsParam.uParameterID = GetParameterID();
			dsParam.szParameter = GetParameterEditorText();
			dsParam.szParameterPath = GetParameterPathEditorText();
			dsParam.bUsesParameterPath = GetUseParameterPathCheck();
			dsParam.bAppendEndSpace = GetEndSpaceCheck();


			// Get the index of the selected parameter in the list box.
			nIndex = m_wndParametersListBox.GetCurSel();

			// Update the data in the link list.
			parameterPosition = m_ParameterList.FindIndex(nIndex);
			if ( parameterPosition != NULL)
			{
				m_ParameterList.SetAt(parameterPosition, dsParam);
			}


			/* Update the data in the list box. */

			// If a parameter is selected in the list box, refresh the data for that item.
			if ( nIndex > UNICHECK_RETURN_NOINDEX )
			{
				m_wndParametersListBox.DeleteString(nIndex);
				m_wndParametersListBox.InsertString(nIndex, dsParam.szParameter);
				m_wndParametersListBox.SetItemData(nIndex, dsParam.uParameterID);
			}
			

			// Reset the Add/Update button to 'Add' mode.
			m_wndAddUpdateParameterButton.SetWindowText(UNICHECK_TEXT_ADDPARAMBTN);

			// Enable the adjust parameters mode.
			OnSelchangeScanParamslistbox();

			// Set the parameter edit to add mode.
			SetParameterMode(UNICHECK_PARAMMODE_ADD);

			break;

		// Default handler.
		default:

			break;
		}

		// Clear the parameter edits once the button has finished processing.
		SetParameterEditorText(NULL_TEXT);
		SetParameterPathEditorText(NULL_TEXT);
		SetEndSpaceCheck(FALSE);
		SetUseParameterPathCheck(FALSE);
	}


	// Call DDX to update the form with the appropritate changes.
	UpdateData(FALSE);
}

/* OnScanEditbtn - Edit Parameter Button Handler */
void CScanDefDlg::OnScanEditbtn() 
{
	/* Local Variable Declarations and Initialization */

	int nIndex = UNICHECK_RETURN_NOINDEX;	// The Index of the Parameter in the Parameters List Box

	POSITION parameterPosition = NULL;		// Position of the Parameter in the Parameters List Box

	DSPARAMETER dsParam;					// Structure Holding the Attributes of the Scan Parameter

	CUniCheckApp* pTheApp = NULL;					// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Clear the parameter structure to ensure clean data.
	if ( pTheApp != NULL )
	{
		pTheApp->ClearParameterStructure(&dsParam);
	}


	/* Change the parameter edits to update mode. */

	// Change the label of the Add/Update button to signify that the parameter edits are in update mode.
	m_wndAddUpdateParameterButton.SetWindowText(UNICHECK_TEXT_UPDPARAMBTN);

	// Disable the edit, delete, and adjust parameters buttons in update mode.
	m_wndEditParameterButton.EnableWindow(FALSE);
	m_wndDeleteParameterButton.EnableWindow(FALSE);
	m_wndAdjustParamUpButton.EnableWindow(FALSE);
	m_wndAdjustParamDownButton.EnableWindow(FALSE);

	// Set the parameter edits to update mode.
	SetParameterMode(UNICHECK_PARAMMODE_EDIT);


	/* Get the parameter attributes. */

	// Get the index of the current selection in the list box.
	nIndex = m_wndParametersListBox.GetCurSel();

	// Get the parameter from the dialog link list.
	parameterPosition = m_ParameterList.FindIndex(nIndex);
	if ( parameterPosition != NULL )
	{
		dsParam = m_ParameterList.GetAt(parameterPosition);
	}


	/* Load the parameter attributes to the parameter edits. */

	// Call DDX to preserve any modified data on the form.
	UpdateData(TRUE);

	// Load the appropriate form controls with the appropriate element of the parameter.
	SetParameterID(dsParam.uParameterID);
	SetParameterEditorText(dsParam.szParameter);
	SetParameterPathEditorText(dsParam.szParameterPath);
	SetUseParameterPathCheck(dsParam.bUsesParameterPath);
	SetEndSpaceCheck(dsParam.bAppendEndSpace);

	// Call DDX to update the new parameter values to the edit form controls.
	UpdateData(FALSE);
}

/* OnScanDeletebtn - Delete Parameter Button Handler */
void CScanDefDlg::OnScanDeletebtn() 
{
	/* Local Variable Declarations and Initialization */

	int nParamIndex = UNICHECK_RETURN_NOINDEX;		// The Index of an Item in the List Box

	POSITION parameterPosition = NULL;				// The Position of the Current Parameter in the Parameter List


	/* Determine the scan to delete. */

	// Get the index of the item being deleted.
	nParamIndex = m_wndParametersListBox.GetCurSel();
	
	// Verify that a scan is selected.
	if ( nParamIndex > UNICHECK_RETURN_NOINDEX )
	{
		/* If a valid parameter is selected, delete the string from the list box and remove the parameter from the linked list. */

		// Get a reference to the parameter to delete.
		parameterPosition = m_ParameterList.FindIndex(nParamIndex);

		// Remove the parameter from the dialog list.
		if ( parameterPosition != NULL )
		{
			m_ParameterList.RemoveAt(parameterPosition);
		}

		// Delete the parameter from the list box.
		m_wndParametersListBox.DeleteString(nParamIndex);
	}
}

/* OnScanAdjparamup - Adjust Parameter Up Button Handler */
void CScanDefDlg::OnScanAdjparamup() 
{
	/* Local Variable Declarations and Initialization */

	int nParamIndex = UNICHECK_RETURN_NOINDEX;	// The Index of an Item in the List Box

	POSITION parameterPosition = NULL;			// The Position of the Current Parameter in the Parameter List
	POSITION predecessorPosition = NULL;		// The Position of the Preceding Parameter in the Parameter List
	POSITION newPosition = NULL;				// The Position of the Parameter in Its New Location in the Parameter List

	DSPARAMETER dsParam;						// The Scan Parameter to Move within the List Box and Dialog Data

	CUniCheckApp* pTheApp = NULL;				// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Clear the parameter structure to ensure clean data.
	if ( pTheApp != NULL )
	{
		pTheApp->ClearParameterStructure(&dsParam);
	}


	/* Determine the scan to move up. */

	// Get the index of the item being deleted.
	nParamIndex = m_wndParametersListBox.GetCurSel();
	
	// Verify that a scan is selected.
	if ( nParamIndex > UNICHECK_RETURN_NOINDEX )
	{
		/* If a valid parameter is selected, move the string up in the list box and swap the parameter and 
		   its predecessor in the linked list. */

		// Get the parameter to move up.
		parameterPosition = m_ParameterList.FindIndex(nParamIndex);
		if ( parameterPosition != NULL )
		{
			dsParam = m_ParameterList.GetAt(parameterPosition);
		}


		/* Swap the parameters in the link list. */

		// Get a reference to the predecessor parameter in the list.
		predecessorPosition = m_ParameterList.FindIndex(nParamIndex - 1);

		// Insert the parameter before the predecessor position.
		if ( predecessorPosition != NULL )
		{
			newPosition = m_ParameterList.InsertBefore(predecessorPosition, dsParam);

			// Only complete the swap if the item was successfully added.
			if ( newPosition != NULL )
			{
				// Remove the parameter from the original location in the list.
				if ( parameterPosition != NULL )
				{
					m_ParameterList.RemoveAt(parameterPosition);
				}

				// Move the parameter up in the list box.
				m_wndParametersListBox.DeleteString(nParamIndex);
				m_wndParametersListBox.InsertString(nParamIndex-1, dsParam.szParameter);
				m_wndParametersListBox.SetItemData(nParamIndex-1, dsParam.uParameterID);

				// Set the current selection on the moved item and enable/disable the appropriate adjust position buttons.
				m_wndParametersListBox.SetCurSel(nParamIndex-1);
				OnSelchangeScanParamslistbox();
			}
		}
	}
}

/* OnScanAdjparamdown - Adjust Parameter Down Button Handler */
void CScanDefDlg::OnScanAdjparamdown() 
{
	/* Local Variable Declarations and Initialization */

	int nParamIndex = UNICHECK_RETURN_NOINDEX;	// The Index of an Item in the List Box

	POSITION parameterPosition = NULL;			// The Position of the Current Parameter in the Parameter List
	POSITION successorPosition = NULL;			// The Position of the Subsequent Parameter in the Parameter List
	POSITION newPosition = NULL;				// The Position of the Parameter in Its New Location in the Parameter List

	DSPARAMETER dsParam;						// The Scan Parameter to Move within the List Box and Dialog Data

	CUniCheckApp* pTheApp = NULL;				// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Clear the parameter structure to ensure clean data.
	if ( pTheApp != NULL )
	{
		pTheApp->ClearParameterStructure(&dsParam);
	}


	/* Determine the scan to move down. */

	// Get the index of the item being deleted.
	nParamIndex = m_wndParametersListBox.GetCurSel();
	
	// Verify that a scan is selected.
	if ( nParamIndex > UNICHECK_RETURN_NOINDEX )
	{
		/* If a valid parameter is selected, move the string down in the list box and swap the parameter and 
		   its successor in the linked list. */

		// Get the parameter to move down.
		parameterPosition = m_ParameterList.FindIndex(nParamIndex);
		if ( parameterPosition != NULL )
		{
			dsParam = m_ParameterList.GetAt(parameterPosition);
		}


		/* Swap the parameters in the link list. */

		// Get a reference to the successor parameter in the list.
		successorPosition = m_ParameterList.FindIndex(nParamIndex + 1);

		// Insert the parameter after the successor position.
		if ( successorPosition != NULL )
		{
			newPosition = m_ParameterList.InsertAfter(successorPosition, dsParam);

			// Only complete the swap if the item was successfully added.
			if ( newPosition != NULL )
			{
				// Remove the parameter from the original location in the list.
				if ( parameterPosition != NULL )
				{
					m_ParameterList.RemoveAt(parameterPosition);
				}

				// Move the parameter down in the list box.
				m_wndParametersListBox.DeleteString(nParamIndex);
				m_wndParametersListBox.InsertString(nParamIndex+1, dsParam.szParameter);
				m_wndParametersListBox.SetItemData(nParamIndex+1, dsParam.uParameterID);

				// Set the current selection on the moved item and enable/disable the appropriate adjust position buttons.
				m_wndParametersListBox.SetCurSel(nParamIndex+1);
				OnSelchangeScanParamslistbox();
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CScanDefDlg Utility Methods

/* BrowseForPath - Select a Path Using a Dialog */
CString CScanDefDlg::BrowseForPath()
{
	/* Local Variable Declarations and Initialization */

	int nResult = IDCANCEL;				// Flag Identifying How the User Exited the Dialog

	TCHAR szPath[MAX_PATH];				// Directory Path from Dialog

	CString szValidatedPath;			// The Fully Processed Path Selected by the User
	CString szCaption;					// Dialog Title

	HWND hWnd = NULL;					// Handle to the Dialog Window

	BROWSEINFO dsBI;					// Windows Shell Browse Structure

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
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

/* BrowseForFile - Select a File Using a Dialog */
CString CScanDefDlg::BrowseForFile(CString szDialogTitle, char BASED_CODE szFilter[])
{
	/* Local Variable Declarations and Initialization */

	int nDialogResult = IDCANCEL;	// Windows Shell Select File Dialog Return Value

	CString szValidatedFilePath;	// The Fully Processed File Path Selected by the User
	CString szFilePath;				// Path to the File Selected in the Dialog


	// Create the "Select Program" dialog.
	CAddFilesDialog wndSelectFileDialog(TRUE, NULL, NULL, OFN_EXPLORER|OFN_HIDEREADONLY, szFilter, this->GetWindow(GW_OWNER));

	// Load the dialog title to the dialog instance.
	wndSelectFileDialog.m_ofn.lpstrTitle = szDialogTitle;

	// Display the "Select File" dialog.
	nDialogResult = wndSelectFileDialog.DoModal();

	// If the user selected a file and confirmed the select (pressed OK instead of Cancel), load the file.
	if (nDialogResult == IDOK)
	{
		// Get the appropriate data about the file from the dialog.
		szFilePath = wndSelectFileDialog.GetPathName();

		// Assign the selected path to the class member variable.
		szValidatedFilePath = szFilePath;
	}

	return szValidatedFilePath;
}

/* LoadScanFormData - Load a Scan Structure to Form Controls */
void CScanDefDlg::LoadScanFormData(UINT uScanType)
{
	/* Local Variable Declarations and Initialization */

	DSSCAN dsScan;						// The Scan to Load to the Form

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Set the scan structure to a fresh state.
	if ( pTheApp != NULL )
	{
		if ( pTheApp->GetInstalledScan(uScanType, &dsScan) )
		{
			// If the scan was fetched successfully, load the scan structure to form controls.
			SetScanType(dsScan.uScanType);
			SetScanTitle(dsScan.szScanTitle);
			SetExecutable(dsScan.szScanEngine);
			SetResultsFile(dsScan.szResultsFile);
			SetResultsFilePath(dsScan.szResultsFilePath);
			SetTempFoldersCheck(dsScan.bUsesTempFolders);
			SetFileSetCheck(dsScan.bUsesFileSet);
			SetCommandPromptCheck(dsScan.bUsesCommandPrompt);
			SetSelectedFileCheck(dsScan.bUsesSelectedFile);

			m_ParameterList.AddTail(&dsScan.ParameterList);
		}
	}
}

/* LoadScanStructure - Load the Form Data to a Scan Structure */
void CScanDefDlg::LoadScanStructure(DSSCAN *dsScan)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Set the scan structure to a fresh state.
	if ( pTheApp != NULL )
	{
		pTheApp->ClearScanStructure(dsScan);
	}

	// Load the form data to a scan structure.
	dsScan->uScanType = GetScanType();
	dsScan->szScanTitle = GetScanTitle();
	dsScan->szScanEngine = GetExecutable();
	dsScan->szResultsFile = GetResultsFile();
	dsScan->szResultsFilePath = GetResultsFilePath();
	dsScan->bUsesTempFolders = GetTempFoldersCheck();
	dsScan->bUsesFileSet = GetFileSetCheck();
	dsScan->bUsesCommandPrompt = GetCommandPromptCheck();
	dsScan->bUsesSelectedFile = GetSelectedFileCheck();

	dsScan->ParameterList.AddTail(&m_ParameterList);
}

/* GenerateScanType - Generate a New Unique Identifier for a Scan */
UINT CScanDefDlg::GenerateScanType()
{
	/* Local Variable Declarations and Initialization */

	UINT uScanType = NULL;				// The Unique Scan Type to Generate for a New Scan

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Retrieve the last scan type loaded by the application.
	if ( pTheApp != NULL )
	{
		uScanType = pTheApp->GetScanTypeSeed();
	}

	// If no scans have been loaded by the application, set the new scan type to a value several values below the base scan type to ensure
	// no conflicts with any possible scan type that may exist in the registry but was not properly loaded.
	if ( uScanType == NULL )
	{
		// This condition should never be encountered.  The application should install the Unicode scan when the application first runs and
		// no installed scans are found.  
		// This condition should never be reached and is provided simply as a "catch-all" attempt to insure the 
		// application does not overwrite installed scans if the application initiallized incorrectly.
		uScanType = UNICHECK_SCANTYPE_ALLSCANS - 10;
	}
	else
	{
		/* If installed scans were found, incrememt the scan seed by one to determine the new scan type and reset the seed value to the 
		   value of the newly generated value. */

		// Generate the new scan type;
		uScanType++;
		
		// Reset the scan type seed value to reflect the newly generated value.
		if ( pTheApp != NULL )
		{
			pTheApp->SetScanTypeSeed(uScanType);
		}
	}

	// Return the newly generated scan type value.
	return uScanType;
}

/* GenerateParameterID - Generate a New Unique Identifier for a Scan Parameter */
UINT CScanDefDlg::GenerateParameterID()
{
	/* Local Variable Declarations and Initialization */

	UINT uParamID = NULL;	// The Unique Parameter Identifier to Generate for a New Scan Parameter


	// Retrieve the last ID loaded for this scan.
	uParamID = GetParamIDSeed();


	/* Incrememt the ID seed by one to determine the new parameter ID and reset the ID value to the 
	   value of the newly generated ID. */

	// Generate the new parameter ID;
	uParamID++;
	
	// Reset the parameter ID seed value to reflect the newly generated value.
	SetParamIDSeed(uParamID);

	// Return the newly generated parameter ID value.
	return uParamID;
}

/* LoadParametersForDisplay - Load All Installed Scans to a List Box User Interface Control */
void CScanDefDlg::LoadParametersForDisplay(CListBox *pParamsListBox)
{
	/* Local Variable Declarations and Initialization */

	int nParamIndex = UNICHECK_RETURN_NOINDEX;		// The Index of an Item in the List Box
	int nNumOfParameters = 0;						// The Number of Parameters Associated with the Current Scan

	UINT uLastParamID = NULL;						// The Largest Parameter ID Value of the Retrieved Parameters

	POSITION parameterPosition = NULL;				// The Position of the Current Parameter in the Parameter List

	DSPARAMETER dsParam;							// The Attribute of a Parameter

	CUniCheckApp* pTheApp = NULL;					// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Only attempt to load the parameters if the list box control exists.
	if ( pParamsListBox != NULL )
	{
		// Clear the list box of any previous scans.
		if ( pParamsListBox != NULL )
		{
			pParamsListBox->ResetContent();
		}


		// Get the position of the initial scan in the dialog's list.
		parameterPosition = m_ParameterList.GetHeadPosition();


		// Get the number of parameters in the dialog's parameter list.  This list should be populated before this method is called.
		nNumOfParameters = m_ParameterList.GetCount();

		// Cycle through each parameter associated with the scan.
		for ( int i = 0; i < nNumOfParameters; i++ )
		{
			// Clear the parameter structure to ensure clean data.
			if ( pTheApp != NULL )
			{
				pTheApp->ClearParameterStructure(&dsParam);
			}

			// Get the next parameter from the dialog list.
			if ( parameterPosition != NULL )
			{
				dsParam = m_ParameterList.GetNext(parameterPosition);

				nParamIndex = pParamsListBox->AddString(dsParam.szParameter);
				if ( nParamIndex > UNICHECK_RETURN_NOINDEX )
				{
					pParamsListBox->SetItemData(nParamIndex, dsParam.uParameterID);
				}

				// Determine the largest scan type value retrieved from the registry.
				if ( dsParam.uParameterID > uLastParamID )
				{
					uLastParamID = dsParam.uParameterID;
				}
			}
		}

		// Store value of the greatest scan type read to use as the seed for the scan type generator.
		SetParamIDSeed(uLastParamID);
	}
}
