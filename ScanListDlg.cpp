// ScanListDlg.cpp : Implementation File
//
/*
 * Copyright (c) 2004 Mark Johnson
 */

#include "stdafx.h"
#include "UniCheck.h"
#include "ScanListDlg.h"

#include "CLPreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CScanListDlg Dialog


/////////////////////////////////////////////////////////////////////////////
// CScanListDlg Construction/Destruction

CScanListDlg::CScanListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScanListDlg)
	//}}AFX_DATA_INIT
}

void CScanListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanListDlg)
	DDX_Control(pDX, IDC_UNICHECK_SCANLIST_PREVIEW, m_wndPreviewButton);
	DDX_Control(pDX, IDC_UNICHECK_SCANSLISTBOX, m_wndScansListBox);
	DDX_Control(pDX, IDC_UNICHECK_SCANLIST_EDITBTN, m_wndEditButton);
	DDX_Control(pDX, IDC_UNICHECK_SCANLIST_DELETEBTN, m_wndDeleteButton);
	//}}AFX_DATA_MAP
}

BOOL CScanListDlg::OnInitDialog() 
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Execute the parent class implementation of the method to complete standard behavior.
	CDialog::OnInitDialog();


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Initialize the dialog's form controls.
	ManageExistingItemControlStates();

	
	/* Load the installed scans to the list box. */

	// Load the installed scans to the list box.
	if ( pTheApp != NULL )
	{
		pTheApp->LoadScansForDisplay(&m_wndScansListBox);
	}

	// Return TRUE unless you set the focus to a control.
	// EXCEPTION: OCX Property Pages should return FALSE.
	return TRUE;
}

