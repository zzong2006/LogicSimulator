// CircuitView.cpp : ���� �����Դϴ�.
////

#include "stdafx.h"
#include "LogicSimulator.h"
#include "CircuitView.h"
#include "LogicSimulatorDoc.h"
#include "MainFrm.h"
#include "LineObject.h"
#include "PropertyView.h"
#include "Sevenseg.h"
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
	//	ON_COMMAND(ID_EDIT_UNDO, &CCircuitView::OnEditUndo)
	//	ON_COMMAND(ID_EDIT_REDO, &CCircuitView::OnEditRedo)
	ON_WM_KEYDOWN()
	ON_WM_COPYDATA()
	ON_COMMAND(ID_EDIT_COPY, &CCircuitView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CCircuitView::OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, &CCircuitView::OnEditCut)
	ON_COMMAND(ID_EDIT_UNDO, &CCircuitView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CCircuitView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CCircuitView::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CCircuitView::OnUpdateEditUndo)
END_MESSAGE_MAP()



// CCircuitView �׸����Դϴ�.

CPoint sdis, edis;
CPoint prevT, prevB;

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
		pDoc->currBox->logicInfo.at(i)->drawLine(&graphics);
		if (pDoc->currBox->logicInfo.at(i)->isSelected)
			pDoc->currBox->logicInfo.at(i)->showSelected(&graphics);
		if (pDoc->currBox->logicInfo.at(i)->label != "") {
			pDoc->currBox->logicInfo.at(i)->showLabel(&graphics);
		}
	}

	//���� �޴����� ������ ������ ���¶�� shadowing �׸���
	if (pDoc->isSelected) {
		Gdiplus::Pen DP(Gdiplus::Color(255, 255, 255), 2); //Dummy Pen 
		pDoc->currBox->temp->set_outputCoord(dec_x, dec_y);
		pDoc->currBox->temp->draw_shadow(&graphics, &DP);
	}

	//���� ���콺�� ��ġ�� �ڸ��� �б� (�Ǵ� ���׸���) ������ ���̶�� ���׶�� ǥ��
	P.SetColor(Gdiplus::Color(20, 20, 250));
	if (pDoc->currBox->CanBeDivided)
		graphics.DrawArc(&P, dec_x - 5, dec_y - 5, 10, 10, 0, 360);
	P.SetColor(Gdiplus::Color(0, 0, 0));

	for (int i = 0; i < pDoc->currBox->lines.size(); i++)
	{
		LineObject* tempLine = pDoc->currBox->lines.at(i);
		tempLine->draw_main(&graphics);
		if (tempLine->isSelected)
			tempLine->draw_showSelected(&graphics);
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

	BOOL check = FALSE;			//���̵� �����̵� �����Ϸ�

	//���� �������� ���� LINE , �⺻���� OBJECT�� ��.
	object = OBJECT;

	//�� ������Ʈ���� ���� �б� �� ���
	// + �޴����� ������ �ȉ��� ��� ���� ��� ����

	////////�б� �˻�//////

	int ln = (int)pDoc->currBox->lines.size();

	for (int i = 0; i < ln; i++)
	{
		if (pDoc->currBox->lines.at(i)->Is_match_IineCoord(point)
			&& !(pDoc->isSelected) && !(pDoc->clickMode))
		{
			LineObject* curline = pDoc->currBox->lines.at(i);
			if (curline->line[0] != cur_pos && curline->line[1] != cur_pos)
				object = LINE;
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
				&& !(pDoc->isSelected) && !(pDoc->clickMode))
			{
				object = LINE;
				break;
			}
		}

		for (int j = 0; j < in; j++)
		{
			pos = pDoc->currBox->logicInfo.at(i)->inputCoord[j].first;
			if (dec_x == pos.x && dec_y == pos.y
				&& !(pDoc->isSelected) && !(pDoc->clickMode))
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


		if (pDoc->isSelected) {
			pDoc->locked = TRUE;
			if (pDoc->objectType == GATE_TYPE)
			{
				Gate *Gtemp;
				Gtemp = NULL;

				switch (pDoc->objectName)
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
					pDoc->CheckPoint();
				}
			}
			else if (pDoc->objectType == WIRING_TYPE)
			{
				Pin *Ptemp = NULL;
				Clock *Ctemp = NULL;
				Out	*Otemp = NULL;
				Sevenseg *Stemp = NULL;
				temp = NULL;

				switch (pDoc->objectName)
				{
				case PIN:
					Ptemp = new Pin(dec_x, dec_y);
					temp = Ptemp;
					//���̺귯�� ���ڿ� ���� �Է� ��ġ�� ã���ش�.
					break;

				case CLOCK:
					Ctemp = new Clock(dec_x, dec_y);
					temp = Ctemp;

					break;

				case OUTPIN:
					Otemp = new Out(dec_x, dec_y);
					temp = Otemp;
					break;
				case SEG7:
					Stemp = new Sevenseg(dec_x, dec_y);
					temp = Stemp;


				}

				if (temp != NULL) {
					temp->set_outputCoord(dec_x, dec_y);
					temp->set_Coord_From_outC(dec_x, dec_y);

					//������� ��� ���� ����.
					//7 seg�� ����������.
					pDoc->currBox->logicInfo.push_back(temp);
					pDoc->CheckPoint();

				}
			}
			else if (pDoc->objectType == FLIPFLOP_TYPE)
			{
				FlipFlop *Ftemp = NULL;

				switch (pDoc->objectName)
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
					pDoc->CheckPoint();
				}
			}
			else if (pDoc->objectType == LIB) {
				Box *Btemp = NULL;

				if (pDoc->objectName == MAIN_LIB)
					Btemp = new Box(dec_x, dec_y, &(pDoc->logicBox[0]));
				else
					Btemp = new Box(dec_x, dec_y, &(pDoc->logicBox[1]));

				Btemp->set_Coord_From_outC(dec_x, dec_y);

				pDoc->currBox->logicInfo.push_back(Btemp);
				pDoc->CheckPoint();
			}

			delete pDoc->currBox->temp;
			pDoc->currBox->temp = NULL;
			pDoc->isSelected = false;
		}

		////Ŭ�� ���///////////////////////////////

		//Ŭ�� ����� ��� 
		//Pin �� Clock �� output �����͸� �ٲܻ� gate �� ������ ����..
		if (pDoc->clickMode) {
			POINT temp_top, temp_bottom;

			for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
			{
				LogicObject* tempLogic = pDoc->currBox->logicInfo.at(i);
				if (pDoc->currBox->IsInput(tempLogic))
				{
					temp_top = tempLogic->get_top();
					temp_bottom = tempLogic->get_bottm();

					CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

					if (PtInRect(rect, point) && (tempLogic->objectName == PIN || tempLogic->objectName == CLOCK))
						tempLogic->toggleOutput();
				}
			}
		}
		else {	//Ŭ�� ��� ���¿����� ��ü ���� ����
			if (!(pDoc->isSelected)) {
				pDoc->currBox->currObject.clear();

				POINT temp_top, temp_bottom;
				BOOL checkFocus = FALSE;

				for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++) {
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

						prevT.x = dec_x;
						prevT.y = dec_y;

						pDoc->currBox->currObject.push_back(pDoc->currBox->logicInfo.at(i));

					}
				}


				//���� ���õ� ���� ������Ʈ�� ���¸� �����ش�.
				if (pDoc->currBox->isOnFocus) {
					if (pDoc->currBox->currObject.size() == 1) {
						CPropertyView *PVCtrl = (CPropertyView *)((CMainFrame*)AfxGetMainWnd())->m_wndSplitterSub.GetPane(1, 0);

						PVCtrl->InitializePropGrid(pDoc->currBox->currObject.at(0));
					}
					else {	//�ټ� ���� 

					}
				}

				for (int i = 0; i < pDoc->currBox->lines.size(); i++)
				{
					pDoc->currBox->lines.at(i)->isSelected = FALSE;
					if (pDoc->currBox->lines.at(i)->Is_match_CLICK(point))
					{
						pDoc->currBox->lines.at(i)->isSelected = TRUE;
						pDoc->currBox->isOnFocus = TRUE;
						checkFocus = TRUE;

						sdis.x = dec_x - pDoc->currBox->lines.at(i)->line[0].x;
						sdis.y = dec_y - pDoc->currBox->lines.at(i)->line[0].y;
						edis.x = dec_x - pDoc->currBox->lines.at(i)->line[1].x;
						edis.y = dec_y - pDoc->currBox->lines.at(i)->line[1].y;

					}
				}

				//���콺 ���� ��ü�� �ϳ��� ������ ���� ��ҵ�.
				if (!checkFocus)
					pDoc->currBox->isOnFocus = FALSE;
			}
		}
	}

	/////////�� ����/////////////////////////////
	else if (object != OBJECT && !(pDoc->isSelected))
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
		if (nFlags == MK_LBUTTON && pDoc->currBox->isOnFocus && !(pDoc->locked) && !(pDoc->clickMode))
		{
			for (int i = 0; i < pDoc->currBox->lines.size(); i++)
			{
				if (pDoc->currBox->lines.at(i)->isSelected == TRUE)
				{
					LineObject *templine = pDoc->currBox->lines.at(i);
					templine->line[0].x = dec_x - sdis.x;
					templine->line[0].y = dec_y - sdis.y;
					templine->line[1].x = dec_x - edis.x;
					templine->line[1].y = dec_y - edis.y;
				}
			}

			for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
			{
				if (pDoc->currBox->logicInfo.at(i)->isSelected == TRUE)
				{
					pDoc->currBox->logicInfo.at(i)->top.x += dec_x - prevT.x;
					pDoc->currBox->logicInfo.at(i)->top.y += dec_y - prevT.y;
					pDoc->currBox->logicInfo.at(i)->bottom.x += dec_x - prevT.x;
					pDoc->currBox->logicInfo.at(i)->bottom.y += dec_y - prevT.y;

					pDoc->currBox->logicInfo.at(i)->set_inputCoord(dec_x - prevT.x, dec_y - prevT.y);
					pDoc->currBox->logicInfo.at(i)->set_outputCoord(dec_x - prevT.x, dec_y - prevT.y);

				}
			}
			//�̵��Ÿ��� ª�ٸ� ������ ���� �ƴϴ�.
			if (dec_x - prevT.x >= 10 || dec_y - prevT.y >= 10)
				pDoc->moved = TRUE;

			prevT.x = dec_x;
			prevT.y = dec_y;

			Invalidate();
		}
		if (pDoc->isSelected) {
			if (pDoc->currBox->temp == NULL) {
				if (pDoc->objectType == GATE_TYPE)
				{
					switch (pDoc->objectName)
					{
					case AND_GATE:
						pDoc->currBox->temp = new andGate();
						break;
					case OR_GATE:
						pDoc->currBox->temp = new orGate();
						break;
					case NOT_GATE:
						pDoc->currBox->temp = new notGate();
						break;
					case NAND_GATE:
						pDoc->currBox->temp = new nandGate();
						break;
					case NOR_GATE:
						pDoc->currBox->temp = new norGate();
						break;
					case XOR_GATE:
						pDoc->currBox->temp = new xorGate();
						break;
					}

				}
				else if (pDoc->objectType == WIRING_TYPE)
				{
					switch (pDoc->objectName)
					{
					case PIN:
						pDoc->currBox->temp = new Pin();

						break;
					case CLOCK:
						pDoc->currBox->temp = new Clock();
						break;
					case OUTPIN:
						pDoc->currBox->temp = new Out();

						break;
					case SEG7:
						pDoc->currBox->temp = new Sevenseg();
						break;
					}
				}
				else if (pDoc->objectType == FLIPFLOP_TYPE)
				{
					switch (pDoc->objectName)
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
				else if (pDoc->objectType == LIB)
				{
					pDoc->currBox->temp = new Box();
				}
			}
			Invalidate();
		}

	}
	else {
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
		!(pDoc->isSelected) && !(pDoc->clickMode)) {
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
	dec_x = Rounding(point.x);
	dec_y = Rounding(point.y);
	CPoint cur_pos(dec_x, dec_y);

	if (pDoc->locked)
		pDoc->locked = FALSE;

	if (pDoc->currBox->isOnFocus) {
		for (int i = 0; i < pDoc->currBox->lines.size(); i++)
		{
			pDoc->currBox->lines.at(i)->isSelected == FALSE;
		}

		for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
		{
			if (pDoc->currBox->logicInfo.at(i)->isSelected == TRUE)
			{
				CString input;
				switch (pDoc->currBox->logicInfo.at(i)->facing) {
				case EAST:
					input = _T("East");
					break;
				case WEST:
					input = _T("West");
					break;
				case NORTH:
					input = _T("North");
					break;
				case SOUTH:
					input = _T("South");
					break;
				}
				pDoc->currBox->logicInfo.at(i)->set_Coord_ByFacing(input);
			}
			pDoc->currBox->logicInfo.at(i)->isSelected == FALSE;
		}

		if (pDoc->moved) {
			pDoc->CheckPoint();
			pDoc->moved = FALSE;
		}
	}

	//�׸��⸦ �������� �ϴ� behavior
	if (object == LINE)
	{
		LineObject* temp_line[2];				//Ŭ���� �� ������ �� �ΰ� �ޱ�

		int curline = pDoc->currBox->lines.size() - 1;
		temp_line[0] = pDoc->currBox->lines.at(cur_line - 1);
		temp_line[1] = pDoc->currBox->lines.at(cur_line);

		if (!(temp_line[0]->line[0] == temp_line[0]->line[1] && temp_line[1]->line[0] == temp_line[1]->line[1]))
		{
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


			for (int i = 1; i > -1; i--)
			{
				if (temp_line[i]->line[0] == temp_line[i]->line[1])
				{
					pDoc->currBox->lines.pop_back();

				}
			}

			for (int i = 0; i < pDoc->currBox->lines.size(); i++)
			{
				pDoc->currBox->lines.at(i)->isSelected = FALSE;
				if (pDoc->currBox->lines.at(i)->Is_match_IineCoord(temp_line[0]->line[0]))
				{
					LineObject* curline = pDoc->currBox->lines.at(i);
					if (curline->line[0] != temp_line[0]->line[0] && curline->line[1] != temp_line[0]->line[0])
					{
						LineObject* newline = new LineObject(temp_line[0]->line[0]);

						newline->line[1] = curline->line[1];
						curline->line[1] = temp_line[0]->line[0];
						newline->shape = curline->shape;

						pDoc->currBox->lines.push_back(newline);
					}
					break;
				}
			}


			//////////////////////////////////////////������ ���¼��� �߶������/////////////////////////////////////
			for (int i = 0; i < pDoc->currBox->lines.size(); i++)
			{
				if (i != cur_line && pDoc->currBox->lines.at(i)->Is_match_IineCoord(point))
				{
					if (pDoc->currBox->lines.at(i)->Is_match_IineCoord(point))
					{
						LineObject* curline = pDoc->currBox->lines.at(i);
						if (curline->line[0] != cur_pos && curline->line[1] != cur_pos)
						{
							LineObject* newline = new LineObject(cur_pos);

							newline->line[1] = curline->line[1];
							curline->line[1] = cur_pos;

							newline->shape = curline->shape;


							pDoc->currBox->lines.push_back(newline);
						}
					}
				}
			}
			//////////////////////////////////////////������ ���¼��� �߶������/////////////////////////////////////

		}
		else
		{
			pDoc->currBox->lines.pop_back();
			pDoc->currBox->lines.pop_back();
		}

		pDoc->currBox->LineCheck();
		object = OBJECT;

		pDoc->CheckPoint();
	}


	pDoc->currBox->CheckCircuit();

	int lin = pDoc->currBox->lines.size();
	int lon = pDoc->currBox->logicInfo.size();

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
		SetTimer(0, 10, NULL);
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

	for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++) {
		LogicObject* tempLogic = pDoc->currBox->logicInfo.at(i);
		if (tempLogic->objectName == CLOCK) {
			Clock* Ctemp = (Clock*)tempLogic;
			Ctemp->moveCycle();
		}

	}

	CView::OnTimer(nIDEvent);
}


