#include "pch.h"
#include "Game.h"
#include "BlackMagic.h"


Game::Game()
{
	blackMagicIniciar();
	transp = new TranspTable(10000);
}


Game::~Game()
{
}
