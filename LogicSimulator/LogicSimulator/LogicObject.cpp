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
		inputCoord[0].x = x - 5 * UNIT;
		inputCoord[0].y = y;
	}
	else {
		inputCoord[0].x = x - 5 * UNIT;
		inputCoord[0].y = y - 1 * UNIT;
		inputCoord[1].x = x - 5 * UNIT;
		inputCoord[1].y = y + 1 * UNIT;
	}
}

void LogicObject::toggleOutput()
{
	output = !output;
}

POINT LogicObject::get_top() const
{
	return top;
}

POINT LogicObject::get_bottm() const
{
	return bottom;
}


int LogicObject::isInputSet() const
{
	int sum = 0;

	for (int i = 0; i < inputNum; i++)
	{
		if (input_line[i]->chk)
			sum++;
	}
	if (sum == inputNum)
		return 1;

	return 0;
}

LogicObject::LogicObject()
{
	output = 0;
}


LogicObject::~LogicObject()
{
}
