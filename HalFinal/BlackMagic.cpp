#include "pch.h"
#include "BlackMagic.h"
#include <math.h>
#include "BoardConstants.h"

int lsb_table[64] = { 63, 30, 3, 32, 59, 14, 11, 33, 60, 24, 50, 9, 55, 19, 21, 34, 61, 29, 2, 53, 51, 23, 41, 18, 56, 28, 1, 43, 46, 27, 0, 35, 62, 31, 58, 4, 5, 49, 54, 6, 15, 52, 12, 40, 7, 42, 45, 16, 25, 57, 48, 13, 10, 39, 8, 44, 20, 47, 38, 22, 17, 37, 36, 26 };

void blackMagicGenCaps()
{
	unsigned long long pos;
	int k;
	for (k = 0;k < 64;k++)
	{
		pos = (unsigned long long) pow(2, k);
		mCavalo[k] = ((pos & (~R8) & (~(C1 | C2))) << 6);
		mCavalo[k] = (pos & (~R8) & (~(C1 | C2))) << 6;
		mCavalo[k] = mCavalo[k] | (pos & (~R8) & (~(C7 | C8))) << 10;
		mCavalo[k] = mCavalo[k] | (pos & (~(R7 | R8)) & (~C8)) << 17;
		mCavalo[k] = mCavalo[k] | (pos & (~(R7 | R8)) & (~C1)) << 15;
		mCavalo[k] = mCavalo[k] | (pos & (~(R1 | R2)) & (~C8)) >> 15;
		mCavalo[k] = mCavalo[k] | (pos & (~(R1 | R2)) & (~C1)) >> 17;
		mCavalo[k] = mCavalo[k] | (pos & (~R1) & (~(C1 | C2))) >> 10;
		mCavalo[k] = mCavalo[k] | (pos & (~R1) & (~(C7 | C8))) >> 6;



		mRei[k] = (pos & (~R8)) << 8;
		mRei[k] = mRei[k] | (pos & (~R1)) >> 8;
		mRei[k] = mRei[k] | (pos & (~C1)) >> 1;
		mRei[k] = mRei[k] | (pos & (~C8)) << 1;
		mRei[k] = mRei[k] | (pos & (~(R8 | C1))) << 7;
		mRei[k] = mRei[k] | (pos & (~(R8 | C8))) << 9;
		mRei[k] = mRei[k] | (pos & (~(R1 | C1))) >> 9;
		mRei[k] = mRei[k] | (pos & (~(R1 | C8))) >> 7;

		aPeao[0][k] = (pos & (~C1)) >> 9;
		aPeao[0][k] = aPeao[0][k] | (pos & (~C8)) >> 7;
		aPeao[1][k] = (pos & (~C1)) << 7;
		aPeao[1][k] = aPeao[1][k] | (pos & (~C8)) << 9;
	}
}



void gerarOcupacoes(unsigned long long mascaraPrincipal, unsigned long long * mascaraAtual)
{
	unsigned long long posicao;
	int i;
	unsigned long long atual = 0;

	posicao = 1;
	unsigned long long zerar = 0;
	for (i = 0; i < 64; i++)
	{
		if (((posicao & (*mascaraAtual)) == 0) && ((posicao & mascaraPrincipal) != 0))
		{
			atual = 0;
			atual |= posicao;
			atual = atual | ((~zerar & (*mascaraAtual)));
			(*mascaraAtual) = atual;
			return;
		}


		posicao <<= 1;
		zerar |= posicao;
	}

	(*mascaraAtual) = 0;
}

unsigned long long blackMagicMovsTorre(unsigned long long pos, unsigned long long occ, bool ataques)
{
	unsigned long long mascara = 0;
	unsigned long long tPos = pos >> 1;
	if ((pos & C1) == 0)
	{
		while (((tPos & (C1 | occ)) == 0))
		{
			mascara |= tPos;
			tPos >>= 1;
		}
		if (ataques)
		{
			mascara |= tPos;
		}
	}

	if ((pos & R1) == 0)
	{
		tPos = pos >> 8;
		while (((tPos & (R1 | occ)) == 0))
		{
			mascara |= tPos;
			tPos >>= 8;
		}
		if (ataques)
		{
			mascara |= tPos;
		}
	}

	if ((pos & R8) == 0)
	{

		tPos = pos << 8;
		while (((tPos & (R8 | occ)) == 0))
		{
			mascara |= tPos;
			tPos <<= 8;
		}
		if (ataques)
		{
			mascara |= tPos;
		}
	}

	if ((pos & C8) == 0)
	{

		tPos = pos << 1;
		while (((tPos & (C8 | occ)) == 0))
		{
			mascara |= tPos;
			tPos <<= 1;
		}
		if (ataques)
		{
			mascara |= tPos;
		}
	}

	return mascara;


}


