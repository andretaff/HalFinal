#include "pch.h"
#include "Board.h"

void boardAddPeca(Board * board, unsigned long long posicao, tipoPeca peca, int index)
{
	int cor = (int)peca % 2;
	board->bbs[(int)peca] ^= posicao;
	board->bbs[cor + PECAS] |= posicao;
	board->vMat[cor] += vPecas[(int)peca];
	if (peca < REI)
		board->vPos[cor] += pPos[(int)peca][index];
//	this.chave ^= transp.chaves[(int)peca, index];
}

void boardRemovePeca(Board * board, unsigned long long posicao, tipoPeca peca, int index)
{
	int cor = (int)peca % 2;
	board->bbs[(int)peca] ^= posicao;
	board->bbs[cor + PECAS] &= ~posicao;
	board->vMat[cor] -= vPecas[(int)peca];
	if (peca < REI)
		board->vPos[cor] -= pPos[(int)peca][index];

//	this.chave ^= transp.chaves[(int)peca, index];
}



bool boardCasaAtacada(Board * board, unsigned long long bb, int corAtacante)
{
	unsigned long long atacantes = board->bbs[PECAS + corAtacante];
	unsigned int index;
	unsigned long long todas = board->bbs[PECAS] | board->bbs[PECAS + 1];
	int iFrom;
	unsigned long long occ;

	iFrom = blackMagicIndex(bb);

	if ((aPeao[1 - corAtacante][iFrom] & board->bbs[PEAO + corAtacante]) != 0)
		return true;

	if ((mRei[iFrom] & board->bbs[(int)REI + corAtacante]) != 0)
		return true;

	if ((mCavalo[iFrom] & board->bbs[(int)CAVALO + corAtacante]) != 0)
		return true;

	index = torre[iFrom].posicao;
	occ = torre[iFrom].mascara | todas;
	occ *= torre[iFrom].fator;
	occ >>= (64 - 12);
	index = index + (unsigned int)occ;

	if ((tabela[index] & (board->bbs[(int)TORRE + corAtacante] | board->bbs[(int)RAINHA + corAtacante])) != 0)
		return true;

	index = bispo[iFrom].posicao;
	occ = bispo[iFrom].mascara | todas;
	occ *= bispo[iFrom].fator;
	occ >>= (64 - 9);
	index = index + (unsigned int)occ;
	// this.print();

	if ((tabela[index] & (board->bbs[(int)BISPO + corAtacante] | board->bbs[(int)RAINHA + corAtacante])) != 0)
		return true;

	return false;

}

unsigned long long  boardAtaquesACasa(Board * board, unsigned long long bb, int iFrom)
{
	unsigned long long ataques = 0;
	unsigned int index;
	unsigned long long todas = board->bbs[PECAS] | board->bbs[PECAS + 1];
	unsigned long long occ;

	ataques |= (aPeao[1][iFrom] & board->bbs[(int)PEAO]);
	ataques |= (aPeao[0][iFrom] & board->bbs[(int)PP]);
	ataques |= (mRei[iFrom] & (board->bbs[(int)REI] | board->bbs[(int)KP]));
	ataques |= (mCavalo[iFrom] & (board->bbs[(int)CAVALO] | board->bbs[(int)CP]));


	index = torre[iFrom].posicao;
	occ = torre[iFrom].mascara | todas;
	occ *= torre[iFrom].fator;
	occ >>= (64 - 12);
	index = index + (unsigned int)occ;

	ataques |= tabela[index] & (board->bbs[(int)TORRE] | board->bbs[(int)TP]
		| board->bbs[(int)RAINHA] | board->bbs[(int)KP]);

	index = bispo[iFrom].posicao;
	occ = bispo[iFrom].mascara | todas;
	occ *= bispo[iFrom].fator;
	occ >>= (64 - 9);
	index = index + (unsigned int)occ;

	ataques |= tabela[index] & (board->bbs[(int)BISPO] | board->bbs[(int)BP]
		| board->bbs[(int)RAINHA] | board->bbs[(int)KP]);

	return ataques;

}

bool boardIsChecked(Board * board)
{
	return boardCasaAtacada(board, board->bbs[(int)REI + board->corMover], 1 - board->corMover);
}

bool boardIsValid(Board * board)
{
	return !boardCasaAtacada(board,board->bbs[(int)REI + 1 - board->corMover], board->corMover);
}


