// UniCheckConstants.h : Header File Containing Constants Definitions for the UNICHECK Application
//
/*
 * Copyright (c) 2004 Mark Johnson
 */


/* Application Constants */

// General Constants
#define NULL_CHAR					NULL
#define NULL_TEXT					TEXT("")
#define SPACE						TEXT(" ")
#define QUOTE						TEXT("\"")
#define QUOTESPACEQUOTE				TEXT("\" \"")
#define SLASH						TEXT("\\")
#define SLASH_CHAR					_T('\\')
#define PERIOD						TEXT(".")
#define PERIOD_CHAR					_T('.')
#define NEWLINE						TEXT("\n")
#define DOUBLENEWLINE				TEXT("\n\n")

// Sizes and Lengths Constants
#define UNICHECK_SIZE_ADDFILEBUFLEN	(200 * MAX_PATH)	// Accommodate Upto 200 Very Long File Names


// Processing Modes
#define UNICHECK_MODE_INTERACTIVE	1000	// Interactive Mode
#define UNICHECK_MODE_COMMANDLINE	1001	// Command Line Mode


// Generic Interal Function Return Values
#define UNICHECK_RETURN_FAILED		0
#define UNICHECK_RETURN_SUCCESS		1

// ExecuteViewResultsProcess Return Values
#define UNICHECK_RETURN_NOEXE		2
#define UNICHECK_RETURN_NOFILE		3

// CreatePatternSet Return Values
#define UNICHECK_RETURN_INITIAL		4
#define UNICHECK_RETURN_REFRESH		5

// AddFileToDocument Return Values
#define UNICHECK_RETURN_FILECORRUPT	6

// CreateControlFile Return Values
#define UNICHECK_RETURN_FILEIOFAIL	7

// GetScanIndex Return Values
#define UNICHECK_RETURN_NOINDEX		-1


// Registry Keys
#define UNICHECK_REG_SOFTWAREKEY	TEXT("Software\\Majic Software\\UniversalChecker")

#define UNICHECK_REG_ROOTKEY		TEXT("Majic Software")
#define UNICHECK_REG_SETTINGSKEY	TEXT("Settings")
#define UNICHECK_REG_PROGPATHKEY	TEXT("Program Path")
#define UNICHECK_REG_HDRPATHKEY		TEXT("Header Path")
#define UNICHECK_REG_SRCPATHKEY		TEXT("Source Path")
#define UNICHECK_REG_FILESETKEY		TEXT("Fileset File")
#define UNICHECK_REG_FILESETPATHKEY	TEXT("Fileset Path")
#define UNICHECK_REG_VIEWERPATHKEY	TEXT("Results Viewer")
#define UNICHECK_REG_MAXFILEKEY		TEXT("Maximum Files")
#define UNICHECK_REG_MAXFILEDEFAULT	20
#define UNICHECK_REG_MAXFILEFLOOR	1
#define UNICHECK_REG_MAXFILECEILING	200

#define UNICHECK_REG_INSTSCANSKEY	TEXT("Installed Scans")
#define UNICHECK_REG_SCANTYPEKEY	TEXT("Type")
#define UNICHECK_REG_SCANTITLEKEY	TEXT("Title")
#define UNICHECK_REG_SCANENGKEY		TEXT("Engine")
#define UNICHECK_REG_SCANRESFILEKEY	TEXT("Results File")
#define UNICHECK_REG_SCANRESPATHKEY	TEXT("Results File Path")
#define UNICHECK_REG_SCANCMDKEY		TEXT("Uses Command Prompt")
#define UNICHECK_REG_SCANTEMPDIRKEY	TEXT("Uses Temporary Analysis Folders")
#define UNICHECK_REG_SCANFILESETKEY	TEXT("Uses File Set")
#define UNICHECK_REG_SCANSELFILEKEY	TEXT("Uses Selected File")

#define UNICHECK_REG_PARAMETERSKEY	TEXT("Parameters")
#define UNICHECK_REG_PARAMIDKEY		TEXT("ID")
#define UNICHECK_REG_PARAMVALUEKEY	TEXT("Value")
#define UNICHECK_REG_PARAMUPATHKEY	TEXT("Uses Parameter Path")
#define UNICHECK_REG_PARAMPATHKEY	TEXT("Parameter Path")
#define UNICHECK_REG_PARAMENDSPKEY	TEXT("Append End Space")

#define UNICHECK_REG_FILETYPESKEY	TEXT("Valid File Types")
#define UNICHECK_REG_FILETYPEKEY	TEXT("Extension")
#define UNICHECK_REG_FTYPEDESCKEY	TEXT("Description")
#define UNICHECK_REG_FTYPEGROUPKEY	TEXT("Grouping")


