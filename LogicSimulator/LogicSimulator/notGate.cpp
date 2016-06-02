#include "stdafx.h"
#include "notGate.h"
#include "LogicSimulator.h"

#define UNIT 10

void notGate::draw_main(Gdiplus::Graphics* gp)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
	if (facing == _T("East"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 30, 30), 30 * 0, 60 * 5, 30, 30, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if (facing == _T("South"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 30, 30), 30 * 1, 60 * 5, 30, 30, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if (facing == _T("West"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 30, 30), 30 * 2, 60 * 5, 30, 30, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if (facing == _T("North"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 30, 30), 30 * 3, 60 * 5, 30, 30, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void notGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Point andPts[4];

	andPts[0] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y - 3 * UNIT);
	andPts[1] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT);
	andPts[2] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	andPts[3] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y + 3 * UNIT);

	gp->DrawArc(p, outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT, 5 * UNIT, 6 * UNIT, 275, 180);
	gp->DrawLines(p, andPts, 4);
}

void notGate::set_Coord_From_outC(int x, int y)
{
	top.x = x - 3 * UNIT; top.y = y - 1.5 * UNIT;
	bottom.x = x; bottom.y = y + 1.5 * UNIT;
}

void notGate::turn()
{

}

void notGate::set_output()
{
	output_line->state = !(input_line[0]->state);
}

notGate::notGate() : Gate()
{
	inputNum = 1;
}

notGate::notGate(int dec_x, int dec_y)
{
	inputNum = 1;
	objectName = NOT_GATE;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
	this->input_line[0] = new LineObject(this->inputCoord[0]);
	this->output_line = new LineObject(this->outputCoord);
}

notGate::~notGate()
{
}
