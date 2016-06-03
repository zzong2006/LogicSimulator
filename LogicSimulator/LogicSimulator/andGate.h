#pragma once
#include "Gate.h"

class andGate : public Gate
{
public:
	void draw_main(Gdiplus::Graphics* gp);
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void set_Coord_From_outC(int x, int y);


	andGate();
	andGate(int dec_x, int dec_y);
	virtual ~andGate();
};

