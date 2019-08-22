#include "game.h"


void TrapAI(_Object Map[100][50], double *BounceTime, double *ElapsedTime)
{
	if (BounceTime < ElapsedTime)
	{
		for (int x = 0; x < 100; x++)
		{
			for (int y = 0; y < 50; y++)
			{
				switch (Map[x][y].Code)
				{
				case 6:
					if (Map[x][y].Active)
					{
						Map[x + 1][y].Code = 6;
						Map[x + 1][y].Active = false;
					}
					break;
				case 7:
					if (Map[x][y].Active)
					{
						Map[x - 1][y].Code = 7;
						Map[x - 1][y].Active = false;
					}
					break;
				}
			}
		}
		*BounceTime = *ElapsedTime + 1.5;
	}
}
void ArrowAI(_Object Map[100][50], double* BounceTime, double* ElapsedTime) {
	if (BounceTime < ElapsedTime)
	{
		return;
	}
	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			if (!Map[x][y].Active)
			{
				switch (Map[x][y].Code)
				{
				case 6:
					Map[x][y].Code = 0;
					if (Map[x + 1][y].Code == 0)
					{
						Map[x + 1][y].Code = 6;
						Map[x + 1][y].Active = false;
						x++;
					}
					break;
				case 7:
					Map[x][y].Code = 0;
					if (Map[x - 1][y].Code == 0)
					{
						Map[x - 1][y].Code = 7;
						Map[x - 1][y].Active = false;
					}
					break;
				default:
					break;
				}
			}
		}
	}
	*BounceTime = *ElapsedTime + 100;
}