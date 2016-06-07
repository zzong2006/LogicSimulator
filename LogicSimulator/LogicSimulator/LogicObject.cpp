#include "stdafx.h"
#include "LogicObject.h"
#include "name_repo.h"
#include "CircuitView.h"
#include "LogicSimulatorDoc.h"
#include "MainFrm.h"

#define UNIT 10

void LogicObject::drawLine(Gdiplus::Graphics * gp)
{
	for (int i = 0; i < inputNum; i++)
	{

	}
	for (int i = 0; i < outputNum; i++)
	{

	}
}

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

void LogicObject::setFacing(int input)
{
	facing = input;
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
		if (objectName == SEG7)
		{
			switch (facing)
			{
			case EAST:
				for (int i = 0; i < 7; i++)
				{
					inputCoord[i].first.x = bottom.x;
					inputCoord[i].first.y = top.y + 2 * UNIT + i * UNIT;
				}
				break;
			case WEST:
				for (int i = 0; i < 7; i++)
				{
					inputCoord[i].first.x = top.x;
					inputCoord[i].first.y = top.y + 2 * UNIT + i * UNIT;
				}
				break;
			case SOUTH:
				for (int i = 0; i < 7; i++)
				{
					inputCoord[i].first.x = top.x + i * UNIT;
					inputCoord[i].first.y = bottom.y;
				}
				break;
			case NORTH:
				for (int i = 0; i < 7; i++)
				{
					inputCoord[i].first.x = top.x + i * UNIT;
					inputCoord[i].first.y = top.y;
				}
				break;

			}
		}
		else if(objectName == OUTPIN) {
			switch (facing)
			{
			case EAST:
				inputCoord[0].first.x = bottom.x;
				inputCoord[0].first.y = bottom.y - UNIT;
				break;
			case WEST:
				inputCoord[0].first.x = top.x;
				inputCoord[0].first.y = top.y + UNIT;
				break;
			case SOUTH:
				inputCoord[0].first.x = bottom.x - UNIT;
				inputCoord[0].first.y = bottom.y;
				break;
			case NORTH:
				inputCoord[0].first.x = top.x + UNIT;
				inputCoord[0].first.y = top.y;
				break;

			}
		}
		else {
			switch (facing)
			{
			case EAST:
				outputCoord[0].first.x = bottom.x;
				outputCoord[0].first.y = bottom.y - UNIT;
				break;
			case WEST:
				outputCoord[0].first.x = top.x;
				outputCoord[0].first.y = top.y + UNIT;
				break;
			case SOUTH:
				outputCoord[0].first.x = bottom.x - UNIT;
				outputCoord[0].first.y = bottom.y;
				break;
			case NORTH:
				outputCoord[0].first.x = top.x + UNIT;
				outputCoord[0].first.y = top.y;
				break;

			}
		}
		break;
	case FLIPFLOP_TYPE:
		if (objectName == JK_FF)
		{
			switch (facing) {
			case EAST:
				outputCoord[0].first.x = outputCoord[1].first.x = bottom.x;
				outputCoord[1].first.y = bottom.y - 1 * UNIT;
				outputCoord[0].first.y = bottom.y - 5 * UNIT;
				inputCoord[0].first.x = inputCoord[1].first.x = inputCoord[2].first.x = top.x;
				inputCoord[0].first.y = top.y + UNIT;		//K
				inputCoord[1].first.y = top.y + 3 * UNIT;	//C
				inputCoord[2].first.y = top.y + 5 * UNIT;	//J
				inputCoord[3].first.y = bottom.y;
				inputCoord[3].first.x = bottom.x - 3 * UNIT; // R
				break;
			case WEST:
				outputCoord[0].first.x = outputCoord[1].first.x = top.x;
				inputCoord[0].first.x = inputCoord[1].first.x = inputCoord[2].first.x =  bottom.x;
				outputCoord[0].first.y = top.y + 5 * UNIT;
				outputCoord[1].first.y = top.y + UNIT;	//Q' 이쪽이 위에
				inputCoord[0].first.y = bottom.y - UNIT;	 //K
				inputCoord[1].first.y = bottom.y - 3 * UNIT; // C
				inputCoord[2].first.y = bottom.y - 5 * UNIT; //J
				inputCoord[3].first.x = top.x + 3 * UNIT;
				inputCoord[3].first.y = top.y ;
				break;
			case SOUTH:
				outputCoord[0].first.y = outputCoord[1].first.y = bottom.y;
				inputCoord[0].first.y = inputCoord[1].first.y = inputCoord[2].first.y = top.y;
				outputCoord[0].first.x = top.x + 5 * UNIT;	//Q
				outputCoord[1].first.x = top.x + UNIT;		//Q'
				inputCoord[0].first.x = bottom.x - UNIT; //K
				inputCoord[1].first.x = bottom.x - 3 * UNIT;	//C
				inputCoord[2].first.x = bottom.x - 5 * UNIT; // J
				inputCoord[3].first.x = top.x;
				inputCoord[3].first.y = top.y + 3 * UNIT;
				break;
			case NORTH:
				outputCoord[0].first.y = outputCoord[1].first.y = top.y;
				inputCoord[0].first.y = inputCoord[1].first.y = inputCoord[2].first.y =  bottom.y;
				outputCoord[0].first.x = top.x + UNIT;
				outputCoord[1].first.x = top.x + 5 * UNIT;
				inputCoord[0].first.x = bottom.x - 5 * UNIT;	//k
				inputCoord[1].first.x = bottom.x - 3 * UNIT;	//C
				inputCoord[2].first.x = bottom.x - UNIT;		//J
				inputCoord[3].first.x = bottom.x;
				inputCoord[3].first.y = bottom.y - 3 * UNIT;
				break;
			}
		}
		else {
			switch (facing) {
			case EAST:
				outputCoord[0].first.x = outputCoord[1].first.x = bottom.x;
				outputCoord[1].first.y = bottom.y - 1 * UNIT;
				outputCoord[0].first.y = bottom.y - 5 * UNIT;
				inputCoord[0].first.x = inputCoord[1].first.x = top.x;
				inputCoord[0].first.y = top.y + UNIT;
				inputCoord[1].first.y = top.y + 5 * UNIT;
				break;
			case WEST:
				outputCoord[0].first.x = outputCoord[1].first.x = top.x;
				inputCoord[0].first.x = inputCoord[1].first.x = bottom.x;
				outputCoord[0].first.y = top.y + 5 * UNIT;
				outputCoord[1].first.y = top.y + UNIT;	//Q' 이쪽이 위에
				inputCoord[0].first.y = bottom.y - UNIT;
				inputCoord[1].first.y = bottom.y - 5 * UNIT;
				break;
			case SOUTH:
				outputCoord[0].first.y = outputCoord[1].first.y = bottom.y;
				inputCoord[0].first.y = inputCoord[1].first.y = top.y;
				outputCoord[0].first.x = top.x + 5 * UNIT;
				outputCoord[1].first.x = top.x + UNIT;
				inputCoord[0].first.x = bottom.x - 5 * UNIT;
				inputCoord[1].first.x = bottom.x - UNIT;
				break;
			case NORTH:
				outputCoord[0].first.y = outputCoord[1].first.y = top.y;
				inputCoord[0].first.y = inputCoord[1].first.y = bottom.y;
				outputCoord[0].first.x = top.x + UNIT;
				outputCoord[1].first.x = top.x + 5 * UNIT;
				inputCoord[0].first.x = bottom.x - 5 * UNIT;
				inputCoord[1].first.x = bottom.x - UNIT;
				break;
			}
		}
		break;
	case LIB:
		switch (facing)
		{
		case EAST:
			for (int i = 0; i < outputNum; i++)
			{
				outputCoord[i].first.x = bottom.x;
				outputCoord[i].first.y = top.y + 1 * UNIT + i * UNIT;
			}
			for (int i = 0; i < inputNum; i++)
			{
				inputCoord[i].first.x = top.x;
				inputCoord[i].first.y = top.y + 1 * UNIT + i * UNIT;
			}
			break;
		case WEST:
			for (int i = 0; i < outputNum; i++)
			{
				outputCoord[i].first.x = top.x;
				outputCoord[i].first.y = top.y + 1 * UNIT + i * UNIT;
			}
			for (int i = 0; i < inputNum; i++)
			{
				inputCoord[i].first.x = bottom.x;
				inputCoord[i].first.y = top.y + 1 * UNIT + i * UNIT;
			}
			break;
		case SOUTH:
			for (int i = 0; i < outputNum; i++)
			{
				outputCoord[i].first.x = top.x + 1 * UNIT + i * UNIT;
				outputCoord[i].first.y = bottom.y;
			}
			for (int i = 0; i < inputNum; i++)
			{
				inputCoord[i].first.x = top.x + 1 * UNIT + i * UNIT;
				inputCoord[i].first.y = top.y;
			}
			break;
		case NORTH:
			for (int i = 0; i < outputNum; i++)
			{
				outputCoord[i].first.x = top.x + 1 * UNIT + i * UNIT;
				outputCoord[i].first.y = top.y;
			}
			for (int i = 0; i < inputNum; i++)
			{
				inputCoord[i].first.x = top.x + 1 * UNIT + i * UNIT;
				inputCoord[i].first.y = bottom.y;
			}
			break;

		}
		break;
		
	}
	//로직 오브젝트의 입출력 좌표를 이용해
	//이미 현재 오브젝트에 상속된 line의 좌표를 끊고 새로 만든다.
}

