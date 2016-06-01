// CircuitView.cpp : 구현 파일입니다.
////

#include "stdafx.h"
#include "LogicSimulator.h"
#include "CircuitView.h"
#include "LogicSimulatorDoc.h"
#include "MainFrm.h"
#include "LineObject.h"
#include "PropertyView.h"

#include <gdiplus.h>
#include <vector>
#define CAPACITY 100

using std::vector;

// CCircuitView

IMPLEMENT_DYNCREATE(CCircuitView, CView)
inline int Rounding(int x)
{
	x += 5;
	return x - (x % UNIT);
}

CCircuitView::CCircuitView()
{
	object = OBJECT;
	
}

CCircuitView::~CCircuitView()
{

}

BEGIN_MESSAGE_MAP(CCircuitView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_CLICK_MODE, &CCircuitView::OnClickMode)
	ON_COMMAND(ID_SELECT_MODE, &CCircuitView::OnSelectMode)
	ON_UPDATE_COMMAND_UI(ID_CLICK_MODE, &CCircuitView::OnUpdateClickMode)
	ON_UPDATE_COMMAND_UI(ID_SELECT_MODE, &CCircuitView::OnUpdateSelectMode)
	ON_COMMAND(ID_ON_SIMULATE, &CCircuitView::OnOnSimulate)
	ON_UPDATE_COMMAND_UI(ID_ON_SIMULATE, &CCircuitView::OnUpdateOnSimulate)
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CCircuitView 그리기입니다.

