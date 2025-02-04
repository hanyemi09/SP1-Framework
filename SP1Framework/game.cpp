﻿// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>



double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];
const short sMapWidth=100, sMapHeight=50;
short sYDisplacement=0;
// Game specific variables here
PlayerVar Player1, Player2;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime[2]; // this is to prevent key bouncing, so we won't trigger keypresses more than once
double  g_dSlideTime[2]; //To track how long player has been wall climbing 
double FrameBouncetime = 0.0;
irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
_Map Map[sMapWidth][sMapHeight];
Object Objects;
// Console object
Console g_Console(100, 50, "Game");
//level counter
int level = 0;

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init(void)
{
	// Set precision for floating point output
	g_dElapsedTime = 0.0;
	g_dBounceTime[1] = 0.0;
	g_dBounceTime[0] = 0.0;
	// sets the initial state for the game
	g_eGameState = S_SPLASHSCREEN;
	//players spawnpoints
	switch (level) {
	case 0: {
		Player1.C.X = 2;
		Player1.C.Y = 14;
		Player2.C.X = 3;
		Player2.C.Y = 14;
		break;
	}
	case 1: {
		Player1.C.X = 2;
		Player1.C.Y = 40;
		Player2.C.X = 1;
		Player2.C.Y = 40;
		break;
	}
	case 2: {
		Player1.C.X = 2;
		Player1.C.Y = 6;
		Player2.C.X = 1;
		Player2.C.Y = 6;
		break;
	}
	}
	//Player2.m_bSolid = true;
	// sets the width, height and the font name to use in the console
	g_Console.setConsoleFont(8, 16, L"Consolas");
	//sets initial spawnpoint
	setRespawn(&Player1);
	MapInitialise(level,Map, &Objects);
	switch (level) {
	case 0:
		MainMenuMusic();
		break;
	}
	
}
//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown(void)
{
	// Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

	g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput(void)
{
	g_abKeyPressed[K_UP] = isKeyPressed(VK_UP);
	g_abKeyPressed[K_DOWN] = isKeyPressed(VK_DOWN);
	g_abKeyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
	g_abKeyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
	g_abKeyPressed[K_SPACE] = isKeyPressed(VK_SPACE);
	g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	g_abKeyPressed[K_ENTER] = isKeyPressed(0x0D);
}
bool isgamepause = false;
//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{

	// get the delta time
	if (g_eGameState == S_GAME)
	{
		g_dElapsedTime += dt;
	}
	g_dDeltaTime = dt;

	switch (g_eGameState)
	{
	case S_SPLASHSCREEN: splashScreenWait(); // game logic for the splash screen
		break;
	case S_GAME: gameplay(); // gameplay logic when we are in the game
		break;
	case S_PAUSE: pausegame();
		break;
	//case S_INTRO: 
	//	break;
	}

}

int Counter = 0;
void ReadTXT(std::string filename,COORD Coord)
{
	std::string output;
	std::ifstream file(filename);
	if (file.is_open()) {

		while (getline(file, output)) {
			for (int x = 0; x < output.size(); x++)
			{
			g_Console.writeToBuffer(Coord, output[x], 0x1f);
			}
		}
	}
}/*
void intro()
{
	COORD c;
	c.X = 1; //(g_Console.getConsoleSize.X - 101);
	c.Y = 1; // (g_Console.getConsoleSize.Y - 3);
	std::string output;
	output.clear();
	std::ifstream logo("LogoAnimation.txt");
	short framecount = g_dElapsedTime/0.5;
		switch (framecount)
		{
		case 0:
			ReadTXT("SplashScreenAnimation1.txt", c);
			break;
		case 1:
			ReadTXT("SplashScreenAnimation2.txt", c);
			break;
		case 2:
			ReadTXT("SplashScreenAnimation3.txt", c);
			break;
		case 3:ReadTXT("SplashScreenAnimation4.txt", c);
			break;
		case 4:ReadTXT("SplashScreenAnimation5.txt", c);
			break;
		case 5:ReadTXT("SplashScreenAnimation6.txt", c);
			break;
		case 6:ReadTXT("SplashScreenAnimation7.txt", c);
			break;
		case 7:ReadTXT("SplashScreenAnimation8.txt", c);
			break;
		case 8:ReadTXT("SplashScreenAnimation9.txt", c);
			break;
		case 9:ReadTXT("SplashScreenAnimation10.txt", c);
			break;
		}
	if (FrameBouncetime > g_dElapsedTime) 
	return;
		for (int line = 0; line < 5; line++)
		{
			getline(logo, output);
			for (int x = 0; x < 102; x++)
			{
				c.X = x;
				c.Y = line;
				g_Console.writeToBuffer(c, output[x], 0x1f);
			}
		}
		FrameBouncetime = g_dElapsedTime + 0.5;
	
}
	//switch (Counter){
	//case 0:
	//	if (map.is_open()) {
	//		int y = g_Console.getConsoleSize().Y-3;
	//		while (getline(map, output)) {
	//			int start;
	//			start = g_Console.getConsoleSize().X - (output.size() / 2);
	//			c.X = start;
	//			for (int x = 0; x < output.length(); ++x) {
	//				switch (output[x]) {
	//				default:
	//					c.X += 1;
	//					c.Y = y;
	//					g_Console.writeToBuffer(c, output[x], 0x06);
	//					break;
	//				}
	//			}
	//			++y;
	//		}
	//	
	//	}
	//	Sleep(500);
	//	++Counter;
	//	break;
	//case 1:
	//	std::string output;
	//	output.clear();
	//	std::ifstream map("SplashScreenAnimation2.txt");//Main_menu_Splash_Art
	//	if (map.is_open()) {
	//		int y = 10;
	//		while (getline(map, output)) {
	//			int start;
	//			start = (sMapWidth / 2) - (output.length() / 2);
	//			c.X = start;
	//			for (int x = 0; x < output.length(); ++x) {
	//				switch (output[x]) {
	//				default:
	//					c.X += 1;
	//					c.Y = y;
	//					g_Console.writeToBuffer(c, output[x], 0x06);
	//					break;
	//				}
	//			}
	//			++y;
	//		}
	//	}
	//	break;

	//}
	*/
