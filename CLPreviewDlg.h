#if !defined(AFX_CLPREVIEW_H__A2841539_ED7C_44CF_B192_B1ED6A0F3A08__INCLUDED_)
#define AFX_CLPREVIEW_H__A2841539_ED7C_44CF_B192_B1ED6A0F3A08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CLPreviewDlg.h : Header File
//
/*
 * Copyright (c) 2004 Mark Johnson
 */

/////////////////////////////////////////////////////////////////////////////
// CCLPreviewDlg Dialog

class CCLPreviewDlg : public CDialog
{
public:
// Dialog Data
	//{{AFX_DATA(CCLPreviewDlg)
	enum { IDD = IDD_UNICHECK_COMMANDLINEPREV };
	CString m_szCommandLineControlText;
	//}}AFX_DATA
	UINT m_uScanType;

// Construction
public:
	CCLPreviewDlg(CWnd* pParent = NULL);   // Standard Constructor

// Member Access Methods
	UINT	GetScanType();
	void	SetScanType(UINT uType);

// Overrides
	// ClassWizard Generated Virtual Function Overrides
	//{{AFX_VIRTUAL(CCLPreviewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated Message Map Functions
	//{{AFX_MSG(CCLPreviewDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLPREVIEW_H__A2841539_ED7C_44CF_B192_B1ED6A0F3A08__INCLUDED_)
