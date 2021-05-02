#pragma once
#include "Move.h"
#include <mutex>
#include <vector>

//TODO: Rever vector->list

struct TranspItem {
	int tipo;
	int score;
	unsigned long long chave;
	unsigned int idade;
	int ply;
	Move move;
	TranspItem();
};

class TranspTable
{
private:
	struct movBusc
	{
		unsigned long long moveHash;
		unsigned long long hash;
	};

	unsigned long size;
	std::mutex ** mtx;
	std::mutex lockMove;
	TranspItem ** transptable;
	std::vector<movBusc*> movsBuscados[MAXTHREADS];

	void read();

public:
	unsigned long long chaves[12][64];
	unsigned long long chavesRoque[4];
	unsigned long long chaveWTM;

	TranspTable(unsigned long size);
	~TranspTable();

	void reiniciarMovsBuscados();
	bool verificaSeBuscado(int threadId, unsigned long long hash, unsigned long long moveHash, int ply);
	void armazenar(TranspItem item);
	bool recuperar(unsigned long long chave, int ply, unsigned int idade, TranspItem &retorno);
	void retiraMov(int threadId, int ply);
	void clear();
	unsigned long long  moveHash(Move *move);
};

extern TranspTable * transp;

