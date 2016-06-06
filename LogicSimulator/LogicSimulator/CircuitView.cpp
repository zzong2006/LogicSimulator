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
//	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_UNDO, &CCircuitView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CCircuitView::OnEditRedo)
	ON_WM_DELETEITEM()
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


	for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
	{
		pDoc->currBox->logicInfo.at(i)->draw_main(&graphics);
		//���� ��ٸ� ǥ������
		if (pDoc->currBox->logicInfo.at(i)->isSelected)
			pDoc->currBox->logicInfo.at(i)->showSelected(&graphics);
		if (pDoc->currBox->logicInfo.at(i)->label != "") {
			pDoc->currBox->logicInfo.at(i)->showLabel(&graphics);
		}
	}
	
	//���� �޴����� ������ ������ ���¶�� shadowing �׸���
	if (pDoc->currBox->isSelected) {
		Gdiplus::Pen DP(Gdiplus::Color(255, 255, 255), 2); //Dummy Pen 
		pDoc->currBox->temp->set_outputCoord(dec_x, dec_y);
		pDoc->currBox->temp->draw_shadow(&graphics, &DP);
	}

	//���� ���콺�� ��ġ�� �ڸ��� �б� (�Ǵ� ���׸���) ������ ���̶�� ���׶�� ǥ��
	P.SetColor(Gdiplus::Color(20, 20, 250));
	if(pDoc->currBox->CanBeDivided)
		graphics.DrawArc(&P, dec_x-5, dec_y-5, 10, 10, 0, 360);
	P.SetColor(Gdiplus::Color(0, 0, 0));

	for (int i = 0; i < pDoc->currBox->lines.size(); i++)
		pDoc->currBox->lines.at(i)->draw_main(&graphics);
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
	CPoint cur_pos(dec_x, dec_y);

	pDoc->currBox->mRedo.RemoveAll();

	//���� �������� ���� LINE , �⺻���� OBJECT�� ��.
	object = OBJECT;

	//�� ������Ʈ���� ���� �б� �� ���
	// + �޴����� ������ �ȉ��� ��� ���� ��� ����

	////////�б� �˻�//////
	int ln = (int)pDoc->currBox->lines.size();

	for (int i = 0; i < pDoc->currBox->lines.size(); i++)		
	{
		if (pDoc->currBox->lines.at(i)->Is_match_IineCoord(point)
			&& !(pDoc->currBox->isSelected) && !(pDoc->currBox->clickMode))
		{
			//////////////////////////////////////////������ ���¼��� �߶������/////////////////////////////////////
			LineObject* curline = pDoc->currBox->lines.at(i);
			if (curline->line[0] != cur_pos && curline->line[1] != cur_pos)
			{
				LineObject* newline = new LineObject(cur_pos);

				newline->line[1] = curline->line[1];
				curline->line[1] = cur_pos;
				newline->shape = curline->shape;

				pDoc->currBox->lines.push_back(newline);
				pDoc->currBox->mUndo.GetHead().line_num++;
				pDoc->currBox->mUndo.GetHead().lineked_line.push_back(curline);
				pDoc->currBox->mUndo.GetHead().lineked_line.push_back(newline);
				object = LINE;
			}
			
			object = LINE;
			break;
		}
	}


	//�� ������Ʈ�� �߿��� �� �б� ���ɰ˻�
	for (int i = 0; i < pDoc->currBox->logicInfo.size() && object == OBJECT; i++)
	{
		int in = pDoc->currBox->logicInfo.at(i)->inputNum;
		int on = pDoc->currBox->logicInfo.at(i)->outputNum;

		CPoint pos;

		for (int j = 0; j < on; j++)
		{
			pos = pDoc->currBox->logicInfo.at(i)->outputCoord[j].first;

			if (dec_x == pos.x && dec_y == pos.y
				&& !(pDoc->currBox->isSelected) && !(pDoc->currBox->clickMode))
			{
				object = LINE;
				break;
			}
		}
		
		for (int j = 0; j < in; j++)
		{
			pos = pDoc->currBox->logicInfo.at(i)->inputCoord[j].first;
			if (dec_x == pos.x && dec_y == pos.y
				&& !(pDoc->currBox->isSelected) && !(pDoc->currBox->clickMode))
			{
				object = LINE;
				break;
			}
		}
	}


	////////////�� ������Ʈ ����/////////////////
	if (object == OBJECT)
	{
		//�޴����� �����ϰ� �ʵ忡�� ���� �Ϸ�� ��
		LogicObject* temp;
		temp = NULL;

		if (pDoc->currBox->isSelected) {
			if (pDoc->currBox->objectType == GATE_TYPE)
			{
				Gate *Gtemp;
				Gtemp = NULL;

				switch (pDoc->currBox->objectName)
				{
				case AND_GATE:
					Gtemp = new andGate(dec_x, dec_y);
					break;
				case OR_GATE:
					Gtemp = new orGate(dec_x, dec_y);
					break;
				case NAND_GATE:
					Gtemp = new nandGate(dec_x, dec_y);
					break;
				case NOR_GATE:
					Gtemp = new norGate(dec_x, dec_y);
					break;
				case XOR_GATE:
					Gtemp = new xorGate(dec_x, dec_y);
					break;
				case NOT_GATE:
					Gtemp = new notGate(dec_x, dec_y);
					break;
				}

				temp = Gtemp; // delete ������ �ʿ���.

				if (Gtemp != NULL) {
					Gtemp->set_Coord_From_outC(dec_x, dec_y);

					pDoc->currBox->logicInfo.push_back(Gtemp);
					pDoc->currBox->mUndo.AddHead(Action(GATE_TYPE, NEW));
				}
			}
			else if (pDoc->currBox->objectType == WIRING_TYPE)
			{
				Pin *Ptemp = NULL;
				Clock *Ctemp = NULL;
				Out	*Otemp = NULL;
				Sevenseg *Stemp = NULL;
				temp = NULL;

				switch (pDoc->currBox->objectName)
				{
				case PIN:
					Ptemp = new Pin(dec_x, dec_y);
					temp = Ptemp;

					pDoc->currBox->mUndo.AddHead(Action(PIN, NEW));
					break;
	
				case CLOCK:
					Ctemp = new Clock(dec_x,dec_y);
					temp = Ctemp;

					pDoc->currBox->mUndo.AddHead(Action(CLOCK, NEW));
					break;

				case OUTPIN:
					Otemp = new Out(dec_x, dec_y);
					temp = Otemp;

					pDoc->currBox->mUndo.AddHead(Action(OUTPIN, NEW));
					break;
				case SEG7:
					Stemp = new Sevenseg(dec_x, dec_y);
					temp = Stemp;

					pDoc->currBox->mUndo.AddHead(Action(SEG7, NEW));
				}

				if (temp != NULL) {
					temp->set_outputCoord(dec_x, dec_y);
					temp->set_Coord_From_outC(dec_x, dec_y);

					//������� ��� ���� ����.
					//7 seg�� ����������.
					pDoc->currBox->logicInfo.push_back(temp);

				}
			}
			else if (pDoc->currBox->objectType == FLIPFLOP_TYPE)
			{
				FlipFlop *Ftemp = NULL;

				switch (pDoc->currBox->objectName)
				{
				case D_FF:
					Ftemp = new DFlipFlop(dec_x, dec_y);
					break;
				case JK_FF:
					Ftemp = new JKFlipFlop(dec_x, dec_y);
					break;
				case T_FF:
					Ftemp = new TFlipFlop(dec_x, dec_y);
					break;
				}

				if (Ftemp != NULL) {
					Ftemp->set_Coord_From_outC(dec_x, dec_y);

					pDoc->currBox->logicInfo.push_back(Ftemp);
					pDoc->currBox->mUndo.AddHead(Action(FLIPFLOP_TYPE, NEW));
				}
			}

			delete pDoc->currBox->temp;
			pDoc->currBox->temp = NULL;
			pDoc->currBox->isSelected = false;
		}
		
		////Ŭ�� ���///////////////////////////////

		//Ŭ�� ����� ��� 
		//Pin �� Clock �� output �����͸� �ٲܻ� gate �� ������ ����..
		if (pDoc->currBox->clickMode) {
			POINT temp_top, temp_bottom;

			for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
			{
				LogicObject* tempLogic = pDoc->currBox->logicInfo.at(i);
				if (pDoc->currBox->IsInput(tempLogic))
				{
					temp_top = tempLogic->get_top();
					temp_bottom = tempLogic->get_bottm();

					CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

					if (PtInRect(rect, point))
						tempLogic->toggleOutput();
				}
			}
		}
		else {	//Ŭ�� ��� ���¿����� ��ü ���� ����
			if (!(pDoc->currBox->isSelected)) {
				pDoc->currBox->currObject.clear();

				POINT temp_top, temp_bottom;
				BOOL checkFocus = FALSE;

				for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++){
					pDoc->currBox->logicInfo.at(i)->isSelected = FALSE;

					temp_top = pDoc->currBox->logicInfo.at(i)->get_top();

					temp_bottom = pDoc->currBox->logicInfo.at(i)->get_bottm();

					CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

					//���콺�� ��ü�ȿ� ������ ������.
					if (PtInRect(rect, point))
					{
						checkFocus = TRUE;
						pDoc->currBox->isOnFocus = TRUE;
						pDoc->currBox->logicInfo.at(i)->isSelected = TRUE;
						pDoc->currBox->currObject.push_back(pDoc->currBox->logicInfo.at(i));
					}
				}
				
				//���콺 ���� ��ü�� �ϳ��� ������ ���� ��ҵ�.
				if (!checkFocus)
					pDoc->currBox->isOnFocus = FALSE;

				//���� ���õ� ���� ������Ʈ�� ���¸� �����ش�.
				if (pDoc->currBox->isOnFocus) {
					if (pDoc->currBox->currObject.size() == 1) {
						CPropertyView *PVCtrl = (CPropertyView *)((CMainFrame*)AfxGetMainWnd())->m_wndSplitterSub.GetPane(1, 0);
						
						PVCtrl->InitializePropGrid(pDoc->currBox->currObject.at(0));
					}
					else {

					}
				}
			}
		}
	}
	/////////�� ����/////////////////////////////
	else if(object != OBJECT && !(pDoc->currBox->isSelected))
	{
		//���׶� ǥ�� �����
		if (pDoc->currBox->CanBeDivided) {
			pDoc->currBox->CanBeDivided = FALSE;
		}

		LineObject* temp_line[2];			//�� �ΰ� ����
		temp_line[0] = new LineObject(dec_x, dec_y);
		temp_line[1] = new LineObject(dec_x, dec_y);

		cur_line = (int)pDoc->currBox->lines.size() + 1;
		pDoc->currBox->lines.push_back(temp_line[0]);
		pDoc->currBox->lines.push_back(temp_line[1]);
	}
	
	pDoc->currBox->CheckCircuit();
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
		if (pDoc->currBox->isSelected)
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
		//���� ����϶� ȭ��ǥ ǥ��
		else if (pDoc->currBox->selectMode)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		//Ŭ�� ����϶� �ո�� ǥ��
		else if (pDoc->currBox->clickMode)
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
		if (pDoc->currBox->isSelected) {
			if (pDoc->currBox->temp == NULL) {
				if (pDoc->currBox->objectType == GATE_TYPE)
				{

					switch (pDoc->currBox->objectName)
					{
					case AND_GATE:
						pDoc->currBox->temp = new andGate();
						break;
					case OR_GATE:
						pDoc->currBox->temp = new orGate();
						break;
					case NOT_GATE :
						pDoc->currBox->temp = new notGate();
						break;
					case NAND_GATE :
						pDoc->currBox->temp = new nandGate();
						break;
					case NOR_GATE :
						pDoc->currBox->temp = new norGate();
						break;
					case XOR_GATE :
						pDoc->currBox->temp = new xorGate();
						break;
					}

				}
				else if (pDoc->currBox->objectType == WIRING_TYPE)
				{
					switch (pDoc->currBox->objectName)
					{
					case PIN:
						pDoc->currBox->temp = new Pin();
						break;
					case CLOCK:
						pDoc->currBox->temp = new Clock();
						break;
					case OUTPIN :
						pDoc->currBox->temp = new Out();
						break;
					case SEG7 :
						pDoc->currBox->temp = new Sevenseg();
						break;
					}
				}
				else if (pDoc->currBox->objectType == FLIPFLOP_TYPE)
				{
					switch (pDoc->currBox->objectName)
					{
					case D_FF:
						pDoc->currBox->temp = new DFlipFlop();
						break;
					case T_FF:
						pDoc->currBox->temp = new TFlipFlop();
						break;
					case JK_FF:
						pDoc->currBox->temp = new JKFlipFlop();
						break;
					}
				}
			}
			Invalidate();
		}

	} else {
		// LINE mode ���� ���� ��ư ������ mouse�� �����϶�
		if (nFlags == MK_LBUTTON)
		{
			CPoint dec = CPoint(dec_x, dec_y);
			LineObject* temp_line[2];				//Ŭ���� �� ������ �� �ΰ� �ޱ�
			temp_line[0] = pDoc->currBox->lines.at(cur_line - 1);
			temp_line[1] = pDoc->currBox->lines.at(cur_line);

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
		!(pDoc->currBox->isSelected) && !(pDoc->currBox->clickMode)) {
		for (int i = 0; i < pDoc->currBox->lines.size(); i++)
		{
			if (pDoc->currBox->lines.at(i)->Is_match_IineCoord(point)) {
				pDoc->currBox->CanBeDivided = TRUE;
				nothingSearched = FALSE;
				Invalidate();
				break;
			}
		}

		//�� ������Ʈ�� �߿��� �� �б� ���ɰ˻�
		//���� �� �˻翡�� ������ ������ ���̻� �˻� ����.
		for (int i = 0; i < pDoc->currBox->logicInfo.size() && nothingSearched; i++)
		{
			int in = pDoc->currBox->logicInfo.at(i)->inputNum;
			int on = pDoc->currBox->logicInfo.at(i)->outputNum;
			CPoint pos;
			for (int j = 0; j < on && nothingSearched; j++)
			{
				pos = pDoc->currBox->logicInfo.at(i)->outputCoord[j].first;
				if (dec_x == pos.x && dec_y == pos.y)
				{
					pDoc->currBox->CanBeDivided = TRUE;
					nothingSearched = FALSE;
					Invalidate();
					break;
				}
			}
			
			for (int j = 0; j < in && nothingSearched; j++)
			{
				pos = pDoc->currBox->logicInfo.at(i)->inputCoord[j].first;
				if (dec_x == pos.x && dec_y == pos.y)
				{
					pDoc->currBox->CanBeDivided = TRUE;
					nothingSearched = FALSE;
					Invalidate();
					break;
				}
			}
		}
	}
	
	if (nothingSearched && pDoc->currBox->CanBeDivided) {
		pDoc->currBox->CanBeDivided = FALSE;
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}


void CCircuitView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	pDoc->currBox->isSelected = false;

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
	dec_x = Rounding(point.x);
	dec_y = Rounding(point.y);
	CPoint cur_pos(dec_x, dec_y);

	//�׸��⸦ �������� �ϴ� behavior
	if (object == LINE)
	{
		LineObject* temp_line[2];				//Ŭ���� �� ������ �� �ΰ� �ޱ�
		int curline = pDoc->currBox->lines.size() - 1;
		temp_line[0] = pDoc->currBox->lines.at(cur_line - 1);
		temp_line[1] = pDoc->currBox->lines.at(cur_line);
		//���� ������Ʈ �˻� ����

		int ln = pDoc->currBox->lines.size();

		/////////////////�� �Ӽ� �߰� ////////////////
		if (temp_line[0]->line[1].x == temp_line[0]->line[0].x)
			temp_line[0]->shape = VERTICAL;
		else
			temp_line[0]->shape = HORIZONTAL;

		if (temp_line[0]->shape == HORIZONTAL)
			temp_line[1]->shape = VERTICAL;
		else temp_line[1]->shape = HORIZONTAL;

		/////////////////////////////�� ����� & �޸𸮿� �߰�////////////////////////////////////

		Action mk_line = Action(LINE,NEW);

		for (int i = 1; i > -1; i--)
		{
			if (temp_line[i]->line[0] == temp_line[i]->line[1])
			{
				//pDoc->lines.pop_back();
				//mk_line.line_num--;
			}
		}

		//////////////////////////////////////////������ ���¼��� �߶������/////////////////////////////////////
		for (int i = 0; i < ln; i++)
		{
			if (i != cur_line && pDoc->currBox->lines.at(i)->Is_match_IineCoord(point))
			{
				if (pDoc->currBox->lines.at(i)->Is_match_IineCoord(point)
					&& !(pDoc->currBox->isSelected) && !(pDoc->currBox->clickMode))
				{
					LineObject* curline = pDoc->currBox->lines.at(i);
					if (curline->line[0] != cur_pos && curline->line[1] != cur_pos)
					{
						LineObject* newline = new LineObject(cur_pos);

						newline->line[1] = curline->line[1];
						curline->line[1] = cur_pos;

						newline->shape = curline->shape;

						mk_line.line_num++;
						mk_line.lineked_line.push_back(curline);
						mk_line.lineked_line.push_back(newline);

						pDoc->currBox->lines.push_back(newline);
					}
				}
			}
		}

		//if (mk_line.line_num > 0)
			pDoc->currBox->mUndo.AddHead(mk_line);
		
		object = OBJECT;
	}
	pDoc->currBox->CheckCircuit();
	Invalidate();

	CView::OnLButtonUp(nFlags, point);
}

