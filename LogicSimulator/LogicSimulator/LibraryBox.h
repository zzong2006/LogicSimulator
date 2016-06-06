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
	//����� Out ������ŭ , �Է��� Pin ������ŭ �޴´�.

	CString selectedType;
	BOOL isSelected;
	BOOL CanBeDivided;		//�б� ���ɻ��¸� ���׶��
	BOOL clickMode;
	BOOL selectMode;
	BOOL simulateMode;
	BOOL isOnFocus;			//���� ���� �𼭸��� �׸�
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