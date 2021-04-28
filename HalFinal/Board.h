#pragma once
#include <list>
#include "BlackMagic.h"
#include "BoardConstants.h"
#include "DNA.h"
#include "Move.h"
#include "TranspTable.h"
struct Board
{
	unsigned long long bbs[16];
	int corMover;
	int vMat[2];
	int vPos[2];
	unsigned int potencialRoque;
	int enPassant = -1;
	unsigned long long chave;
};


void boardAddPeca(Board * board, unsigned long long posicao, tipoPeca peca, int index);
void boardRemovePeca(Board * board, unsigned long long posicao, tipoPeca peca, int index);
void boardGerarMovimentos(Board * board, std::list<Move*> * moves, bool quiet);
unsigned long long boardGetChave(Board * board);
bool boardIsChecked(Board * board)