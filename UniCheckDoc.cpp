// UniCheckDoc.cpp : Implementation of the CUniCheckDoc Class
//
/*
 * Copyright (c) 2004 Mark Johnson
 */

#include "stdafx.h"
#include "UniCheck.h"
#include "UniCheckDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CUniCheckDoc

IMPLEMENT_DYNCREATE(CUniCheckDoc, CDocument)

BEGIN_MESSAGE_MAP(CUniCheckDoc, CDocument)
	//{{AFX_MSG_MAP(CUniCheckDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUniCheckDoc Construction/Destruction

CUniCheckDoc::CUniCheckDoc()
{
}

CUniCheckDoc::~CUniCheckDoc()
{
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckDoc Serialization Methods

/* Serialize - Transfer Data to and from a Disk File */
void CUniCheckDoc::Serialize(CArchive& ar)
{
	/* Save or load the application data. */

	// Call the serialize method of the CList members of the document.
	m_LoadedFiles.Serialize(ar);	// List of File Names
	m_LoadedPaths.Serialize(ar);	// List of File Paths
	m_LoadedExts.Serialize(ar);		// List of File Extensions

	if ( ar.IsStoring() )
	{
		// Save non-Class document members to a file.
	}
	else
	{
		// Load non-Class document members from a file.
	}
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckDoc Diagnostic Methods

#ifdef _DEBUG
void CUniCheckDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUniCheckDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CUniCheckDoc Command Handlers

/* OnNewDocument - New File Handler */
BOOL CUniCheckDoc::OnNewDocument()
{
	/* Prepare a new document. */

	// Delete source and header files from the temporary directories and reset the document data.
	ClearEvalFolders();
	if(!ClearDocumentData())
	{
		// If the document data fails to refresh, exit and signal failure.
		return FALSE;
	}
	ClearBackupData();

	// Process the parent implementation of the function, exiting if fails.
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

/* OnOpenDocument - Open File Handler */
BOOL CUniCheckDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	/* Local Variable Declarations and Initialization */

	CString szFileExtension;		// The Extension of the File Being Opened

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	/* Prepare a new document. */

	// Delete source and header files from the temporary directories and reset the document data.
	ClearEvalFolders();
	if(!ClearDocumentData())
	{
		// If the document data fails to refresh, exit and signal failure.
		return FALSE;
	}
	ClearBackupData();


	/* Process valid, but non-UniversalChecker, file types. */

	// Determine the file extension to determine the file type.
	if ( pTheApp != NULL )
	{
		szFileExtension = pTheApp->DetermineFileExtension(lpszPathName);
	}


	// If the file is a Visual Studio 6 project file, open it accordingly.
	if ( szFileExtension == UNICHECK_FILETYPE_VS6 )
	{
		// Treat this as extracting data from the project file into a new list, so fully process a new document and then
		// extract the data.
		if (!CUniCheckDoc::OnNewDocument())
			return FALSE;

		// Extract the file references from the project file, add them to the document data, and return success or failure.
		return ProcessVSProjectFile(lpszPathName, UNICHECK_DATAFILE_VS6PATHID, UNICHECK_DATAFILE_VS6PATHTERM);
	}
		
	// If the file is a Visual Studio 7/7.1 project file, open it accordingly.
	if ( szFileExtension == UNICHECK_FILETYPE_VS7 )
	{
		// Treat this as extracting data from the project file into a new list, so fully process a new document and then
		// extract the data.
		if (!CUniCheckDoc::OnNewDocument())
			return FALSE;

		// Extract the file references from the project file, add them to the document data, and return success or failure.
		return ProcessVSProjectFile(lpszPathName, UNICHECK_DATAFILE_VS7PATHID, UNICHECK_DATAFILE_VS7PATHTERM);
	}


	// Process the parent implementation of the function, exiting if fails.
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckDoc Data Access Methods

/* AddFileToDocument - Add a Specific File to the Document Data */
UINT CUniCheckDoc::AddFileToDocument(DSLOADEDFILE dsFile)
{
	// Save copies of the un-modified document data for restoration if an error occurs.
	if(!BackupDocumentData())
	{
		// If a backup of document data couldn't be created, return and signal failure 
		// to add the file to the document.
		return UNICHECK_RETURN_FAILED;
	}

	// Add each of the three elements of the file to the appropriate
	// link list.
	if ( ( m_LoadedExts.AddTail(dsFile.szFileExtension) != NULL) &&
		 ( m_LoadedFiles.AddTail(dsFile.szFileName) != NULL) &&
		 ( m_LoadedPaths.AddTail(dsFile.szOriginalPath) != NULL) )
	{
		/* The file was added to each list successfully. */

		// Mark the document as modified to prompt the user to save upon exit.
		SetModifiedFlag(TRUE);

		// Return that the file was added correctly.
		return UNICHECK_RETURN_SUCCESS;
	}
	else
	{
		/* One or more of the file elements failed to add to the list. */

		// Restore the original state of the document data.
		if(!RestoreDocumentDataFromBackup())
		{
			// If the original document data cannot be restored, return that the document is corrupt.
			return UNICHECK_RETURN_FILECORRUPT;
		}

		// Return that the file could not be added to the document data.
		return UNICHECK_RETURN_FAILED;
	}	
}

/* CopyFileToTempDirectory - Copy a Specific File to the Appropriate Temporary Directory */
BOOL CUniCheckDoc::CopyFileToTempDirectory(DSLOADEDFILE dsFile)
{
	/* Local Variable Declarations and Initialization */

	CString szDestinationPath;	// Directory Path to Temporary Directory
	CString szCategory;			// Category Associated with the File's Type

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Get the category for the file type.
	if ( pTheApp != NULL )
	{
		if ( !pTheApp->GetValidFileType(dsFile.szFileExtension, NULL, &szCategory) )
		{
			// If the category could not be retreived, exit and signal the file failed to copy.
			return FALSE;
		}
	}


	// Get a reference to the appropriate tree control group node.
	if ( (dsFile.szFileExtension == UNICHECK_FILETYPE_HEADER) || (szCategory == UNICHECK_GROUPING_HEADER) )
	{
		// If the file is a C/CPP header file (.h) (to ensure the integrated Unicode scan works) or the valid file type
		// is associated with the header group, then load the Header File temporary directory path.
		if (pTheApp != NULL)
		{
			szDestinationPath = pTheApp->GetHeaderPath();
		}
	}
	else
	{
		// If the file is a C++ source file (.cpp) (to ensure the integrated Unicode scan works) or the valid file type
		// is associated with the source group, then load the Source File temporary directory path.
		if ( (dsFile.szFileExtension == UNICHECK_FILETYPE_SOURCE) || (szCategory == UNICHECK_GROUPING_SOURCE) )
		{
			if (pTheApp != NULL)
			{
				szDestinationPath = pTheApp->GetSourcePath();
			}
		}
	}


	// Copy the file only if the directory portion of the destination path has a valid value.
	if ( !szDestinationPath.IsEmpty() )
	{
		// Build the full directory path to the source file temporary directory.
		szDestinationPath = szDestinationPath + dsFile.szFileName;

		// Copy the file to the source files temporary directory.
		if ( !CopyFile(dsFile.szOriginalPath, szDestinationPath, FALSE) )
		{
			// Return that the file failed to copy.
			return FALSE;
		}
		else
		{
			// Return that the file was copied successfully.
			return TRUE;
		}
	}
	else
	{
		// If no directory is set, return that the file failed to copy.
		return FALSE;
	}
}

/* CopyFileToTempDirectory - Copy a Specific File to the Appropriate Temporary Directory Using the File Name */
BOOL CUniCheckDoc::CopyFileToTempDirectory(CString szFileName)
{
	/* Local Variable Declarations and Initialization */

	DSLOADEDFILE dsFile;	// The Complete File Retrieved from the Link Lists


	// Only attempt to copy if a file name is passed.
	if ( !szFileName.IsEmpty() )
	{
		// Get the associated file.
		dsFile = GetSpecificFileFromDocument(szFileName);

		if (dsFile.szOriginalPath != UNICHECK_SEARCH_NOFILEFOUND)
		{
			// Return the result of the DSLOADEDFILE version of CopyFileToTempDirectory.
			return CopyFileToTempDirectory(dsFile);
		}
		else
		{
			// If no file was found in the document data, signal failure.
			return FALSE;
		}
	}
	else
	{
		// If no file name is passed, signal failure.
		return FALSE;
	}
}

/* GetFirstFileInDocument - Get a Reference to the First File in the Document Data */
BOOL CUniCheckDoc::GetFirstFileInDocument(LPDSLISTPOSITION dsPosition)
{
	// Get the initial position of each file attribute link list in the document
	// data.
	dsPosition->extPosition  = m_LoadedExts.GetHeadPosition();
	dsPosition->filePosition = m_LoadedFiles.GetHeadPosition();
	dsPosition->pathPosition = m_LoadedPaths.GetHeadPosition();

	// Check if all three lists returned a valid initial position.
	if ( (dsPosition->extPosition != NULL) && 
		 (dsPosition->filePosition != NULL) &&
		 (dsPosition->pathPosition != NULL) )
	{
		// If all three returned a valid position return TRUE.
		return TRUE;
	}
	else
	{
		// If any of the three failed to return a valid position return FALSE.
		return FALSE;
	}
}

/* GetSpecificFileFromDocument - Get a Specific File from the Document Data */
DSLOADEDFILE CUniCheckDoc::GetSpecificFileFromDocument(LPDSLISTPOSITION dsPosition)
{
	/* Local Variable Declarations and Initialization */

	DSLOADEDFILE dsFile;	// The Complete File Retrieved from the Link Lists


	// Get all three attributes of the file at the input position and load all the
	// attributes to the file data structure.
	if ( (dsPosition->extPosition != NULL) && 
		 (dsPosition->filePosition != NULL) &&
		 (dsPosition->pathPosition != NULL) )
	{
		dsFile.szFileExtension = m_LoadedExts.GetNext(dsPosition->extPosition);
		dsFile.szFileName = m_LoadedFiles.GetNext(dsPosition->filePosition);
		dsFile.szOriginalPath = m_LoadedPaths.GetNext(dsPosition->pathPosition);
	}

	// Verify that a valid file was found.
	if ( (dsFile.szFileExtension.IsEmpty()) &&
		 (dsFile.szOriginalPath.IsEmpty()) &&
		 (dsFile.szFileExtension.IsEmpty()) )
	{
		// If a valid file was not found, set the path attribute to NFF to signal failure.
		dsFile.szOriginalPath = UNICHECK_SEARCH_NOFILEFOUND;
	}

	// Return the file.
	return dsFile;
}

/* GetSpecificFileFromDocument - Get a Specific File from the Document Data Using the File Name */
DSLOADEDFILE CUniCheckDoc::GetSpecificFileFromDocument(CString szFileName)
{
	/* Local Variable Declarations and Initialization */

	DSLOADEDFILE dsFile;			// The File Currently Being Processed
	DSLISTPOSITION dsPosition;		// Link List Position of the Current File in Document Data


	// Memset data structures.
	ZeroMemory(&dsPosition, sizeof(DSLISTPOSITION));


	// Only attempt to search for the file if a file name is passed.
	if ( !szFileName.IsEmpty() )
	{
		// Get a reference to the first element in the document data.
		if (GetFirstFileInDocument(&dsPosition))
		{
			// Retrieve the value of the current element in each link list, and increment references to the next element.
			dsFile = GetSpecificFileFromDocument(&dsPosition);

			/* Cycle through each element of the lists until the selected file name is found. */
			for(int i = 0; i < GetNumberOfLoadedFilesInDocument();i++)
			{
				// If the selected file name does match the file name at the list position, 
				// return the file structure.
				if (dsFile.szFileName == szFileName)
				{
					return dsFile;
				}

				// Retrieve the value of the current element in each link list, and increment references to the next element.
				dsFile = GetSpecificFileFromDocument(&dsPosition);
			}
		}
	}

	// If no file was found, or no file name passed, return the file structure with the path attribute set to No File Found ("NFF").
	dsFile.szFileName = NULL_TEXT;
	dsFile.szOriginalPath = UNICHECK_SEARCH_NOFILEFOUND;
	dsFile.szFileExtension = NULL_TEXT;
	return dsFile;
}

/* RemoveSpecificFileFromDocument - Completely Remove a File Reference from the Document */
BOOL CUniCheckDoc::RemoveSpecificFileFromDocument(DSLISTPOSITION dsPosition)
{
	// Remove each file attribute at the input position from each document
	// link list.
	if( (dsPosition.filePosition != NULL) &&
		(dsPosition.pathPosition != NULL) &&
		(dsPosition.extPosition != NULL) )
	{
		m_LoadedFiles.RemoveAt(dsPosition.filePosition);
		m_LoadedPaths.RemoveAt(dsPosition.pathPosition);
		m_LoadedExts.RemoveAt(dsPosition.extPosition);

		// Mark the document as modified to prompt the user to save upon exit.
		SetModifiedFlag(TRUE);

		// Return success.
		return TRUE;
	}
	else
	{
		// Valid positions were not passed, signal failure.
		return FALSE;
	}
}

/* RemoveSpecificFileFromDocument - Completely Remove a File Reference from the Document Using the File Name*/
BOOL CUniCheckDoc::RemoveSpecificFileFromDocument(CString szFileName)
{
	/* Local Variable Declarations and Initialization */

	DSLOADEDFILE dsFile;			// The File Currently Being Processed
	DSLISTPOSITION dsPosition;		// Link List Position of the Current File in Document Data
	DSLISTPOSITION dsPreviousPosition;	// Link List Position of the Previous File in Document Data


	// Memset data structures.
	ZeroMemory(&dsPosition, sizeof(DSLISTPOSITION));
	ZeroMemory(&dsPreviousPosition, sizeof(DSLISTPOSITION));


	// Only attempt to remove the file if a file name is passed.
	if ( !szFileName.IsEmpty() )
	{
		// Get a reference to the first element in each link list in the document.
		if ( GetFirstFileInDocument(&dsPosition) )
		{

			// Save the current references before the values at the current position is retreived.
			dsPreviousPosition = dsPosition;

			// Retrieve the value of the current element in each link list, and increment references to the next element.
			dsFile = GetSpecificFileFromDocument(&dsPosition);

			/* Cycle through each element of the lists until the selected file name is found. */
			while (dsFile.szFileName != szFileName)
			{
				// If the selected file name does not match the file name at the list posiiton, 
				// fetch the next element in the list.

				// If the entire contents of the document data are searched and no match is found, return and signal that 
				// the file could not be removed.
				if ( (dsPosition.extPosition == NULL) ||
					 (dsPosition.filePosition == NULL) ||
					 (dsPosition.pathPosition == NULL) )
				{
					return FALSE;
				}

				// Preserve the current position.
				dsPreviousPosition = dsPosition;

				// Retrieve the value of the current element in each link list, and increment references to the next element.
				dsFile = GetSpecificFileFromDocument(&dsPosition);
			}
		}

		// Remove the file from the document using the preserved position and return the result.
		return(RemoveSpecificFileFromDocument(dsPreviousPosition));
	}
	else
	{
		// If no file name is passed, return and signal that the file could not be removed.
		return FALSE;
	}
}

/* DeleteSpecificFileFromTempDirectory - Delete a Specific File from the Appropriate Temporary Directory */
BOOL CUniCheckDoc::DeleteSpecificFileFromTempDirectory(CString szFileName)
{
	/* Local Variable Declarations and Initialization */

	CString szTempFilePath;			// Path of the File Currently Being Processed
	CString szCategory;				// Category Associated with the File's Type

	DSLOADEDFILE dsLoadedFile;		// Loaded File Attributes Structure

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application Instance


	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	
	// Only attempt to delete the file if a file name is passed.
	if ( !szFileName.IsEmpty() )
	{
		// Get the file from document data.
		dsLoadedFile = GetSpecificFileFromDocument(szFileName);

		// Verify that a valid file was retrieved.
		if ( dsLoadedFile.szOriginalPath != UNICHECK_SEARCH_NOFILEFOUND )
		{
			// Get the category for the file type.
			if ( pTheApp != NULL )
			{
				if ( !pTheApp->GetValidFileType(dsLoadedFile.szFileExtension, NULL, &szCategory) )
				{
					// If the category could not be retreived, exit and signal the file failed to delete.
					return FALSE;
				}
			}

			// Build the full path to the file to delete in the temporary directory.
			if ( (dsLoadedFile.szFileExtension == UNICHECK_FILETYPE_HEADER) || (szCategory == UNICHECK_GROUPING_HEADER) )
			{
				// If the file is a C/CPP header file (.h) (to ensure the integrated Unicode scan works) or the valid file type
				// is associated with the header group, delete the file from the Header Files temporary directory path.
				if ( pTheApp != NULL )
				{
					szTempFilePath = pTheApp->GetHeaderPath();
				}
			}
			else 
			{
				// If the file is a C++ source file (.cpp) (to ensure the integrated Unicode scan works) or the valid file type
				// is associated with the source group, delete the file from the Source Files temporary directory path.
				if ( (dsLoadedFile.szFileExtension == UNICHECK_FILETYPE_SOURCE) || (szCategory == UNICHECK_GROUPING_SOURCE) )
				{
					if ( pTheApp != NULL )
					{
						szTempFilePath = pTheApp->GetSourcePath();
					}
				}
			}

			// Verify that the temporary directory path is set.
			if ( !szTempFilePath.IsEmpty() )
			{
				// Append the file name to the path.
				szTempFilePath = szTempFilePath + szFileName;

				// Delete the file from the temporary directory.
				if(!DeleteFile(szTempFilePath))
				{
					// Return that the file failed to delete.
					return FALSE;
				}
				else
				{
					// Return that the file was deleted successfully.
					return TRUE;
				}
			}
			else
			{
				// Signal failure if the temporary directory path is not set.
				return FALSE;
			}
		}
		else
		{
			// Signal failure if the file was not found in the document data.
			return FALSE;
		}
	}
	else
	{
		// Signal failure if no file name was passed.
		return FALSE;
	}
}

/* ClearDocumentData - Completely Remove All Data from the Document */
BOOL CUniCheckDoc::ClearDocumentData()
{
	// Remove file attributes from each document link list.
	m_LoadedFiles.RemoveAll();
	m_LoadedPaths.RemoveAll();
	m_LoadedExts.RemoveAll();

	// Verify the operation completed successfully.
	if ( (m_LoadedFiles.IsEmpty()) &&
		 (m_LoadedPaths.IsEmpty()) &&
		 (m_LoadedExts.IsEmpty()) )
	{
		// If all document data is cleared, signal success.
		return TRUE;
	}
	else
	{
		// If any data failed to clear, signal failure.
		return FALSE;
	}
}

/* GetNumberOfLoadedFilesInDocument - Get the Number of Elements in the Document Data */
int CUniCheckDoc::GetNumberOfLoadedFilesInDocument()
{
	/* Local Variable Declarations and Initialization */

	int nFileElements = 0;				// Number of File Elements in the Link Lists of the Document Data
	int nPathElements = 0;				// Number of Path Elements in the Link Lists of the Document Data
	int nExtElements = 0;				// Number of File Extenstion Elements in the Link Lists of the Document Data


	/* Return the number of elements in one of the file attribute link lists
	   in the document data.  All the file attribute lists should have the 
	   same count. */

	// Get the number of elements in each list.
	nFileElements = m_LoadedFiles.GetCount();
	nPathElements = m_LoadedPaths.GetCount();
	nExtElements = m_LoadedExts.GetCount();

	// Verify that all three lists have the same number of elements.
	if ( (nFileElements == nPathElements) &&
		 (nPathElements == nExtElements) )
	{
		// Return the valid element count.
		return nFileElements;
	}
	else
	{
		// If the lists do not agree on the number of elements in the document data, the document is corrupt.
		// Return -1 to indicate that the document is corrupt.
		return -1;
	}
}


/////////////////////////////////////////////////////////////////////////////
// CUniCheckDoc Utility Methods

/* BackupDocumentData - Create a Point-in-Time Copy of the Document Data */
BOOL CUniCheckDoc::BackupDocumentData()
{
	// Clear the any existing backup data.
	if(!ClearBackupData())
	{
		// If the backup data can not be reset to a pristine state, return and signal failure to create the backup.
		return FALSE;
	}


	// Save copies of the un-modified document data to backup class members.
	m_backupLoadedFiles.AddTail(&m_LoadedFiles);
	m_backupLoadedPaths.AddTail(&m_LoadedPaths);
	m_backupLoadedExts.AddTail(&m_LoadedExts);

	// Verify that the data was copied.
	if( (!m_backupLoadedFiles.IsEmpty()) &&
		(!m_backupLoadedPaths.IsEmpty()) &&
		(!m_backupLoadedExts.IsEmpty()) )
	{
		// If each list was copied successfully, signal success.
		return TRUE;
	}
	else
	{
		if ( (m_LoadedFiles.GetCount() == 0) &&
			 (m_LoadedPaths.GetCount() == 0) &&
			 (m_LoadedExts.GetCount() == 0) )
		{
			// If there are no elements in the original document data, return success even though the backups are empty.
			return TRUE;
		}
		else
		{
			// Clear any backup data on an error.
			ClearBackupData();

			// If any list failed to copy, signal failure.
			return FALSE;
		}
	}
}

/* RestoreDocumentDataFromBackup - Restore the Document Data Using the Point-in-Time Copy Created by BackupDocumentData */
BOOL CUniCheckDoc::RestoreDocumentDataFromBackup()
{
	// Clear all document data before attempting to restore data from a backup.
	if(!ClearDocumentData())
	{
		// If the document data could not be reset to a pristine state, return and signal failure to restore the backup.
		return FALSE;
	}


	// Restore the previous document data from the backup data.
	m_LoadedFiles.AddTail(&m_backupLoadedFiles);
	m_LoadedPaths.AddTail(&m_backupLoadedPaths);
	m_LoadedExts.AddTail(&m_backupLoadedExts);

	// Verify that the data was restored.
	if( (!m_LoadedFiles.IsEmpty()) &&
		(!m_LoadedPaths.IsEmpty()) &&
		(!m_LoadedExts.IsEmpty()) )
	{
		// If each list was restored successfully, signal success.
		return TRUE;
	}
	else
	{
		if ( (m_backupLoadedFiles.GetCount() == 0) &&
			 (m_backupLoadedPaths.GetCount() == 0) &&
			 (m_backupLoadedExts.GetCount() == 0) )
		{
			// If there are no elements in the original document data, return success even though the backups are empty.
			return TRUE;
		}
		else
		{
			// If any list failed to restore, signal failure.
			return FALSE;
		}
	}
}

/* ClearBackupData - Remove All Data from the Backup Class Members */
BOOL CUniCheckDoc::ClearBackupData()
{
	// Remove file attributes from each document link list.
	m_backupLoadedFiles.RemoveAll();
	m_backupLoadedPaths.RemoveAll();
	m_backupLoadedExts.RemoveAll();

	// Verify the operation completed successfully.
	if ( (m_backupLoadedFiles.IsEmpty()) &&
		 (m_backupLoadedPaths.IsEmpty()) &&
		 (m_backupLoadedExts.IsEmpty()) )
	{
		// If all document data is cleared, signal success.
		return TRUE;
	}
	else
	{
		// If any data failed to clear, signal failure.
		return FALSE;
	}
}

/* CreateSet - Build a Psuedo Visual Studio 6 Project File for Use by Scan Utilities */
UINT CUniCheckDoc::CreateSet()
{
	/* Local Variable Declarations and Initialization */

	int nElements = 0;					// Number of Elements in the Link Lists of the Document Data

	UINT uReturnValue = UNICHECK_RETURN_FAILED;	// Return Value Indicating the Status of the Method Run

	BOOL bDataExists = FALSE;			// Flag Specifying Whether Data Exists in the Document Link Lists

	CString szFilePath;					// Full Path to the File to Create
	CString szFilesetFile;				// Name of the File to Create
	CString szFilesetFilePath;			// Path to the Location Where the File should be Created
	CString szSetFileLine;				// Line of Text to Write to the patternset.txt File

	FILE* fPatternMatchSetFile = NULL;	// Pointer the Physical File to Create

	DSLISTPOSITION	dsFilePosition;		// Current Position in the Link Lists of the Document Data
	DSLOADEDFILE	dsFile;				// File Attributes of the Current Source File in the Document Data

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application


	// Memset data structures.
	ZeroMemory(&dsFilePosition, sizeof(DSLISTPOSITION));

	
	// Grab a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Get the fileset file, and fileset file path.
	if ( pTheApp != NULL )
	{
		szFilesetFile = pTheApp->GetFilesetFile();
	}
	if ( pTheApp != NULL )
	{
		szFilesetFilePath = pTheApp->GetFilesetFilePath();
	}


	// Only attempt to create the file set file if the fileset name, and fileset path contain values.
	if ( (!szFilesetFile.IsEmpty()) &&
		 (!szFilesetFilePath.IsEmpty()) )
	{
		// Build the full path to the fileset file.
		szFilePath = szFilesetFilePath + szFilesetFile;

		// Try to delete the file to remove any previous fileset file.
		if(!DeleteSet())
		{
			// Set the return value to indicate that the inital fileset file is being created.
			uReturnValue = UNICHECK_RETURN_INITIAL;
		}
		else
		{
			// Set the return value to indicate that the fileset file is being refreshed.
			uReturnValue = UNICHECK_RETURN_REFRESH;
		}	

		// Get references to the first elements in the path and extensions link lists of the document data.
		bDataExists = GetFirstFileInDocument(&dsFilePosition);

		// If references were retreived for the first element in the document data, add the full path to each file 
		// in the document data to the fileset file.
		if ( bDataExists )
		{
			// Get the number of elements in the link lists.
			nElements = GetNumberOfLoadedFilesInDocument();

			// Verify that the document data is not corrupt.
			if (nElements != -1)
			{
				// Create the fileset file.
				if ( (fPatternMatchSetFile = fopen(szFilePath.GetBufferSetLength(szFilePath.GetLength()), UNICHECK_FILESET_CREATEMODE)) == NULL )
				{
					// Exit the method and indicate that the fileset file failed to create.
					return UNICHECK_RETURN_FAILED;
				}
				else
				{
					/* If the file was created, add the source files in the document to the file. */

					// If the document data is not corrupt, loop through each element of the document data.
					for ( int i = 0; i < nElements; i++ )
					{
						// Get the full file path and extension from the document data.
						dsFile = GetSpecificFileFromDocument(&dsFilePosition);

						// Verify that a valid file was retrieved.
						if (dsFile.szOriginalPath != UNICHECK_SEARCH_NOFILEFOUND)
						{
							// Build the psuedo-VS6 file reference string.
							szSetFileLine = UNICHECK_FILESET_LINEHEADER + dsFile.szOriginalPath + NEWLINE;

							// Write the file reference string to the fileset file.
							fwrite(szSetFileLine.GetBuffer(szSetFileLine.GetLength()), 
															  sizeof(TCHAR), 
															  szSetFileLine.GetLength(), 
															  fPatternMatchSetFile);
						}
					}

					// Close the fileset file.
					if (fPatternMatchSetFile != NULL)
					{
						fclose(fPatternMatchSetFile);
					}
				}
			}
			else
			{
				// If the document data is corrupt, exit the method and signal that the document data is corrupt.
				return UNICHECK_RETURN_FILECORRUPT;
			}
		}
	}

	// Return the result of the file creation process.
	return uReturnValue;
}

/* DeleteSet - Delete the Psuedo Visual Studio 6 Project File Used as Input by Scans */
BOOL CUniCheckDoc::DeleteSet()
{
	/* Local Variable Declarations and Initialization */

	CString szFilePath;				// Full Path to the File to Create
	CString szFilesetFile;			// Name of the File to Create
	CString szFilesetFilePath;		// Path to the Location Where the File should be Created

	CUniCheckApp* pTheApp = NULL;	// Pointer to the Application

	
	// Grab a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Get the fileset file, and fileset file path.
	if ( pTheApp != NULL )
	{
		szFilesetFile = pTheApp->GetFilesetFile();
	}
	if ( pTheApp != NULL )
	{
		szFilesetFilePath = pTheApp->GetFilesetFilePath();
	}


	// Only attempt to delete the file set file if the fileset name, and fileset path contain values.
	if ( (!szFilesetFile.IsEmpty()) &&
		 (!szFilesetFilePath.IsEmpty()) )
	{
		// Build the full path to the fileset file.
		szFilePath = szFilesetFilePath + szFilesetFile;

		// Try to delete the file to remove any previous fileset file.
		if(!DeleteFile(szFilePath))
		{
			// Return that the file failed to delete.
			return FALSE;
		}
		else
		{
			// Return that the file deleted successfully.
			return TRUE;
		}	
	}
	else
	{
		// Return that the file failed to delete if the program path is not set.
		return FALSE;
	}
}

/* DeleteSelectedFile - Delete a Specific File from the Document and Temporary Directory */
UINT CUniCheckDoc::DeleteSelectedFile(CString szNodeName)
{
	/* Local Variable Declarations and Initialization */

	BOOL bSuccess = FALSE;		// Flag Specifying Whether the File was Completely Removed from All Locations


	// Only attempt to delete the selected file if the node name has a value
	if ( !szNodeName.IsEmpty() )
	{
		// Save copies of the un-modified document data for restoration if an error occurs.
		if(!BackupDocumentData())
		{
			// If a backup of document data couldn't be created, return and signal failure 
			// to add the file to the document.
			return UNICHECK_RETURN_FAILED;
		}

		// Delete the file from the temporary directory.
		if(!DeleteSpecificFileFromTempDirectory(szNodeName))
		{
			// If the file was not removed from the temporary directory, signal that the file failed to be removed.
			bSuccess = FALSE;
		}
		else
		{
			// If the DeleteFile API succeeded, remove the file from the document data.
			if(!RemoveSpecificFileFromDocument(szNodeName))
			{
				// If the file was not removed from the document data, signal that the file failed to be removed.
				bSuccess = FALSE;
			}

			// Signal that the file was removed successfully.
			bSuccess = TRUE;
		}

		if (bSuccess)
		{
			/* The file was removed from each location successfully. */

			// Return that the file was removed correctly.
			return UNICHECK_RETURN_SUCCESS;
		}
		else
		{
			/* One or more of the locations failed to clear. */

			// Restore the original state of the document data.
			if(!RestoreDocumentDataFromBackup())
			{
				// If the original document data cannot be restored, return that the document is corrupt.
				return UNICHECK_RETURN_FILECORRUPT;
			}

			// Return that the file could not be added to the document data.
			return UNICHECK_RETURN_FAILED;
		}
	}
	else
	{
		// If no node name is passed, return that the file could not be added to the document data.
		return UNICHECK_RETURN_FAILED;
	}
}

/* ClearEvalFolders - Delete the Source and Header Files from the Temporary Directories */
void CUniCheckDoc::ClearEvalFolders()
{
	/* Local Variable Declarations and Initialization */

	CString szSourcePath;	// Directory Path of the Source File Temporary Directory
	CString szHeaderPath;	// Directory Path of the Header File Temporary Directory
	CString szFilePath;		// Full Path of the File Currently Being Processed

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance

	
	// Get a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();

	// Retrieve the temporary source and header paths.
	if ( pTheApp != NULL )
	{
		szSourcePath = pTheApp->GetSourcePath();
	}
	if ( pTheApp != NULL )
	{
		szHeaderPath = pTheApp->GetHeaderPath();
	}


	/* Delete All Files from the Source Temporary Directory */

	// Only attempt to clear the source temporary directory if the source path has a value.
	if ( !szSourcePath.IsEmpty() )
	{
		if ( pTheApp != NULL )
		{
			pTheApp->DeleteAllFilesFromDirectory(szSourcePath);
		}
	}

	
	/* Delete All Files from the Header Temporary Directory */

	// Only attempt to clear the header temporary directory if the header path has a value.
	if ( !szHeaderPath.IsEmpty() )
	{
		if ( pTheApp != NULL )
		{
			pTheApp->DeleteAllFilesFromDirectory(szHeaderPath);
		}
	}


	// Delete the psuedo-Visual Studio 6 project file.
	DeleteSet();
}

/* ProcessVSProjectFile - Parse and Extract the Data from a Visual Studio Project File */
BOOL CUniCheckDoc::ProcessVSProjectFile(CString szFilePath, CString szPathIdentifier, CString szPathTerminator)
{
	/* Local Variable Declarations and Initialization */

	TCHAR lpszProjectFileLine[UNICHECK_DATAFILE_READBUFSIZE] = {NULL_CHAR};	// Line of Text Extracted from the Project File

	CString szFileReference;			// Full Path to the File Reference Extracted from the Project File

	FILE* fProjectFile = NULL;			// Pointer the Physical File to Read

	DSLOADEDFILE dsFile;				// File Attributes of the Source File Extracted from the Project File

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance

	
	// Grab a reference to the application instance.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Open the project file.
	if ( (fProjectFile = fopen(szFilePath.GetBufferSetLength(szFilePath.GetLength()), UNICHECK_DATAFILE_READMODE)) == NULL )
	{
		// Exit the method and indicate that the project file could not be read.
		return FALSE;
	}
	else
	{
		/* If the file was opened, extract the file references from project file. */

		// Cycle through each line in the project file until the end of the file is reached.
		while ( fgets(lpszProjectFileLine, UNICHECK_DATAFILE_READBUFSIZE, fProjectFile) != NULL )
		{
			if ( pTheApp != NULL )
			{
				// If a file path was found, add it to the document.
				if ( pTheApp->ExtractPathFromString(lpszProjectFileLine, szPathIdentifier, szPathTerminator, &szFileReference) )
				{
					// Load all three file attributes to a file structure.
					dsFile.szOriginalPath = szFileReference;
					dsFile.szFileName = pTheApp->ExtractFileNameFromPath(szFileReference);
					dsFile.szFileExtension = pTheApp->DetermineFileExtension(dsFile.szFileName);

					// Add the file reference to the document.
					if ( AddFileToDocument(dsFile) != UNICHECK_RETURN_SUCCESS )
					{
						// If the file could not be added to the document, return and signal failure.
						if (fProjectFile != NULL)
						{
							fclose(fProjectFile);
						}
						return FALSE;
					}
				}
			}
		}


		// Close the project file.
		if (fProjectFile != NULL)
		{
			fclose(fProjectFile);
		}


		// Return that the project file was fully processed.
		return TRUE;
	}
}
