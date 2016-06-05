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

//�־��� input state�� Ȱ���� output line state�� �����Ѵ�.
void Gate::setOutput()
{
	switch (objectName) {
	case AND_GATE:
		outputCoord[0].second = inputCoord[0].second && inputCoord[1].second;
		break;
	case OR_GATE:
		outputCoord[0].second = inputCoord[0].second || inputCoord[1].second;
		break;
	case NOT_GATE:
		outputCoord[0].second = !inputCoord[0].second;
		break;
	case NAND_GATE :
		outputCoord[0].second = !(inputCoord[0].second && inputCoord[1].second);
		break;
	case NOR_GATE :
		outputCoord[0].second = (inputCoord[0].second || inputCoord[1].second);
		break;
	case XOR_GATE :
		outputCoord[0].second = inputCoord[0].second ^ inputCoord[1].second;
		break;
	}
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
