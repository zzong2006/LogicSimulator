// PropertyView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "PropertyView.h"


// CPropertyView

IMPLEMENT_DYNCREATE(CPropertyView, CFormView)

CPropertyView::CPropertyView()
	: CFormView(IDD_PROPERTYVIEW)
{

}

CPropertyView::~CPropertyView()
{
}

void CPropertyView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPropertyView, CFormView)
END_MESSAGE_MAP()


// CPropertyView 진단입니다.

#ifdef _DEBUG
void CPropertyView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPropertyView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPropertyView 메시지 처리기입니다.


void CPropertyView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//스크롤바 무효화 
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	CRect rectDummy, rc;

	rectDummy.SetRectEmpty();

	m_pGridInfo.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2);
	
	//PropertyGrid 기본 셋팅
	CPropertyView::GetClientRect(&rc);
	CPropertyView::MapWindowPoints(this, &rc);

	m_pGridInfo.MoveWindow(&rc);

	m_pGridInfo.EnableHeaderCtrl(FALSE);
	m_pGridInfo.SetVSDotNetLook();
	m_pGridInfo.EnableDescriptionArea();

	//정보 추가
	CMFCPropertyGridProperty* pGroupInfo = new CMFCPropertyGridProperty(_T("Information"));
	
	m_pGridInfo.AddProperty(pGroupInfo);
	m_pGridInfo.UpdateData(FALSE);
}

void CPropertyView::InitializePropGrid(LogicObject *tempLO)
{
	m_pGridInfo.RemoveAll();
	m_pGridInfo.EnableHeaderCtrl(FALSE);
	m_pGridInfo.SetVSDotNetLook();
	m_pGridInfo.EnableDescriptionArea(FALSE);

	CMFCPropertyGridProperty* pGroupInfo = new CMFCPropertyGridProperty(_T("Information"));
	CMFCPropertyGridProperty* Label = new CMFCPropertyGridProperty(_T("Facing"), tempLO->facing,_T("DWdaw"), 1);

	pGroupInfo->AddSubItem(new CMFCPropertyGridProperty(_T("Label"), tempLO->label, 0));

	Label->AddOption(_T("East"));
	Label->AddOption(_T("West"));
	Label->AddOption(_T("North"));
	Label->AddOption(_T("South"));
	Label->AllowEdit(FALSE);
	pGroupInfo->AddSubItem(Label);

	m_pGridInfo.AddProperty(pGroupInfo);
	
	m_pGridInfo.UpdateData(FALSE);
}


