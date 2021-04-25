#include "pch.h"
#include "BlackMagic.h"
#include <math.h>
#include "BoardConstants.h"

int lsb_table[64] = { 63, 30, 3, 32, 59, 14, 11, 33, 60, 24, 50, 9, 55, 19, 21, 34, 61, 29, 2, 53, 51, 23, 41, 18, 56, 28, 1, 43, 46, 27, 0, 35, 62, 31, 58, 4, 5, 49, 54, 6, 15, 52, 12, 40, 7, 42, 45, 16, 25, 57, 48, 13, 10, 39, 8, 44, 20, 47, 38, 22, 17, 37, 36, 26 };

unsigned long long tabela[88508];
unsigned long long aPeao[2][64];
unsigned long long mCavalo[64];
unsigned long long mRei[64];

magic torre[64] = { {  0x80280013ff84ffff,  10890 ,0 },
											  {  0x5ffbfefdfef67fff,  56054 ,0 },
											  {  0xffeffaffeffdffff,  67495 ,0 },
											  {  0x003000900300008a,  72797 ,0 },
											  {  0x0030018003500030,  17179 ,0 },
											  {  0x0020012120a00020,  63978 ,0 },
											  {  0x0030006000c00030,  56650 ,0 },
											  {  0xffa8008dff09fff8,  15929 ,0 },
											  {  0x7fbff7fbfbeafffc,  55905 ,0 },
											  {  0x0000140081050002,  26301 ,0 },
											  {  0x0000180043800048,  78100 ,0 },
											  {  0x7fffe800021fffb8,  86245 ,0 },
											  {  0xffffcffe7fcfffaf,  75228 ,0 },
											  {  0x00001800c0180060,  31661 ,0 },
											  {  0xffffe7ff8fbfffe8,  38053 ,0 },
											  {  0x0000180030620018,  37433 ,0 },
											  {  0x00300018010c0003,  74747 ,0 },
											  {  0x0003000c0085ffff,  53847 ,0 },
											  {  0xfffdfff7fbfefff7,  70952 ,0 },
											  {  0x7fc1ffdffc001fff,  49447 ,0 },
											  {  0xfffeffdffdffdfff,  62629 ,0 },
											  {  0x7c108007befff81f,  58996 ,0 },
											  {  0x20408007bfe00810,  36009 ,0 },
											  {  0x0400800558604100,  21230 ,0 },
											  {  0x0040200010080008,  51882 ,0 },
											  {  0x0010020008040004,  11841 ,0 },
											  {  0xfffdfefff7fbfff7,  25794 ,0 },
											  {  0xfebf7dfff8fefff9,  49689 ,0 },
											  {  0xc00000ffe001ffe0,  63400 ,0 },
											  {  0x2008208007004007,  33958 ,0 },
											  {  0xbffbfafffb683f7f,  21991 ,0 },
											  {  0x0807f67ffa102040,  45618 ,0 },
											  {  0x200008e800300030,  70134 ,0 },
											  {  0x0000008780180018,  75944 ,0 },
											  {  0x0000010300180018,  68392 ,0 },
											  {  0x4000008180180018,  66472 ,0 },
											  {  0x008080310005fffa,  23236 ,0 },
											  {  0x4000188100060006,  19067 ,0 },
											  {  0xffffff7fffbfbfff,  0 ,0 },
											  {  0x0000802000200040,  43566 ,0 },
											  {  0x20000202ec002800,  29810 ,0 },
											  {  0xfffff9ff7cfff3ff,  65558 ,0 },
											  {  0x000000404b801800,  77684 ,0 },
											  {  0x2000002fe03fd000,  73350 ,0 },
											  {  0xffffff6ffe7fcffd,  61765 ,0 },
											  {  0xbff7efffbfc00fff,  49282 ,0 },
											  {  0x000000100800a804,  78840 ,0 },
											  {  0xfffbffefa7ffa7fe,  82904 ,0 },
											  {  0x0000052800140028,  24594 ,0 },
											  {  0x00000085008a0014,  9513 ,0 },
											  {  0x8000002b00408028,  29012 ,0 },
											  {  0x4000002040790028,  27684 ,0 },
											  {  0x7800002010288028,  27901 ,0 },
											  {  0x0000001800e08018,  61477 ,0 },
											  {  0x1890000810580050,  25719 ,0 },
											  {  0x2003d80000500028,  50020 ,0 },
											  {  0xfffff37eefefdfbe,  41547 ,0 },
											  {  0x40000280090013c1,  4750 ,0 },
											  {  0xbf7ffeffbffaf71f,  6014 ,0 },
											  {  0xfffdffff777b7d6e,  41529 ,0 },
											  {  0xeeffffeff0080bfe,  84192 ,0 },
											  {  0xafe0000fff780402,  33433 ,0 },
											  {  0xee73fffbffbb77fe,  8555 ,0 },
											  {  0x0002000308482882,  1009 ,0 }
};

