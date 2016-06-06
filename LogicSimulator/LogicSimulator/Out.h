#pragma once
#include "Wiring.h"

class Out : public Wiring
{
protected :
	int connectedNum;
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	void toggleOutput();
	void setOutput();
	void setConNum(int input);
	int getConNum() { return connectedNum; };

	Out();
	Out(int dec_x, int dec_y);
	Out(int dec_x, int dec_y, int ConNum);
	virtual ~Out();
};

