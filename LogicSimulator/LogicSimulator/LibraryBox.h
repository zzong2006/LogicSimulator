#pragma once

#include "LogicObject.h"
#include "Clock.h"
#include "FlipFlop.h"
#include "Gate.h"
#include "Pin.h"
#include "Out.h"
#include "name_repo.h"


class Action
{
public:
	Action(LineObject* line)
	{
		lines.push_back(line);
		Type = LINE;
	}
	Action(LineObject** line)
	{
		lines.push_back(line[0]);
		lines.push_back(line[1]);
		Type = LINE;
	}
	Action(LogicObject* clock)
	{
		logicInfo.push_back(clock);
		Type = OBJECT;
	}
	Action(int type)
	{
		if (type == LINE)
		{
			Type = type;
			line_num = 2;
		}
	}
	Action()
	{

	}
	enum {
		line_add
	};

	int Type;
	int line_num;
	std::vector <LineObject *> lineked_line;
	std::vector <LogicObject *> logicInfo;
	std::vector <LineObject *> lines;
};

class LibraryBox : public LogicObject
{
public:
	//출력은 Out 갯수만큼 , 입력은 Pin 갯수만큼 받는다.

	CString selectedType;
	BOOL isSelected;
	BOOL CanBeDivided;		//분기 가능상태면 동그라미
	BOOL clickMode;
	BOOL selectMode;
	BOOL simulateMode;
	BOOL isOnFocus;			//선택 상태 모서리에 네모
	std::vector <LogicObject *> currObject;
	std::vector <LogicObject *> logicInfo;
	std::vector <LineObject *> lines;
	std::vector <Clock *> clockInfo;
	std::vector <FlipFlop *> FFInfo;
	std::vector <Gate *> gateInfo;
	std::vector <Pin *> pinInfo;
	std::vector <Out *> outInfo;

	int objectType;
	int objectName;

	int gateNum;

	LogicObject* temp;

	// undo/redo
	CList <Action> mUndo;
	CList <Action> mRedo;

	BOOL CanUndo();
	BOOL CanRedo();
	void Undo();
	void Redo();

	LibraryBox();
	~LibraryBox();
};