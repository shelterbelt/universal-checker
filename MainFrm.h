// MainFrm.h : Interface of the CMainFrame Class
//
/*
 * Copyright (c) 2004 Mark Johnson
 */
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__720BCDB2_5326_403B_9A2E_617DBAE82996__INCLUDED_)
#define AFX_MAINFRM_H__720BCDB2_5326_403B_9A2E_617DBAE82996__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{	
protected: // Create from serialization only.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	virtual ~CMainFrame();


// Attributes
public:

protected:  // Control Bar Embedded Members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

// Diagnostic Methods
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated Message Map Functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__720BCDB2_5326_403B_9A2E_617DBAE82996__INCLUDED_)
