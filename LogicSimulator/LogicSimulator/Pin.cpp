#include "stdafx.h"
#include "Pin.h"
#include <string>
#include "resource.h"
#include "LogicSimulator.h"

void Pin::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_WIRING_SHADOW));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ
	if (!output)
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 2* UNIT, outputCoord[0].first.y - UNIT, 20, 20), 20 * 0, 20 * 2, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 2* UNIT, outputCoord[0].first.y - UNIT, 20, 20), 20 * 1, 20 * 2, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void Pin::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_WIRING));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ
	if(!output)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 20, 20), 20 * 0, 20 * 2, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 20, 20), 20 * 1, 20 * 2, 20, 20, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void Pin::set_Coord_From_outC(int x, int y)
{
	top.x = x - 2 * UNIT; top.y = y - 1 * UNIT;
	bottom.x = x; bottom.y = y + 1 * UNIT;

}

void Pin::turn()
{

}

void Pin::toggleOutput()
{
	output = !output;
}


Pin::Pin() : Wiring()
{
	width = 2;
	height = 2;

}

Pin::Pin(int dec_x, int dec_y) : Wiring()
{
	objectType = WIRING_TYPE;
	objectName = PIN;

	outputNum = 1;
	inputNum = 0;

	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
}

Pin::~Pin()
{
}
