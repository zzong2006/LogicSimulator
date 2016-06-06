#include "stdafx.h"
#include "LibraryBox.h"


LibraryBox::LibraryBox()
{
}


LibraryBox::~LibraryBox()
{
}

BOOL LibraryBox::IsInput(LogicObject * lo)
{
	return lo->objectName == PIN || CLOCK;
}

BOOL LibraryBox::IsGate(LogicObject * lo)
{
	return lo->objectType == GATE_TYPE || FLIPFLOP_TYPE;
}

BOOL LibraryBox::IsOutput(LogicObject * lo)
{
	return lo->objectType == OUTPIN || SEG7;
}

BOOL LibraryBox::CanUndo()
{
	while (mUndo.GetCount() > MAX_UNDO)
	{
		mUndo.RemoveTail();
	}
	return (mUndo.GetCount() > 0);
}

BOOL LibraryBox::CanRedo()
{
	while (mRedo.GetCount() > MAX_UNDO)
	{
		mRedo.RemoveTail();
	}
	return (mRedo.GetCount() > 0);
}

void LibraryBox::Undo()
{
	Action temp;
	CPoint stp, edp;

	temp = mUndo.RemoveHead();
	int lkedn = temp.lineked_line.size();

	switch (temp.Act)
	{
	case NEW:
		if (temp.Type == LINE)
		{

			for (int i = lkedn - 1; i > 0; i -= 2)
			{
				temp.lineked_line.at(i - 1)->line[1] = temp.lineked_line.at(i)->line[1];
				lines.pop_back();
			}

			for (int i = 0; i < 2; i++)
			{
				temp.lines.push_back(lines.at(lines.size() - 1));
				lines.pop_back();
			}
		}
		else {
			temp.logicInfo.push_back(logicInfo.at(logicInfo.size() - 1));
			logicInfo.pop_back();
		}
		break;
	case DELETE:

		break;
	case MOVE:
		break;
	case COPY:
		break;
	case PASTE:
		break;
	}
	mRedo.AddHead(temp);
}

void LibraryBox::Redo()
{
	Action temp;
	CPoint stp, edp;

	temp = mRedo.RemoveHead();
	int lkedn = temp.lineked_line.size();

	switch (temp.Act)
	{
	case NEW:
		if (temp.Type == LINE)
		{
			for (int i = 1; i > -1; i--)
			{
				lines.push_back(temp.lines.at(i));
				temp.lines.pop_back();
			}

			for (int i = 0; i < lkedn; i += 2)
			{
				temp.lineked_line.at(i)->line[1] = temp.lineked_line.at(i + 1)->line[0];
				lines.push_back(temp.lineked_line.at(i + 1));
			}
		}
		else {
			logicInfo.push_back(temp.logicInfo.at(0));
			temp.logicInfo.pop_back();
		}
		break;
	case DELETE:
		break;
	case MOVE:
		break;
	case COPY:
		break;
	case PASTE:
		break;
	}
	mUndo.AddHead(temp);
}
