// UniCheckView.cpp : Implementation of the CUniCheckView Class
//

#include "stdafx.h"
#include "UniCheck.h"
#include "UniCheckView.h"

#include "AddFilesDialog.h"
#include "OptionsDlg.h"
#include "ScanListDlg.h"
#include "ScanDefDlg.h"
#include "FileTypeListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView

IMPLEMENT_DYNCREATE(CUniCheckView, CFormView)

BEGIN_MESSAGE_MAP(CUniCheckView, CFormView)
	//{{AFX_MSG_MAP(CUniCheckView)
	ON_BN_CLICKED(IDC_VALIDATEBTN, OnValidatebtn)
	ON_BN_CLICKED(IDC_RESULTSBTN, OnResultsbtn)
	ON_COMMAND(ID_TOOLS_CLEARRESULTSFILE, OnToolsClearresultsfile)
	ON_COMMAND(ID_TOOLS_CLEARNOTIFICATIONPANE, OnToolsClearnotificationpane)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_RELOADFILE, OnEditReloadfile)
	ON_COMMAND(ID_EDIT_ADDFILE, OnEditAddfile)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RELOADFILE, OnUpdateEditReloadfile)
	ON_COMMAND(ID_VALIDATE_VALIDATE, OnValidateValidate)
	ON_COMMAND(ID_VALIDATE_VIEWRESULTS, OnValidateViewresults)
	ON_BN_CLICKED(IDC_ADDFILESBTN, OnAddfilesbtn)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDFILE, OnUpdateEditAddfile)
	ON_UPDATE_COMMAND_UI(ID_VALIDATE_VALIDATE, OnUpdateValidateValidate)
	ON_UPDATE_COMMAND_UI(ID_VALIDATE_VIEWRESULTS, OnUpdateValidateViewresults)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CLEARRESULTSFILE, OnUpdateToolsClearresultsfile)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_TOOLS_SCANDEF, OnToolsScandef)
	ON_LBN_DBLCLK(IDC_SCANLIST, OnDblclkScanlist)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SCANDEF, OnUpdateToolsScandef)
	ON_COMMAND(ID_TOOLS_FILETYPES, OnToolsFiletypes)
	//}}AFX_MSG_MAP
	// Standard Printing Commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView Construction/Destruction

CUniCheckView::CUniCheckView()
	: CFormView(CUniCheckView::IDD)
{
	//{{AFX_DATA_INIT(CUniCheckView)
	//}}AFX_DATA_INIT
}

CUniCheckView::~CUniCheckView()
{
	/* Local Variable Declarations and Initialization */

	CUniCheckDoc* pTheDoc = NULL;		// Pointer to the Document


	// Get a reference to the document.
	pTheDoc = GetDocument();


	// Delete any files from the temporary source and header directories on exit.
	if ( pTheDoc != NULL )
	{
		pTheDoc->ClearEvalFolders();
	}
}

void CUniCheckView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUniCheckView)
	DDX_Control(pDX, IDC_SCANLIST, m_wndScansListBox);
	DDX_Control(pDX, IDC_RESULTSBTN, m_wndResultsButton);
	DDX_Control(pDX, IDC_ADDFILESBTN, m_wndAddFilesButton);
	DDX_Control(pDX, IDC_VALIDATEBTN, m_wndValidateButton);
	DDX_Control(pDX, IDC_MONITORWIN, m_wndMonitorWindow);
	DDX_Control(pDX, IDC_FILETREE, m_wndLoadedFilesTree);
	//}}AFX_DATA_MAP
}

BOOL CUniCheckView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Execute the parent class implementation of the method to complete standard behavior.
	return CFormView::PreCreateWindow(cs);
}

void CUniCheckView::OnInitialUpdate()
{
	/* Initialize the Application View */

	// Execute the parent class implementation of the method to complete standard behavior.
	CFormView::OnInitialUpdate();

	// Initialize the view controls and load document data.
	InitializeInterface();
	LoadDocDataToInterface();

	// Resize the main frame to the size of the form view.
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

/* InitializeInterface - Initialize the Application to a Fresh State */
void CUniCheckView::InitializeInterface()
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application


	// Get references to the application instance and the document.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Rebuild the base nodes of the empty loaded file tree control.
	InitalizeFileTree();

	// Loaded the scan types to the interface.
	InitializeScanListBox();

	// Clear all messages from the User Feedback Display window.
	OnToolsClearnotificationpane();

	// If the application is not fully initialized, display an error to the user describing the action needed 
	// to fully enable the application.
	if (pTheApp != NULL )
	{
		if ( !pTheApp->IsAppInitialized() )
		{
			// Write a message to the User Feedback Display window.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_APPXINIT, 
									 UNICHECK_MESSAGE_SETVALENG, 
									 TRUE);
			}
		}
		else
		{
			// If the application is properly initialized, write a message to the User Feedback Display window 
			// stating that the application is ready to process.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_APPINIT, 
									 NULL_TEXT, 
									 FALSE);
			}
		}
	}
}

/* InitalizeFileTree - Restore the Loaded Files Tree Control to a Fresh State */
void CUniCheckView::InitalizeFileTree()
{
	/* Local Variable Declarations and Initialization */

	HTREEITEM hRootItem;			// Handle to the Root Node of the Tree Control
	HTREEITEM hSourceFileNode;		// Handle to the Source Files Category Node
	HTREEITEM hHeaderFileNode;		// Handle to the Header Files Category Node

	TVINSERTSTRUCT dsTreeStruct;	// Tree Control Item Insertion Structure


	// Initialize stuctures.
	ZeroMemory(&dsTreeStruct, sizeof(TVINSERTSTRUCT));


	// Remove all nodes from the tree control.
	m_wndLoadedFilesTree.DeleteAllItems();


	// Load the values for the root node of the tree control.
	dsTreeStruct.hParent = NULL;							// Handle to Parent Node
	dsTreeStruct.hInsertAfter = NULL;						// Handle to Predecessor Node
	dsTreeStruct.item.mask = TVIF_TEXT;						// Item Attribute being Set (Only Text Here)
	dsTreeStruct.item.pszText = UNICHECK_FILETREE_ROOTNODE;	// Text of the Node

	// Add the root node to the tree.
	hRootItem = m_wndLoadedFilesTree.InsertItem(&dsTreeStruct);

	// Validate that the item was created.
	if (hRootItem != NULL)
	{
		// Reset the tree control item insertion structure.
		ZeroMemory(&dsTreeStruct, sizeof(TVINSERTSTRUCT));

		// Load the values for the source files category node of the tree control.
		dsTreeStruct.hParent = hRootItem;						// Handle to Parent Node
		dsTreeStruct.hInsertAfter = NULL;						// Handle to Predecessor Node
		dsTreeStruct.item.mask = TVIF_TEXT;						// Item Attribute being Set (Only Text Here)
		dsTreeStruct.item.pszText = UNICHECK_FILETREE_SOURCENODE;	// Text of the Node

		// Add the source files category node to the tree.
		hSourceFileNode = m_wndLoadedFilesTree.InsertItem(&dsTreeStruct);


		// Reset the tree control item insertion structure.
		ZeroMemory(&dsTreeStruct, sizeof(TVINSERTSTRUCT));

		// Load the values for the header files category node of the tree control.
		if ( hSourceFileNode != NULL)
		{
			dsTreeStruct.hParent = hRootItem;						// Handle to Parent Node
			dsTreeStruct.hInsertAfter = hSourceFileNode;			// Handle to Predecessor Node
			dsTreeStruct.item.mask = TVIF_TEXT;						// Item Attribute being Set (Only Text Here)
			dsTreeStruct.item.pszText = UNICHECK_FILETREE_HEADERNODE;	// Text of the Node

			// Add the header files category node to the tree.
			hHeaderFileNode = m_wndLoadedFilesTree.InsertItem(&dsTreeStruct);
		}


		// Expand the root node of the tree control by default.
		m_wndLoadedFilesTree.Expand(hRootItem, TVE_EXPAND);
	}
}

/* InitializeScanListBox - Restore the Scans Checked List Box Control to a Fresh State */
void CUniCheckView::InitializeScanListBox()
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Load the installed scans to the list box.
	if ( pTheApp != NULL )
	{
		pTheApp->LoadScansForDisplay(&m_wndScansListBox);
	}
}

