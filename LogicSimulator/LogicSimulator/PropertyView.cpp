// PropertyView.cpp : ���� �����Դϴ�.
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


// CPropertyView �����Դϴ�.

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


// CPropertyView �޽��� ó�����Դϴ�.


void CPropertyView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//��ũ�ѹ� ��ȿȭ 
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	CRect rectDummy, rc;

	rectDummy.SetRectEmpty();

	m_pGridInfo.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2);
	
	//PropertyGrid �⺻ ����
	CPropertyView::GetClientRect(&rc);
	CPropertyView::MapWindowPoints(this, &rc);

	m_pGridInfo.MoveWindow(&rc);

	m_pGridInfo.EnableHeaderCtrl(FALSE);
	m_pGridInfo.SetVSDotNetLook();
	m_pGridInfo.EnableDescriptionArea();
}
