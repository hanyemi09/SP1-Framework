// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

double  g_dElapsedTime;
double  g_dDeltaTime[2];
double  g_dInvincibleTime[2];
double  g_dArrowDeltaTime[2];
bool    g_abKeyPressed[K_COUNT];
const short sMapWidth=100, sMapHeight=50;
// Game specific variables here
PlayerVar Player[2];
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime[2]; // this is to prevent key bouncing, so we won't trigger keypresses more than once
double  g_dArrowBounceTime[2]; //to control speed and rate of fire of darts
double  g_dSlideTime[2]; //To track how long player has been wall climbing 
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
	g_dBounceTime[1] = 0.0;
	g_dBounceTime[0] = 0.0;
	g_dArrowBounceTime[0] = 0.0;
	g_dArrowBounceTime[1] = 0.0;
	// sets the initial state for the game
	g_eGameState = S_SPLASHSCREEN;
	Player[1].C.X = 2; //g_Console.getConsoleSize().X / 2;
	Player[1].C.Y = 14; //g_Console.getConsoleSize().Y / 2;
	//Player[1].m_bActive = true;
	Player[0].C.X = 3; //g_Console.getConsoleSize().X / 2;
	Player[0].C.Y = 14; //g_Console.getConsoleSize().Y / 2;
	//Player[0].m_bActive = true;
	// sets the width, height and the font name to use in the console
	g_Console.setConsoleFont(8, 16, L"Consolas");
	//sets initial spawnpoint
	setRespawn(Player[1]);
	MapPrinting();
	switch (level) {
	case 0:
		MainMenuMusic();
		break;
	}
	//JumpMusic();
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
	g_dDeltaTime[1] = dt;
	g_dDeltaTime[0] = dt;
	g_dArrowDeltaTime[0] = dt;
	g_dArrowDeltaTime[1] = dt;

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
	moveCharacter1();    // moves the character, collision detection, physics, etc
	moveCharacter2();
	ArrowAI();
	TrapAI();
	//MovementSounds(); // sound can be played here too.
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
void TrapAI()
{
	if (g_dArrowBounceTime[0] > g_dElapsedTime)
		return;
	for (int x = 0; x < sMapWidth; x++)
	{
		for (int y = 0; y < sMapHeight; y++)
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
	g_dArrowBounceTime[0] = g_dElapsedTime + 1.5;
}
void ArrowAI() {
	if (g_dArrowBounceTime[1] > g_dElapsedTime)
		return;
		for (int y = 0; y < sMapHeight; y++)
	{
	for (int x = 0; x < sMapWidth; x++)
		{
			if (!Map[x][y].Active)
			{
				switch (Map[x][y].Code)
				{
				case 6:
					Map[x][y].Code = 0;
					if (!Map[x + 1][y].Active)
					{
						Map[x + 1][y].Code = 6;
						Map[x + 1][y].Active = false;
						x++;
					}
					break;
				case 7:
					Map[x][y].Code = 0;
					if (!Map[x - 1][y].Active)
					{
						Map[x - 1][y].Code = 7;
						Map[x - 1][y].Active = false;
						x++;
					}
					break;
				default:
					break;
				}
			}
		}
	}
	g_dArrowBounceTime[1] = g_dElapsedTime + 0.1;
}
void moveCharacter1()
{
	if (g_dBounceTime[1] > g_dElapsedTime)
		return;
	Player[1].bSomethingHappened = false;
	Player[1].bGravity = true;
	//Jumping
	Player[1].bIsGrounded = false;
	//Lever interaction
	if (g_abKeyPressed[K_DOWN] && Map[Player[1].C.X][Player[1].C.Y].Code == 2)
	{
		switch (Map[Player[1].C.X][Player[1].C.Y].LeverType)
		{
		case Lever:
			scanMap(Map[Player[1].C.X][Player[1].C.Y].Link);
		default:
			break;
		}
		Player[1].bSomethingHappened = true;
	}
	if (Map[Player[1].C.X][Player[1].C.Y + 1].Active == true)
	{
		Player[1].bIsGrounded = true;
		Player[1].bCanJump = true;
		Player[1].bCanWallJumpL = false;
		Player[1].bCanWallJumpR = false;
		Player[1].sJump = 2;
		Player[1].sDisplacementSinceGrounded = 0;
	}
	else if (!Player[1].bWasWallJ && !Player[1].bCanWallJumpL && !Player[1].bCanWallJumpR)
	{
		Player[1].sDisplacementSinceGrounded++;
	}
	if (Player[1].sDisplacementSinceGrounded == 1)
	{
		Player[1].bWasGrounded = true;
	}
	else
	{
		Player[1].bWasGrounded = false;
	}
	//Wall Jumping
	if (Player[1].bCanWallJumpL || Player[1].bCanWallJumpR)
	{
		if (g_dSlideTime[1] < g_dElapsedTime)
		{
			Player[1].C.Y++;
			g_dSlideTime[1] = g_dElapsedTime + 1;
			Player[1].bGravity = false;
		}
	}
	if (g_abKeyPressed[K_UP] && Player[1].C.Y > 0 && Player[1].bWasWallJ && Map[Player[1].C.X][Player[1].C.Y - 1].Code != 1)
	{
		Player[1].C.Y--;
		//g_dBounceTime[1] = g_dElapsedTime + 0.125;
		Player[1].bWasWallJC = true;
	}
	Player[1].bWasWallJ = false;
	if (g_abKeyPressed[K_LEFT] && Player[1].C.X > 0)
	{
		if (!Map[Player[1].C.X - 1][Player[1].C.Y].Active)
		{
			Player[1].C.X--;
		}
		else if (Map[Player[1].C.X - 1][Player[1].C.Y].Active && !Map[Player[1].C.X][Player[1].C.Y - 1].Active && !Map[Player[1].C.X - 1][Player[1].C.Y - 1].Active && Player[1].bIsGrounded)
		{
			Player[1].C.X--;
			Player[1].C.Y--;
		}
		else if (Map[Player[1].C.X - 1][Player[1].C.Y].Active)
		{
			Player[1].bCanWallJumpL = true;
			if(g_dElapsedTime> g_dSlideTime[1])
			g_dSlideTime[1] = g_dElapsedTime + 1;
		}
		Player[1].bSomethingHappened = true;
	}
	if (!Map[Player[1].C.X - 1][Player[1].C.Y].Active)
	{
		Player[1].bCanWallJumpL = false;
	}
	if (g_abKeyPressed[K_RIGHT] && Player[1].C.X < g_Console.getConsoleSize().X - 1)
	{
		if (!Map[Player[1].C.X + 1][Player[1].C.Y].Active)
		{
			Player[1].C.X++;
		}
		else if (Map[Player[1].C.X + 1][Player[1].C.Y].Active && !Map[Player[1].C.X][Player[1].C.Y - 1].Active && !Map[Player[1].C.X + 1][Player[1].C.Y - 1].Active && Player[1].bIsGrounded)
		{
			Player[1].C.X++;
			Player[1].C.Y--;
		}
		else if (Map[Player[1].C.X + 1][Player[1].C.Y].Active)
		{
			Player[1].bCanWallJumpR = true;
			if (g_dElapsedTime > g_dSlideTime[1])
				g_dSlideTime[1] = g_dElapsedTime + 1;
		}
		Player[1].bSomethingHappened = true;
	}
	if (!Map[Player[1].C.X + 1][Player[1].C.Y].Active)
	{
		Player[1].bCanWallJumpR = false;
	}
	if (g_abKeyPressed[K_UP] && Player[1].C.Y > 0)
	{
		if (Map[Player[1].C.X][Player[1].C.Y - 1].Active || Player[1].sJump <= 0)
		{
			Player[1].bCanJump = false;
			Player[1].sJump = 0;
		}
		else if (Player[1].bWasGrounded)
		{
			Player[1].bCanJump = true;
		}
		//Beep(1440, 30);
		if (Player[1].bCanJump)
		{
			Player[1].C.Y -= 1;
			Player[1].sJump--;
		}
		else if (Player[1].bCanWallJumpL && !Map[Player[1].C.X - 1][Player[1].C.Y - 1].Active)
		{
			Player[1].C.X--;
			Player[1].C.Y--;
		}
		else if (Player[1].bCanWallJumpR && !Map[Player[1].C.X + 1][Player[1].C.Y - 1].Active)
		{
			Player[1].C.X++;
			Player[1].C.Y--;
		}
		else if (Player[1].bCanWallJumpL && !Map[Player[1].C.X + 1][Player[1].C.Y - 1].Active)
		{
			Player[1].C.X++;
			Player[1].C.Y--;
			Player[1].bCanWallJumpL = false;
			Player[1].bWasWallJ = true;
			Player[1].bWasWallJC = true;
		}
		else if (Player[1].bCanWallJumpR && !Map[Player[1].C.X - 1][Player[1].C.Y - 1].Active)
		{
			Player[1].C.X--;
			Player[1].C.Y--;
			Player[1].bCanWallJumpR = false;
			Player[1].bWasWallJ = true;
			Player[1].bWasWallJC = true;
		}
		Player[1].bSomethingHappened = true;
	}
	else
	{
		Player[1].bCanJump = false;
		Player[1].bWasGrounded = true;
	}

	if (g_abKeyPressed[K_DOWN])
	{
		Player[1].bCanWallJumpL = false;
		Player[1].bCanWallJumpR = false;
		g_dSlideTime[1] = NULL;
		Player[1].bSomethingHappened = true;
	}
	/*if (g_abKeyPressed[K_SPACE])
	{
		Player[1].m_bActive = !Player[1].m_bActive;
		Player[1].bSomethingHappened = true;
	}*/
	if (Map[Player[1].C.X][Player[1].C.Y + 1].Active)
		Player[1].bIsGrounded = true;
	//Gravity
	if (!Player[1].bIsGrounded && !Player[1].bCanJump && !Player[1].bCanWallJumpL && !Player[1].bCanWallJumpR && !Player[1].bWasWallJC && Player[1].bGravity)
	{
		Player[1].C.Y++;
		Player[1].bSomethingHappened = true;
	}
	if (Player[1].bWasWallJC)
	{
		Player[1].bWasWallJC = false;
	}
	if (Map[Player[1].C.X][Player[1].C.Y].Code == 5)
	{
		Player1Respawn(Player[1]);
	}
	if ((Map[Player[1].C.X][Player[1].C.Y].Code == 6 && !Map[Player[1].C.X][Player[1].C.Y].Active) || (Map[Player[1].C.X][Player[1].C.Y].Code == 7 && !Map[Player[1].C.X][Player[1].C.Y].Active)) {

		Player[1].health--;
		HpUpdate(Player[1]);
		Player[1].bSomethingHappened = true;

	}
	if ((Map[Player[1].C.X][Player[1].C.Y].Code == 6 && !Map[Player[1].C.X][Player[1].C.Y].Active) || (Map[Player[1].C.X][Player[1].C.Y].Code == 7 && !Map[Player[1].C.X][Player[1].C.Y].Active)) {

	}
	//Player interation with interactable objects

	if (Player[1].bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime[1] = g_dElapsedTime + 0.125; // 125ms should be enough
	}
	if (Map[Player[1].C.X][Player[1].C.Y].Code == 4) {
		setRespawn(Player[1]);
	}
}


void moveCharacter2()
{
	if (g_dBounceTime[0] > g_dElapsedTime)
		return;
	Player[0].bSomethingHappened = false;
	Player[0].bGravity = true;
	//Jumping
	Player[0].bIsGrounded = false;
	//Lever interaction
	if (isKeyPressed(0x53) && Map[Player[0].C.X][Player[0].C.Y].Code == 2)
	{
		switch (Map[Player[0].C.X][Player[0].C.Y].LeverType)
		{
		case Lever:
			scanMap(Map[Player[0].C.X][Player[0].C.Y].Link);
		default:
			break;
		}
		Player[0].bSomethingHappened = true;
	}
	if (Map[Player[0].C.X][Player[0].C.Y + 1].Active == true)
	{
		Player[0].bIsGrounded = true;
		Player[0].bCanJump = true;
		Player[0].bCanWallJumpL = false;
		Player[0].bCanWallJumpR = false;
		Player[0].sJump = 2;
		Player[0].sDisplacementSinceGrounded = 0;
	}
	else if (!Player[0].bWasWallJ && !Player[0].bCanWallJumpL && !Player[0].bCanWallJumpR)
	{
		Player[0].sDisplacementSinceGrounded++;
	}
	if (Player[0].sDisplacementSinceGrounded == 1)
	{
		Player[0].bWasGrounded = true;
	}
	else
	{
		Player[0].bWasGrounded = false;
	}
	//Wall Jumping
	if (Player[0].bCanWallJumpL || Player[0].bCanWallJumpR)
	{
		if (g_dSlideTime[0] < g_dElapsedTime)
		{
			Player[0].C.Y++;
			g_dSlideTime[0] = g_dElapsedTime + 1;
			Player[0].bGravity = false;
		}
	}
	if (isKeyPressed(0x57) && Player[0].C.Y > 0 && Player[0].bWasWallJ && Map[Player[0].C.X][Player[0].C.Y - 1].Code != 1)
	{
		Player[0].C.Y--;
		g_dBounceTime[0] = g_dElapsedTime + 0.125;
		Player[0].bWasWallJC = true;
	}
	Player[0].bWasWallJ = false;
	if (isKeyPressed(0x41) && Player[0].C.X > 0)
	{
		if (!Map[Player[0].C.X - 1][Player[0].C.Y].Active)
		{
			Player[0].C.X--;
		}
		else if (Map[Player[0].C.X - 1][Player[0].C.Y].Active && !Map[Player[0].C.X][Player[0].C.Y - 1].Active && !Map[Player[0].C.X - 1][Player[0].C.Y - 1].Active && Player[0].bIsGrounded)
		{
			Player[0].C.X--;
			Player[0].C.Y--;
		}
		else if (Map[Player[0].C.X - 1][Player[0].C.Y].Active)
		{
			Player[0].bCanWallJumpL = true;
			if (g_dElapsedTime > g_dSlideTime[0])
				g_dSlideTime[0] = g_dElapsedTime + 1;
		}
		Player[0].bSomethingHappened = true;
	}
	if (!Map[Player[0].C.X - 1][Player[0].C.Y].Active)
	{
		Player[0].bCanWallJumpL = false;
	}
	if (isKeyPressed(0x44) && Player[0].C.X < g_Console.getConsoleSize().X - 1)
	{
		if (!Map[Player[0].C.X + 1][Player[0].C.Y].Active)
		{
			Player[0].C.X++;
		}
		else if (Map[Player[0].C.X + 1][Player[0].C.Y].Active && !Map[Player[0].C.X][Player[0].C.Y - 1].Active && !Map[Player[0].C.X + 1][Player[0].C.Y - 1].Active && Player[0].bIsGrounded)
		{
			Player[0].C.X++;
			Player[0].C.Y--;
		}
		else if (Map[Player[0].C.X + 1][Player[0].C.Y].Active)
		{
			Player[0].bCanWallJumpR = true;
			if (g_dElapsedTime > g_dSlideTime[0])
				g_dSlideTime[0] = g_dElapsedTime + 1;
		}
		Player[0].bSomethingHappened = true;
	}
	if (!Map[Player[0].C.X + 1][Player[0].C.Y].Active)
	{
		Player[0].bCanWallJumpR = false;
	}
	if (isKeyPressed(0x57) && Player[0].C.Y > 0)
	{
		if (Map[Player[0].C.X][Player[0].C.Y - 1].Active || Player[0].sJump <= 0)
		{
			Player[0].bCanJump = false;
			Player[0].sJump = 0;
		}
		else if (Player[0].bWasGrounded)
		{
			Player[0].bCanJump = true;
		}
		//Beep(1440, 30);
		if (Player[0].bCanJump)
		{
			Player[0].C.Y -= 1;
			Player[0].sJump--;
		}
		else if (Player[0].bCanWallJumpL && !Map[Player[0].C.X - 1][Player[0].C.Y - 1].Active)
		{
			Player[0].C.X--;
			Player[0].C.Y--;
		}
		else if (Player[0].bCanWallJumpR && !Map[Player[0].C.X + 1][Player[0].C.Y - 1].Active)
		{
			Player[0].C.X++;
			Player[0].C.Y--;
		}
		else if (Player[0].bCanWallJumpL && !Map[Player[0].C.X + 1][Player[0].C.Y - 1].Active)
		{
			Player[0].C.X++;
			Player[0].C.Y--;
			Player[0].bCanWallJumpL = false;
			Player[0].bWasWallJ = true;
			Player[0].bWasWallJC = true;
		}
		else if (Player[0].bCanWallJumpR && !Map[Player[0].C.X - 1][Player[0].C.Y - 1].Active)
		{
			Player[0].C.X--;
			Player[0].C.Y--;
			Player[0].bCanWallJumpR = false;
			Player[0].bWasWallJ = true;
			Player[0].bWasWallJC = true;
		}
		Player[0].bSomethingHappened = true;
	}
	else
	{
		Player[0].bCanJump = false;
		Player[0].bWasGrounded = true;
	}

	if (isKeyPressed(0x53))
	{
		Player[0].bCanWallJumpL = false;
		Player[0].bCanWallJumpR = false;
		g_dSlideTime[0] = NULL;
		Player[0].bSomethingHappened = true;
	}
	/*if (g_abKeyPressed[K_SPACE])
	{
		Player[0].m_bActive = !Player[0].m_bActive;
		Player[0].bSomethingHappened = true;
	}*/
	if (Map[Player[0].C.X][Player[0].C.Y + 1].Active)
		Player[0].bIsGrounded = true;
	if (!Player[0].bIsGrounded && !Player[0].bCanJump && !Player[0].bCanWallJumpL && !Player[0].bCanWallJumpR && !Player[0].bWasWallJC && Player[0].bGravity)//Gravity
	{
		Player[0].C.Y++;
		Player[0].bSomethingHappened = true;
	}
	if (Player[0].bWasWallJC)
	{
		Player[0].bWasWallJC = false;
	}
		//Player interation with interactable objects
	if (Map[Player[0].C.X][Player[0].C.Y].Code == 5)
	{
		Player2Respawn(Player[0]);
	}
	if ((Map[Player[0].C.X][Player[0].C.Y].Code == 6 && !Map[Player[0].C.X][Player[0].C.Y].Active) || (Map[Player[0].C.X][Player[0].C.Y].Code == 7 && !Map[Player[0].C.X][Player[0].C.Y].Active))
	{
		Player[0].health--;
		HpUpdate(Player[0]);
		Player[0].bSomethingHappened = true;
	}		

	if (Player[0].bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime[0] = g_dElapsedTime + 0.125; // 125ms should be enough
	}
	if (Map[Player[0].C.X][Player[0].C.Y].Code == 4) {
		setRespawn(Player[0]);
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
	if (isKeyPressed(0x45)) {
		Player1Respawn(Player[1]);
		Player2Respawn(Player[0]);
	}
}

void clearScreen()
{
	// Clears the buffer with this colour attribute
	g_Console.clearBuffer(0x1F);
}

void renderSplashScreen()  // renders the splash screen
{
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
	std::ifstream map("Main_menu_Splash_Art.txt");
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
					g_Console.writeToBuffer(c, output[x], 0x09);
					break;
				}
			}
			++y;
		}
	}	
}

