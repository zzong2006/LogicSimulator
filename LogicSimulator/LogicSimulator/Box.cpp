#include "stdafx.h"
#include "Box.h"
#include "resource.h"

void Box::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_LIBRARY));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 10 * UNIT, outputCoord[0].first.y - 5 * UNIT, 100, 100), 100 * 1, 100 * 0, 100, 100, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void Box::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_LIBRARY));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 100, 100), 100 * 0, 100 * 0, 100, 100, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void Box::set_Coord_From_outC(int x, int y)
{
	top.x = x - 10 * UNIT;
	top.y = y - 5 * UNIT;
	bottom.x = x;
	bottom.y = y + 5 * UNIT;
}

Box::Box(int x, int y, LibraryBox* LB) : LogicObject()
{
	connectedBox = LB;
	objectName = BOX;
	objectType = LIB;
	inputNum = connectedBox->NumInput;
	outputNum = connectedBox->NumOuput;

	this->set_outputCoord(x, y);
	this->set_inputCoord(x, y);
}

Box::Box() : LogicObject()
{
	objectName = BOX;
	objectType = LIB;
}


Box::~Box()
{
}