/* LoadDocDataToInterface - Load Document Data to the User Interface */
void CUniCheckView::LoadDocDataToInterface()
{
	/* Local Variable Declarations and Initialization */

	int nElements = 0;			// Number of Elements in Link Lists in Document Data

	BOOL bDataExists = FALSE;	// Flag to Verify that Data Exists in the Document

	DSLOADEDFILE dsLoadedFile;	// Loaded File Attributes Structure
	DSLISTPOSITION	dsPosition;	// Link List Position in Document Data

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance
	CUniCheckDoc* pTheDoc = NULL;	// Pointer to the Document


	// Initialize stuctures.
	ZeroMemory(&dsPosition, sizeof(DSLISTPOSITION));


	// Get references to the application instance and document.
	pTheApp = (CUniCheckApp*) AfxGetApp();
	pTheDoc = GetDocument();

	// Get references to the first element in the document data.
	if (pTheDoc != NULL)
	{
		bDataExists = pTheDoc->GetFirstFileInDocument(&dsPosition);
	}

	// If a first element is found in the document data, load the document data to the user interface.
	if ( bDataExists )
	{
		// Get the number of elements in the document data.
		if (pTheDoc != NULL)
		{
			nElements = pTheDoc->GetNumberOfLoadedFilesInDocument();
		}

		// Verify that the document data is not corrupt.
		if (nElements != -1)
		{
			// Loop through each element of the linked lists.
			for ( int i = 0; i < nElements; i++ )
			{
				// Get the current element of each list and advance the list reference to the next element.
				if (pTheDoc != NULL)
				{
					dsLoadedFile = pTheDoc->GetSpecificFileFromDocument(&dsPosition);
				}

				// Verify that the file was retrieved.
				if (dsLoadedFile.szOriginalPath != UNICHECK_SEARCH_NOFILEFOUND)
				{
					/* In the case of loading serialized document data to the interface, the files are first loaded to the 
					   interface then copied.  This is the opposite of the Addfilesbtn handler.  This is intentional.  In 
					   the case of serialized data, the user should be given an opportunity to ensure the files exit in the
					   location specified by the document and reload the specifications manually.  Adding them to the interface
					   (nearly a guaranteed success) before copying the spec.s (the likely point of failure) will ensure the 
					   interface reflects what is in the document and present a better user experience (easier to refresh spec.s, 
					   etc...)

					   In the case of adding new files to a document, since the file was never valid (for whatever reason), it
					   should never be added to the interface.  Therefore the order of the operations is reversed. */

					// Add the file to the user interface.
					if(AddFileToLoadedFilesTreeControl(dsLoadedFile))
					{
						// If the add succeeded, copy the file to the appropriate temporary directory.
						CopyFileToAnalysisLocation(dsLoadedFile);
					}
				}
			}
		}
		else
		{
			// If the document data is corrupt, display an error to the user and kill the application instance.

			// Display an error to the user.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_FILECORUPT, UNICHECK_MESSAGE_APPMUSTDIE, TRUE);
			}

			// Kill the application instance.
			if (pTheApp != NULL)
			{
				pTheApp->KillApp();
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView Printing Methods

BOOL CUniCheckView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CUniCheckView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CUniCheckView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CUniCheckView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView Diagnostics

#ifdef _DEBUG
void CUniCheckView::AssertValid() const
{
	CFormView::AssertValid();
}

void CUniCheckView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CUniCheckDoc* CUniCheckView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUniCheckDoc)));
	return (CUniCheckDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView Message Handlers

// Form Control Handlers

/* OnAddfilesbtn - Add Files Button Handler */
void CUniCheckView::OnAddfilesbtn() 
{
	/* Local Variable Declarations and Initialization */

	int nDialogResult = IDCANCEL;		// Windows Shell Select File Dialog Return Value

	TCHAR szFileNameBuffer[UNICHECK_SIZE_ADDFILEBUFLEN] = {NULL_CHAR};	// Buffer for File Names Returned by Select File Dialog

	CString szFilter;					// The File Types Filter Based on Installed Valid File Types

	POSITION fileNamePosition = NULL;	// Position within the szFileNameBuffer.

	DSLOADEDFILE dsLoadedFile;			// File Seleted by the User in the Select File Dialog

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance
	CUniCheckDoc* pTheDoc = NULL;		// Pointer to the Document


	// Get references to the application instance, document, and owner.
	pTheApp = (CUniCheckApp*) AfxGetApp();
	pTheDoc = GetDocument();


	// Load the valid file types filter from the registry.
	if ( pTheApp != NULL )
	{
		pTheApp->LoadAllValidFileTypes(NULL, &szFilter);
	}

 
	// Create the "Select File" dialog.
	CAddFilesDialog wndSelectFileDialog(TRUE, NULL, NULL, OFN_EXPLORER|OFN_ALLOWMULTISELECT|OFN_HIDEREADONLY, szFilter, this->GetWindow(GW_OWNER));

	// Load the dialog title and file name buffer to the dialog instance.
	wndSelectFileDialog.m_ofn.lpstrFile = szFileNameBuffer;
	wndSelectFileDialog.m_ofn.nMaxFile = UNICHECK_SIZE_ADDFILEBUFLEN;
	wndSelectFileDialog.m_ofn.lpstrTitle = UNICHECK_TEXT_ADDFILE;


	// Display the "Select File" dialog.
	nDialogResult = wndSelectFileDialog.DoModal();

	// If the user selected a file and confirmed the select (pressed OK instead of Cancel), load the file.
	if (nDialogResult == IDOK)
	{
		// Display the wait cursor while the selected files are loaded.
		if(pTheApp != NULL)
		{
			pTheApp->BeginWaitCursor();
		}

		// Get the position of the first file name.
		fileNamePosition = wndSelectFileDialog.GetStartPosition();

		// Loop through the buffer of returned file names and handle each one appropriately.
		while ( fileNamePosition != NULL )
		{
			// Get the appropriate data about the file from the dialog.
			dsLoadedFile.szOriginalPath = wndSelectFileDialog.GetNextPathName(fileNamePosition);
			dsLoadedFile.szFileName = pTheApp->ExtractFileNameFromPath(dsLoadedFile.szOriginalPath);
			dsLoadedFile.szFileExtension = pTheApp->DetermineFileExtension(dsLoadedFile.szOriginalPath);


			// Verify that the file is of a valid file type.
			dsLoadedFile.szFileExtension.MakeLower();
			if ( pTheApp != NULL )
			{
				if ( pTheApp->IsFileTypeValid(dsLoadedFile.szFileExtension) )
				{
					/* Load the selected file to the document and the interface. */

					// If the file was added to the interface successfully, then add the file to the document data.
					if (pTheDoc != NULL)
					{
						switch(pTheDoc->AddFileToDocument(dsLoadedFile))
						{
						case UNICHECK_RETURN_SUCCESS:
							// If the file was added to the document correctly, copy the file to the approriate temp. directory.
							if(CopyFileToAnalysisLocation(dsLoadedFile))
							{
								// If the copy succeeded, add the file to the user interface.  If it fails, process the same as 
								// if the AddFileToDocument call failed.
								if (AddFileToLoadedFilesTreeControl(dsLoadedFile))
								{
									// Only jump out of the switch statement if the file is added to the interface. If it fails, 
									// process the same as if the AddFileToDocument call failed.
									break;
								}
							}

						case UNICHECK_RETURN_FAILED:
							// If the file could not be added to the document or loaded to the temporary directory, as a fail-safe ensure 
							// the file is removed from both locations in case one of the two operations succeeded and display an error to the user.

							// Remove file from the document and temporary directory.
							if (pTheDoc != NULL)
							{
								pTheDoc->DeleteSelectedFile(dsLoadedFile.szFileName);
							}

							// Display an error to the user.
							if (pTheApp != NULL)
							{
								pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_FILESP + dsLoadedFile.szFileName + UNICHECK_MESSAGE_FAILLOAD), NULL_TEXT, TRUE);
							}
							break;

						case UNICHECK_RETURN_FILECORRUPT:
							// If the document data is corrupt, display an error to the user and kill the application.

							// Display an error to the user.
							if (pTheApp != NULL)
							{
								pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_FILECORUPT, UNICHECK_MESSAGE_APPMUSTDIE, TRUE);
							}

							// Kill the application instance.
							if (pTheApp != NULL)
							{
								pTheApp->KillApp();
							}
							break;

						default:
							break;
						}
					}
				}
				else
				{
					// If the file is not a C Source or C/CPP header file (.c or.h), display an error message box.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_FILETYPE + dsLoadedFile.szFileExtension + UNICHECK_MESSAGE_CNEVALED), UNICHECK_MESSAGE_SEEVALTYPE, TRUE);
					}
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_FILESP + dsLoadedFile.szFileName + UNICHECK_MESSAGE_XADDSET), NULL_TEXT, FALSE);
					}
				}
			}
		}

		// Return control to the user, signalling that processing is complete.
		if(pTheApp != NULL)
		{
			pTheApp->EndWaitCursor();
		}
	}
}

