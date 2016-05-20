#include "stdafx.h"
#include "star.h"
#include <cmath>

double randBound(double low, double high, bool neg);

Star::Star()
{
	xPos = randBound(100, 150, true);
	yPos = randBound(100, 150, true);
	zPos = randBound(100, 150, true);
	xSpeed = ((double)rand() / RAND_MAX) + 1;
	ySpeed = ((double)rand() / RAND_MAX) + 1;
	zSpeed = ((double)rand() / RAND_MAX) + 1;
	mass = ((double)rand() / RAND_MAX) + 1;
	planeZ = PLANE_Z;
	if (S_TRAILS)
	{
		xOld = new double[S_TRAIL_LENGTH];
		yOld = new double[S_TRAIL_LENGTH];
		zOld = new double[S_TRAIL_LENGTH];

		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = xPos;
			yOld[i] = yPos;
			zOld[i] = zPos;
		}

		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = xPos + STAR_SIZE / 2;
			yOld[i] = yPos + STAR_SIZE / 2;
			xOld[i] = xPos + STAR_SIZE / 2;
		}
		buffPlace = 0;
	}
	updateLast();
}



Star::Star(
	double xPos,
	double yPos,
	double zPos,
	double xSpeed,
	double ySpeed,
	double zSpeed,
	double mass,
	HBRUSH brush
	)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->zPos = zPos;
	this->xSpeed = zSpeed;
	this->ySpeed = ySpeed;
	this->zSpeed = zSpeed;
	this->mass = mass;
	this->brush = brush;
	this->planeZ = PLANE_Z;

	if (S_TRAILS)
	{
		xOld = new double[S_TRAIL_LENGTH];
		yOld = new double[S_TRAIL_LENGTH];
		zOld = new double[S_TRAIL_LENGTH];

		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = xPos;
			yOld[i] = yPos;
			zOld[i] = zPos;

		}

		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = xPos + STAR_SIZE / 2;
			yOld[i] = yPos + STAR_SIZE / 2;
			xOld[i] = xPos + STAR_SIZE / 2;
		}
	}
	buffPlace = 0;
	updateLast();
}



Star::Star(
	double galX,
	double galY,
	double galZ,
	HBRUSH brush
	)
{
	/*
		xPos = randBound(0, STAR_ORBIT_R, true);
		yPos = sqrt(pow(STAR_ORBIT_R, 2) - pow(xPos, 2)) * pow(-1, rand());
		zPos = sqrt(pow(STAR_ORBIT_R, 2) - pow(xPos, 2) - pow(yPos, 2)) * pow(-1, rand()) + randBound(0, STAR_ORBIT_TOL, true);
		xPos += galX;
		yPos += galY;
		zPos += galZ;
	*/

		xPos = galX + (rand() % STAR_ORBIT_TOL + STAR_ORBIT_R) * pow(-1, rand());
		yPos = galY + (rand() % STAR_ORBIT_TOL + STAR_ORBIT_R) * pow(-1, rand());
		zPos = galZ + (rand() % STAR_ORBIT_TOL + STAR_ORBIT_R) * pow(-1, rand());
	
	if (CIRC_ORBIT)
	{
		ySpeed = randBound(.75, 1, false);
		zSpeed = randBound(.75, 1, false);
		xSpeed = (((yPos - galY) * ySpeed) + ((zPos - galZ)*zSpeed)) / (galX - xPos);
	}
	
	else
	{
		xSpeed = (((double)rand() / RAND_MAX) + 1) * pow(-1, rand());
		ySpeed = (((double)rand() / RAND_MAX) + 1) * pow(-1, rand());
		zSpeed = (((double)rand() / RAND_MAX) + 1) * pow(-1, rand());
	}
	

	mass = STAR_MASS;
	planeZ = PLANE_Z;
	this->brush = brush;
	if (S_TRAILS)
	{
		xOld = new double[S_TRAIL_LENGTH];
		yOld = new double[S_TRAIL_LENGTH];
		zOld = new double[S_TRAIL_LENGTH];
		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = xPos;
			yOld[i] = yPos;
			xOld[i] = xPos;
		}
	}

	buffPlace = 0;
	updateLast();
}


