#include "game.h"
//self-explanitory
COORD Respawn;
void setRespawn(PlayerVar *Player)
{
	Respawn.X = Player->C.X;
	Respawn.Y = Player->C.Y;
}
void Player1Respawn(PlayerVar *Player)
{
	Player->C.X= Respawn.X;
	Player->C.Y = Respawn.Y;
	Player->health = 3;
}
void Player2Respawn(PlayerVar *Player)
{
	Player->C.X = Respawn.X - 1;
	Player->C.Y = Respawn.Y;
	Player->health = 3;
}

void HpUpdate(PlayerVar *Player)
{
	if (Player->health == 0)
	{
		Player1Respawn(Player);
	}

	if (Player->health == 0)
	{
		Player2Respawn(Player);
	}

}