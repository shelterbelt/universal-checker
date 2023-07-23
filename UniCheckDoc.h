// UniCheckDoc.h : Interface of the CUniCheckDoc Class
//
/*
 * Copyright (c) 2004 Mark Johnson
 */
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNICHECKDOC_H__F821B59C_80F1_40B5_8F02_D5976C526CDA__INCLUDED_)
#define AFX_UNICHECKDOC_H__F821B59C_80F1_40B5_8F02_D5976C526CDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/* Document Data Structures */

// Loaded File Attributes Structure
typedef struct DSLOADEDFILE
{
	CString szFileName;			// Name of the File
	CString szOriginalPath;		// Location of the Original File on Disk
	CString	szFileExtension;	// Extension of the File
} *LPDSLOADEDFILE;

// Document Lists Positions Structure
typedef struct DSLISTPOSITION
{
	// These three values should not be manipulated separately.
	POSITION filePosition;		// Current Position in the File Link List of the Document Data
	POSITION pathPosition;		// Current Position in the File Path Link List of the Document Data
	POSITION extPosition;		// Current Position in the File Extension Link List of the Document Data
} *LPDSLISTPOSITION;


/////////////////////////////////////////////////////////////////////////////
// CUniCheckDoc:
//

class CUniCheckDoc : public CDocument
{
public:
	CUniCheckDoc();		// Allow creation other than serialization only for command line processing.
	DECLARE_DYNCREATE(CUniCheckDoc)
	virtual ~CUniCheckDoc();

// Attributes
private:
	// Document Data 
	CList<CString, CString&> m_LoadedFiles;		// List of File Names
	CList<CString, CString&> m_LoadedPaths;		// List of File Paths
	CList<CString, CString&> m_LoadedExts;		// List of File Extensions

	// Back-up of Document Data 
	CList<CString, CString&> m_backupLoadedFiles;		// Point-in-Time Copy of List of File Names
	CList<CString, CString&> m_backupLoadedPaths;		// Point-in-Time Copy of List of File Paths
	CList<CString, CString&> m_backupLoadedExts;		// Point-in-Time Copy of List of File Extensions

// Operations
public:

// Overrides
	// ClassWizard Generated Virtual Function Overrides
	//{{AFX_VIRTUAL(CUniCheckDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
// Data Access Methods
	UINT AddFileToDocument(DSLOADEDFILE dsFile);
	BOOL CopyFileToTempDirectory(DSLOADEDFILE dsFile);
	BOOL CopyFileToTempDirectory(CString szFileName);
	BOOL GetFirstFileInDocument(LPDSLISTPOSITION dsPosition);
	DSLOADEDFILE GetSpecificFileFromDocument(LPDSLISTPOSITION dsPosition);
	DSLOADEDFILE GetSpecificFileFromDocument(CString szFileName);
	BOOL RemoveSpecificFileFromDocument(DSLISTPOSITION dsPosition);
	BOOL RemoveSpecificFileFromDocument(CString szFileName);
	BOOL DeleteSpecificFileFromTempDirectory(CString szFileName);
	BOOL ClearDocumentData();
	int GetNumberOfLoadedFilesInDocument();

// Utility Methods
	BOOL BackupDocumentData();
	BOOL RestoreDocumentDataFromBackup();
	BOOL ClearBackupData();
	UINT CreateSet();
	BOOL DeleteSet();
	UINT DeleteSelectedFile(CString szNodeName);
	void ClearEvalFolders();
	BOOL ProcessVSProjectFile(CString szFilePath, CString szPathIdentifier, CString szPathTerminator);

// Diagnostic Methods
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated Message Map Functions
protected:
	//{{AFX_MSG(CUniCheckDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNICHECKDOC_H__F821B59C_80F1_40B5_8F02_D5976C526CDA__INCLUDED_)