void renderGame()
{
	renderMap();        // renders the map to the buffer first
	renderCharacter();  // renders the character into the buffer
}
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
		break;
	}
	case 1: {
		std::string output;
		output.clear();
		std::ifstream map("map1.txt");
		if (map.is_open()) {
			int y = 1;
			while (getline(map, output)) {
				MapSetting(output, y);
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
				MapSetting(output, y);
				++y;
			}
		}
		break;
	}
	}
	
}

void MapReset() {
	COORD c;
	for (int x = 0; x < sMapWidth; ++x) {
		for (int y = 0; y < sMapHeight; ++y) {
			Map[x][y] = {' '};
		}
	}
}
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
				if (Map[x][y].LeverType == Lever) {
					g_Console.writeToBuffer(c, '/', 0x25C9);
				}
				else {
					g_Console.writeToBuffer(c, ' ', colors[1]);
				}
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
				g_Console.writeToBuffer(c, 'Û', colors[2]);
				break;
			case 6:
				Map[x][y].Code = 6;
				c.X = x;
				c.Y = y;
				if (Map[x][y].Active)
				{
				g_Console.writeToBuffer(c, (char)10, colors[12]);
				}
				else
				{
					g_Console.writeToBuffer(c, (char)15, colors[12]);
				}
				break;
			case 7:
				Map[x][y].Code = 7;
				c.X = x;
				c.Y = y;
				if (Map[x][y].Active)
				{
					g_Console.writeToBuffer(c, (char)10, colors[12]);
				}
				else
				{
					g_Console.writeToBuffer(c, (char)15, colors[12]);
				}
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
			case 9:
				Map[x][y].Code = 9;
				c.X = x;
				c.Y = y;
				g_Console.writeToBuffer(c, 'Û', colors[4]);
				break;
			}
			

		}
	}
	if (Map[Player[1].C.X][Player[1].C.Y].Code == 9&& Map[Player[0].C.X][Player[0].C.Y].Code == 9) {
		++level;
		MapReset();
		init();
	}
}

void renderCharacter()
{
	// Draw the location of the character
	WORD charColor = 0x0C;
	/*if (Player[1].m_bActive)
	{
		charColor = 0x0A;
	}*/
	g_Console.writeToBuffer(Player[1].C.X,Player[1].C.Y, (char)2, charColor);
	charColor = 0x0A;
	/*if (Player[0].m_bActive)
	{
		charColor = 0x0C;
	}*/
	g_Console.writeToBuffer(Player[0].C.X, Player[0].C.Y, (char)3, charColor);

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
	ss << 1.0 / g_dDeltaTime[0] << "fps";
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
