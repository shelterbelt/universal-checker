/////////////////////////////////////////////////////////////////////////////
// CAboutDlg Dialog Used for App About
/*
 * Copyright (c) 2004 Mark Johnson
 */

#if !defined(UNICHECK_ABOUTDLG_H)
#define UNICHECK_ABOUTDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(UNICHECK_ABOUTDLG_H)
