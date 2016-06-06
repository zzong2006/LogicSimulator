#pragma once
#include "LogicObject.h"
#include "LibraryBox.h"
class Box : public LogicObject
{
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	Box(int x, int y, LibraryBox * LB);
	
	LibraryBox* connectedBox;

	Box();
	~Box();
};

