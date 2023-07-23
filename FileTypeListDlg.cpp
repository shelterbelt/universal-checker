// FileTypeListDlg.cpp : Implementation File
//
/*
 * Copyright (c) 2004 Mark Johnson
 */

#include "stdafx.h"
#include "UniCheck.h"
#include "FileTypeListDlg.h"

#include "FileTypeDefDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFileTypeListDlg Dialog


/////////////////////////////////////////////////////////////////////////////
// CFileTypeListDlg Construction/Destruction

CFileTypeListDlg::CFileTypeListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTypeListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileTypeListDlg)
	//}}AFX_DATA_INIT
}

void CFileTypeListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileTypeListDlg)
	DDX_Control(pDX, IDC_FILETYPELIST_TYPELISTBOX, m_wndFileTypesListBox);
	DDX_Control(pDX, IDC_FILETYPELIST_DELETEBUTTON, m_wndDeleteButton);
	DDX_Control(pDX, IDC_FILETYPELIST_EDITBUTTON, m_wndEditButton);
	//}}AFX_DATA_MAP
}

BOOL CFileTypeListDlg::OnInitDialog() 
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Call the parent implementation of the function to execute devault processing.
	CDialog::OnInitDialog();

	
	// Toggle the enabled states of the existing item form controls.
	ManageExistingItemControlStates();

	// Load all the valid file types to the file type list box.
	if ( pTheApp != NULL )
	{
		pTheApp->LoadAllValidFileTypes(&m_wndFileTypesListBox, NULL);
	}
	
	// Return TRUE unless you set the focus to a control.
	// EXCEPTION: OCX Property Pages should return FALSE.
	return TRUE;
}

BEGIN_MESSAGE_MAP(CFileTypeListDlg, CDialog)
	//{{AFX_MSG_MAP(CFileTypeListDlg)
	ON_LBN_DBLCLK(IDC_FILETYPELIST_TYPELISTBOX, OnDblclkFiletypelistTypelistbox)
	ON_LBN_SELCHANGE(IDC_FILETYPELIST_TYPELISTBOX, OnSelchangeFiletypelistTypelistbox)
	ON_BN_CLICKED(IDC_FILETYPELIST_ADDBUTTON, OnFiletypelistAddbutton)
	ON_BN_CLICKED(IDC_FILETYPELIST_EDITBUTTON, OnFiletypelistEditbutton)
	ON_BN_CLICKED(IDC_FILETYPELIST_DELETEBUTTON, OnFiletypelistDeletebutton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFileTypeListDlg Message Handlers

/* OnFiletypelistAddbutton - Add File Type Button Handler */
void CFileTypeListDlg::OnFiletypelistAddbutton() 
{
	/* Local Variable Declarations and Initialization */

	int nResult = 0;					// Dialog Result

	CString szFileType;					// The File Type Specified by the User
	CString szFileTypeDescription;		// The File Type Description Entered by the User
	CString szFileTypeGrouping;			// The Group Associated with the File Type Entered by the User

	CFileTypeDefDlg wndFileTypeDlg;		// The File Type Definition Dialog Box

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	/* Prompt the user from new values. */

	// Display the file type dialog.
	nResult = wndFileTypeDlg.DoModal();

	// If the user pressed OK, handle the new state of the dialog.
	if (nResult == IDOK)
	{
		// Get the file type data from the dialog.
		szFileType = wndFileTypeDlg.GetFileType();
		szFileTypeDescription = wndFileTypeDlg.GetFileTypeDescription();
		szFileTypeGrouping = wndFileTypeDlg.GetGrouping();

		// Add the file type to the list box and the registry if a type and description are both entered.
		if ( !szFileType.IsEmpty() && !szFileTypeDescription.IsEmpty() )
		{
			// Load the file type to the registry.
			if ( pTheApp != NULL )
			{
				if ( pTheApp->InstallValidType(szFileType, szFileTypeDescription, szFileTypeGrouping) )
				{
					// If the file type was installed succesfully, add the file type to the list box.
					m_wndFileTypesListBox.AddString(szFileType);
				}
				else
				{
					// If the file type failed to install, display an error to the user.
					if ( pTheApp != NULL )
					{
						pTheApp->DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_FTYPE + szFileType + UNICHECK_MESSAGE_XINSTALLED),
													  NULL_TEXT, TRUE);
					}
				}
			}
		}
	}
}

