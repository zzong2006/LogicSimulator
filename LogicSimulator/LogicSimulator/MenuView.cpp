// MenuView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "MenuView.h"


// CMenuView

IMPLEMENT_DYNCREATE(CMenuView, CTreeView)

CMenuView::CMenuView()
{

}

CMenuView::~CMenuView()
{
}

BEGIN_MESSAGE_MAP(CMenuView, CTreeView)
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
	treeCtrl.ModifyStyle(NULL, TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);

	TVINSERTSTRUCT tvi;
	tvi.hParent = TVI_ROOT;
	tvi.hInsertAfter = TVI_LAST;
	tvi.item.mask = TVIF_TEXT;
	tvi.item.pszText = _T("Gates");
	HTREEITEM hItem = treeCtrl.InsertItem(&tvi);

	tvi.hParent = hItem;
	tvi.hInsertAfter = TVI_LAST;
	tvi.item.mask = TVIF_TEXT;
	tvi.item.pszText = _T("AND Gate");
	treeCtrl.InsertItem(&tvi);

	tvi.hParent = hItem;
	tvi.hInsertAfter = TVI_LAST;
	tvi.item.mask = TVIF_TEXT;
	tvi.item.pszText = _T("OR Gate");
	treeCtrl.InsertItem(&tvi);

	tvi.hParent = hItem;
	tvi.hInsertAfter = TVI_LAST;
	tvi.item.mask = TVIF_TEXT;
	tvi.item.pszText = _T("NAND Gate");
	treeCtrl.InsertItem(&tvi);

	tvi.hParent = hItem;
	tvi.hInsertAfter = TVI_LAST;
	tvi.item.mask = TVIF_TEXT;
	tvi.item.pszText = _T("XOR Gate");
	treeCtrl.InsertItem(&tvi);
}
