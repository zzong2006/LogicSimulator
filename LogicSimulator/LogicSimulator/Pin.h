#pragma once
#include "LogicObject.h"
#include "Wiring.h"

class Pin : public Wiring
{
protected:
	int state;
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	virtual void turn();
	void toggleOutput();

	Pin();
	Pin(int dec_x, int dec_y);
	~Pin();
};