/* OnValidatebtn - Validate Button Handler */
void CUniCheckView::OnValidatebtn() 
{
	/* Local Variable Declarations and Initialization */
	
	BOOL bFilesExist = FALSE;		// Flag Specifying that Files have been Copied to the Temp Directories

	int nNumberOfInstalledScans = 0;	// The Number of Scans in the Scans List Box

	UINT uScanType = NULL;			// The Scan Type of the Current Scan in the List Box

	CString szSelectedFile;			// The Path of the File Selected in the File Tree
	CString szSourcePath;			// The Path String to the Tempory Source Files Directory
	CString szHeaderPath;			// The Path String to the Tempory Header Files Directory
	CString szExecutablePath;		// The Path String to the Scan .exe
	CString szScanArguments;		// The Scan .exe Arguments

	HTREEITEM hSelectedNode = NULL;	// Handle to the Selected File Tree Node

	DSSCAN dsScan;					// The Attributes of a Scan in the List Box
	DSLOADEDFILE dsFile;			// The Attributes of the Selected File in the File Tree

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance
	CUniCheckDoc* pTheDoc = NULL;	// Pointer to the Document


	// Grab references to the application instance, and document.
	pTheApp = (CUniCheckApp*) AfxGetApp();
	pTheDoc = GetDocument();


	// Verify the temporary analysis folders paths are set.
	if (pTheApp != NULL)
	{
		// If the application did not fully initialize, exit the method and display message the user.
		if ( !pTheApp->IsAppInitialized() )
		{
			// Write an error message to the User Feedback Display window.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_APPXSET, UNICHECK_MESSAGE_VERPATHSET, TRUE);	
			}

			return;
		}
	}


	/* Begin Validation Processing */

	// Set the cursor to the hour-glass to signify to user that processing is occurring and 
	// to prevent the user from interacting with the interface.
	if (pTheApp != NULL)
	{
		pTheApp->BeginWaitCursor();
	}


	/* No Scan Selected by User */

	// If no scan is selected by the user, display a message and do no processing.
	if ( !IsScanSelected() )
	{
		// Write an error message to the User Feedback Display window.
		if (pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_0SCANEXE, NULL_TEXT, TRUE);
		}
	}
	else
	{
		/* A Scan Type Was Selected */

		// Load the full paths to the temporary source and header directories for Unicode scan.
		// These values are not validated after the fetch because the IsAppInitialized call earlier verified that the paths have values.
		if (pTheApp != NULL)
		{
			szSourcePath = pTheApp->GetSourcePath();
		}

		if (pTheApp != NULL)
		{
			szHeaderPath = pTheApp->GetHeaderPath();
		}


		/* Validate that the C source files and C/CPP files to scan have been copied to the tempory directories. */

		// Scan the source files temporary directory to see if any files exist in that directory.
		if (pTheApp != NULL)
		{
			bFilesExist = pTheApp->VerifyThatFilesExistInDirectory(szSourcePath);
		}

		// Scan the header files temporary directory to see if any files exist in that directory.
		if (!bFilesExist)
		{
			// Only scan the header directory if no files were found in the source directory.
			if (pTheApp != NULL)
			{
				bFilesExist = pTheApp->VerifyThatFilesExistInDirectory(szHeaderPath);
			}
		}


		// If files exist in the temp directories, continue processing the selected scans.
		if (bFilesExist)
		{
			/* Initialization for scans requiring the temporary analysis folders. */

			// Create pseudo VS6 project file to pass as a parameter to scans requiring the file set file as an input parameter.
			if (CreateSetFileForScanInput() == UNICHECK_RETURN_FAILED)
			{
				// If the fileset file could not be created, exit and signal that the scan could not be initialized 
				// and sould be disabled.
				return;
			}


			/* Cycle through each selected scan and display the reults file. */

			// Get the number of scans in the scans list box.
			nNumberOfInstalledScans = m_wndScansListBox.GetCount();

			// Cycle through each scan in the list box.
			for ( int i = 0; i < nNumberOfInstalledScans; i++ )
			{
				// Clear the scan structure to ensure clean data.
				if ( pTheApp != NULL )
				{
					pTheApp->ClearScanStructure(&dsScan);
				}

				// Reinitialize the scan .exe command line string and scan arguments string.
				szExecutablePath = NULL_TEXT;
				szScanArguments = NULL_TEXT;

				// Get the scan type from the list box item.
				uScanType = m_wndScansListBox.GetItemData(i);

				// If a scan type is found, get the scan attributes and display the results file.
				if ( uScanType != NULL )
				{
					// If the scan is selected, display its results file.
					if ( IsScanSelected(uScanType) )
					{
						// Get the scan attributes.
						if ( pTheApp != NULL )
						{
							if ( pTheApp->GetInstalledScan(uScanType, &dsScan) )
							{
								/* If the scan is installed, attempt to run the scan. */

								// Build the command linestring for the current scan process.
								if ( pTheApp != NULL )
								{
									/* Get the path to the selected file in the file tree. */
									
									// Get the selected file name.
									hSelectedNode = m_wndLoadedFilesTree.GetSelectedItem();
									if (hSelectedNode != NULL )
									{
										szSelectedFile = m_wndLoadedFilesTree.GetItemText(hSelectedNode);
									}

									// If an item is selected, get it's full path.
									if ( !szSelectedFile.IsEmpty() )
									{
										// Get the file's attributes from the document data.
										if ( pTheDoc != NULL )
										{
											dsFile = pTheDoc->GetSpecificFileFromDocument(szSelectedFile);
										}

										// Load the path to the selected file to the parameter to BuildScanCommandLine.
										szSelectedFile = dsFile.szOriginalPath;
									}
									else
									{
										// If no file is selected, pass null to BuildScanCommandLine.
										szSelectedFile = NULL_TEXT;
									}

									if ( pTheApp->BuildScanCommandLine(dsScan.uScanType, &szExecutablePath, &szScanArguments, szSelectedFile) )
									{
										// If the command line is successfully built, can the document data.
										RunScan(szExecutablePath, szScanArguments, dsScan.szScanTitle);
									}
									else
									{
										// If the scan's command line could not be built, display an error to the user and disble the scan.

										// Display error to the User Feedback Display window.
										if ( pTheApp != NULL )
										{
											pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_THESCAN + dsScan.szScanTitle + UNICHECK_MESSAGE_XINIT, 
																		  dsScan.szScanTitle + UNICHECK_MESSAGE_SCANUNAVAIL, FALSE);
										}
							

										// Disable the scan in the scans list box.
										EnableScan(dsScan.uScanType, FALSE);							

										// Because the inability to build the command line string could indicate a serious configuration
										// problem on the target system, no path to remedy the problem (re-enable the scan without closing and re-opening
										// the application) will not be provided.
									}
								}
							}
							else
							{
								// If the scan is not properly installed, display a message to the user and remove the scan from the list box.

								// Display error to the User Feedback Display window.
								if ( pTheApp != NULL )
								{
									pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_THESCAN + dsScan.szScanTitle + UNICHECK_MESSAGE_XPROPINST, 
																  UNICHECK_MESSAGE_SCANTBREMOVED, FALSE);
								}

								// Remove the scan from the scans list box.
								m_wndScansListBox.DeleteString(i);
							}
						}
					}
				}
			}

		}
		else
		{
			// If no source or header files exist in the tempory directories, write a message to the User Feedback Display window.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_0FILEINTEMP, NULL_TEXT, FALSE);
			}
		}
	}

	// Return control to the user, signalling that processing is complete.
	if(pTheApp != NULL)
	{
		pTheApp->EndWaitCursor();
	}
}

/* OnResultsbtn - Display Results Button Handler */
void CUniCheckView::OnResultsbtn() 
{
	/* Local Variable Declarations and Initialization */

	int nNumberOfInstalledScans = 0;			// The Number of Scans in the Scans List Box

	UINT uScanType = NULL;			// The Scan Type of the Current Scan in the List Box

	CString szViewerPath;			// Path to the Executable to View Scan Results

	DSSCAN dsScan;					// The Attributes of a Scan in the List Box

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Verify that the application is fully initialized.
	if (pTheApp != NULL)
	{
		if ( pTheApp->IsAppInitialized() )
		{
			// Get the executable to view scan results.
			szViewerPath = pTheApp->GetViewerPath();


			/* No Scan Type Selected */

			// If the user did not select which results to display, display a message to the user describing the problem and do no further processing.
			if ( !IsScanSelected() )
			{
				// Display an error message in the User Feedback Display window.
				if (pTheApp != NULL)
				{
					pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_0RESDISP, NULL_TEXT, TRUE);
				}
			}
			else
			{
				/* Cycle through each selected scan and display the reults file. */

				// Get the number of scans in the scans list box.
				nNumberOfInstalledScans = m_wndScansListBox.GetCount();

				// Cycle through each scan in the list box.
				for ( int i = 0; i < nNumberOfInstalledScans; i++ )
				{
					// Clear the scan structure to ensure clean data.
					if ( pTheApp != NULL )
					{
						pTheApp->ClearScanStructure(&dsScan);
					}

					// Get the scan type from the list box item.
					uScanType = m_wndScansListBox.GetItemData(i);

					// If a scan type is found, get the scan attributes and display the results file.
					if ( uScanType != NULL )
					{
						// If the scan is selected, display its results file.
						if ( IsScanSelected(uScanType) )
						{
							// Get the scan attributes.
							if ( pTheApp != NULL )
							{
								if ( pTheApp->GetInstalledScan(uScanType, &dsScan) )
								{
									// If the scan is installed, display the results file.
									ViewResults(szViewerPath, (dsScan.szResultsFilePath + dsScan.szResultsFile), dsScan.szScanTitle );
								}
								else
								{
									// If the scan is not properly installed, display a message to the user and remove the scan from the list box.

									// Display error to the User Feedback Display window.
									if ( pTheApp != NULL )
									{
										pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_THESCAN + dsScan.szScanTitle + UNICHECK_MESSAGE_XPROPINST, 
																	  UNICHECK_MESSAGE_SCANTBREMOVED, FALSE);
									}

									// Remove the scan from the scans list box.
									m_wndScansListBox.DeleteString(i);
								}
							}
						}
					}
				}
			}
		}
	}
}

/* OnDblclkScanlist - Handler for Double Clicking on an Item in the Scans List Box */
void CUniCheckView::OnDblclkScanlist() 
{
	/* Local Variable Declarations and Initialization */

	int nResult = 0;				// Dialog Result
	int nScanIndex = UNICHECK_RETURN_NOINDEX;	// Index of the Selected List Box Item

	UINT uSelectedScanType = NULL;	// The Scan Type of the Scan that was Double Clicked by the User

	CScanDefDlg wndScanDefDlg;		// The Scan Definition Dialog Box

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


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
			/* If the user set or modified values of the scan, update the scans list box. */

			// Lock the interface and visually notify user that processing is occuring.
			BeginWaitCursor();

			// Reload the installed scans.
			if ( pTheApp != NULL )
			{
				pTheApp->LoadScansForDisplay(&m_wndScansListBox);

			}

			// Return control to the user and visually signify that processing is complete.
			EndWaitCursor();
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView Menu Handlers

/* OnEditAddfile - Add File Menu Handler */
void CUniCheckView::OnEditAddfile() 
{
	// Call the corresponding form control handler.
	OnAddfilesbtn();
}

/* OnEditReloadfile - Reload Menu Handler */
void CUniCheckView::OnEditReloadfile() 
{
	/* Local Variable Declarations and Initialization */

	CString szNodeName;				// Text of the Currently Selected Tree Node

	HTREEITEM hSelectedNode = NULL;	// Handle to the Currently Selected Tree Node

	DSLOADEDFILE dsLoadedFile;		// Loaded File Attributes Structure

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance
	CUniCheckDoc* pTheDoc = NULL;	// Pointer to the Document


	// Get references to the application instance and document.
	pTheApp = (CUniCheckApp*) AfxGetApp();
	pTheDoc = GetDocument();

	// Get a reference to the currently selected tree node.
	hSelectedNode = m_wndLoadedFilesTree.GetSelectedItem();

	// Verify that a node is selected.
	if (hSelectedNode != NULL)
	{
		// Retrieve the file name from the tree node.
		szNodeName = m_wndLoadedFilesTree.GetItemText(hSelectedNode);

		// Only refresh the file if access to the document instance is available.
		if (pTheDoc != NULL)
		{
			// Get the file from the document data.
			dsLoadedFile = pTheDoc->GetSpecificFileFromDocument(szNodeName);

			// Verify that the file was retrieved.
			if (dsLoadedFile.szOriginalPath != UNICHECK_SEARCH_NOFILEFOUND)
			{
				// Recopy the file to the temporary directory.
				if (CopyFileToAnalysisLocation(dsLoadedFile))
				{
					// If the file was refreshed successfully, display a message to the user.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_FILESP + dsLoadedFile.szFileName + UNICHECK_MESSAGE_RELOADED), NULL_TEXT, FALSE);
					}
				}
			}
		}
		else
		{
			// If the pointer to the document is invalid, display a message to the user.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_XDOC, NULL_TEXT, TRUE);
			}
		}
	}
}

