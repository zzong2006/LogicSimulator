// CircuitView.cpp : 구현 파일입니다.
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


// CCircuitView 그리기입니다.

void CCircuitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	CRect rect;
	GetClientRect(&rect);

	for (int i = 0; i < rect.right; i+= UNIT)
	{
		for (int j = 0; j < rect.bottom; j+= UNIT)
		{
			pDC->SetPixel(i,j, RGB(128, 128, 128));
		}
	}
}


// CCircuitView 진단입니다.

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


// CCircuitView 메시지 처리기입니다.


void CCircuitView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CClientDC dc(this);
	Graphics graphics(dc);
	Point andPts[4];
	Pen P(Color(0, 0, 0),2);
	int dec_x, dec_y;

	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;

	andPts[0] = Point(dec_x - 2 * UNIT, dec_y - 2 * UNIT);
	andPts[1] = Point(dec_x - 5 * UNIT, dec_y - 2 * UNIT);
	andPts[2] = Point(dec_x - 5 * UNIT, dec_y + 3 * UNIT);
	andPts[3] = Point(dec_x - 2 * UNIT, dec_y + 3 * UNIT);
	graphics.DrawArc(&P, dec_x - 5 * UNIT, dec_y - 2 * UNIT, 5 * UNIT, 5 *UNIT, -83,173);
	graphics.DrawLines(&P, andPts, 4);

	
	CView::OnLButtonDown(nFlags, point);
}
