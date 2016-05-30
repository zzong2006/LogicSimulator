// CircuitView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogicSimulator.h"
#include "CircuitView.h"
#include "LogicSimulatorDoc.h"
#include "LineObject.h"

#include <gdiplus.h>
#include <vector>

using std::vector;

// CCircuitView
#define LINE 100
#define GATE 101

int object = LINE;
int cur_line;
CPoint line[3];
int move_state = 0;
vector < LineObject *> lines;

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
	ON_WM_PAINT()
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

}


void CCircuitView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	Gdiplus::Graphics graphics(dc);
	Gdiplus::Pen P(Gdiplus::Color(0, 0, 0), 2);

	// TODO: 여기에 그리기 코드를 추가합니다.
	CRect rect;
	GetClientRect(&rect);
	CheckCircuit();

	/*for (int i = 0; i < rect.right; i += UNIT)
	{
	for (int j = 0; j < rect.bottom; j += UNIT)
	{
	dc.SetPixel(i, j, RGB(128, 128, 128));
	}
	}*/

	if (!pDoc->isSelected) {
		for (int i = 0; i < pDoc->logicInfo.size(); i++)
		{
			pDoc->logicInfo.at(i)->draw_main(&graphics);
		}
	}

	LineObject* templine;

	for (int i = 0; i < lines.size(); i++)
		lines.at(i)->draw_main(&graphics);

	if (lines.size() > 0)
	{
		templine = lines.at(cur_line);
		dc.Rectangle(templine->line[0].x - 5, templine->line[0].y - 5, templine->line[0].x + 5, templine->line[0].y + 5);
		dc.Rectangle(templine->line[2].x - 5, templine->line[2].y - 5, templine->line[2].x + 5, templine->line[2].y + 5);
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
	Gdiplus::Graphics graphics(dc);
	Gdiplus::Pen P(Gdiplus::Color(0, 0, 0), 2);

	int dec_x, dec_y;
	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;
	//and east

	object = GATE;

	for (int i = 0; i < pDoc->logicInfo.size(); i++)
	{
		if (pDoc->logicInfo.at(i)->Is_match_inputCoord(point) != -1
			|| pDoc->logicInfo.at(i)->Is_match_outputCoord(point) == TRUE)
		{
			object = LINE;
			break;
		}
	}

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines.at(i)->Is_match_IineCoord(point))
		{
			object = LINE;
			break;
		}
	}

	//로직 객체인 경우
	if (object == GATE)
	{
		//메뉴에서 선택하고 필드에서 지정 완료될 때
		if (pDoc->isSelected) {
			LogicObject *temp;
			temp = NULL;

			if (pDoc->selectedType == _T("AND Gate")) {
				temp = new andGate();
				temp->input_line[0] = NULL;
				temp->input_line[1] = NULL;
				temp->output_line = NULL;
			}
			else if (pDoc->selectedType == _T("OR Gate")) {
				temp = new orGate();
				temp->input_line[0] = NULL;
				temp->input_line[1] = NULL;
				temp->output_line = NULL;
			}
			else if (pDoc->selectedType == _T("Pin")) {
				temp = new Pin();
				temp->connect_line = NULL;
			}
			else if (pDoc->selectedType == _T("Clock")) {
				Clock *Ctemp = new Clock();
				temp = Ctemp;
				temp->connect_line = NULL;
				pDoc->clockInfo.push_back(Ctemp);
			}
			if (temp != NULL) {
				temp->set_outputCoord(dec_x, dec_y);
				temp->set_Coord_From_outC(dec_x, dec_y);
				pDoc->logicInfo.push_back(temp);
				
			}

			delete pDoc->temp;
			pDoc->temp = NULL;

			pDoc->isSelected = false;
		}

		//클릭 모드인 경우
		if (pDoc->clickMode) {
			CPoint pos;

			GetCursorPos(&pos);
			ScreenToClient(&pos);

			//마우스 위에 있는 로직 객체는 output 정보가 바뀐다.
			for (int i = 0; i < pDoc->logicInfo.size(); i++) {
				POINT temp_top, temp_bottom;
				temp_top = pDoc->logicInfo.at(i)->get_top();
				temp_bottom = pDoc->logicInfo.at(i)->get_bottm();

				CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

				if (PtInRect(rect, pos)) {
					pDoc->logicInfo.at(i)->toggleOutput();
				}
			}
		}
	} else {
		LineObject* temp_line = new LineObject();
		cur_line = lines.size();
		lines.push_back(temp_line);
		temp_line->state = OFF_SIGNAL;
		for (int i = 0; i < pDoc->logicInfo.size(); i++)
		{
			if (pDoc->logicInfo.at(i)->type == ("Pin"))
			{
				if (pDoc->logicInfo.at(i)->Is_match_outputCoord(point))
				{
					pDoc->logicInfo.at(i)->connect_line = temp_line;
				}
			}
			else if (pDoc->logicInfo.at(i)->type == ("Gate"))
			{
				int index = 0;
				if (pDoc->logicInfo.at(i)->Is_match_outputCoord(point))
				{
					pDoc->logicInfo.at(i)->output_line = temp_line;
				}
				else if ((index = pDoc->logicInfo.at(i)->Is_match_inputCoord(point)) != -1)
				{
					pDoc->logicInfo.at(i)->input_line[index] = temp_line;
				}
			}
		}

		for (int i = 0; i < lines.size(); i++)
		{
			if (i != cur_line && lines.at(i)->Is_match_IineCoord(point))
				lines.at(i)->connect_lines.push_back(temp_line);
		}

		temp_line->line[0] = CPoint(dec_x, dec_y);
		temp_line->line[1] = CPoint(dec_x, dec_y);
		temp_line->line[2] = CPoint(dec_x, dec_y);
	}

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

