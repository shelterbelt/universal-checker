// UniCheckView.h : Interface of the CUniCheckView Class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNICHECKVIEW_H__A9B1F03E_3335_4F2B_9D8D_B5D6E1476A14__INCLUDED_)
#define AFX_UNICHECKVIEW_H__A9B1F03E_3335_4F2B_9D8D_B5D6E1476A14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CUniCheckView:
//

class CUniCheckView : public CFormView
{
protected: // Create from serialization only.
	CUniCheckView();
	DECLARE_DYNCREATE(CUniCheckView)

public:
	virtual ~CUniCheckView();

public:
	//{{AFX_DATA(CUniCheckView)
	enum { IDD = IDD_UNICHECK_FORM };
	CCheckListBox	m_wndScansListBox;
	CButton			m_wndResultsButton;
	CButton			m_wndAddFilesButton;
	CButton			m_wndValidateButton;
	CListBox		m_wndMonitorWindow;
	CTreeCtrl		m_wndLoadedFilesTree;
	//}}AFX_DATA

// Attributes
public:
	CUniCheckDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUniCheckView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
// Construction Methods
	void InitializeInterface();
	void InitalizeFileTree();
	void InitializeScanListBox();
	void LoadDocDataToInterface();

// Menu/Control State Handlers
	void ManageBaseMenusStates(CCmdUI* pCmdUI, CButton* pButton);
	void ManageSaveMenusStates(CCmdUI* pCmdUI);
	void ManageEditMenuStates(CCmdUI* pCmdUI);
	void ManageValidationMenuStates(CCmdUI* pCmdUI, CButton* pButton);

// Utility Methods
	BOOL AddFileToLoadedFilesTreeControl(DSLOADEDFILE dsFile);
	BOOL CopyFileToAnalysisLocation(DSLOADEDFILE dsFile);
	BOOL RemoveSelectedFile(CString szNodeName);
	BOOL IsScanSelected();
	BOOL IsScanSelected(UINT uType);
	void EnableScan(BOOL bEnable);
	void EnableScan(UINT uType, BOOL bEnable);
	int GetScanIndex(UINT uType);
	void DeleteResultsFile(CString szFileName, CString szScanType);
	UINT CreateSetFileForScanInput();
	BOOL CreateUnicodeControlScript();
	void RunScan(CString szScanExecutable, CString szScanArguments, CString szScanType);
	void ViewResults(CString szExecutablePath, CString szResultsFileName, CString szResultsType);

// Diagnostic Methods
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated Message Map Functions
protected:
	//{{AFX_MSG(CUniCheckView)
	afx_msg void OnValidatebtn();
	afx_msg void OnResultsbtn();
	afx_msg void OnToolsClearresultsfile();
	afx_msg void OnToolsClearnotificationpane();
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditReloadfile();
	afx_msg void OnEditAddfile();
	afx_msg void OnUpdateEditReloadfile(CCmdUI* pCmdUI);
	afx_msg void OnValidateValidate();
	afx_msg void OnValidateViewresults();
	afx_msg void OnAddfilesbtn();
	afx_msg void OnToolsOptions();
	afx_msg void OnUpdateEditAddfile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateValidateValidate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateValidateViewresults(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsClearresultsfile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnToolsScandef();
	afx_msg void OnDblclkScanlist();
	afx_msg void OnUpdateToolsScandef(CCmdUI* pCmdUI);
	afx_msg void OnToolsFiletypes();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // Debug Version in UniCheckView.cpp
inline CUniCheckDoc* CUniCheckView::GetDocument()
   { return (CUniCheckDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNICHECKVIEW_H__A9B1F03E_3335_4F2B_9D8D_B5D6E1476A14__INCLUDED_)
