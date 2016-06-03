#include "stdafx.h"
#include "notGate.h"
#include "LogicSimulator.h"

#define UNIT 10

void notGate::draw_main(Gdiplus::Graphics* gp)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE_ORI));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
	 if (facing == EAST)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 40, 40), 40 * 0, 60 * 5, 40, 40, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == SOUTH)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 40, 40), 40 * 1, 60 * 5, 40, 40, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == WEST)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 40, 40), 40 * 2, 60 * 5, 40, 40, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == NORTH)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 40, 40), 40 * 3, 60 * 5, 40, 40, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void notGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE_SHADOW));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표

	gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 4 * UNIT, (int)(outputCoord[0].first.y - 2 * UNIT), 40, 40), 40 * 0, 60 * 5, 40, 40, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void notGate::set_Coord_From_outC(int x, int y)
{
	top.x = x - 4 * UNIT; top.y = y - (int)(2 * UNIT);
	bottom.x = x; bottom.y = y + (int)(2 * UNIT);
}

void notGate::turn()
{

}

void notGate::set_output()
{
	
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
	
}

notGate::~notGate()
{
}
