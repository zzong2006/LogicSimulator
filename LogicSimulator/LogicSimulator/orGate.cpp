#include "stdafx.h"
#include "orGate.h"

#define UNIT 10

void orGate::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Pen *p;
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);

	gp->DrawBezier(p, outputCoord.x, outputCoord.y,
		outputCoord.x - 1 * UNIT, outputCoord.y - 1 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT);
	gp->DrawBezier(p, outputCoord.x, outputCoord.y,
		outputCoord.x - 1 * UNIT, outputCoord.y + 1 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y + 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	gp->DrawBezier(p, outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y + 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);

	p->SetColor(Gdiplus::Color(255, 0, 0));
	gp->DrawLine(p, outputCoord.x, outputCoord.y, outputCoord.x + 3, outputCoord.y );

	this->set_inputCoord(outputCoord.x, outputCoord.y);


	p->SetColor(Gdiplus::Color(0, 0, 255));
	for (int i = 0; i < inputNum; i++)
	{
		gp->DrawLine(p, inputCoord[i].x, inputCoord[i].y, inputCoord[i].x + 8, inputCoord[i].y);
	}

	delete p;

}

void orGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	gp->DrawBezier(p, outputCoord.x , outputCoord.y,
		outputCoord.x - 1 *UNIT, outputCoord.y - 1 *UNIT,
		outputCoord.x - 4 *UNIT, outputCoord.y - 3 *UNIT,
		outputCoord.x - 5 *UNIT, outputCoord.y - 3 *UNIT);
	gp->DrawBezier(p, outputCoord.x, outputCoord.y,
		outputCoord.x - 1 * UNIT, outputCoord.y + 1 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y + 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	gp->DrawBezier(p, outputCoord.x -5 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y - 3* UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y + 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
}

void orGate::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT; top.y = y - 3 * UNIT;
	bottom.x = x; bottom.y = y + 3 * UNIT;
}

bool orGate::calOutput()
{
	output = input[0] | input[1];

	return output;
}

void orGate::turn()
{
}

orGate::orGate()
{
	inputNum = 2;
}


orGate::~orGate()
{
}
