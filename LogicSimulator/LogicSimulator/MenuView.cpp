// MenuView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "MenuView.h"


// CMenuView

IMPLEMENT_DYNCREATE(CMenuView, CFormView)
 
CMenuView::CMenuView()
	: CFormView(IDD_MENUVIEW)
{

}

CMenuView::~CMenuView()
{
}

void CMenuView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMenuView, CFormView)
END_MESSAGE_MAP()


// CMenuView 진단입니다.

#ifdef _DEBUG
void CMenuView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMenuView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMenuView 메시지 처리기입니다.