void CCircuitView::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	// 메모리 DC 선언
	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS); // 흰색으로 초기화
		
	DrawImage(&memDC);			// 메모리 DC에 그리기

	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CCircuitView::DrawImage(CDC *pDC)
{
	CPaintDC dc(this); // device context for painting
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	Gdiplus::Graphics graphics(pDC->m_hDC);
	Gdiplus::Pen P(Gdiplus::Color(0, 0, 0), 2);
	CPoint pos;			//현재 마우스 포지션

	GetCursorPos(&pos);
	ScreenToClient(&pos);

	CRect rect;
	GetClientRect(&rect);

	pDoc->CheckCircuit();

	for (int i = 0; i < pDoc->logicInfo.size(); i++)
	{
		pDoc->logicInfo.at(i)->draw_main(&graphics);
		//선택 됬다면 표시해줌
		if (pDoc->logicInfo.at(i)->isSelected)
			pDoc->logicInfo.at(i)->showSelected(&graphics);
	}
	
	//현재 마우스가 위치한 자리가 분기 (또는 선그리기) 가능한 점이라면 동그라미 표시
	P.SetColor(Gdiplus::Color(20, 20, 250));
	if(pDoc->CanBeDivided)
		graphics.DrawArc(&P, dec_x-5, dec_y-5, 10, 10, 0, 360);
	P.SetColor(Gdiplus::Color(0, 0, 0));

	for (int i = 0; i < pDoc->lines.size(); i++)
		pDoc->lines.at(i)->draw_main(&graphics);
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

/*
	마우스 왼쪽 클릭했을때 일어나야 할 일들
	1. 메뉴에서 논리 오브젝트 선택하고 오른쪽에서 선택하면
	그려진다.
	2. 메뉴에서 논리 오브젝트 선택하지 않고 (isSelected 가 False 인 경우) field 에서 선택하면 표시가 나면서 선택되어짐.
	3. 

*/

void CCircuitView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	CClientDC dc(this);
	Gdiplus::Graphics graphics(dc);
	Gdiplus::Pen P(Gdiplus::Color(0, 0, 0), 2);

	dec_x = Rounding(point.x);
	dec_y = Rounding(point.y);

	//선을 선택했을 경우는 LINE , 기본값은 OBJECT로 함.
	object = OBJECT;

	//선 오브젝트에서 선이 분기 될 경우
	// + 메뉴에서 선택이 안됬을 경우 라인 모드 진입
	for (int i = 0; i < pDoc->lines.size(); i++)		
	{
		if (pDoc->lines.at(i)->Is_match_IineCoord(point)
			&& !(pDoc->isSelected) && !(pDoc->clickMode))
		{
			object = LINE;
			break;
		}
	}


	if (object == OBJECT)
	{
		//메뉴에서 선택하고 필드에서 지정 완료될 때
		LogicObject* temp;
		temp = NULL;

		if (pDoc->isSelected) {
			if (pDoc->objectType == GATE_TYPE)
			{
				Gate *Gtemp;
				Gtemp = NULL;

				switch (pDoc->objectName)
				{
				case AND_GATE:
					Gtemp = new andGate(dec_x, dec_y);
					Gtemp->objectName = AND_GATE;
					break;
				case OR_GATE:
					Gtemp = new orGate(dec_x, dec_y);
					Gtemp->objectName = OR_GATE;
					break;
				}

				temp = Gtemp; // delete 용으로 필요함.

				if (Gtemp != NULL) {
					Gtemp->set_Coord_From_outC(dec_x, dec_y);
					
					pDoc->lines.push_back(Gtemp->input_line[0]);
					pDoc->lines.push_back(Gtemp->input_line[1]);
					pDoc->lines.push_back(Gtemp->output_line);

					pDoc->logicInfo.push_back(Gtemp);
					pDoc->gateInfo.push_back(Gtemp);
				}
			}
			else if (pDoc->objectType == WIRING_TYPE)
			{
				Pin *Ptemp = NULL;
				Clock *Ctemp = NULL;
				temp = NULL;

				switch (pDoc->objectName)
				{
				case PIN:
					Ptemp = new Pin();
					Ptemp->objectType = WIRING_TYPE;
					Ptemp->objectName = PIN;
					Ptemp->output_line = new LineObject(dec_x, dec_y);
					pDoc->pinInfo.push_back(Ptemp);
					temp = Ptemp;
					break;
	
				case CLOCK:
					Ctemp = new Clock();
					Ctemp->objectType = WIRING_TYPE;
					Ctemp->objectName = CLOCK;
					Ctemp->output_line = new LineObject(dec_x, dec_y);
					pDoc->clockInfo.push_back(Ctemp);
					temp = Ctemp;
					break;
				}

				if (temp != NULL) {
					temp->set_outputCoord(dec_x, dec_y);
					temp->set_Coord_From_outC(dec_x, dec_y);

					pDoc->lines.push_back(temp->output_line);
					pDoc->logicInfo.push_back(temp);

				}
			}

			delete pDoc->temp;
			pDoc->temp = NULL;
			pDoc->isSelected = false;
		}

		//클릭 모드인 경우 
		//Pin 과 Clock 의 output 데이터를 바꿀뿐 gate 는 영향이 없다..
		if (pDoc->clickMode) {
			POINT temp_top, temp_bottom;

			for (int i = 0; i < pDoc->pinInfo.size(); i++)
			{
				
				temp_top = pDoc->pinInfo.at(i)->get_top();
				temp_bottom = pDoc->pinInfo.at(i)->get_bottm();

				CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

				if (PtInRect(rect, point)) 
					pDoc->pinInfo.at(i)->toggleOutput();
			}

			for (int i = 0; i < pDoc->clockInfo.size(); i++)
			{
				temp_top = pDoc->clockInfo.at(i)->get_top();
				temp_bottom = pDoc->clockInfo.at(i)->get_bottm();

				CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

				if (PtInRect(rect, point))
					pDoc->clockInfo.at(i)->toggleOutput();
			}
		}
		else {	//클릭 모드 상태에서만 객체 선택 가능
			if (!(pDoc->isSelected)) {
				pDoc->currObject.clear();

				POINT temp_top, temp_bottom;
				BOOL checkFocus = FALSE;

				for (int i = 0; i < pDoc->logicInfo.size(); i++){
					pDoc->logicInfo.at(i)->isSelected = FALSE;

					temp_top = pDoc->logicInfo.at(i)->get_top();

					temp_bottom = pDoc->logicInfo.at(i)->get_bottm();

					CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

					//마우스가 객체안에 있으면 선택함.
					if (PtInRect(rect, point))
					{
						checkFocus = TRUE;
						pDoc->isOnFocus = TRUE;
						pDoc->logicInfo.at(i)->isSelected = TRUE;
						pDoc->currObject.push_back(pDoc->logicInfo.at(i));
					}
				}
				
				//마우스 내에 객체가 하나라도 없으면 선택 취소됨.
				if (!checkFocus)
					pDoc->isOnFocus = FALSE;

				//현재 선택된 로직 오브젝트의 상태를 보여준다.
				if (pDoc->isOnFocus) {
					if (pDoc->currObject.size() == 1) {
						CPropertyView *PVCtrl = (CPropertyView *)((CMainFrame*)AfxGetMainWnd())->m_wndSplitterSub.GetPane(1, 0);
						
						PVCtrl->InitializePropGrid(pDoc->currObject.at(0));
					}
					else {

					}
				}
			}
		}
	}
	else if(object != OBJECT && !(pDoc->isSelected))
	{
		//동그란 표시 지우기
		if (pDoc->CanBeDivided) {
			pDoc->CanBeDivided = FALSE;
		}

		LineObject* temp_line[2];			//선 두개 생성
		temp_line[0] = new LineObject(dec_x, dec_y);
		temp_line[1] = new LineObject(dec_x, dec_y);

		cur_line = (int)pDoc->lines.size() + 1;
		pDoc->lines.push_back(temp_line[0]);
		pDoc->lines.push_back(temp_line[1]);

		temp_line[0]->connect_lines.push_back(temp_line[1]);	//두 선 서로 연결
		temp_line[1]->connect_lines.push_back(temp_line[0]);

		//첫번째 선에게만 오브젝트 연결
		for (int i = 0; i < pDoc->lines.size(); i++)	
		{
			if (i != cur_line && pDoc->lines.at(i)->Is_match_IineCoord(point))
			{
				pDoc->lines.at(i)->connect_lines.push_back(temp_line[0]);
				temp_line[0]->connect_lines.push_back(pDoc->lines.at(i));
			}
		}
		
	}

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

BOOL CCircuitView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (nHitTest == HTCLIENT)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		//배치 모드일땐 십자 표시
		if (pDoc->isSelected)
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		//선택 모드일땐 화살표 표시
		else if (pDoc->selectMode)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		//클릭 모드일땐 손모양 표시
		else if (pDoc->clickMode)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));

		return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CCircuitView::OnMouseMove(UINT nFlags, CPoint point)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	CClientDC dc(this);
	Gdiplus::Graphics graphics(dc);

	Gdiplus::Pen P(Gdiplus::Color(190, 190, 190), 2);
	Gdiplus::Pen DP(Gdiplus::Color(255, 255, 255), 2);

	dec_x = Rounding(point.x);
	dec_y = Rounding(point.y);

	//메뉴에서 오브젝트가 선택된 상태라면 움직이면
	//오브젝트가 그려지게 된다.

	if (object == OBJECT)
	{
		if (pDoc->isSelected) {
			if (pDoc->temp == NULL) {
				if (pDoc->objectType == GATE_TYPE)
				{

					switch (pDoc->objectName)
					{
					case AND_GATE:
						pDoc->temp = new andGate();
						break;
					case OR_GATE:
						pDoc->temp = new orGate();
						break;
					}

				}
				else if (pDoc->objectType == WIRING_TYPE)
				{
					switch (pDoc->objectName)
					{
					case PIN:
						pDoc->temp = new Pin();
						break;
					case CLOCK:
						pDoc->temp = new Clock();
						break;
					}
				}
			}
			
			if (pDoc->temp != NULL) {
				pDoc->temp->draw_shadow(&graphics, &DP);
				pDoc->temp->set_outputCoord(dec_x, dec_y);
				pDoc->temp->draw_shadow(&graphics, &P);
			}
		}

	} else {
		// LINE mode 에서 왼쪽 버튼 누르며 mouse가 움직일때
		if (nFlags == MK_LBUTTON)
		{
			CPoint dec = CPoint(dec_x, dec_y);
			LineObject* temp_line[2];				//클릭할 때 만들어둔 선 두개 받기
			temp_line[0] = pDoc->lines.at(cur_line - 1);
			temp_line[1] = pDoc->lines.at(cur_line);

			CPoint sp = temp_line[0]->line[0], cp = temp_line[0]->line[1];
			
			//선 자르고 그리기 수정 완료
			if (sp == cp)
			{
				temp_line[0]->line[1] = dec;

			}
			else
			{
				if (cp.x != sp.x)
				{
					temp_line[0]->line[1].x = dec.x;
					temp_line[0]->line[1].y = sp.y;		//확인사살
				}
				else if (cp.y != sp.y)
				{
					temp_line[0]->line[1].y = dec.y;
					temp_line[0]->line[1].x = sp.x;		//확인사살
				}
			}

			temp_line[1]->line[0] = temp_line[0]->line[1];
			temp_line[1]->line[1] = dec;
			Invalidate();
		}
	}

	BOOL nothingSearched = TRUE;
	//선을 이동중이거나 다른 작업중이면 동그라미 표시가 생기면 안된다. 클릭 모드 포함 (손 모양)
	if (nFlags != MK_LBUTTON &&
		!(pDoc->isSelected) && !(pDoc->clickMode)) {
		for (int i = 0; i < pDoc->lines.size(); i++)
		{
			if (pDoc->lines.at(i)->Is_match_IineCoord(point)) {
				pDoc->CanBeDivided = TRUE;
				nothingSearched = FALSE;
				Invalidate();
				break;
			}
		}
	}
	
	if (nothingSearched && pDoc->CanBeDivided) {
		pDoc->CanBeDivided = FALSE;
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}


