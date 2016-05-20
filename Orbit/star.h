#ifndef STAR_
#define STAR_

class Galaxy;
class Universe;

class Star
{
	friend class Galaxy;
	friend class Universe;
private:
	double xPos, yPos, zPos;
	double xLast, yLast, zLast;
	double xSpeed, ySpeed, zSpeed;
	double xDisplay_L, yDisplay_L, zDisplay_L; //coordinates for 3d display (left eye)
	double xDisplay_R, yDisplay_R, zDisplay_R; //coordinates for 3d display (right eye)
	double mass;
	//double xOld[S_TRAIL_LENGTH], yOld[S_TRAIL_LENGTH], zOld[S_TRAIL_LENGTH];
	double *xOld, *yOld, *zOld;
	int buffPlace;
	int planeZ;
	HBRUSH brush;

public:
	Star();
	Star(
		double xPos,
		double yPos,
		double zPos,
		double xSpeed,
		double ySpeed,
		double zSpeed,
		double mass,
		HBRUSH brush
		);
	Star(
		double galX,
		double galY,
		double galZ,
		HBRUSH brush
		);

	void update(double otherX, double otherY, double otherZ, double otherMass);
	void updateLast()
	{
		xLast = xPos;
		yLast = yPos;
		zLast = zPos;
	}
	void display(HDC hdc, HWND hWnd);

	double x() { return xPos; };
	double y() { return yPos; };
	double z() { return zPos; };
	double m() { return mass; };
	void movePlane(int d) { planeZ += d; }
};
#endif