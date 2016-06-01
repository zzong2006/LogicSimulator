#pragma once
#include "stdafx.h"
#include <vector>

#define START 0
#define VTOH 1
#define HTOV 2

#define OFF_SIGNAL 0
#define ON_SIGNAL 1
#define INPUT_SIGNAL 2
#define OUTPUT_SIGNAL 3

class LineObject
{
protected:

public:
	CPoint line[2];

	BOOL state = FALSE;		//line�� ��
	BOOL chk;				//�湮 �ߴ��� ���ߴ��� Ȯ��

	int connect_pin;
	std::vector<LineObject *> connect_lines;

	void draw_shadow(Gdiplus::Graphics* gp, Gdiplus::Pen * p);		//�̵��Ҷ� �ʿ��� shadow
	void draw_main(Gdiplus::Graphics* gp);

	void check_connect();

	BOOL Is_match_IineCoord(CPoint src);

	LineObject(int x, int y);
	LineObject(CPoint point);
	LineObject();
	~LineObject();
};