void CCircuitView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if (nChar == VK_DELETE)
	{
		CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

		int lin = pDoc->currBox->lines.size();
		int lon = pDoc->currBox->logicInfo.size();

		for (int i = 0; i < lin; i++)
		{
			if (pDoc->currBox->lines.at(i)->isSelected == TRUE)
			{

				delete pDoc->currBox->lines.at(i);
				pDoc->currBox->lines.erase(pDoc->currBox->lines.begin() + i);
				lin--;
			}
		}

		for (int i = 0; i < lon; i++)
		{
			if (pDoc->currBox->logicInfo.at(i)->isSelected == TRUE)
			{
				//OUT & INPIN Ư�� ���
				if (pDoc->currBox->logicInfo.at(i)->objectName == PIN) {
					Pin* TP = (Pin*)pDoc->currBox->logicInfo.at(i);
					if (TP->getConNum() >= 0)
						pDoc->currBox->ConnInput[TP->getConNum()] = FALSE;

					pDoc->currBox->NumInput--;
				}
				else if (pDoc->currBox->logicInfo.at(i)->objectName == OUTPIN) {
					Out* TO = (Out*)pDoc->currBox->logicInfo.at(i);
					if (TO->getConNum() >= 0)
						pDoc->currBox->ConnOutput[TO->getConNum()] = FALSE;

					pDoc->currBox->NumOuput--;
				}

				delete pDoc->currBox->logicInfo.at(i);

				pDoc->currBox->logicInfo.erase(pDoc->currBox->logicInfo.begin() + i);
				lon--;
			}
		}

		pDoc->currBox->LineCheck();
		pDoc->CheckPoint();
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CCircuitView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	AfxMessageBox(_T(""));
	return CView::OnCopyData(pWnd, pCopyDataStruct);
}