//pause screen
void pausegame()
{
	COORD c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = c.X / 2 - 9;
	g_Console.writeToBuffer(c, "Press O to continue", 0x03);

	if (isKeyPressed(0x4F))
	{
		isgamepause = false;
		g_eGameState = S_GAME;
	}
	if (g_abKeyPressed[K_ESCAPE] == true)
	{
		g_bQuitGame = true;
	}
}


//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
	clearScreen();      // clears the current screen and draw from scratch 
	switch (g_eGameState)
	{
	case S_SPLASHSCREEN: renderSplashScreen();
		break;
	case S_GAME: renderGame();
		break;
	case S_PAUSE: pausegame();
		break;
	/*case S_INTRO: intro();
		break;*/
	}
	renderFramerate();  // renders debug information, frame rate, elapsed time, etc
	renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    
{
	if (g_abKeyPressed[K_ENTER] == true) // Press ENTER to start
	{
		g_eGameState = S_GAME;
	}

	if (g_abKeyPressed[K_ESCAPE] == true) // Press ENTER to start
	{
		g_bQuitGame = true;
	}
	
}

void gameplay()            // gameplay logic
{
	processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
	//Moves arrows
	for (int i = 0; i < Objects.Arrows.size(); i++)
	{
		Objects.Arrows[i].MoveArrow(g_dElapsedTime, Map);
	}
	moveCharacter1();    // moves the character, collision detection, physics, etc
	moveCharacter2();
	//erases arrows that hit a solid tile
	for (int i = 0; i < Objects.Arrows.size(); i++)
	{
		if (Map[Objects.Arrows[i].C.X][Objects.Arrows[i].C.Y].Solid)
		{
			Objects.Arrows.erase(Objects.Arrows.begin() + i);
		}
	}
	//arrow traps shoot new arrows
	for (int i = 0; i < Objects.Traps.size(); i++)
	{
		Objects.Traps[i].CreateArrow(g_dElapsedTime, &Objects.Arrows, Map);
	}
	//MovementSounds(); // sound can be played here too.
}
//void scanMap(char _Link)
//{
//	for (int Y = 0; Y < sMapHeight; Y++)
//	{
//		for (int X = 0; X < sMapWidth; X++)
//		{
//			if (Map[X][Y].Link == _Link && Map[X][Y].Code == 8)
//			{
//				if (Map[X][Y].Solid == false)
//				{
//					Map[X][Y].Solid = true;
//				}
//				else if(Map[X][Y].Solid == true)
//				{
//					Map[X][Y].Solid = false;
//				}
//			}
//		}
//	}
//}
void moveCharacter1()
{
	if (g_dBounceTime[1] > g_dElapsedTime)
		return;
	//initialize values
	Player1.bSomethingHappened = false;
	Player1.bGravity = true;
	COORD PrevPos1 = Player1.C;
	Player1.bIsGrounded = false;
	//Lever interaction
	if (g_abKeyPressed[K_DOWN] && Map[Player1.C.X][Player1.C.Y].Code == 2)
	{
		for (short i = 0; i < Objects.Levers.size(); i++)
		{
			for (short j = 0; j < Objects.Levers[i].C.size();j++)
			{
				if (Player1.C.X == Objects.Levers[i].C[j].X && Player1.C.Y == Objects.Levers[i].C[j].Y && Objects.Levers[i].LeverType == LEVER)
				{
					if (Objects.Levers[i].Active == false)
					{
						Objects.Levers[i].Active = true;
					}
					else
					{
						Objects.Levers[i].Active = false;
					}
					Objects.UpdateBlockSolidL(Map, Objects.Levers[i].Link, Objects.Levers[i].Active);
					break;
				}
			}
		}
		Player1.bSomethingHappened = true;
	}
	//jumping variables
	if (Map[Player1.C.X][Player1.C.Y + 1].Solid == true)
	{
		Player1.bIsGrounded = true;
		Player1.bCanJump = true;
		Player1.bCanWallJumpL = false;
		Player1.bCanWallJumpR = false;
		Player1.sJump = 2;
		Player1.sDisplacementSinceGrounded = 0;
	}
	else if (!Player1.bWasWallJ && !Player1.bCanWallJumpL && !Player1.bCanWallJumpR)
	{
		Player1.sDisplacementSinceGrounded++;
	}
	if (Player1.sDisplacementSinceGrounded == 1)
	{
		Player1.bWasGrounded = true;
	}
	else
	{
		Player1.bWasGrounded = false;
	}
	//Wall Jumping
	if (Player1.bCanWallJumpL || Player1.bCanWallJumpR)
	{
		if (g_dSlideTime[1] < g_dElapsedTime)
		{
			Player1.C.Y++;
			g_dSlideTime[1] = g_dElapsedTime + 1;
			Player1.bGravity = false;
		}
	}
	if (g_abKeyPressed[K_UP] && Player1.C.Y > 0 && Player1.bWasWallJ && Map[Player1.C.X][Player1.C.Y - 1].Code != 1)
	{
		Player1.C.Y--;
		Player1.bWasWallJC = true;	
	}
	Player1.bWasWallJ = false;
	//logic for left key
	if (g_abKeyPressed[K_LEFT] && Player1.C.X > 0 && !g_abKeyPressed[K_RIGHT])
	{
		//moving left
		if (!Map[Player1.C.X - 1][Player1.C.Y].Solid)
		{
			Player1.C.X--;
			//multiplayer physics
			if (Map[Player1.C.X+1][Player1.C.Y - 1].Occupied && !Map[Player2.C.X-1][Player2.C.Y].Solid)
			{
				Map[Player2.C.X][Player2.C.Y].Solid = false;
				Map[Player2.C.X][Player2.C.Y].Occupied = false;
				Player2.C.X--;
				Map[Player2.C.X][Player2.C.Y].Solid = true;
				Map[Player2.C.X][Player2.C.Y].Occupied = true;
			}
		}
		//step climbing
		else if (Map[Player1.C.X - 1][Player1.C.Y].Solid && !Map[Player1.C.X][Player1.C.Y - 1].Solid && !Map[Player1.C.X - 1][Player1.C.Y - 1].Solid && Player1.bIsGrounded)
		{
			Player1.C.X--;
			Player1.C.Y--;
		}
		//detects if player can Wall Jump
		else if (Map[Player1.C.X - 1][Player1.C.Y].Solid)
		{
			Player1.bCanWallJumpL = true;
			if(g_dElapsedTime> g_dSlideTime[1])
			g_dSlideTime[1] = g_dElapsedTime + 1;
		}
		Player1.bSomethingHappened = true;
	}
	if (!Map[Player1.C.X - 1][Player1.C.Y].Solid)
	{
		Player1.bCanWallJumpL = false;
	}
	//same logic as left key 
	if (g_abKeyPressed[K_RIGHT] && Player1.C.X < g_Console.getConsoleSize().X - 1 && !g_abKeyPressed[K_LEFT])
	{
		if (!Map[Player1.C.X + 1][Player1.C.Y].Solid)
		{
			Player1.C.X++;
			if (Map[Player1.C.X - 1][Player1.C.Y - 1].Occupied && !Map[Player2.C.X + 1][Player2.C.Y].Solid)
			{
				Map[Player2.C.X][Player2.C.Y].Solid = false;
				Map[Player2.C.X][Player2.C.Y].Occupied = false;
				Player2.C.X++;
				Map[Player2.C.X][Player2.C.Y].Solid = true;
				Map[Player2.C.X][Player2.C.Y].Occupied = true;
			}
		}
		else if (Map[Player1.C.X + 1][Player1.C.Y].Solid && !Map[Player1.C.X][Player1.C.Y - 1].Solid && !Map[Player1.C.X + 1][Player1.C.Y - 1].Solid && Player1.bIsGrounded)
		{
			Player1.C.X++;
			Player1.C.Y--;
		}
		else if (Map[Player1.C.X + 1][Player1.C.Y].Solid)
		{
			Player1.bCanWallJumpR = true;
			if (g_dElapsedTime > g_dSlideTime[1])
				g_dSlideTime[1] = g_dElapsedTime + 1;
		}
		Player1.bSomethingHappened = true;
	}
	if (!Map[Player1.C.X + 1][Player1.C.Y].Solid)
	{
		Player1.bCanWallJumpR = false;
	}
	//logic for up key
	if (g_abKeyPressed[K_UP] && Player1.C.Y > 0)
	{
		//detects if player can jump
		if ((Map[Player1.C.X][Player1.C.Y - 1].Solid && !Map[Player1.C.X][Player1.C.Y - 1].Occupied) || Player1.sJump <= 0 || (Map[Player1.C.X][Player1.C.Y - 1].Occupied && Map[Player1.C.X][Player1.C.Y - 2].Solid))
		{
			Player1.bCanJump = false;
			Player1.sJump = 0;
		}
		else if (Player1.bWasGrounded)
		{
			Player1.bCanJump = true;
		}
		//player jumping 
		if (Player1.bCanJump)
		{
			if (Map[Player1.C.X][Player1.C.Y - 1].Occupied)
			{
				Map[Player1.C.X][Player1.C.Y].Occupied = false;
				Player2.C.Y--;
			}
			if (Player1.sJump == 2){
				engine->play2D("jump2.wav", false, false);
			}
			Player1.C.Y -= 1;
			Player1.sJump--;
		}
		//edge climbing
		else if (Player1.bCanWallJumpL && !Map[Player1.C.X - 1][Player1.C.Y - 1].Solid)
		{
			Player1.C.X--;
			Player1.C.Y--;
		}
		else if (Player1.bCanWallJumpR && !Map[Player1.C.X + 1][Player1.C.Y - 1].Solid)
		{
			Player1.C.X++;
			Player1.C.Y--;
		}
		//wall jumping
		else if (Player1.bCanWallJumpL && !Map[Player1.C.X + 1][Player1.C.Y - 1].Solid)
		{
			Player1.C.X++;
			Player1.C.Y--;
			engine->play2D("jump2.wav", false, false);
			Player1.bCanWallJumpL = false;
			Player1.bWasWallJ = true;
			Player1.bWasWallJC = true;
			g_dSlideTime[1] =g_dElapsedTime+ 1;
		}
		else if (Player1.bCanWallJumpR && !Map[Player1.C.X - 1][Player1.C.Y - 1].Solid)
		{
			Player1.C.X--;
			Player1.C.Y--;
			engine->play2D("jump2.wav", false, false);
			Player1.bCanWallJumpR = false;
			Player1.bWasWallJ = true;
			Player1.bWasWallJC = true;
			g_dSlideTime[1] = g_dElapsedTime + 1;
		}
		Player1.bSomethingHappened = true;
	}
	else
	{
		Player1.bCanJump = false;
		Player1.bWasGrounded = true;
	}
	//release from wall climb
	if (g_abKeyPressed[K_DOWN])
	{
		Player1.bCanWallJumpL = false;
		Player1.bCanWallJumpR = false;
		g_dSlideTime[1] = NULL;
		Player1.bSomethingHappened = true;
	}
	/*if (g_abKeyPressed[K_SPACE])
	{
		Player1.m_bSolid = !Player1.m_bSolid;
		Player1.bSomethingHappened = true;
	}*/
	if (Map[Player1.C.X][Player1.C.Y + 1].Solid)
		Player1.bIsGrounded = true;
	//Gravity
	if (!Player1.bIsGrounded && !Player1.bCanJump && !Player1.bCanWallJumpL && !Player1.bCanWallJumpR && !Player1.bWasWallJC && Player1.bGravity)
	{
		Player1.C.Y++;
		Player1.bSomethingHappened = true;
	}
	if (Player1.bWasWallJC)
	{
		Player1.bWasWallJC = false;
	}
	//Player interation with interactable objects
	if (Map[Player1.C.X][Player1.C.Y].Code == 5)//lava
	{
		engine->play2D("deathsound.mp3", false, false);
		Player1Respawn(&Player1);
		Player1.health = 3;
	}
	if (Map[Player1.C.X][Player1.C.Y].Code == 4) {//checkpoint
		setRespawn(&Player1);
	}
	//arrow
	if ((Map[Player1.C.X][Player1.C.Y].Code == 6 && !Map[Player1.C.X][Player1.C.Y].Solid) || (Map[Player1.C.X][Player1.C.Y].Code == 7 && !Map[Player1.C.X][Player1.C.Y].Solid)) {
		engine->play2D("hitsound.mp3", false, false);
		Player1.health--;
		if(Player1.health == 0)
			engine->play2D("deathsound.mp3", false, false);
		HpUpdate(&Player1);
		Map[Player1.C.X][Player1.C.Y].Code = 0;
	}
	//if (Map[PrevPos1.X][PrevPos1.Y].LeverType == PRESSUREPLATE || Map[Player1.C.X][Player1.C.Y].LeverType == PRESSUREPLATE)
	//updates pressureplate's effects
	Objects.UpdateBlockSolidPP(Map);

	if (Player1.bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime[1] = g_dElapsedTime + 0.125; // 125ms should be enough
	}
	//stores player vaiable for multiplayer physics
	Map[PrevPos1.X][PrevPos1.Y].Occupied = false;
	Map[PrevPos1.X][PrevPos1.Y].Solid = false;
	Map[Player1.C.X][Player1.C.Y].Solid = true;
	Map[Player1.C.X][Player1.C.Y].Occupied = true;
}
//Old Code
	/*
void PPFunc(PlayerVar Player)
{
	for (short i = 0; i < Objects.Levers.size(); i++)
	{
		if (Objects.Levers[i].C.X == Player.C.X && Objects.Levers[i].C.Y == Player.C.Y && Objects.Levers[i].LeverType == PRESSUREPLATE)
		{
			for (short j = 0; j < Objects.Blocks.size(); j++)
			{
				if (Objects.Levers[i].Link == Objects.Blocks[j].Link)
				{
					if (Objects.Levers[i].Active)
					{
						switch (Objects.Blocks[j].OriginalSolid)
						{
						case true:
							Map[Objects.Blocks[j].C.X][Objects.Blocks[j].C.Y].Solid = false;
							break;
						case false:
							Map[Objects.Blocks[j].C.X][Objects.Blocks[j].C.Y].Solid = true;
							break;
						}
					}
					else if (!Objects.Levers[i].Active)
					{
						switch (Objects.Blocks[j].OriginalSolid)
						{
						case true:
							Map[Objects.Blocks[j].C.X][Objects.Blocks[j].C.Y].Solid = true;
							break;
						case false:
							Map[Objects.Blocks[j].C.X][Objects.Blocks[j].C.Y].Solid = false;
							break;
						}
					}
				}
			}
		}
	}
		void Suffocate(PlayerVar *Suffocatee, _Map Map[][50], ActivatableBlock Blocks)
		{
			if (Suffocatee->C.X == Blocks.C.X && Suffocatee->C.Y == Blocks.C.Y)
			{
				Map[Suffocatee->C.X][Suffocatee->C.Y].Occupied = false;
				Map[Suffocatee->C.X][Suffocatee->C.Y].Solid = true;
				PlayerRespawn(&Suffocatee);
			}
			else if (!Map[x][y].Active&&Map[x][y].Occupied)
			{
				scanMap(Map[x][y].Link);
				Map[x][y].Active = true;
				if (Map[Suffocater.C.X][Suffocater.C.Y].Link == Map[Suffocatee.C.X][Suffocatee.C.Y].Link && Map[Suffocatee.C.X][Suffocatee.C.Y].Code == 8)
				{
					if (Map[Suffocater.C.X][Suffocater.C.Y].Link == Map[Suffocatee.C.X][Suffocatee.C.Y].Link && Map[Suffocatee.C.X][Suffocatee.C.Y].Code == 8)
					{
						Map[Suffocatee.C.X][Suffocatee.C.Y].Occupied = false;
						Map[Suffocatee.C.X][Suffocatee.C.Y].Solid = true;
						SuffocateeRespawn(&Suffocatee);
					}
					if (Map[Suffocatee.C.X][Suffocatee.C.Y].Link == Map[Suffocater.C.X][Suffocater.C.Y].Link && Map[Suffocater.C.X][Suffocater.C.Y].Code == 8)
					{
						Map[Suffocater.C.X][Suffocater.C.Y].Occupied = false;
						Map[Suffocater.C.X][Suffocater.C.Y].Solid = true;
						Player1Respawn(&Suffocater);
					}
				}
			}
		}
				if (Map[Player1.C.X][Player1.C.Y].Link == Map[Player2.C.X][Player2.C.Y].Link && Map[Player2.C.X][Player2.C.Y].Code == 8)
				{
					if (Map[Player1.C.X][Player1.C.Y].Link == Map[Player2.C.X][Player2.C.Y].Link && Map[Player2.C.X][Player2.C.Y].Code == 8)
					{
						Map[Player2.C.X][Player2.C.Y].Occupied = false;
						Map[Player2.C.X][Player2.C.Y].Solid = true;
						Player2Respawn(&Player2);
					}
					if (Map[Player2.C.X][Player2.C.Y].Link == Map[Player1.C.X][Player1.C.Y].Link && Map[Player1.C.X][Player1.C.Y].Code == 8)
					{
						Map[Player1.C.X][Player1.C.Y].Occupied = false;
						Map[Player1.C.X][Player1.C.Y].Solid = true;
						Player1Respawn(&Player1);
					}
				}
			else if (!Map[x][y].Active&&Map[x][y].Occupied)
			{
				scanMap(Map[x][y].Link);
				Map[x][y].Active = true;
				if (Map[Player1.C.X][Player1.C.Y].Link == Map[Player2.C.X][Player2.C.Y].Link && Map[Player2.C.X][Player2.C.Y].Code == 8)
				{
					if (Map[Player1.C.X][Player1.C.Y].Link == Map[Player2.C.X][Player2.C.Y].Link && Map[Player2.C.X][Player2.C.Y].Code == 8)
					{
						Map[Player2.C.X][Player2.C.Y].Occupied = false;
						Map[Player2.C.X][Player2.C.Y].Solid = true;
						Player2Respawn(&Player2);
					}
					if (Map[Player2.C.X][Player2.C.Y].Link == Map[Player1.C.X][Player1.C.Y].Link && Map[Player1.C.X][Player1.C.Y].Code == 8)
					{
						Map[Player1.C.X][Player1.C.Y].Occupied = false;
						Map[Player1.C.X][Player1.C.Y].Solid = true;
						Player1Respawn(&Player1);
					}
				}
			}
}
	for (int x = 0; x < sMapWidth;x++)
	{
		for (int y = 0; y < sMapHeight; y++)
		{
			if (Map[x][y].LeverType == PRESSUREPLATE)
			{
				if (Map[x][y].Active && !Map[x][y].Occupied)
				{
						scanMap(Map[x][y].Link);
						Map[x][y].Active = false;
						if (Map[Player1.C.X][Player1.C.Y].Link == Map[Player2.C.X][Player2.C.Y].Link && Map[Player2.C.X][Player2.C.Y].Code == 8)
						{
							if (Map[Player1.C.X][Player1.C.Y].Link == Map[Player2.C.X][Player2.C.Y].Link && Map[Player2.C.X][Player2.C.Y].Code == 8)
							{
								Map[Player2.C.X][Player2.C.Y].Occupied = false;
								Map[Player2.C.X][Player2.C.Y].Solid = true;
								engine->play2D("deathsound.mp3", false, false);
								Player2Respawn(&Player2);
							}
							if (Map[Player2.C.X][Player2.C.Y].Link == Map[Player1.C.X][Player1.C.Y].Link && Map[Player1.C.X][Player1.C.Y].Code == 8)
							{
								Map[Player1.C.X][Player1.C.Y].Occupied = false;
								Map[Player1.C.X][Player1.C.Y].Solid = true;
								engine->play2D("deathsound.mp3", false, false);
								Player1Respawn(&Player1);
							}
						}
				}
				else if (!Map[x][y].Active&&Map[x][y].Occupied)
				{
						scanMap(Map[x][y].Link);
						Map[x][y].Active = true;
						if (Map[Player1.C.X][Player1.C.Y].Link == Map[Player2.C.X][Player2.C.Y].Link && Map[Player2.C.X][Player2.C.Y].Code == 8)
						{
							if (Map[Player1.C.X][Player1.C.Y].Link == Map[Player2.C.X][Player2.C.Y].Link && Map[Player2.C.X][Player2.C.Y].Code == 8)
							{
								Map[Player2.C.X][Player2.C.Y].Occupied = false;
								Map[Player2.C.X][Player2.C.Y].Solid = true;
								engine->play2D("deathsound.mp3", false, false);
								Player2Respawn(&Player2);
							}
							if (Map[Player2.C.X][Player2.C.Y].Link == Map[Player1.C.X][Player1.C.Y].Link && Map[Player1.C.X][Player1.C.Y].Code == 8)
							{
								Map[Player1.C.X][Player1.C.Y].Occupied = false;
								Map[Player1.C.X][Player1.C.Y].Solid = true;
								engine->play2D("deathsound.mp3", false, false);
								Player1Respawn(&Player1);
							}
						}
				}
			}
		}
	}
	*/
