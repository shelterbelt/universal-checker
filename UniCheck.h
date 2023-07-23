// UniCheck.h : Main Header File for the UNICHECK Application
//
/*
 * Copyright (c) 2004 Mark Johnson
 */

#if !defined(AFX_UNICHECK_H__DB414D49_04AB_4D84_9B8F_4E2542DF8A26__INCLUDED_)
#define AFX_UNICHECK_H__DB414D49_04AB_4D84_9B8F_4E2542DF8A26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "MainFrm.h"
#include "UniCheckDoc.h"
#include "UniCheckView.h"


/* Application Data Structures */

// Scan Parameter Structure
typedef struct DSPARAMETER
{
	UINT	uParameterID;			// The Unique Identifier of the Parameter
	CString	szParameter;			// The Value Passed as a Parameter to the Scan
	BOOL	bUsesParameterPath;		// Flag Specifying that the Parameter Needs to have a Path Prepended
	CString szParameterPath;		// The Path to Prepend to the Parameter Value
	BOOL	bAppendEndSpace;		// Flag Specifying that the Parameter Should End with a Space
} *LPDSPARAMETER;

// Scan Attributes Structure
typedef struct DSSCAN
{
	UINT	uScanType;				// The Unique Identifier of the Scan
	CString szScanTitle;			// Name of the Scan
	CString szScanEngine;			// Path to the Executable Program File for the Scan
	CString szResultsFile;			// Name of the Results File That will be Created by the Scan
	CString	szResultsFilePath;		// Directory Path Where the Results File Will be Created
	BOOL	bUsesCommandPrompt;		// Flag Specifying that the Scan Needs the Command Prompt Emulator
	BOOL	bUsesTempFolders;		// Flag Specifying that the Scan Uses the Temporary Analysis Directories
	BOOL	bUsesFileSet;			// Flag Specifying that the Scan Uses the Fileset File
	BOOL	bUsesSelectedFile;		// Flag Specifying that the Scan Uses the Selected File in the File Tree
	CList<DSPARAMETER, DSPARAMETER> ParameterList;	// The Collection of Parameters Associated with this Scan
} *LPDSSCAN;

/////////////////////////////////////////////////////////////////////////////
// CUniCheckApp:
// See UniCheck.cpp for the implementation of this class.
//

class CUniCheckApp : public CWinApp
{
public:
	CUniCheckApp();

// Attributes
private:
	CString m_szProgramPath;
	CString m_szHeaderPath;
	CString m_szSourcePath;
	CString m_szFilesetFile;
	CString m_szFilesetFilePath;
	CString m_szViewerPath;
	int		m_nMaxFiles;
	UINT	m_uProcessingMode;
	UINT	m_uScanTypeSeed;

public:
// Member Access Methods
	CString GetProgramPath();
	void SetProgramPath(CString szPath);
	CString GetHeaderPath();
	void SetHeaderPath(CString szPath);
	CString GetSourcePath();
	void SetSourcePath(CString szPath);
	CString GetFilesetFile();
	void SetFilesetFile(CString szFile);
	CString GetFilesetFilePath();
	void SetFilesetFilePath(CString szPath);
	CString GetViewerPath();
	void SetViewerPath(CString szPath);
	int GetMaxFiles();
	void SetMaxFiles(int nMaxNumber);
	UINT GetProcessingMode();
	void SetProcessingMode(UINT uMode);
	UINT GetScanTypeSeed();
	void SetScanTypeSeed(UINT uLastScanType);

// Utility Methods
	void LoadApplicationSettings();
	BOOL GetInstalledScan(UINT uScanType, DSSCAN* dsScan);
	BOOL InstallScan(DSSCAN* dsScan);
	BOOL UninstallScan(UINT uScanType);
	BOOL LoadAllValidFileTypes(CListBox* pTypesListBox, CString* szFilter);
	BOOL GetValidFileType(CString szType, CString* szDescription, CString* szGrouping);
	BOOL InstallValidType(CString szType, CString szDescription, CString szGrouping);
	BOOL UninstallValidType(CString szType);
	BOOL IsFileTypeValid(CString szType);
	void ClearScanStructure(DSSCAN* dsScan);
	void ClearParameterStructure(DSPARAMETER* dsParam);
	void CopyScanStructure(DSSCAN* dsTargetStructure, DSSCAN* dsSourceStructure);
	BOOL BuildScanCommandLine(UINT uScanType, CString* szExecutablePath, CString* szScanArguments, CString szCurrentFile);
	BOOL IsAppInitialized();
	CString ValidatePath(CString szPath);
	CString GetShortFormatPath(CString szPath);
	UINT ProcessCommandLineFiles(CCommandLineInfo* cmdInfo);
	BOOL ExtractPathFromString(CString szLineToSearch, CString szPathIdentifier, CString szPathTerminator, CString* szFoundPath);
	CString ExtractFileNameFromPath(CString szPath);
	CString DetermineFileExtension(CString szFileName);
	CString ChangeFileExtension(CString szFileName, CString szNewExtension);
	BOOL DeleteSpecificResultsFile(CString szFileName);
	BOOL VerifyThatFilePathExists(CString szFilePath);
	BOOL VerifyThatFilesExistInDirectory(CString szPath);
	void DeleteAllFilesFromDirectory(CString szDirectory);
	BOOL ExecuteScanProcess(CString szScanExecutable, CString szScanArguments);
	UINT ExecuteViewResultsProcess(CString szExecutablePath, CString szResultsFileName);
	void LoadScansForDisplay(CListBox* pScansListBox);
	void DisplayMessageToUser(CListBox* pUserFeedbackDisplay, CString szMessageLine1, CString szMessageLine2, BOOL bIsError);
	void CommandLineModeProcessing(CCommandLineInfo* cmdInfo);
	void KillApp();
	UINT CreateControlFile();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUniCheckApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CUniCheckApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNICHECK_H__DB414D49_04AB_4D84_9B8F_4E2542DF8A26__INCLUDED_)
