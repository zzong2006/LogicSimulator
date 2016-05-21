// MenuView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "MenuView.h"
#include "LogicSimulatorDoc.h"

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
}


void CMenuView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	HTREEITEM hTreeItem = pNMTreeView->itemNew.hItem;
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	
	pDoc->selectedType = treeCtrl.GetItemText(hTreeItem);
	pDoc->isSelected = TRUE;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