//same thing as move character 1
void moveCharacter2()
{
	if (g_dBounceTime[0] > g_dElapsedTime)
		return;
	Player2.bSomethingHappened = false;
	Player2.bGravity = true;
	COORD PrevPos2 = Player2.C;
	//Map[Player2.C.X][Player2.C.Y].Solid = false;
	//Jumping
	Player2.bIsGrounded = false;
	//Lever interaction
	if (isKeyPressed(0x53) && Map[Player2.C.X][Player2.C.Y].Code == 2)
	{
		for (short i = 0; i < Objects.Levers.size(); i++)
		{
			for(short j=0;j< Objects.Levers[i].C.size();j++)
			{
			if (Player2.C.X == Objects.Levers[i].C[j].X && Player2.C.Y == Objects.Levers[i].C[j].Y && Objects.Levers[i].LeverType == LEVER)
			{
				if (Objects.Levers[i].Active == false)
				{
					Objects.Levers[i].Active = true;
				}
				else
				{
					Objects.Levers[i].Active = false;
				}
				Objects.UpdateBlockSolidL(Map, Objects.Levers[i].Link, Objects.Levers[i].Active);
				break;
			}
			}
		}
		Player2.bSomethingHappened = true;
		}
	if (Map[Player2.C.X][Player2.C.Y + 1].Solid == true)
	{
		Player2.bIsGrounded = true;
		Player2.bCanJump = true;
		Player2.bCanWallJumpL = false;
		Player2.bCanWallJumpR = false;
		Player2.sJump = 2;
		Player2.sDisplacementSinceGrounded = 0;
	}
	else if (!Player2.bWasWallJ && !Player2.bCanWallJumpL && !Player2.bCanWallJumpR)
	{
		Player2.sDisplacementSinceGrounded++;
	}
	if (Player2.sDisplacementSinceGrounded == 1)
	{
		Player2.bWasGrounded = true;
	}
	else
	{
		Player2.bWasGrounded = false;
	}
	//Wall Jumping
	if (Player2.bCanWallJumpL || Player2.bCanWallJumpR)
	{
		if (g_dSlideTime[0] < g_dElapsedTime)
		{
			Player2.C.Y++;
			g_dSlideTime[0] = g_dElapsedTime + 1;
			Player2.bGravity = false;
		}
	}
	if (isKeyPressed(0x57) && Player2.C.Y > 0 && Player2.bWasWallJ && Map[Player2.C.X][Player2.C.Y - 1].Code != 1)
	{
		Player2.C.Y--;
		g_dBounceTime[0] = g_dElapsedTime + 0.125;
		Player2.bWasWallJC = true;
	}
	Player2.bWasWallJ = false;
	if (isKeyPressed(0x41) && Player2.C.X > 0 && !isKeyPressed(0x44))
	{
		if (!Map[Player2.C.X - 1][Player2.C.Y].Solid)
		{
			Player2.C.X--;
			if (Map[Player2.C.X + 1][Player2.C.Y - 1].Occupied && !Map[Player1.C.X - 1][Player1.C.Y].Solid)
			{
				Map[Player1.C.X][Player1.C.Y].Solid = false;
				Map[Player1.C.X][Player1.C.Y].Occupied = false;
				Player1.C.X--;
				Map[Player1.C.X][Player1.C.Y].Solid = true;
				Map[Player1.C.X][Player1.C.Y].Occupied = true;
			}
		}
		else if (Map[Player2.C.X - 1][Player2.C.Y].Solid && !Map[Player2.C.X][Player2.C.Y - 1].Solid && !Map[Player2.C.X - 1][Player2.C.Y - 1].Solid && Player2.bIsGrounded)
		{
			Player2.C.X--;
			Player2.C.Y--;
		}
		else if (Map[Player2.C.X - 1][Player2.C.Y].Solid)
		{
			Player2.bCanWallJumpL = true;
			if (g_dElapsedTime > g_dSlideTime[0])
				g_dSlideTime[0] = g_dElapsedTime + 1;
		}
		Player2.bSomethingHappened = true;
	}
	if (!Map[Player2.C.X - 1][Player2.C.Y].Solid)
	{
		Player2.bCanWallJumpL = false;
	}
	if (isKeyPressed(0x44) && Player2.C.X < g_Console.getConsoleSize().X - 1 && !isKeyPressed(0x41))
	{
		if (!Map[Player2.C.X + 1][Player2.C.Y].Solid)
		{
			Player2.C.X++;
			if (Map[Player2.C.X - 1][Player2.C.Y - 1].Occupied && !Map[Player1.C.X + 1][Player1.C.Y].Solid)
			{
				Map[Player1.C.X][Player1.C.Y].Solid = false;
				Map[Player1.C.X][Player1.C.Y].Occupied = false;
				Player1.C.X++;
				Map[Player1.C.X][Player1.C.Y].Solid = true;
				Map[Player1.C.X][Player1.C.Y].Occupied = true;
			}
		}
		else if (Map[Player2.C.X + 1][Player2.C.Y].Solid && !Map[Player2.C.X][Player2.C.Y - 1].Solid && !Map[Player2.C.X + 1][Player2.C.Y - 1].Solid && Player2.bIsGrounded)
		{
			Player2.C.X++;
			Player2.C.Y--;
		}
		else if (Map[Player2.C.X + 1][Player2.C.Y].Solid)
		{
			Player2.bCanWallJumpR = true;
			if (g_dElapsedTime > g_dSlideTime[0])
				g_dSlideTime[0] = g_dElapsedTime + 1;
		}
		Player2.bSomethingHappened = true;
	}
	if (!Map[Player2.C.X + 1][Player2.C.Y].Solid)
	{
		Player2.bCanWallJumpR = false;
	}
	if (isKeyPressed(0x57) && Player2.C.Y > 0)
	{
		if ((Map[Player2.C.X][Player2.C.Y - 1].Solid && !Map[Player2.C.X][Player2.C.Y - 1].Occupied) || Player2.sJump <= 0 || (Map[Player2.C.X][Player2.C.Y - 1].Occupied && Map[Player2.C.X][Player2.C.Y - 2].Solid))
		{
			Player2.bCanJump = false;
			Player2.sJump = 0;
		}
		else if (Player2.bWasGrounded)
		{
			Player2.bCanJump = true;
		}
		//Beep(1440, 30);
		if (Player2.bCanJump)
		{
			if (Map[Player2.C.X][Player2.C.Y - 1].Occupied)
			{
				Map[Player1.C.X][Player1.C.Y].Occupied = false;
				Player1.C.Y--;
			}
			if (Player2.sJump == 2) {
				engine->play2D("jumpmp3.mp3", false, false);
			}
			Player2.C.Y -= 1;
			Player2.sJump--;
		}
		else if (Player2.bCanWallJumpL && !Map[Player2.C.X - 1][Player2.C.Y - 1].Solid)
		{
			Player2.C.X--;
			Player2.C.Y--;
		}
		else if (Player2.bCanWallJumpR && !Map[Player2.C.X + 1][Player2.C.Y - 1].Solid)
		{
			Player2.C.X++;
			Player2.C.Y--;
		}
		else if (Player2.bCanWallJumpL && !Map[Player2.C.X + 1][Player2.C.Y - 1].Solid)
		{
			engine->play2D("jumpmp3.mp3", false, false);
			Player2.C.X++;
			Player2.C.Y--;
			Player2.bCanWallJumpL = false;
			Player2.bWasWallJ = true;
			Player2.bWasWallJC = true;
			g_dSlideTime[1] = g_dElapsedTime + 1;
		}
		else if (Player2.bCanWallJumpR && !Map[Player2.C.X - 1][Player2.C.Y - 1].Solid)
		{
			engine->play2D("jumpmp3.mp3", false, false);
			Player2.C.X--;
			Player2.C.Y--;
			Player2.bCanWallJumpR = false;
			Player2.bWasWallJ = true;
			Player2.bWasWallJC = true;
			g_dSlideTime[1] = g_dElapsedTime + 1;
		}
		Player2.bSomethingHappened = true;
	}
	else
	{
		Player2.bCanJump = false;
		Player2.bWasGrounded = true;
	}

	if (isKeyPressed(0x53))
	{
		Player2.bCanWallJumpL = false;
		Player2.bCanWallJumpR = false;
		g_dSlideTime[0] = NULL;
		Player2.bSomethingHappened = true;
	}
	/*if (g_abKeyPressed[K_SPACE])
	{
		Player2.m_bSolid = !Player2.m_bSolid;
		Player2.bSomethingHappened = true;
	}*/
	if (Map[Player2.C.X][Player2.C.Y + 1].Solid)
		Player2.bIsGrounded = true;
	if (!Player2.bIsGrounded && !Player2.bCanJump && !Player2.bCanWallJumpL && !Player2.bCanWallJumpR && !Player2.bWasWallJC && Player2.bGravity)//Gravity
	{
		Player2.C.Y++;
		Player2.bSomethingHappened = true;
	}
	if (Player2.bWasWallJC)
	{
		Player2.bWasWallJC = false;
	}
		//Player interation with interactable objects
	if (Map[Player2.C.X][Player2.C.Y].Code == 5)
	{
		engine->play2D("deathsound.mp3", false, false);
		Player2Respawn(&Player2);
		Player2.health = 3;
	}
	if ((Map[Player2.C.X][Player2.C.Y].Code == 6 && !Map[Player2.C.X][Player2.C.Y].Solid) || (Map[Player2.C.X][Player2.C.Y].Code == 7 && !Map[Player2.C.X][Player2.C.Y].Solid))
	{
		engine->play2D("hitsound.mp3", false, false);
		Player2.health--;
		if (Player2.health == 0)
			engine->play2D("deathsound.mp3", false, false);
		HpUpdate(&Player2);
		Map[Player2.C.X][Player2.C.Y].Code = 0;
	}
	//if(Map[PrevPos2.X][PrevPos2.Y].LeverType==PRESSUREPLATE || Map[Player2.C.X][Player2.C.Y].LeverType==PRESSUREPLATE)
	Objects.UpdateBlockSolidPP(Map);

	if (Player2.bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime[0] = g_dElapsedTime + 0.125; // 125ms should be enough
	}
	if (Map[Player2.C.X][Player2.C.Y].Code == 4) {

		setRespawn(&Player2);
	}
	Map[PrevPos2.X][PrevPos2.Y].Occupied = false;
	Map[PrevPos2.X][PrevPos2.Y].Solid = false;
	Map[Player2.C.X][Player2.C.Y].Occupied = true;
	Map[Player2.C.X][Player2.C.Y].Solid = true;
}
//for transitioning to pause sceen and manual respawning
void processUserInput()
{
	// quits the game if player hits the escape key
	if (g_abKeyPressed[K_ESCAPE])
		g_bQuitGame = true;

	if (isKeyPressed(0x50))
	{
		isgamepause = true;
		g_eGameState = S_PAUSE;
	}
	if (isKeyPressed(0x55)) {
		engine->play2D("deathsound.mp3", false, false);
		Player1Respawn(&Player1);
		Player2Respawn(&Player2);
		for (int y = 0; y < sMapHeight; y++)
		{
			for (int x = 0; x < sMapWidth; x++)
			{
				if (Map[x][y].Occupied)
				{
					Map[x][y].Occupied = false;
					Map[x][y].Solid = false;
				}
			}
		}
	}
}