void boardMakeMove(Board * board, Move * move)
{
	move->enPassant = board->enPassant;
	move->potencialRoque = board->potencialRoque;

	board->enPassant = -1;

	if ((board->potencialRoque & (ROQUE_REI_BRANCO | ROQUE_RAINHA_BRANCO)) != 0)
	{
		if (move->peca == REI)
			board->potencialRoque &= (ROQUE_REI_PRETO | ROQUE_RAINHA_PRETO);
		if (((move->bbFrom | move->bbTo) & I56) != 0)
			board->potencialRoque &= ~ROQUE_RAINHA_BRANCO;
		if (((move->bbFrom | move->bbTo) & I63) != 0)
			board->potencialRoque &= ~ROQUE_REI_BRANCO;
	}
	if ((board->potencialRoque & (ROQUE_REI_PRETO | ROQUE_RAINHA_PRETO)) != 0)
	{
		if (move->peca == KP)
			board->potencialRoque &= (ROQUE_REI_BRANCO | ROQUE_RAINHA_BRANCO);
		if (((move->bbFrom | move->bbTo) & I00) != 0)
			board->potencialRoque &= ~ROQUE_RAINHA_PRETO;
		if (((move->bbFrom | move->bbTo) & I07) != 0)
			board->potencialRoque &= ~ROQUE_REI_PRETO;
	}


	switch (move->tipo)
	{
	case MNORMAL:
	{
		boardRemovePeca(board,move->bbFrom, move->peca, move->indiceDe);
		boardAddPeca(board,move->bbTo, move->peca, move->indicePara);
	} break;
	case MDUPLO:
	{
		boardRemovePeca(board,move->bbFrom, move->peca, move->indiceDe);
		boardAddPeca(board,move->bbTo, move->peca, move->indicePara);
		if (board->corMover == 0)
			board->enPassant = move->indiceDe - 8;
		else
			board->enPassant = move->indiceDe + 8;
	} break;
	case MCAP:
	{
		//if (move->pecaCap == tipoPeca.NENHUMA)
		//{
		//    this.print();
		//    move->print();
		//}
		boardRemovePeca(board,move->bbFrom, move->peca, move->indiceDe);
		boardRemovePeca(board,move->bbTo, move->pecaCap, move->indicePara);
		boardAddPeca(board,move->bbTo, move->peca, move->indicePara);
	} break;
	case MCAPENPASSANT:
	{
		boardRemovePeca(board,move->bbFrom, move->peca, move->indiceDe);
		boardAddPeca(board,move->bbTo, move->peca, move->indicePara);
		if (board->corMover == 0)
			boardRemovePeca(board,move->bbTo << 8, move->pecaCap, move->indicePara + 8);
		else
			boardRemovePeca(board,move->bbTo >> 8, move->pecaCap, move->indicePara - 8);
	} break;

	case MROQUEK:
	{
		int cor = (int)move->peca % 2;
		if (cor == 0)
		{
			boardRemovePeca(board, I60, REI, 60);
			boardRemovePeca(board, I63, TORRE, 63);
			boardAddPeca(board, I62, REI, 62);
			boardAddPeca(board,I61, TORRE, 61);
			board->potencialRoque |= ROQUE_BRANCO;
		}
		else
		{
			boardRemovePeca(board,I04, KP, 4);
			boardRemovePeca(board,I07, TP, 7);
			boardAddPeca(board,I06, KP, 6);
			boardAddPeca(board,I05, TP, 5);
			board->potencialRoque |= ROQUE_PRETO;
		}
	} break;

	case MROQUEQ:
	{
		int cor = (int)move->peca % 2;
		if (cor == 0)
		{
			boardRemovePeca(board, I60, REI, 60);
			boardRemovePeca(board, I56, TORRE, 56);
			boardAddPeca(board, I58, REI, 58);
			boardAddPeca(board, I59, TORRE, 59);
			board->potencialRoque |= ROQUE_BRANCO;
		}
		else
		{
			boardRemovePeca(board,I04, KP, 4);
			boardRemovePeca(board, I00, TP, 0);
			boardAddPeca(board, I02, KP, 2);
			boardAddPeca(board, I03, TP, 3);
			board->potencialRoque |= ROQUE_PRETO;
		}
	}
	break;
	default:
	{
		if ((int)move->tipo > (int)MPROMOCAP)
		{
			tipoPeca promo = (tipoPeca)((int)move->tipo - (int)MPROMOCAP);
			boardRemovePeca(board,move->bbFrom, move->peca, move->indiceDe);
			boardRemovePeca(board,move->bbTo, move->pecaCap, move->indicePara);
			boardAddPeca(board,move->bbTo, promo, move->indicePara);
		}
		else
		{
			tipoPeca promo = (tipoPeca)((int)move->tipo - (int)MPROMO);
			boardRemovePeca(board, move->bbFrom, move->peca, move->indiceDe);
			boardAddPeca(board,move->bbTo, promo, move->indicePara);
		}

	} break;

	}
	board->corMover = 1 - board->corMover;
}



