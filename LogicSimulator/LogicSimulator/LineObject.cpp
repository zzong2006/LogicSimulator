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

/*���� ����...
	Pin(Wiring) �� ����ȉ��� ��� -> �Ķ���
	Pin �� ����ǰ� �Է°��� 1�� ��� -> ���� �ʷϻ�
	Pin �� ����ǰ� �Է°��� 0�� ��� -> ��ο� �ʷϻ�
*/
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
		p->SetColor(Gdiplus::Color(60, 130, 20));
		break;
	case ON_SIGNAL:
		p->SetColor(Gdiplus::Color(50, 250, 60));
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

//Line �� draw_shadow�� �ʿ� ����.
void LineObject::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{

}

void LineObject::check_connect()
{
	for (int i = 0; i < connect_lines.size(); i++)
	{
		connect_lines.at(i)->state = this->state;
	}
}

// �ʱ� ����: ��� ���� �ʱ� ���� 0
LineObject::LineObject()
{
	state = OFF_SIGNAL;
}


LineObject::~LineObject()
{

}