#if !defined(AFX_ADDFILESDIALOG_H__B0CC678D_F18C_4399_9098_BF8E06040D18__INCLUDED_)
#define AFX_ADDFILESDIALOG_H__B0CC678D_F18C_4399_9098_BF8E06040D18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddFilesDialog.h : Header File
//


/////////////////////////////////////////////////////////////////////////////
// CAddFilesDialog Dialog

class CAddFilesDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CAddFilesDialog)

public:
	CAddFilesDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddFilesDialog)
	public:
	virtual BOOL OnFileNameOK();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAddFilesDialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDFILESDIALOG_H__B0CC678D_F18C_4399_9098_BF8E06040D18__INCLUDED_)
