#pragma once
#include "LogicObject.h"

class FlipFlop : public LogicObject
{

protected:
	BOOL UpTrigger;		//ÃÊ±â ¼³Á¤ »ó½Â¿§Áö
public:
	virtual void set_Coord_From_outC(int x, int y);
	FlipFlop();
	~FlipFlop();
};