//Ŭ�� ��� ��ư�� Ŭ������ ���
void CCircuitView::OnClickMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->currBox->clickMode = TRUE;
	pDoc->currBox->selectMode = FALSE;

}

//���� ��� ��ư�� Ŭ������ ���
void CCircuitView::OnSelectMode()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pDoc->currBox->clickMode = FALSE;
	pDoc->currBox->selectMode = TRUE;
}

//Ŭ�� ��带 üũ�Ҷ��� ���� Ȯ��
void CCircuitView::OnUpdateClickMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->currBox->clickMode == TRUE);
}

//���ø�带 üũ�� ���� ���� Ȯ��
void CCircuitView::OnUpdateSelectMode(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->currBox->selectMode == TRUE);
}


void CCircuitView::OnOnSimulate()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (pDoc->currBox->simulateMode) {
		pDoc->currBox->simulateMode = FALSE;
		KillTimer(0);
	}
	else {
		pDoc->currBox->simulateMode = TRUE;
		SetTimer(0, 10, NULL);
	}
		
}


void CCircuitView::OnUpdateOnSimulate(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	pCmdUI->SetCheck(pDoc->currBox->simulateMode == TRUE);
}


/*�ùķ��̼� ���� ���� ���ư� �̺�Ʈ
*��� Clock�� cycle�� 1000�� ���ش� (1�ʸ���) 
*���� cycle�� 0�� �� ��� output toggle �� cycle �ʱ�ȭ
*/
void CCircuitView::OnTimer(UINT_PTR nIDEvent)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	
	for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++) {
		LogicObject* tempLogic = pDoc->currBox->logicInfo.at(i);
		if (tempLogic->objectName == CLOCK){
			Clock* Ctemp = (Clock*)tempLogic;
			Ctemp->moveCycle();
		}
			
	}

	CView::OnTimer(nIDEvent);
}



void CCircuitView::OnEditUndo()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if(pDoc->currBox->CanUndo())
		pDoc->currBox->Undo();
	Beep(800, 50);
	Invalidate();
}


void CCircuitView::OnEditRedo()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

	if (pDoc->currBox->CanRedo())
		pDoc->currBox->Redo();
	Beep(300, 50);
	Invalidate();
}


void CCircuitView::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	int lin = pDoc->currBox->lines.size();
	int lon = pDoc->currBox->logicInfo.size();

	for (int i = 0; i < lin; i++)
	{
		
	}

	for (int i = 0; i < lon; i++)
	{
		if (pDoc->currBox->logicInfo.at(i)->isSelected == TRUE)
		{

		}
	}

	CView::OnDeleteItem(nIDCtl, lpDeleteItemStruct);
}
