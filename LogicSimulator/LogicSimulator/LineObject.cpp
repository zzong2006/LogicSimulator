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

/*선의 색은...
	Pin(Wiring) 과 연결안됬을 경우 -> 파란색
	Pin 과 연결되고 입력값이 1일 경우 -> 밝은 초록색
	Pin 과 연결되고 입력값이 0일 경우 -> 어두운 초록색
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

//Line 은 draw_shadow가 필요 없음.
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

// 초기 설정: 모든 선의 초기 값은 0
LineObject::LineObject()
{
	state = OFF_SIGNAL;
}


LineObject::~LineObject()
{

}