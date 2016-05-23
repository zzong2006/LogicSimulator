#include "stdafx.h"
#include "LogicObject.h"

#define UNIT 10

void LogicObject::setLabel(CString input)
{
	label = input;
}

void LogicObject::set_outputCoord(int x, int y)
{
	outputCoord.x = x;
	outputCoord.y = y;
}

void LogicObject::set_inputCoord(int x, int y)
{
	if (inputNum == 1)			// NOT GATE
	{
		inputCoord[0].x = x - 3 * UNIT;
		inputCoord[0].y = y * UNIT;
	}
	else {
		inputCoord[0].x = x - 5 * UNIT;
		inputCoord[0].y = y - 1 * UNIT;
		inputCoord[1].x = x - 5 * UNIT;
		inputCoord[1].y = y + 1 * UNIT;
	}
}

LogicObject::LogicObject()
{

}


LogicObject::~LogicObject()
{
}
