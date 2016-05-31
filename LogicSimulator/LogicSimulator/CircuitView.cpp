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

	object = OBJECT;



	for (int i = 0; i < lines.size(); i++)						//선 오브젝트에서 선이 분기 될 경우
	{
		if (lines.at(i)->Is_match_IineCoord(point))
		{
			object = LINE;
			break;
		}
	}

	if (object == OBJECT)
	{
		if (pDoc->isSelected) {
			if (pDoc->objectType == GATE_TYPE)
			{
				Gate *temp;
				temp = NULL;

				switch (pDoc->objectName)
				{
				case AND_GATE:
					temp = new andGate(dec_x, dec_y);
					temp->objectName = AND_GATE;
					break;
				case OR_GATE:
					temp = new orGate(dec_x, dec_y);
					temp->objectName = OR_GATE;
					break;
				}

				if (temp != NULL) {
					temp->set_Coord_From_outC(dec_x, dec_y);
					
					lines.push_back(temp->input_line[0]);
					lines.push_back(temp->input_line[1]);
					lines.push_back(temp->output_line);

					pDoc->logicInfo.push_back(temp);
					pDoc->gateInfo.push_back(temp);
				}
			}
			else if (pDoc->objectType == WIRING_TYPE)
			{
				Pin *temp;
				temp = NULL;

				switch (pDoc->objectName)
				{
				case PIN:
					temp = new Pin();
					temp->objectType = WIRING_TYPE;
					temp->objectName = PIN;
					temp->output_line = new LineObject(dec_x, dec_y);
					break;
				}

				if (temp != NULL) {
					temp->set_outputCoord(dec_x, dec_y);
					temp->set_Coord_From_outC(dec_x, dec_y);

					lines.push_back(temp->output_line);
					pDoc->logicInfo.push_back(temp);
					pDoc->pinInfo.push_back(temp);
				}

			}

			free(pDoc->temp);
			pDoc->temp = NULL;
			pDoc->isSelected = false;
		}

		if (pDoc->clickMode) {
			CPoint pos;

			GetCursorPos(&pos);
			ScreenToClient(&pos);

			for (int i = 0; i < pDoc->logicInfo.size(); i++) {
				POINT temp_top, temp_bottom;
				temp_top = pDoc->logicInfo.at(i)->get_top();
				temp_bottom = pDoc->logicInfo.at(i)->get_bottm();

				CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

				if (PtInRect(rect, pos))
					pDoc->logicInfo.at(i)->toggleOutput();
			}


		}
	}
	else
	{
		LineObject* temp_line[2];								//선 두개 생성
		temp_line[0] = new LineObject(dec_x, dec_y);
		temp_line[1] = new LineObject(dec_x, dec_y);

		cur_line = lines.size() + 1;
		lines.push_back(temp_line[0]);
		lines.push_back(temp_line[1]);

		temp_line[0]->connect_lines.push_back(temp_line[1]);	//두 선 서로 연결
		temp_line[1]->connect_lines.push_back(temp_line[0]);

		for (int i = 0; i < lines.size(); i++)					//0에게만 오브젝트 연결
		{
			if (i != cur_line && lines.at(i)->Is_match_IineCoord(point))
			{
				lines.at(i)->connect_lines.push_back(temp_line[0]);
			}
		}
	}

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

void CCircuitView::CheckCircuit()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	for (int i = 0; i < pDoc->pinInfo.size(); i++)
	{
		if (pDoc->pinInfo.at(i)->output_line != NULL)
		{
			pDoc->pinInfo.at(i)->output_line->state = (pDoc->pinInfo.at(i)->output == TRUE) ? ON_SIGNAL : OFF_SIGNAL;
			pDoc->pinInfo.at(i)->output_line->check_connect();
		}
	}

	for (int i = 0; i < pDoc->gateInfo.size(); i++)
	{
		if (pDoc->gateInfo.at(i)->output_line != NULL && pDoc->gateInfo.at(i)->input_line[0] != NULL && pDoc->gateInfo.at(i)->input_line[1] != NULL
			&& pDoc->gateInfo.at(i)->input_line[0]->state == ON_SIGNAL && pDoc->gateInfo.at(i)->input_line[1]->state == ON_SIGNAL)
		{
			pDoc->gateInfo.at(i)->output_line->state = ON_SIGNAL;
		}
		else if (pDoc->gateInfo.at(i)->output_line != NULL)
			pDoc->gateInfo.at(i)->output_line->state = OUTPUT_SIGNAL;
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
	CPoint dec = CPoint(dec_x, dec_y);

	//로직 오브젝트 검색 삭제

	for (int i = 0; i < lines.size(); i++)
	{
		if (lines.at(i)->Is_match_IineCoord(point))
		{
			dc.Rectangle(dec_x - 5, dec_y - 5, dec_x + 5, dec_y + 5);
			break;
		}
	}

	//and east
	if (object == OBJECT)
	{
		if (pDoc->isSelected && pDoc->temp == NULL) {
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
				}
			}
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
			LineObject* temp_line[2];				//클릭할 때 만들어둔 선 두개 받기
			temp_line[0] = lines.at(cur_line - 1);
			temp_line[1] = lines.at(cur_line);

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
		//로직 오브젝트 검색 삭제
		for (int i = 0; i < lines.size(); i++)
		{
			if (i != cur_line && lines.at(i)->Is_match_IineCoord(point))
			{
				lines.at(i)->connect_lines.push_back(lines.at(cur_line));
				lines.at(cur_line)->connect_lines.push_back(lines.at(i));
			}
		}
	}
	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}

void CCircuitView::OnClickMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = TRUE;
	pDoc->selectMode = FALSE;

}


void CCircuitView::OnSelectMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = FALSE;
	pDoc->selectMode = TRUE;
}



void CCircuitView::OnUpdateClickMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->clickMode == TRUE);
}


void CCircuitView::OnUpdateSelectMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->selectMode == TRUE);
}
