#pragma once
class LogicObject
{
protected :
	POINT top, bottom ,right, left;
	POINT labelPos;
	int width, height;
	CString label;

	POINT inputCoord[2], outputCoord;
	bool input[2], output;
	int inputNum;			//입력선 개수
	int direct;				//게이트 방향

public:
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;
	virtual void turn() = 0;
	void setLabel(CString input);				//라벨 이름
	void set_outputCoord(int x, int y);
	void set_inputCoord(int x, int y);
	LogicObject();
	virtual ~LogicObject();
};

