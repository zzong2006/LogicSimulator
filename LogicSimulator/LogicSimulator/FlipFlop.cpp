#include "stdafx.h"
#include "FlipFlop.h"
#include "name_repo.h"

void FlipFlop::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT; top.y = y - 3 * UNIT;
	bottom.x = x; bottom.y = y + 3 * UNIT;
}

FlipFlop::FlipFlop() : LogicObject()
{
	objectType = FLIPFLOP_TYPE;
	UpTrigger = TRUE;

	outputNum = 2;			//Q , Q'

	if (objectName == JK_FF)
		inputNum = 2;
	else
		inputNum = 1;
}


FlipFlop::~FlipFlop()
{
}
