#include "stdafx.h"
#include "andGate.h"
#include "Gate.h"
#include "LogicSimulator.h"

bool andGate::output()
{
	return false;
}

void andGate::draw(Graphics* gp, Pen* p)
{
	Point andPts[4];

	andPts[0] = Point(outputCoord.x - 2 * UNIT, outputCoord.y - 2 * UNIT);
	andPts[1] = Point(outputCoord.x - 5 * UNIT, outputCoord.y - 2 * UNIT);
	andPts[2] = Point(outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	andPts[3] = Point(outputCoord.x - 2 * UNIT, outputCoord.y + 3 * UNIT);


	gp->DrawArc(p, outputCoord.x - 5 * UNIT, outputCoord.y - 2 * UNIT, 5 * UNIT, 5 * UNIT, -80, 173);
}

void andGate::turn()
{

}

andGate::andGate() : Gate()
{
	
}


andGate::~andGate()
{
}
