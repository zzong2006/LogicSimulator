#include "stdafx.h"
#include "Sevenseg.h"
#include "Resource.h"
#include "LogicSimulator.h"

void Sevenseg::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_SEVEN_SEGMENT));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 6 * UNIT, outputCoord[0].first.y - 5 * UNIT, 60, 100), 60 * 0, 100 * 0, 60, 100, Gdiplus::UnitPixel, &imAtt, NULL, NULL);


	delete pBitmap;
}

void Sevenseg::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Bitmap *pBitmap;		//이미지 불러오기
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_SEVEN_SEGMENT));

	Gdiplus::ImageAttributes imAtt;		//이미지 투명 처리
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: 필드상에서 표시될 위치 & 옆의 좌표는 이미지에서 잘라올 좌표
	int horizon = convertBinaryToDecimal(innerOuput[3], innerOuput[2], innerOuput[1], innerOuput[0]);
	int vertical = convertBinaryToDecimal(0, innerOuput[6], innerOuput[5], innerOuput[4]);

	gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 100), 60 * horizon, 100 * vertical, 60, 100, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	
}

void Sevenseg::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT;
	top.y = y - 5 * UNIT;
	bottom.x = x;
	bottom.y = y + 5 * UNIT;
}

int Sevenseg::convertBinaryToDecimal(int a, int b, int c, int d)
{
	int result = a * pow(2, 3);
	result += b * pow(2, 2);
	result += c * pow(2, 1);
	result += d * pow(2, 0);

	return result;
}

void Sevenseg::setOutput()
{
	for (int i = 0; i < 7; i++)
	{
		innerOuput[i] = inputCoord[i].second;
	}
}

Sevenseg::Sevenseg() :Wiring()
{
	outputNum = 0;
	inputNum = 7;
	objectName = SEG7;
}

Sevenseg::Sevenseg(int dec_x, int dec_y) : Wiring()
{
	outputNum = 0;
	inputNum = 7;
	objectName = SEG7;

	for (int i = 0; i < 7; i++)
	{
		innerOuput[i] = 0;
	}

	this->set_inputCoord(dec_x, dec_y);

	width = 5;
	height = 10;
}
Sevenseg::~Sevenseg()
{
}
