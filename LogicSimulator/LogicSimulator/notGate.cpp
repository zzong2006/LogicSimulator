#include "stdafx.h"
#include "notGate.h"
#include "LogicSimulator.h"

#define UNIT 10

void notGate::draw_main(Gdiplus::Graphics* gp)
{
	Gdiplus::Point andPts[4];
	Gdiplus::Pen *p;
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);

	andPts[0] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y - 3 * UNIT);
	andPts[1] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT);
	andPts[2] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	andPts[3] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y + 3 * UNIT);

	gp->DrawArc(p, outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT, 5 * UNIT, 6 * UNIT, 275, 180);
	gp->DrawLines(p, andPts, 4);

	p->SetColor(Gdiplus::Color(255, 0, 0));
	gp->DrawLine(p, outputCoord.x, outputCoord.y, outputCoord.x + 3, outputCoord.y);

	p->SetColor(Gdiplus::Color(0, 0, 255));
	for (int i = 0; i < inputNum; i++)
	{
		gp->DrawEllipse(p, Gdiplus::Rect(inputCoord[i].x - 1, inputCoord[i].y - 1, 2, 2));
	}

	delete p;
}

void notGate::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	Gdiplus::Point andPts[4];

	andPts[0] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y - 3 * UNIT);
	andPts[1] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT);
	andPts[2] = Gdiplus::Point(outputCoord.x - 5 * UNIT, outputCoord.y + 3 * UNIT);
	andPts[3] = Gdiplus::Point(outputCoord.x - 2 * UNIT, outputCoord.y + 3 * UNIT);

	gp->DrawArc(p, outputCoord.x - 5 * UNIT, outputCoord.y - 3 * UNIT, 5 * UNIT, 6 * UNIT, 275, 180);
	gp->DrawLines(p, andPts, 4);
}

void notGate::set_Coord_From_outC(int x, int y)
{
	top.x = x - 6 * UNIT; top.y = y - 3 * UNIT;
	bottom.x = x; bottom.y = y + 3 * UNIT;
}

void notGate::turn()
{

}

void notGate::set_output()
{
	if (input_line[0]->state == ON_SIGNAL)
		output_line->state = OFF_SIGNAL;
	else output_line->state = ON_SIGNAL;
}

notGate::notGate()
{
	inputNum = 1;
}

notGate::notGate(int dec_x, int dec_y)
{
	inputNum = 1;
	this->set_outputCoord(dec_x, dec_y);
	this->set_inputCoord(dec_x, dec_y);
	this->input_line[0] = new LineObject(this->inputCoord[0]);
	this->output_line = new LineObject(this->outputCoord);
}

notGate::~notGate()
{
}