void CCircuitView::CheckCircuit()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	if (!pDoc->isSelected)
		for (int i = 0; i < pDoc->logicInfo.size(); i++)
		{
			if (pDoc->logicInfo.at(i)->type == ("Pin"))
			{
				if (pDoc->logicInfo.at(i)->connect_line != NULL)
				{
					pDoc->logicInfo.at(i)->connect_line->state = (pDoc->logicInfo.at(i)->output == TRUE)? ON_SIGNAL : OFF_SIGNAL;
					pDoc->logicInfo.at(i)->connect_line->check_connect();
				}
			}
		}
	if (!pDoc->isSelected)
		for (int i = 0; i < pDoc->logicInfo.size(); i++)
		{
			if (pDoc->logicInfo.at(i)->type == ("Gate"))
			{
				if (pDoc->logicInfo.at(i)->output_line != NULL && pDoc->logicInfo.at(i)->input_line[0] != NULL && pDoc->logicInfo.at(i)->input_line[1] != NULL
					&& pDoc->logicInfo.at(i)->input_line[0]->state == ON_SIGNAL && pDoc->logicInfo.at(i)->input_line[1]->state == ON_SIGNAL)
				{
					pDoc->logicInfo.at(i)->output_line->state = ON_SIGNAL;
				}
				else if (pDoc->logicInfo.at(i)->output_line != NULL)
					pDoc->logicInfo.at(i)->output_line->state = OUTPUT_SIGNAL;
			}
		}
}


BOOL CCircuitView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (nHitTest == HTCLIENT)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		if (pDoc->isSelected)
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		else if (pDoc->selectMode)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
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

	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;

	for (int i = 0; i < pDoc->logicInfo.size(); i++)
	{
		if (pDoc->logicInfo.at(i)->Is_match_inputCoord(point) != -1
			|| pDoc->logicInfo.at(i)->Is_match_outputCoord(point) == TRUE)
		{
			dc.Rectangle(dec_x - 5, dec_y - 5, dec_x + 5, dec_y + 5);
			break;
		}
	}

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines.at(i)->Is_match_IineCoord(point))
		{
			dc.Rectangle(dec_x - 5, dec_y - 5, dec_x + 5, dec_y + 5);
			break;
		}
	}

	if (object == GATE)
	{
		if (pDoc->isSelected) {
			if (pDoc->selectedType == _T("AND Gate")) {
				if (pDoc->temp == NULL)
					pDoc->temp = new andGate();
			}
			else if (pDoc->selectedType == _T("OR Gate")) {
				if (pDoc->temp == NULL)
					pDoc->temp = new orGate();
			}
			else if (pDoc->selectedType == _T("Pin")) {
				if (pDoc->temp == NULL)
					pDoc->temp = new Pin();
			}
			else if (pDoc->selectedType == _T("Clock")) {
				if (pDoc->temp == NULL)
					pDoc->temp = new Clock();
			}
			//다른 메뉴를 선택했을때 강제로 종료된다면 이 구문을 if문 안쪽으로 넣으면 해결됨.

			if (pDoc->temp != NULL) {
				pDoc->temp->draw_shadow(&graphics, &DP);
				pDoc->temp->set_outputCoord(dec_x, dec_y);
				pDoc->temp->draw_shadow(&graphics, &P);

			}
		}
	}
	else
	{
		if (nFlags == MK_LBUTTON)
		{
			LineObject* temp_line = lines.at(cur_line);
			if (move_state == HTOV)
			{
				temp_line->line[1].y = temp_line->line[0].y;
				temp_line->line[1].x = dec_x;
				temp_line->line[2].x = temp_line->line[1].x;
				temp_line->line[2].y = dec_y;
				if (dec_x == temp_line->line[0].x)
					move_state = VTOH;
			}
			else if (move_state == VTOH)
			{
				temp_line->line[1].x = temp_line->line[0].x;
				temp_line->line[1].y = dec_y;
				temp_line->line[2].y = temp_line->line[1].y;
				temp_line->line[2].x = dec_x;

				if (dec_y == temp_line->line[0].y)
					move_state = HTOV;
			}
			else{
				if (dec_y != temp_line->line[0].y)
					move_state = VTOH;
				else if (dec_x != temp_line->line[0].x)
					move_state = HTOV;
			}
			Invalidate();
		}
	}
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


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

	return CView::OnEraseBkgnd(pDC);
}


void CCircuitView::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnMouseLeave();
}


void CCircuitView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (object == LINE)
	{
		for (int i = 0; i < lines.size(); i++)
		{
			if (i != cur_line && lines.at(i)->Is_match_IineCoord(point))
			{
				lines.at(i)->connect_lines.push_back(lines.at(cur_line));
				lines.at(cur_line)->connect_lines.push_back(lines.at(i));
			}
		}
		for (int i = 0; i < pDoc->logicInfo.size(); i++)
		{
			if (pDoc->logicInfo.at(i)->type == ("Pin"))
			{
				if (pDoc->logicInfo.at(i)->Is_match_outputCoord(point))
				{
					pDoc->logicInfo.at(i)->connect_line = lines.at(cur_line);
				}
			}
			else if (pDoc->logicInfo.at(i)->type == ("Gate"))
			{
				int index = 0;
				if (pDoc->logicInfo.at(i)->Is_match_outputCoord(point))
				{
					pDoc->logicInfo.at(i)->output_line = lines.at(cur_line);
				}
				else if ((index = pDoc->logicInfo.at(i)->Is_match_inputCoord(point)) != -1)
				{
					pDoc->logicInfo.at(i)->input_line[index] = lines.at(cur_line);
				}
			}
		}
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
