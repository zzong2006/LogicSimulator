#include "stdafx.h"
#include "Clock.h"
#include "resource.h"
#include "LogicSimulator.h"
#include "CircuitView.h"
#include "MainFrm.h""
#include "LogicSimulatorDoc.h"

void Clock::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_WIRING_SHADOW));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
	if (!output)
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 2 * UNIT, outputCoord[0].first.y - UNIT, 20, 20), 20 * 0, 20 * 0, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 2 * UNIT, outputCoord[0].first.y - UNIT, 20, 20), 20 * 1, 20 * 0, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void Clock::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_WIRING));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
	if (!output)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 20, 20), 20 * 0, 20 * 0, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 20, 20), 20 * 1, 20 * 0, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void Clock::set_Coord_From_outC(int x, int y)
{
	top.x = x - 2 * UNIT; top.y = y - 1 * UNIT;
	bottom.x = x; bottom.y = y + 1 * UNIT;
}

void Clock::turn()
{
}

void Clock::toggleOutput()
{
	output = !output;
}

void Clock::moveCycle()
{

	cycle -= 1;
	
	if (cycle == 0)
	{
		CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

		CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)pFrame->GetActiveDocument();
		CCircuitView *CVCtrl = (CCircuitView *)(pFrame->m_wndSplitterMain.GetPane(0, 1));

		cycle = oriCycle;
		toggleOutput();
		
		pDoc->CheckCircuit();
		CVCtrl->Invalidate();
	}

}

void Clock::setCycle(int input)
{
	cycle = oriCycle = (int)(1000 / input);
}


Clock::Clock() : Wiring()
{
	cycle = 1000;
	oriCycle = cycle;
}

Clock::Clock(int dec_x, int dec_y) :Wiring()
{
	cycle = 1000;
	oriCycle = cycle;

	objectType = WIRING_TYPE;
	objectName = CLOCK;
	
	width = 2;
	height = 2;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);

	outputNum = 1;
	inputNum = 0;
}
Clock::~Clock()
{
}
