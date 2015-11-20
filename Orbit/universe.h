#ifndef UNIVERSE_
#define UNIVERSE_

#include "galaxy.h"
#include "star.h"

class Universe
{
private:
	Galaxy galaxy[NUM_GALAXY];
	double speed = 10;
public:
	Universe();
	void update();
	void display(HDC hdc, HWND hWnd);
	void speedUp() { speed += SPEED_INC; }
	void speedDown() { speed -= SPEED_INC; }
	Galaxy getGal(int index) { return galaxy[index]; }
	double getSpeed() { return speed; }
};
#endif