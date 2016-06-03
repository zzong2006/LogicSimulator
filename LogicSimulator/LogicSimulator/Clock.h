#pragma once

#include "LogicObject.h"
#include "LineObject.h"
#include "Wiring.h"

class Clock : public Wiring
{

protected:
	int state;
	int cycle;			//�ֱ� 1000�� 1����. (�ʱⰪ 1000)
	int oriCycle;		//������ �ֱ� ����Ŭ (cycle�� ��� ���鼭 üũ)
public:
	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);
	void draw_main(Gdiplus::Graphics* gp);
	void set_Coord_From_outC(int x, int y);
	virtual void turn();
	void toggleOutput();

	void moveCycle();	//�ùķ����� ������ ����Ŭ�� ������.

	Clock();
	Clock(int dec_x, int dec_y);
	~Clock();
};

