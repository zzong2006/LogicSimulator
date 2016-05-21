// CircuitView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "CircuitView.h"
#include "LogicSimulatorDoc.h"

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
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
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
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	CClientDC dc(this);
	Graphics graphics(dc);
	Pen P(Color(0, 0, 0),2);
	Point temp[4];

	int dec_x, dec_y;
	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;
	//and east
	

	if (pDoc->isSelected && pDoc->selectedType == _T("AND Gate")) {
		for (int i = 0; i < andPts.GetSize(); i++)
			temp[i] = andPts[i];

		andPts[0] = Point(dec_x - 2 * UNIT, dec_y - 2 * UNIT);
		andPts[1] = Point(dec_x - 5 * UNIT, dec_y - 2 * UNIT);
		andPts[2] = Point(dec_x - 5 * UNIT, dec_y + 3 * UNIT);
		andPts[3] = Point(dec_x - 2 * UNIT, dec_y + 3 * UNIT);
		graphics.DrawArc(&P, dec_x - 5 * UNIT, dec_y - 2 * UNIT, 5 * UNIT, 5 * UNIT, -80, 173);
		graphics.DrawLines(&P, temp, 4);

		prev_x = prev_y = INT_MAX;

		for (int i = 0; i < andPts.GetSize(); i++)
			andPts[i] = Point(0, 0);
	}
	pDoc->isSelected = false;
	
	CView::OnLButtonDown(nFlags, point);
}


BOOL CCircuitView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *) GetDocument();
	
	if (nHitTest==HTCLIENT)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);
		CRgn rgn;
		if (pDoc->isSelected)
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		else
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CCircuitView::OnMouseMove(UINT nFlags, CPoint point)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	CClientDC dc(this);
	Graphics graphics(dc);

	Point temp[4];
	Pen P(Color(190,190,190), 2);
	Pen DP(Color(255, 255, 255), 2);
	
	
	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;
	if (prev_x == INT_MAX)
	{
		prev_x = dec_x;
		prev_y = dec_y;
	}
	//and east

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pDoc->isSelected && pDoc->selectedType == _T("AND Gate")) {

		for (int i = 0; i < andPts.GetSize(); i++)
			temp[i] = andPts[i];
		

		graphics.DrawArc(&DP, prev_x - 5 * UNIT, prev_y - 2 * UNIT, 5 * UNIT, 5 * UNIT, -80, 173);

		graphics.DrawLines(&DP, temp, 4);
		
		andPts[0] = Point(dec_x - 2 * UNIT, dec_y - 2 * UNIT);
		andPts[1] = Point(dec_x - 5 * UNIT, dec_y - 2 * UNIT);
		andPts[2] = Point(dec_x - 5 * UNIT, dec_y + 3 * UNIT);
		andPts[3] = Point(dec_x - 2 * UNIT, dec_y + 3 * UNIT);
		graphics.DrawArc(&P, dec_x - 5 * UNIT, dec_y - 2 * UNIT, 5 * UNIT, 5 * UNIT, -80, 173);
		
		graphics.DrawLines(&P, temp, 4);
		prev_x = dec_x;
		prev_y = dec_y;
	}

	CView::OnMouseMove(nFlags, point);
}


void CCircuitView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	pDoc->isSelected = false;
	andPts.SetSize(4);
	prev_x = INT_MAX;
	prev_y = INT_MAX;
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