BEGIN_MESSAGE_MAP(CScanListDlg, CDialog)
	//{{AFX_MSG_MAP(CScanListDlg)
	ON_BN_CLICKED(IDC_UNICHECK_SCANLIST_ADDBTN, OnUnicheckScanlistAddbtn)
	ON_BN_CLICKED(IDC_UNICHECK_SCANLIST_EDITBTN, OnUnicheckScanlistEditbtn)
	ON_BN_CLICKED(IDC_UNICHECK_SCANLIST_DELETEBTN, OnUnicheckScanlistDeletebtn)
	ON_LBN_DBLCLK(IDC_UNICHECK_SCANSLISTBOX, OnDblclkUnicheckScanslistbox)
	ON_LBN_SELCHANGE(IDC_UNICHECK_SCANSLISTBOX, OnSelchangeUnicheckScanslistbox)
	ON_BN_CLICKED(IDC_UNICHECK_SCANLIST_PREVIEW, OnScanPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScanListDlg Member Access Methods


/////////////////////////////////////////////////////////////////////////////
// CScanListDlg Message Handlers

/* OnUnicheckScanlistAddbtn - Add Scan Button Handler */
void CScanListDlg::OnUnicheckScanlistAddbtn() 
{
	/* Local Variable Declarations and Initialization */

	int nResult = 0;				// Dialog Result

	CScanDefDlg wndScansDefDlg;		// The Scan Definition Dialog Box


	// Display the empty dialog.
	nResult = wndScansDefDlg.DoModal();

	// If the user pressed OK, handle the new state of the dialog.
	if (nResult == IDOK)
	{
		// If the user set or modified values on the form, install the new scan.
		ProcessNewScanDefData(&wndScansDefDlg);
	}
}

/* OnUnicheckScanlistEditbtn - Edit Scan Button Handler */
void CScanListDlg::OnUnicheckScanlistEditbtn() 
{
	/* Local Variable Declarations and Initialization */

	int nResult = 0;				// Dialog Result
	int nScanIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected List Box Item

	UINT uSelectedScanType = NULL;	// The Scan Type of the Scan that was Double Clicked by the User

	CScanDefDlg wndScanDefDlg;		// The Scan Definition Dialog Box


	/* Get the scan type of the selected list box item. */

	// Get the index of the selected item in the list box.
	nScanIndex = m_wndScansListBox.GetCurSel();

	// Only attempt to get the scan type if an item is selected.
	if ( nScanIndex != UNICHECK_RETURN_NOINDEX )
	{
		uSelectedScanType = m_wndScansListBox.GetItemData(nScanIndex);
	}

	// If the scan type is valid, load the scan type to the dialog.
	if ( uSelectedScanType != NULL )
	{
		// Load the scan data to the dialog.
		wndScanDefDlg.LoadScanFormData(uSelectedScanType);


		// Display the dialog.
		nResult = wndScanDefDlg.DoModal();

		// If the user pressed OK, handle the new state of the dialog.
		if (nResult == IDOK)
		{
			// If the user set or modified values on the form, update the scan.
			ProcessNewScanDefData(&wndScanDefDlg);
		}
	}
}

/* OnUnicheckScanlistDeletebtn - Delete Scan Button Handler */
void CScanListDlg::OnUnicheckScanlistDeletebtn() 
{
	/* Local Variable Declarations and Initialization */

	int nScanIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected List Box Item

	UINT uSelectedScanType = NULL;	// The Scan Type of the Scan that was Double Clicked by the User

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get references to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	/* Get the index and scan type of the selected list box item. */

	// Get the current index of the selected item in the scans list box.
	nScanIndex = m_wndScansListBox.GetCurSel();

	// Only attempt to get the scan type if an item is selected.
	if ( nScanIndex != UNICHECK_RETURN_NOINDEX )
	{
		uSelectedScanType = m_wndScansListBox.GetItemData(nScanIndex);
	}


	/* Uninstall the scan (i.e. remove it from the registry). */

	// Only attempt to uninstall the scan if the list box item's scan type data is valid.
	if ( uSelectedScanType != NULL )
	{
		if ( pTheApp != NULL )
		{
			// Verify the scan is properly installed.
			if ( pTheApp->GetInstalledScan(uSelectedScanType, NULL) )
			{
				// If the scan is properly installed, attempt to uninstall it.
				if ( pTheApp->UninstallScan(uSelectedScanType) )
				{
					// If the scan was uninstalled, remove it from the list box.
					m_wndScansListBox.DeleteString(nScanIndex);
				}
			}
			else
			{
				// If the scan is not properly installed, simply remove it from the list box.
				m_wndScansListBox.DeleteString(nScanIndex);
			}
		}

		// Set the list box selection to the next item in the list box.
		m_wndScansListBox.SetCurSel(nScanIndex);
	}
}

/* OnDblclkUnicheckScanslistbox - Double-Click on Scans List Box Handler */
void CScanListDlg::OnDblclkUnicheckScanslistbox() 
{
	/* Local Variable Declarations and Initialization */

	int nIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected Item in the List Box


	// Get the current index of the selected item in the scans list box.
	nIndex = m_wndScansListBox.GetCurSel();

	// If an installed scan is selected, enable the launch the edit item handler.
	if ( nIndex > UNICHECK_RETURN_NOINDEX )
	{
		// Call the corresponding form control handler.
		OnUnicheckScanlistEditbtn();
	}
}

void CScanListDlg::OnSelchangeUnicheckScanslistbox() 
{
	// Toggle the enabled states of the existing item form controls.
	ManageExistingItemControlStates();	
}

/* OnScanPreview - Display the Command Line of the Scan Based on the Scan's Current Settings */
void CScanListDlg::OnScanPreview() 
{
	/* Local Variable Declarations and Initialization */

	int nScanIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected List Box Item

	UINT uSelectedScanType = NULL;				// The Scan Type of the Scan that was Double Clicked by the User

	CCLPreviewDlg wndPreviewDialog;				// The Dialog to Display the Command Line


	/* Get the index and scan type of the selected list box item. */

	// Get the current index of the selected item in the scans list box.
	nScanIndex = m_wndScansListBox.GetCurSel();

	// Only attempt to get the scan type if an item is selected.
	if ( nScanIndex != UNICHECK_RETURN_NOINDEX )
	{
		uSelectedScanType = m_wndScansListBox.GetItemData(nScanIndex);
	}


	/* Display the command line preview dialog for the selected scan. */

	// Load the selected scan type to the preview dialog.
	wndPreviewDialog.SetScanType(uSelectedScanType);

	// Display the dialog.
	wndPreviewDialog.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CScanListDlg Generic Form Control State Handlers

/* ManageExistingItemControlStates - Generic State Handler for Form Controls Needing a Selected Item */
void CScanListDlg::ManageExistingItemControlStates()
{
	/* Local Variable Declarations and Initialization */

	int nIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected Item in the List Box

	UINT uSelectedScanType = NULL;			// The Scan Type of the Scan That is Selected


	// Reset the buttons to an initial disabled state on each change.
	// This forces the code to specifically enable appropriate controls.
	m_wndEditButton.EnableWindow(FALSE);
	m_wndDeleteButton.EnableWindow(FALSE);


	// Get the current index and scan type of the selected item in the scans list box.
	nIndex = m_wndScansListBox.GetCurSel();
	uSelectedScanType = m_wndScansListBox.GetItemData(nIndex);


	// If an installed scan is selected, enable the existing item form controls.
	if ( nIndex > UNICHECK_RETURN_NOINDEX )
	{
		m_wndEditButton.EnableWindow(TRUE);
		m_wndPreviewButton.EnableWindow(TRUE);

		// Only enable the delete button if the scan is not the pre-installed Unicode scan.
		if ( uSelectedScanType != UNICHECK_SCANTYPE_UNICODE )
		{
			m_wndDeleteButton.EnableWindow(TRUE);
		}
	}
	else
	{
		// If no scan is selected, disable the existing item form controls.
		m_wndEditButton.EnableWindow(FALSE);
		m_wndDeleteButton.EnableWindow(FALSE);
		m_wndPreviewButton.EnableWindow(FALSE);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CScanListDlg Utility Methods

/* ProcessNewScanDefData - Validate, Then Install or Update New Scan Definition Data */
void CScanListDlg::ProcessNewScanDefData(CScanDefDlg* wndScanDefDlg)
{
	/* Local Variable Declarations and Initialization */

	int nScanIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected List Box Item

	UINT uSelectedScanType = NULL;	// The Scan Type of the Scan That was Double Clicked by the User

	DSSCAN dsScan;					// The Scan to Install


	// Load the form data to a scan structure.
	wndScanDefDlg->LoadScanStructure(&dsScan);

	// If the scan type is valid, install the scan.
	if ( dsScan.uScanType != NULL )
	{
		// If the scan is valid, add (or update) the scan to the list box.
		if ( &m_wndScansListBox != NULL )
		{
			/* Check to see if a scan is currently selected in the list box. */

			// Get the selected list box item's index and scan type.
			nScanIndex = m_wndScansListBox.GetCurSel();
			if ( nScanIndex != UNICHECK_RETURN_NOINDEX )
			{
				uSelectedScanType = m_wndScansListBox.GetItemData(nScanIndex);
			}

			// If the scan type that was just modified matches the scan type of the selected list box item,
			// delete the list box item before re-adding the scan to the list box.
			if ( uSelectedScanType == dsScan.uScanType )
			{
				m_wndScansListBox.DeleteString(nScanIndex);
			}
			
			// Add the scan to scans list box.
			nScanIndex = m_wndScansListBox.AddString(dsScan.szScanTitle);
			m_wndScansListBox.SetItemData(nScanIndex, dsScan.uScanType);

			// Set the list box selection to the current item in the list box.
			m_wndScansListBox.SetCurSel(nScanIndex);
		}
	}
}
