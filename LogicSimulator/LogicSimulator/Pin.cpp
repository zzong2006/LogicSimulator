#include "stdafx.h"
#include "Pin.h"
#include <string>
#define UNIT 10

//객체가 통합되면 그리는 방식을 수정할 것.
void Pin::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	gp->DrawRectangle(p, outputCoord.x - 2 *UNIT, outputCoord.y - 1 * UNIT, 2 *UNIT, 2 *UNIT);
}

//객체가 통합되면 수정할 것.
void Pin::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Pen *p;
	CString out;
	Gdiplus::SolidBrush B(Gdiplus::Color(160, 130, 20));
	out.Format(_T("%d"), output);
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);

	if(output)
		B.SetColor(Gdiplus::Color(100, 200, 40));

	//draw inner circle
	gp->FillEllipse(&B, outputCoord.x - 1.8 * UNIT, outputCoord.y - (int)(0.8 * UNIT) , 1.5 * UNIT, 1.5 * UNIT);

	//draw font 
	B.SetColor(Gdiplus::Color(255, 255, 255));
	Gdiplus::Font F(L"Arial", 10);
	Gdiplus::PointF P(outputCoord.x - UNIT * 1.5,outputCoord.y- UNIT * 0.8);
	
	gp->DrawString(out, -1, &F, P, &B);

	//draw sekel
	gp->DrawRectangle(p, outputCoord.x - 2 * UNIT, outputCoord.y - 1 * UNIT, 2 * UNIT, 2 * UNIT);

	delete p;
}

void Pin::set_Coord_From_outC(int x, int y)
{
	top.x = x- 2 * UNIT; top.y = y -1 *UNIT;
	bottom.x = x; bottom.y = y + 1 * UNIT;

}

void Pin::turn()
{
}

void Pin::toggleOutput()
{
	output = !output;
}

bool Pin::calOutput()
{
	return output;
}

Pin::Pin() : LogicObject()
{
	width = 2 ;
	height = 2 ;
}


Pin::~Pin()
{
}
