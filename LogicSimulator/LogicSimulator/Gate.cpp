#include "stdafx.h"
#include "Gate.h"
#include "LogicSimulator.h"

Gate::Gate()
{
	outputCoord.x = 0;
	outputCoord.y = 0;
}

void Gate::addLabel(CString input)
{
	label = input;
}

void Gate::set_outputCoord(int x, int y)
{
	outputCoord.x = x;
	outputCoord.y = y;
}

void Gate::set_inputCoord(int x, int y)
{
	if (inputNum == 1)			// NOT GATE
	{
		inputCoord[0].x = x - 3 *UNIT;
		inputCoord[0].y = y * UNIT;
	} else {
		inputCoord[0].x = x - 5 *UNIT;
		inputCoord[0].y = y - 1 * UNIT;
		inputCoord[1].x = x - 5 * UNIT;
		inputCoord[1].y = y + 2 * UNIT;
	}
}

Gate::~Gate()
{

}
