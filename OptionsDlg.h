#if !defined(AFX_OPTIONSDLG_H__EE0AC74B_9FD2_4263_B99F_F1DD04D78FD6__INCLUDED_)
#define AFX_OPTIONSDLG_H__EE0AC74B_9FD2_4263_B99F_F1DD04D78FD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : Header File
//


/////////////////////////////////////////////////////////////////////////////
// COptionsDlg Dialog

class COptionsDlg : public CDialog
{
public:
// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_UNICHECK_OPTIONS };
	CSpinButtonCtrl	m_wndMaxFilesAdjustmentCtrl;
	CString	m_szProgramPathControlText;
	int		m_nMaxFilesToAddAtOneTime;
	CString	m_szHeaderPathControlText;
	CString	m_szSourcePathControlText;
	CString	m_szViewerPathControlText;
	CString	m_szFilesetNameControlText;
	CString	m_szFilesetFilePathControlText;
	//}}AFX_DATA

// Construction
public:
	COptionsDlg(CWnd* pParent = NULL);   // Standard Constructor

// Member Access Methods
	CString GetProgramPath();
	void	SetProgramPath(CString szPath);
	CString GetHeaderPath();
	void	SetHeaderPath(CString szPath);
	CString GetSourcePath();
	void	SetSourcePath(CString szPath);
	CString GetFilesetFileName();
	void	SetFilesetFileName(CString szFile);
	CString GetFilesetFilePath();
	void	SetFilesetFilePath(CString szPath);
	CString GetViewerPath();
	void	SetViewerPath(CString szPath);
	int		GetMaxFile();
	void	SetMaxFile(int nMaxNumber);

// Utility Methods
	CString BrowseForPath();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionsBrowsebtn();
	afx_msg void OnOptionsBrowsehdrbtn();
	afx_msg void OnOptionsBrowsesrcbtn();
	afx_msg void OnOptionsBrowsevwrbtn();
	afx_msg void OnOptionsBrowsefspbtn();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__EE0AC74B_9FD2_4263_B99F_F1DD04D78FD6__INCLUDED_)
