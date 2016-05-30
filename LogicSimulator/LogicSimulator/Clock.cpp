#include "stdafx.h"
#include "Clock.h"
#include "resource.h"

void Clock::draw_shadow(Gdiplus::Graphics * gp, Gdiplus::Pen * p)
{
	gp->DrawRectangle(p, outputCoord.x - 2 * UNIT, outputCoord.y - 1 * UNIT, 2 * UNIT, 2 * UNIT);
}

void Clock::draw_main(Gdiplus::Graphics * gp)
{
	Gdiplus::Pen *p;
	Gdiplus::SolidBrush B(Gdiplus::Color(160, 130, 20));
	
	//출력 기본값을 FALSE으로 생각하여 검은녹색으로 설정
	p = new Gdiplus::Pen(Gdiplus::Color(160, 130, 20), 2);

	//tempPT : ㄹ 자 모양 포인터를 받을 배열
	Gdiplus::Point tempPT[6] = { Gdiplus::Point(top.x + 5, top.y + 5),
		Gdiplus::Point(top.x + 5, top.y + 15),
		Gdiplus::Point(top.x + 10, top.y + 15),
		Gdiplus::Point(top.x + 10, top.y + 5),
		Gdiplus::Point(top.x + 15, top.y + 5),
		Gdiplus::Point(top.x + 15, top.y + 15)
	};

	//결과값이 TRUE일때 초록색 출력 및 ㄹ 생김새 변화
	if (output) {
		p->SetColor(Gdiplus::Color(100, 200, 40));
		tempPT[0].Y += UNIT; tempPT[1].Y -= UNIT;	tempPT[2].Y -= UNIT;
		tempPT[3].Y += UNIT;	tempPT[4].Y += UNIT;	tempPT[5].Y -= UNIT;
	}
	
	gp->DrawLines(p, tempPT, 6);

	//겉모양 그리기
	p->SetColor(Gdiplus::Color(0, 0, 0));
	gp->DrawRectangle(p, outputCoord.x - 2 * UNIT, outputCoord.y - 1 * UNIT, 2 * UNIT, 2 * UNIT);

	delete p;
}

void Clock::set_Coord_From_outC(int x, int y)
{
	top.x = x - 2 * UNIT; top.y = y - 1 * UNIT;
	bottom.x = x; bottom.y = y + 1 * UNIT;
}

void Clock::turn()
{
}

void Clock::toggleOutput()
{
}

bool Clock::calOutput()
{
	return false;
}

Clock::Clock()
{
}


Clock::~Clock()
{
}