void clearScreen()
{
	// Clears the buffer with this colour attribute
	g_Console.clearBuffer(0x03);//0x1F
}

void renderSplashScreen()  // renders the splash screen
{
	/*intro();*/
	/*COORD c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = c.X / 2 - 9;
	g_Console.writeToBuffer(c, "Press <ENTER> to start", 0x03);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 12;
	g_Console.writeToBuffer(c, "Use the Arrow Keys to move", 0x09);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 9;
	g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);*/

	COORD c;
	c.Y = 1;
	std::string output;
	output.clear();
	switch (level) {
	case 0: {
		std::ifstream map("splash.txt");//Main_menu_Splash_Art
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				int start;
				start = (sMapWidth / 2) - (output.length() / 2);
				c.X = start;
				for (int x = 0; x < output.length(); ++x) {
					switch (output[x]) {
					default:
						c.X += 1;
						c.Y = y;
						g_Console.writeToBuffer(c, output[x], 0x06);
						break;
					}
				}
				++y;
			}
		}
		break;
	}
	case 1: {
		std::ifstream map("level2splash.txt");
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				int start;
				start = (sMapWidth / 2) - (output.length() / 2);
				c.X = start;
				for (int x = 0; x < output.length(); ++x) {
					switch (output[x]) {
					default:
						c.X += 1;
						c.Y = y;
						g_Console.writeToBuffer(c, output[x], 0x06);
						break;
					}
				}
				++y;
			}
		}
		break;
	}
	case 2: {
		std::ifstream map("level3splash.txt");//Main_menu_Splash_Art
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				int start;
				start = (sMapWidth / 2) - (output.length() / 2);
				c.X = start;
				for (int x = 0; x < output.length(); ++x) {
					switch (output[x]) {
					default:
						c.X += 1;
						c.Y = y;
						g_Console.writeToBuffer(c, output[x], 0x06);
						break;
					}
				}
				++y;
			}
		}
		break;
	}
	}
}

