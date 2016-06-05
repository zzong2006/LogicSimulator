#pragma once
#include "LogicObject.h"
#include "FlipFlop.h"
class DFlipFlop : public FlipFlop
{
public:
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	virtual void draw_main(Gdiplus::Graphics* gp);
	DFlipFlop();
	DFlipFlop(int dec_x, int dec_y);
	virtual ~DFlipFlop();
};

