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