/* OnEditDelete - Delete Menu Handler */
void CUniCheckView::OnEditDelete() 
{
	/* Local Variable Declarations and Initialization */

	CString szFileName;			// Name of File for Selected Tree Node

	HTREEITEM hSelectedNode = NULL;	// Handle to the Selected Node in the Loaded Files Tree Control

	CUniCheckDoc* pTheDoc = NULL;	// Pointer to the Document


	// Get a reference to the document.
	pTheDoc = GetDocument();

	// Get a reference to the selected tree node.
	hSelectedNode = m_wndLoadedFilesTree.GetSelectedItem();

	// If a reference to a node is retrieved, delete the node from the tree control and the associated
	// file from the temporary directory and document.
	if (hSelectedNode != NULL)
	{
		// Get the file name from the tree node.
		szFileName = m_wndLoadedFilesTree.GetItemText(hSelectedNode);

		// Delete the file from the temporary directory and remove the file from the document.
		RemoveSelectedFile(szFileName);

		// Delete the node from the tree control.
		if(hSelectedNode != NULL)
		{
			m_wndLoadedFilesTree.DeleteItem(hSelectedNode);
		}

		// Refresh the tree control.
		m_wndLoadedFilesTree.Invalidate();
	}
}

/* OnValidateUnicode - Validate Menu Handler */
void CUniCheckView::OnValidateValidate() 
{
	// Call the corresponding form control handler.
	OnValidatebtn();
}

/* OnValidateViewresults - View Results Menu Handler */
void CUniCheckView::OnValidateViewresults() 
{
	// Call the corresponding form control handler.
	OnResultsbtn();
}

/* OnToolsClearnotificationpane - Clear All Text from User Feedback Display Window */
void CUniCheckView::OnToolsClearnotificationpane() 
{
	/* Local Variable Declarations and Initialization */

	int nMonitorLinesRemaining = 0;		// Number of Lines Currently Loaded in the User Feedback Display Window


	// Delete the first line of text from User Feedback Display window and get the number of lines remaining.
	nMonitorLinesRemaining = m_wndMonitorWindow.DeleteString(0);

	// Loop while lines of text exist in the User Feedback Display window.
	while (nMonitorLinesRemaining > 0)
	{
		// Delete the now first line of text from User Feedback Display window and get the number of lines remaining.
		nMonitorLinesRemaining = m_wndMonitorWindow.DeleteString(0);
	}
}

/* OnToolsClearresultsfile - Clear Results Files Menu Handler */
void CUniCheckView::OnToolsClearresultsfile() 
{
	/* Local Variable Declarations and Initialization */

	int nNumberOfInstalledScans = 0;	// The Number of Scans in the Scans List Box

	UINT uScanType = NULL;			// The Scan Type of the Current Scan in the List Box

	DSSCAN dsScan;					// The Attributes of a Scan in the List Box

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	/* No Scan Results Selected for Deletion */

	// If the user did not select any scan types to delete, write a message to the User Feedback Display window.
	if ( !IsScanSelected() )
	{
		if (pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_0RESCLEAR, NULL_TEXT, FALSE);
		}
	}
	else
	{
		/* A Scan Type Was Selected */


		/* Cycle through each selected scan and display the reults file. */

		// Get the number of scans in the scans list box.
		nNumberOfInstalledScans = m_wndScansListBox.GetCount();

		// Cycle through each scan in the list box.
		for ( int i = 0; i < nNumberOfInstalledScans; i++ )
		{
			// Clear the scan structure to ensure clean data.
			if ( pTheApp != NULL )
			{
				pTheApp->ClearScanStructure(&dsScan);
			}

			// Get the scan type from the list box item.
			uScanType = m_wndScansListBox.GetItemData(i);

			// If a scan type is found, get the scan attributes and display the results file.
			if ( uScanType != NULL )
			{
				// If the scan is selected, display its results file.
				if ( IsScanSelected(uScanType) )
				{
					// Get the scan attributes.
					if ( pTheApp != NULL )
					{
						if ( pTheApp->GetInstalledScan(uScanType, &dsScan) )
						{
							// If the scan is installed, delete the results file.
							DeleteResultsFile( (dsScan.szResultsFilePath + dsScan.szResultsFile), dsScan.szScanTitle );
						}
						else
						{
							// If the scan is not properly installed, display a message to the user and remove the scan from the list box.

							// Display error to the User Feedback Display window.
							if ( pTheApp != NULL )
							{
								pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_THESCAN + dsScan.szScanTitle + UNICHECK_MESSAGE_XPROPINST, 
															  UNICHECK_MESSAGE_SCANTBREMOVED, FALSE);
							}

							// Remove the scan from the scans list box.
							m_wndScansListBox.DeleteString(i);
						}
					}
				}
			}
		}
	}
}

/* OnToolsFiletypes - Display the Dialog to Valid File Types */
void CUniCheckView::OnToolsFiletypes() 
{
	/* Local Variable Declarations and Initialization */

	CFileTypeListDlg wndTypesListDlg;	// The List of Valid File Types Dialog Box

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Display the list of valid file types dialog.
	wndTypesListDlg.DoModal();
}

/* OnToolsScandef - Display the Dialog to Add and Edit Installed Scans */
void CUniCheckView::OnToolsScandef() 
{
	/* Local Variable Declarations and Initialization */

	CScanListDlg wndScansListDlg;	// The List of Installed Scans Dialog Box

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Display the list of installed scans dialog.
	wndScansListDlg.DoModal();


	/* Refresh the installed scans list box. */

	// Lock the interface and visually notify user that processing is occuring.
	BeginWaitCursor();

	// Reload the installed scans.
	if ( pTheApp != NULL )
	{
		pTheApp->LoadScansForDisplay(&m_wndScansListBox);

	}

	// Return control to the user and visually signify that processing is complete.
	EndWaitCursor();
}

