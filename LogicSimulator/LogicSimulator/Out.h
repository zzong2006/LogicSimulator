#pragma once
#include "Wiring.h"

class Out : public Wiring
{
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	void turn();
	void toggleOutput();
	void setOutput();
	int getOutput();

	Out();
	Out(int dec_x, int dec_y);
	virtual ~Out();
};

