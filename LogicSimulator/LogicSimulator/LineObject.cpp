#include "stdafx.h"
#include "LineObject.h"

BOOL LineObject::Is_match_IineCoord(CPoint src)
{
	if (src.x >= line[0].x && src.x < line[0].x + 10 && src.y >= line[0].y && src.y < line[0].y + 10)
		return TRUE;
	if (src.x >= line[2].x && src.x < line[2].x + 10 && src.y >= line[2].y && src.y < line[2].y + 10)
		return TRUE;
	return FALSE;
}

void LineObject::move_line(int x, int y, int& move_state)
{
	if (move_state == HTOV)
	{
		line[1].y = line[0].y;
		line[1].x = x;
		line[2].x = line[1].x;
		line[2].y = y;
		if (x == line[0].x)
			move_state = VTOH;
	}
	else if (move_state == VTOH)
	{
		line[1].x = line[0].x;
		line[1].y = y;
		line[2].y = line[1].y;
		line[2].x = x;

		if (y == line[0].y)
			move_state = HTOV;
	}
	else{
		if (y != line[0].y)
			move_state = VTOH;
		else if (x != line[0].x)
			move_state = HTOV;
	}
}

void LineObject::draw_main(Gdiplus::Graphics *gp)
{
	Gdiplus::Pen *p;
	Gdiplus::Point drw_line[3];

	drw_line[0].X = line[0].x; drw_line[0].Y = line[0].y;
	drw_line[1].X = line[1].x; drw_line[1].Y = line[1].y;
	drw_line[2].X = line[2].x; drw_line[2].Y = line[2].y;

	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);
	switch (state)
	{
	case OFF_SIGNAL:
		p->SetColor(Gdiplus::Color(0, 0, 0));
		break;
	case ON_SIGNAL:
		p->SetColor(Gdiplus::Color(255, 0, 0));
		break;
	case INPUT_SIGNAL:
		p->SetColor(Gdiplus::Color(0, 0, 255));
		break;
	case OUTPUT_SIGNAL:
		p->SetColor(Gdiplus::Color(0, 255, 0));
		break;
	}

	gp->DrawLines(p, drw_line, 3);

}
void LineObject::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	//gp->DrawRectangle(p, outputCoord.x - 2 * UNIT, outputCoord.y - 1 * UNIT, 2 * UNIT, 2 * UNIT);
}

void LineObject::check_connect()
{
	for (int i = 0; i < connect_lines.size(); i++)
	{
		connect_lines.at(i)->state = this->state;
	}
}

/*void LineObject::connect_object(CLogicSimulatorDoc* pDoc, CPoint point)
{
	for (int i = 0; i < pDoc->gateInfo.size(); i++)
	{
		int index = 0;
		if (pDoc->gateInfo.at(i)->Is_match_outputCoord(point))
		{
			pDoc->gateInfo.at(i)->output_line = this;
		}
		else if ((index = pDoc->gateInfo.at(i)->Is_match_inputCoord(point)) != -1)
		{
			pDoc->gateInfo.at(i)->input_line[index] = this;
		}
	}

	for (int i = 0; i < pDoc->pinInfo.size(); i++)
	{
		if (pDoc->pinInfo.at(i)->Is_match_outputCoord(point))
		{
			pDoc->pinInfo.at(i)->connect_line = this;
		}
	}
}*/

LineObject::LineObject(int x, int y)
{
	this->line[0] = CPoint(x, y);
	this->line[1] = CPoint(x, y);
	this->line[2] = CPoint(x, y);
	state = OFF_SIGNAL;
}

LineObject::LineObject(CPoint point)
{
	this->line[0] = point;
	this->line[1] = point;
	this->line[2] = point;
	state = OFF_SIGNAL;
}


LineObject::~LineObject()
{
}