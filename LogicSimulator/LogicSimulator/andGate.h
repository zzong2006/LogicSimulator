#pragma once
#include "Gate.h"

class andGate : public Gate
{
public:
	void draw_main(Gdiplus::Graphics* gp);
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	bool calOutput(); 
	void turn();
	virtual bool output();
	andGate();
	virtual ~andGate();
};

