#include "stdafx.h"
#include "LogicObject.h"
#include "name_repo.h"
#include "CircuitView.h"
#include "LogicSimulatorDoc.h"
#include "MainFrm.h"

#define UNIT 10

void LogicObject::showSelected(Gdiplus::Graphics * gp)
{
	Gdiplus::Pen *p;
	p = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);

	gp->DrawRectangle(p, top.x - 2, top.y - 2, 4, 4);
	gp->DrawRectangle(p, bottom.x - 2, top.y - 2, 4, 4);
	gp->DrawRectangle(p, top.x - 2, bottom.y - 2, 4, 4);
	gp->DrawRectangle(p, bottom.x - 2, bottom.y - 2, 4, 4);

	delete p;
}

void LogicObject::showLabel(Gdiplus::Graphics * gp)
{
	Gdiplus::Font F(L"Arial", 15, Gdiplus::FontStyleRegular,Gdiplus::UnitPixel);
	Gdiplus::PointF P((top.x + bottom.x) / 2, top.y - (UNIT * 1.5));
	Gdiplus::SolidBrush B(Gdiplus::Color(0, 0, 0));
	Gdiplus::StringFormat SF;

	SF.SetAlignment(Gdiplus::StringAlignmentCenter);
	SF.SetLineAlignment(Gdiplus::StringAlignmentCenter);


	gp->DrawString(label,-1,&F,P,&SF,&B);
}

void LogicObject::setLabel(CString input)
{
	label = input;
}

void LogicObject::setFacing(CString input)
{
	if (input == "East") 
		facing = EAST;
	else if (input == "West") 
		facing = WEST;
	else if (input == "South") 
		facing = SOUTH;
	else if (input == "North") 
		facing = NORTH;

}

void LogicObject::set_Coord_ByFacing(CString input)
{
	CLogicSimulatorDoc *pDoc = (CLogicSimulatorDoc *)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();


	switch (objectType) {
	case GATE_TYPE:
		if (objectName != NOT_GATE) {
			switch (facing)
			{
			case EAST:
				inputCoord[0].first.x = inputCoord[1].first.x = top.x;
				inputCoord[0].first.y = top.y + 2 * UNIT;
				inputCoord[1].first.y = top.y + 4 * UNIT;
				outputCoord[0].first.x = bottom.x;
				outputCoord[0].first.y = bottom.y - 3 * UNIT;
				break;
			case WEST:
				inputCoord[0].first.x = inputCoord[1].first.x = bottom.x;
				inputCoord[0].first.y = bottom.y - 2 * UNIT;
				inputCoord[1].first.y = bottom.y - 4 * UNIT;
				outputCoord[0].first.x = top.x;
				outputCoord[0].first.y = top.y + 3 * UNIT;
				break;
			case SOUTH:
				inputCoord[0].first.y = inputCoord[1].first.y = top.y;
				inputCoord[0].first.x = top.x + 2 * UNIT;
				inputCoord[1].first.x = top.x + 4 * UNIT;
				outputCoord[0].first.y = bottom.y;
				outputCoord[0].first.x = bottom.x - 3 * UNIT;
				break;
			case NORTH:
				inputCoord[0].first.y = inputCoord[1].first.y = bottom.y;
				inputCoord[0].first.x = bottom.x - 2 * UNIT;
				inputCoord[1].first.x = bottom.x - 4 * UNIT;
				outputCoord[0].first.x = top.x + 3 *UNIT;
				outputCoord[0].first.y = top.y ;
				break;
			}
		}
		else {
			switch (facing)
			{
			case EAST:
				inputCoord[0].first.x = top.x;
				inputCoord[0].first.y = top.y - 2 * UNIT;
				outputCoord[0].first.x = bottom.x;
				outputCoord[0].first.y = bottom.y - 2  * UNIT;
				break;
			case WEST:
				inputCoord[0].first.x = bottom.x;
				inputCoord[0].first.y = bottom.y - 2 * UNIT;
				outputCoord[0].first.x = top.x;
				outputCoord[0].first.y = top.y +2  * UNIT;
				break;
			case SOUTH:
				inputCoord[0].first.x = top.x + 2 * UNIT;
				inputCoord[0].first.y = top.y;
				outputCoord[0].first.x = bottom.x - 2 * UNIT;
				outputCoord[0].first.y = bottom.y;
				break;
			case NORTH:
				inputCoord[0].first.x = bottom.x - 2 * UNIT;
				inputCoord[0].first.y = bottom.y;
				outputCoord[0].first.x = top.x + 2 * UNIT;
				outputCoord[0].first.y = top.y;
				break;
			}
		}
		break;
	case WIRING_TYPE:
		switch (facing)
		{
		case EAST:
			inputCoord[0].first.x = inputCoord[1].first.x = top.x;
			inputCoord[0].first.y = top.y - 2 * UNIT;
			inputCoord[1].first.y = top.y - 4 * UNIT;
			outputCoord[0].first.x = bottom.x;
			outputCoord[0].first.y = bottom.y - 3 * UNIT;
			break;
		case WEST:
			break;
		case SOUTH:
			break;
		case NORTH:
			break;
		}
		break;
	case FLIPFLOP_TYPE:
		break;
	}
	//로직 오브젝트의 입출력 좌표를 이용해
	//이미 현재 오브젝트에 상속된 line의 좌표를 끊고 새로 만든다.
}

void LogicObject::move_Coord(long &x, long &change)
{
	if ((x > 0 && change > 0) || (x <0 && change <0))
		x = change; 
	if (x > 0 && change < 0)
		x += change;
	if (x < 0 && change > 0)
		x -= change;
}

void LogicObject::set_outputCoord(int x, int y)
{
	outputCoord[0].first.x = x;
	outputCoord[0].first.y = y;
}

void LogicObject::set_inputCoord(int x, int y)
{
	switch (objectType) {
	case GATE_TYPE:
		if (objectName == NOT_GATE)
		{
			inputCoord[0].first.x = x - 4 * UNIT;
			inputCoord[0].first.y = y;
		}else {
			inputCoord[0].first.x = x - 6 * UNIT;
			inputCoord[0].first.y = y - 1 * UNIT;
			inputCoord[1].first.x = x - 6 * UNIT;
			inputCoord[1].first.y = y + 1 * UNIT;
		}
		break;
	case WIRING_TYPE:
		inputCoord[0].first.x = x;
		inputCoord[0].first.y = y;
	}
	
}

void LogicObject::toggleOutput()
{
	output = !output;
}

POINT LogicObject::get_top() const
{
	return top;
}

POINT LogicObject::get_bottm() const
{
	return bottom;
}


int LogicObject::isInputSet() const
{
	for (int i = 0; i < inputNum; i++)
	{
		if (inputCoord[i].second == -1)
			return 0;
	}
		return 1;
}

LogicObject::LogicObject()
{
	output = 0;
	facing = EAST;
}


LogicObject::~LogicObject()
{

}