void renderGame()
{
	sYDisplacement = ((Player1.C.Y + Player2.C.Y) / 2)-15;
	if (sYDisplacement < 0)
	{
		sYDisplacement = 0;
	}
	renderMap();        // renders the map to the buffer first
	renderCharacter();  // renders the character into the buffer
}
//void MapSetting(std::string output, short y) {
//	Trap temp;
//	for (short x = 0; x < output.size(); ++x) {
//		switch (output[x]) {
//		case ' ':
//			Map[x][y].Code = 0;
//			break;
//		case '1':
//			Map[x][y].Code = 1;
//			Map[x][y].Solid = true;
//			break;
//		case '2':
//			Map[x][y].Code = 2;
//			break;
//		case '3':
//			Map[x][y].Code = 3;
//			break;
//		case '4':
//			Map[x][y].Code = 4;
//			break;
//		case '5':
//			Map[x][y].Code = 5;
//			break;
//		case '>':
//			Map[x][y].Code = 6;
//			Map[x][y].Solid = true;
//			temp = { x, y, A_RIGHT };
//			Traps.push_back(temp);
//			break;
//		case '<':
//			Map[x][y].Code = 7;
//			Map[x][y].Solid = true;
//			temp = { x, y, A_LEFT };
//			Traps.push_back(temp);
//			break;
//		case '7':
//			Map[x][y].Code = 7;
//			break;
//		case '9':
//			Map[x][y].Code = 9;
//			break;
//		default:
//			if (output[x] >= 97 && output[x] <= 109)
//			{
//				Map[x][y].Code = 8;
//				Map[x][y].Link = output[x];
//				Map[x][y].Solid = false;
//			}
//			else if (output[x] >= 65 && output[x] <= 77)
//			{
//				Map[x][y].Code = 8;
//				Map[x][y].Link = output[x] + 32;
//				Map[x][y].Solid = true;
//			}
//			else {
//				if (output[x] >= 110 && output[x] <= 122)
//				{
//					Map[x][y].Code = 2;
//					Map[x][y].Link = output[x] - 13;
//					Map[x][y].LeverType = PRESSUREPLATE;
//				}
//				else
//					if (output[x] >= 78 && output[x] <= 90)
//					{
//						Map[x][y].Code = 2;
//						Map[x][y].Link = output[x] + 19;
//						Map[x][y].LeverType = Lever;
//					}
//			}
//		}
//	}
//}


