#pragma once

#include "LogicObject.h"
#include "LineObject.h"
#include "Wiring.h"

class Clock : public Wiring
{

protected:
	int state;
	int cycle;			//주기 1000이 1초임. (초기값 1000)
	int oriCycle;		//보관용 주기 사이클 (cycle은 계속 빼면서 체크)
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	virtual void turn();
	void toggleOutput();

	void moveCycle();	//시뮬레이터 시작후 사이클을 돌린다.

	Clock();
	Clock(int dec_x, int dec_y);
	~Clock();
};

