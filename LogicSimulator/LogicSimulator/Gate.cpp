#include "stdafx.h"
#include "Gate.h"
#include "LogicSimulator.h"

Gate::Gate()
{
	//test input_size
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		input_state[i] = 2;
	}

	output_state = OUTPUT_SIGNAL;
	type.SetString(_T("Gate"));
	this->objectType = GATE_TYPE;
}

Gate::Gate(int dec_x, int dec_y)
{
	//test input_size
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		input_state[i] = 2;
	}

	output_state = OUTPUT_SIGNAL;
	type.SetString(_T("Gate"));
	this->objectType = GATE_TYPE;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
}

int Gate::getOutput()
{
	return 0;
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
