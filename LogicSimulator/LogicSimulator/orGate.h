#pragma once

#include "Gate.h"

class orGate : public Gate
{
public:
	void draw_main(Gdiplus::Graphics* gp);
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	bool calOutput();
	void turn();
	orGate();
	~orGate();
};

