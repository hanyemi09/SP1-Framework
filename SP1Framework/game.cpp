﻿// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];
const short sMapWidth=100, sMapHeight=50;
// Game specific variables here
SGameChar   g_sChar[2];
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
//Map objects
_Object Map[sMapWidth][sMapHeight] = {};

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
	g_dBounceTime = 0.0;

	// sets the initial state for the game
	g_eGameState = S_SPLASHSCREEN;
	g_sChar[1].m_cLocation.X = 2; //g_Console.getConsoleSize().X / 2;
	g_sChar[1].m_cLocation.Y = 5; //g_Console.getConsoleSize().Y / 2;
	g_sChar[1].m_bActive = true;
	// sets the width, height and the font name to use in the console
	g_Console.setConsoleFont(8, 16, L"Consolas");
	//sets initial spawnpoint
	setRespawn();
	MapPrinting();
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
	g_abKeyPressed[K_W] = isKeyPressed(0x57);
	g_abKeyPressed[K_S] = isKeyPressed(0x53);
	g_abKeyPressed[K_A] = isKeyPressed(0x41);
	g_abKeyPressed[K_D] = isKeyPressed(0x44);
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
	}

}


void pausegame()
{
	COORD c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = c.X / 2 - 9;
	g_Console.writeToBuffer(c, "Press R to continue", 0x03);

	if (isKeyPressed(0x52))
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
	}
	renderFramerate();  // renders debug information, frame rate, elapsed time, etc
	renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
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
	moveCharacter();    // moves the character, collision detection, physics, etc
						// sound can be played here too.
}
COORD Respawn;
void setRespawn()
{
	Respawn.X = 2;
	Respawn.Y = 4;
}
void playerRespawn()
{
	g_sChar[1].m_cLocation.X = Respawn.X;
	g_sChar[1].m_cLocation.Y = Respawn.Y;
}

void scanMap(char _Link)
{
	for (int Y = 0; Y < sMapHeight; Y++)
	{
		for (int X = 0; X < sMapWidth; X++)
		{
			if (Map[X][Y].Link == _Link && Map[X][Y].Code == 8)
			{
				if (Map[X][Y].Active == false)
				{
					Map[X][Y].Active = true;
				}
				else if(Map[X][Y].Active == true)
				{
					Map[X][Y].Active = false;
				}
			}
		}
	}
}
bool bWasGrounded = false;
bool bCanJump = true;
short sJump = 2;
short sDisplacementSinceGrounded = 0;
void moveCharacter()
{
	if (g_dBounceTime > g_dElapsedTime)
		return;
	bool bSomethingHappened = false;
	bool bIsGrounded = false;
	if (g_abKeyPressed[K_DOWN]&& Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y].Code==2)
	{
		switch (Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y].LeverType)
		{
		case Lever:
			scanMap(Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y].Link);
		default:
			break;
		}
		bSomethingHappened = true;
	}
	if (Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y + 1].Active == true)
	{
		bIsGrounded = true;
		bCanJump = true;;
		sJump = 2;
		sDisplacementSinceGrounded = 0;
	}
	else
	{
		sDisplacementSinceGrounded++;
	}
	if (sDisplacementSinceGrounded == 1)
		bWasGrounded = true;

		// Updating the location of the character based on the key press
		//Jumping
	if (g_abKeyPressed[K_UP] && g_sChar[1].m_cLocation.Y > 0)
	{

		if (Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y - 1].Active == true || sJump <= 0)
		{
			bCanJump = false;
			sJump = 0;
		}
		else if (bWasGrounded)
		{
			bCanJump = true;
		}
		//Beep(1440, 30);
		if (bCanJump)
		{
			g_sChar[1].m_cLocation.Y -= 1;
			sJump--;
		}
		bSomethingHappened = true;
	}
	else
	{
		bCanJump = false;
		bWasGrounded = true;
	}
	if (g_abKeyPressed[K_LEFT] && g_sChar[1].m_cLocation.X > 0)
	{
		//Beep(1440, 30);
		if (Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y].Active == false)
		{
			g_sChar[1].m_cLocation.X--;
		}
		else if (Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y].Active == true && Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y - 1].Active == false && Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y - 1].Active == false)
		{
			g_sChar[1].m_cLocation.X--;
			g_sChar[1].m_cLocation.Y--;
		}
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_RIGHT] && g_sChar[1].m_cLocation.X < g_Console.getConsoleSize().X - 1)
	{
		//Beep(1440, 30);
		if (Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y].Active == false)
		{
			g_sChar[1].m_cLocation.X++;
		}
		else if (Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y].Active == true && Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y - 1].Active == false && Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y - 1].Active == false)
		{
			g_sChar[1].m_cLocation.X++;
			g_sChar[1].m_cLocation.Y--;
		}
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_SPACE])
	{
		g_sChar[1].m_bActive = !g_sChar[1].m_bActive;
		bSomethingHappened = true;
	}
	if (Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y + 1].Code == 1)
		bIsGrounded = true;

	//Gravity
	if (!bIsGrounded && !bCanJump  )
	{
		g_sChar[1].m_cLocation.Y++;
		bSomethingHappened = true;
	}
	if (Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y].Code == 5)
	{
		playerRespawn();
	}
	//Player interation with interactable objects
	
	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
	}


}
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
}