void boardUnmakeMove(Board * board, Move * move)
{
	board->enPassant = move->enPassant;
	board->potencialRoque = move->potencialRoque;
	switch (move->tipo) {
	case MNORMAL:
	{
		boardAddPeca(board,move->bbFrom, move->peca, move->indiceDe);
		boardRemovePeca(board,move->bbTo, move->peca, move->indicePara);
	} break;
	case MDUPLO:
	{
		boardAddPeca(board,move->bbFrom, move->peca, move->indiceDe);
		boardRemovePeca(board, move->bbTo, move->peca, move->indicePara);
	} break;
	case MCAP:
	{
		boardAddPeca(board,move->bbFrom, move->peca, move->indiceDe);
		boardAddPeca(board, move->bbTo, move->pecaCap, move->indicePara);
		boardRemovePeca(board, move->bbTo, move->peca, move->indicePara);
	}
	break;
	case tipoMovimento.MCAPENPASSANT:
	{
		this.addPeca(move->bbFrom, move->peca, move->indiceDe);
		this.removePeca(move->bbTo, move->peca, move->indicePara);
		if ((int)move->peca % 2 == 0)
			this.addPeca(move->bbTo << 8, move->pecaCap, move->indicePara + 8);
		else
			this.addPeca(move->bbTo >> 8, move->pecaCap, move->indicePara - 8);
	}
	break;


	case tipoMovimento.MROQUEK:
	{
		int cor = (int)move->peca % 2;
		if (cor == 0)
		{
			addPeca(bbConstants.I60, tipoPeca.REI, 60);
			addPeca(bbConstants.I63, tipoPeca.TORRE, 63);
			removePeca(bbConstants.I62, tipoPeca.REI, 62);
			removePeca(bbConstants.I61, tipoPeca.TORRE, 61);
		}
		else
		{
			addPeca(bbConstants.I04, tipoPeca.KP, 4);
			addPeca(bbConstants.I07, tipoPeca.TP, 7);
			removePeca(bbConstants.I06, tipoPeca.KP, 6);
			removePeca(bbConstants.I05, tipoPeca.TP, 5);
		}
	}
	break;

	case tipoMovimento.MROQUEQ:
	{
		int cor = (int)move->peca % 2;
		if (cor == 0)
		{
			addPeca(bbConstants.I60, tipoPeca.REI, 60);
			addPeca(bbConstants.I56, tipoPeca.TORRE, 56);
			removePeca(bbConstants.I58, tipoPeca.REI, 58);
			removePeca(bbConstants.I59, tipoPeca.TORRE, 59);
		}
		else
		{
			addPeca(bbConstants.I04, tipoPeca.KP, 4);
			addPeca(bbConstants.I00, tipoPeca.TP, 0);
			removePeca(bbConstants.I02, tipoPeca.KP, 2);
			removePeca(bbConstants.I03, tipoPeca.TP, 3);
		}
	}
	break;
	default:
	{
		if ((int)move->tipo > (int)tipoMovimento.MPROMOCAP)
		{
			tipoPeca promo = (tipoPeca)((int)move->tipo - (int)tipoMovimento.MPROMOCAP);
			addPeca(move->bbFrom, move->peca, move->indiceDe);
			addPeca(move->bbTo, move->pecaCap, move->indicePara);
			removePeca(move->bbTo, promo, move->indicePara);
		}
		else
		{
			tipoPeca promo = (tipoPeca)((int)move->tipo - (int)tipoMovimento.MPROMO);
			addPeca(move->bbFrom, move->peca, move->indiceDe);
			removePeca(move->bbTo, promo, move->indicePara);
		}

	}
	break;

	}
	this.corMover = 1 - corMover;

}