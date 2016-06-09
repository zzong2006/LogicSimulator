// CircuitView.cpp : 구현 파일입니다.
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



// CCircuitView 그리기입니다.

CPoint sdis, edis;
CPoint prevT, prevB;

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


	for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++)
	{
		pDoc->currBox->logicInfo.at(i)->draw_main(&graphics);
		//선택 됬다면 표시해줌
		pDoc->currBox->logicInfo.at(i)->drawLine(&graphics);
		if (pDoc->currBox->logicInfo.at(i)->isSelected)
			pDoc->currBox->logicInfo.at(i)->showSelected(&graphics);
		if (pDoc->currBox->logicInfo.at(i)->label != "") {
			pDoc->currBox->logicInfo.at(i)->showLabel(&graphics);
		}
	}

	//현재 메뉴에서 아이템 선택한 상태라면 shadowing 그리기
	if (pDoc->isSelected) {
		Gdiplus::Pen DP(Gdiplus::Color(255, 255, 255), 2); //Dummy Pen 
		pDoc->currBox->temp->set_outputCoord(dec_x, dec_y);
		pDoc->currBox->temp->draw_shadow(&graphics, &DP);
	}

	//현재 마우스가 위치한 자리가 분기 (또는 선그리기) 가능한 점이라면 동그라미 표시
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
	CPoint cur_pos(dec_x, dec_y);

	BOOL check = FALSE;			//선이든 로직이든 생성완료

	//선을 선택했을 경우는 LINE , 기본값은 OBJECT로 함.
	object = OBJECT;

	//선 오브젝트에서 선이 분기 될 경우
	// + 메뉴에서 선택이 안됬을 경우 라인 모드 진입

	////////분기 검사//////

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

	//논리 오브젝트들 중에서 선 분기 가능검사
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


	////////////논리 오브젝트 생성/////////////////
	if (object == OBJECT)
	{
		//메뉴에서 선택하고 필드에서 지정 완료될 때
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

				temp = Gtemp; // delete 용으로 필요함.

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
					//라이브러리 상자에 대한 입력 위치를 찾아준다.
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

					//출력핀은 출력 선이 없다.
					//7 seg도 마찬가지임.
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

		////클릭 모드///////////////////////////////

		//클릭 모드인 경우 
		//Pin 과 Clock 의 output 데이터를 바꿀뿐 gate 는 영향이 없다..
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
		else {	//클릭 모드 상태에서만 객체 선택 가능
			if (!(pDoc->isSelected)) {
				pDoc->currBox->currObject.clear();

				POINT temp_top, temp_bottom;
				BOOL checkFocus = FALSE;

				for (int i = 0; i < pDoc->currBox->logicInfo.size(); i++) {
					pDoc->currBox->logicInfo.at(i)->isSelected = FALSE;

					temp_top = pDoc->currBox->logicInfo.at(i)->get_top();

					temp_bottom = pDoc->currBox->logicInfo.at(i)->get_bottm();

					CRect rect(temp_top.x, temp_top.y, temp_bottom.x, temp_bottom.y);

					//마우스가 객체안에 있으면 선택함.
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


				//현재 선택된 로직 오브젝트의 상태를 보여준다.
				if (pDoc->currBox->isOnFocus) {
					if (pDoc->currBox->currObject.size() == 1) {
						CPropertyView *PVCtrl = (CPropertyView *)((CMainFrame*)AfxGetMainWnd())->m_wndSplitterSub.GetPane(1, 0);

						PVCtrl->InitializePropGrid(pDoc->currBox->currObject.at(0));
					}
					else {	//다수 선택 

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

				//마우스 내에 객체가 하나라도 없으면 선택 취소됨.
				if (!checkFocus)
					pDoc->currBox->isOnFocus = FALSE;
			}
		}
	}

	/////////선 생성/////////////////////////////
	else if (object != OBJECT && !(pDoc->isSelected))
	{
		//동그란 표시 지우기
		if (pDoc->currBox->CanBeDivided) {
			pDoc->currBox->CanBeDivided = FALSE;
		}

		LineObject* temp_line[2];			//선 두개 생성
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
			//이동거리가 짧다면 움직인 것이 아니다.
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
		// LINE mode 에서 왼쪽 버튼 누르며 mouse가 움직일때
		if (nFlags == MK_LBUTTON)
		{
			CPoint dec = CPoint(dec_x, dec_y);
			LineObject* temp_line[2];				//클릭할 때 만들어둔 선 두개 받기
			temp_line[0] = pDoc->currBox->lines.at(cur_line - 1);
			temp_line[1] = pDoc->currBox->lines.at(cur_line);

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
		for (int i = 0; i < pDoc->currBox->lines.size(); i++)
		{
			if (pDoc->currBox->lines.at(i)->Is_match_IineCoord(point)) {
				pDoc->currBox->CanBeDivided = TRUE;
				nothingSearched = FALSE;
				Invalidate();
				break;
			}
		}

		//논리 오브젝트들 중에서 선 분기 가능검사
		//앞의 선 검사에서 오케이 됬으면 더이상 검사 안함.
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

	//그리기를 마쳤을때 하는 behavior
	if (object == LINE)
	{
		LineObject* temp_line[2];				//클릭할 때 만들어둔 선 두개 받기

		int curline = pDoc->currBox->lines.size() - 1;
		temp_line[0] = pDoc->currBox->lines.at(cur_line - 1);
		temp_line[1] = pDoc->currBox->lines.at(cur_line);

		if (!(temp_line[0]->line[0] == temp_line[0]->line[1] && temp_line[1]->line[0] == temp_line[1]->line[1]))
		{
			//로직 오브젝트 검색 삭제

			int ln = pDoc->currBox->lines.size();

			/////////////////선 속성 추가 ////////////////
			if (temp_line[0]->line[1].x == temp_line[0]->line[0].x)
				temp_line[0]->shape = VERTICAL;
			else
				temp_line[0]->shape = HORIZONTAL;

			if (temp_line[0]->shape == HORIZONTAL)
				temp_line[1]->shape = VERTICAL;
			else temp_line[1]->shape = HORIZONTAL;

			/////////////////////////////점 지우기 & 메모리에 추가////////////////////////////////////


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


			//////////////////////////////////////////선에서 나온순간 잘라버리기/////////////////////////////////////
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
			//////////////////////////////////////////선에서 나온순간 잘라버리기/////////////////////////////////////

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
		SetTimer(0, 10, NULL);
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
				//OUT & INPIN 특별 취급
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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


	//복사할 선 정보받기
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
	//복사할 오브젝트 정보 받기
	for (int i = 0; i < lon; i++)
	{
		//이름 받고 타입 받고 bottom하고 top 받는다.
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

				temp = Gtemp; // delete 용으로 필요함.

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
					//라이브러리 상자에 대한 입력 위치를 찾아준다.
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

					//출력핀은 출력 선이 없다.
					//7 seg도 마찬가지임.

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
// TODO: 여기에 명령 처리기 코드를 추가합니다.

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
	//복사할 선 정보받기

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
	//복사할 오브젝트 정보 받기
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

	//복사하고 지워준다.

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
			//OUT & INPIN 특별 취급
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
