// CircuitView.cpp : ���� �����Դϴ�.
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

int object = OBJECT;
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
//	ON_WM_PAINT()
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


// CCircuitView �׸����Դϴ�.

void CCircuitView::OnDraw(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	// �޸� DC ����
	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	// ȭ�� DC�� ȣȯ�Ǵ� �޸� DC ��ü�� ����
	memDC.CreateCompatibleDC(pDC);

	// ���������� ȭ�� DC�� ȣȯ�Ǵ� Bitmap ����
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS); // ������� �ʱ�ȭ
		
	DrawImage(&memDC);			// �޸� DC�� �׸���

	// �޸� DC�� ȭ�� DC�� ��� ����
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

	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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
}

//void CCircuitView::OnPaint()
//{
//	
//
//}


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

/*
	���콺 ���� Ŭ�������� �Ͼ�� �� �ϵ�
	1. �޴����� �� ������Ʈ �����ϰ� �����ʿ��� �����ϸ�
	�׷�����.
	2. �޴����� �� ������Ʈ �������� �ʰ� (isSelected �� False �� ���) field ���� �����ϸ� ǥ�ð� ���鼭 ���õǾ���.
	3. 

*/
void CCircuitView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	CClientDC dc(this);
	Gdiplus::Graphics graphics(dc);
	Gdiplus::Pen P(Gdiplus::Color(0, 0, 0), 2);

	int dec_x, dec_y;
	dec_x = point.x - point.x % UNIT;
	dec_y = point.y - point.y % UNIT;

	//���� �������� ���� LINE , �⺻���� OBJECT�� ��.
	object = OBJECT;

	// ������ �ϳ��� ���콺 ������ ���� �ִٰ� �ϸ�
	// �� ���� �����ϰ� �����̰���.
	for (int i = 0; i < pDoc->gateInfo.size(); i++)            //�� ������Ʈ���� ���� �б� �� ���
		 {
		if (pDoc->gateInfo.at(i)->Is_match_inputCoord(point) != -1
			 || pDoc->gateInfo.at(i)->Is_match_outputCoord(point) == TRUE)
			 {
			object = LINE;
			break;
			}
		}
	for (int i = 0; i < pDoc->pinInfo.size(); i++)            //�� ������Ʈ���� ���� �б� �� ���
		 {
		if (pDoc->pinInfo.at(i)->Is_match_inputCoord(point) != -1
			 || pDoc->pinInfo.at(i)->Is_match_outputCoord(point) == TRUE)
			 {
			object = LINE;
			break;
			}
		}
	
		for (int i = 0; i < pDoc->clockInfo.size(); i++)            //�� ������Ʈ���� ���� �б� �� ���
		 {
		if (pDoc->clockInfo.at(i)->Is_match_inputCoord(point) != -1
			 || pDoc->clockInfo.at(i)->Is_match_outputCoord(point) == TRUE)
			 {
			object = LINE;
			break;
			}
		}


	if (object == OBJECT)
	{
		//�޴����� �����ϰ� �ʵ忡�� ���� �Ϸ�� ��
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
					Gtemp = new andGate();
					Gtemp->objectName = AND_GATE;
					Gtemp->objectType = GATE_TYPE;
					Gtemp->input_line[0] = NULL;
					Gtemp->input_line[1] = NULL;
					Gtemp->output_line = NULL;
					break;
				case OR_GATE:
					Gtemp = new orGate();
					Gtemp->objectName = OR_GATE;
					Gtemp->objectType = GATE_TYPE;
					Gtemp->input_line[0] = NULL;
					Gtemp->input_line[1] = NULL;
					Gtemp->output_line = NULL;
					break;
				}

				if (Gtemp != NULL) {
					Gtemp->set_outputCoord(dec_x, dec_y);
					Gtemp->set_Coord_From_outC(dec_x, dec_y);
					pDoc->gateInfo.push_back(Gtemp);
					pDoc->logicInfo.push_back(Gtemp);
				}
			}
			else if (pDoc->objectType == WIRING_TYPE)
			{
				Pin *Ptemp;
				Clock *Ctemp;
				temp = NULL;

				switch (pDoc->objectName)
				{
				case PIN :
					Ptemp = new Pin();
					Ptemp->objectType = WIRING_TYPE;
					Ptemp->objectName = PIN;
					Ptemp->connect_line = NULL;
					pDoc->pinInfo.push_back(Ptemp);
					temp = Ptemp;
					break;

				case CLOCK:
					Ctemp = new Clock();
					Ctemp->objectType = WIRING_TYPE;
					Ctemp->objectName = CLOCK;
					Ctemp->connect_line = NULL;
					pDoc->clockInfo.push_back(Ctemp);
					temp = Ctemp;
					break;
				}
				if (temp != NULL) {
					temp->set_outputCoord(dec_x, dec_y);
					temp->set_Coord_From_outC(dec_x, dec_y);
					pDoc->logicInfo.push_back(temp);
				}
			}

			delete pDoc->temp;
			pDoc->temp = NULL;
			pDoc->isSelected = false;
		}

		//Ŭ�� ����� ��� 
		//Pin �� Clock �� output �����͸� �ٲܻ� gate �� ������ ����..
		if (pDoc->clickMode) {
			CPoint pos;

			GetCursorPos(&pos);
			ScreenToClient(&pos);
			POINT temp_top, temp_bottom;

			for (int i = 0; i < pDoc->pinInfo.size(); i++)
			{
				
				temp_top = pDoc->pinInfo.at(i)->get_top();
				temp_bottom = pDoc->pinInfo.at(i)->get_bottm();

				CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

				if (PtInRect(rect, pos)) 
					pDoc->pinInfo.at(i)->toggleOutput();
			}

			for (int i = 0; i < pDoc->clockInfo.size(); i++)
			{
				temp_top = pDoc->clockInfo.at(i)->get_top();
				temp_bottom = pDoc->clockInfo.at(i)->get_bottm();

				CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

				if (PtInRect(rect, pos))
					pDoc->clockInfo.at(i)->toggleOutput();
			}
		}
	} else {		//������ LINE MODE�ΰ��� Ȯ���ϰ� ������
		LineObject* temp_line = new LineObject();
		cur_line = lines.size();
		lines.push_back(temp_line);
		temp_line->state = OFF_SIGNAL;
		
		for (int i = 0; i < pDoc->gateInfo.size(); i++)
		{
			int index = 0;
			if (pDoc->gateInfo.at(i)->Is_match_outputCoord(point))
			{
				pDoc->gateInfo.at(i)->output_line = temp_line;
			}
			else if ((index = pDoc->gateInfo.at(i)->Is_match_inputCoord(point)) != -1)
			{
				pDoc->gateInfo.at(i)->input_line[index] = temp_line;
			}
		}

		for (int i = 0; i < pDoc->pinInfo.size(); i++)
		{
			if (pDoc->pinInfo.at(i)->Is_match_outputCoord(point))
			{
				pDoc->pinInfo.at(i)->connect_line = temp_line;
			}
		}

		for (int i = 0; i < lines.size(); i++)
		{
			if (i != cur_line && lines.at(i)->Is_match_IineCoord(point))
				lines.at(i)->connect_lines.push_back(temp_line);
		}

		temp_line->line[0] = CPoint(dec_x, dec_y);
		temp_line->line[1] = CPoint(dec_x, dec_y);
		temp_line->line[0] = CPoint(dec_x, dec_y);
	}

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