/* OnFiletypelistEditbutton - Edit File Type Button Handler */
void CFileTypeListDlg::OnFiletypelistEditbutton() 
{
	/* Local Variable Declarations and Initialization */

	int nResult = 0;					// Dialog Result
	int nTypeIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected List Box Item

	CString szSelectedFileType;			// The File Type Selected in the List Box
	CString szSelectedTypeDescription;	// The Description of the File Type Selected in the List Box
	CString szSelectedGrouping;			// The Group Associated with the File Type Selected in the List Box

	CFileTypeDefDlg wndFileTypeDlg;		// The File Type Definition Dialog Box

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	/* Get the scan type of the selected list box item. */

	// Get the index of the selected item in the list box.
	nTypeIndex = m_wndFileTypesListBox.GetCurSel();

	// Only load the file type if an item is selected.
	if ( nTypeIndex != UNICHECK_RETURN_NOINDEX )
	{
		/* Load the file type to the dialog. */

		// Get the file type text from the list box.
		m_wndFileTypesListBox.GetText(nTypeIndex, szSelectedFileType);

		// Get the file type description from the registry.
		if ( pTheApp != NULL )
		{
			pTheApp->GetValidFileType(szSelectedFileType, &szSelectedTypeDescription, &szSelectedGrouping);
		}

		// Load the file type attributes to the dialog.
		wndFileTypeDlg.SetFileType(szSelectedFileType);
		wndFileTypeDlg.SetFileTypeDescription(szSelectedTypeDescription);
		wndFileTypeDlg.SetGrouping(szSelectedGrouping);

		
		/* Prompt the user from new values. */

		// Display the file type dialog.
		nResult = wndFileTypeDlg.DoModal();

		// If the user pressed OK, handle the new state of the dialog.
		if (nResult == IDOK)
		{
			/* Remove the original file type references. */

			// Delete the file type from the registry.
			if ( pTheApp != NULL )
			{
				if ( pTheApp->UninstallValidType(szSelectedFileType) )
				{
					// If the file type was uninstalled succesfully, delete the file type to the list box.
					m_wndFileTypesListBox.DeleteString(nTypeIndex);
				}
				else
				{
					// If the file type failed to uninstall, display an error to the user.
					if ( pTheApp != NULL )
					{
						pTheApp->DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_FTYPE + szSelectedFileType + UNICHECK_MESSAGE_XUNINSTALLED),
													  NULL_TEXT, TRUE);
					}

					// Reset the list box selection to the current item.
					m_wndFileTypesListBox.SetCurSel(nTypeIndex);

					// Exit without attempting to install the new scan settings.
					return;
				}
			}


			/* Reinstall the new settings. */

			// Get the updated values from the dialog.
			szSelectedFileType = wndFileTypeDlg.GetFileType();
			szSelectedTypeDescription = wndFileTypeDlg.GetFileTypeDescription();
			szSelectedGrouping = wndFileTypeDlg.GetGrouping();

			// Load the file type to the registry.
			if ( pTheApp != NULL )
			{
				if ( pTheApp->InstallValidType(szSelectedFileType, szSelectedTypeDescription, szSelectedGrouping) )
				{
					// If the file type was installed succesfully, add the file type to the list box.
					m_wndFileTypesListBox.InsertString(nTypeIndex, szSelectedFileType);

					// Reset the list box selection to the updated item.
					m_wndFileTypesListBox.SetCurSel(nTypeIndex);
				}
				else
				{
					// If the file type failed to install, display an error to the user.
					if ( pTheApp != NULL )
					{
						pTheApp->DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_FTYPE + szSelectedFileType + UNICHECK_MESSAGE_XINSTALLED),
													  NULL_TEXT, TRUE);
					}
				}
			}
		}
	}
}

/* OnFiletypelistDeletebutton - Delete File Type Button Handler */
void CFileTypeListDlg::OnFiletypelistDeletebutton() 
{
	/* Local Variable Declarations and Initialization */

	int nResult = 0;					// Dialog Result
	int nTypeIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected List Box Item

	CString szSelectedFileType;			// The File Type Selected in the List Box

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	/* Get the scan type of the selected list box item. */

	// Get the index of the selected item in the list box.
	nTypeIndex = m_wndFileTypesListBox.GetCurSel();

	// Only delete the file type if an item is selected.
	if ( nTypeIndex != UNICHECK_RETURN_NOINDEX )
	{
		// Get the file type text from the list box.
		m_wndFileTypesListBox.GetText(nTypeIndex, szSelectedFileType);

		// Delete the file type from the registry.
		if ( pTheApp != NULL )
		{
			if ( pTheApp->UninstallValidType(szSelectedFileType) )
			{
				// If the file type was uninstalled succesfully, delete the file type to the list box.
				m_wndFileTypesListBox.DeleteString(nTypeIndex);

				// Reset the list box selection to the updated item.
				m_wndFileTypesListBox.SetCurSel(nTypeIndex);
			}
			else
			{
				// If the file type failed to uninstall, display an error to the user.
				if ( pTheApp != NULL )
				{
					pTheApp->DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_FTYPE + szSelectedFileType + UNICHECK_MESSAGE_XUNINSTALLED),
												  NULL_TEXT, TRUE);
				}
			}
		}
	}
}

/* OnDblclkFiletypelistTypelistbox - Handler for Double Clicking on an Item in the File Types List Box */
void CFileTypeListDlg::OnDblclkFiletypelistTypelistbox() 
{
	// Call the edit button handler to edit the double clicked file type.
	OnFiletypelistEditbutton();
}

/* OnSelchangeFiletypelistTypelistbox - Handler for Changing a Selection in the File Types List Box */
void CFileTypeListDlg::OnSelchangeFiletypelistTypelistbox() 
{
	// Toggle the enabled states of the existing item form controls.
	ManageExistingItemControlStates();
}


/////////////////////////////////////////////////////////////////////////////
// CFileTypeListDlg Generic Form Control State Handlers

/* ManageExistingItemControlStates - Generic State Handler for Form Controls Needing a Selected Item */
void CFileTypeListDlg::ManageExistingItemControlStates()
{
	/* Local Variable Declarations and Initialization */

	int nIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected Item in the List Box

	UINT uSelectedFileType = NULL;			// The File Type That is Selected


	// Reset the buttons to an initial disabled state on each change.
	// This forces the code to specifically enable appropriate controls.
	m_wndEditButton.EnableWindow(FALSE);
	m_wndDeleteButton.EnableWindow(FALSE);


	// Get the current index and file type of the selected item in the file types list box.
	nIndex = m_wndFileTypesListBox.GetCurSel();
	uSelectedFileType = m_wndFileTypesListBox.GetItemData(nIndex);


	// If a file type is selected, enable the existing item form controls.
	if ( nIndex > UNICHECK_RETURN_NOINDEX )
	{
		m_wndEditButton.EnableWindow(TRUE);
		m_wndDeleteButton.EnableWindow(TRUE);
	}
	else
	{
		// If no file type is selected, disable the existing item form controls.
		m_wndEditButton.EnableWindow(FALSE);
		m_wndDeleteButton.EnableWindow(FALSE);
	}
}
