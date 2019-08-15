// This is the main file for the game logic and function
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

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
//Map objects
_Object Map[100][50] = {};

// Console object
Console g_Console(100, 50, "Game");

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
	g_sChar.m_cLocation.X = 2; //g_Console.getConsoleSize().X / 2;
	g_sChar.m_cLocation.Y = 5; //g_Console.getConsoleSize().Y / 2;
	g_sChar.m_bActive = true;
	// sets the width, height and the font name to use in the console
	g_Console.setConsoleFont(8, 16, L"Consolas");
	//Sets initial spawnpoint
	setRespawn();
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
	g_dElapsedTime += dt;
	g_dDeltaTime = dt;

	switch (g_eGameState)
	{
	case S_SPLASHSCREEN: splashScreenWait(); // game logic for the splash screen
		break;
	case S_GAME: gameplay(); // gameplay logic when we are in the game
		break;
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
	g_sChar.m_cLocation.X = Respawn.X;
	g_sChar.m_cLocation.Y = Respawn.Y;
}
bool bCanJump = true;
bool bWasGrounded = false;
short sJump = 2;
void moveCharacter()
{
	if (g_dBounceTime > g_dElapsedTime)
		return;
	bool bSomethingHappened = false;
	bool bIsGrounded = false;
	if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y + 1].Code == 1)
	{
		bIsGrounded = true;
		bCanJump = true;;
		sJump = 2;
		bWasGrounded = false;
	}
	else
		if (bWasGrounded)
		{
			bWasGrounded = false;
		}
		else if (!bIsGrounded && !bCanJump)
		{
			bWasGrounded = true;
		}

	// Updating the location of the character based on the key press
	// providing a beep sound whenver we shift the character
	//Jumping
	if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
	{

		if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y - 1].Code == 1 || sJump <= 0)
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
			g_sChar.m_cLocation.Y -= 1;
			sJump--;
		}
		bSomethingHappened = true;
	}
	else
	{
		bCanJump = false;
	}
	if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
	{
		//Beep(1440, 30);
		if (Map[g_sChar.m_cLocation.X - 1][g_sChar.m_cLocation.Y].Code != 1)
		{
			g_sChar.m_cLocation.X--;
		}
		else if (Map[g_sChar.m_cLocation.X - 1][g_sChar.m_cLocation.Y].Code == 1 && Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y - 1].Code != 1 && Map[g_sChar.m_cLocation.X - 1][g_sChar.m_cLocation.Y - 1].Code != 1)
		{
			g_sChar.m_cLocation.X--;
			g_sChar.m_cLocation.Y--;
		}
		bSomethingHappened = true;
	}
	/*
	if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
	{
		//Beep(1440, 30);
		if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y + 1].Code != 1)
		{
			g_sChar.m_cLocation.Y++;
		}
		bSomethingHappened = true;
	}
	*/
	if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
	{
		//Beep(1440, 30);
		if (Map[g_sChar.m_cLocation.X + 1][g_sChar.m_cLocation.Y].Code != 1)
		{
			g_sChar.m_cLocation.X++;
		}
		else if (Map[g_sChar.m_cLocation.X + 1][g_sChar.m_cLocation.Y].Code == 1 && Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y - 1].Code != 1 && Map[g_sChar.m_cLocation.X + 1][g_sChar.m_cLocation.Y - 1].Code != 1)
		{
			g_sChar.m_cLocation.X++;
			g_sChar.m_cLocation.Y--;
		}
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_SPACE])
	{
		g_sChar.m_bActive = !g_sChar.m_bActive;
		bSomethingHappened = true;
	}
	if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y + 1].Code == 1)
		bIsGrounded = true;
	if (!bIsGrounded && !bCanJump)//Gravity
	{
		g_sChar.m_cLocation.Y++;
		bSomethingHappened = true;
	}

	if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y].Code == 5)
	{
		playerRespawn();
	}


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
int Color, iTileCode;
char cTileChar;
int* ipColor = &Color;
int* ipTileCode = &iTileCode;
char* cpTileChar = &cTileChar;

void MapPrinting(std::string output, int x) {
	switch (output[x]) {
	case ' ':
		*ipColor = 12;//color picker
		*ipTileCode = 0;//object map.code
		*cpTileChar = ' ';//visual in console
		break;
	case '1':
		*ipColor = 12;
		*ipTileCode = 1;
		*cpTileChar = 'Û';
		break;
	case '2':
		*ipColor = 1;
		*ipTileCode = 2;
		*cpTileChar = '2';
		break;
	case '3':
		*ipColor = 0;
		*ipTileCode = 3;
		*cpTileChar = '3';
		break;
	case '4':
		*ipColor = 0;
		*ipTileCode = 4;
		*cpTileChar = '4';
		break;
	case '5':
		*ipColor = 2;
		*ipTileCode = 5;
		*cpTileChar = 'Û';
		break;
	case '6':
		*ipColor = 0;
		*ipTileCode = 6;
		*cpTileChar = '6';
		break;
	case '7':
		*ipColor = 0;
		*ipTileCode = 7;
		*cpTileChar = '7';
		break;
	case '8':
		*ipColor = 0;
		*ipTileCode = 8;
		*cpTileChar = 'Û';
		break;

	}
}
void renderMap()
{
	// Set up sample colours, and output shadings
	const WORD colors[] = {
		0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
		0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6,0x808080
	};

	COORD c;
	c.X = 0;
	c.Y = 2;
	std::string output;
	// °±²Û

	std::ifstream map("map.txt");
	if (map.is_open()) {
		int y = 1;
		while (getline(map, output)) {
			for (int x = 0; x < output.size(); ++x) {
				c.X = x;
				c.Y = y;
				MapPrinting(output, x);
				Map[c.X][c.Y].Code = *ipTileCode;
				g_Console.writeToBuffer(c, *cpTileChar, colors[*ipColor]);
			}
			++y;
		}
	}
	if (Map[4][16].Active == true) {
		Map[47][8].Code = 0;
		c.X = 47;
		c.Y = 8;
		g_Console.writeToBuffer(c, 'Û', colors[1]);
		Map[48][8].Code = 0;
		c.X = 48;
		c.Y = 8;
		g_Console.writeToBuffer(c, 'Û', colors[1]);
	}
	else if (Map[4][16].Active == false) {
		Map[47][8].Code = 1;
		c.X = 47;
		c.Y = 8;
		g_Console.writeToBuffer(c, 'Û', colors[12]);
		Map[48][8].Code = 1;
		c.X = 48;
		c.Y = 8;
		g_Console.writeToBuffer(c, 'Û', colors[12]);
	}
}

void renderCharacter()
{
	// Draw the location of the character
	WORD charColor = 0x0C;
	if (g_sChar.m_bActive)
	{
		charColor = 0x0A;
	}
	g_Console.writeToBuffer(g_sChar.m_cLocation, (char)2, charColor);
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
