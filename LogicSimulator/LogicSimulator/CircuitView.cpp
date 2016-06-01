// CircuitView.cpp : ���� �����Դϴ�.
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
	CPoint pos;			//���� ���콺 ������

	GetCursorPos(&pos);
	ScreenToClient(&pos);

	CRect rect;
	GetClientRect(&rect);

	pDoc->CheckCircuit();

	for (int i = 0; i < pDoc->logicInfo.size(); i++)
	{
		pDoc->logicInfo.at(i)->draw_main(&graphics);
		//���� ��ٸ� ǥ������
		if (pDoc->logicInfo.at(i)->isSelected)
			pDoc->logicInfo.at(i)->showSelected(&graphics);
	}
	
	//���� ���콺�� ��ġ�� �ڸ��� �б� (�Ǵ� ���׸���) ������ ���̶�� ���׶�� ǥ��
	P.SetColor(Gdiplus::Color(20, 20, 250));
	if(pDoc->CanBeDivided)
		graphics.DrawArc(&P, dec_x-5, dec_y-5, 10, 10, 0, 360);
	P.SetColor(Gdiplus::Color(0, 0, 0));

	for (int i = 0; i < pDoc->lines.size(); i++)
		pDoc->lines.at(i)->draw_main(&graphics);
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

	dec_x = Rounding(point.x);
	dec_y = Rounding(point.y);

	//���� �������� ���� LINE , �⺻���� OBJECT�� ��.
	object = OBJECT;

	//�� ������Ʈ���� ���� �б� �� ���
	// + �޴����� ������ �ȉ��� ��� ���� ��� ����
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
					Gtemp = new andGate(dec_x, dec_y);
					Gtemp->objectName = AND_GATE;
					break;
				case OR_GATE:
					Gtemp = new orGate(dec_x, dec_y);
					Gtemp->objectName = OR_GATE;
					break;
				}

				temp = Gtemp; // delete ������ �ʿ���.

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

		//Ŭ�� ����� ��� 
		//Pin �� Clock �� output �����͸� �ٲܻ� gate �� ������ ����..
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
		else {	//Ŭ�� ��� ���¿����� ��ü ���� ����
			if (!(pDoc->isSelected)) {
				pDoc->currObject.clear();

				POINT temp_top, temp_bottom;
				BOOL checkFocus = FALSE;

				for (int i = 0; i < pDoc->logicInfo.size(); i++){
					pDoc->logicInfo.at(i)->isSelected = FALSE;

					temp_top = pDoc->logicInfo.at(i)->get_top();

					temp_bottom = pDoc->logicInfo.at(i)->get_bottm();

					CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

					//���콺�� ��ü�ȿ� ������ ������.
					if (PtInRect(rect, point))
					{
						checkFocus = TRUE;
						pDoc->isOnFocus = TRUE;
						pDoc->logicInfo.at(i)->isSelected = TRUE;
						pDoc->currObject.push_back(pDoc->logicInfo.at(i));
					}
				}
				
				//���콺 ���� ��ü�� �ϳ��� ������ ���� ��ҵ�.
				if (!checkFocus)
					pDoc->isOnFocus = FALSE;

				//���� ���õ� ���� ������Ʈ�� ���¸� �����ش�.
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
		//���׶� ǥ�� �����
		if (pDoc->CanBeDivided) {
			pDoc->CanBeDivided = FALSE;
		}

		LineObject* temp_line[2];			//�� �ΰ� ����
		temp_line[0] = new LineObject(dec_x, dec_y);
		temp_line[1] = new LineObject(dec_x, dec_y);

		cur_line = (int)pDoc->lines.size() + 1;
		pDoc->lines.push_back(temp_line[0]);
		pDoc->lines.push_back(temp_line[1]);

		temp_line[0]->connect_lines.push_back(temp_line[1]);	//�� �� ���� ����
		temp_line[1]->connect_lines.push_back(temp_line[0]);

		//ù��° �����Ը� ������Ʈ ����
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

	dec_x = Rounding(point.x);
	dec_y = Rounding(point.y);

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
			CPoint dec = CPoint(dec_x, dec_y);
			LineObject* temp_line[2];				//Ŭ���� �� ������ �� �ΰ� �ޱ�
			temp_line[0] = pDoc->lines.at(cur_line - 1);
			temp_line[1] = pDoc->lines.at(cur_line);

			CPoint sp = temp_line[0]->line[0], cp = temp_line[0]->line[1];
			
			//�� �ڸ��� �׸��� ���� �Ϸ�
			if (sp == cp)
			{
				temp_line[0]->line[1] = dec;

			}
			else
			{
				if (cp.x != sp.x)
				{
					temp_line[0]->line[1].x = dec.x;
					temp_line[0]->line[1].y = sp.y;		//Ȯ�λ��
				}
				else if (cp.y != sp.y)
				{
					temp_line[0]->line[1].y = dec.y;
					temp_line[0]->line[1].x = sp.x;		//Ȯ�λ��
				}
			}

			temp_line[1]->line[0] = temp_line[0]->line[1];
			temp_line[1]->line[1] = dec;
			Invalidate();
		}
	}

	BOOL nothingSearched = TRUE;
	//���� �̵����̰ų� �ٸ� �۾����̸� ���׶�� ǥ�ð� ����� �ȵȴ�. Ŭ�� ��� ���� (�� ���)
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
		//���� ������Ʈ �˻� ����
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