unsigned long long blackMagicMovsBipo(unsigned long long pos, unsigned long long occ, bool ataques)
{
	unsigned long long mascara = 0;
	unsigned long long tPos = pos >> 9;

	if ((pos & (R1 | C1)) == 0)
	{
		while (((tPos & (R1 | C1 | occ)) == 0))
		{
			mascara |= tPos;
			tPos >>= 9;
		}
		if (ataques)
		{
			mascara |= tPos;
		}
	}

	if ((pos & (R1 | C8)) == 0)
	{
		tPos = pos >> 7;
		while (((tPos & (R1 | C8 | occ)) == 0))
		{
			mascara |= tPos;
			tPos >>= 7;
		}
		if (ataques)
		{
			mascara |= tPos;
		}
	}

	if ((pos & (R8 | C8)) == 0)
	{
		tPos = pos << 9;
		while (((tPos & (R8 | C8 | occ)) == 0))
		{
			mascara |= tPos;
			tPos <<= 9;
		}
		if (ataques)
		{
			mascara |= tPos;
		}
	}

	if ((pos & (R8 | C1)) == 0)
	{
		tPos = pos << 7;
		while (((tPos & (R8 | C1 | occ)) == 0))
		{
			mascara |= tPos;
			tPos <<= 7;
		}
		if (ataques)
		{
			mascara |= tPos;
		}
	}
	return mascara;

}


void blackMagicIniciar()
{
	int i;
	unsigned long long pos;
	unsigned long long mascara;
	unsigned long long ataques;
	unsigned long long oc;
	unsigned long long posicao;

	blackMagicGenCaps();
	for (i = 0; i < 64; i++)
	{
		pos = (unsigned long long)pow(2, i);
		mascara = blackMagicMovsBipo(pos, 0, false);
		bispo[i].mascara = ~mascara;
		oc = 0;
		do
		{
			gerarOcupacoes(mascara, &oc);
			ataques = blackMagicMovsBipo(pos, oc, true);

			posicao = bispo[i].mascara | oc;
			posicao *= bispo[i].fator;
			posicao >>= (64 - 9);
			posicao = posicao + bispo[i].posicao;
			tabela[posicao] = ataques;


		} while (oc != 0);


		mascara = blackMagicMovsTorre(pos, 0, false);
		torre[i].mascara = ~mascara;
		oc = 0;
		do
		{
			gerarOcupacoes(mascara, &oc);
			ataques = blackMagicMovsTorre(pos, oc, true);

			posicao = torre[i].mascara | oc;
			posicao *= torre[i].fator;
			posicao >>= (64 - 12);
			posicao = posicao + torre[i].posicao;
			tabela[posicao] = ataques;


		} while (oc != 0);


	}


}

int blackMagicIndex(unsigned long long bb)
{
	unsigned int fold;

	bb = bb ^ (bb - 1);
	fold = (unsigned int)(bb ^ (bb >> 32));
	fold = ((fold * 0x78291ACF) >> 26);
	return lsb_table[fold];
}

std::string blackMagicBBToString(unsigned long long bb)
{
	std::string s = "";
	if ((bb & C1) != 0)
		s +="a";
	else if ((bb & C2) != 0)
		s += "b";
	else if ((bb & C3) != 0)
		s +="c";
	else if ((bb & C4) != 0)
		s +="d";
	else if ((bb & C5) != 0)
		s += "e";
	else if ((bb & C6) != 0)
		s +="f";
	else if ((bb & C7) != 0)
		s + "g";
	else if ((bb & C8) != 0)
		s += "h";

	if ((bb & R1) != 0)
		s += "8";
	else if ((bb & R2) != 0)
		s+="7";
	else if ((bb & R3) != 0)
		s+="6";
	else if ((bb & R4) != 0)
		s+="5";
	else if ((bb & R5) != 0)
		s+="4";
	else if ((bb & R6) != 0)
		s+="3";
	else if ((bb & R7) != 0)
		s+="2";
	else if ((bb & R8) != 0)
		s+="1";

	return s;
}


