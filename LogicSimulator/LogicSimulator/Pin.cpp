#include "stdafx.h"
#include "Pin.h"

//객체가 통합되면 그리는 방식을 수정할 것.
void Pin::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	gp->DrawRectangle(p, connectCoord.x - 2, connectCoord.y - 1, 2, 2);
}

//객체가 통합되면 수정할 것.
void Pin::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Pen *p;
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);
	//draw font 

	delete p;
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