void renderMap()
{
	PlaySound(NULL, NULL, 0);
	// Set up sample colours, and output shadings
	const WORD colors[] = {
		0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
		0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6,0x808080,//0x25C9
	};
	// °±²Û
	COORD c;
	c.X = 0;
	c.Y = 2;
	
	//rendering from Map array
	for (short x = 0; x < sMapWidth; ++x) {
		for (short y = 1; y <= sMapHeight; ++y) {
			if (y<=sYDisplacement)
			{
				continue;
			}
			switch (Map[x][y].Code) {
			case 0:
				Map[x][y].Code = 0;//air block
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, ' ', colors[12]);
				break;
			case 1://normal blocks
				Map[x][y].Code = 1;
				Map[x][y].Solid = true;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, 'Û', colors[12]);
				break;
			case 2://levers and pressureplates
				Map[x][y].Code = 2;
				c.X = x;
				c.Y = y;
				if (Map[x][y].LeverType == LEVER) {
					g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, '/', 0x71);
				}
				else if(Map[x][y].LeverType == PRESSUREPLATE)
				{
					g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, (char)254, colors[1]);
				}
				break;
			case 3://scrapped idea
				Map[x][y].Code = 3;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, 'Û', colors[1]);
				break;
			case 4://checkpoint
				Map[x][y].Code = 4;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, 'P', colors[5]);
				break;
			case 5://Lava
				Map[x][y].Code = 5;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, 'Û', colors[2]);
				break;
			case 6://traps or darts
				c.X = x;
				c.Y = y;
				Map[x][y].Code = 6;
				if (Map[x][y].Solid) 
				{
				g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, (char)10, colors[12]);
				}
				else if(!Map[x][y].Solid)
				{
					g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, (char)15, colors[12]);
				}
				break;
			case 7://traps or darts
				Map[x][y].Code = 7;
				c.X = x;
				c.Y = y;
				if (Map[x][y].Solid)
				{
				g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, (char)10, colors[12]);
				}
				else if (!Map[x][y].Solid)
				{
					g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, (char)15, colors[12]);
				}
				break;
			case 8://activatable blocks
				Map[x][y].Code = 8;
				c.X = x;
				c.Y = y;
				if (Map[x][y].Solid == false) {
					g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, ' ', colors[12]);
				}
				else {
					g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, 'Û', colors[12]);
				}
				break;
			case 9://end point
				Map[x][y].Code = 9;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, 'Û', colors[4]);
				break;
			default:
				break;
			}
			/*
			if (Map[x][y].Occupied)
			{
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c.X, c.Y - sYDisplacement, '/', colors[4]);
			}
			*/
		}
	}
	//stage transition
	if (Map[Player1.C.X][Player1.C.Y].Code == 9&& Map[Player2.C.X][Player2.C.Y].Code == 9) {
		++level;
		MapReset(sMapWidth,sMapHeight,Map, &Objects);
		init();
	}
}