/* OnToolsOptions - Display the Dialog to Adjust User Preferences and Application Settings */
void CUniCheckView::OnToolsOptions() 
{
	/* Local Variable Declarations and Initialization */

	int nResult = 0;				// Dialog Result
	int nMaxFiles = 0;				// The Maximum Number of Files the User Can Add to a List at One Time
	int nRegistryValueDefault = 0;	// Default Max. Files Value to Return if No Registry Value Found

	CString szProgramPath;			// The Directory of the Application
	CString szHeaderPath;			// The Directory of the Header Files Temporary Directory
	CString szSourcePath;			// The Directory of the Source Files Temporary Directory
	CString szFilesetFile;			// Name of the Psuedo-Visual Studio Project File to Use as Scan Input
	CString szFilesetFilePath;		// Path of the Psuedo-Visual Studio Project File
	CString szViewerPath;			// Path of the Executable to View Scan Results
	CString szRegistryKey;			// Application Registry Key Name
	CString szRegistryValue;		// Specific Setting Registry Key Name

	COptionsDlg wndOptionsDlg;		// The Options Dialog Box

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp *) AfxGetApp();


	// Load the registry key strings to get the program path string from the registry.
	szRegistryKey = UNICHECK_REG_SETTINGSKEY;
	szRegistryValue = UNICHECK_REG_PROGPATHKEY;

	// Get the program path from the registry and load the value to the dialog.
	if (pTheApp != NULL)
	{
		wndOptionsDlg.SetProgramPath( pTheApp->GetProfileString(szRegistryKey, szRegistryValue, NULL) );
	}


	// Load the registry key strings to get the header path string from the registry.
	szRegistryValue = UNICHECK_REG_HDRPATHKEY;

	// Get the header path from the registry and load the value to the dialog.
	if (pTheApp != NULL)
	{
		wndOptionsDlg.SetHeaderPath( pTheApp->GetProfileString(szRegistryKey, szRegistryValue, NULL) );
	}


	// Load the registry key string to get the source path string from the registry.
	szRegistryValue = UNICHECK_REG_SRCPATHKEY;

	// Get the source path from the registry and load the value to the dialog.
	if (pTheApp != NULL)
	{
		wndOptionsDlg.SetSourcePath( pTheApp->GetProfileString(szRegistryKey, szRegistryValue, NULL) );
	}


	// Load the registry key string to get the fileset file string from the registry.
	szRegistryValue = UNICHECK_REG_FILESETKEY;

	// Get the results fileset file from the registry and load the value to the dialog.
	if (pTheApp != NULL)
	{
		wndOptionsDlg.SetFilesetFileName( pTheApp->GetProfileString(szRegistryKey, szRegistryValue, NULL) );
	}


	// Load the registry key string to get the fileset file path string from the registry.
	szRegistryValue = UNICHECK_REG_FILESETPATHKEY;

	// Get the results fileset file path from the registry and load the value to the dialog.
	if (pTheApp != NULL)
	{
		wndOptionsDlg.SetFilesetFilePath( pTheApp->GetProfileString(szRegistryKey, szRegistryValue, NULL) );
	}


	// Load the registry key string to get the results viewer path string from the registry.
	szRegistryValue = UNICHECK_REG_VIEWERPATHKEY;

	// Get the results viewer path from the registry and load the value to the dialog.
	if (pTheApp != NULL)
	{
		wndOptionsDlg.SetViewerPath( pTheApp->GetProfileString(szRegistryKey, szRegistryValue, NULL) );
	}


	// Load the registry key values to get max. files limit from the registry.
	szRegistryValue = UNICHECK_REG_MAXFILEKEY;
	nRegistryValueDefault = UNICHECK_REG_MAXFILEDEFAULT;

	// Get the max. files limit from the registry and load the value to the dialog.
	if (pTheApp != NULL)
	{
		wndOptionsDlg.SetMaxFile( pTheApp->GetProfileInt(szRegistryKey, szRegistryValue, nRegistryValueDefault) );
	}


	// Display the dialog.
	nResult = wndOptionsDlg.DoModal();

	// If the user pressed OK, handle the new state of the dialog.
	if (nResult == IDOK)
	{
		/* If the user set or modified values on the form, set class member variables and adjust 
		   appropriate program controls. */


		/* Program Path Setting */

		// Get the user selected value from the dialog.
		szProgramPath = wndOptionsDlg.GetProgramPath();


		// Verify that a value was selected.
		if (!szProgramPath.IsEmpty())
		{
			/* The user selected a valid value. */

			// Load the path value to a class member.
			if (pTheApp != NULL)
			{
				pTheApp->SetProgramPath(szProgramPath);
			}
		
			// Write the path value to the registry.
			if (pTheApp != NULL)
			{
				if ( pTheApp->WriteProfileString(UNICHECK_REG_SETTINGSKEY, UNICHECK_REG_PROGPATHKEY, szProgramPath) )
				{
					/* If the path value is loaded to the registry appropriately, write a message
					   to the User Feedback Display window. */

					// Display the message in the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_SETAPPPATH + szProgramPath), NULL_TEXT, FALSE);
					}
				}
				else
				{
					/* If the path value is not loaded to the registry, write a message to the 
					   User Feedback Display. */

					// Display the message to the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_XSETAPPPATH + szProgramPath), NULL_TEXT, TRUE );
					}
				}
			}
		}
		else
		{
			/* No values appeared to be set by the user, even though the user pressed OK. */

			// Load NULL strings to the application class members.
			if (pTheApp != NULL)
			{
				pTheApp->SetProgramPath(NULL_TEXT);
			}
		}


		/* Header Path Setting */

		// Get the user selected value from the dialog.
		szHeaderPath = wndOptionsDlg.GetHeaderPath();


		// Verify that a value was selected.
		if (!szHeaderPath.IsEmpty())
		{
			/* The user selected a valid value. */

			// Load all the path values to class members.
			if (pTheApp != NULL)
			{
				pTheApp->SetHeaderPath(szHeaderPath);
			}

			// Write the path value to the registry.
			if (pTheApp != NULL)
			{
				if ( pTheApp->WriteProfileString(UNICHECK_REG_SETTINGSKEY, UNICHECK_REG_HDRPATHKEY, szHeaderPath) )
				{
					/* If the path value is loaded to the registry appropriately, write a message
					   to the User Feedback Display window. */

					// Display the message in the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_SETAPPPATH + szHeaderPath), NULL_TEXT, FALSE);
					}
				}
				else
				{
					/* If the path value is not loaded to the registry, write a message to the 
					   User Feedback Display. */

					// Display the message to the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_XSETAPPPATH + szHeaderPath), NULL_TEXT, TRUE );
					}
				}
			}
		}
		else
		{
			/* No values appeared to be set by the user, even though the user pressed OK. */

			// Load NULL string to the application class member.
			if (pTheApp != NULL)
			{
				pTheApp->SetHeaderPath(NULL_TEXT);
			}
		}


		/* Source Path Setting */

		// Get the user selected value from the dialog.
		szSourcePath = wndOptionsDlg.GetSourcePath();


		// Verify that a value was selected.
		if (!szSourcePath.IsEmpty())
		{
			/* The user selected a valid value. */

			// Load all the path values to class members.
			if (pTheApp != NULL)
			{
				pTheApp->SetSourcePath(szSourcePath);
			}

			// Write the path value to the registry.
			if (pTheApp != NULL)
			{
				if ( pTheApp->WriteProfileString(UNICHECK_REG_SETTINGSKEY, UNICHECK_REG_SRCPATHKEY, szSourcePath) )
				{
					/* If the path value is loaded to the registry appropriately, write a message
					   to the User Feedback Display window. */

					// Display the message in the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_SETAPPPATH + szSourcePath), NULL_TEXT, FALSE);
					}
				}
				else
				{
					/* If the path value is not loaded to the registry, write a message to the 
					   User Feedback Display. */

					// Display the message to the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_XSETAPPPATH + szSourcePath), NULL_TEXT, TRUE );
					}
				}
			}
		}
		else
		{
			/* No values appeared to be set by the user, even though the user pressed OK. */

			// Load NULL string to the application class member.
			if (pTheApp != NULL)
			{
				pTheApp->SetSourcePath(NULL_TEXT);
			}
		}


		/* Create the Unicode scan control script. */

		// Only create the script if all components of the script have a value.
		if ( (!szHeaderPath.IsEmpty()) &&
			 (!szSourcePath.IsEmpty()) )
		{
			// Create the command script of the Unicode scan (method handles creation failure.).
			CreateUnicodeControlScript();
		}


		/* Fileset File Name Setting */

		// Get the user selected value from the dialog.
		szFilesetFile = wndOptionsDlg.GetFilesetFileName();


		// Verify that a value was selected.
		if (!szFilesetFile.IsEmpty())
		{
			/* The user selected a valid value. */

			// Load all the path value to class members.
			if (pTheApp != NULL)
			{
				pTheApp->SetFilesetFile(szFilesetFile);
			}

			// Write the file name value to the registry.
			if (pTheApp != NULL)
			{
				if ( pTheApp->WriteProfileString(UNICHECK_REG_SETTINGSKEY, UNICHECK_REG_FILESETKEY, szFilesetFile) )
				{
					/* If the file name value is loaded to the registry appropriately, write a message
					   to the User Feedback Display window. */

					// Display the message in the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_SETFSFILE + szFilesetFile), NULL_TEXT, FALSE);
					}
				}
				else
				{
					/* If the file name value is not loaded to the registry, write a message to the 
					   User Feedback Display. */

					// Display the message to the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_XSETFSFILE + szFilesetFile), NULL_TEXT, TRUE );
					}
				}
			}
		}
		else
		{
			/* No values appeared to be set by the user, even though the user pressed OK. */

			// Load NULL string to the application class member.
			if (pTheApp != NULL)
			{
				pTheApp->SetFilesetFile(NULL_TEXT);
			}
		}


		/* Filest File Path Setting */

		// Get the user selected value from the dialog.
		szFilesetFilePath = wndOptionsDlg.GetFilesetFilePath();


		// Verify that a value was selected.
		if (!szFilesetFilePath.IsEmpty())
		{
			/* The user selected a valid value. */

			// Load all the path values to class members.
			if (pTheApp != NULL)
			{
				pTheApp->SetFilesetFilePath(szFilesetFilePath);
			}

			// Write the path value to the registry.
			if (pTheApp != NULL)
			{
				if ( pTheApp->WriteProfileString(UNICHECK_REG_SETTINGSKEY, UNICHECK_REG_FILESETPATHKEY, szFilesetFilePath) )
				{
					/* If the path value is loaded to the registry appropriately, write a message
					   to the User Feedback Display window. */

					// Display the message in the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_SETAPPPATH + szFilesetFilePath), NULL_TEXT, FALSE);
					}
				}
				else
				{
					/* If the path value is not loaded to the registry, write a message to the 
					   User Feedback Display. */

					// Display the message to the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_XSETAPPPATH + szFilesetFilePath), NULL_TEXT, TRUE );
					}
				}
			}
		}
		else
		{
			/* No values appeared to be set by the user, even though the user pressed OK. */

			// Load NULL string to the application class member.
			if (pTheApp != NULL)
			{
				pTheApp->SetFilesetFilePath(NULL_TEXT);
			}
		}


		/* Results Viewer Path Setting */

		// Get the user selected value from the dialog.
		szViewerPath = wndOptionsDlg.GetViewerPath();


		// Verify that a value was selected.
		if (!szViewerPath.IsEmpty())
		{
			/* The user selected a valid value. */

			// Load all the path values to class members.
			if (pTheApp != NULL)
			{
				pTheApp->SetViewerPath(szViewerPath);
			}

			// Write the path value to the registry.
			if (pTheApp != NULL)
			{
				if ( pTheApp->WriteProfileString(UNICHECK_REG_SETTINGSKEY, UNICHECK_REG_VIEWERPATHKEY, szViewerPath) )
				{
					/* If the path value is loaded to the registry appropriately, write a message
					   to the User Feedback Display window. */

					// Display the message in the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_SETAPPPATH + szViewerPath), NULL_TEXT, FALSE);
					}
				}
				else
				{
					/* If the path value is not loaded to the registry, write a message to the 
					   User Feedback Display. */

					// Display the message to the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_XSETAPPPATH + szViewerPath), NULL_TEXT, TRUE );
					}
				}
			}
		}
		else
		{
			/* No values appeared to be set by the user, even though the user pressed OK. */

			// Load NULL string to the application class member.
			if (pTheApp != NULL)
			{
				pTheApp->SetViewerPath(NULL_TEXT);
			}
		}


		/* Max Files Setting */

		// Get the user selected value from the dialog.
		nMaxFiles = wndOptionsDlg.GetMaxFile();


		// Verify that a valid value was selected.
		if ( (nMaxFiles >= UNICHECK_REG_MAXFILEFLOOR) && (nMaxFiles <= UNICHECK_REG_MAXFILECEILING) )
		{
			/* The user selected a valid value. */

			// Load all the path values to class members.
			if ( pTheApp != NULL )
			{
				pTheApp->SetMaxFiles(nMaxFiles);
			}

		
			// Write the max. files value to the registry.
			if ( pTheApp != NULL )
			{
				if ( !pTheApp->WriteProfileInt(UNICHECK_REG_SETTINGSKEY, UNICHECK_REG_MAXFILEKEY, nMaxFiles) )
				{
					/* If the max. files value is not loaded to the registry, write a message to the 
					   User Feedback Display. */

					// Display the message to the User Feedback Display window.
					if (pTheApp != NULL)
					{
						pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (UNICHECK_MESSAGE_XSETMAXFILE), NULL_TEXT, TRUE );
					}
				}
			}
		}
		else
		{
			/* No valid value appeared to be set by the user, even though the user pressed OK and the Options dialog validates the value. */

			// Load the default value to the application class member.
			if ( pTheApp != NULL )
			{
				pTheApp->SetMaxFiles(UNICHECK_REG_MAXFILEDEFAULT);
			}
		}
	}	
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView Menu State Handlers

