#include "stdafx.h"
#include "Gate.h"
#include "LogicSimulator.h"

Gate::Gate() : LogicObject()
{
	//test input_size
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		input_state[i] = 2;
	}

	this->objectType = GATE_TYPE;

}

Gate::Gate(int dec_x, int dec_y)
{
	//test input_size
	for (int i = 0; i < INPUT_SIZE; i++)
	{
		input_state[i] = 2;
	}

	this->objectType = GATE_TYPE;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
}

//주어진 input state을 활용해 output line state를 설정한다.
void Gate::setOutput()
{
	switch (objectName) {
	case AND_GATE:
		output_line->state =
			input_line[0]->state & input_line[1]->state;
		break;
	case OR_GATE:
		output_line->state =
			input_line[0]->state | input_line[1]->state;
		break;
	case NOT_GATE:
		output_line->state =
			!(input_line[0]->state);
		break;
	case NAND_GATE :
		output_line->state =
			!(input_line[0]->state & input_line[1]->state);
		break;
	case NOR_GATE :
		output_line->state =
			!(input_line[0]->state | input_line[1]->state);
		break;
	case XOR_GATE :
		output_line->state = 
			input_line[0]->state^input_line[1]->state;
		break;
	}
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
