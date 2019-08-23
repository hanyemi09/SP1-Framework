#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include <fstream>
#include <chrono>
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include <vector>
#pragma comment( lib, "Winmm.lib" )
#include <sstream>

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
	bool Occupied = false;
	short Code;
	short LeverType;
	bool Solid;
	bool Active=false;
	char Link;
};
struct PlayerVar 
{
	COORD C;
	bool bGravity=false;
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
enum Direction
{
	A_LEFT,
	A_RIGHT
};
struct Arrow
{
	COORD C;
	bool Direction;
	double BounceTime = 0.0;
	double DeltaTime = 0.0;
	void MoveArrow(double ElapsedTime, _Object Map[][50])
	{
		if (!Map[C.X][C.Y].Solid)
		{
			if (Direction == A_RIGHT)
			{
				Map[C.X][C.Y].Code = 0;
				if (Map[C.X + 1][C.Y].Code == 0)
				{
					Map[C.X + 1][C.Y].Code = 6;
					Map[C.X + 1][C.Y].Solid = false;
					C.X++;
				}
			}
			if (Direction == A_LEFT)
			{
				Map[C.X][C.Y].Code = 0;
				if (Map[C.X - 1][C.Y].Code == 0)
				{
					Map[C.Y - 1][C.Y].Code = 7;
					Map[C.Y - 1][C.Y].Solid = false;
				}
			}
			BounceTime = ElapsedTime + 0.1;
		}
	}
};
std::vector<Arrow> Arrows;
struct Trap
{
	COORD C;
	bool Direction;
	double BounceTime = 0.0;
	double DeltaTime = 0.0;
	void CreateArrow(double ElapsedTime, std::vector<Arrow> Arrows,_Object Map[][50])
	{
		if (BounceTime > ElapsedTime)
			return;
		if (Direction == A_RIGHT)
		{
			Arrow temp = { C.X + 1,C.Y,A_RIGHT };
			Arrows.push_back(temp);
			Map[C.X + 1][C.Y].Code = 6;
			Map[C.X + 1][C.Y].Solid = false;
		}
		if (Direction == A_LEFT)
		{
			Arrow temp = { C.X - 1,C.Y,A_LEFT };
			Arrows.push_back(temp);
			Map[C.X - 1][C.Y].Code = 7;
			Map[C.X - 1][C.Y].Solid = false;
		}
		BounceTime = ElapsedTime + 1.5;
	}
};
std::vector<Trap> Traps;
void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory
        //Printing of the map

void PPFunc();
void MapPrinting();         //Printing of the map
void scanMap(char _Link);	// checks map for activatable blocks and change state accordingly
void setRespawn(PlayerVar *Player);// sets respawn point
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

void MapInitialise(int level,_Object Map[100][50]);
void MapReset(int sMapWidth, int sMapHeight, _Object Map[100][50]);

void HpUpdate(PlayerVar *Player);
void Player1Respawn(PlayerVar *Player);
void Player2Respawn(PlayerVar *Player);
#endif // _GAME_H