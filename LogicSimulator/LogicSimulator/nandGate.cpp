#include "stdafx.h"
#include "nandGate.h"
#include "LogicSimulator.h"

#define UNIT 10

void nandGate::draw_main(Gdiplus::Graphics* gp)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE_ORI));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ
	 if (facing == EAST)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 0, 60 * 1, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == SOUTH)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 1, 60 * 1, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == WEST)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 2, 60 * 1, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else  if (facing == NORTH)
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 3, 60 * 1, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void nandGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE_SHADOW));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ

	gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 6 * UNIT, outputCoord[0].first.y - 3 * UNIT, 60, 60), 60 * 0, 60 * 1, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void nandGate::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT; top.y = y - 3 * UNIT;
	bottom.x = x; bottom.y = y + 3 * UNIT;
}

void nandGate::turn()
{

}

void nandGate::set_output()
{
	
}

nandGate::nandGate()
{
	inputNum = 2;
	outputNum = 1;
}

nandGate::nandGate(int dec_x, int dec_y)
{
	inputNum = 2;
	objectName = NAND_GATE;
	objectType = GATE_TYPE;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
	
}

nandGate::~nandGate()
{
}
