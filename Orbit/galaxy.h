#ifndef GALAXY_
#define GALAXY_
#include "star.h"

class Galaxy
{
	friend class Universe;
private:
	double xPos, yPos, zPos;
	double xLast, yLast, zLast;
	double xDisplay_L, yDisplay_L, zDisplay_L; //coordinates for 3d display (left eye)
	double xDisplay_R, yDisplay_R, zDisplay_R; //coordinates for 3d display (right eye)
	double xSpeed, ySpeed, zSpeed;
	double xOld[G_TRAIL_LENGTH], yOld[G_TRAIL_LENGTH], zOld[G_TRAIL_LENGTH];
	int buffPlace;
	double mass;
	int planeZ;
	Star star[NUM_STAR];
	HBRUSH brush;
public:
	Galaxy();
	Galaxy
		(
			double xPos,
			double yPos,
			double zPos,
			double xSpeed,
			double ySpeed,
			double zSpeed,
			double mass
			);
	void update(double otherX, double otherY, double otherZ, double otherMass);
	void updateSelf();
	void updateLast()
	{
		xLast = xPos;
		yLast = yPos;
		zLast = zPos;
	}
	void display(HDC hdc, HWND hWnd);

	double starX(int s) { return star[s].xPos; }
	double starY(int s) { return star[s].yPos; }
	double starZ(int s) { return star[s].zPos; }
	double starM(int s) { return star[s].mass; }
	Star getStar(int index) { return star[index]; }
	void movePlane(int d) { planeZ += d; }

	void updateEntireGal();
};
#endif