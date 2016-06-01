// MenuView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "MenuView.h"
#include "LogicSimulatorDoc.h"
#include "andGate.h"
#include "Resource.h"
// CMenuView

IMPLEMENT_DYNCREATE(CMenuView, CTreeView)

CMenuView::CMenuView()
{

}
 
CMenuView::~CMenuView()
{

}

BEGIN_MESSAGE_MAP(CMenuView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CMenuView::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_CLICK, &CMenuView::OnNMClick)
	ON_COMMAND(ID_CLICK_MODE, &CMenuView::OnClickMode)
	ON_UPDATE_COMMAND_UI(ID_CLICK_MODE, &CMenuView::OnUpdateClickMode)
	ON_COMMAND(ID_SELECT_MODE, &CMenuView::OnSelectMode)
	ON_UPDATE_COMMAND_UI(ID_SELECT_MODE, &CMenuView::OnUpdateSelectMode)
END_MESSAGE_MAP()


// CMenuView 진단입니다.

#ifdef _DEBUG
void CMenuView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CMenuView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMenuView 메시지 처리기입니다.


void CMenuView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	CImageList il;
	il.Create(IDB_BITMAP1, 16, 1, RGB(255, 255, 255));

	treeCtrl.ModifyStyle(NULL, TVS_HASLINES | TVS_LINESATROOT | TVS_TRACKSELECT | TVS_HASBUTTONS);
	treeCtrl.SetImageList(&il, TVSIL_NORMAL);
	il.Detach();

	HTREEITEM hGate = treeCtrl.InsertItem(_T("Gates"), 0, 0, TVI_ROOT, TVI_LAST);

	treeCtrl.InsertItem(_T("AND Gate"), 1, 1, hGate, TVI_LAST);
	treeCtrl.InsertItem(_T("OR Gate"), 2, 2, hGate, TVI_LAST);
	treeCtrl.InsertItem(_T("NOT Gate"), 3, 3, hGate, TVI_LAST);
	treeCtrl.InsertItem(_T("NAND Gate"), 4, 4, hGate, TVI_LAST);
	treeCtrl.InsertItem(_T("NOR Gate"), 5, 5, hGate, TVI_LAST);
	treeCtrl.InsertItem(_T("XOR Gate"), 6,6, hGate, TVI_LAST);

	HTREEITEM hWiring = treeCtrl.InsertItem(_T("Wiring"), 0, 0, TVI_ROOT, TVI_LAST);

	treeCtrl.InsertItem(_T("Pin"), 7,7, hWiring, TVI_LAST);
	treeCtrl.InsertItem(_T("Out"), 8,8, hWiring, TVI_LAST);
	treeCtrl.InsertItem(_T("Clock"), 9, 9, hWiring, TVI_LAST);
	treeCtrl.InsertItem(_T("7-Seg"), 10, 10, hWiring, TVI_LAST);

	HTREEITEM hFF = treeCtrl.InsertItem(_T("Flip-Flop"), 0, 0, TVI_ROOT, TVI_LAST);

	treeCtrl.InsertItem(_T("D Flip-Flop"), 11, 11, hFF, TVI_LAST);
	treeCtrl.InsertItem(_T("JK Flip-Flop"), 12, 12, hFF, TVI_LAST);
	treeCtrl.InsertItem(_T("T Flip-Flop"), 13, 13, hFF, TVI_LAST);
}


void CMenuView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	
	*pResult = 0;
}


void CMenuView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	CTreeCtrl& treeCtrl = GetTreeCtrl();

	TV_HITTESTINFO p;
	//화면 상에서 마우스의 위치를 얻는다.
	GetCursorPos(&p.pt);

	//얻은 마우스 좌표를 트리 컨트롤 기준의 좌표로 변경한다.
	::ScreenToClient(treeCtrl.m_hWnd, &p.pt);

	//현재 마우스 좌표가 위치한 항목 정보를 얻는다..
	HTREEITEM current_item = treeCtrl.HitTest(&p);

	if (current_item != NULL) {
		treeCtrl.Select(current_item, TVGN_CARET);
		pDoc->isSelected = TRUE;
	}

	//항목 정보의 이름을 얻는다.
	CString typeTemp = treeCtrl.GetItemText(current_item);


	if (typeTemp == "AND Gate")
	{
		pDoc->objectName = AND_GATE;
		pDoc->objectType = GATE_TYPE;
	}
	else if (typeTemp == "OR Gate")
	{
		pDoc->objectName = OR_GATE;
		pDoc->objectType = GATE_TYPE;
	}
	else if (typeTemp == "Pin")
	{
		pDoc->objectName = PIN;
		pDoc->objectType = WIRING_TYPE;
	}
	else if (typeTemp == "Clock")
	{
		pDoc->objectName = CLOCK;
		pDoc->objectType = WIRING_TYPE;
	}
	else {
		pDoc->isSelected = FALSE;
	}

	//폴더를 선택했을 경우에는 선택한게 아니므로 FALSE
	if (typeTemp == "Gates" || typeTemp == "Wiring")
	{
		pDoc->isSelected = FALSE;
	}
	
	if (pDoc->temp != NULL) {
		delete pDoc->temp;
		pDoc->temp = NULL;
	}


	*pResult = 0;
}


void CMenuView::OnClickMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = TRUE;
	pDoc->selectMode = FALSE;
}


void CMenuView::OnUpdateClickMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->clickMode == TRUE);
}


void CMenuView::OnSelectMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = FALSE;
	pDoc->selectMode = TRUE;
}


void CMenuView::OnUpdateSelectMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->selectMode == TRUE);
}
