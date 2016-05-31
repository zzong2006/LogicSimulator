#pragma once
#include "LineObject.h"

#define INPUT_SIZE 2

#define UNIT 10

class LogicObject
{
protected:
	POINT top, bottom; // left top / bottom
	POINT labelPos;
	int width, height;

	bool input[2];
	int inputNum;			//입력선 개수
	int direct;				//게이트 방향

public:
	CString label, type;
	int objectType, objectName;
	bool output;
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;
	virtual void turn() = 0;
	virtual void set_Coord_From_outC(int x, int y) = 0;
	void setLabel(CString input);				//라벨 이름
	void set_outputCoord(int x, int y);
	void set_inputCoord(int x, int y);

	int Is_match_inputCoord(POINT src);
	BOOL Is_match_outputCoord(POINT src);
	int Is_match_inputCoord(int src_x, int src_y);
	BOOL Is_match_outputCoord(int src_x, int src_y);

	void toggleOutput();
	POINT get_top() const;
	POINT get_bottm() const;

	LineObject* connect_line;
	LineObject* input_line[2], *output_line;
	POINT inputCoord[2], outputCoord;

	LogicObject();
	virtual ~LogicObject();
};

