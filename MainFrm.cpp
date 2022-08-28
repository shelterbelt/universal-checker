// MainFrm.cpp : Implementation of the CMainFrame Class
//

#include "stdafx.h"
#include "UniCheck.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

// Status Line Indicator Values
static UINT indicators[] =
{
	ID_SEPARATOR,           // Status Line Indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame Construction/Destruction

CMainFrame::CMainFrame()
{	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	/* Create the application frame window. */

	// Create the main window.
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;      // Exit with error if failed to create.

	// Create the application toolbar.
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // Exit with error if failed to create.
	}

	// Create the application status bar.
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // Exit with error if failed to create.
	}

	// Make the toolbar dockable.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);		// Allow toolbar to dock to any side of the main frame.
	EnableDocking(CBRS_ALIGN_ANY);					// Enable all sides of the main frame to accept a toolbar.
	DockControlBar(&m_wndToolBar);					// Initially dock the toolbar.

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// Modify the application window's style to prevent maximizing.
	cs.style = FWS_ADDTOTITLE|WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX|WS_BORDER;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame Diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG
