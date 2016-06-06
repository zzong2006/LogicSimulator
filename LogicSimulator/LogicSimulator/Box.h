#pragma once
#include "LogicObject.h"
class LibraryBox;

class Box : public LogicObject
{
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	Box(int x, int y, LibraryBox * LB);

	void setOutput();
	
	LibraryBox* connectedBox;

	Box();
	~Box();
};

