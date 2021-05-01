#pragma once
#include "TranspTable.h"
#include "Board.h"
#include "BoardConstants.h"
#include "Uci.h"
class Game
{
private:
	Move * bestmove;
	Board * tabuleiro;

public:
	Game();
	~Game();
};