//���� ���ư��鼭 0,1 ���� üũ�Ѵ�.
void CCircuitView::CheckCircuit()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	for (int i = 0; i < pDoc->pinInfo.size(); i++)
	{
		if (pDoc->pinInfo.at(i)->connect_line != NULL)
		{
			pDoc->pinInfo.at(i)->connect_line->state = (pDoc->pinInfo.at(i)->output == TRUE) ? ON_SIGNAL : OFF_SIGNAL;
			pDoc->pinInfo.at(i)->connect_line->check_connect();
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
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (nHitTest == HTCLIENT)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		//��ġ ����϶� ���� ǥ��
		if (pDoc->isSelected)
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		//���� ����϶� ȭ��ǥ ǥ��
		else if (pDoc->selectMode)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		//Ŭ�� ����϶� �ո�� ǥ��
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

	//���� �� ���¸� �����ַ� �ϴ� �ڵ� ������,
	//�� �κ��� ������ �ִ�. �ֳĸ� ��� line �� ���� ���� ���鼭 Ȯ���ϸ� �ѹ��� �ذ�Ǳ� �����̴�.


	//�޴����� ������Ʈ�� ���õ� ���¶�� �����̸�
	//������Ʈ�� �׷����� �ȴ�.

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
		// LINE mode ���� ���� ��ư ������ mouse�� �����϶�
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
	

	CView::OnMouseMove(nFlags, point);
}


void CCircuitView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	pDoc->isSelected = false;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}



BOOL CCircuitView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return FALSE;
	//return CView::OnEraseBkgnd(pDC);
}


void CCircuitView::OnMouseLeave()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CView::OnMouseLeave();
}


void CCircuitView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	//�׸��⸦ �������� �ϴ� behavior
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

		for (int i = 0; i < pDoc->gateInfo.size(); i++)
		{
			int index = 0;
			if (pDoc->gateInfo.at(i)->Is_match_outputCoord(point))
			{
				pDoc->gateInfo.at(i)->output_line = lines.at(cur_line);
			}
			else if ((index = pDoc->gateInfo.at(i)->Is_match_inputCoord(point)) != -1)
			{
				pDoc->gateInfo.at(i)->input_line[index] = lines.at(cur_line);
			}
		}

		for (int i = 0; i < pDoc->pinInfo.size(); i++)
		{
			if (pDoc->pinInfo.at(i)->Is_match_outputCoord(point))
			{
				pDoc->pinInfo.at(i)->connect_line = lines.at(cur_line);
			}
		}
		object = OBJECT;
	}

	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}

//Ŭ�� ��� ��ư�� Ŭ������ ���
void CCircuitView::OnClickMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = TRUE;
	pDoc->selectMode = FALSE;

}

//���� ��� ��ư�� Ŭ������ ���
void CCircuitView::OnSelectMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->clickMode = FALSE;
	pDoc->selectMode = TRUE;
}

//Ŭ�� ��带 üũ�Ҷ��� ���� Ȯ��
void CCircuitView::OnUpdateClickMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->clickMode == TRUE);
}

//���ø�带 üũ�� ���� ���� Ȯ��
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


/*�ùķ��̼� ���� ���� ���ư� �̺�Ʈ
*��� Clock�� cycle�� 1000�� ���ش� (1�ʸ���) 
*���� cycle�� 0�� �� ��� output toggle �� cycle �ʱ�ȭ
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
