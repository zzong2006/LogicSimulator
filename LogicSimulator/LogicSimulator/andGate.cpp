#include "stdafx.h"
#include "andGate.h"
#include "LogicSimulator.h"

#define UNIT 10

bool andGate::output()
{
	return false;
}


void andGate::draw(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Point andPts[4];

	andPts[0] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y - 2 * UNIT);
	andPts[1] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y - 2 * UNIT);
	andPts[2] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	andPts[3] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y + 3 * UNIT);

	gp->DrawArc(p, outputCoord.x - 5 * UNIT, outputCoord.y - 2 * UNIT, 5 * UNIT, 5 * UNIT, -80, 173);
	gp->DrawLines(p, andPts, 4);
}

bool andGate::calOutput()
{
	return false;
}

void andGate::turn()
{

}

andGate::andGate()
{
	
}


andGate::~andGate()
{
}
