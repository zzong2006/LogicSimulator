#include "stdafx.h"
#include "andGate.h"
#include "LogicSimulator.h"

#define UNIT 10

void andGate::draw_main(Gdiplus::Graphics* gp)
{ 
	Gdiplus::Point andPts[4];
	Gdiplus::Pen *p;
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);

	andPts[0] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y - 2 * UNIT);
	andPts[1] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y - 2 * UNIT);
	andPts[2] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	andPts[3] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y + 3 * UNIT);
	
	gp->DrawArc(p, outputCoord.x - 5 * UNIT, outputCoord.y - 2 * UNIT, 5 * UNIT, 5 * UNIT, -80, 173);
	gp->DrawLines(p, andPts, 4);

	p->SetColor(Gdiplus::Color(255, 0, 0));
	gp->DrawLine(p, outputCoord.x, outputCoord.y, outputCoord.x + 3, outputCoord.y);

	this->set_inputCoord(outputCoord.x, outputCoord.y);

	p->SetColor(Gdiplus::Color(0, 0, 255));
	for (int i = 0; i < inputNum; i++)
	{
		gp->DrawEllipse(p, Gdiplus::Rect(inputCoord[i].x - 1, inputCoord[i].y - 1, 2, 2));
	}

	delete p;
}

void andGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Point andPts[4];

	andPts[0] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y - 2 * UNIT);
	andPts[1] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y - 2 * UNIT);
	andPts[2] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	andPts[3] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y + 3 * UNIT);

	gp->DrawArc(p, outputCoord.x - 5 * UNIT, outputCoord.y - 2 * UNIT, 5 * UNIT, 5 * UNIT, -80, 173);

	gp->DrawLines(p, andPts, 4);
}

void andGate::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT; top.y = y - 3 * UNIT;
	bottom.x = x; bottom.y = y + 3 * UNIT;
}

bool andGate::calOutput()
{
	output = input[0] & input[1];

	return output;
}

void andGate::turn()
{

}

andGate::andGate()
{
	inputNum = 2;
}


andGate::~andGate()
{
}