/* OnUpdateFileSave - Save Menu Update Handler */
void CUniCheckView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// Call generic handler for this type of menu.
	ManageSaveMenusStates(pCmdUI);
}

/* OnUpdateEditAddfile - Add File Menu Update Handler */
void CUniCheckView::OnUpdateEditAddfile(CCmdUI* pCmdUI) 
{
	// Call generic handler for this type of menu.
	ManageBaseMenusStates(pCmdUI, &m_wndAddFilesButton);
}

/* OnUpdateEditReloadfile - Reload File Menu Update Handler */
void CUniCheckView::OnUpdateEditReloadfile(CCmdUI* pCmdUI) 
{
	// Call generic handler for this type of menu.
	ManageEditMenuStates(pCmdUI);
}

/* OnUpdateEditDelete - Delete Menu State Handler */
void CUniCheckView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	// Call generic handler for this type of menu.
	ManageEditMenuStates(pCmdUI);
}

/* OnUpdateValidateUnicode - Validate Menu State Handler */
void CUniCheckView::OnUpdateValidateValidate(CCmdUI* pCmdUI) 
{
	// Call generic handler for this type of menu.
	ManageValidationMenuStates(pCmdUI, &m_wndValidateButton);
}

/* OnUpdateValidateViewresults - View Results Menu State Handler */
void CUniCheckView::OnUpdateValidateViewresults(CCmdUI* pCmdUI) 
{
	// Call generic handler for this type of menu.
	ManageValidationMenuStates(pCmdUI, &m_wndResultsButton);
}

/* OnUpdateToolsClearresultsfile - Clear Results Files Menu State Handler */
void CUniCheckView::OnUpdateToolsClearresultsfile(CCmdUI* pCmdUI) 
{
	// Call generic handler for this type of menu.
	ManageValidationMenuStates(pCmdUI, NULL);	
}

/* OnUpdateToolsScandef - Scan Definitions Files Menu State Handler */
void CUniCheckView::OnUpdateToolsScandef(CCmdUI* pCmdUI) 
{
	// Call generic handler for this type of menu.
	ManageBaseMenusStates(pCmdUI, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView Generic Menu State Handlers

/* ManageBaseMenusStates - Generic Menu State Handler for Menus Needing Only an Initialized State */
void CUniCheckView::ManageBaseMenusStates(CCmdUI* pCmdUI, CButton* pButton)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp *) AfxGetApp();


	if (pTheApp != NULL)
	{
		if ( !pTheApp->IsAppInitialized() )
		{
			// If the application is not properly initialized, disable this type of menu
			// and appropriate form controls.

			// The Menu Item
			if (pCmdUI != NULL)
			{
				pCmdUI->Enable(FALSE);
			}

			// The Corresponding Form Button
			if (pButton != NULL)
			{
				pButton->EnableWindow(FALSE);
			}

			// The Scans List Box Control
			m_wndScansListBox.EnableWindow(FALSE);
		}
		else
		{
			// If the program path is properly set, enable this type of menu and
			// appropriate form controls.

			// The Menu Item
			if (pCmdUI != NULL)
			{
				pCmdUI->Enable(TRUE);
			}

			// The Corresponding Form Button
			if (pButton != NULL)
			{
				pButton->EnableWindow(TRUE);
			}

			// The Scans List Box Control
			m_wndScansListBox.EnableWindow(TRUE);
		}
	}
}

/* ManageSaveMenusStates - Generic Menu State Handler for Menus Used for Saving a UniversalChecker List */
void CUniCheckView::ManageSaveMenusStates(CCmdUI* pCmdUI)
{
	/* Local Variable Declarations and Initialization */

	CString szFilePath;				// Path of the Open Document File
	CString szFileExtension;		// File Extension of the Open Document File
	CString szDocumentTitle;		// Title of the Open Document File Displayed in the Application Bar

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance
	CUniCheckDoc* pTheDoc = NULL;	// Pointer to the Document Instance


	// Get a reference to the application instance and document.
	pTheApp = (CUniCheckApp *) AfxGetApp();
	pTheDoc = GetDocument();


	// Get a copy of the document title to preserve it even though we're changing the file name.
	if (pTheDoc != NULL)
	{
		szDocumentTitle = pTheDoc->GetTitle();
	}

	// Get the file path for the open document.
	if (pTheDoc != NULL)
	{
		szFilePath = pTheDoc->GetPathName();
	}

	// Determine the type of file that's currently open.
	if (pTheApp != NULL)
	{
		szFileExtension = pTheApp->DetermineFileExtension(szFilePath);
	}

	// If the path is to a Visual Studio project file, change the file path to a UniversalChecker list file.
	if ( (szFileExtension == UNICHECK_FILETYPE_VS6) || (szFileExtension == UNICHECK_FILETYPE_VS7) )
	{
		if (pTheApp != NULL)
		{
			szFilePath = pTheApp->ChangeFileExtension(szFilePath, UNICHECK_FILETYPE_LIST);
		}

		// Update the new file path to the framework.
		if (pTheDoc != NULL)
		{
			pTheDoc->SetPathName(szFilePath, FALSE);
		}
	}

	// Determine the file extension of the document title to determine if the Save menu still needs to be disabled.
	if (pTheApp != NULL)
	{
		szFileExtension = pTheApp->DetermineFileExtension(szDocumentTitle);
	}


	// Disable the Save menu if the document title is a Visual Studio project file.  This is to signify to the
	// user that the project file is open, but cannot be saved as a Visual Studio project file.
	if ( (szFileExtension == UNICHECK_FILETYPE_VS6) || (szFileExtension == UNICHECK_FILETYPE_VS7) )
	{
		// If the open document not a UniversalChecker list (i.e. a Visual Studio project file), 
		// disable this type of menu.

		// The Menu Item
		if (pCmdUI != NULL)
		{
			pCmdUI->Enable(FALSE);
		}
	}
	else
	{
		// If the open document is a UniversalChecker list, enable this type of menu.

		// The Menu Item
		if (pCmdUI != NULL)
		{
			pCmdUI->Enable(TRUE);
		}
	}

	// Update the preserved title to the framework.
	if (pTheDoc != NULL)
	{
		pTheDoc->SetTitle(szDocumentTitle);
	}
}

/* ManageEditMenuStates - Generic Menu State Handler for Edit Menus */
void CUniCheckView::ManageEditMenuStates(CCmdUI *pCmdUI)
{
	/* Local Variable Declarations and Initialization */

	CString nodeName;			// Text of the Currenly Selected Node of the Tree Control

	HTREEITEM hSelectedNode;	// Handle to the Currently Selected Node of Tree Control


	// Get a reference to the selected tree control node.
	hSelectedNode = m_wndLoadedFilesTree.GetSelectedItem();

	// Get the text of the selected tree node.
	if (hSelectedNode != NULL)
	{
		nodeName = m_wndLoadedFilesTree.GetItemText(hSelectedNode);
	}

	// Verify that the node text was retrieved.
	if (!nodeName.IsEmpty())
	{
		// If the selected node is one of the base classification nodes, disable the Delete menu item.
		if ( (nodeName == UNICHECK_FILETREE_ROOTNODE)	||
			 (nodeName == UNICHECK_FILETREE_SOURCENODE) ||
			 (nodeName == UNICHECK_FILETREE_HEADERNODE) )
		{
			if (pCmdUI != NULL)
			{
				pCmdUI->Enable(FALSE);
			}
		}
		else
		{
			// If the selected node is not a base classification node, enable the Delete menu item.
			if (pCmdUI != NULL)
			{
				pCmdUI->Enable(TRUE);
			}
		}
	}
	else
	{
		// If no node text was retrieved, disable the delete menu item.
		if (pCmdUI != NULL)
		{
			pCmdUI->Enable(FALSE);
		}
	}
}

