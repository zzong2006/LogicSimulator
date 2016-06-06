#include "stdafx.h"
#include "Gate.h"
#include "LogicSimulator.h"

Gate::Gate() : LogicObject()
{
	outputNum = 1;

	for (int i = 0; i < INPUT_SIZE; i++)
	{
		input_state[i] = 2;
	}

	this->objectType = GATE_TYPE;

}

Gate::Gate(int dec_x, int dec_y)
{
	outputNum = 1;
	
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		input_state[i] = 2;
	}

	this->objectType = GATE_TYPE;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
}

void Gate::draw_main(Gdiplus::Graphics* gp)
{
	Gdiplus::Point andPts[4];
	Gdiplus::Pen *p;
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);
	
}

Gate::~Gate()
{

}
