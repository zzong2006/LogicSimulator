#pragma once

#include "Gate.h"

class notGate : public Gate
{
public:
	void draw_main(Gdiplus::Graphics* gp);
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void set_Coord_From_outC(int x, int y);
	bool calOutput();
	void set_output();
	void turn();
	notGate();
	notGate(int dec_x, int dec_y);
	~notGate();
};

