#include "stdafx.h"
#include "nandGate.h"
#include "LogicSimulator.h"

#define UNIT 10

void nandGate::draw_main(Gdiplus::Graphics* gp)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_GATE));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ
	if (facing == _T("East"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 0, 60 * 1, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if (facing == _T("South"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 1, 60 * 1, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if (facing == _T("West"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 2, 60 * 1, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);
	else if (facing == _T("North"))
		gp->DrawImage(pBitmap, Gdiplus::Rect(top.x, top.y, 60, 60), 60 * 3, 60 * 1, 60, 60, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void nandGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Point andPts[4];

	andPts[0] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y - 3 * UNIT);
	andPts[1] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT);
	andPts[2] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	andPts[3] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y + 3 * UNIT);

	gp->DrawArc(p, outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT, 5 * UNIT, 6 * UNIT, 275, 180);
	gp->DrawLines(p, andPts, 4);
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
	if (input_line[0]->state == ON_SIGNAL && input_line[1]->state == ON_SIGNAL)
		output_line->state = OFF_SIGNAL;
	else output_line->state = ON_SIGNAL;
}

nandGate::nandGate()
{
	inputNum = 2;
}

nandGate::nandGate(int dec_x, int dec_y)
{
	inputNum = 2;
	objectName = NAND_GATE;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
	this->input_line[0] = new LineObject(this->inputCoord[0]);
	this->input_line[1] = new LineObject(this->inputCoord[1]);
	this->output_line = new LineObject(this->outputCoord);
}

nandGate::~nandGate()
{
}
