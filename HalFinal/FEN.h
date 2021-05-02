#pragma once
#include "Board.h"
#include "BlackMagic.h"

class FEN
{
public:
	FEN();
	~FEN();

	void tabuleiroPadrao(Board * board);
	void lerFen(Board * board, std::string fenString);
};

