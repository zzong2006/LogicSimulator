#include "stdafx.h"
#include "Out.h"
#include "resource.h"
#include "LogicSimulator.h"

void Out::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_WIRING_SHADOW));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
	if (!output)
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord.x - 2 * UNIT, outputCoord.y - UNIT, 20, 20), 20 * 0, 20 * 1, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord.x - 2 * UNIT, outputCoord.y - UNIT, 20, 20), 20 * 1, 20 * 1, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void Out::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_WIRING));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
	if (!output)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 20, 20), 20 * 0, 20 * 1, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 20, 20), 20 * 1, 20 * 1, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
}

void Out::set_Coord_From_outC(int x, int y)
{
	top.x = x - 2 * UNIT; top.y = y - 1 * UNIT;
	bottom.x = x; bottom.y = y + 1 * UNIT;
}

void Out::turn()
{
}

void Out::toggleOutput()
{
	output = !output;
}

void Out::setOutput()
{
	output = input_line[0]->state;
}

int Out::getOutput()
{
	return output;
}

Out::Out() : Wiring()
{
	width = 2;
	height = 2;
}

Out::Out(int dec_x, int dec_y) : Wiring()
{
	inputNum = 1;

	objectType = WIRING_TYPE;
	objectName = OUTPIN;

	this->set_inputCoord(dec_x, dec_y);
	this->input_line[0] = new LineObject(dec_x,dec_y);

	width = 2;
	height = 2;
}


Out::~Out()
{
}
