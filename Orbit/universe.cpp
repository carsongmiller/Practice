#include "stdafx.h"
#include "universe.h"
#include "galaxy.h"
#include "star.h" 

Universe::Universe()
{
	for (int i = 0; i < NUM_GALAXY; i++)
	{
		if (i == 0)
			galaxy[i] = Galaxy(G1_XPOS, G1_YPOS, G1_ZPOS, G1_XSPEED, G1_YSPEED, G1_ZSPEED, GALAXY_MASS);
		else if (i == 1)
			galaxy[i] = Galaxy(G2_XPOS, G2_YPOS, G2_ZPOS, G2_XSPEED, G2_YSPEED, G2_ZSPEED, GALAXY_MASS);
		else if (i == 2)
			galaxy[i] = Galaxy(G3_XPOS, G3_YPOS, G3_ZPOS, G3_XSPEED, G3_YSPEED, G3_ZSPEED, GALAXY_MASS);
		else
			galaxy[i] = Galaxy();
	}
}



void Universe::update()
{
	/*
	The following interactions are accounted for:
	star->star
	star->galaxy
	galaxy->star
	galaxy->galaxy

	a = updating galaxy
	b = updating star
	c = interacting galaxy
	d = interacting star
	*/

	//update all star->star interactions
	if (STAR_STAR_INTER)
	{
		for (int a = 0; a < NUM_GALAXY; a++)
		{
			for (int b = 0; b < NUM_STAR; b++)
			{
				for (int c = 0; c < NUM_GALAXY; c++)
				{
					for (int d = 0; d < NUM_STAR; d++)
					{
						if (a != c || b != d)
						{
							galaxy[a].star[b].update
								(
									galaxy[c].star[d].xLast,
									galaxy[c].star[d].yLast,
									galaxy[c].star[d].zLast,
									galaxy[c].star[d].mass
									);
						}
					}
				}
			}
		}
	}

	//update all star->galaxy interactions
	if (STAR_GAL_INTER)
	{
		for (int a = 0; a < NUM_GALAXY; a++)
		{
			for (int c = 0; c < NUM_GALAXY; c++)
			{
				for (int d = 0; d < NUM_STAR; d++)
				{
					galaxy[a].update
						(
							galaxy[c].star[d].xLast,
							galaxy[c].star[d].yLast,
							galaxy[c].star[d].zLast,
							galaxy[c].star[d].mass
							);
				}
			}
		}
	}

	//update all galaxy->star interactions
	if (GAL_STAR_INTER)
	{
		for (int a = 0; a < NUM_GALAXY; a++)
		{
			for (int b = 0; b < NUM_STAR; b++)
			{
				for (int c = 0; c < NUM_GALAXY; c++)
				{
					galaxy[a].star[b].update
						(
							galaxy[c].xLast,
							galaxy[c].yLast,
							galaxy[c].zLast,
							galaxy[c].mass
							);
				}
			}
		}
	}



	//updating all galaxy->galaxy interactions
	if(GAL_GAL_INTER)
	{
		for (int a = 0; a < NUM_GALAXY; a++)
		{
			for (int c = 0; c < NUM_GALAXY; c++)
			{
				if (a != c)
				{
					galaxy[a].update
						(
							galaxy[c].xLast,
							galaxy[c].yLast,
							galaxy[c].zLast,
							galaxy[c].mass
							);
				}
			}
		}
	}

	if (!GAL_GAL_INTER && !STAR_GAL_INTER)
	{
		for (int i = 0; i < NUM_GALAXY; i++)
			galaxy[i].updateSelf();
	}

	//set all "last" values to the new position values
	for (int i = 0; i < NUM_GALAXY; i++)
	{
		for (int j = 0; j < NUM_STAR; j++)
		{
			galaxy[i].star[j].updateLast();
		}
		galaxy[i].updateLast();
	}
}



void Universe::display(HDC hdc, HWND hWnd)
{
	for (int i = 0; i < NUM_GALAXY; i++)
		galaxy[i].display(hdc, hWnd);
}