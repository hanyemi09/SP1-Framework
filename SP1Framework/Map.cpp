#include "game.h"
//read from text file and store data
void MapSetting(std::string output, short y, _Map Map[100][50], Object *Objects) {
	Trap temp;
	for (short x = 0; x < output.size(); ++x) {
		switch (output[x]) {
		case ' ':
			Map[x][y].Code = 0;
			break;
		case '1':
			Map[x][y].Code = 1;
			Map[x][y].Solid = true;
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
			Map[x][y].Solid = true;
			temp = { x, y, A_RIGHT };
			Objects->Traps.push_back(temp);
			break;
		case '<':
			Map[x][y].Code = 7;
			Map[x][y].Solid = true;
			temp = { x, y, A_LEFT };
			Objects->Traps.push_back(temp);
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
				ActivatableBlock temp = { x,y,output[x],false };
				Objects->Blocks.push_back(temp);
				Map[x][y].Solid = false;
			}
			else if (output[x] >= 65 && output[x] <= 77)
			{
				Map[x][y].Code = 8;
				ActivatableBlock temp = { x,y,output[x] + 32,true };
				Objects->Blocks.push_back(temp);
				Map[x][y].Solid = true;
			}
			else {
				if (output[x] >= 110 && output[x] <= 122)
				{
					Map[x][y].Code = 2;
					Map[x][y].LeverType = PRESSUREPLATE;
					bool MatchFound = false;
					COORD ctemp = { x,y };
					for (int i = 0; i < Objects->Levers.size(); i++)
					{
						if (output[x] - 13 == Objects->Levers[i].Link&&Objects->Levers[i].LeverType == PRESSUREPLATE)
						{
							Objects->Levers[i].C.push_back(ctemp); 
							MatchFound = true;
							break;
						}
					}
					if (!MatchFound)
					{
						Lever temp;
						temp.C.push_back(ctemp);
						temp = { temp.C,output[x] - 13,PRESSUREPLATE };
					Objects->Levers.push_back(temp);
					}
				}
				else
					if (output[x] >= 78 && output[x] <= 90)
					{
						Map[x][y].Code = 2;
						Map[x][y].LeverType = LEVER;
						bool MatchFound = false;
						COORD ctemp = { x,y };
						for (int i = 0; i < Objects->Levers.size(); i++)
						{
							if (output[x] + 19 == Objects->Levers[i].Link && Objects->Levers[i].LeverType == LEVER)
							{
								Objects->Levers[i].C.push_back(ctemp);
								MatchFound = true;
								break;
							}
						}
						if (!MatchFound)
						{
							Lever temp;
							temp.C.push_back(ctemp);
							temp= {temp.C,output[x] + 19,LEVER };
							Objects->Levers.push_back(temp);
						}
					}
			}
		}
	}
}

void MapInitialise(int level, _Map Map[100][50], Object *Objects) {
	//initialise things in map array
	switch (level) {
	case 0: {
		std::string output;
		std::ifstream map("basics.txt");
		if (map.is_open()) {
			short y = 1;
			while (getline(map, output)) {
				MapSetting(output, y, Map, Objects);
				++y;
			}
		}
		break;
	}
	case 1: {
		std::string output;
		output.clear();
		std::ifstream map("b2.txt");
		if (map.is_open()) {
			short y = 1;
			while (getline(map, output)) {
				MapSetting(output, y, Map, Objects);
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
			short y = 1;
			while (getline(map, output)) {
				MapSetting(output, y, Map, Objects);
				++y;
			}
		}
		break;
	}
	}

}
//reset all map stuff to prepare for next level
void MapReset(short sMapWidth,short sMapHeight,_Map Map[100][50],Object *Objects) {
	for (short x = 0; x < sMapWidth; ++x) {
		for (short y = 0; y < sMapHeight; ++y) {
			Map[x][y] = { " " };
			Map[x][y].Occupied = false;
		}
	}
	Objects->Arrows.clear();
	Objects->Traps.clear();
}