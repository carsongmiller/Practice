#include "stdafx.h"
#include "galaxy.h"
#include "star.h"
#include <cmath>

Galaxy::Galaxy()
{
	xPos = rand() % 400;
	yPos = rand() % 400;
	zPos = rand() % 400;
	xSpeed = ((double)rand() / RAND_MAX) + .25;
	ySpeed = ((double)rand() / RAND_MAX) + .25;
	zSpeed = ((double)rand() / RAND_MAX) + .25;
	mass = (rand() % 10000) + 5000;
	brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	buffPlace = 0;
	planeZ = PLANE_Z;

	for (int i = 0; i < NUM_STAR; i++)
		star[i] = Star(xPos, yPos, zPos, brush);
}



Galaxy::Galaxy
(
	double xPos,
	double yPos,
	double zPos,
	double xSpeed,
	double ySpeed,
	double zSpeed,
	double mass
	)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->zPos = zPos;
	this->xSpeed = xSpeed;
	this->ySpeed = ySpeed;
	this->zSpeed = zSpeed;
	this->mass = mass;
	brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	this->buffPlace = 0;

	if (USE_3D)
	{
		this->planeZ = PLANE_Z;

		xDisplay_L = this->xPos - LEYE_X;
		yDisplay_L = this->yPos - LEYE_Y;
		zDisplay_L = this->zPos - LEYE_Z;
		xDisplay_R = this->xPos - REYE_X;
		yDisplay_R = this->yPos - REYE_Y;
		zDisplay_R = this->zPos - REYE_Z;

		double lt = PLANE_Z / zDisplay_L; //works for display plane parallel to xy-plane
		double rt = PLANE_Z / zDisplay_R; //works for display plane parallel to xy-plane

		xDisplay_L *= lt;
		yDisplay_L *= lt;
		zDisplay_L *= lt;
		xDisplay_R *= rt;
		yDisplay_R *= rt;
		zDisplay_R *= rt;
	}

	for (int i = 0; i < NUM_STAR; i++)
		star[i] = Star(xPos, yPos, zPos, brush);
}



void Galaxy::update(double otherX, double otherY, double otherZ, double otherMass)
{
	double g, gx, gy, gz, r;
	r = sqrt(pow(xPos - otherX, 2) + pow(yPos - otherY, 2) + pow(zPos - otherZ, 2));
	g = G_C * (mass * otherMass) / pow(r, 2);
	
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

	if (G_TRAILS && !USE_3D)
	{
		xOld[buffPlace] = xPos;
		yOld[buffPlace] = yPos;
		zOld[buffPlace] = zPos;
		if (buffPlace < G_TRAIL_LENGTH - 1) buffPlace++;
		else buffPlace = 0;
	}
}



void Galaxy::updateSelf()
{
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

	if (G_TRAILS && !USE_3D)
	{
		xOld[buffPlace] = xPos;
		yOld[buffPlace] = yPos;
		zOld[buffPlace] = zPos;
		if (buffPlace < G_TRAIL_LENGTH - 1) buffPlace++;
		else buffPlace = 0;
	}
}



void Galaxy::display(HDC hdc, HWND hWnd)
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
				xDisplay_L - GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos),
				yDisplay_L - GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos),
				xDisplay_L + GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos),
				yDisplay_L + GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos)
				);
			SelectObject(hdc, red);
			Ellipse(hdc,
				xDisplay_R - GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos),
				yDisplay_R - GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos),
				xDisplay_R + GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos),
				yDisplay_R + GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos)
				);

			DeleteObject(red);
			DeleteObject(blue);
		}

		else if (USE_Z_SCALING)
		{
			SelectObject(hdc, brush);
			Ellipse(hdc,
				xPos - GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos),
				yPos - GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos),
				xPos + GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos),
				yPos + GALAXY_SIZE*pow(S_GROWTH_FACTOR, zPos)
				);
		}
		else
		{
			SelectObject(hdc, brush);
			Ellipse(hdc, xPos - STAR_SIZE, yPos - STAR_SIZE, xPos + STAR_SIZE, yPos + STAR_SIZE);
		}

		if (G_TRAILS && !USE_3D)
		{
			HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, white);
			RECT rect;
			for (int i = 0; i < G_TRAIL_LENGTH; i++)
			{
				rect.left = xOld[i];
				rect.right = xOld[i] + 1;
				rect.top = yOld[i];
				rect.bottom = yOld[i] + 1;
				FillRect(hdc, &rect, white);
			}
			DeleteObject(white);
		}
		for (int i = 0; i < NUM_STAR; i++)
			star[i].display(hdc, hWnd);
	}
}



void Galaxy::updateEntireGal()
{
	for (int i = 0; i < NUM_STAR; i++)
	{
		star[i].update(xPos, yPos, zPos, mass); //update stars for galaxy center
		update(star[i].xPos, star[i].yPos, star[i].zPos, star[i].mass); //update galaxy center for all stars
		/*
		for (int j = 0; j < NUM_STAR; j++)
		{
			if (j != i)
				star[i].update(star[j].xPos, star[j].yPos, star[j].zPos, star[j].mass); //update each star for all other stars
		}
		*/
	}

	for (int i = 0; i < NUM_STAR; i++)
		star[i].updateLast();

	updateLast();
}