void CCircuitView::OnEditCopy()
{
	PASTED = TRUE;
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	
	pDoc->currBox->store.name.clear();
	pDoc->currBox->store.type.clear();
	pDoc->currBox->store.lines.clear();

	int lin = pDoc->currBox->lines.size();
	int lon = pDoc->currBox->logicInfo.size();


	//������ �� �����ޱ�
	for (int i = 0; i < lin; i++)
	{
		if (pDoc->currBox->lines.at(i)->isSelected == TRUE)
		{
			LineObject temp = *pDoc->currBox->lines.at(i);
			temp.line[0].x += 50;
			temp.line[0].y += 50;
			temp.line[1].x += 50;
			temp.line[1].y += 50;
		}
	}
	//������ ������Ʈ ���� �ޱ�
	for (int i = 0; i < lon; i++)
	{
		//�̸� �ް� Ÿ�� �ް� bottom�ϰ� top �޴´�.
		if (pDoc->currBox->logicInfo.at(i)->isSelected == TRUE)
		{
			pDoc->currBox->store.name.push_back(pDoc->currBox->logicInfo.at(i)->objectName);
			pDoc->currBox->store.type.push_back(pDoc->currBox->logicInfo.at(i)->objectType);
			pDoc->currBox->store.facing = pDoc->currBox->logicInfo.at(i)->facing;
			pDoc->currBox->store.dec.x = pDoc->currBox->logicInfo.at(i)->bottom.x + 50;
			pDoc->currBox->store.dec.y = (pDoc->currBox->logicInfo.at(i)->top.y + pDoc->currBox->logicInfo.at(i)->bottom.y) /2 + 50;
		}
	}
}


