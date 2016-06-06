#include "stdafx.h"
#include "DFlipFlop.h"
#include "LogicSimulator.h"

#include "resource.h"

void DFlipFlop::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_FLIPFLOP_SHADOW));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ
	if (top.x == -INT_MAX) {
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 6 * UNIT, outputCoord[0].first.y - UNIT, 60, 60), 60 * 0, 60 * 0, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	}
	else {
		
	}
	

	delete pBitmap;
}

void DFlipFlop::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_FLIPFLOP));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ
	if (facing == EAST)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 0, 60 * 0, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if (facing == SOUTH)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 1, 60 * 0, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == WEST)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 2, 60 * 0, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == NORTH)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 3, 60 * 0, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;

}


DFlipFlop::DFlipFlop() : FlipFlop()
{
	
}

DFlipFlop::DFlipFlop(int dec_x, int dec_y) : FlipFlop()
{
	objectName = D_FF;
	objectType = FLIPFLOP_TYPE;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
}

DFlipFlop::~DFlipFlop()
{
}
