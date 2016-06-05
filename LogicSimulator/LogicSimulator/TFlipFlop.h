#pragma once
#include "FlipFlop.h"
class TFlipFlop : public FlipFlop
{
public:
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	virtual void draw_main(Gdiplus::Graphics* gp);
	TFlipFlop();
	TFlipFlop(int dec_x, int dec_y);
	virtual ~TFlipFlop();
};
