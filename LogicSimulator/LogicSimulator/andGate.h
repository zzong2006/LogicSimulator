#pragma once

class andGate : public Gate
{
public:
	void draw(Graphics* gp, Pen* p);
	void turn();
	virtual bool output();
	andGate();
	virtual ~andGate();
};

