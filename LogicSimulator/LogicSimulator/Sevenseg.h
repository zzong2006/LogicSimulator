#pragma once
#include "Wiring.h"

class Sevenseg : public Wiring
{
protected:
	int innerOuput[7];
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	//abcd��� 2������ 10���� ������ �ٲ۴�.
	int convertBinaryToDecimal(int a , int b, int c, int d);
	void setOutput();

	Sevenseg();
	Sevenseg(int dec_x, int dec_y);
	~Sevenseg();
};