void CCircuitView::OnEditPaste()
{
	if (PASTED || CUTED) {
		CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();

		for (int i = 0; i < pDoc->currBox->store.name.size(); i++)
		{
			LogicObject* temp;
			int dec_x = pDoc->currBox->store.dec.x, dec_y = pDoc->currBox->store.dec.y;

			if (pDoc->currBox->store.type.at(i) == GATE_TYPE)
			{
				Gate *Gtemp;
				Gtemp = NULL;

				switch (pDoc->currBox->store.name.at(i))
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
				}
			}
			else if (pDoc->currBox->store.type.at(i) == WIRING_TYPE)
			{
				Pin *Ptemp = NULL;
				Clock *Ctemp = NULL;
				Out	*Otemp = NULL;
				Sevenseg *Stemp = NULL;
				temp = NULL;

				switch (pDoc->currBox->store.name.at(i))
				{
				case PIN:
					Ptemp = new Pin(dec_x, dec_y);
					temp = Ptemp;
					//���̺귯�� ���ڿ� ���� �Է� ��ġ�� ã���ش�.
					break;

				case CLOCK:
					Ctemp = new Clock(dec_x, dec_y);
					temp = Ctemp;

					break;

				case OUTPIN:
					Otemp = new Out(dec_x, dec_y);
					temp = Otemp;
					break;
				case SEG7:
					Stemp = new Sevenseg(dec_x, dec_y);
					temp = Stemp;


				}

				if (temp != NULL) {
					temp->set_outputCoord(dec_x, dec_y);
					temp->set_Coord_From_outC(dec_x, dec_y);

					//������� ��� ���� ����.
					//7 seg�� ����������.

				}
			}
			else if (pDoc->currBox->store.type.at(i) == FLIPFLOP_TYPE)
			{
				FlipFlop *Ftemp = NULL;

				switch (pDoc->currBox->store.name.at(i))
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
					temp = Ftemp;
				}
			}
			else if (pDoc->currBox->store.type.at(i) == LIB) {
				Box *Btemp = NULL;

				if (pDoc->currBox->store.name.at(i) == MAIN_LIB);
				//Btemp = new Box(dec_x, dec_y, &(pDoc->logicBox[0]));
				else
					Btemp = new Box(dec_x, dec_y, &(pDoc->logicBox[1]));

				Btemp->set_Coord_From_outC(dec_x, dec_y);
				temp = Btemp;
			}
			temp->facing = pDoc->currBox->store.facing;
			temp->set_Coord_ByFacing(temp->facing);
			pDoc->currBox->logicInfo.push_back(temp);
			temp = NULL;
		}

		pDoc->CheckPoint();
		Invalidate();

		CUTED = FALSE;
	}
	
}
// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

