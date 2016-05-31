#pragma once
#include "LogicObject.h"


class Pin : public LogicObject
{
protected:
	int state;
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	void turn();
	void toggleOutput();
	bool calOutput();	//결과 생성
	Pin();
	~Pin();
};