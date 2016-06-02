#include "stdafx.h"
#include "orGate.h"
#include "resource.h"
#include "name_repo.h"
#define UNIT 10

void orGate::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE_ORI));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ
	if(facing == _T("East"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 0, 60 * 3, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if(facing == _T("South"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 1, 60 * 3, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if(facing == _T("West"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 2, 60 * 3, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if(facing == _T("North"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 3, 60 * 3, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;

}

void orGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE_SHADOW));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ

	gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord.x - 6 * UNIT, outputCoord.y - 3 * UNIT, 60, 60), 60 * 0, 60 * 3, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void orGate::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT; top.y = y - 3 * UNIT;
	bottom.x = x; bottom.y = y + 3 * UNIT;
}

void orGate::turn()
{

}


orGate::orGate() : Gate()
{
	inputNum = 2;
}

orGate::orGate(int dec_x, int dec_y)
{
	inputNum = 2;
	objectName = OR_GATE;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
	this->input_line[0] = new LineObject(this->inputCoord[0]);
	this->input_line[1] = new LineObject(this->inputCoord[1]);
	this->output_line = new LineObject(this->outputCoord);
}


orGate::~orGate()
{
}
