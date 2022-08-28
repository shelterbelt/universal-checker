// UniCheck.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "UniCheck.h"

#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CUniCheckApp

BEGIN_MESSAGE_MAP(CUniCheckApp, CWinApp)
	//{{AFX_MSG_MAP(CUniCheckApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUniCheckApp Construction

CUniCheckApp::CUniCheckApp()
{
	// Place all significant initialization in InitInstance.
	m_szHeaderPath = NULL_TEXT;
	m_szProgramPath = NULL_TEXT;
	m_szSourcePath = NULL_TEXT;
	m_nMaxFiles = 0;
	m_uProcessingMode = NULL;
	m_uScanTypeSeed = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// The One and Only CUniCheckApp Object

CUniCheckApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CUniCheckApp Initialization

BOOL CUniCheckApp::InitInstance()
{
	// Set the default processing mode.
	SetProcessingMode(UNICHECK_MODE_INTERACTIVE);	// Flag to specify which mode the application is running, interactive or command line mode

	// Load 3D control libraries.
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL.
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically.
#endif

	// Specify location of the application settings in the registry.
	SetRegistryKey(UNICHECK_REG_ROOTKEY);

	LoadStdProfileSettings();  // Load standard INI file options (including MRU).

	// Load the application settings from the registry.
	LoadApplicationSettings();

	// Register the application's document templates.
	// The application only handles one document type, UniversalChecker list (.ucl), with a single application view.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CUniCheckDoc),
		RUNTIME_CLASS(CMainFrame),       // Main SDI Frame Window
		RUNTIME_CLASS(CUniCheckView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open.
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	/* Application Mode Processing */

	// Process any files passed as parameters on the command line.
	switch(ProcessCommandLineFiles(&cmdInfo))
	{
	case UNICHECK_RETURN_FAILED:
		// If a non-C source file (.c), non-C/CPP header file (.h), non-Visual Studio project (.dsp or .vcproj), or 
		// non-UniversalChecker list (.ucl) is passed as a parameter on the command line, exit the application 
		// without further processing.
		return FALSE;
		break;

	case UNICHECK_MODE_COMMANDLINE:
		// If the application is running in command line mode, scan the input file, display the results, and cleanup the application.
		CommandLineModeProcessing(&cmdInfo);

		// Exit the application once command line mode processing finishes.
		return FALSE;
		break;

	default:
		// If the application should be running in interactive mode (either no file was passed as a parameter or a UniversalChecker 
		// list was passed as a parameter), run the application as normal.
		break;
	}


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;  // Exit the application if the command line parameters could not be processed.

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();  // Allow the user to drop a UniversalChecker list directly on the application window to open it.

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckApp Message Handlers

/* OnFileOpen - App Command to Run the "Open File" Dialog */
void CUniCheckApp::OnFileOpen() 
{
	/* This override is needed to add the Visual Studio project file types to the filter, and to remove "All Files (*.*)" from the filter. */

	/* Local Variable Declarations and Initialization */

	int nDialogResult = IDCANCEL;	// Windows Shell Select File Dialog Return Value

	CString szFilePath;				// Full Path of the File Selected by the User


	// Set the extension filters for the "Open File" dialog.
	static char BASED_CODE szFilter[] = UNICHECK_OPENFILEDLG_FILTER;
 
	// Create the "Open File" dialog.
	CFileDialog wndOpenFileDialog(TRUE, NULL, NULL, OFN_EXPLORER|OFN_HIDEREADONLY, szFilter, GetMainWnd());


	// Display the "Open File" dialog.
	nDialogResult = wndOpenFileDialog.DoModal();

	// If the user selected a file and confirmed the select (pressed OK instead of Cancel), open the file.
	if (nDialogResult == IDOK)
	{
		// 
		// Get the selected file path from the dialog.
		szFilePath = wndOpenFileDialog.GetPathName();

		// Open the document file.
		if ( !szFilePath.IsEmpty() )
		{
			OpenDocumentFile(szFilePath);
		}
	}
}

/* OnAppAbout - App Command to Run the "About" Dialog */
void CUniCheckApp::OnAppAbout()
{
	/* Local Variable Declarations and Initialization */

	CAboutDlg wndAboutDlg;		// The About Dialog Class Instance


	// Display the About Dialog window.
	wndAboutDlg.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckApp Class Member Access Methods

/* GetProgramPath - Assign a Value to the Class Member m_szProgramPath */
CString CUniCheckApp::GetProgramPath()
{
	return m_szProgramPath;
}

/* SetProgramPath - Return the Class Member m_szProgramPath */
void CUniCheckApp::SetProgramPath(CString szPath)
{
	// Validate the path and assign the processed value to the class member.
	m_szProgramPath = ValidatePath(szPath);
}

/* GetHeaderPath - Return the Class Member m_szHeaderPath */
CString CUniCheckApp::GetHeaderPath()
{
	return m_szHeaderPath;
}

/* SetHeaderPath - Assign a Value to the Class Member m_szHeaderPath */
void CUniCheckApp::SetHeaderPath(CString szPath)
{
	// Validate the path and assign the processed value to the class member.
	m_szHeaderPath = ValidatePath(szPath);
}

/* GetSourcePath - Return the Class Member m_szSourcePath */
CString CUniCheckApp::GetSourcePath()
{
	return m_szSourcePath;
}

/* SetSourcePath - Assign a Value to the Class Member m_szSourcePath */
void CUniCheckApp::SetSourcePath(CString szPath)
{
	// Validate the path and assign the processed value to the class member.
	m_szSourcePath = ValidatePath(szPath);
}

/* GetFilesetFile - Return the Class Member m_szFilesetFile */
CString CUniCheckApp::GetFilesetFile()
{
	return m_szFilesetFile;
}

/* SetFilesetFile - Assign a Value to the Class Member m_szFilesetFile */
void CUniCheckApp::SetFilesetFile(CString szFile)
{
	m_szFilesetFile = szFile;
}

/* GetFilesetFilePath - Return the Class Member m_szFilesetFilePath */
CString CUniCheckApp::GetFilesetFilePath()
{
	return m_szFilesetFilePath;
}

/* SetFilesetFilePath - Assign a Value to the Class Member m_szFilesetFilePath */
void CUniCheckApp::SetFilesetFilePath(CString szPath)
{
	// Validate the path and assign the processed value to the class member.
	m_szFilesetFilePath = ValidatePath(szPath);
}

/* GetViewerPath - Return the Class Member m_szViewerPath */
CString CUniCheckApp::GetViewerPath()
{
	return m_szViewerPath;
}

/* SetViewerPath - Assign a Value to the Class Member m_szViewerPath */
void CUniCheckApp::SetViewerPath(CString szPath)
{
	// Only assign the passed value if the path exists.
	if ( !VerifyThatFilePathExists(szPath) )
	{
		// If the executable does not exist, then assign blank.
		szPath = NULL_TEXT;
	}
	else
	{
		// If the executable does exist, assign the valid path to the class member.
		m_szViewerPath = szPath;
	}
}

/* GetMaxFiles - Return the Class Member m_nMaxFiles */
int CUniCheckApp::GetMaxFiles()
{
	return m_nMaxFiles;
}

/* SetMaxFiles - Assign a Value to the Class Member m_nMaxFiles */
void CUniCheckApp::SetMaxFiles(int nMaxNumber)
{
	m_nMaxFiles = nMaxNumber;
}

/* GetProcessingMode - Return the Class Member m_ProcessingMode */
UINT CUniCheckApp::GetProcessingMode()
{
	return m_uProcessingMode;
}

/* SetProcessingMode - Assign a Value to the Class Member m_ProcessingMode */
void CUniCheckApp::SetProcessingMode(UINT uMode)
{
	m_uProcessingMode = uMode;
}

/* GetScanTypeSeed - Return the Class Member m_uScanTypeSeed */
UINT CUniCheckApp::GetScanTypeSeed()
{
	return m_uScanTypeSeed;
}

/* SetScanTypeSeed - Assign a Value to the Class Member m_uScanTypeSeed */
void CUniCheckApp::SetScanTypeSeed(UINT uLastScanType)
{
	m_uScanTypeSeed = uLastScanType;
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckApp Utility Methods

/* LoadApplicationSettings - Retrieve All Application Settings from Registry, Custom .ini, etc... */
void CUniCheckApp::LoadApplicationSettings()
{
	/* Local Variable Declarations and Initialization */

	int nMaxFiles = 0;				// Maximum Number of Files That Can Be Loaded to the List at One Time
	int nRegistryDefaultValue = 0;	// Default Value to Use if No Value Found in the Registry

	CString szRegistryKey;			// Application Registry Key Name
	CString szRegistryValue;		// Program Path Registry Key Name
	CString szProgramPath;			// Directory Path of the Program Directory
	CString szHeaderPath;			// Directory Path of the Header Files Temporary Directory
	CString szSourcePath;			// Directory Path of the Source Files Temporary Directory
	CString szFilesetFile;			// Name of the Psuedo-Visual Studio File to Use as Scan Input
	CString szFilesetFilePath;		// Path to the Psuedo-Visual Studio File to Use as Scan Input
	CString szViewerPath;			// Path of the Executable to View Scan Results


	/* Program Path Setting */

	// Load the registry key strings to get the program path string from the registry.
	szRegistryKey = UNICHECK_REG_SETTINGSKEY;
	szRegistryValue = UNICHECK_REG_PROGPATHKEY;

	// Get the program path from the registry.
	szProgramPath = GetProfileString(szRegistryKey, szRegistryValue, NULL);

	// Check if a value was retrieved for the program directory.
	if ( szProgramPath.IsEmpty() )
	{
		// If the program path does not have a value, load and empty string to the program path class member.
		SetProgramPath(NULL_TEXT);
	}
	else
	{
		// Load the retrieved value to a class member.
		SetProgramPath(szProgramPath);
	}


	/* Header Path Setting */

	// Load the registry key strings to get the header files temporary path string from the registry.
	szRegistryValue = UNICHECK_REG_HDRPATHKEY;

	// Get the header files temporary path from the registry.
	szHeaderPath = GetProfileString(szRegistryKey, szRegistryValue, NULL);

	// Check if a value was retrieved for the header directory.
	if ( szHeaderPath.IsEmpty() )
	{
		// If the header path does not have a value, load empty string to the header path class member.
		SetHeaderPath(NULL_TEXT);
	}
	else
	{
		// Load the retrieved value to a class member.
		SetHeaderPath(szHeaderPath);
	}


	/* Source Path Setting */

	// Load the registry key strings to get the source files temporary path string from the registry.
	szRegistryValue = UNICHECK_REG_SRCPATHKEY;

	// Get the source files temporary path from the registry.
	szSourcePath = GetProfileString(szRegistryKey, szRegistryValue, NULL);

	// Check if a value was retrieved for the source directory.
	if ( szSourcePath.IsEmpty() )
	{
		// If the source path does not have a value, load empty string to the source path class member.
		SetSourcePath(NULL_TEXT);
	}
	else
	{
		// Load the retrieved value to a class member.
		SetSourcePath(szSourcePath);
	}


	/* Create the Unicode scan command script. */

	// Only create the script if all components of the script have a value.
	if ( (!szHeaderPath.IsEmpty()) &&
		 (!szSourcePath.IsEmpty()) )
	{
		// Create the command script of the Unicode scan.
		if ( CreateControlFile() != UNICHECK_RETURN_SUCCESS)
		{
			// If the command script failed to create, notify the user and prompt to verify the application settings.
			DisplayMessageToUser(NULL, UNICHECK_MESSAGE_XINITUNICODE, UNICHECK_MESSAGE_CHECKOPTS, TRUE);
		}
	}


	/* Fileset File Setting */

	// Load the registry key strings to get the fileset file string from the registry.
	szRegistryValue = UNICHECK_REG_FILESETKEY;

	// Get the fileset file from the registry.
	szFilesetFile = GetProfileString(szRegistryKey, szRegistryValue, NULL);

	// Check if a value was retrieved for the fileset file.
	if ( szFilesetFile.IsEmpty() )
	{
		// If the fileset file does not have a value, load empty string to the fileset file class member.
		SetFilesetFile(NULL_TEXT);
	}
	else
	{
		// Load the retrieved value to a class member.
		SetFilesetFile(szFilesetFile);
	}


	/* Fileset File Path Setting */

	// Load the registry key strings to get the fileset file path string from the registry.
	szRegistryValue = UNICHECK_REG_FILESETPATHKEY;

	// Get the fileset file path from the registry.
	szFilesetFilePath = GetProfileString(szRegistryKey, szRegistryValue, NULL);

	// Check if a value was retrieved for the fileset file path.
	if ( szFilesetFilePath.IsEmpty() )
	{
		// If the fileset file path does not have a value, load empty string to the fileset file path class member.
		SetFilesetFilePath(NULL_TEXT);
	}
	else
	{
		// Load the retrieved value to a class member.
		SetFilesetFilePath(szFilesetFilePath);
	}


	/* Results Viewer Path Setting */

	// Load the registry key strings to get the viewer for results path string from the registry.
	szRegistryValue = UNICHECK_REG_VIEWERPATHKEY;

	// Get the viewer for results path from the registry.
	szViewerPath = GetProfileString(szRegistryKey, szRegistryValue, NULL);

	// Check if a value was retrieved for the viewer for results path.
	if ( szViewerPath.IsEmpty() )
	{
		// If the viewer for results path does not have a value, load empty string to the viewer for results path class member.
		SetViewerPath(NULL_TEXT);
	}
	else
	{
		// Load the retrieved value to a class member.
		SetViewerPath(szViewerPath);
	}


	/* Max Files to Add at One Time Setting */

	// Load the registry key value to get the max. files value from the registry.
	szRegistryValue = UNICHECK_REG_MAXFILEKEY;
	nRegistryDefaultValue = UNICHECK_REG_MAXFILEDEFAULT;

	// Get the max. files value from the registry.
	nMaxFiles = GetProfileInt(szRegistryKey, szRegistryValue, NULL);

	// Check if a value was retrieved for the program directory.
	if ( (nMaxFiles < UNICHECK_REG_MAXFILEFLOOR) && (nMaxFiles > UNICHECK_REG_MAXFILECEILING) )
	{
		/* If the max. files value does not have a valid value, load the default number to the class member. */

		SetMaxFiles(UNICHECK_REG_MAXFILEDEFAULT);
	}
	else
	{
		/* If the max. files value has a value, load the retrieved value to a class member. */

		// Load the retrieved value to a class member.
		SetMaxFiles(nMaxFiles);
	}
}

/* GetInstalledScan - Retrieve all Attributes of an Installed Scan */
BOOL CUniCheckApp::GetInstalledScan(UINT uScanType, DSSCAN* dsScan)
{
	/* Local Variable Declarations and Initializations */

	int nIndex = 0;										// The Enumeration Index Used to Walk the Installed Scans Subkeys
	int nResult = ERROR_SUCCESS;						// The Result of the Registry APIs

	BOOL bReturnValue = FALSE;							// Return Value Indicating the Status of the Method Run

	TCHAR szScanType[MAX_PATH] = {NULL_CHAR};			// String Representation of the Scan Type
	TCHAR szParamID[MAX_PATH] = {NULL_CHAR};			// String Representation of the Parameter ID
	TCHAR szScansKey[MAX_PATH] = {NULL_CHAR};			// The String Specifying the Short Path to the Scans Registry Key
	TCHAR szParamsKey[MAX_PATH] = {NULL_CHAR};			// The String Specifying the Short Path to the Parameters Registry Key
	TCHAR szParameterKey[MAX_PATH] = {NULL_CHAR};		// The String Specifying the Short Path to the Individual Parameter Registry Keys
	TCHAR szScansKeyParameter[MAX_PATH] = {NULL_CHAR};	// The String Specifying the Full Path to the Scan's Parameters Key

	POSITION parameterPosition = NULL;					// Position of the Current Parameter in the Link List

	HKEY hRegistryKey = NULL;							// Handle to the Scan's Parameters Registry Key

	DSSCAN dsLocalScan;									// Local Copy of the Scan Structure Used to Actually Hold the Retrieved Scan Values
	DSPARAMETER dsParam;								// The Attributes of the Current Parameter


	// Reset the scan data structures to ensure clean data is returned.
	if (dsScan != NULL)
	{
		ClearScanStructure(dsScan);
	}
	ClearScanStructure(&dsLocalScan);


	// Convert the scan type to a string.
	_itot(uScanType, szScanType, 10);

	// Build the path to the scan type registy key.
	wsprintf(szScansKey, "%s%s%s", UNICHECK_REG_INSTSCANSKEY, SLASH, szScanType);


	/* Fetch each attribute of the scan structure to the registry. */

	// Fetch the registry key for the scan type.
	dsLocalScan.uScanType = GetProfileInt(szScansKey, UNICHECK_REG_SCANTYPEKEY, NULL);

	// Verify that the scan type registry value was found.
	if ( dsLocalScan.uScanType != NULL )
	{
		/* If the base key was found, continue fetching each attribute of the scan. */

		// If the base key was fetched successfully, set the return value assuming success on remaining processing.
		bReturnValue = TRUE;

		// Fetch the scan title key.
		dsLocalScan.szScanTitle = GetProfileString(szScansKey, UNICHECK_REG_SCANTITLEKEY, NULL);
		if ( dsLocalScan.szScanTitle.IsEmpty() )
		{
			// If the fetch failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Fetch the scan engine key.
		dsLocalScan.szScanEngine = GetProfileString(szScansKey, UNICHECK_REG_SCANENGKEY, NULL);
		if ( dsLocalScan.szScanEngine.IsEmpty() )
		{
			// If the fetch failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Fetch the scan results file key.
		dsLocalScan.szResultsFile = GetProfileString(szScansKey, UNICHECK_REG_SCANRESFILEKEY, NULL);
		if ( dsLocalScan.szResultsFile.IsEmpty() )
		{
			// If the fetch failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Fetch the scan results file path key.
		dsLocalScan.szResultsFilePath = GetProfileString(szScansKey, UNICHECK_REG_SCANRESPATHKEY, NULL);
		if ( dsLocalScan.szResultsFilePath.IsEmpty() )
		{
			// If the fetch failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Fetch the scan command prompt flag key.
		dsLocalScan.bUsesCommandPrompt = GetProfileInt(szScansKey, UNICHECK_REG_SCANCMDKEY, FALSE);

		// Fetch the scan temporary directory flag key.
		dsLocalScan.bUsesTempFolders = GetProfileInt(szScansKey, UNICHECK_REG_SCANTEMPDIRKEY, FALSE);

		// Fetch the scan fileset flag key.
		dsLocalScan.bUsesFileSet = GetProfileInt(szScansKey, UNICHECK_REG_SCANFILESETKEY, FALSE);

		// Fetch the scan selected file flag key.
		dsLocalScan.bUsesSelectedFile = GetProfileInt(szScansKey, UNICHECK_REG_SCANSELFILEKEY, FALSE);

		// If the base of the scan installed correctly, install the parameters of the scan.
		if ( bReturnValue )
		{
			/* Install the parameters. */

			// Build the path to the parameters registy key.
			wsprintf(szScansKeyParameter, "%s%s%s%s%s", UNICHECK_REG_SOFTWAREKEY, SLASH, szScansKey, SLASH, UNICHECK_REG_PARAMETERSKEY);

			// Get a handle to the parameters registry key.
			nResult = RegOpenKeyEx(HKEY_CURRENT_USER, szScansKeyParameter, 0, KEY_ENUMERATE_SUB_KEYS, &hRegistryKey);

			// If the Installed Scans key is opened, load all the installed scan to the interface.
			if ( nResult == ERROR_SUCCESS )
			{
				/* If the Parameters key exists, loop through each installed parameter and load it to the scan structure. */

				if (hRegistryKey != NULL )
				{
					// Fetch each parameter.
					while ( RegEnumKey(hRegistryKey, nIndex, szParameterKey, 255) != ERROR_NO_MORE_ITEMS )
					{
						// Clear the parameter structure to ensure clean data.
						ClearParameterStructure(&dsParam);

						// Build the path to the parameter registy key.
						wsprintf(szParamsKey, "%s%s%s%s%s", szScansKey, SLASH, UNICHECK_REG_PARAMETERSKEY, SLASH, szParameterKey);

						// Fetch the registry key for the scan type.
						dsParam.uParameterID = GetProfileInt(szParamsKey, UNICHECK_REG_PARAMIDKEY, NULL);

						// Verify that the scan type registry value was found.
						if ( dsParam.uParameterID != NULL )
						{
							/* If the base key was created, continue writing each attribute of the scan. */

							// Fetch the parameter value key.
							dsParam.szParameter = GetProfileString(szParamsKey, UNICHECK_REG_PARAMVALUEKEY, NULL);
							if ( dsLocalScan.szResultsFilePath.IsEmpty() )
							{
								// If the fetch failed, set the return value to signal failure.
								bReturnValue = FALSE;
							}

							// Fetch the parameter uses path flag key.
							dsParam.bUsesParameterPath = GetProfileInt(szParamsKey, UNICHECK_REG_PARAMUPATHKEY, FALSE);

							// Fetch the parameter path key.
							dsParam.szParameterPath = GetProfileString(szParamsKey, UNICHECK_REG_PARAMPATHKEY, NULL);
							if ( dsLocalScan.szResultsFilePath.IsEmpty() )
							{
								// If the fetch failed, set the return value to signal failure.
								bReturnValue = FALSE;
							}

							// Fetch the parameter append end space flag key.
							dsParam.bAppendEndSpace = GetProfileInt(szParamsKey, UNICHECK_REG_PARAMENDSPKEY, FALSE);
						}
						else
						{
							// If the write failed, set the return value to signal failure.
							bReturnValue = FALSE;
						}

						// Increment the index and reset the parameter and parameters key strings to ensure clean data.
						nIndex++;
						ZeroMemory(szParamsKey, sizeof(szParamsKey));
						ZeroMemory(szParameterKey, sizeof(szParameterKey));

						// If no error occurred, load the parameter to the scan structure.
						if ( bReturnValue )
						{
							dsLocalScan.ParameterList.AddTail(dsParam);
						}
					}
				}
			}
		}
	}

	// If all of the attributes of the scan were retrieved correctly, assign the retrieved values to the return structure.
	if ( bReturnValue )
	{
		// Only attempt to return retrieved values if a return structure is provided.
		if ( dsScan != NULL)
		{
			// Copy each attribute of the retrieved scan to the return structure.
			CopyScanStructure(dsScan, &dsLocalScan);
		}
	}

	// Close the handle to the installed scans registry key if it is open.
	if ( hRegistryKey != NULL )
	{
		RegCloseKey(hRegistryKey);
	}

	// Return the result of attempting to write each attribute to the registry.
	return bReturnValue;
}

/* InstallScan - Write all Attributes of a Scan to Persistent Storage */
BOOL CUniCheckApp::InstallScan(DSSCAN* dsScan)
{
	/* Local Variable Declarations and Initializations */

	int nParameterIndex = 0;					// Order of the Parameters (Used to Name the Parameter Keys)
	BOOL bReturnValue = FALSE;					// Return Value Indicating the Status of the Method Run

	TCHAR szScanType[MAX_PATH] = {NULL_CHAR};	// String Representation of the Scan Type
	TCHAR szParamIndex[MAX_PATH] = {NULL_CHAR};	// String Representation of the Parameter Index
	TCHAR szScansKey[MAX_PATH] = {NULL_CHAR};	// The String Specifying the Short Path to the Scans Registry Key
	TCHAR szParamsKey[MAX_PATH] = {NULL_CHAR};	// The String Specifying the Short Path to the Parameters Registry Key

	POSITION parameterPosition = NULL;			// Position of the Current Parameter in the Link List
	
	DSPARAMETER dsParam;						// The Attributes of the Current Parameter


	// Convert the scan type to a string.
	_itot(dsScan->uScanType, szScanType, 10);

	// Build the path to the scan type registy key.
	wsprintf(szScansKey, "%s%s%s", UNICHECK_REG_INSTSCANSKEY, SLASH, szScanType);


	/* If the scan is already installed, attempt to delete the scan type's key to remove any previous scan settings for the scan.
	   (i.e. the user is attempting to update an installed scan's settings.) */

	// Attempt to fetch the scan's settings to see if it is already installed.
	if ( GetInstalledScan(dsScan->uScanType, NULL) )
	{
		// If the scan is installed, uninstall it to clear any previous settings. 
		if ( !UninstallScan(dsScan->uScanType) )
		{
			// If the scan could not be uninstalled, return and signal that the scan could not be updated.
			return FALSE;
		}
	}


	/* Write each attribute of the scan structure to the registry. */

	// Write the registry key for the scan type.
	if ( WriteProfileInt(szScansKey, UNICHECK_REG_SCANTYPEKEY, dsScan->uScanType) )
	{
		/* If the base key was created, continue writing each attribute of the scan. */

		// If the base key was written successfully, set the return value assuming success on remaining processing.
		bReturnValue = TRUE;

		// Write the scan title key.
		if ( !WriteProfileString(szScansKey, UNICHECK_REG_SCANTITLEKEY, dsScan->szScanTitle) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Write the scan engine key.
		if ( !WriteProfileString(szScansKey, UNICHECK_REG_SCANENGKEY, dsScan->szScanEngine) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Write the scan results file key.
		if ( !WriteProfileString(szScansKey, UNICHECK_REG_SCANRESFILEKEY, dsScan->szResultsFile) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Write the scan results file path key.
		if ( !WriteProfileString(szScansKey, UNICHECK_REG_SCANRESPATHKEY, dsScan->szResultsFilePath) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Write the scan command prompt flag key.
		if ( !WriteProfileInt(szScansKey, UNICHECK_REG_SCANCMDKEY, dsScan->bUsesCommandPrompt) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Write the scan temporary directory flag key.
		if ( !WriteProfileInt(szScansKey, UNICHECK_REG_SCANTEMPDIRKEY, dsScan->bUsesTempFolders) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Write the scan fileset flag key.
		if ( !WriteProfileInt(szScansKey, UNICHECK_REG_SCANFILESETKEY, dsScan->bUsesFileSet) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Write the scan selected file flag key.
		if ( !WriteProfileInt(szScansKey, UNICHECK_REG_SCANSELFILEKEY, dsScan->bUsesSelectedFile) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// If the base of the scan installed correctly, install the parameters of the scan.
		if ( bReturnValue )
		{
			/* Install the parameters. */

			// Get a reference to the first parameter in the link list.
			parameterPosition = dsScan->ParameterList.GetHeadPosition();

			// If the a first position is found, attempt to install the parameters to the registry.
			while ( parameterPosition != NULL )
			{
				// Clear the parameter structure and parameter key string to ensure clean data.
				ClearParameterStructure(&dsParam);
				memset(szParamsKey, NULL_CHAR, sizeof(szParamsKey));

				// Get the values from the link list.
				dsParam = dsScan->ParameterList.GetNext(parameterPosition);

				/* Build the registry key string for the current parameter. */

				// Convert the parameter ID to a string.
				_itot(nParameterIndex, szParamIndex, 10);

				// Build the path to the parameter registy key.
				wsprintf(szParamsKey, "%s%s%s%s%s", szScansKey, SLASH, UNICHECK_REG_PARAMETERSKEY, SLASH, szParamIndex);

				// Write the registry key for the parameter ID.
				if ( WriteProfileInt(szParamsKey, UNICHECK_REG_PARAMIDKEY, dsParam.uParameterID) )
				{
					/* If the base key was created, continue writing each attribute of the scan. */

					// Write the parameter value key.
					if ( !WriteProfileString(szParamsKey, UNICHECK_REG_PARAMVALUEKEY, dsParam.szParameter) )
					{
						// If the write failed, set the return value to signal failure.
						bReturnValue = FALSE;
					}

					// Write the parameter uses path flag key.
					if ( !WriteProfileInt(szParamsKey, UNICHECK_REG_PARAMUPATHKEY, dsParam.bUsesParameterPath) )
					{
						// If the write failed, set the return value to signal failure.
						bReturnValue = FALSE;
					}

					// Write the parameter path key.
					if ( !WriteProfileString(szParamsKey, UNICHECK_REG_PARAMPATHKEY, dsParam.szParameterPath) )
					{
						// If the write failed, set the return value to signal failure.
						bReturnValue = FALSE;
					}

					// Write the parameter append end space flag key.
					if ( !WriteProfileInt(szParamsKey, UNICHECK_REG_PARAMENDSPKEY, dsParam.bAppendEndSpace) )
					{
						// If the write failed, set the return value to signal failure.
						bReturnValue = FALSE;
					}
				}
				else
				{
					// If the write failed, set the return value to signal failure.
					bReturnValue = FALSE;
				}

				// Increment the parameter index to the next value.
				nParameterIndex++;

				// If an error occurred, stop installing parameters.
				if ( !bReturnValue )
				{
					parameterPosition = NULL;
				}
			}
		}
	}


	// If an error occurred attempting to write any one of the settings, attempt to delete the entire tree (i.e. uninstall the scan).
	// If it fails, there is no need to catch it.  The app. has already made a reasonable effort to clean up after itself.
	if ( !bReturnValue )
	{
		UninstallScan(dsScan->uScanType);
	}


	// Return the result of attempting to write each attribute to the registry.
	return bReturnValue;
}

/* UninstallScan - Remove all Attributes of a Scan from Persistent Storage */
BOOL CUniCheckApp::UninstallScan(UINT uScanType)
{
	/* Local Variable Declarations and Initializations */

	BOOL bReturnValue = FALSE;						// Return Value Indicating the Status of the Method Run

	TCHAR szScanType[MAX_PATH] = {NULL_CHAR};		// String Representation of the Scan Type
	TCHAR szScansKeyFull[MAX_PATH] = {NULL_CHAR};	// The String Specifying the Full Path to the Scans Registry Key

	HKEY hRegistryKey = NULL;						// Handle to the Installed Scan Registry Key


	// Convert the scan type to a string.
	_itot(uScanType, szScanType, 10);


	// Build the full path to the scan type registry key.
	wsprintf(szScansKeyFull, "%s%s%s", UNICHECK_REG_SOFTWAREKEY, SLASH, UNICHECK_REG_INSTSCANSKEY);

	// Get a handle to the scans registry key.
	if ( RegOpenKeyEx(HKEY_CURRENT_USER, szScansKeyFull, 0, KEY_WRITE, &hRegistryKey) == ERROR_SUCCESS )
	{
		// If the registry key can be opened, attempt to delete the key to completely remove the scan's settings.
		if ( hRegistryKey != NULL )
		{
			if ( DelRegTree(hRegistryKey, szScanType) != ERROR_SUCCESS )
			{
				// If the scan's previous settings cannot be cleared, exit the function signalling failure.
				bReturnValue = FALSE;
			}
			else
			{
				// If the scan's settings were deleted, return and signal that the scan was successfully uninstalled.
				bReturnValue = TRUE;
			}
		}
	}

	// Close the handle to the installed scans registry key if it is open.
	if ( hRegistryKey != NULL )
	{
		RegCloseKey(hRegistryKey);
	}

	// Return the result of the uninstall.
	return bReturnValue;
}

/* LoadAllValidFileTypes - Retrieve All Installed File Types in Either a List Box or File Dialog Filter String */
BOOL CUniCheckApp::LoadAllValidFileTypes(CListBox* pTypesListBox, CString* szFilter)
{
	/* Local Variable Declarations and Initialization */

	BOOL bReturnValue = FALSE;							// Return Value Indicating the Status of the Method Run

	int nIndex = 0;										// The Enumeration Index Used to Walk the Installed Scans Subkeys
	int nTypeIndex = UNICHECK_RETURN_NOINDEX;			// The Index of an Item in the List Box
	int nResult = ERROR_SUCCESS;						// The Result of the Registry APIs

	TCHAR szFileTypeKey[MAX_PATH] = {NULL_CHAR};		// The Parent Key Name for Each Installed Type's Settings
	TCHAR szTypesKeyFull[MAX_PATH] = {NULL_CHAR};		// The String Specifying the Full Path to the Valid File Types Registry Key

	CString szFileTypeDescrition;						// The Retreived File Type Description

	HKEY hRegistryKey = NULL;							// Handle to the Installed Scan Registry Key


	// Only attempt to load file types if the list box control or filter string exists for return.
	if ( (pTypesListBox != NULL) || (szFilter != NULL) )
	{
		/* Attempt to get the Installed Scans registry key. */

		// Build the full path to the valid file types registry key.
		wsprintf(szTypesKeyFull, "%s%s%s", UNICHECK_REG_SOFTWAREKEY, SLASH, UNICHECK_REG_FILETYPESKEY);


		// Get a handle to the installed scans registry key.
		nResult = RegOpenKeyEx(HKEY_CURRENT_USER, szTypesKeyFull, 0, KEY_ENUMERATE_SUB_KEYS, &hRegistryKey);

		// If the Installed Scans key is opened, load all the installed scan to the interface.
		if ( nResult == ERROR_SUCCESS )
		{
			/* If the Installed Scans key exists, loop through each installed scan and load it to the interface. */

			// Clear the list box and filter string of any previous data to ensure clean return data.
			if ( pTypesListBox != NULL )
			{
				pTypesListBox->ResetContent();
			}
			if ( szFilter != NULL )
			{
				szFilter->Empty();
			}

			// Append the "All Files" filter to the beginning of the filter string.
			if ( szFilter != NULL )
			{
				*szFilter = *szFilter + UNICHECK_FILTERCOMPS_AFILES;
			}


			/* Cycle through the installed valid file types. */

			if (hRegistryKey != NULL )
			{
				// Set the return value to signal success assuming all types are retrieved from the registry.
				bReturnValue = TRUE;

				// Fetch each installed scan type.
				while ( RegEnumKey(hRegistryKey, nIndex, szFileTypeKey, 255) != ERROR_NO_MORE_ITEMS )
				{
					// Fetch each installed file type.
					if ( GetValidFileType(szFileTypeKey, &szFileTypeDescrition, NULL) )
					{
						// If the file type is retrieved, load the scan to the list box.
						if ( pTypesListBox != NULL )
						{
							nTypeIndex = pTypesListBox->AddString(szFileTypeKey);
						}

						// If the file type is retrieved, build the filter string.
						if ( szFilter != NULL )
						{
							*szFilter = *szFilter + szFileTypeDescrition + UNICHECK_FILTERCOMPS_COMP1 + szFileTypeKey + UNICHECK_FILTERCOMPS_COMP2 + szFileTypeKey + UNICHECK_FILTERCOMPS_COMP3;
						}
					}
					else
					{
						// If a type is not found in the registry, return signalling failure.
						bReturnValue = FALSE;
					}

					// Increment the index and reset the key name string.
					nIndex++;
					ZeroMemory(szFileTypeKey, sizeof(szFileTypeKey));
				}
			}

			// Append the end marker (|) to the end of the filter string.
			if ( szFilter != NULL )
			{
				*szFilter = *szFilter + UNICHECK_FILTERCOMPS_COMP3;
			}

		}

		// Close the handle to the installed scans registry key if it is open.
		if ( hRegistryKey != NULL )
		{
			RegCloseKey(hRegistryKey);
		}

		// Return and signal the result of the run.
		return bReturnValue;
	}
	else
	{
		// If no return structures are passed, return and signal failure.
		return bReturnValue;
	}
}

/* GetValidFileType - Retrieve an Installed File Type's Description */
BOOL CUniCheckApp::GetValidFileType(CString szType, CString* szDescription, CString* szGrouping)
{
	/* Local Variable Declarations and Initializations */

	BOOL bReturnValue = TRUE;					// Return Value Indicating the Status of the Method Run

	TCHAR szTypesKey[MAX_PATH] = {NULL_CHAR};	// The String Specifying the Short Path to the Valid File Types Registry Key

	CString szFetchedDescription;				// The Description Value Retrieved from the Registry
	CString szFetchedGrouping;					// The Group Value Retrieved from the Registry


	// Build the path to the file type registy key.
	wsprintf(szTypesKey, "%s%s%s", UNICHECK_REG_FILETYPESKEY, SLASH, szType);


	/* Fetch each attribute of the file type from the registry. */

	// Fetch the file type extension key.
	szType = GetProfileString(szTypesKey, UNICHECK_REG_FILETYPEKEY, NULL);
	if ( szType.IsEmpty() )
	{
		// If the fetch failed, set the return value to signal failure.
		bReturnValue = FALSE;
	}

	// Fetch the file type description key.
	szFetchedDescription = GetProfileString(szTypesKey, UNICHECK_REG_FTYPEDESCKEY, NULL);
	if ( szFetchedDescription.IsEmpty() )
	{
		// If the fetch failed, set the return value to signal failure.
		bReturnValue = FALSE;
	}

	// Fetch the file type grouping key.
	szFetchedGrouping = GetProfileString(szTypesKey, UNICHECK_REG_FTYPEGROUPKEY, NULL);
	if ( szFetchedGrouping.IsEmpty() )
	{
		// If the fetch failed, set the return value to signal failure.
		bReturnValue = FALSE;
	}

	// If all of the attributes of the scan were retrieved correctly, assign the retrieved values to the return structure.
	if ( bReturnValue )
	{
		// Only attempt to return retrieved description if a return string is provided.
		if ( szDescription != NULL)
		{
			*szDescription = szFetchedDescription;
		}

		// Only attempt to return retrieved group if a return string is provided.
		if ( szGrouping != NULL)
		{
			*szGrouping = szFetchedGrouping;
		}
	}


	// Return the result of attempting to write each attribute to the registry.
	return bReturnValue;
}

/* InstallValidType - Write all Attributes of a Valid File Type to Persistent Storage */
BOOL CUniCheckApp::InstallValidType(CString szType, CString szDescription, CString szGrouping)
{
	/* Local Variable Declarations and Initializations */

	BOOL bReturnValue = FALSE;					// Return Value Indicating the Status of the Method Run

	TCHAR szTypesKey[MAX_PATH] = {NULL_CHAR};	// The String Specifying the Short Path to the Valid File Types Registry Key


	// Build the path to the file type registy key.
	wsprintf(szTypesKey, "%s%s%s", UNICHECK_REG_FILETYPESKEY, SLASH, szType);


	/* If the scan is already installed, attempt to delete the scan type's key to remove any previous scan settings for the scan.
	   (i.e. the user is attempting to update an installed scan's settings.) */

	// Attempt to fetch the file type's settings to see if it is already installed.
	if ( GetValidFileType(szType, NULL, NULL) )
	{
		// If the file type is installed, uninstall it to clear any previous settings. 
		if ( !UninstallValidType(szType) )
		{
			// If the file type could not be uninstalled, return and signal that the file type could not be updated.
			return FALSE;
		}
	}


	/* Write each attribute of the file type to the registry. */

	// Write the registry key for the file type extension.
	if ( WriteProfileString(szTypesKey, UNICHECK_REG_FILETYPEKEY, szType) )
	{
		/* If the base key was created, continue writing each attribute of the scan. */

		// If the base key was written successfully, set the return value assuming success on remaining processing.
		bReturnValue = TRUE;

		// Write the file type description key.
		if ( !WriteProfileString(szTypesKey, UNICHECK_REG_FTYPEDESCKEY, szDescription) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}

		// Write the file type grouping key.
		if ( !WriteProfileString(szTypesKey, UNICHECK_REG_FTYPEGROUPKEY, szGrouping) )
		{
			// If the write failed, set the return value to signal failure.
			bReturnValue = FALSE;
		}
	}


	// If an error occurred attempting to write any one of the settings, attempt to delete the entire tree (i.e. uninstall the type).
	// If it fails, there is no need to catch it.  The app. has already made a reasonable effort to clean up after itself.
	if ( !bReturnValue )
	{
		UninstallValidType(szType);
	}


	// Return the result of attempting to write each attribute to the registry.
	return bReturnValue;
}

/* UninstallValidType - Retrieve all Attributes of a Valid File Type from Persistent Storage */
BOOL CUniCheckApp::UninstallValidType(CString szType)
{
	/* Local Variable Declarations and Initializations */

	BOOL bReturnValue = FALSE;							// Return Value Indicating the Status of the Method Run

	TCHAR szFileTypesKeyFull[MAX_PATH] = {NULL_CHAR};	// The String Specifying the Full Path to the File Types Registry Key

	HKEY hRegistryKey = NULL;							// Handle to the Installed Scan Registry Key


	// Build the full path to the scan type registry key.
	wsprintf(szFileTypesKeyFull, "%s%s%s", UNICHECK_REG_SOFTWAREKEY, SLASH, UNICHECK_REG_FILETYPESKEY);

	// Get a handle to the scans registry key.
	if ( RegOpenKeyEx(HKEY_CURRENT_USER, szFileTypesKeyFull, 0, KEY_WRITE, &hRegistryKey) == ERROR_SUCCESS )
	{
		// If the registry key can be opened, attempt to delete the key to completely remove the scan's settings.
		if ( hRegistryKey != NULL )
		{
			if ( DelRegTree(hRegistryKey, szType) != ERROR_SUCCESS )
			{
				// If the scan's previous settings cannot be cleared, exit the function signalling failure.
				bReturnValue = FALSE;
			}
			else
			{
				// If the scan's settings were deleted, return and signal that the scan was successfully uninstalled.
				bReturnValue = TRUE;
			}
		}
	}

	// Close the handle to the installed scans registry key if it is open.
	if ( hRegistryKey != NULL )
	{
		RegCloseKey(hRegistryKey);
	}

	// Return the result of the uninstall.
	return bReturnValue;
}

/* IsFileTypeValid - Determine If the Specified File Type is in the List of Valid File Types */
BOOL CUniCheckApp::IsFileTypeValid(CString szType)
{
	/* Local Variable Declarations and Initializations */

	int nNumLoadedTypes = 0;				// The Number of Valid File Types

	CString szFileType;						// Current File Type Value

	CListBox* pFilterList = new CListBox;	// List Box Used as an Easy-Access Array to Validate File Types


	// Create the list box.
	pFilterList->Create(WS_CHILD|LBS_STANDARD, CRect(0,0,0,0), this->GetMainWnd(), 0);

	// Get the list of valid file types.
	if ( LoadAllValidFileTypes(pFilterList, NULL) )
	{
		/* Cycle through each element in the list and determine if there is a match. */

		// Get the number of valid types;
		nNumLoadedTypes = pFilterList->GetCount();

		// Loop through each type in the list.
		for ( int i = 0; i < nNumLoadedTypes; i++ )
		{
			// Get the current type from the list;
			pFilterList->GetText(i, szFileType);

			// If the file type passed and the current file type match, return and signal the type is valid.
			if ( szFileType == szType )
			{
				// Clean up allocated memory.
				if ( pFilterList != NULL )
				{
					delete pFilterList;
				}

				return TRUE;
			}
		}
	}

	// Clean up allocated memory.
	if ( pFilterList != NULL )
	{
		delete pFilterList;
	}

	// If no match was made, return and signal the type is not valid.
	return FALSE;
}

/* ClearScanStructure - Reset All Attributes of a Scan Structure to NULL Values */
void CUniCheckApp::ClearScanStructure(DSSCAN* dsScan)
{
	// Clear all elements of the scan structure.
	dsScan->uScanType = NULL;
	dsScan->szScanTitle = NULL_TEXT;
	dsScan->szScanEngine = NULL_TEXT;
	dsScan->szResultsFile = NULL_TEXT;
	dsScan->szResultsFilePath = NULL_TEXT;
	dsScan->bUsesCommandPrompt = NULL;
	dsScan->bUsesTempFolders = NULL;
	dsScan->bUsesFileSet = NULL;
	dsScan->bUsesSelectedFile = NULL;
	dsScan->ParameterList.RemoveAll();
}

/* ClearParameterStructure - Reset All Attributes of a Parameter Structure to NULL Values */
void CUniCheckApp::ClearParameterStructure(DSPARAMETER* dsParam)
{
	// Clear all elements of the parameter structure.
	dsParam->uParameterID = NULL;
	dsParam->szParameter = NULL_TEXT;
	dsParam->szParameterPath = NULL_TEXT;
	dsParam->bUsesParameterPath = NULL;
	dsParam->bAppendEndSpace = NULL;
}

/* CopyScanStructure - Copy All Attributes of a Scan Structure to a Different Scan Structure */
void CUniCheckApp::CopyScanStructure(DSSCAN* dsTargetStructure, DSSCAN* dsSourceStructure)
{
	// Reset the content of the target structure to a fresh state.
	ClearScanStructure(dsTargetStructure);

	// Copy all elements of the scan structure.
	dsTargetStructure->uScanType = dsSourceStructure->uScanType;
	dsTargetStructure->szScanTitle = dsSourceStructure->szScanTitle;
	dsTargetStructure->szScanEngine = dsSourceStructure->szScanEngine;
	dsTargetStructure->szResultsFile = dsSourceStructure->szResultsFile;
	dsTargetStructure->szResultsFilePath = dsSourceStructure->szResultsFilePath;
	dsTargetStructure->bUsesCommandPrompt = dsSourceStructure->bUsesCommandPrompt;
	dsTargetStructure->bUsesTempFolders = dsSourceStructure->bUsesTempFolders;
	dsTargetStructure->bUsesFileSet = dsSourceStructure->bUsesFileSet;
	dsTargetStructure->bUsesSelectedFile = dsSourceStructure->bUsesSelectedFile;
	dsTargetStructure->ParameterList.AddTail(&dsSourceStructure->ParameterList);
}

/* BuildScanCommandLine - Returns the Command Line Components to Run the Specified Scan */
BOOL CUniCheckApp::BuildScanCommandLine(UINT uScanType, CString* szExecutablePath, CString* szScanArguments, CString szCurrentFile)
{
	/* Local Variable Declarations and Initializations */

	TCHAR szLegacyString[MAX_PATH] = {NULL_CHAR};	// Classic Character Array Used to Retrieve Strings from Specific APIs

	CString szCommandEmulPath;			// CString Version of the Path to the Command Line Emulator
	CString szFilesetFile;				// The Name of the Psuedo-Visual Studio File to Use as Scan Input
	CString szFilesetFilePath;			// The Path of the Psuedo-Visual Studio File to Use as Scan Input

	POSITION parameterPosition = NULL;	// Position of the Current Parameter in the Link List

	DSSCAN dsScan;						// The Attributes of the Specified Installed Scan
	DSPARAMETER dsParam;				// The Attributes of a Parameter in the Selected Scan


	// Clear the scan and parameter structures to ensure clean data.
	ClearScanStructure(&dsScan);
	ClearParameterStructure(&dsParam);


	// Get the specified scan's attributes.
	if ( GetInstalledScan(uScanType, &dsScan) )
	{
		/* If the scan is installed, build the command line based on the scan's attributes. */

		// Get the fileset file name and path.
		szFilesetFile = GetFilesetFile();
		szFilesetFilePath = GetFilesetFilePath();

		// If either elements of the fileset file is empty, display an error to the user and exit, 
		// signalling to disable the scan.
		if ( szFilesetFile.IsEmpty() ||
			 szFilesetFilePath.IsEmpty() )
		{
			// Display error to the user..
			DisplayMessageToUser(NULL, UNICHECK_MESSAGE_APPXSET, UNICHECK_MESSAGE_VERFSSET, TRUE);

			// If either elements of the fileset file cannot be loaded, exit and signal that the scan could not be initialized 
			// and should be disabled.
			return FALSE;
		}
		else
		{
			// Convert the fileset path to the short directory name.
			szFilesetFilePath = GetShortFormatPath(szFilesetFilePath);
		}


		/* Initialization for scans requiring the command line emulator. */

		// Get the path to the command prompt emulator (cmd.exe).
		if (!GetEnvironmentVariable(UNICHECK_COMEMUL_SYSVARNAME, szLegacyString, MAX_PATH))
		{
			// If the path to the command prompt emulator cannot be loaded, display an error to the user.
			DisplayMessageToUser(NULL, UNICHECK_MESSAGE_XCOMPROMPT, NULL_TEXT, TRUE);

			// If the path to the command prompt emulator cannot be loaded, exit and signal that the scan could not be initialized 
			// and should be disabled.
			return FALSE;
		}

		// Convert the path to the command prompt emulator to CString.
		szCommandEmulPath = szLegacyString;

		// Reset the shellCmd string.
		ZeroMemory(szLegacyString, sizeof(szLegacyString));


		// Append the appropriate character set processing flag to the command line.
		#ifdef _UNICODE
			szCommandEmulPath += UNICHECK_COMEMUL_UNIOUTPUT;	// Unicode Output
		#else	
			szCommandEmulPath += UNICHECK_COMEMUL_ANSIOUTPUT;	// ANSI Output
		#endif

		// Append the processing flag to terminate the cmd.exe once the scan is finished to the command line.
		szCommandEmulPath += UNICHECK_COMEMUL_TERMINATE;


		/* Build the executable path. */
		
		// If the scan is using the command prompt emulator, add the emulator to the executable path.
		if ( dsScan.bUsesCommandPrompt )
		{
			*szExecutablePath = *szExecutablePath + szCommandEmulPath + QUOTE;
		}

		// Convert the scan engine path to the short directory name.
		dsScan.szScanEngine = GetShortFormatPath(dsScan.szScanEngine);

		// Add the scan engine to the path.
		*szExecutablePath = *szExecutablePath
							+ QUOTE
							+ dsScan.szScanEngine
							+ QUOTE;


		/* Build the scan arguments string. */

		// Get a reference to the first parameter from the scan data.
		parameterPosition = dsScan.ParameterList.GetHeadPosition();

		// Cycle through each parameter to build the scan arguments string.
		while ( parameterPosition != NULL )
		{
			// Get the parameter attributes from the scan data.
			dsParam = dsScan.ParameterList.GetNext(parameterPosition);

			// If the parameter uses a path, add it and quotes first.
			if ( dsParam.bUsesParameterPath )
			{
				// Convert the parameter path to the short directory name.
				dsParam.szParameterPath = GetShortFormatPath(dsParam.szParameterPath);

				// Add the converted string to the arguments sting.
				*szScanArguments = *szScanArguments 
								   + QUOTE 
								   + dsParam.szParameterPath;
			}

			
			/* Evaluate the parameter value and perform necessary substitution values.*/

			if ( (dsScan.bUsesSelectedFile) && (dsParam.szParameter == UNICHECK_PARAMSUB_SELFILE) )
			{
				// If the scan specifies that it uses the selected file and the current parameter is the selected file identifier
				// (${sf}), add the selected file path as the parameter value.

				// Convert the selected file path to the short directory name.
				szCurrentFile = GetShortFormatPath(szCurrentFile);

				*szScanArguments = *szScanArguments
								   + QUOTE
								   + szCurrentFile
								   + QUOTE;
			}
			else
			{
				if ( (dsScan.bUsesFileSet) && (dsParam.szParameter == UNICHECK_PARAMSUB_FSFILE) )
				{
					// If the scan specifies that it uses the fileset file and the current parameter is the fileset file identifier
					// (${ff}), add the fileset file path as the parameter value.
					*szScanArguments = *szScanArguments
									   + QUOTE
									   + szFilesetFilePath
									   + szFilesetFile
									   + QUOTE;
				}
				else
				{
					if ( (dsParam.szParameter == UNICHECK_PARAMSUB_RESFILE) )
					{
						// If the current parameter is the results file identifier (${rf}), 
						// add the results file path as the parameter value.

						// Convert the results file path to the short directory name.
						dsScan.szResultsFilePath = GetShortFormatPath(dsScan.szResultsFilePath);

						*szScanArguments = *szScanArguments
										   + QUOTE
										   + dsScan.szResultsFilePath
										   + dsScan.szResultsFile
										   + QUOTE;
					}
					else
					{
						// Add the parameter value to the string.
						*szScanArguments = *szScanArguments + dsParam.szParameter;
					}
				}
			}
			

			// If the parameter uses a path, close the quotes.
			if ( dsParam.bUsesParameterPath )
			{
				*szScanArguments = *szScanArguments + QUOTE;
			}

			// If the parameter specifies and ending space, add one.
			if ( dsParam.bAppendEndSpace )
			{
				*szScanArguments = *szScanArguments + SPACE;
			}
		}

		// If the scan is using the command prompt emulator, close the quotes around the full command line.
		if ( dsScan.bUsesCommandPrompt )
		{
			*szScanArguments = *szScanArguments + QUOTE;
		}

		return TRUE;
	}
	else
	{
		// If the scan is not installed exit the method and signal that the scan should be disabled.
		return FALSE;
	}
}

/* IsAppInitialized - Indicates Whether the Application Has All Required Settings Loaded */
BOOL CUniCheckApp::IsAppInitialized()
{
	/* Local Variable Declarations and Initializations */

	CString szProgramPath;			// Directory Path of the Program Directory
	CString szHeaderPath;			// Directory Path of the Header Files Temporary Directory
	CString szSourcePath;			// Directory Path of the Source Files Temporary Directory
	CString szFilesetFile;			// Name of the Pseudo-Visual Studio File to Use as Scan Input
	CString szFilesetFilePath;		// Path of the Pseudo-Visual Studio File to Use as Scan Input
	CString szViewerPath;			// Path of the Executable to View Scan Results


	// Get all paths from the application instance.
	szProgramPath = GetProgramPath();
	szHeaderPath = GetHeaderPath();
	szSourcePath = GetSourcePath();
	szFilesetFile = GetFilesetFile();
	szFilesetFilePath = GetFilesetFilePath();
	szViewerPath = GetViewerPath();

	// Verify that all program paths are set.
	if ( (!szProgramPath.IsEmpty()) &&
		 (!szHeaderPath.IsEmpty()) &&
		 (!szSourcePath.IsEmpty()) && 
		 (!szFilesetFile.IsEmpty()) && 
		 (!szFilesetFilePath.IsEmpty()) && 
		 (!szViewerPath.IsEmpty()) )
	{
		// If all paths have a value, return that the application is initalized.
		return TRUE;
	}
	else
	{
		// If any of the paths does not have a value, return that the applicaiton is not initialized.
		return FALSE;
	}
}

/* ValidatePath - Verify the Path Exists and Is Formatted Correctly */
CString CUniCheckApp::ValidatePath(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	int nStringLength = 0;				// Number of Characters in a String (Used for Parsing)

	TCHAR cPathLastChar = NULL_CHAR;	// Last Character in a String (Used for Parsing)

	CString szReturnPath = NULL_TEXT;	// The Processed Path to Return to the Calling Object


	// Verify the path really exists.
	if ( VerifyThatFilePathExists(szPath) )
	{
		/* If the path does exist, validate the path is well formed. */

		// Get the number of characters in the program path string.
		nStringLength = szPath.GetLength();

		// Only append a windows file system directory identifier ('\') if the path has a value.
		if (nStringLength > 0)
		{
			// Get the last character of the program path.
			cPathLastChar = szPath.GetAt(nStringLength - 1);

			// If the program path does not terminate in a windows file system directory identifier ('\'), add one.
			if (cPathLastChar != SLASH_CHAR)
			{
				szPath = szPath + SLASH_CHAR;
			}
		}
		szReturnPath = szPath;
	}
	else
	{
		// If the path does not exist, set the path to NULL.
		szReturnPath = NULL_TEXT;
	}

	return szReturnPath;
}

/* GetShortFormatPath - Convert a Path to the 8 Character DOS Format */
CString CUniCheckApp::GetShortFormatPath(CString szPath)
{
	/* Local Variable Declarations and Initializations */

	TCHAR szLegacyString[MAX_PATH] = {NULL_CHAR};	// Classic Character Array Used to Retrieve Strings from Specific APIs


	/* Convert the path to DOS format. Ensures the command prompt emulator will recognize the path. */

	// If the path happens to be the preview placeholder for the currently selected file ({SelectedFile}), then don't attempt
	// to get the short format.
	if ( szPath != UNICHECK_TEXT_SELFILE )
	{
		// Get the DOS format of the parameter path.
		if(!GetShortPathName(szPath, szLegacyString, MAX_PATH))
		{
			// If the DOS format version of the path cannot be loaded, exit the method and display
			// an error message to the user.
			DisplayMessageToUser(NULL, UNICHECK_MESSAGE_APPXSET, UNICHECK_MESSAGE_VERPATHSET, TRUE);	
		}

		// Convert the path to the short directory name.
		szPath = szLegacyString;

		// Reset the legacy string.
		ZeroMemory(szLegacyString, sizeof(szLegacyString));
	}

	// Return the short format path.
	return szPath;
}

/* ProcessCommandLineFiles - Examine Files Passed as Parameters on the Command Line and Determine the Processing Mode for the Application */
UINT CUniCheckApp::ProcessCommandLineFiles(CCommandLineInfo *cmdInfo)
{
	/* Local Variable Declarations and Initializations */

	CString szFileExtension;	// The File Extension of the File Passed as a Command Line Parameter


	// If a file name is passed as part of the command line, determine the mode to launch the application.
	if ( !cmdInfo->m_strFileName.IsEmpty() )
	{
		/* Handle input files passed as parameters on the command line. */

		// Verify the file really exists.
		if ( !VerifyThatFilePathExists(cmdInfo->m_strFileName) )
		{
			// If the file does not exist, display an error to the user and signal failure.
			DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_FILESP + cmdInfo->m_strFileName + UNICHECK_MESSAGE_XNEXISTS), NULL_TEXT, TRUE);
			return UNICHECK_RETURN_FAILED;
		}


		// Load the extension of the file name passed in the command line to a temp variable.
		szFileExtension = DetermineFileExtension(cmdInfo->m_strFileName);

		// If the file is not a UniversalChecker list (.ucl) or Visual Studio project file (.dsp, .vcproj), then run the application in command line mode.
		if ( (szFileExtension != UNICHECK_FILETYPE_LIST) && (szFileExtension != UNICHECK_FILETYPE_VS6) && (szFileExtension != UNICHECK_FILETYPE_VS7) )
		{
			// If the file is a C source (.c) or C/CPP header (.h), then execute the application in command mode.
			if ( (szFileExtension == UNICHECK_FILETYPE_SOURCE) || (szFileExtension == UNICHECK_FILETYPE_HEADER) ) 
			{
				// Set a class member flag to specify that the application is now processing in command line mode.
				SetProcessingMode(UNICHECK_MODE_COMMANDLINE);
			}
			else
			{
				// If the file is NOT a C source (.c) or C/CPP header (.h), display an error to the user and signal failure.
				DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_FILESP + cmdInfo->m_strFileName + UNICHECK_MESSAGE_CNEVALED), UNICHECK_MESSAGE_VALTYPE, TRUE);
				return UNICHECK_RETURN_FAILED;
			}
		}
	}

	// Return the determined processing mode.
	return GetProcessingMode();
}

/* ExtractPathFromString - Parse Any String Return a File Path if It is Between the szPathIdentifier and szPathTerminator Values */
BOOL CUniCheckApp::ExtractPathFromString(CString szLineToSearch, CString szPathIdentifier, CString szPathTerminator, CString *szFoundPath)
{
	/* Local Variable Declarations and Initializations */

	int nLoc = 0;				// Current Character Position within the String to Search
	int nStartOfFilePath = 0;	// Character Position of the First Character of the File Path within the String to Search
	int nEndOfFilePath = 0;		// Character Position of the First Character of the File Path within the String to Search


	// Attempt to find the path identifier string.
	nLoc = szLineToSearch.Find(szPathIdentifier, nLoc);

	// Verify that the path identifier was found.
	if ( nLoc == -1 )
	{
		// If no path identifier was found, exit the method and signal that no file path was found.
		return FALSE;
	}
	else
	{
		/* If the path identifier was found, extract the file path. */

		// Set the position of the first character in the path string.
		nStartOfFilePath = nLoc + szPathIdentifier.GetLength();

		// Locate the end of the file path.
		nEndOfFilePath = szLineToSearch.Find(szPathTerminator, nStartOfFilePath);
		

		// Return the path value to the calling object.
		*szFoundPath = szLineToSearch.Mid(nStartOfFilePath, (nEndOfFilePath - nStartOfFilePath));


		// Exit and singal that a file path was found.
		return TRUE;
	}
}

/* ExtractFileNameFromPath - Parse a String Assuming It is a Valid File Path and Return the File Name */
CString CUniCheckApp::ExtractFileNameFromPath(CString szPath)
{
	/* Local Variable Declarations and Initializations */

	int nPathLen = 0;			// Length of the Full File Path String
	int nLoc = 0;				// Current Character Position within the File Path String
	int nStartOfFileName = 0;	// Character Position of the First Character of the File Name within the Path String

	CString szFileName;			// The File Name Extracted from the Path


	// Verify that the path has a value.
	if(!szPath.IsEmpty())
	{
		/* Extract the file name from the file path */

		// Get the length of the path string.
		nPathLen = szPath.GetLength();

		// Loop through the path string, finding each instance of the directory separator, exiting when no separator found.
		while ( (nLoc != -1) )
		{
			// Assign the start character of the path as the current location in the string.
			nStartOfFileName = nLoc;
			// Find the next instance of the directory separator.
			nLoc = szPath.Find(SLASH, nLoc);
			// If a directory separator is found, increment the location by one to the character immediately following the separator.
			if (nLoc != -1)
			{
				nLoc += 1;
			}
		}

		// Extract the file name from the path string.
		szFileName = szPath.Right(nPathLen - nStartOfFileName);

		// Convert the file name to upper case for ease of string comparison.  (Looks better than lower case in the user interface.)
		szFileName.MakeUpper();
	}

	// Return the file name.
	return szFileName;
}

/* DetermineFileExtension - Parse a String Assuming It is a Valid File Path or Name and Return the File Extension */
CString CUniCheckApp::DetermineFileExtension(CString szFileName)
{
	/* Local Variable Declarations and Initializations */

	int nFileNameLen = 0;					// Length of the Full File Path String

	TCHAR cPathLastChar = NULL_CHAR;		// Last Character in a String (Used for Parsing)

	CString szFileExtension = NULL_TEXT;	// The Extension of the File


	/* Extract the file extension from the file name/path */

	// Get the length of the path string.
	nFileNameLen = szFileName.GetLength();

	// Verify that the file name has a value.
	if (nFileNameLen > 0)
	{
		// Get the last n characters of the program path until the first extension identifier ('.') is found.
		do
		{
			// Build the file extension one character at a time.
			if (cPathLastChar != NULL)
			{
				szFileExtension.Insert(0, cPathLastChar);
			}

			// Decriment the character position by one to get the index of the preceding character in the file name.
			nFileNameLen -= 1;

			// Get the next character in the file name.
			if (nFileNameLen >= 0)
			{
				cPathLastChar = szFileName.GetAt(nFileNameLen);
			}

			// If the file name does not contain an extension identifier ('.'), then return a null string.
			// The directory identifier ('\') condition is included in case a full file path is passed instead
			// of a simple file name.  Only parse through the file name, ignore the rest of the path.
			if ( (cPathLastChar == SLASH_CHAR) || (nFileNameLen < 0) )
			{
				return NULL_TEXT;
			}

		} while (cPathLastChar != PERIOD_CHAR);
	}

	// Convert the file extension to lower case for ease of string comparison.
	szFileExtension.MakeLower();

	// Return the file extension.
	return szFileExtension;
}

/* ChangeFileExtension - Parse a String Assuming It is a Valid File Path or Name, Change the File Extension, and Return the New File Name/Path */
CString CUniCheckApp::ChangeFileExtension(CString szFileName, CString szNewExtension)
{
	/* Local Variable Declarations and Initializations */
	
	int nFileNameLen = 0;					// Length of the Full File Path String

	TCHAR cPathChar = NULL_CHAR;			// Selected Character in a String (Used for Parsing)

	CString szNewFileName = NULL_TEXT;		// The File Name or Path with the New File Extension


	// Copy the file name to the string to modify.
	szNewFileName = szFileName;


	/* Ensure the new file extension begins with an extension identifier. */

	// Only attempt to add a period if a string is passed, if blank is passed for the new extension, this function will
	// return the file name with no extension.
	if ( !szNewExtension.IsEmpty() )
	{
		// Extract the first character from the new extension.
		cPathChar = szNewFileName.GetAt(0);

		// If the new extension does not include an extension identifier, add one.
		if ( cPathChar != PERIOD_CHAR )
		{
			szNewExtension = PERIOD + szNewExtension;
		}

		// Reset cPathChar.
		cPathChar = NULL_CHAR;
	}


	/* Delete the file extension from the file name/path. */

	// Get the length of the path string.
	nFileNameLen = szNewFileName.GetLength();

	// Verify that the file name has a value.
	if (nFileNameLen > 0)
	{
		// Get the last n characters of the program path until the first extension identifier ('.') is found.
		do
		{
			// Decriment the character position by one to get the index of the preceding character in the file name.
			nFileNameLen -= 1;

			// Get the next character in the file name.
			if (nFileNameLen > 0)
			{
				cPathChar = szNewFileName.GetAt(nFileNameLen);
			}

			// Delete the existing file extension one character at a time.
			szNewFileName.Delete(nFileNameLen, 1);

			// If the file name does not contain an extension identifier ('.'), then append the extension directly to the passed name..
			// The directory identifier ('\') condition is included in case a full file path is passed instead
			// of a simple file name.  Only parse through the file name, ignore the rest of the path.
			if ( (cPathChar == SLASH_CHAR) || (nFileNameLen < 0) )
			{
				// Only append the extension if the path has a file name (i.e. a directory identifier is not the last character).
				if ( nFileNameLen != (szFileName.GetLength() - 1) )
				{
					return (szFileName + szNewExtension);
				}
			}

		} while (cPathChar != PERIOD_CHAR);
	}


	/* Append the new extension to the file name. */

	// Add the new extension to the processed file name/path.
	szNewFileName = szNewFileName + szNewExtension;

	// Return the new file name/path.
	return szNewFileName;
}

/* DeleteSpecificResultsFile - Delete the Results File Named szFileName */
BOOL CUniCheckApp::DeleteSpecificResultsFile(CString szFileName)
{
	// Verify that the file name path has a value.
	if (!szFileName.IsEmpty())
	{
		// Delete the results file.
		if (!DeleteFile(szFileName))
		{
			// Return that the file failed to delete.
			return FALSE;
		}
		else
		{
			// Return that the file was delete successfully.
			return TRUE;
		}
	}
	else
	{
		// Return that the file failed to delete if the file name path is not set.
		return FALSE;
	}
}

/* VerifyThatFilePathExists - Verify That the Input File or Path Exists */
BOOL CUniCheckApp::VerifyThatFilePathExists(CString szFilePath)
{
	/* Local Variable Declarations and Initialization */
	BOOL bResult = FALSE;							// The Return Value of the Method Signifying Whether the File or Path Exists

	int nStringLength = 0;							// Number of Characters in a String (Used for Parsing)

	TCHAR cPathLastChar = NULL_CHAR;				// Last Character in a String (Used for Parsing)

	HANDLE hCurrentFile = INVALID_HANDLE_VALUE;		// Handle to the File being Procesed

	WIN32_FIND_DATA dsFileInfo;						// Windows Shell File Data Structure


	// Memset data structures.
	ZeroMemory(&dsFileInfo, sizeof(WIN32_FIND_DATA));


	// Only verify if the path has a value.
	if(!szFilePath.IsEmpty())
	{
		/* If the input parameter ends with a directory identifier, assume it is a directory and append the all files wildcard. */

		// Get the number of characters in the program path string.
		nStringLength = szFilePath.GetLength();

		// Get the last character of the program path.
		if ( nStringLength > 0 )
		{
			cPathLastChar = szFilePath.GetAt(nStringLength - 1);
		}

		// If the file path does terminate in a windows file system directory identifier ('\'), append "all-files" wildcard.
		if (cPathLastChar == SLASH_CHAR)
		{
			// Append "all-files" wildcard ('*.*') to the input path.
			szFilePath = szFilePath + UNICHECK_PATH_ALLFILES;
		}


		// Grab the first file in the tempory source directory.
		hCurrentFile = FindFirstFile(szFilePath, &dsFileInfo);

		if (hCurrentFile != INVALID_HANDLE_VALUE)
		{
			// The file or path was found, signal success.
			bResult = TRUE;
		}
		else
		{
			// If the file or path was not found, signal failure.
			bResult = FALSE;
		}
	}

	// Close the file handle.
	if (hCurrentFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hCurrentFile);
	}

	// Signal failure if the entire directory is scanned and no files are found
	return bResult;
}

/* VerifyThatFilesExistInDirectory - Verify That Files Exists the Input Path */
BOOL CUniCheckApp::VerifyThatFilesExistInDirectory(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	BOOL bResult = FALSE;							// The Return Value of the Method Signifying Whether Any Files Exist in the Directory

	HANDLE hCurrentFile = INVALID_HANDLE_VALUE;		// Handle to the Current Source/Header File being Procesed

	WIN32_FIND_DATA dsFileInfo;						// Windows Shell File Data Structure


	// Memset data structures.
	ZeroMemory(&dsFileInfo, sizeof(WIN32_FIND_DATA));


	// Only verify if the path has a value.
	if(!szPath.IsEmpty())
	{
		// Append "all-files" wildcard ('*.*') to the input path.
		szPath = szPath + UNICHECK_PATH_ALLFILES;

		/* Validate that the C source or C/CPP header files to scan for Unicode have been copied to the appropriate
		   temporary directory. */

		// Grab the first file in the tempory source directory.
		hCurrentFile = FindFirstFile(szPath, &dsFileInfo);

		if (hCurrentFile != INVALID_HANDLE_VALUE)
		{
			// Cycle through each item in the directory.
			do
			{
				// Ignore the Windows file system "Current Directory" identifier.
				if(dsFileInfo.cFileName[1]==0 && *dsFileInfo.cFileName=='.')
					continue;
				// Ignore the Windows file system "Parent Directory" identifier.
				else if(dsFileInfo.cFileName[2]==0 && *dsFileInfo.cFileName=='.' && dsFileInfo.cFileName[1]=='.')
					continue;

				// Return success if a file is found in the temporary directory.
				bResult = TRUE;
			}
			while( (FindNextFile(hCurrentFile, &dsFileInfo)) && (bResult != TRUE) );
		}
	}

	// Close the file handle.
	if (hCurrentFile != INVALID_HANDLE_VALUE)
	{
		FindClose(hCurrentFile);
	}

	// Signal failure if the entire directory is scanned and no files are found
	return bResult;
}

/* DeleteAllFilesFromDirectory - Delete All Files from the Input Path */
void CUniCheckApp::DeleteAllFilesFromDirectory(CString szPath)
{
	/* Local Variable Declarations and Initialization */

	CString szFilePath;		// Full Path of the File Currently Being Processed
	CString szSearchString;	// Full Path of the Directory to Scan with Filter Wildcards

	HANDLE hCurrentFile = INVALID_HANDLE_VALUE;	// Handle to the File Currently Being Processed

	WIN32_FIND_DATA dsFileInfo;	// Windows Shell File Information Structure


	// Memset data structures.
	ZeroMemory(&dsFileInfo, sizeof(WIN32_FIND_DATA));


	// Only delete files if the path has a value.
	if(!szPath.IsEmpty())
	{
		// Append "all-files" wildcard ('*.*') to the input path.
		szSearchString = szPath + UNICHECK_PATH_ALLFILES;

		// Get a reference to the first file in the temporary source directory.
		hCurrentFile = FindFirstFile(szSearchString, &dsFileInfo);

		if (hCurrentFile != INVALID_HANDLE_VALUE)
		{
			// Loop through each file in the temporary source directory.
			do
			{
				// Ignore the Windows file system current directory indicator.
				if(dsFileInfo.cFileName[1]==0 && *dsFileInfo.cFileName=='.')
					continue;
				// Ignore the Windows file system parent directory indicator.
				else if(dsFileInfo.cFileName[2]==0 && *dsFileInfo.cFileName=='.' && dsFileInfo.cFileName[1]=='.')
					continue;

				// Build the full path to the current file.
				szFilePath = szPath + dsFileInfo.cFileName;

				// Delete the current file.
				if(!DeleteFile(szFilePath))
				{
					/* If the DeleteFile API fails in this instance, no need to handle the error. */

					// Assuming the API will only fail if the file doesn't exist, then there is
					// no need to handle the error.
				}
			}
			while(FindNextFile(hCurrentFile, &dsFileInfo));
		}

		// Close the file handle.
		if (hCurrentFile != INVALID_HANDLE_VALUE)
		{
			FindClose(hCurrentFile);
		}
	}
}

/* ExecuteScanProcess - Launch Process to Scan Document Data */
BOOL CUniCheckApp::ExecuteScanProcess(CString szScanExecutable, CString szScanArguments)
{
	/* Local Variable Declarations and Initialization */

	CString	szScanCommandLine;			// Full Command Line for Scan Executable

	STARTUPINFO dsProcessInfo;			// Windows Process Creation Information Structure
	PROCESS_INFORMATION dsProcessProc;	// Windows Process Information Structure


	// Memset data structures.
	ZeroMemory(&dsProcessInfo, sizeof(STARTUPINFO));
	ZeroMemory(&dsProcessProc, sizeof(PROCESS_INFORMATION));


	// Only scan if a valid exicutable was passed.
	if (!szScanExecutable.IsEmpty())
	{
		// Build the command line for scan process.
		szScanCommandLine = szScanExecutable 
						  + SPACE
						  + szScanArguments;

		// Load process structures.
		dsProcessInfo.cb = sizeof(dsProcessInfo);
		if (GetProcessingMode() == UNICHECK_MODE_INTERACTIVE)
		{
			// If the application is running in interactive mode, hide the process's window.
			dsProcessInfo.dwFlags = STARTF_USESHOWWINDOW;
			dsProcessInfo.wShowWindow = SW_HIDE;
		}

		// Create the process.
		if ( CreateProcess(NULL, szScanCommandLine.GetBuffer(sizeof(szScanCommandLine)), NULL, NULL, FALSE, 0, NULL, NULL, &dsProcessInfo, &dsProcessProc) )
		{
			/* The process started successfully. */

			// Wait for the process to finish before continuing.
			if (dsProcessProc.hProcess != NULL)
			{
				WaitForSingleObject(dsProcessProc.hProcess, INFINITE);
			}

			// Close the handles the process created.
			if (dsProcessProc.hProcess != INVALID_HANDLE_VALUE)
			{
				CloseHandle(dsProcessProc.hProcess);
			}
			if (dsProcessProc.hThread != INVALID_HANDLE_VALUE)
			{
				CloseHandle(dsProcessProc.hThread);
			}

			// Signal that the process completed successfully.
			return TRUE;
		}
		else
		{
			/* The process failed to start. */

			// Signal that the process failed to complete.
			return FALSE;
		}
	}
	else
	{
		// If no executable is passed, signal that the process failed to complete.
		return FALSE;
	}

}

/* ExecuteViewResultsProcess - Launch a Process to Display Scan Results */
UINT CUniCheckApp::ExecuteViewResultsProcess(CString szExecutablePath, CString szResultsFileName)
{
	/* Local Variable Declarations and Initialization */

	UINT uResult = UNICHECK_RETURN_FAILED;	// The Return Value of the Function Signifying The Status of Launching the View Process

	UINT uExecutableResult = 0;	// WinExec API Return Value

	CString szCommandLine;		// Full Command Line String to Display the Results File


	// Only execute the view process if all required information is available.
	if ( (!szExecutablePath.IsEmpty()),
		 (!szResultsFileName.IsEmpty()) )
	{
		// Build the command line to display the results file.
		szCommandLine = QUOTE
					  + szExecutablePath
					  + QUOTESPACEQUOTE
					  + szResultsFileName
					  + QUOTE;

		// If the results file exists, open it in Notepad.
		if ( VerifyThatFilePathExists(szResultsFileName) )
		{
			uExecutableResult = WinExec(szCommandLine, SW_SHOW);

			// If WinExec returned an error, display an error to the user about the problem with Notepad.
			if (uExecutableResult == SE_ERR_FNF || uExecutableResult == SE_ERR_PNF)
			{
				// Return that notepad.exe failed to launch.
				uResult = UNICHECK_RETURN_FAILED;
			}
			else
			{
				// Return that the results were displayed successfully.
				uResult = UNICHECK_RETURN_SUCCESS;
			}
		}
		else
		{
			// Return that the results file does not exist.
			uResult = UNICHECK_RETURN_NOFILE;
		}
	}

	// Return the result of the method run.
	return uResult;
}

/* LoadScansForDisplay - Load All Installed Scans to a List Box User Interface Control */
void CUniCheckApp::LoadScansForDisplay(CListBox *pScansListBox)
{
	/* Local Variable Declarations and Initialization */

	int nIndex = 0;										// The Enumeration Index Used to Walk the Installed Scans Subkeys
	int nScanIndex = UNICHECK_RETURN_NOINDEX;			// The Index of an Item in the List Box
	int nResult = ERROR_SUCCESS;						// The Result of the Registry APIs

	UINT uLastScanType = NULL;							// The Largest Scan Type Value of the Retrieved Scans

	TCHAR szScanType[MAX_PATH] = {NULL_CHAR};			// String Representation of the Scan Type
	TCHAR szScansKeyFull[MAX_PATH] = {NULL_CHAR};		// The String Specifying the Full Path to the Scans Registry Key
	TCHAR szScansKeyUnicode[MAX_PATH] = {NULL_CHAR};	// The String Specifying the Full Path to the Unicode Scan Registry Key
	TCHAR szScanTypeKey[MAX_PATH] = {NULL_CHAR};		// The Parent Key Name for Each Installed Scan's Settings

	CString szProgramPath;								// Directory Path to the Application Installation Directory

	HKEY hRegistryKey = NULL;							// Handle to the Installed Scan Registry Key

	DSSCAN dsScan;										// The Attributes of the Currently Selected Installed Scan
	DSPARAMETER dsParam;								// The Attributes of a Scan Parameter


	// Only attempt to load scans if the application is fully initialized and the list box control exists.
	if ( (IsAppInitialized()) && (pScansListBox != NULL) )
	{
		// Get the program path.
		szProgramPath = GetProgramPath();


		/* Attempt to get the Installed Scans registry key. */

		// Reset the scan and parameter structures to ensure clean data.
		ClearScanStructure(&dsScan);
		ClearParameterStructure(&dsParam);

		// Convert the scan type to a string.
		_itot(UNICHECK_SCANTYPE_UNICODE, szScanType, 10);

		// Build the full path to the installed scans registry key and the Unicode scan key.
		wsprintf(szScansKeyFull, "%s%s%s", UNICHECK_REG_SOFTWAREKEY, SLASH, UNICHECK_REG_INSTSCANSKEY);
		wsprintf(szScansKeyUnicode, "%s%s%s", szScansKeyFull, SLASH, szScanType);


		// Get a handle to the installed Unicode scan registry key.
		nResult = RegOpenKeyEx(HKEY_CURRENT_USER, szScansKeyUnicode, 0, KEY_ENUMERATE_SUB_KEYS, &hRegistryKey);

		// If the registry key does not exist, create the registry key, load initial settings, and install the Unicode scan.
		if ( nResult != ERROR_SUCCESS )
		{
			/* If the Unicode scan has not been installed (no scans have ever been installed), install the default scans. */

			// Load the default Unicode scan settings.
			dsScan.uScanType = UNICHECK_SCANTYPE_UNICODE;
			dsScan.szScanTitle = UNICHECK_SCANTITLE_UNICODE;
			dsScan.szScanEngine = szProgramPath + UNICHECK_SCANENG_UNICODE;
			dsScan.szResultsFile = UNICHECK_RESULTFILE_UNICODE;
			dsScan.szResultsFilePath = szProgramPath;
			dsScan.bUsesCommandPrompt = FALSE;
			dsScan.bUsesFileSet = FALSE;
			dsScan.bUsesSelectedFile = FALSE;
			dsScan.bUsesTempFolders = TRUE;

			dsParam.uParameterID = 1;
			dsParam.szParameter = UNICHECK_SCANATT_UNISCRIPT;
			dsParam.bUsesParameterPath = TRUE;
			dsParam.szParameterPath = szProgramPath;
			dsParam.bAppendEndSpace = TRUE;
			dsScan.ParameterList.AddTail(dsParam);

			ClearParameterStructure(&dsParam);
			dsParam.uParameterID = 2;
			dsParam.szParameter = UNICHECK_PARAMSUB_RESFILE;
			dsParam.bUsesParameterPath = FALSE;
			dsParam.szParameterPath = NULL_TEXT;
			dsParam.bAppendEndSpace = TRUE;
			dsScan.ParameterList.AddTail(dsParam);

			ClearParameterStructure(&dsParam);
			dsParam.uParameterID = 3;
			dsParam.szParameter = UNICHECK_SCANATT_UNIMODE;
			dsParam.bUsesParameterPath = FALSE;
			dsParam.szParameterPath = NULL_TEXT;
			dsParam.bAppendEndSpace = FALSE;
			dsScan.ParameterList.AddTail(dsParam);

			// Install the Unicode scan.
			if ( !InstallScan(&dsScan) )
			{
				// If the scan failed to install, no need to handle it.
			}
		}


		// Get a handle to the installed scans registry key.
		nResult = RegOpenKeyEx(HKEY_CURRENT_USER, szScansKeyFull, 0, KEY_ENUMERATE_SUB_KEYS, &hRegistryKey);

		// If the Installed Scans key is opened, load all the installed scan to the interface.
		if ( nResult == ERROR_SUCCESS )
		{
			/* If the Installed Scans key exists, loop through each installed scan and load it to the interface. */

			// Clear the list box of any previous scans.
			if ( pScansListBox != NULL )
			{
				pScansListBox->ResetContent();
			}

			if (hRegistryKey != NULL )
			{
				// Fetch each installed scan type.
				while ( RegEnumKey(hRegistryKey, nIndex, szScanTypeKey, 255) != ERROR_NO_MORE_ITEMS )
				{
					// Clear the scan structure to ensure clean data.

					ClearScanStructure(&dsScan);

					// Fetch each installed scan.
					if ( GetInstalledScan(_ttoi(szScanTypeKey), &dsScan) )
					{
						// If the scan is retrieved, load the scan to the list box.
						nScanIndex = pScansListBox->AddString(dsScan.szScanTitle);
						if ( nScanIndex > UNICHECK_RETURN_NOINDEX )
						{
							pScansListBox->SetItemData(nScanIndex, dsScan.uScanType);
						}
					}

					// Determine the largest scan type value retrieved from the registry.
					if ( dsScan.uScanType > uLastScanType )
					{
						uLastScanType = dsScan.uScanType;
					}

					// Increment the index and reset the key name string.
					nIndex++;
					ZeroMemory(szScanTypeKey, sizeof(szScanTypeKey));
				}

				// Store value of the greatest scan type read to use as the seed for the scan type generator.
				SetScanTypeSeed(uLastScanType);

				// Clear the scan type structure to prevent unexpected usage of the values in later processing.
				ClearScanStructure(&dsScan);
			}
		}

		// Close the handle to the installed scans registry key if it is open.
		if ( hRegistryKey != NULL )
		{
			RegCloseKey(hRegistryKey);
		}
	}
}

/* DisplayMessageToUser - Display a Message/Error to the User That is Appropriate for the Application Processing Mode */
void CUniCheckApp::DisplayMessageToUser(CListBox* pUserFeedbackDisplay, CString szMessageLine1, CString szMessageLine2, BOOL bIsError)
{
	/* Determine how to display message to the user. */

	// If the application is running in interactive mode, then write all messages to the User Feedback Display Window.
	if (GetProcessingMode() == UNICHECK_MODE_INTERACTIVE)
	{
		// If the control is properly initialized, then write the message to the User Feedback Display Window.
		if (pUserFeedbackDisplay != NULL)
		{
			if (!szMessageLine1.IsEmpty())
			{
				pUserFeedbackDisplay->AddString(szMessageLine1);
			}
			if (!szMessageLine2.IsEmpty())
			{
				pUserFeedbackDisplay->AddString(szMessageLine2);
			}
		}
	}

	// If the message is an error, also display an error message box.
	// This is the standard mechanism for displaying messages in command line mode.
	if ( bIsError )
	{
		if (!szMessageLine1.IsEmpty())
		{
			if (!szMessageLine2.IsEmpty())
			{
				szMessageLine1 = szMessageLine1 + DOUBLENEWLINE + szMessageLine2;
			}

			MessageBox(NULL, szMessageLine1.GetBuffer(sizeof(szMessageLine1)), UNICHECK_TEXT_ERRORTITLE, MB_ICONERROR);
		}
	}
}

/* CommandLineModeProcessing - Process Non-UniveralChecker List Files of Valid Types Passed as Parameters on the Command Line */
void CUniCheckApp::CommandLineModeProcessing(CCommandLineInfo *cmdInfo)
{
	/* Local Variable Declarations and Initialization */

	BOOL bFilesExist = FALSE;	// Flag Specifying that Files have been Copied to the Temp Directories

	CString szProgramPath;		// The Path String to the UniversalChecker Program Directory
	CString szSourcePath;		// The Path String to the Tempory Source Files Directory
	CString szHeaderPath;		// The Path String to the Tempory Header Files Directory
	CString szExecutablePath;	// The Path String to the Scan .exe
	CString szScanArguments;	// The Scan .exe Arguments
	CString szScanTitle;		// The Title of Scan to Perform on the Document Data
	CString szScanEngine;		// The Executable of the Scan to Perform on the Document Data
	CString szViewExecutable;	// The Executable of the Program to Launch to View Scan Results

	DSLOADEDFILE	dsLoadedFile;		// The File to Process
	DSSCAN			dsUnicodeScan;		// The Attributes of the Unicode Scan

	CUniCheckDoc	theCommandLineDoc;	// The Document Instance to Use for Command Line Processing


	/* Load the attributes of the Unicode scan. */

	// Clear the scan structure to ensure clean data.
	ClearScanStructure(&dsUnicodeScan);

	// Fetch the Unicode scan attributes and continue processing if the settings are retrieved.
	if ( GetInstalledScan(UNICHECK_SCANTYPE_UNICODE, &dsUnicodeScan) )
	{
		// Load the program path string, scan type, scan engine, and view executable.
		szProgramPath = GetProgramPath();
		szViewExecutable = GetViewerPath();
		szScanTitle = dsUnicodeScan.szScanTitle;
		szScanEngine = dsUnicodeScan.szScanEngine;


		// Load the full paths to the temporary source and header directories needed for Unicode scan.
		szSourcePath = GetSourcePath();
		szHeaderPath = GetHeaderPath();


		// Only process command line mode functionality if the application has initialized correctly.
		if ( IsAppInitialized() )
		{
			// Delete any previous Unicode results file before the scan runs.
			DeleteSpecificResultsFile(UNICHECK_RESULTFILE_UNICODE);


			// Build the file from the command line information.
			dsLoadedFile.szOriginalPath = cmdInfo->m_strFileName;
			dsLoadedFile.szFileName = ExtractFileNameFromPath(cmdInfo->m_strFileName);
			dsLoadedFile.szFileExtension = DetermineFileExtension(cmdInfo->m_strFileName);

			// Copy the file to the appropriate directory.
			if( !theCommandLineDoc.CopyFileToTempDirectory(dsLoadedFile) )
			{
				// If the copy API failed, display an error to the user and exit the method.
				DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_FILESP + dsLoadedFile.szFileName + UNICHECK_MESSAGE_FAILLOAD), NULL_TEXT, TRUE);
				return;
			}


			/* Unicode Scan */

			/* Validate that the C source files and C/CPP files to scan for Unicode have been copied to the tempory directories. */

			// Scan the source files temporary directory to see if any files exist in that directory.
			bFilesExist = VerifyThatFilesExistInDirectory(szSourcePath);

			// Scan the header files temporary directory to see if any files exist in that directory.
			if (!bFilesExist)
			{
				// Only scan the header directory if no files were found in the source directory.
				bFilesExist = VerifyThatFilesExistInDirectory(szHeaderPath);
			}

			// If files exist in the temp directories, continue processing the Unicode scan.
			if (bFilesExist)
			{
				// Build the command line for Unicode scan process.
				BuildScanCommandLine(UNICHECK_SCANTYPE_UNICODE, &szExecutablePath, &szScanArguments, NULL_TEXT);

				// Scan the document data.
				if(!ExecuteScanProcess(szExecutablePath, szScanArguments))
				{
					// If the scan process failed, write an error message to the User Feedback Display window.
					DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_VALENG + szScanEngine + UNICHECK_MESSAGE_FAILCOMP), 
										 (szScanTitle + UNICHECK_MESSAGE_XCOMP), TRUE);
				}
				
				// Reinitialize the scan .exe command line and arguments strings.
				szExecutablePath = NULL_TEXT;
				szScanArguments = NULL_TEXT;
			}
			else
			{
				// If no source or header files exist in the tempory directories, write a message to the User Feedback Display window.
				DisplayMessageToUser(NULL, UNICHECK_MESSAGE_0FILEUNI, NULL_TEXT, TRUE);
			}


			/* Display Results */

			// Display the results of the scan.	
			switch (ExecuteViewResultsProcess(szViewExecutable, (dsUnicodeScan.szResultsFilePath + dsUnicodeScan.szResultsFile)))
			{
			case UNICHECK_RETURN_FAILED:
				// If the view process failed to launch, write a message to the user.
				DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_CNLOCATE + szViewExecutable + UNICHECK_MESSAGE_TODISPRES), NULL_TEXT, TRUE);
				break;

			case UNICHECK_RETURN_NOFILE:
				// If the results file does not exist, display an error in the user.
				DisplayMessageToUser(NULL, (szScanTitle + UNICHECK_MESSAGE_RESXEXIST), NULL_TEXT, TRUE);
				break;

			default:
				break;
			}


			// Clean up after the scan is complete.
			DeleteSpecificResultsFile(UNICHECK_RESULTFILE_UNICODE);
			theCommandLineDoc.ClearEvalFolders();
		}
	}
	else
	{
		// If the Unicode scan is not installed, display a message to the user.
		DisplayMessageToUser(NULL, UNICHECK_MESSAGE_UNIXINSTALLED, UNICHECK_MESSAGE_XCOMMANDLINE, TRUE);
	}
}

/* KillApp - Manually Close the Application Instance */
void CUniCheckApp::KillApp()
{
	/* Local Variable Declarations and Initialization */

	HWND hWnd = NULL;	// Handle to the Main Frame Window

	
	// Attempt to get a handle to the application's main window.
	hWnd = this->GetMainWnd()->GetSafeHwnd();

	// Verify that a valid handle was retrieved before firing the message.
	if(hWnd != NULL)
	{
		// Send the WM_CLOSE message to have the application close using the normal close protocol.
		SendMessage(hWnd, WM_CLOSE, NULL, NULL);
	}
}

/* CreateControlFile - Build a Control Text File Required by the Unicode Scan */
UINT CUniCheckApp::CreateControlFile()
{
	/* Local Variable Declarations and Initialization */

	UINT uReturnValue = UNICHECK_RETURN_SUCCESS;	// Return Value Indicating the Status of the Function Run

	CString szFilePath;					// Full Path to the File to Create
	CString szProgramPath;				// Directory Path to the Program Directory
	CString szSourcePath;				// Directory Path to the Source Temporary Directory
	CString szHeaderPath;				// Directory Path to the Header Temporary Directory
	CString szScriptFileLine;			// Line of Text to Write to the patternset.txt File

	FILE* fControlScriptFile = NULL;	// Pointer the Physical File to Create


	// Get the program path, temporary source path, and temporary header path.
	szProgramPath = GetProgramPath();
	szSourcePath = GetSourcePath();
	szHeaderPath = GetHeaderPath();


	// Only attempt to create the script file if the all paths contain a value.
	if ( IsAppInitialized() )
	{
		// Build the full path to the script file.
		szFilePath = szProgramPath + UNICHECK_CONTROLSCRIPT_FILE;

		// Try to delete the script file to remove any previous command script.
		if(!DeleteFile(szFilePath))
		{
			// Assuming this will only fail if the file does not exist, there 
			// is no need to handle a failure to delete the script file here.
		}

		// Create the script file.
		if ( (fControlScriptFile = fopen(szFilePath.GetBufferSetLength(szFilePath.GetLength()), UNICHECK_FILESET_CREATEMODE)) == NULL )
		{
			// Bypass further processing and indicate that the script file failed to create.
			uReturnValue = UNICHECK_RETURN_FILEIOFAIL;
		}
		else
		{
			/* If the file was created, add the temporary source and header paths to the script. */

			// Build the control script file command string for the temporary source directory.
			szScriptFileLine = szSourcePath + UNICHECK_CONTROLSCRIPT_LINETRAILER;

			// Write the command referencing the temporary source directory path to the script file.
			fwrite(szScriptFileLine.GetBuffer(szScriptFileLine.GetLength()), 
											  sizeof(TCHAR), 
											  szScriptFileLine.GetLength(), 
											  fControlScriptFile);

			// Build the control script file command string for the temporary header directory.
			szScriptFileLine = szHeaderPath + UNICHECK_CONTROLSCRIPT_LINETRAILER;

			// Write the command referencing the temporary header directory path to the script file.
			fwrite(szScriptFileLine.GetBuffer(szScriptFileLine.GetLength()), 
											  sizeof(TCHAR), 
											  szScriptFileLine.GetLength(), 
											  fControlScriptFile);

			// Close the fileset file.
			if (fControlScriptFile != NULL)
			{
				fclose(fControlScriptFile);
			}

			// Return that the script file was created successfully.
			uReturnValue = UNICHECK_RETURN_SUCCESS;
		}
	}

	// Return the result of the file creation process.
	return uReturnValue;
}
