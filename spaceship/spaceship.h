#ifndef SPACESHIP_
#define SPCAESHIP_

#define SCALE 10

#include "resource.h"


class Spaceship
{
private:
	//coordinates
		int noseX, noseY;
		int leftWingX, leftWingY;
		int rightWingX, rightWingY;
		int midX, midY;
		int mainX, mainY;

		int speedX, speedY;
		int dirX, dirY;
		double angle = 0;

public:
	Spaceship(RECT *rc);
	Spaceship();
	
	void drawShip(HDC hdc);
	void updateShip();
	void updateShip(int scale);
	double shipAngle();
	void accelX();
	void accelY();
	void spinCW() { angle--; }
	void spinCCW() { angle++; }

};

#endif