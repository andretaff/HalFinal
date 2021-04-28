#include "pch.h"
#include "Avaliador.h"
#include "BlackMagic.h"
int avaliar(Board * tabuleiro)
{
	int mB, mP;
	mB = 0; mP = 0;

	if ((tabuleiro->vMat[0] + tabuleiro->vMat[1] > 1000))
	{
		mB = tabuleiro->vPos[0] + pPos[0][blackMagicIndex(tabuleiro->bbs[(int)REI])];
		mP = tabuleiro->vPos[1] + pPos[1][blackMagicIndex(tabuleiro->bbs[(int)KP])];
	}
	else
	{
		mB = tabuleiro->vPos[0] + pPos[0][blackMagicIndex(tabuleiro->bbs[(int)REI+2])];
		mP = tabuleiro->vPos[1] + pPos[1][blackMagicIndex(tabuleiro->bbs[(int)KP+2])];
	}
	
	if (tabuleiro->corMover == 0)
		return tabuleiro->vMat[0] + 1 - tabuleiro->vMat[1] + mB - mP;
	else
		return tabuleiro->vMat[1] + 1 - tabuleiro->vMat[0] + mP - mB;

}













