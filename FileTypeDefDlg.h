#if !defined(AFX_FILETYPEDEFDLG_H__60960EE4_0F74_4BA3_8C7D_5819A8B479F3__INCLUDED_)
#define AFX_FILETYPEDEFDLG_H__60960EE4_0F74_4BA3_8C7D_5819A8B479F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileTypeDefDlg.h : Header File
//
/*
 * Copyright (c) 2004 Mark Johnson
 */

/////////////////////////////////////////////////////////////////////////////
// CFileTypeDefDlg Dialog

class CFileTypeDefDlg : public CDialog
{
public:
// Dialog Data
	//{{AFX_DATA(CFileTypeDefDlg)
	enum { IDD = IDD_UNICHECK_FILETYPE };
	CString	m_szFileTypeControlText;
	CString	m_szFileTypeDescriptionControlText;
	int		m_nGroupingRadioButton;
	//}}AFX_DATA

// Construction
public:
	CFileTypeDefDlg(CWnd* pParent = NULL);   // Standard Constructor

// Member Access Methods
	CString GetFileTypeDescription();
	void SetFileTypeDescription(CString szDescription);
	CString GetFileType();
	void SetFileType(CString szType);
	CString GetGrouping();
	void SetGrouping(CString szGroup);

// Overrides
	// ClassWizard Generated Virtual Function Overrides
	//{{AFX_VIRTUAL(CFileTypeDefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated Message Map Functions
	//{{AFX_MSG(CFileTypeDefDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETYPEDEFDLG_H__60960EE4_0F74_4BA3_8C7D_5819A8B479F3__INCLUDED_)
