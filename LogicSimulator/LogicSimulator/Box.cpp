#include "stdafx.h"
#include "Box.h"
#include "LibraryBox.h"
#include "resource.h"

void Box::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_LIBRARY));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ
		gp->DrawImage(pBitmap, Gdiplus::Rect(outputCoord[0].first.x - 10 * UNIT, outputCoord[0].first.y - 5 * UNIT, 100, 100), 100 * 1, 100 * 0, 100, 100, Gdiplus::UnitPixel, &imAtt, NULL, NULL);

	delete pBitmap;
}

void Box::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Bitmap *pBitmap;		//�̹��� �ҷ�����
	pBitmap = Gdiplus::Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_LIBRARY));

	Gdiplus::ImageAttributes imAtt;		//�̹��� ���� ó��
	imAtt.SetColorKey(Gdiplus::Color(255, 255, 255), Gdiplus::Color(255, 255, 255), Gdiplus::ColorAdjustTypeBitmap);

	//Rect :: �ʵ�󿡼� ǥ�õ� ��ġ & ���� ��ǥ�� �̹������� �߶�� ��ǥ
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

void Box::setOutput()
{
	//�� ��ȣ�� �ش��ϴ� input�� �� �ڸ��� �־��ش�.
	for (int i = 0; i < inputNum; i++)
	{
		for (int j = 0; j < connectedBox->logicInfo.size(); j++)
		{
			if (connectedBox->logicInfo.at(j)->objectName == PIN) {
				Pin* tempP =(Pin *)connectedBox->logicInfo.at(j);
				if (tempP->getConNum() == i) {
					tempP->output = inputCoord[i].second;
					break;
				}
			}
		}
	}
	//�ѹ� �˰��� �����ش�.
	connectedBox->CheckCircuit();

	//�� ��ȣ�� �ش��ϴ� output�� �� �ڸ��� ��´�.

	for (int i = 0; i < outputNum; i++)
	{
		for (int j = 0; j < connectedBox->logicInfo.size(); j++)
		{
			if (connectedBox->logicInfo.at(j)->objectName == OUTPIN) {
				Out* tempP = (Out *)connectedBox->logicInfo.at(j);
				if (tempP->getConNum() == i) {
					outputCoord[i].second = tempP->output;
					break;
				}
			}
		}
	}
}

Box::Box() : LogicObject()
{
	objectName = BOX;
	objectType = LIB;
}


Box::~Box()
{
}
