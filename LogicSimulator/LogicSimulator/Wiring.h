#pragma once
#include "LogicObject.h"
class Wiring : public LogicObject
{
public:
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;
	virtual void turn() = 0;
	virtual void set_Coord_From_outC(int x, int y) = 0;

	Wiring();
	virtual ~Wiring();
};

