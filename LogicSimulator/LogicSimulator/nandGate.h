#pragma once
#include "Gate.h"

class nandGate : public Gate
{
public:
	void draw_main(Gdiplus::Graphics* gp);
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void set_Coord_From_outC(int x, int y);
	bool calOutput();
	void turn();
	void set_output();
	nandGate();
	nandGate(int dec_x, int dec_y);
	virtual ~nandGate();
};

