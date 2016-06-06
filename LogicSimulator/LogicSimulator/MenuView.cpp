// MenuView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "MenuView.h"
#include "LogicSimulatorDoc.h"
#include "CircuitView.h"
#include "MainFrm.h"
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
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMenuView::OnNMDblclk)
END_MESSAGE_MAP()


// CMenuView �����Դϴ�.

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


// CMenuView �޽��� ó�����Դϴ�.


void CMenuView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	CImageList il;
	il.Create(IDB_BITMAP1, 16, 1, RGB(255, 255, 255));

	treeCtrl.ModifyStyle(NULL, TVS_HASLINES | TVS_LINESATROOT | TVS_TRACKSELECT | TVS_HASBUTTONS);
	treeCtrl.SetImageList(&il, TVSIL_NORMAL);
	il.Detach();

	HTREEITEM Main = treeCtrl.InsertItem(_T("Main Library Box"), 14, 14, TVI_ROOT, TVI_LAST);

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

	HTREEITEM hLibrary = treeCtrl.InsertItem(_T("Library"), 0, 0, TVI_ROOT, TVI_LAST);


	treeCtrl.InsertItem(_T("Sub Library Box"), 15, 15, hLibrary, TVI_LAST);
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
	//ȭ�� �󿡼� ���콺�� ��ġ�� ��´�.
	GetCursorPos(&p.pt);

	//���� ���콺 ��ǥ�� Ʈ�� ��Ʈ�� ������ ��ǥ�� �����Ѵ�.
	::ScreenToClient(treeCtrl.m_hWnd, &p.pt);

	//���� ���콺 ��ǥ�� ��ġ�� �׸� ������ ��´�..
	HTREEITEM current_item = treeCtrl.HitTest(&p);

	if (current_item != NULL) {
		treeCtrl.Select(current_item, TVGN_CARET);
		pDoc->isSelected = TRUE;
	}

	//�׸� ������ �̸��� ��´�.
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
	else if (typeTemp == "NOT Gate")
	{
		pDoc->objectName = NOT_GATE;
		pDoc->objectType = GATE_TYPE;
	}
	else if (typeTemp == "NAND Gate")
	{
		pDoc->objectName = NAND_GATE;
		pDoc->objectType = GATE_TYPE;
	}
	else if (typeTemp == "NOR Gate")
	{
		pDoc->objectName = NOR_GATE;
		pDoc->objectType = GATE_TYPE;
	}
	else if (typeTemp == "XOR Gate")
	{
		pDoc->objectName = XOR_GATE;
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
	else if (typeTemp == "Out")
	{
		pDoc->objectName = OUTPIN;
		pDoc->objectType = WIRING_TYPE;
	}
	else if (typeTemp == "D Flip-Flop")
	{
		pDoc->objectName = D_FF;
		pDoc->objectType = FLIPFLOP_TYPE;
	}
	else if (typeTemp == "JK Flip-Flop")
	{
		pDoc->objectName = JK_FF;
		pDoc->objectType = FLIPFLOP_TYPE;
	}
	else if (typeTemp == "T Flip-Flop")
	{
		pDoc->objectName = T_FF;
		pDoc->objectType = FLIPFLOP_TYPE;
	}
	else if (typeTemp == "7-Seg") {
		pDoc->objectName = SEG7;
		pDoc->objectType = WIRING_TYPE;
	}
	else if (typeTemp == "Main Library Box") {
		//Sub�� ����� �ְԲ� �ؾ���.
			pDoc->isSelected = FALSE;
	}
	else if (typeTemp == "Sub Library Box") {
		if (pDoc->currBox->ID != SUB_LIB) {
			pDoc->objectName = SUB_LIB;
			pDoc->objectType = LIB;
		}
		else
			pDoc->isSelected = FALSE;
	}
	else {
		pDoc->isSelected = FALSE;
	}

	//������ �������� ��쿡�� �����Ѱ� �ƴϹǷ� FALSE
	if (typeTemp == "Gates" || typeTemp == "Wiring" || typeTemp == "Flip-Flop" || typeTemp == "Library")
	{
		pDoc->isSelected = FALSE;
	}
	
	if (pDoc->currBox->temp != NULL) {
		delete pDoc->currBox->temp;
		pDoc->currBox->temp = NULL;
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



void CMenuView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	CTreeCtrl& treeCtrl = GetTreeCtrl();

	TV_HITTESTINFO p;
	//ȭ�� �󿡼� ���콺�� ��ġ�� ��´�.
	GetCursorPos(&p.pt);

	//���� ���콺 ��ǥ�� Ʈ�� ��Ʈ�� ������ ��ǥ�� �����Ѵ�.
	::ScreenToClient(treeCtrl.m_hWnd, &p.pt);

	//���� ���콺 ��ǥ�� ��ġ�� �׸� ������ ��´�..
	HTREEITEM current_item = treeCtrl.HitTest(&p);

	if (current_item != NULL) {
		treeCtrl.Select(current_item, TVGN_CARET);
	}

	//�׸� ������ �̸��� ��´�.
	CString typeTemp = treeCtrl.GetItemText(current_item);

	//���̺귯���� ����Ŭ���ϸ� �ش�	������
	//�������� �����ش�.
	if (typeTemp == "Sub Library Box")
	{
		pDoc->currBox = &(pDoc->logicBox[1]);

		CCircuitView *CVCtrl = (CCircuitView *)((CMainFrame*)AfxGetMainWnd())->m_wndSplitterMain.GetPane(0, 1);

		CVCtrl->Invalidate();
	}
	else if (typeTemp == "Main Library Box")
	{
		pDoc->currBox = &(pDoc->logicBox[0]);

		CCircuitView *CVCtrl = (CCircuitView *)((CMainFrame*)AfxGetMainWnd())->m_wndSplitterMain.GetPane(0, 1);

		CVCtrl->Invalidate();
	}

	pDoc->isSelected = FALSE;
	

	if (pDoc->currBox->temp != NULL) {
		delete pDoc->currBox->temp;
		pDoc->currBox->temp = NULL;
	}

	*pResult = 0;
}
