#if !defined(AFX_SCANLISTDLG_H__C03E93DF_8309_4FF6_9EEF_BAA4F9BDC371__INCLUDED_)
#define AFX_SCANLISTDLG_H__C03E93DF_8309_4FF6_9EEF_BAA4F9BDC371__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanListDlg.h : Header File
//

#include "ScanDefDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CScanListDlg Dialog

class CScanListDlg : public CDialog
{
public:
// Dialog Data
	//{{AFX_DATA(CScanListDlg)
	enum { IDD = IDD_UNICHECK_SCANLIST };
	CButton	m_wndPreviewButton;
	CListBox	m_wndScansListBox;
	CButton	m_wndEditButton;
	CButton	m_wndDeleteButton;
	//}}AFX_DATA

// Construction
public:
	CScanListDlg(CWnd* pParent = NULL);   // Standard Constructor

// Member Access Methods

// Generic Form Control Handlers
	void ManageExistingItemControlStates();

// Utility Methods
	void ProcessNewScanDefData(CScanDefDlg* wndScanDefDlg);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CScanListDlg)
	afx_msg void OnUnicheckScanlistAddbtn();
	afx_msg void OnUnicheckScanlistEditbtn();
	afx_msg void OnUnicheckScanlistDeletebtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkUnicheckScanslistbox();
	afx_msg void OnSelchangeUnicheckScanslistbox();
	afx_msg void OnScanPreview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANLISTDLG_H__C03E93DF_8309_4FF6_9EEF_BAA4F9BDC371__INCLUDED_)
