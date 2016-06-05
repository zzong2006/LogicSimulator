#pragma once
#include "LogicObject.h"

class FlipFlop : public LogicObject
{
public:
	virtual void set_Coord_From_outC(int x, int y);
	FlipFlop();
	~FlipFlop();
};

