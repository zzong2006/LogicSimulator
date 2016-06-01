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

	pGroupInfo->AddSubItem(new CMFCPropertyGridProperty(_T("Comanty"), _T("DWAD"), _T("회사이름을 입력하세요."), 0));

	CMFCPropertyGridProperty* pTitle = new CMFCPropertyGridProperty(_T("Title"), _T("테스트"), _T("직책을 선택하세요."), 1);

	pTitle->AddOption(_T("사무무반"));
	pTitle->AddOption(_T("양념갑"));
	pTitle->AllowEdit(FALSE);
	
	pGroupInfo->AddSubItem(pTitle);
	pGroupInfo->AddSubItem(new CMFCPropertyGridProperty(_T("Name"), _T("test"), _T("이름을 입력하세요."), 2));
	m_pGridInfo.AddProperty(pGroupInfo);

	CMFCPropertyGridProperty* pGroupPrint = new CMFCPropertyGridProperty(_T("Appearences"));

	pGroupPrint->AddSubItem(new CMFCPropertyGridProperty(_T("이름 글꼴"), CF_EFFECTS | CF_SCREENFONTS, _T("회사명,직책,이름의 글꼴을 지정합니다."), 6));

	m_pGridInfo.AddProperty(pGroupPrint);
	m_pGridInfo.UpdateData(FALSE);
}

void CPropertyView::InitializePropGrid(LogicObject *tempLO)
{

}


