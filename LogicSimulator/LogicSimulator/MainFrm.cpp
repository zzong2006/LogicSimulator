
// MainFrm.cpp : CMainFrame Ŭ������ ����
//
  
#include "stdafx.h"
#include "LogicSimulator.h"
#include "MenuView.h"
#include "CircuitView.h"
#include "PropertyView.h"
#include "MainFrm.h"
 // feafe
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	if (!m_wndSplitterMain.CreateStatic(this, 1, 2))
	{
		TRACE0("Fail to create splitter.\n");
		return FALSE;
	}
	m_wndSplitterMain.SetColumnInfo(0, 300, 10);

	m_wndSplitterMain.CreateView(0, 1, RUNTIME_CLASS(CCircuitView), CSize(300, 300), pContext);

	// STEP 2: ���� ���� �������� ��/�� ����
	if (!m_wndSplitterSub.CreateStatic(&m_wndSplitterMain, 2, 1, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL, m_wndSplitterMain.IdFromRowCol(0, 0)))
	{
		TRACE0("Fail to create splitter.\n");
		return FALSE;
	}

	
	m_wndSplitterSub.CreateView(0, 0, RUNTIME_CLASS(CMenuView), CSize(300, 300), pContext);
	m_wndSplitterSub.CreateView(1, 0, RUNTIME_CLASS(CPropertyView), CSize(300, 300), pContext);




	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

// CMainFrame ����

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


// CMainFrame �޽��� ó����

