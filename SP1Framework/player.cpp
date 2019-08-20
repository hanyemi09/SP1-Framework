#include "game.h"
#include "Framework/console.h"
#include "Framework/timer.h"

bool bWasGrounded = false;
bool bCanJump = true;
short sJump = 2;
short sDisplacementSinceGrounded = 0;
//Wall Jumping
bool bCanWallJumpR = false;
bool bCanWallJumpL = false;
bool bWasWallJ = false;
void moveCharacter()
{
	if (g_dBounceTime > g_dElapsedTime)
		return;
	bool bSomethingHappened = false;
	//Jumping
	bool bIsGrounded = false;
	if (g_abKeyPressed[K_DOWN] && Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y].Code == 2)
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
		bCanJump = true;
		sJump = 2;
		sDisplacementSinceGrounded = 0;
	}
	else if (!bWasWallJ)
	{
		sDisplacementSinceGrounded++;
	}
	if (sDisplacementSinceGrounded == 1)
		bWasGrounded = true;
	//Wall Jumping
	if (g_abKeyPressed[K_UP] && g_sChar[1].m_cLocation.Y > 0 && bWasWallJ && Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y - 1].Code != 1)
	{
		g_sChar[1].m_cLocation.Y--;
		g_dBounceTime = g_dElapsedTime + 0.125;
		bIsGrounded = true;
	}
	bWasWallJ = false;
	if (g_abKeyPressed[K_LEFT] && g_sChar[1].m_cLocation.X > 0)
	{
		if (!Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y].Active)
		{
			g_sChar[1].m_cLocation.X--;
		}
		else if (Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y].Active && !Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y - 1].Active && !Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y - 1].Active && bIsGrounded)
		{
			g_sChar[1].m_cLocation.X--;
			g_sChar[1].m_cLocation.Y--;
		}
		else if (Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y].Active)
		{
			bCanWallJumpL = true;
		}
		bSomethingHappened = true;
	}
	if (!Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y].Active)
	{
		bCanWallJumpL = false;
	}
	if (g_abKeyPressed[K_RIGHT] && g_sChar[1].m_cLocation.X < g_Console.getConsoleSize().X - 1)
	{
		if (!Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y].Active)
		{
			g_sChar[1].m_cLocation.X++;
		}
		else if (Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y].Active && !Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y - 1].Active && !Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y - 1].Active && bIsGrounded)
		{
			g_sChar[1].m_cLocation.X++;
			g_sChar[1].m_cLocation.Y--;
		}
		else if (Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y].Active)
		{
			bCanWallJumpR = true;
		}
		bSomethingHappened = true;
	}
	if (!Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y].Active)
	{
		bCanWallJumpR = false;
	}
	if (g_abKeyPressed[K_UP] && g_sChar[1].m_cLocation.Y > 0)
	{

		if (Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y - 1].Active || sJump <= 0)
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
		else if (bCanWallJumpL && !Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y - 1].Active)
		{
			g_sChar[1].m_cLocation.X--;
			g_sChar[1].m_cLocation.Y--;
		}
		else if (bCanWallJumpR && !Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y - 1].Active)
		{
			g_sChar[1].m_cLocation.X++;
			g_sChar[1].m_cLocation.Y--;
		}
		else if (bCanWallJumpL && !Map[g_sChar[1].m_cLocation.X + 1][g_sChar[1].m_cLocation.Y - 1].Active)
		{
			g_sChar[1].m_cLocation.X++;
			g_sChar[1].m_cLocation.Y--;
			bCanWallJumpL = false;
			bWasWallJ = true;
		}
		else if (bCanWallJumpR && !Map[g_sChar[1].m_cLocation.X - 1][g_sChar[1].m_cLocation.Y - 1].Active)
		{
			g_sChar[1].m_cLocation.X--;
			g_sChar[1].m_cLocation.Y--;
			bCanWallJumpR = false;
			bWasWallJ = true;
		}
		bSomethingHappened = true;
	}
	else
	{
		bCanJump = false;
		bWasGrounded = true;
	}

	if (g_abKeyPressed[K_DOWN])
	{
		bCanWallJumpL = false;
		bCanWallJumpR = false;
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_SPACE])
	{
		g_sChar[1].m_bActive = !g_sChar[1].m_bActive;
		bSomethingHappened = true;
	}
	if (Map[g_sChar[1].m_cLocation.X][g_sChar[1].m_cLocation.Y + 1].Active)
		bIsGrounded = true;
	if (!bIsGrounded && !bCanJump && !bCanWallJumpL && !bCanWallJumpR && !bWasWallJ)//Gravity
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