void renderCharacter()
{
	// Draw the location of the character
	WORD charColor = 0x0C;
	/*if (Player1.m_bSolid)
	{
		charColor = 0x0A;
	}*/
	g_Console.writeToBuffer(Player1.C.X,Player1.C.Y-sYDisplacement, (char)2, charColor);
	charColor = 0x0A;
	/*if (Player2.m_bSolid)
	{
		charColor = 0x0C;
	}*/
	g_Console.writeToBuffer(Player2.C.X, Player2.C.Y- sYDisplacement, (char)3, charColor);

	/*g_Console.writeToBuffer(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y - 3, (char)2, charColor);
	for (int i = -1; i < 2; i++)
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation.X + i, g_sChar.m_cLocation.Y - 2, (char)22, charColor);
	}
	g_Console.writeToBuffer(g_sChar.m_cLocation.X , g_sChar.m_cLocation.Y - 1, (char)0, charColor);
	g_Console.writeToBuffer(g_sChar.m_cLocation.X-1, g_sChar.m_cLocation.Y,(char)0, charColor);
	g_Console.writeToBuffer(g_sChar.m_cLocation.X + 1, g_sChar.m_cLocation.Y, (char)0, charColor);*/
}

void MainMenuMusic() {

	TCHAR wavfile[] = _T("main_menu.wav");
	PlaySound(wavfile, NULL, SND_LOOP | SND_ASYNC);
}

