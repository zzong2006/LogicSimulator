#include "stdafx.h"
#include "orGate.h"

#define UNIT 10

void orGate::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Pen *p;
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);

	gp->DrawBezier(p, outputCoord.x, outputCoord.y,
		outputCoord.x - 1 * UNIT, outputCoord.y - 1 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT);
	gp->DrawBezier(p, outputCoord.x, outputCoord.y,
		outputCoord.x - 1 * UNIT, outputCoord.y + 1 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y + 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	gp->DrawBezier(p, outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y + 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);

	p->SetColor(Gdiplus::Color(255, 0, 0));
	gp->DrawLine(p, outputCoord.x, outputCoord.y, outputCoord.x + 3, outputCoord.y);

	this->set_inputCoord(outputCoord.x, outputCoord.y);


	p->SetColor(Gdiplus::Color(0, 0, 255));
	for (int i = 0; i < inputNum; i++)
	{
		gp->DrawLine(p, inputCoord[i].x, inputCoord[i].y, inputCoord[i].x + 8, inputCoord[i].y);
	}

	delete p;

}

void orGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	gp->DrawBezier(p, outputCoord.x, outputCoord.y,
		outputCoord.x - 1 * UNIT, outputCoord.y - 1 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT);
	gp->DrawBezier(p, outputCoord.x, outputCoord.y,
		outputCoord.x - 1 * UNIT, outputCoord.y + 1 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y + 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	gp->DrawBezier(p, outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y - 3 * UNIT,
		outputCoord.x - 4 * UNIT, outputCoord.y + 3 * UNIT,
		outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
}

void orGate::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT; top.y = y - 3 * UNIT;
	bottom.x = x; bottom.y = y + 3 * UNIT;
}

void orGate::turn()
{

}

orGate::orGate() : Gate()
{
	inputNum = 2;
}

orGate::orGate(int dec_x, int dec_y)
{
	inputNum = 2;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
	this->input_line[0] = new LineObject(this->inputCoord[0]);
	this->input_line[1] = new LineObject(this->inputCoord[1]);
	this->output_line = new LineObject(this->outputCoord);
}


orGate::~orGate()
{
}
