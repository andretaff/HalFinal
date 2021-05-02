#pragma once
#include "NegaClass.h"
#include "FEN.h"
#include "helpers.h"
class Game
{
private:
	FEN * fen;
	NegaClass *negaclass;

	Move * bestmove;
	Board * tabuleiro;
	std::thread * timer;

public:
	Game();
	~Game();

	void run();
	void newGame();
	void start(int tipo, unsigned long long milisecs);
	void makeHumanMoves(std::string moves);
	void stop();
	void setFenPosition(std::string fenStr);


};

