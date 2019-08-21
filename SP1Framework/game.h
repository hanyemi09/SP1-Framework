#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include <fstream>
#include <chrono>
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#pragma comment( lib, "Winmm.lib" )


extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_SPACE,
    K_COUNT,
	K_ENTER,
	K_W,
	K_S,
	K_A,
	K_D
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_SPLASHSCREEN,
    S_GAME,
    S_COUNT,
	S_PAUSE
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

//variables for for map objects
enum LeverTypes
{
	Lever,
	PressurePlate
};
struct _Object
{
	short Code;
	short LeverType;
	bool Active=false;
	char Link;
};
struct PlayerVar
{
	//Jumping
	bool bIsGrounded = false;
	bool bWasGrounded = false;
	bool bCanJump = false;
	short sJump = 2;
	short sDisplacementSinceGrounded = 0;
	bool bSomethingHappened = false;
	//Wall Jumping
	bool bCanWallJumpR = false;
	bool bCanWallJumpL = false;
	bool bWasWallJ = false;//detection if player was Wall jumping
	bool bWasWallJC = false;//checking if player was Wall jumping
	//health
	short health = 3;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory
void ArrowAI();
void TrapAI();

void MapPrinting();         //Printing of the map
void scanMap(char _Link);	// checks map for activatable blocks and change state accordingly
void setRespawn(int PlayerNumber);// sets respawn point
void pausegame();			// pauses game
void splashScreenWait();    // waits for time to pass in splash screen
void gameplay();            // gameplay logic
void moveCharacter1();      // moves the character, collision detection, physics, etc
void moveCharacter2();		// moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void MainMenuMusic();
//void JumpMusic();
//void MovementSounds();
void health();

#endif // _GAME_H