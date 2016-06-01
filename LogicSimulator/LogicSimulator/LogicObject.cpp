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

int LogicObject::Is_match_inputCoord(POINT src)
{
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		if (src.x >= inputCoord[i].x && src.x < inputCoord[i].x + 10
			&& src.y >= inputCoord[i].y && src.y < inputCoord[i].y + 10)
			return i;
	}
	return -1;
}

BOOL LogicObject::Is_match_outputCoord(POINT src)
{
	if (src.x >= outputCoord.x && src.x < outputCoord.x + 10
		&& src.y >= outputCoord.y && src.y < outputCoord.y + 10)
		return TRUE;
	return FALSE;
}

int LogicObject::Is_match_inputCoord(int src_x, int src_y)
{
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		if (src_x >= inputCoord[i].x && src_x < inputCoord[i].x + 10
			&& src_y >= inputCoord[i].y && src_y < inputCoord[i].y + 10)
			return i;
	}
	return -1;
}

BOOL LogicObject::Is_match_outputCoord(int src_x, int src_y)
{
	if (src_x >= outputCoord.x && src_x < outputCoord.x + 10
		&& src_y >= outputCoord.y && src_y < outputCoord.y + 10)
		return TRUE;
	return FALSE;
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
