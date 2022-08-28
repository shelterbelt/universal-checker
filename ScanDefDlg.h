#if !defined(AFX_SCANDEFDLG_H__6A4881F5_08FF_4190_ABE2_28B6D73D93E2__INCLUDED_)
#define AFX_SCANDEFDLG_H__6A4881F5_08FF_4190_ABE2_28B6D73D93E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanDefDlg.h : Header File
//


/////////////////////////////////////////////////////////////////////////////
// CScanDefDlg Dialog

class CScanDefDlg : public CDialog
{
public:
// Dialog Data
	//{{AFX_DATA(CScanDefDlg)
	enum { IDD = IDD_UNICHECK_SCAN };
	CButton	m_wndAdjustParamUpButton;
	CButton	m_wndAdjustParamDownButton;
	CListBox	m_wndParametersListBox;
	CEdit	m_wndParameterEditControl;
	CButton	m_wndFileSetCheckBox;
	CButton	m_wndEndSpaceCheckBox;
	CButton	m_wndCommandPromptCheckBox;
	CButton	m_wndUseParameterPathCheckBox;
	CButton	m_wndTempFoldersCheckBox;
	CButton	m_wndSelectedFileCheckBox;
	CButton	m_wndBrowseExeButton;
	CEdit	m_wndResultsFileEditControl;
	CEdit	m_wndParameterPathEditControl;
	CEdit	m_wndExecutableEditControl;
	CButton	m_wndAddUpdateParameterButton;
	CButton	m_wndEditParameterButton;
	CButton	m_wndDeleteParameterButton;
	CButton	m_wndBrowseParameterPathButton;
	BOOL	m_bCommandPromptCheckBox;
	BOOL	m_bEndSpaceCheckBox;
	BOOL	m_bSelectedFileCheckBox;
	BOOL	m_bFileSetCheckBox;
	BOOL	m_bTempFoldersCheckBox;
	CString	m_szScanTitleControlText;
	CString	m_szExecutableControlText;
	CString	m_szParameterControlText;
	CString	m_szParameterPathControlText;
	CString	m_szResultsFileControlText;
	CString	m_szResultsFilePathControlText;
	BOOL	m_bUseParameterPathCheckBox;
	//}}AFX_DATA

private:
	UINT m_uScanType;
	UINT m_uParameterEditMode;
	UINT m_uParamIDSeed;
	UINT m_uCurrentParameterID;
	CList<DSPARAMETER, DSPARAMETER> m_ParameterList;

// Construction
public:
	CScanDefDlg(CWnd* pParent = NULL);   // Standard Constructor

// Member Access Methods
	UINT GetParameterMode();
	void SetParameterMode(UINT uMode);
	UINT GetScanType();
	void SetScanType(UINT uType);
	CString GetScanTitle();
	void SetScanTitle(CString szTitle);
	CString GetExecutable();
	void SetExecutable(CString szExecutable);
	CString GetResultsFile();
	void SetResultsFile(CString szFile);
	CString GetResultsFilePath();
	void SetResultsFilePath(CString szPath);
	UINT GetParameterID();
	void SetParameterID(UINT uID);
	CString GetParameterEditorText();
	void SetParameterEditorText(CString szParameterValue);
	BOOL GetUseParameterPathCheck();
	void SetUseParameterPathCheck(BOOL bCheckedState);
	CString GetParameterPathEditorText();
	void SetParameterPathEditorText(CString szPath);
	BOOL GetEndSpaceCheck();
	void SetEndSpaceCheck(BOOL bCheckedState);
	BOOL GetCommandPromptCheck();
	void SetCommandPromptCheck(BOOL bCheckedState);
	BOOL GetFileSetCheck();
	void SetFileSetCheck(BOOL bCheckedState);
	BOOL GetTempFoldersCheck();
	void SetTempFoldersCheck(BOOL bCheckedState);
	BOOL GetSelectedFileCheck();
	void SetSelectedFileCheck(BOOL bCheckedState);
	UINT GetParamIDSeed();
	void SetParamIDSeed(UINT uLastIDUsed);

// Utility Methods
	CString BrowseForPath();
	CString BrowseForFile(CString szDialogTitle, char BASED_CODE szFilter[]);
	void LoadScanFormData(UINT uScanType);
	void LoadScanStructure(DSSCAN* dsScan);
	UINT GenerateScanType();
	UINT GenerateParameterID();
	void LoadParametersForDisplay(CListBox *pParamsListBox);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanDefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CScanDefDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnScanBrowseexebtn();
	afx_msg void OnScanBrowseresbtn();
	afx_msg void OnScanBrowseparbtn();
	afx_msg void OnScanUseparampathchkbx();
	afx_msg void OnScanParmaddupdtbtn();
	afx_msg void OnScanEditbtn();
	afx_msg void OnScanDeletebtn();
	afx_msg void OnSelchangeScanParamslistbox();
	afx_msg void OnScanAdjparamup();
	afx_msg void OnScanAdjparamdown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANDEFDLG_H__6A4881F5_08FF_4190_ABE2_28B6D73D93E2__INCLUDED_)
