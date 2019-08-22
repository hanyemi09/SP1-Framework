#include "game.h"

void MapSetting(std::string output, int y, _Object Map[100][50]) {
	for (int x = 0; x < output.size(); ++x) {
		switch (output[x]) {
		case ' ':
			Map[x][y].Code = 0;
			break;
		case '1':
			Map[x][y].Code = 1;
			Map[x][y].Active = true;
			break;
		case '2':
			Map[x][y].Code = 2;
			break;
		case '3':
			Map[x][y].Code = 3;
			break;
		case '4':
			Map[x][y].Code = 4;
			break;
		case '5':
			Map[x][y].Code = 5;
			break;
		case '>':
			Map[x][y].Code = 6;
			Map[x][y].Active = true;
			break;
		case '<':
			Map[x][y].Code = 7;
			Map[x][y].Active = true;
			break;
		case '7':
			Map[x][y].Code = 7;
			break;
		case '9':
			Map[x][y].Code = 9;
			break;
		default:
			if (output[x] >= 97 && output[x] <= 109)
			{
				Map[x][y].Code = 8;
				Map[x][y].Link = output[x];
				Map[x][y].Active = false;
			}
			else if (output[x] >= 65 && output[x] <= 77)
			{
				Map[x][y].Code = 8;
				Map[x][y].Link = output[x] + 32;
				Map[x][y].Active = true;
			}
			else {
				if (output[x] >= 110 && output[x] <= 122)
				{
					Map[x][y].Code = 2;
					Map[x][y].Link = output[x] - 13;
					Map[x][y].LeverType = PressurePlate;
				}
				else
					if (output[x] >= 78 && output[x] <= 90)
					{
						Map[x][y].Code = 2;
						Map[x][y].Link = output[x] + 19;
						Map[x][y].LeverType = Lever;
					}
			}
		}
	}
}

void MapInitialise(int level,_Object Map[100][50]) {
	//initialise things in map array
	switch (level) {
	case 0: {
		std::string output;
		std::ifstream map("map.txt");
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				MapSetting(output, y, Map);
				++y;
			}
		}
		break;
	}
	case 1: {
		std::string output;
		output.clear();
		std::ifstream map("map1.txt");
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				MapSetting(output, y,Map);
				++y;
			}
		}
		break;
	}
	case 2: {
		std::string output;
		output.clear();
		std::ifstream map(".txt");
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				MapSetting(output, y,Map);
				++y;
			}
		}
		break;
	}
	}

}

void MapReset(int sMapWidth,int sMapHeight,_Object Map[100][50]) {
	COORD c;
	for (int x = 0; x < sMapWidth; ++x) {
		for (int y = 0; y < sMapHeight; ++y) {
			Map[x][y] = { " " };
		}
	}
}