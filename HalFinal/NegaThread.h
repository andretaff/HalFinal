#pragma once
#include "Board.h"
#include "BoardConstants.h"
#include "SharedQueue.h"
#include "Move.h"
#include "TimeControl.h"
#include "TranspTable.h"
//TODO: Tentar remover move da negadata, deve ser possível usando a tabela de transposição no retorno


class NegaResult
{
public:
	int nota;
	unsigned long nodes;
	Move move;
	unsigned long hits;
};
struct NegaData {
	Board * board;
	int maxply;
	unsigned long nodes;
	unsigned long hits;
	unsigned int id;
	int age;
	Move * move;
};

void negarun(int id, Board * board, SharedQueue<NegaResult> * results, int maxply, int age);
int negamax(NegaData * nega, int alpha, int beta, int ply, int depth);
int negaquies(NegaData * nega, int alfa, int beta, int ply, int depth);
