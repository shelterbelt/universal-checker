// CFileTypeDefDlg.cpp : Implementation File
//
/*
 * Copyright (c) 2004 Mark Johnson
 */

#include "stdafx.h"
#include "UniCheck.h"
#include "FileTypeDefDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFileTypeDefDlg Dialog


/////////////////////////////////////////////////////////////////////////////
// CFileTypeDefDlg Construction/Destruction

CFileTypeDefDlg::CFileTypeDefDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTypeDefDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileTypeDefDlg)
	m_szFileTypeControlText = _T("");
	m_szFileTypeDescriptionControlText = _T("");
	m_nGroupingRadioButton = -1;
	//}}AFX_DATA_INIT
}

void CFileTypeDefDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileTypeDefDlg)
	DDX_Text(pDX, IDC_FILETYPE_FILETYPEEDIT, m_szFileTypeControlText);
	DDX_Text(pDX, IDC_FILETYPE_FILETYPEDESCEDIT, m_szFileTypeDescriptionControlText);
	DDX_Radio(pDX, IDC_FILETYPE_SOURCEGROUPRADIO, m_nGroupingRadioButton);
	//}}AFX_DATA_MAP
}

BOOL CFileTypeDefDlg::OnInitDialog() 
{
	// Call the parent implementation of the function to execute devault processing.
	CDialog::OnInitDialog();
	

	// If the grouping radio buttons have not been set, set the grouping radio buttons to a default value.
	if ( m_nGroupingRadioButton == -1 )
	{
		// Set the radio buttons to the Source Files group.
		SetGrouping(UNICHECK_GROUPING_SOURCE);

		// Call DDX to refresh the form controls.
		UpdateData(FALSE);
	}

	
	// Return TRUE unless you set the focus to a control.
	// EXCEPTION: OCX Property Pages should return FALSE.
	return TRUE;
}

BEGIN_MESSAGE_MAP(CFileTypeDefDlg, CDialog)
	//{{AFX_MSG_MAP(CFileTypeDefDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFileTypeDefDlg Member Access Methods

/* GetFileType - Return the Value of m_szFileTypeControlText Member Variable */
CString CFileTypeDefDlg::GetFileType()
{
	/* Local Variable Declarations and Initialization */

	CString szValidatedTypeText;		// The Validated File Type Text
	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	/* Validate the file extension does not contain an extension identifier (.). */

	// Attempt to extract an extension from the control text.
	if ( pTheApp != NULL )
	{
		szValidatedTypeText = pTheApp->DetermineFileExtension(m_szFileTypeControlText);
	}

	// Verify that an extension was returned.
	if ( !szValidatedTypeText.IsEmpty() )
	{
		// If the validation function returns a value, return it as the intended extension.
		m_szFileTypeControlText = szValidatedTypeText;
	}
	else
	{
		// If no value was returned from the validation method, assume the entire string 
		// is well formed and return the entire control text.
	}

	// Force the entered text to lower case.
	m_szFileTypeControlText.MakeLower();

	return m_szFileTypeControlText;
}

/* SetFileType -  Assign a Value to m_szFileTypeControlText Member Variable */
void CFileTypeDefDlg::SetFileType(CString szType)
{
	m_szFileTypeControlText = szType;
}

/* GetFileTypeDescription - Return the Value of m_szFileTypeDescriptionControlText Member Variable */
CString CFileTypeDefDlg::GetFileTypeDescription()
{
	return m_szFileTypeDescriptionControlText;
}

/* SetFileTypeDescription -  Assign a Value to m_szFileTypeDescriptionControlText Member Variable */
void CFileTypeDefDlg::SetFileTypeDescription(CString szDescription)
{
	m_szFileTypeDescriptionControlText = szDescription;
}

/* GetGrouping - Return the Value of m_nGroupingRadioButton Member Variable */
CString CFileTypeDefDlg::GetGrouping()
{
	// Return the appropriate group depending on which radio button is checked.
	if ( m_nGroupingRadioButton == 0 )
	{
		return UNICHECK_GROUPING_SOURCE;
	}
	else 
	{
		if ( m_nGroupingRadioButton == 1 )
		{
			return UNICHECK_GROUPING_HEADER;
		}
	}

	// If no radio button selected, return NULL.
	return NULL_TEXT;
}

/* SetGrouping -  Assign a Value to m_nGroupingRadioButton Member Variable */
void CFileTypeDefDlg::SetGrouping(CString szGroup)
{
	// Check the appropriate radio button depending on the group passed.
	if ( szGroup == UNICHECK_GROUPING_SOURCE )
	{
		m_nGroupingRadioButton = 0;
	}
	else if ( szGroup == UNICHECK_GROUPING_HEADER )
	{
		m_nGroupingRadioButton = 1;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CFileTypeDefDlg Message Handlers

/* OnOK -  OK Button Handler */
void CFileTypeDefDlg::OnOK() 
{
	/* Local Variable Declarations and Initialization */

	BOOL bValidated = TRUE;				// Flag which Indicates if the Dialog Data Is Well Formed

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Call DDX to refresh all the form control variables with the user entered values.
	UpdateData(TRUE);


	// Verify that all three form elements contain a valid value.
	if ( (m_szFileTypeControlText.IsEmpty()) || 
		 (m_szFileTypeDescriptionControlText.IsEmpty()) || 
		 (m_nGroupingRadioButton < 0) || 
		 (m_nGroupingRadioButton > 1) )
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
