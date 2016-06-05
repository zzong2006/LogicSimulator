#include "stdafx.h"
#include "LineObject.h"
#include "name_repo.h"

BOOL LineObject::Is_match_IineCoord(CPoint src)
{
	POINT max, min;
	min.x = MIN(line[0].x, line[1].x);
	min.y = MIN(line[0].y, line[1].y);
	max.x = MAX(line[0].x, line[1].x);
	max.y = MAX(line[0].y, line[1].y);

	CRect area(min.x - 5, min.y - 5, max.x + 5, max.y + 5);

	if (area.PtInRect(src))
		return TRUE;

	return FALSE;
}


/*���� ����...
	Pin(Wiring) �� ����ȉ��� ��� -> �Ķ���
	Pin �� ����ǰ� �Է°��� 1�� ��� -> ���� �ʷϻ�
	Pin �� ����ǰ� �Է°��� 0�� ��� -> ��ο� �ʷϻ�
	
	�������� �������� �˻��ؼ� ���� ���(2����) ��������
*/
void LineObject::draw_main(Gdiplus::Graphics *gp)
{
	Gdiplus::Pen *p;
	Gdiplus::Point drw_line[2];
	BOOL vertical = FALSE;

	if (line[0].x == line[1].x)	//������ ���
		vertical = TRUE;

	drw_line[0].X = MIN(line[0].x, line[1].x);
	drw_line[0].Y = MIN(line[0].y, line[1].y);
	drw_line[1].X = MAX(line[0].x, line[1].x);
	drw_line[1].Y = MAX(line[0].y, line[1].y);

	if (vertical) {
		drw_line[0].Y -= 2;
		drw_line[1].Y += 2;
	}
	else {
		drw_line[0].X -= 2;
		drw_line[1].X += 2;
	}

	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 3);

	switch (state)
	{
	case OFF_SIGNAL:
		p->SetColor(Gdiplus::Color(220, 100, 10));
		break;
	case ON_SIGNAL:
		p->SetColor(Gdiplus::Color(0, 250, 0));
		break;
	default:
		p->SetColor(Gdiplus::Color(120, 120, 124));
		break;

	}

	gp->DrawLines(p, drw_line, 2);

}

//Line �� draw_shadow�� �ʿ� ����.
void LineObject::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{

}

void LineObject::check_connect()
{

}

LineObject::LineObject(int x, int y)
{
	this->line[0] = CPoint(x, y);
	this->line[1] = CPoint(x, y);
	state = OFF_SIGNAL;
}

LineObject::LineObject(CPoint point)
{
	this->line[0] = point;
	this->line[1] = point;
	state = OFF_SIGNAL;
}

LineObject::LineObject(CPoint fpos, CPoint spos)
{
	this->line[0] = fpos;
	this->line[1] = spos;
	state = OFF_SIGNAL;
}

LineObject::~LineObject()
{

}