#include "LogicObject.h"
#pragma once

class Pin : public LogicObject
{
protected:
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void turn();
	bool calOutput();	//결과 생성
	Pin();
	~Pin();
};