void LogicObject::set_Coord_ByFacing(int input)
{
	CString temp;
	switch (input) {
	case EAST:
		temp = _T("East");
		break;
	case WEST:

		temp = _T("West");
		break;
	case NORTH:

		temp = _T("North");
		break;
	case SOUTH:

		temp = _T("South");
		break;
	}
	set_Coord_ByFacing(temp);
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
	if (objectType == FLIPFLOP_TYPE)
	{
		outputCoord[0].first.x = x ;		//Q
		outputCoord[0].first.y = y - 2 * UNIT;

		outputCoord[1].first.x = x;		//Q'
		outputCoord[1].first.y = y + 2 * UNIT;
	}
	else 
	{
		outputCoord[0].first.x = x;
		outputCoord[0].first.y = y;
	}
	
	if (objectType == LIB)
	{
		for (int i = 0; i < outputNum; i++)
		{
			outputCoord[i].first.x = x;
			outputCoord[i].first.y = (y - 4 * UNIT) + i * UNIT;
		}
	}
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
		if (objectName == SEG7)
		{
			for (int i = 0; i < 7; i++)
				inputCoord[i].first.x = x;
			
			for (int i = 0; i < 7; i++)
				inputCoord[i].first.y = (y - 3 * UNIT) + (UNIT * i);
		}
		else {

			inputCoord[0].first.x = x;
			inputCoord[0].first.y = y;
		}
		break;
	case FLIPFLOP_TYPE:
		if (objectName == JK_FF)
		{
			inputCoord[0].first.x = x - 6 * UNIT; // K
			inputCoord[0].first.y = y - 2 * UNIT;
			inputCoord[1].first.x = x - 6 * UNIT; // C
			inputCoord[1].first.y = y;
			inputCoord[2].first.x = x - 6 * UNIT; // J
			inputCoord[2].first.y = y + 2 * UNIT;
			inputCoord[3].first.x = x - 3 * UNIT;	//R
			inputCoord[3].first.y = y + 3 * UNIT;

		}
		else {
			inputCoord[0].first.x = x - 6 * UNIT;	//D,T
			inputCoord[0].first.y = y - 2 * UNIT;
			inputCoord[1].first.x = x - 6 * UNIT;	// C
			inputCoord[1].first.y = y + 2 * UNIT;
		}
		break;
	case LIB:
		for (int i = 0; i < inputNum; i++)
		{
			inputCoord[i].first.x = x - 10 *UNIT;
			inputCoord[i].first.y = (y - 4 * UNIT) + i * UNIT;
		}
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
	if (objectName == JK_FF) {
		for (int i = 0; i < inputNum-1; i++)
		{
			if (inputCoord[i].second == -1)
				return 0;
		}
	}
	else {
		for (int i = 0; i < inputNum; i++)
		{
			if (inputCoord[i].second == -1)
				return 0;
		}
	}
	
		return 1;
}

//주어진 input state을 활용해 output line state를 설정한다.
void LogicObject::setOutput()
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
	case NAND_GATE:
		outputCoord[0].second = !(inputCoord[0].second && inputCoord[1].second);
		break;
	case NOR_GATE:
		outputCoord[0].second = (inputCoord[0].second || inputCoord[1].second);
		break;
	case XOR_GATE:
		outputCoord[0].second = inputCoord[0].second ^ inputCoord[1].second;
		break;
	case OUTPIN :
		output = inputCoord[0].second;
		break;
	}
}

LogicObject::LogicObject()
{
	output = 0;
	facing = EAST;
	bottom.x = bottom.y = top.y = top.x = -INT_MAX;
	isSelected = FALSE;
}


LogicObject::~LogicObject()
{

}