magic bispo[64] = { { 0x107ac08050500bff, 66157,0 },
													  { 0x7fffdfdfd823fffd,71730,0 },
													  {  0x0400c00fe8000200,  37781 ,0 },
													  {  0x103f802004000000,  21015 ,0 },
													  {  0xc03fe00100000000,  47590 ,0 },
													  {  0x24c00bffff400000,  835 ,0 },
													  {  0x0808101f40007f04,  23592 ,0 },
													  {  0x100808201ec00080,  30599 ,0 },
													  {  0xffa2feffbfefb7ff,  68776 ,0 },
													  {  0x083e3ee040080801,  19959 ,0 },
													  {  0x040180bff7e80080,  21783 ,0 },
													  {  0x0440007fe0031000,  64836 ,0 },
													  {  0x2010007ffc000000,  23417 ,0 },
													  {  0x1079ffe000ff8000,  66724 ,0 },
													  {  0x7f83ffdfc03fff80,  74542 ,0 },
													  {  0x080614080fa00040,  67266 ,0 },
													  {  0x7ffe7fff817fcff9,  26575 ,0 },
													  {  0x7ffebfffa01027fd,  67543 ,0 },
													  {  0x20018000c00f3c01,  24409 ,0 },
													  {  0x407e0001000ffb8a,  30779 ,0 },
													  {  0x201fe000fff80010,  17384 ,0 },
													  {  0xffdfefffde39ffef,  18778 ,0 },
													  {  0x7ffff800203fbfff,  65109 ,0 },
													  {  0x7ff7fbfff8203fff,  20184 ,0 },
													  {  0x000000fe04004070,  38240 ,0 },
													  {  0x7fff7f9fffc0eff9,  16459 ,0 },
													  {  0x7ffeff7f7f01f7fd,  17432 ,0 },
													  {  0x3f6efbbf9efbffff,  81040 ,0 },
													  {  0x0410008f01003ffd,  84946 ,0 },
													  {  0x20002038001c8010,  18276 ,0 },
													  {  0x087ff038000fc001,  8512 ,0 },
													  {  0x00080c0c00083007,  78544 ,0 },
													  {  0x00000080fc82c040,  19974 ,0 },
													  {  0x000000407e416020,  23850 ,0 },
													  {  0x00600203f8008020,  11056 ,0 },
													  {  0xd003fefe04404080,  68019 ,0 },
													  {  0x100020801800304a,  85965 ,0 },
													  {  0x7fbffe700bffe800,  80524 ,0 },
													  {  0x107ff00fe4000f90,  38221 ,0 },
													  {  0x7f8fffcff1d007f8,  64647 ,0 },
													  {  0x0000004100f88080,  61320 ,0 },
													  {  0x00000020807c4040,  67281 ,0 },
													  {  0x00000041018700c0,  79076 ,0 },
													  {  0x0010000080fc4080,  17115 ,0 },
													  {  0x1000003c80180030,  50718 ,0 },
													  {  0x2006001cf00c0018,  24659 ,0 },
													  {  0xffffffbfeff80fdc,  38291 ,0 },
													  {  0x000000101003f812,  30605 ,0 },
													  {  0x0800001f40808200,  37759 ,0 },
													  {  0x084000101f3fd208,  4639 ,0 },
													  {  0x080000000f808081,  21759 ,0 },
													  {  0x0004000008003f80,  67799 ,0 },
													  {  0x08000001001fe040,  22841 ,0 },
													  {  0x085f7d8000200a00,  66689 ,0 },
													  {  0xfffffeffbfeff81d,  62548 ,0 },
													  {  0xffbfffefefdff70f,  66597 ,0 },
													  {  0x100000101ec10082,  86749 ,0 },
													  {  0x7fbaffffefe0c02f,  69558 ,0 },
													  {  0x7f83fffffff07f7f,  61589 ,0 },
													  {  0xfff1fffffff7ffc1,  62533 ,0 },
													  {  0x0878040000ffe01f,  64387 ,0 },
													  {  0x005d00000120200a,  26581 ,0 },
													  {  0x0840800080200fda,  76355 ,0 },
													  {  0x100000c05f582008,  11140 ,0 }
};




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

unsigned long long blackMagicGetBBIndex(int indice)
{
	if (indice == 10)
		indice = 10;
	return (unsigned long long) ((unsigned long long) 1 )<< indice;
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


