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

Gate::~Gate()
{

}