/* ManageValidationMenuStates - Generic Menu State Handler for Validation Menus */
void CUniCheckView::ManageValidationMenuStates(CCmdUI *pCmdUI, CButton* pButton)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp *) AfxGetApp();


	// If the application is not properly initialized, disable this type of menu and form button.
	if (pTheApp != NULL)
	{
		if ( !pTheApp->IsAppInitialized() )
		{
			if (pTheApp != NULL)
			{
				pCmdUI->Enable(FALSE);
			}

			if (pButton != NULL)
			{
				pButton->EnableWindow(FALSE);
			}
		}
		else
		{
			// If the program path is properly set, check if a scan type is selected.
			if( !IsScanSelected() ) 
			{
				// If no scan type is selected, disable this type of menu and form button.
				if (pTheApp != NULL)
				{
					pCmdUI->Enable(FALSE);
				}

				if (pButton != NULL)
				{
					pButton->EnableWindow(FALSE);
				}
			}
			else
			{
				// If no scan type is selected, enable this type of menu and form button.
				if (pTheApp != NULL)
				{
					pCmdUI->Enable(TRUE);
				}

				if (pButton != NULL)
				{
					pButton->EnableWindow(TRUE);
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView Utility Functions

/* AddFileToLoadedFilesTreeControl - Load Source/Head Files to the Loaded Files Tree Control */
BOOL CUniCheckView::AddFileToLoadedFilesTreeControl(DSLOADEDFILE dsFile)
{
	/* Local Variable Declarations and Initialization */

	BOOL bLoadedFile = FALSE;		// Flag Specifying that the File is Already Part of the Document

	CString szNodeName;				// Text of File Node in the Loaded Files Tree Control
	CString szCategory;				// Category Associated with the File's Type

	HTREEITEM hTreeNode = NULL;		// Handle to the Loaded Files Tree Root Node
	HTREEITEM hGroupNode = NULL;	// Handle to the Source and Header Group Nodes
	HTREEITEM hFileNode = NULL;		// Handle to an Individual File Node

	TVINSERTSTRUCT dsTreeStruct;	// Tree Control Information Structure

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Initialize stuctures.
	ZeroMemory(&dsTreeStruct, sizeof(TVINSERTSTRUCT));


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Get a handle to the tree control root node.
	hTreeNode = m_wndLoadedFilesTree.GetRootItem();


	// Get the category for the file type.
	if ( pTheApp != NULL )
	{
		if ( !pTheApp->GetValidFileType(dsFile.szFileExtension, NULL, &szCategory) )
		{
			// If the category could not be retreived, exit and signal the file could not be added.
			return FALSE;
		}
	}


	// Get a reference to the appropriate tree control group node.
	if ( (dsFile.szFileExtension == UNICHECK_FILETYPE_HEADER) || (szCategory == UNICHECK_GROUPING_HEADER) )
	{
		/* If the file is a C/CPP header file (.h) (to ensure the integrated Unicode scan works) or the valid file type
		   is associated with the header group, get a reference to the Header Files tree node. */

		// Get a handle to the Source Files group node.
		if (hTreeNode != NULL)
		{
			hGroupNode = m_wndLoadedFilesTree.GetChildItem(hTreeNode);
		}

		if (hGroupNode != NULL)
		{
			hGroupNode = m_wndLoadedFilesTree.GetNextItem(hGroupNode, TVGN_NEXT);
		}

		// Get a handle to the first file node under the Source Files node.
		if (hGroupNode != NULL)
		{
			hFileNode = m_wndLoadedFilesTree.GetChildItem(hGroupNode);
		}
	}
	else
	{
		/* If the file is a C++ source file (.cpp) (to ensure the integrated Unicode scan works) or the valid file type
		   is associated with the source group, get a reference to the Source Files tree node. */

		if ( (dsFile.szFileExtension == UNICHECK_FILETYPE_SOURCE) || (szCategory == UNICHECK_GROUPING_SOURCE) )
		{
			// Get a handle to the Source Files group node.
			if (hTreeNode != NULL)
			{
				hGroupNode = m_wndLoadedFilesTree.GetChildItem(hTreeNode);
			}

			// Get a handle to the first file node under the Source Files node.
			if (hGroupNode != NULL)
			{
				hFileNode = m_wndLoadedFilesTree.GetChildItem(hGroupNode);
			}
		}
		else
		{
			// If the file extension or category are not recognized, exit signalling the file could not be added and display 
			// an error to the user.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_XTREEUPDATE, UNICHECK_MESSAGE_SEEVALTYPE, TRUE);
				return FALSE;
			}
		}
	}

	
	// Verify that the file has not already been loaded.
	while ( (hFileNode != NULL) && (bLoadedFile != TRUE) )
	{
		/* Cycle through the currently loaded files nodes. */

		// Get the name of the file from the tree node.
		if (hFileNode != NULL)
		{
			szNodeName = m_wndLoadedFilesTree.GetItemText(hFileNode);
		}

		// Compare the node text to the file name.
		if (szNodeName == dsFile.szFileName)
		{
			// If the file names match, set a flag to specify that the file was already loaded.
			bLoadedFile = TRUE;
		}

		// Get the next file node.
		if (hFileNode != NULL)
		{
			hFileNode = m_wndLoadedFilesTree.GetNextItem(hFileNode, TVGN_NEXT);
		}
	}


	// If the file was not already a member of the file tree, load it to the file tree.
	if (bLoadedFile != TRUE)
	{
		if (hGroupNode != NULL)
		{
			dsTreeStruct.hParent = hGroupNode;
			dsTreeStruct.hInsertAfter = NULL;
			dsTreeStruct.item.mask = TVIF_TEXT;
			dsTreeStruct.item.pszText = dsFile.szFileName.GetBufferSetLength(dsFile.szFileName.GetLength());

			// Load the new node to the loaded files tree control.
			if (m_wndLoadedFilesTree.InsertItem(&dsTreeStruct) == NULL)
			{
				// If the file could not be added to the user interface, display an error to the user and signal failure.
				if (pTheApp != NULL)
				{
					pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_XTREEUPDATE, NULL_TEXT, TRUE);
					return FALSE;
				}
			}
		}
	}


	// Redraw the file tree control.
	m_wndLoadedFilesTree.Invalidate();


	// Return that the file was added to the tree.
	return TRUE;
}

/* CopyFileToAnalysisLocation - Copy a File to the Appropriate Temporary Folder for Scan Analysis */
BOOL CUniCheckView::CopyFileToAnalysisLocation(DSLOADEDFILE dsFile)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance
	CUniCheckDoc* pTheDoc = NULL;	// Pointer to the Document


	// Get references to the application instance and document.
	pTheApp = (CUniCheckApp*) AfxGetApp();
	pTheDoc = GetDocument();

	// Copy the file to the appropriate directory.

	if (pTheDoc != NULL)
	{
		if ( !pTheDoc->CopyFileToTempDirectory(dsFile.szFileName) )
		{
			// If the copy API failed, display an error to the user and signal failure.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_FILESP + dsFile.szFileName + UNICHECK_MESSAGE_FAILLOADFROM + dsFile.szOriginalPath + PERIOD), NULL_TEXT, TRUE);
			}
			return FALSE;
		}
		else
		{
			// If the copy API succeeded, display a message to the user and signal success.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_FILESP + dsFile.szFileName + UNICHECK_MESSAGE_LOADFROM + dsFile.szOriginalPath + PERIOD), NULL_TEXT, FALSE);
			}
			return TRUE;
		}
	}

	// As a fail-safe, signal failure.
	return FALSE;
}

/* RemoveSelectedFile - Remove the Selected File from the Document and Temporary Folder Locations */
BOOL CUniCheckView::RemoveSelectedFile(CString szNodeName)
{
	/* Local Variable Declarations and Initialization */

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance
	CUniCheckDoc* pTheDoc = NULL;	// Pointer to the Document


	// Get references to the application instance and document.
	pTheApp = (CUniCheckApp*) AfxGetApp();
	pTheDoc = GetDocument();


	// Delete the file from the temporary directory and document data.
	if (pTheDoc != NULL)
	{
		switch(pTheDoc->DeleteSelectedFile(szNodeName))
		{
		case UNICHECK_RETURN_FAILED:
			// If the delete API failed, display an error to the user and signal failure.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_FILESP + szNodeName + UNICHECK_MESSAGE_XREMOVE), NULL_TEXT, TRUE);
			}
			return FALSE;
			break;

		case UNICHECK_RETURN_SUCCESS:
			// If the delete API succeeded, display a message to the user and signal success.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_FILESP + szNodeName + UNICHECK_MESSAGE_REMOVE), NULL_TEXT, FALSE);
			}
			return TRUE;
			break;

		case UNICHECK_RETURN_FILECORRUPT:
			// If the document data is corrupt, display an error to the user and kill the application instance.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_FILECORUPT, UNICHECK_MESSAGE_APPMUSTDIE, TRUE);
			}
			if (pTheApp != NULL)
			{
				pTheApp->KillApp();
			}
			break;
			
		default:
			break;
		}
	}

	// As a fail-safe, signal failure.
	// The code should only reach this point if the document data is corrupt.
	return FALSE;
}

/* IsScanSelected - Determines If Any Scan has been Selected */
BOOL CUniCheckView::IsScanSelected()
{
	/* Local Variable Declarations and Initialization */
	
	int nNumberOfScans = 0;			// Number of Items in the Scans List Box


	// Get the number of scans in the list box.
	nNumberOfScans = m_wndScansListBox.GetCount();

	// Cycle through each item in the list box.
	for(int i = 0; i < nNumberOfScans; i++)
	{
		// If any scan is selected, signal success.
		if ( m_wndScansListBox.GetCheck(i) )
		{
			return TRUE;
		}
	}

	// If the entire contents of the scans list box have been examined and none is checked (i.e. the method hasn't 
	// exited and signalled success yet), signal failure.
	return FALSE;
}

/* IsScanSelected - Determines If the Requested Scan has been Selected */
BOOL CUniCheckView::IsScanSelected(UINT uType)
{
	/* Local Variable Declarations and Initialization */
	
	int nScanIndex = 0;			// The Index of the Requested Scan in the Scans List Box


	// Get the index of the requested scan.
	nScanIndex = GetScanIndex(uType);

	// If the requested scan is selected, signal success.
	if ( m_wndScansListBox.GetCheck(nScanIndex) )
	{
		return TRUE;
	}
	else
	{
		// The scan is not selected, signal failure.
		return FALSE;
	}
}

