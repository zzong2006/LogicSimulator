#include "stdafx.h"
#include "xorGate.h"
#include "LogicSimulator.h"

#define UNIT 10

void xorGate::draw_main(Gdiplus::Graphics* gp)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE_ORI));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
	 if (facing == EAST)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 0, 60 * 4, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == SOUTH)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 1, 60 * 4, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == WEST)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 2, 60 * 4, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == NORTH)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 3, 60 * 4, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void xorGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE_SHADOW));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표

	gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 6 * UNIT, outputCoord[0].first.y - 3 * UNIT, 60, 60), 60 * 0, 60 * 4, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void xorGate::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT; top.y = y - 3 * UNIT;
	bottom.x = x; bottom.y = y + 3 * UNIT;
}

void xorGate::turn()
{

}

void xorGate::set_output()
{

}

xorGate::xorGate()
{
	inputNum = 2;
	outputNum = 1;
}

xorGate::xorGate(int dec_x, int dec_y)
{
	inputNum = 2;
	objectName = XOR_GATE;
	objectType = GATE_TYPE;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
}

xorGate::~xorGate()
{
}
