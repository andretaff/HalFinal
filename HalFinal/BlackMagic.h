#pragma once
#include <string>
struct magic
{
	unsigned long long fator;
	unsigned int posicao;
	unsigned long long mascara;
};

extern unsigned long long tabela[88508];
extern unsigned long long aPeao[2][64];
extern unsigned long long mCavalo[64];
extern unsigned long long mRei[64];


void blackMagicIniciar();
int blackMagicIndex(unsigned long long bb);
unsigned long long blackMagicGetBBIndex(int indice);
std::string blackMagicBBToString(unsigned long long bb);

void printBitBoard(unsigned long long bb);


extern magic bispo[64];
extern magic torre[64];