void Star::update(double otherX, double otherY, double otherZ, double otherMass)
{
	double g, gx, gy, gz, r;
	r = sqrt(pow(xPos - otherX, 2) + pow(yPos - otherY, 2) + pow(zPos - otherZ, 2));
	g = (mass * otherMass) / pow(r, 2);
	gx = g * (otherX - xPos) / r;
	gy = g * (otherY - yPos) / r;
	gz = g * (otherZ - zPos) / r;

	xSpeed += (gx / mass)*TIME_INT;
	ySpeed += (gy / mass)*TIME_INT;
	zSpeed += (gz / mass)*TIME_INT;

	xPos += xSpeed;
	yPos += ySpeed;
	zPos += zSpeed;

	if (USE_3D)
	{
		xDisplay_L = xPos - LEYE_X;
		yDisplay_L = yPos - LEYE_Y;
		zDisplay_L = zPos - LEYE_Z;
		xDisplay_R = xPos - REYE_X;
		yDisplay_R = yPos - REYE_Y;
		zDisplay_R = zPos - REYE_Z;

		double lt = PLANE_Z / zDisplay_L; //works for display plane parallel to xy-plane
		double rt = PLANE_Z / zDisplay_R; //works for display plane parallel to xy-plane

		xDisplay_L *= lt;
		yDisplay_L *= lt;
		zDisplay_L *= lt;
		xDisplay_R *= rt;
		yDisplay_R *= rt;
		zDisplay_R *= rt;
	}

	if (S_TRAILS && !USE_3D)
	{
		xOld[buffPlace] = xPos;
		yOld[buffPlace] = yPos;
		zOld[buffPlace] = zPos;
		if (buffPlace < S_TRAIL_LENGTH - 1) buffPlace++;
		else buffPlace = 0;
	}
}



void Star::display(HDC hdc, HWND hWnd)
{
	if(zPos < Z_RENDER_BOUNDARY)
	{
		if (USE_3D)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			double XC = rc.right / 2;
			double YC = rc.bottom / 2;
			HBRUSH red;
			HBRUSH blue;
			SetBkMode(hdc, TRANSPARENT);
			if (HATCH_3D)
			{
				red = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 0, 0));
				blue = CreateHatchBrush(HS_FDIAGONAL, RGB(0, 0, 255));
			}
			else
			{
				red = CreateSolidBrush(RGB(255, 0, 0));
				blue = CreateSolidBrush(RGB(0, 0, 255));
			}

			SelectObject(hdc, blue);
			Ellipse(hdc,
				xDisplay_L - STAR_SIZE*pow(S_GROWTH_FACTOR, zPos - planeZ + PLANE_Z),
				yDisplay_L - STAR_SIZE*pow(S_GROWTH_FACTOR, zPos - planeZ + PLANE_Z),
				xDisplay_L + STAR_SIZE*pow(S_GROWTH_FACTOR, zPos - planeZ + PLANE_Z),
				yDisplay_L + STAR_SIZE*pow(S_GROWTH_FACTOR, zPos - planeZ + PLANE_Z)
				);
			SelectObject(hdc, red);
			Ellipse(hdc,
				xDisplay_R - STAR_SIZE*pow(S_GROWTH_FACTOR, zPos - planeZ + PLANE_Z),
				yDisplay_R - STAR_SIZE*pow(S_GROWTH_FACTOR, zPos - planeZ + PLANE_Z),
				xDisplay_R + STAR_SIZE*pow(S_GROWTH_FACTOR, zPos - planeZ + PLANE_Z),
				yDisplay_R + STAR_SIZE*pow(S_GROWTH_FACTOR, zPos - planeZ + PLANE_Z)
				);

			DeleteObject(red);
			DeleteObject(blue);
		}
		else if (USE_Z_SCALING)
		{
			SelectObject(hdc, brush);
			Ellipse(hdc,
				xPos - STAR_SIZE*pow(S_GROWTH_FACTOR, zPos),
				yPos - STAR_SIZE*pow(S_GROWTH_FACTOR, zPos),
				xPos + STAR_SIZE*pow(S_GROWTH_FACTOR, zPos),
				yPos + STAR_SIZE*pow(S_GROWTH_FACTOR, zPos)
				);
		}
		else
		{
			SelectObject(hdc, brush);
			Ellipse(hdc, xPos - STAR_SIZE, yPos - STAR_SIZE, xPos + STAR_SIZE, yPos + STAR_SIZE);
		}
		if (S_TRAILS && !USE_3D)
		{
			HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, white);
			RECT rect;
			for (int i = 0; i < S_TRAIL_LENGTH; i++)
			{
				rect.left = xOld[i];
				rect.right = xOld[i] + 1;
				rect.top = yOld[i];
				rect.bottom = yOld[i] + 1;
				FillRect(hdc, &rect, white);
			}
			DeleteObject(white);
		}
	}
}



double randBound(double low, double high, bool neg)
{
	if (neg)
		return ((((double)rand() / RAND_MAX) * (high - low)) + low) * pow(-1, rand());
	else
		return (((double)rand() / RAND_MAX) * (high - low)) + low;
}