/* EnableScan - Enable or Disable All Scans in the Scans List Box Control */
void CUniCheckView::EnableScan(BOOL bEnable)
{
	/* Local Variable Declarations and Initialization */
	
	int nNumberOfScans = 0;	// Number of Items in the Scans List Box


	// Get the number of scans in the list box.
	nNumberOfScans = m_wndScansListBox.GetCount();

	/* Cycle through each scan in the list box control and toggle its enabled state. */

	// Cycle through each item in the list box.
	for(int i = 0; i < nNumberOfScans; i++)
	{
		// Only toggle the scan's enabled state if is different from the requested state.
		if ( m_wndScansListBox.IsEnabled(i) != bEnable )
		{
			// Clear the selection of each scan.
			m_wndScansListBox.SetCheck(i, BST_UNCHECKED);

			// Toggle the item's enabled state
			m_wndScansListBox.Enable(i, bEnable);
		}
	}
}

/* EnableScan - Enable or Disable a Scan in the Scans List Box Control */
void CUniCheckView::EnableScan(UINT uType, BOOL bEnable)
{
	/* Local Variable Declarations and Initialization */
	
	int nScanIndex = 0;		// The Index of the Requested Scan in the List Box


	// Get the index of the requested scan.
	nScanIndex = GetScanIndex(uType);

	// Only attempt to disable if the scan was found in the list box.
	if ( nScanIndex != UNICHECK_RETURN_NOINDEX )
	{
		// Only toggle the scan's enabled state if is different from the requested state.
		if ( m_wndScansListBox.IsEnabled(nScanIndex) != bEnable )
		{
			// Clear the selection of the selected scan.
			m_wndScansListBox.SetCheck(nScanIndex, BST_UNCHECKED);

			// Toggle the scan's enabled state.
			m_wndScansListBox.Enable(nScanIndex, bEnable);
		}
	}
}

/* GetScanIndex - Retrieve the Scans List Box Index of the Requested Scan */
int CUniCheckView::GetScanIndex(UINT uType)
{
	/* Local Variable Declarations and Initialization */
	
	int nNumberOfScans = 0;			// Number of Items in the Scans List Box


	// Get the number of scans in the list box.
	nNumberOfScans = m_wndScansListBox.GetCount();

	// Cycle through each item in the list box.
	for(int i = 0; i < nNumberOfScans; i++)
	{
		// If any scan is selected, return the current index.
		if ( m_wndScansListBox.GetItemData(i) == uType )
		{
			return i;
		}
	}

	// If the entire contents of the scans list box have been examined and none is checked (i.e. the method hasn't 
	// exited yet), signal failure.
	return UNICHECK_RETURN_NOINDEX;
}

/* DeleteResultFile - Delete a Result File and Display Appropriate Message */
void CUniCheckView::DeleteResultsFile(CString szFileName, CString szScanType)
{
	/* Local Variable Declarations and Initialization */
	
	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance

	
	// Grab a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Delete the result file.
	if (pTheApp != NULL)
	{
		if(!pTheApp->DeleteSpecificResultsFile(szFileName))
		{
			// If the delete API failed, write an error to the User Feedback Display window.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_NOSP + szScanType + UNICHECK_MESSAGE_RESTOCLEAR), NULL_TEXT, FALSE);
			}
		}
		else
		{
			// If the delete API succeeded, write a message to signify the deletion to the User Feedback Display window.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_FILESP + szFileName + UNICHECK_MESSAGE_DELETED),
											  (szScanType + UNICHECK_MESSAGE_RESCLEARED), FALSE);
			}
		}
	}
}

/* CreateSetFileForScanInput - Build a Psuedo Visual Studio 6 Project File for Use by Scan Utilities */
UINT CUniCheckView::CreateSetFileForScanInput()
{
	/* Local Variable Declarations and Initialization */
	
	UINT uCreateResult = UNICHECK_RETURN_FAILED;	// The Return Value from CreateSet
	
	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance
	CUniCheckDoc* pTheDoc = NULL;	// Pointer to the Document


	// Grab references to the application instance and document.
	pTheApp = (CUniCheckApp*) AfxGetApp();
	pTheDoc = GetDocument();

	// Attempt to create the set file.
	if (pTheDoc != NULL)
	{
		uCreateResult = pTheDoc->CreateSet();
	}

	// Process the results of attempting to create the set file.
	switch (uCreateResult)
	{
	case UNICHECK_RETURN_FAILED:
		// Display an error if the file could not be created.
		if (pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_XCREATESET, NULL_TEXT, TRUE);
		}
		break;

	case UNICHECK_RETURN_INITIAL:
		// If the file was not deleted, write message to the user stating
		// that the initial fileset file is being created.
		if (pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_CREATE1SET, NULL_TEXT, FALSE);
		}
		break;

	case UNICHECK_RETURN_REFRESH:
		// If the file was deleted, write message to the user stating
		// that the fileset file is being re-created.
		if (pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_RESETSET, NULL_TEXT, FALSE);
		}
		break;

	case UNICHECK_RETURN_FILECORRUPT:
		// If the document data is corrupt, display an error to the user and kill the application instance.
		if (pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_FILECORUPT, UNICHECK_MESSAGE_APPMUSTDIE, TRUE);
		}
		if (pTheApp != NULL)
		{
			pTheApp->KillApp();
		}
		break;

	default:
		break;
	}

	// Return the result of CreateSet().
	return uCreateResult;
}

/* CreateUnicodeControlScript - Build a Control Text File Required by the Unicode Scan */
BOOL CUniCheckView::CreateUnicodeControlScript()
{
	/* Local Variable Declarations and Initialization */
	
	UINT uCreateResult = UNICHECK_RETURN_FAILED;	// The Return Value from CreatePatternSet
	
	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Grab references to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Attempt to create the set file.
	if (pTheApp != NULL)
	{
		uCreateResult = pTheApp->CreateControlFile();
	}

	// Process the results of attempting to create the set file.
	switch (uCreateResult)
	{
	case UNICHECK_RETURN_SUCCESS:
		// Write message to the user stating that the Unicode control script is being created.
		if (pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_CREATESCRIPT, NULL_TEXT, FALSE);
		}

		// Return and signal that the script was created successfully.
		return TRUE;
		break;

	case UNICHECK_RETURN_FAILED:
		// Display an error if the program path is not set.
		if (pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_APPXSET, UNICHECK_MESSAGE_VERPATHSET, TRUE);
		}

		// Return and signal failure.
		return FALSE;
		break;

	case UNICHECK_RETURN_FILEIOFAIL:
		// If the script file could not be created, display an error to the user and disable the Unicode scan.
		if (pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, UNICHECK_MESSAGE_XCREATESCRPT, UNICHECK_MESSAGE_XUNICODESCAN, TRUE);
		}

		// Disable the Unicode scan.
		EnableScan(UNICHECK_SCANTYPE_UNICODE, FALSE);

		// Because the inability to create the script file could indicate a wide variety of problems on the target system, 
		// no path to remedy the problem (re-enable the Unicode scan without closing and re-opening the application) 
		// will be provided.

		// Return and signal failure.
		return FALSE;
		break;

	default:
		// If an unknown result is returned, return and signal failure.
		return FALSE;
		break;
	}
}

/* RunScan - Run a Specific Scan Over the Document Data */
void CUniCheckView::RunScan(CString szScanExecutable, CString szScanArguments, CString szScanType)
{
	/* Local Variable Declarations and Initialization */
	
	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance

	
	// Grab a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Lauch the scan process.
	if (pTheApp != NULL)
	{
		if(pTheApp->ExecuteScanProcess(szScanExecutable, szScanArguments))
		{
			// Write message to the User Feedback Display window.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser( &m_wndMonitorWindow, (szScanType + UNICHECK_MESSAGE_COMP), NULL_TEXT, FALSE );
			}
		}
		else
		{
			// Write an error message to the User Feedback Display window.
			if (pTheApp != NULL)
			{
				pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_VALENG + szScanExecutable + UNICHECK_MESSAGE_FAILCOMP), 
									 (szScanType + UNICHECK_MESSAGE_XCOMP), TRUE);
			}
		}
	}
}

/* ViewResults - Display the Results of a Scan */
void CUniCheckView::ViewResults(CString szExecutablePath, CString szResultsFileName, CString szResultsType)
{
	/* Local Variable Declarations and Initialization */

	UINT uViewResult = UNICHECK_RETURN_FAILED;	// The Return Value from ExecuteViewResultsProcess
	
	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance

	
	// Grab a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Open the results file in an appropriate viewer.
	if(pTheApp != NULL)
	{
		uViewResult = pTheApp->ExecuteViewResultsProcess(szExecutablePath, szResultsFileName);
	}

	// Process the results of attempting to open the input file.
	switch (uViewResult)
	{
	case UNICHECK_RETURN_FAILED:
		// If the process failed to launch, write a message to the user.
		if(pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (UNICHECK_MESSAGE_CNLOCATE + szExecutablePath + UNICHECK_MESSAGE_TODISPRES), NULL_TEXT, TRUE);
		}
		break;

	case UNICHECK_RETURN_SUCCESS:
		// If everything processed OK, display a message in the user showing that processing
		// is done.
		if(pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (szResultsType + UNICHECK_MESSAGE_RESDISP), NULL_TEXT, FALSE);
		}
		break;

	case UNICHECK_RETURN_NOFILE:
		// If the results file does not exist, display an error in the user.
		if(pTheApp != NULL)
		{
			pTheApp->DisplayMessageToUser(&m_wndMonitorWindow, (szResultsType + UNICHECK_MESSAGE_RESXEXIST), NULL_TEXT, FALSE);
		}
		break;

	default:
		break;
	}
}