// Paths
#define UNICHECK_PATH_ALLFILES		TEXT("*.*")

// File Types (Extensions)
#define UNICHECK_FILETYPE_SOURCE	TEXT("cpp")
#define UNICHECK_FILETYPE_HEADER	TEXT("h")
#define UNICHECK_FILETYPE_LIST		TEXT("ucl")
#define UNICHECK_FILETYPE_VS6		TEXT("dsp")
#define UNICHECK_FILETYPE_VS7		TEXT("vcproj")

// Base Scan Type
#define UNICHECK_SCANTYPE_ALLSCANS	2000

// Unicode Scan Attributes
#define UNICHECK_SCANTYPE_UNICODE	2001	// This scan requires specific logic built right into the app.
#define UNICHECK_SCANTITLE_UNICODE	TEXT("Unicode")
#define UNICHECK_SCANENG_UNICODE	TEXT("CodeChangeCom.exe")
#define UNICHECK_RESULTFILE_UNICODE	TEXT("unicoderesults.txt")
#define UNICHECK_SCANATT_UNISCRIPT	TEXT("command.txt")
#define UNICHECK_SCANATT_UNIMODE	TEXT("audit")

// Fileset File Processing
#define UNICHECK_FILESET_CREATEMODE	TEXT("w")
#define UNICHECK_FILESET_LINEHEADER	TEXT("SOURCE=")

// Non-UniversalChecker List File Processing
#define UNICHECK_DATAFILE_READMODE		TEXT("r")
#define UNICHECK_DATAFILE_READBUFSIZE	512
#define UNICHECK_DATAFILE_VS6PATHID		TEXT("SOURCE=")
#define UNICHECK_DATAFILE_VS7PATHID		TEXT("RelativePath=\"")
#define UNICHECK_DATAFILE_VS6PATHTERM	TEXT("\n")
#define UNICHECK_DATAFILE_VS7PATHTERM	TEXT("\"")

// Unicode Control Script File Processing
#define UNICHECK_CONTROLSCRIPT_FILE			TEXT("command.txt")
#define UNICHECK_CONTROLSCRIPT_LINETRAILER	TEXT(" -r\n")

// File Search Return Values
#define UNICHECK_SEARCH_NOFILEFOUND	TEXT("NFF")

// Loaded Files Tree Nodes
#define UNICHECK_FILETREE_ROOTNODE		TEXT("Files to Validate")
#define UNICHECK_FILETREE_SOURCENODE	TEXT("Source Files")
#define UNICHECK_FILETREE_HEADERNODE	TEXT("Header Files")

// Files Filters
#define UNICHECK_FILTERCOMPS_AFILES	TEXT("All Files (*.*)|*.*|")
#define UNICHECK_FILTERCOMPS_COMP1	TEXT(" (*.")
#define UNICHECK_FILTERCOMPS_COMP2	TEXT(")|*.")
#define UNICHECK_FILTERCOMPS_COMP3	TEXT("|")
#define UNICHECK_OPENFILEDLG_FILTER	"UniversalChecker Lists (*.ucl)|*.ucl|Visual Studio 6 Projects (*.dsp)|*.dsp|Visual Studio 7/7.1 Projects (*.vcproj)|*.vcproj||"
#define UNICHECK_EXECDLG_FILTER		"Executables (*.exe)|*.exe|Command Processors (*.com)|*.com|Batch Scripts (*.bat)|*.bat||"

// Command Prompt Emulator
#define UNICHECK_COMEMUL_SYSVARNAME	_T("ComSpec")
#define UNICHECK_COMEMUL_UNIOUTPUT	TEXT(" /U")
#define UNICHECK_COMEMUL_ANSIOUTPUT	TEXT(" /A")
#define UNICHECK_COMEMUL_TERMINATE	TEXT(" /C ")

// Scan Parameter Edit Modes
#define UNICHECK_PARAMMODE_ADD		3000
#define UNICHECK_PARAMMODE_EDIT		3001

// Scan Parameter Substitution IDs
#define UNICHECK_PARAMSUB_FSFILE	TEXT("${ff}")
#define UNICHECK_PARAMSUB_RESFILE	TEXT("${rf}")
#define UNICHECK_PARAMSUB_SELFILE	TEXT("${sf}")

// Scan Parameter Substitution IDs
#define UNICHECK_GROUPING_HEADER	TEXT("H")
#define UNICHECK_GROUPING_SOURCE	TEXT("S")


/* Application Text Definitions */

// COptionsDlg Text
#define UNICHECK_TEXT_SELPATH		TEXT("Select Program Path:")

