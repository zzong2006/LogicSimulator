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

	POINT inputCoord[2], outputCoord;
	bool input[2];
	int inputNum;			//입력선 개수
	int direct;				//게이트 방향

public:
	CString label, type;
	int objectType, objectName;
	bool output;
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;		//그림자 그리기
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;		//실제로 그리기
	//객체 방향 지정하여 돌리기
	virtual void turn() = 0;						
	//출력 좌표를 이용하여 로직 객체 전체(top/bottom)의 좌표를 정함.
	virtual void set_Coord_From_outC(int x, int y) = 0;		
	
	void setLabel(CString input);				//라벨 이름 붙이기
	void set_outputCoord(int x, int y);		//출력값 설정
	void set_inputCoord(int x, int y);		//입력값 설정

	int Is_match_inputCoord(POINT src);
	BOOL Is_match_outputCoord(POINT src);
	int Is_match_inputCoord(int src_x, int src_y);
	BOOL Is_match_outputCoord(int src_x, int src_y);

	void toggleOutput();						//출력값 치환 0->1 or 1->0
	POINT get_top() const;
	POINT get_bottm() const;

	LineObject* connect_line;
	LineObject* input_line[2], *output_line;

	LogicObject();
	virtual ~LogicObject();
};