void CCircuitView::OnEditCut()
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)GetDocument();
	int lin = pDoc->currBox->lines.size();
	int lon = pDoc->currBox->logicInfo.size();

	pDoc->currBox->store.lines.clear();
	pDoc->currBox->store.type.clear();
	pDoc->currBox->store.name.clear();

	CUTED = TRUE;
	PASTED = FALSE;
	//������ �� �����ޱ�

	for (int i = 0; i < lin; i++)
	{
		if (pDoc->currBox->lines.at(i)->isSelected == TRUE)
		{
			LineObject temp = *pDoc->currBox->lines.at(i);
			temp.line[0].x += 50;
			temp.line[0].y += 50;
			temp.line[1].x += 50;
			temp.line[1].y += 50;
			pDoc->currBox->store.lines.push_back(temp);
		}
	}
	//������ ������Ʈ ���� �ޱ�
	for (int i = 0; i < lon; i++)
	{
		if (pDoc->currBox->logicInfo.at(i)->isSelected == TRUE)
		{
			pDoc->currBox->store.name.push_back(pDoc->currBox->logicInfo.at(i)->objectName);
			pDoc->currBox->store.type.push_back(pDoc->currBox->logicInfo.at(i)->objectType);
			pDoc->currBox->store.facing = pDoc->currBox->logicInfo.at(i)->facing;
			pDoc->currBox->store.dec.x = pDoc->currBox->logicInfo.at(i)->bottom.x + 50;
			pDoc->currBox->store.dec.y = (pDoc->currBox->logicInfo.at(i)->top.y + pDoc->currBox->logicInfo.at(i)->bottom.y) / 2 + 50;
		}
	}

	//�����ϰ� �����ش�.

	lin = pDoc->currBox->lines.size();
	lon = pDoc->currBox->logicInfo.size();

	for (int i = 0; i < lin; i++)
	{
		if (pDoc->currBox->lines.at(i)->isSelected == TRUE)
		{
			delete pDoc->currBox->lines.at(i);
			pDoc->currBox->lines.erase(pDoc->currBox->lines.begin() + i);
			lin--;
		}
	}

	for (int i = 0; i < lon; i++)
	{
		if (pDoc->currBox->logicInfo.at(i)->isSelected == TRUE)
		{
			//OUT & INPIN Ư�� ���
			if (pDoc->currBox->logicInfo.at(i)->objectName == PIN) {
				Pin* TP = (Pin*)pDoc->currBox->logicInfo.at(i);
				if (TP->getConNum() >= 0)
					pDoc->currBox->ConnInput[TP->getConNum()] = FALSE;

				pDoc->currBox->NumInput--;
			}
			else if (pDoc->currBox->logicInfo.at(i)->objectName == OUTPIN) {
				Out* TO = (Out*)pDoc->currBox->logicInfo.at(i);
				if (TO->getConNum() >= 0)
					pDoc->currBox->ConnOutput[TO->getConNum()] = FALSE;

				pDoc->currBox->NumOuput--;
			}

			delete pDoc->currBox->logicInfo.at(i);

			pDoc->currBox->logicInfo.erase(pDoc->currBox->logicInfo.begin() + i);
			lon--;
		}
	}

	Invalidate();
}


void CCircuitView::OnEditUndo()
{
	CLogicSimulatorDoc* pDoc = (CLogicSimulatorDoc*)GetDocument();
	pDoc->Undo();
	pDoc->UpdateAllViews(NULL);
}


void CCircuitView::OnEditRedo()
{
	CLogicSimulatorDoc* pDoc = (CLogicSimulatorDoc*)GetDocument();
	pDoc->Redo();
	pDoc->UpdateAllViews(NULL);
}


void CCircuitView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc* pDoc = (CLogicSimulatorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->CanRedo());
}


void CCircuitView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	CLogicSimulatorDoc* pDoc = (CLogicSimulatorDoc*)GetDocument();
	pCmdUI->Enable(pDoc->CanUndo());
}