// CAddFileDialog Text
#define UNICHECK_TEXT_ADDFILE		TEXT("Select Files to Validate")
#define UNICHECK_TEXT_SELVIEWER		TEXT("Select Program to View Results")
#define UNICHECK_TEXT_SELSCANEXE	TEXT("Select Scan Program")

// CCLPreviewDlg Text
#define UNICHECK_TEXT_SELFILE		TEXT("{SelectedFile}")

// CScanDefDlg Text
#define UNICHECK_TEXT_ADDPARAMBTN	TEXT("Add")
#define UNICHECK_TEXT_UPDPARAMBTN	TEXT("Update")

// Message Box Text
#define UNICHECK_TEXT_ERRORTITLE	TEXT("UniversalChecker Error")
#define UNICHECK_TEXT_WARNINGTITLE	TEXT("UniversalChecker Warning")

// Message Text
#define UNICHECK_MESSAGE_FILESP			TEXT("File ")
#define UNICHECK_MESSAGE_NOSP			TEXT("No ")
#define UNICHECK_MESSAGE_CNEVALED		TEXT(" cannot be evaluated.")
#define UNICHECK_MESSAGE_VALTYPE		TEXT("The file must be a .cpp or .h format file.")
#define UNICHECK_MESSAGE_SEEVALTYPE		TEXT("See \"File Types\" under the \"Tools\" menu for a list of valid types.")
#define UNICHECK_MESSAGE_LOADFROM		TEXT(" specifications loaded from ")
#define UNICHECK_MESSAGE_FAILLOAD		TEXT(" failed to load.")
#define UNICHECK_MESSAGE_FAILLOADFROM	TEXT(" failed to load from ")
#define UNICHECK_MESSAGE_VALENG			TEXT("Validation engine ")
#define UNICHECK_MESSAGE_FAILCOMP		TEXT(" failed to complete.")
#define UNICHECK_MESSAGE_COMP			TEXT(" compliance of files evaluated.")
#define UNICHECK_MESSAGE_XCOMP			TEXT(" compliance cannot be evaluated.")
#define UNICHECK_MESSAGE_0FILEUNI		TEXT("No files exist in the staging location for the Unicode scan to evaluate.")
#define UNICHECK_MESSAGE_0FILEINTEMP	TEXT("No files have been loaded to evaluate.")
#define UNICHECK_MESSAGE_CNLOCATE		TEXT("Unable to locate ")
#define UNICHECK_MESSAGE_TODISPRES		TEXT(" to display results.")
#define UNICHECK_MESSAGE_RESXEXIST		TEXT(" results do not exist.")
#define UNICHECK_MESSAGE_APPXINIT		TEXT("UniversalChecker failed to fully initialize.")
#define UNICHECK_MESSAGE_SETVALENG		TEXT("The location of the program, temporary analysis directories, or results viewer must be set.")
#define UNICHECK_MESSAGE_APPINIT		TEXT("UniversalChecker initialized.  Ready for processing.")
#define UNICHECK_MESSAGE_FILECORUPT		TEXT("The UniversalCheck list is corrupt.")
#define UNICHECK_MESSAGE_APPMUSTDIE		TEXT("The application must shut down.")
#define UNICHECK_MESSAGE_FILETYPE		TEXT("Files of type .")
#define UNICHECK_MESSAGE_XADDSET		TEXT(" not added to set.")
#define UNICHECK_MESSAGE_APPXSET		TEXT("UniversalChecker is not properly set up.")
#define UNICHECK_MESSAGE_VERPATHSET		TEXT("Verify that the program and analysis paths are set.")
#define UNICHECK_MESSAGE_VERFSSET		TEXT("Verify that the fileset file and path is set.")
#define UNICHECK_MESSAGE_0SCANEXE		TEXT("No scan type selected.  No scan executed.")
#define UNICHECK_MESSAGE_XCOMPROMPT		TEXT("Unable to locate command prompt.  Loop, pattern, and table scans are unavailable.")
#define UNICHECK_MESSAGE_0RESDISP		TEXT("No scan type selected.  No results displayed.")
#define UNICHECK_MESSAGE_RELOADED		TEXT(" re-loaded.")
#define UNICHECK_MESSAGE_XDOC			TEXT("The UniversalChecker list is unavailable.")
#define UNICHECK_MESSAGE_0RESCLEAR		TEXT("No scan type selected.  No results files cleared.")
#define UNICHECK_MESSAGE_SETFSFILE		TEXT("UniversalChecker will use fileset file ")
#define UNICHECK_MESSAGE_XSETFSFILE		TEXT("UniversalChecker is unable to store the fileset file name ")
#define UNICHECK_MESSAGE_SETAPPPATH		TEXT("UniversalChecker will use path ")
#define UNICHECK_MESSAGE_XSETAPPPATH	TEXT("UniversalChecker is unable to store the path ")
#define UNICHECK_MESSAGE_SETMAXFILEHDR	TEXT("User can add up to ")
#define UNICHECK_MESSAGE_SETMAXFILETLR	TEXT(" files at one time.")
#define UNICHECK_MESSAGE_XSETMAXFILE	TEXT("UniversalChecker is unable to store the maximum number of files to add at one time.")
#define UNICHECK_MESSAGE_XTREEUPDATE	TEXT("The application was unable to update the file tree.")
#define UNICHECK_MESSAGE_XREMOVE		TEXT(" could not be removed.")
#define UNICHECK_MESSAGE_REMOVE			TEXT(" successfully removed.")
#define UNICHECK_MESSAGE_RESTOCLEAR		TEXT(" results file to clear.")
#define UNICHECK_MESSAGE_DELETED		TEXT(" deleted.")
#define UNICHECK_MESSAGE_RESCLEARED		TEXT(" results file cleared.")
#define UNICHECK_MESSAGE_XCREATESET		TEXT("Failed to create pattern set file.")
#define UNICHECK_MESSAGE_CREATE1SET		TEXT("Creating initial pattern set.")
#define UNICHECK_MESSAGE_RESETSET		TEXT("Pattern set file reset.")
#define UNICHECK_MESSAGE_RESDISP		TEXT(" results displayed in separate window.")
#define UNICHECK_MESSAGE_CREATESCRIPT	TEXT("Creating Unicode command script.")
#define UNICHECK_MESSAGE_XCREATESCRPT	TEXT("Failed to create Unicode command script.")
#define UNICHECK_MESSAGE_XUNICODESCAN	TEXT("The Unicode scan is unavailable.")
#define UNICHECK_MESSAGE_MAXFILEEXCEED	TEXT("The number of files selected exceeds the maximum number allowed.  Selecting a large number of files could consume a very large amount of disk space.")
#define UNICHECK_MESSAGE_MAXFILEIS		TEXT("The maximum number of files that can be added to a list at one time is ")
#define UNICHECK_MESSAGE_XINITUNICODE	TEXT("The Unicode scan failed to initialize.")
#define UNICHECK_MESSAGE_CHECKOPTS		TEXT("Please verify the program settings by selecting Options from the Tools menu.")
#define UNICHECK_MESSAGE_XNEXISTS		TEXT(" does not exist.")
#define UNICHECK_MESSAGE_XINSTSCAN		TEXT("The scan failed to install.")
#define UNICHECK_MESSAGE_THESCAN		TEXT("The scan ")
#define UNICHECK_MESSAGE_XINIT			TEXT(" failed to initialize.")
#define UNICHECK_MESSAGE_SCANUNAVAIL	TEXT(" scan is unavailable.")
#define UNICHECK_MESSAGE_XPROPINST		TEXT(" is not properly installed.")
#define UNICHECK_MESSAGE_SCANTBREMOVED	TEXT("The scan will be removed.")
#define UNICHECK_MESSAGE_FTYPE			TEXT("File type .")
#define UNICHECK_MESSAGE_XINSTALLED		TEXT(" failed to install.")
#define UNICHECK_MESSAGE_XUNINSTALLED	TEXT(" failed to uninstall.")
#define UNICHECK_MESSAGE_FTDEFXVALIDATE	TEXT("All fields must have a value.")
#define UNICHECK_MESSAGE_SDEFXVALTITLE	TEXT("The scan title field must have a value.")
#define UNICHECK_MESSAGE_SDEFXVALENG	TEXT("The scan program field must have a value.")
#define UNICHECK_MESSAGE_SDEFXVALRFILE	TEXT("The results file field must have a value.")
#define UNICHECK_MESSAGE_SDEFXVALRFPATH	TEXT("The results file path field must have a value.")
#define UNICHECK_MESSAGE_SDEFXVALPARAM	TEXT("The parameter value field must have a value.")
#define UNICHECK_MESSAGE_SDEFXVALPPATH	TEXT("The param. path field must have a value.")
#define UNICHECK_MESSAGE_UNIXINSTALLED	TEXT("The integrated Unicode scan is not installed.")
#define UNICHECK_MESSAGE_XCOMMANDLINE	TEXT("Command line processing is not available.")
#define UNICHECK_MESSAGE_TEMPFLDRWARN	TEXT("CAUTION:\nThe selected directory must be empty.\nAny files in the selected directory will be permanently deleted during scan processing.\n\nPlease select a directory that is normally empty and used exclusively by UniversalChecker.")