//void JumpMusic() {
//	TCHAR wavfile[] = _T("jump_04.wav");
//	PlaySound(wavfile,NULL,SND_ASYNC);
//}
//
//void MovementSounds() {
//	if (g_abKeyPressed[K_UP]) {
//		Beep(1440, 300);
//		//JumpMusic();
//	}
//}

void renderFramerate()
{
	COORD c;
	// displays the framerate
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(3);
	ss << 1.0 / g_dDeltaTime << "fps";
	c.X = g_Console.getConsoleSize().X - 9;
	c.Y = 0;
	g_Console.writeToBuffer(c, ss.str());

	// displays the elapsed time
	
	if (g_eGameState == S_GAME)
	{
		ss.str("");
		ss << g_dElapsedTime << "secs";
		c.X = 0;
		c.Y = 0;
		g_Console.writeToBuffer(c, ss.str(), 0x59);

		ss.str("");
		ss << "Player 1 HP : ";
		c.X = 10;
		c.Y = 0;
		for (int i = 0; i < Player1.health; i++)
		{
			ss << (char)3 << " ";
		}
		g_Console.writeToBuffer(c, ss.str(), 0x0C);
		ss.str("");
		c.X = 35;
		c.Y = 0;
		ss << "Player 2 HP : ";

		for (int i = 0; i < Player2.health; i++)
		{
			ss << (char)3 << " ";
		}
		g_Console.writeToBuffer(c, ss.str(), 0x0C);
	}
}
void renderToScreen()
{
	// Writes the buffer to the console, hence you will see what you have written
	g_Console.flushBufferToConsole();
}
