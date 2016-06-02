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
	int direct;				//게이트 방향
				

public:
	CString label, facing;
	int objectType, objectName;

	int inputNum, outputNum;			//입력선 개수, 출력선 개수
	bool output, isSelected;

	BOOL chk;			//알고리즘 돌때 방문했는가?

	//그림자 그리기
	virtual void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p) = 0;	
	//실제로 그리기
	virtual void draw_main(Gdiplus::Graphics* gp) = 0;		
	//객체 방향 지정하여 돌리기
	virtual void turn() = 0;						
	//출력 좌표를 이용하여 로직 객체 전체(top/bottom)의 좌표를 정함.
	virtual void set_Coord_From_outC(int x, int y) = 0;	

	//객체가 선택됬다면 양 끝 모서리에 네모를 표시함.
	void showSelected(Gdiplus::Graphics* gp);
	//라벨 표시하기
	void showLabel(Gdiplus::Graphics* gp);
	//라벨 이름 설정하기
	void setLabel(CString input);
	void setFacing(CString input);
	void set_outputCoord(int x, int y);		//출력 좌표 설정
	void set_inputCoord(int x, int y);		//입력 좌표 설정
	
	void toggleOutput();		//출력값 치환 0->1 or 1->0
	POINT get_top() const;
	POINT get_bottm() const;
	BOOL getOutput() const { return output; } ;

	//현재 연결된 선들이 모두 방문된 상태인가를 체크함
	int isInputSet() const;

	LineObject* input_line[2], *output_line;
	POINT inputCoord[2], outputCoord;

	LogicObject();
	virtual ~LogicObject();
};

