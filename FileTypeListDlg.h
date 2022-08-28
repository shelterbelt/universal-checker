#if !defined(AFX_FILETYPELISTDLG_H__4914EC7D_02EB_491B_8DB0_2F2C2073370A__INCLUDED_)
#define AFX_FILETYPELISTDLG_H__4914EC7D_02EB_491B_8DB0_2F2C2073370A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileTypeListDlg.h : Header File
//

/////////////////////////////////////////////////////////////////////////////
// CFileTypeListDlg Dialog

class CFileTypeListDlg : public CDialog
{
public:
// Dialog Data
	//{{AFX_DATA(CFileTypeListDlg)
	enum { IDD = IDD_UNICHECK_FILETYPELIST };
	CListBox	m_wndFileTypesListBox;
	CButton	m_wndDeleteButton;
	CButton	m_wndEditButton;
	//}}AFX_DATA

// Construction
public:
	CFileTypeListDlg(CWnd* pParent = NULL);   // Standard Constructor

// Generic Form Control Handlers
	void ManageExistingItemControlStates();

// Overrides
	// ClassWizard Generated Virtual Function Overrides
	//{{AFX_VIRTUAL(CFileTypeListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated Message Map Functions
	//{{AFX_MSG(CFileTypeListDlg)
	afx_msg void OnDblclkFiletypelistTypelistbox();
	afx_msg void OnSelchangeFiletypelistTypelistbox();
	afx_msg void OnFiletypelistAddbutton();
	afx_msg void OnFiletypelistEditbutton();
	afx_msg void OnFiletypelistDeletebutton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETYPELISTDLG_H__4914EC7D_02EB_491B_8DB0_2F2C2073370A__INCLUDED_)
