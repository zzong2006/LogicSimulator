#include "stdafx.h"
#include "Pin.h"

#define UNIT 10

//��ü�� ���յǸ� �׸��� ����� ������ ��.
void Pin::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	p->SetColor(Gdiplus::Color(255, 0, 0));
	gp->DrawRectangle(p, outputCoord.x - 2 *UNIT, outputCoord.y - 1 * UNIT, 2 *UNIT, 2 *UNIT);
}

//��ü�� ���յǸ� ������ ��.
void Pin::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Pen *p;
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);
	//draw font 

	//draw sekel
	gp->DrawRectangle(p, outputCoord.x - 2 * UNIT, outputCoord.y - 1 * UNIT, 2 * UNIT, 2 * UNIT);

	delete p;
}

void Pin::turn()
{
}

bool Pin::calOutput()
{
	return output;
}

Pin::Pin()
{
	output = 0;
}


Pin::~Pin()
{
}
