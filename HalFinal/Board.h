#pragma once
#include "BlackMagic.h"
#include "BoardConstants.h"
#include "DNA.h"
#include "Move.h"
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