void clearScreen()
{
	// Clears the buffer with this colour attribute
	g_Console.clearBuffer(0x1F);
}

void renderSplashScreen()  // renders the splash screen
{
	COORD c = g_Console.getConsoleSize();
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
	g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
	renderMap();        // renders the map to the buffer first
	renderCharacter();  // renders the character into the buffer
}

<<<<<<< Updated upstream
/*
int Color, iTileCode;
char cTileChar, cLinkChar;
bool bActive;
short sLeverType;
int* ipColor = &Color;
int* ipTileCode = &iTileCode;
char* cpTileChar = &cTileChar;
char* cpLinkChar = &cLinkChar;
bool* bpActive = &bActive;
short* spLeverType = &sLeverType;
*/

void MapPrinting() {
	//initialise things in map array
	std::string output;
	std::ifstream map("map.txt");
	if (map.is_open()) {
		int y = 1;
		while (getline(map, output)) {
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
				case '6':
					Map[x][y].Code = 6;
					break;
				case '7':
					Map[x][y].Code = 7;
					break;
				default:
					if (output[x] >= 97 && output[x] <= 109)
					{
						Map[x][y].Code = 8;
						Map[x][y].Link = output[x];
						Map[x][y].Active = true;
					}
					else if (output[x] >= 65 && output[x] <= 77)
					{
						Map[x][y].Code = 8;
						Map[x][y].Link = output[x] + 32;
						Map[x][y].Active = false;
=======
void MapSetting(std::string output, int y) {
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
		case '6':
			Map[x][y].Code = 6;
			break;
		case '7':
			Map[x][y].Code = 7;
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
>>>>>>> Stashed changes
					}
			}
		}
	}
}

void MapPrinting() {
	//initialise things in map array
	switch (level) {
	case 0:{
		std::string output;
		std::ifstream map("map.txt");
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				MapSetting(output, y);
				++y;
			}
		}
	}
	case 1: {
		std::string output;
		std::ifstream map("map1.txt");
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				MapSetting(output, y);
				++y;
			}
		}
	}
	case 2: {
		std::string output;
		std::ifstream map(".txt");
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				MapSetting(output, y);
				++y;
			}
		}
	}
	}
	
}

	

void renderMap()
{
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
	for (int x = 0; x < sMapWidth; ++x) {
		for (int y = 1; y <= sMapHeight; ++y) {
			switch (Map[x][y].Code) {
			case 0:
				Map[x][y].Code = 0;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c, ' ', colors[12]);
				break;
			case 1:
				Map[x][y].Code = 1;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c, 'Û', colors[12]);
				break;
			case 2:
				Map[x][y].Code = 2;
				c.X = x;
				c.Y = y;
<<<<<<< Updated upstream
				g_Console.writeToBuffer(c, 'Û', 0x25C9);
=======
				if (Map[x][y].LeverType == Lever) {
					g_Console.writeToBuffer(c, '/', 0x25C9);
				}
				else {
					g_Console.writeToBuffer(c, 'Û', colors[1]);
				}
>>>>>>> Stashed changes
				break;
			case 3:
				Map[x][y].Code = 3;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c, 'Û', colors[1]);
				break;
			case 4:
				Map[x][y].Code = 4;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c, 'Û', colors[1]);
				break;
			case 5:
				Map[x][y].Code = 5;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c, 'Û', colors[1]);
				break;
			case 6:
				Map[x][y].Code = 6;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c, 'Û', colors[1]);
				break;
			case 7:
				Map[x][y].Code = 7;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c, 'Û', colors[4]);
				break;
				
			case 8:
				Map[x][y].Code = 8;
				c.X = x;
				c.Y = y;
				if (Map[x][y].Active == false) {
					g_Console.writeToBuffer(c, ' ', colors[12]);
				}
				else {
					g_Console.writeToBuffer(c, 'Û', colors[12]);
				}
				break;
			}
		}
	}
	if (Map[g_sChar->m_cLocation.X][g_sChar->m_cLocation.Y].Code == 9) {
		++level;
		init();
	}

}

void renderCharacter()
{
	// Draw the location of the character
	WORD charColor = 0x0C;
	if (g_sChar[1].m_bActive)
	{
		charColor = 0x0A;
	}
	g_Console.writeToBuffer(g_sChar[1].m_cLocation, (char)2, charColor);
	/*g_Console.writeToBuffer(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y - 3, (char)2, charColor);
	for (int i = -1; i < 2; i++)
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation.X + i, g_sChar.m_cLocation.Y - 2, (char)22, charColor);
	}
	g_Console.writeToBuffer(g_sChar.m_cLocation.X , g_sChar.m_cLocation.Y - 1, (char)0, charColor);
	g_Console.writeToBuffer(g_sChar.m_cLocation.X-1, g_sChar.m_cLocation.Y,(char)0, charColor);
	g_Console.writeToBuffer(g_sChar.m_cLocation.X + 1, g_sChar.m_cLocation.Y, (char)0, charColor);*/
}

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
	ss.str("");
	ss << g_dElapsedTime << "secs";
	c.X = 0;
	c.Y = 0;
	g_Console.writeToBuffer(c, ss.str(), 0x59);
}
void renderToScreen()
{
	// Writes the buffer to the console, hence you will see what you have written
	g_Console.flushBufferToConsole();
}
