// CLPreviewDlg.cpp : Implementation File
//

#include "stdafx.h"
#include "UniCheck.h"
#include "CLPreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCLPreviewDlg Dialog


/////////////////////////////////////////////////////////////////////////////
// CCLPreviewDlg Construction/Destruction

CCLPreviewDlg::CCLPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCLPreviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCLPreview)
	m_szCommandLineControlText = NULL_TEXT;
	//}}AFX_DATA_INIT
}


void CCLPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCLPreview)
	DDX_Text(pDX, IDC_COMMANDLINEPREVIEW_COMMANDLINEEDIT, m_szCommandLineControlText);
	//}}AFX_DATA_MAP
}

BOOL CCLPreviewDlg::OnInitDialog() 
{
	/* Local Variable Declarations and Initialization */
	
	UINT uScanType = NULL;			// The Scan To Preview

	CString szExecPath;				// The Executable Path Based on the Scan Settings
	CString szScanArgs;				// The Scan Arguments Based on the Scan Settings

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Execute the parent class implementation of the method to complete standard behavior.
	CDialog::OnInitDialog();


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Get the scan type specified to preview.
	uScanType = GetScanType();

	// If a scan type is loaded, build the command line.
	if ( uScanType != NULL )
	{
		// Build the command line.
		if ( pTheApp != NULL )
		{
			pTheApp->BuildScanCommandLine(uScanType, &szExecPath, &szScanArgs, UNICHECK_TEXT_SELFILE);
		}

		// Load the command line to the form control.
		m_szCommandLineControlText = szExecPath + SPACE + szScanArgs;

		// Call DDX to refresh the dialog form controls.
		UpdateData(FALSE);
	}

	// Return TRUE unless you set the focus to a control.
	// EXCEPTION: OCX Property Pages should return FALSE.
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCLPreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CCLPreview)
		// NOTE: The ClassWizard will add message map macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCLPreview Member Access Methods

/* GetScanType - Return the Value of m_uScanType Member Variable */
UINT CCLPreviewDlg::GetScanType()
{
	return m_uScanType;
}

/* SetScanType -  Assign a Value to m_uScanType Member Variable */
void CCLPreviewDlg::SetScanType(UINT uType)
{
	m_uScanType = uType;
}
