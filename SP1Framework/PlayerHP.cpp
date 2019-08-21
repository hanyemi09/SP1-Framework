#include "game.h"

COORD Respawn;

void setRespawn(SGameChar g_sChar)
{
	Respawn.X = g_sChar.m_cLocation.X;
	Respawn.Y = g_sChar.m_cLocation.Y;
}
void Player1Respawn(SGameChar g_sChar)
{
	g_sChar.m_cLocation.X = Respawn.X;
	g_sChar.m_cLocation.Y = Respawn.Y;
}
void Player2Respawn(SGameChar g_sChar)
{
	g_sChar.m_cLocation.X = Respawn.X - 1;
	g_sChar.m_cLocation.Y = Respawn.Y;
}

void HpUpdate(PlayerVar Player, SGameChar g_sChar)
{
	if (Player.health == 0)
	{
		Player1Respawn(g_sChar);
		Player.health = 3;
	}

	if (Player.health == 0)
	{
		Player2Respawn(g_sChar);
		Player.health = 3;
	}

}