void CCircuitView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	pDoc->isSelected = false;

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}



BOOL CCircuitView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return FALSE;
	//return CView::OnEraseBkgnd(pDC);
}


void CCircuitView::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnMouseLeave();
}


void CCircuitView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	//그리기를 마쳤을때 하는 behavior
	if (object == LINE)
	{
		//로직 오브젝트 검색 삭제
		for (int i = 0; i < pDoc->lines.size(); i++)
		{
			if (i != cur_line && pDoc->lines.at(i)->Is_match_IineCoord(point))
			{
				pDoc->lines.at(i)->connect_lines.push_back(pDoc->lines.at(cur_line));
				pDoc->lines.at(cur_line)->connect_lines.push_back(pDoc->lines.at(i));
			}
		}
		object = OBJECT;
	}

	Invalidate();

	CView::OnLButtonUp(nFlags, point);
}

//클릭 모드 버튼을 클릭했을 경우
void CCircuitView::OnClickMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = TRUE;
	pDoc->selectMode = FALSE;

}

//선택 모드 버튼을 클릭했을 경우
void CCircuitView::OnSelectMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = FALSE;
	pDoc->selectMode = TRUE;
}

//클릭 모드를 체크할때의 조건 확인
void CCircuitView::OnUpdateClickMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->clickMode == TRUE);
}

//선택모드를 체크할 때의 조건 확인
void CCircuitView::OnUpdateSelectMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->selectMode == TRUE);
}


void CCircuitView::OnOnSimulate()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (pDoc->simulateMode) {
		pDoc->simulateMode = FALSE;
		KillTimer(0);
	}
	else {
		pDoc->simulateMode = TRUE;
		SetTimer(0, 1000, NULL);
	}
		
}


void CCircuitView::OnUpdateOnSimulate(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->simulateMode == TRUE);
}


/*시뮬레이션 모드로 들어갈때 돌아갈 이벤트
*모든 Clock의 cycle을 1000씩 빼준다 (1초마다) 
*만약 cycle이 0이 될 경우 output toggle 및 cycle 초기화
*/
void CCircuitView::OnTimer(UINT_PTR nIDEvent)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	for (int i = 0; i < pDoc->clockInfo.size(); i++) {
		pDoc->clockInfo.at(i)->moveCycle();
	}

	Invalidate();
	CView::OnTimer(nIDEvent);
}

