// CircuitView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "CircuitView.h"


// CCircuitView

IMPLEMENT_DYNCREATE(CCircuitView, CView)

CCircuitView::CCircuitView()
{

}

CCircuitView::~CCircuitView()
{
}

BEGIN_MESSAGE_MAP(CCircuitView, CView)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CCircuitView �׸����Դϴ�.

void CCircuitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	CRect rect;
	GetClientRect(&rect);

	for (int i = 0; i < rect.right; i+= 10)
	{
		for (int j = 0; j < rect.bottom; j+=10)
		{
			pDC->SetPixel(i,j, RGB(128, 128, 128));
		}
	}
}


// CCircuitView �����Դϴ�.

#ifdef _DEBUG
void CCircuitView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CCircuitView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCircuitView �޽��� ó�����Դϴ�.


void CCircuitView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CClientDC dc(this);
	
	dc.SetROP2(R2_COPYPEN);
	dc.Ellipse(point.x, point.y, point.x + 10, point.y + 10);
	CView::OnLButtonDown(nFlags, point);
}
