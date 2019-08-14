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
SGameChar g_sChar1;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
_Object Map[100][45] = {};


// Console object
Console g_Console(100, 45, "Game");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;
	g_sChar1.m_cLocation.X = 5;
	g_sChar1.m_cLocation.Y = 4;
	g_sChar.m_cLocation.X = 3; //g_Console.getConsoleSize().X / 2;
	g_sChar.m_cLocation.Y = 4; //g_Console.getConsoleSize().Y / 2;
    g_sChar.m_bActive = true;
	g_sChar1.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(8, 16, L"Consolas");
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
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
void getInput( void )
{    
    g_abKeyPressed[K_UP]     = isKeyPressed(VK_UP);
    g_abKeyPressed[K_DOWN]   = isKeyPressed(VK_DOWN);
    g_abKeyPressed[K_LEFT]   = isKeyPressed(VK_LEFT);
    g_abKeyPressed[K_RIGHT]  = isKeyPressed(VK_RIGHT);
    g_abKeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	g_abKeyPressed[K_ENTER] = isKeyPressed(0x0D);
	g_abKeyPressed[K_W] = isKeyPressed(0x57);
	g_abKeyPressed[K_A] = isKeyPressed(0x41);
	g_abKeyPressed[K_S] = isKeyPressed(0x53);
	g_abKeyPressed[K_D] = isKeyPressed(0x44);


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
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
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
	moveCharacter1();
                        // sound can be played here too.
}
COORD Respawn;
void setRespawn()
{
	Respawn.X = g_sChar.m_cLocation.Y;
	Respawn.Y= g_sChar.m_cLocation.X;
}

void playerRespawn()
{
	g_sChar.m_cLocation= Respawn;
}
bool bCanJump = true;
short sJump = 3;
void moveCharacter()
{
    bool bSomethingHappened = false;
	bool bIsGrounded = false;
	if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y + 1].Code == 1)
	{
		bIsGrounded = true;
		bCanJump=true;
	}
    if (g_dBounceTime > g_dElapsedTime)
        return;

    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character
	if (bIsGrounded)
	{
		sJump = 3;
	}
    if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0&& bCanJump == true)
    {
        //Beep(1440, 30);
		if(Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y-1].Code!=1)
		{ 
        g_sChar.m_cLocation.Y-=2;
		sJump--;
		}	
		if (sJump <= 0)
		{
			bCanJump = false;
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
		if (Map[g_sChar.m_cLocation.X-1][g_sChar.m_cLocation.Y].Code != 1)
		{
			g_sChar.m_cLocation.X--;
		}
			bSomethingHappened = true;
    }
	/*
    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
		if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y + 1] != "1")
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
        bSomethingHappened = true;
    }
    if (g_abKeyPressed[K_SPACE])
    {
        g_sChar.m_bActive = !g_sChar.m_bActive;
        bSomethingHappened = true;
    }

	if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y + 1].Code != 1)//Gravity
	{
		g_sChar.m_cLocation.Y++;
		bSomethingHappened = true;
	}
	if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y].Code == 5)
	{
		Sleep(500);
		playerRespawn();
	}

	if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y].Code == 4)
	{
		setRespawn();
	}
    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
    }
}
bool bCanJump1 = true;
short sJump1 = 3;
void moveCharacter1()
{
	bool bSomethingHappened1 = false;
	bool bIsGrounded1 = false;
	if (Map[g_sChar1.m_cLocation.X][g_sChar1.m_cLocation.Y + 1].Code == 1)
	{
		bIsGrounded1 = true;
		bCanJump1 = true;
	}
	if (g_dBounceTime > g_dElapsedTime)
		return;

	// Updating the location of the character based on the key press
	// providing a beep sound whenver we shift the character
	if (bIsGrounded1)
	{
		sJump1 = 3;
	}
	if (g_abKeyPressed[K_W] && g_sChar1.m_cLocation.Y > 0 && bCanJump1 == true)
	{
		//Beep(1440, 30);
		if (Map[g_sChar1.m_cLocation.X][g_sChar1.m_cLocation.Y - 1].Code != 1)
		{
			g_sChar1.m_cLocation.Y -= 2;
			sJump1--;
		}
		if (sJump <= 0)
		{
			bCanJump1 = false;
		}
		bSomethingHappened1 = true;
	}
	else
	{
		bCanJump1 = false;
	}
	if (g_abKeyPressed[K_A] && g_sChar1.m_cLocation.X > 0)
	{
		//Beep(1440, 30);
		if (Map[g_sChar1.m_cLocation.X - 1][g_sChar1.m_cLocation.Y].Code != 1)
		{
			g_sChar1.m_cLocation.X--;
		}
		bSomethingHappened1 = true;
	}
	/*
	if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
	{
		//Beep(1440, 30);
		if (Map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y + 1] != "1")
		{
			g_sChar.m_cLocation.Y++;
		}
			bSomethingHappened = true;
	}
	*/
	if (g_abKeyPressed[K_D] && g_sChar1.m_cLocation.X < g_Console.getConsoleSize().X - 1)
	{
		//Beep(1440, 30);
		if (Map[g_sChar1.m_cLocation.X + 1][g_sChar1.m_cLocation.Y].Code != 1)
		{
			g_sChar1.m_cLocation.X++;
		}
		bSomethingHappened1 = true;
	}
	if (g_abKeyPressed[K_SPACE])
	{
		g_sChar1.m_bActive = !g_sChar1.m_bActive;
		bSomethingHappened1 = true;
	}

	if (Map[g_sChar1.m_cLocation.X][g_sChar1.m_cLocation.Y + 1].Code != 1)//Gravity
	{
		g_sChar1.m_cLocation.Y++;
		bSomethingHappened1 = true;
	}
	if (Map[g_sChar1.m_cLocation.X][g_sChar1.m_cLocation.Y].Code == 5)
	{
		Sleep(500);
		playerRespawn();
	}

	if (Map[g_sChar1.m_cLocation.X][g_sChar1.m_cLocation.Y].Code == 4)
	{
		setRespawn();
	}
	if (bSomethingHappened1)
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

void renderMap()
{
	// Set up sample colours, and output shadings
	const WORD colors[] = {
		0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
		0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
	};

	COORD c;
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 45; ++i)
		{
			c.X = i;
			c.Y = 5 * j;
			colour(colors[i]);
			g_Console.writeToBuffer(c, "�", colors[0]);// ����
			Map[c.X][c.Y].Code = 1;

			//g_Console.writeToBuffer(c, " ����", colors[i]);

		}
		for (int i = 0; i < 45; ++i)
		{
			c.X = i+45;
			c.Y = 5 * j+3;
			colour(colors[i]);
			g_Console.writeToBuffer(c, "�", colors[0]);// ����
			Map[c.X][c.Y].Code = 1;

			//g_Console.writeToBuffer(c, " ����", colors[i]);

		}
		
		for (int i = 10; i < 50; i++)
		{
			c.X = i;
			c.Y = 2;
			colour(colors[i]);
			g_Console.writeToBuffer(c, "�", colors[3]);// ����
			Map[c.X][c.Y].Code = 5;
		}
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
	if (g_sChar1.m_bActive)
	{
		charColor = 0x0A;
	}
	g_Console.writeToBuffer(g_sChar1.m_cLocation, (char)2, charColor);

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
