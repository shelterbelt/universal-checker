// AddFilesDialog.cpp : Implementation File
//
/*
 * Copyright (c) 2004 Mark Johnson
 */

#include "stdafx.h"
#include "UniCheck.h"
#include "AddFilesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAddFilesDialog


/////////////////////////////////////////////////////////////////////////////
// CAddFilesDialog Construction/Destruction

IMPLEMENT_DYNAMIC(CAddFilesDialog, CFileDialog)

CAddFilesDialog::CAddFilesDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CAddFilesDialog, CFileDialog)
	//{{AFX_MSG_MAP(CAddFilesDialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAddFilesDialog Message Handlers

/* OnFileNameOK - Validation Method for the OK Button */
CAddFilesDialog::OnFileNameOK()
{
	/* Local Variable Declarations and Initialization */

	int nNumFiles = 0;					// The Number of File Names in the Buffer
	int nMaxFilesAllowed = 0;			// The Max. Files User Preference Setting

	CString szMaxFilesAllowed;			// The String Version of the Max. Files Setting
	CString szFileName;					// Name of the File Selected by the User

	POSITION fileNamePosition = NULL;	// Position within the szFileNameBuffer

	CUniCheckApp* pTheApp = NULL;		// Pointer to the Application Instance


	// Get references to the application instance, document, and owner.
	pTheApp = (CUniCheckApp*) AfxGetApp();


	// Execute the parent class implementation of the method to complete standard validation behavior.
	if (CFileDialog::OnFileNameOK())
	{
		// If the standard data validation failed, return that the data is not valid.
		return TRUE;
	}
	else
	{
		/* If the standard validation passes, run the custom validation for UniversalChecker. */

		
		/* Verify that less than or equal to the maximum number of allowed files has been selected. */

		// Get the max. files user preference setting from the registry.
		if ( pTheApp != NULL )
		{
			nMaxFilesAllowed = pTheApp->GetProfileInt(UNICHECK_REG_SETTINGSKEY, UNICHECK_REG_MAXFILEKEY, UNICHECK_REG_MAXFILEDEFAULT);
		}

		// Verify that a valid value was retrieved for the max. file setting.
		if ( (nMaxFilesAllowed < UNICHECK_REG_MAXFILEFLOOR) || (nMaxFilesAllowed > UNICHECK_REG_MAXFILECEILING) )
		{
			// If a valid value was not retrieved from the registry, set the value to the default max. value setting.
			nMaxFilesAllowed = UNICHECK_REG_MAXFILEDEFAULT;
		}

		// Once the max. file value is determined, convert the value to a string for User Feedback Display messaging.
		szMaxFilesAllowed = _itot(nMaxFilesAllowed, szMaxFilesAllowed.GetBuffer(UNICHECK_REG_MAXFILECEILING), 10);

		// Get the position of the first file name in the file name buffer.
		fileNamePosition = GetStartPosition();

		// Determine the number of selected files by looping through each file name in the buffer.
		while ( fileNamePosition != NULL )
		{
			// For each file name found, increment the file name counter by one.
			nNumFiles++;

			// If the number of selected files exceeds the max. files user preference setting, exit
			// the validation method and signal that the number of selected files exceeds the number
			// allowed.
			if ( nNumFiles > nMaxFilesAllowed )
			{
				if ( pTheApp != NULL )
				{
					pTheApp->DisplayMessageToUser(NULL, UNICHECK_MESSAGE_MAXFILEEXCEED, (UNICHECK_MESSAGE_MAXFILEIS + szMaxFilesAllowed + PERIOD), TRUE);
				}
				return TRUE;
			}

			// Get the position of the next file name in the buffer.
			szFileName = GetNextPathName(fileNamePosition);

			// Verify that the file actually exists.
			if ( pTheApp != NULL )
			{
				// Verify the file really exists.
				if ( !pTheApp->VerifyThatFilePathExists(szFileName) )
				{
					// If the file does not exist, display an error to the user and signal failure.
					if ( pTheApp != NULL )
					{
						pTheApp->DisplayMessageToUser(NULL, (UNICHECK_MESSAGE_FILESP + szFileName + UNICHECK_MESSAGE_XNEXISTS), NULL_TEXT, TRUE);
					}
					return TRUE;
				}
			}
		}

		// If the entire file name buffer is scanned and the number of found files does not exceed 
		// the max. file setting, exit the method and signal that the data is valid.
		return FALSE;
	}
}
