#pragma once

#include "LogicObject.h"
#include "Clock.h"
#include "FlipFlop.h"
#include "Gate.h"
#include "Pin.h"
#include "Out.h"
#include "name_repo.h"
#include "Sevenseg.h"

class Action
{
public:
	Action(int type, int act)
	{
		Type = type;
		Act = act;
		if (type == LINE)
			line_num = 2;

	}
	Action()
	{

	}
	enum {
		line_add
	};
	int Act, Type;
	int line_num;


	std::vector <LineObject *> lineked_line;
	std::vector <LogicObject *> logicInfo;
	std::vector <Sevenseg *> segInfo;
	std::vector <LineObject *> lines;
};

class LibraryBox : public LogicObject
{
public:
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

	int objectType;
	int objectName;

	LogicObject* temp;

	// undo/redo
	CList <Action> mUndo;
	CList <Action> mRedo;

	BOOL IsInput(LogicObject* lo);
	BOOL IsGate(LogicObject* lo);
	BOOL IsOutput(LogicObject* lo);

	BOOL CanUndo();
	BOOL CanRedo();
	void Undo();
	void Redo();

	//method
	

	LibraryBox();
	~LibraryBox();
};