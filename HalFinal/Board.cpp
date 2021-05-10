#include "pch.h"
#include "Board.h"
#include  <algorithm>
#include <iostream>


void boardPrint(Board * board)
{
		unsigned long long posicao = 1;
		int index = 0;
		std::string linha = "";
		std::cout << linha << std::endl;
		std::cout << linha << std::endl;
		do
		{
			int i;
			for (i = 0; i < PECAS; i++)
				if ((board->bbs[i] & posicao) != 0)
				{
					linha += sPecas[i];
					break;
				}
			if (i == PECAS)
				linha += " ";

			if ((index + 1) % 8 == 0)
			{
				std::cout << linha << std::endl;
				linha = "";
			}
			index++;
			posicao <<= 1;
		} while (index < 64);
		if (board->corMover == 0)
			std::cout<<"Branco para mover"<<std::endl;
		else
			std::cout << "Preto para mover"<<std::endl;

		std::cout << "EnPass " + std::to_string(board->enPassant) << std::endl;
		std::cout << "Roque  " + std::to_string(board->potencialRoque) << std::endl;
		std::cout << "Chave " + std::to_string(boardGetChave(board)) << std::endl;
}
void boardReset(Board * board)
{
	for (int i = 0; i < todosBBs; i++)
		board->bbs[i] = 0;
	board->enPassant = 0;
	board->potencialRoque = 0;
	board->vMat[0] = 0;
	board->vMat[1] = 0;
	board->vPos[0] = 0;
	board->vPos[1] = 0;
	board->corMover = 0;
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
 		| board->bbs[(int)RAINHA] | board->bbs[(int)RP]);

	index = bispo[iFrom].posicao;
	occ = bispo[iFrom].mascara | todas;
	occ *= bispo[iFrom].fator;
	occ >>= (64 - 9);
	index = index + (unsigned int)occ;

	ataques |= tabela[index] & (board->bbs[(int)BISPO] | board->bbs[(int)BP]
		| board->bbs[(int)RAINHA] | board->bbs[(int)RP]);

	return ataques;

}

void boardAddPecaHumana(Board * board, tipoPeca peca, int index)
{
	unsigned long long posicao = (unsigned long long) std::pow(2, index);
	boardAddPeca(board, posicao, peca, index);
}


void boardAddPeca(Board * board, unsigned long long posicao, tipoPeca peca, int index)
{
	int cor = (int)peca % 2;
	board->bbs[(int)peca] ^= posicao;
	board->bbs[cor + PECAS] |= posicao;
	board->vMat[cor] += vPecas[(int)peca];
	if (peca < REI)
		board->vPos[cor] += pPos[(int)peca][index];
	board->chave ^= transp->chaves[(int)peca][index];
}

void boardRemovePeca(Board * board, unsigned long long posicao, tipoPeca peca, int index)
{
	int cor = (int)peca % 2;
	board->bbs[(int)peca] ^= posicao;
	board->bbs[cor + PECAS] &= ~posicao;
	board->vMat[cor] -= vPecas[(int)peca];
	if (peca < REI)
		board->vPos[cor] -= pPos[(int)peca][index];

	board->chave ^= transp->chaves[(int)peca][index];
}


unsigned long long  boardRecuperarAtacanteMaisBarato(Board * board, unsigned long long ataques, int cor, tipoPeca * peca)
{
	int i;
	unsigned long long ataque;
	for (i = (int)PEAO + cor; i < PECAS; i += 2)
	{
		ataque = ataques & board->bbs[i];
		if (ataque != 0)
		{
			*peca = (tipoPeca)i;
			return (unsigned long long)((long long)ataque & -(long long)ataque);
		}
	}
	return 0;
}

unsigned long long boardAtaquesRaioX(Board * board, unsigned long long ocupacao, int iFrom,unsigned long long removidos)
{
	unsigned int index;
	unsigned long long occ;
	unsigned long long resposta;
	index = torre[iFrom].posicao;
	occ = torre[iFrom].mascara | ocupacao;
	occ *= torre[iFrom].fator;
	occ >>= (64 - 12);
	index = index + (unsigned int)occ;

	unsigned long long xRays = ocupacao & board->bbs[(int)TORRE] | board->bbs[(int)RAINHA] |
		board->bbs[(int)TP] | board->bbs[(int)RP];

	resposta = (tabela[index] & (xRays));

	index = bispo[iFrom].posicao;
	occ = bispo[iFrom].mascara | ocupacao;
	occ *= bispo[iFrom].fator;
	occ >>= (64 - 9);
	index = index + (unsigned int)occ;
	xRays = ocupacao & board->bbs[(int)BISPO] | board->bbs[(int)RAINHA] |
		board->bbs[(int)BP] | board->bbs[(int)RP];

	resposta |= (tabela[index] & xRays);

	return resposta & (~removidos);

}

int boardSee(Board * board, int toIndex, unsigned long long toBB, tipoPeca alvo, int fromIndex, unsigned long long fromBB, tipoPeca peca)
{
	int d = 0;
	int cor = (int)peca % 2;
	int ganho[32];
	unsigned long long ataquesPossiveis = board->bbs[(int)(PEAO)] | board->bbs[(int)(PP)] | board->bbs[(int)BISPO] | board->bbs[(int)(BP)]
		| board->bbs[(int)(TORRE)] | board->bbs[(int)(TP)] | board->bbs[(int)RAINHA] | board->bbs[(int)(RP)];



	unsigned long long occ = board->bbs[(int)PECAS] | board->bbs[(int)PECAS + 1];
	unsigned long long ataques = boardAtaquesACasa(board,toBB, toIndex);
	unsigned long long removidos =0;
	//boardPrint(board);

	ganho[d] = vPecas[(int)alvo];
	do
	{
		d++;
		ganho[d] = vPecas[(int)peca] - ganho[d - 1];
		if (std::max(-ganho[d - 1], ganho[d]) < 0) break;
		ataques &= ~fromBB;
		occ &= ~fromBB;
		removidos |= fromBB;
		if ((fromBB & ataquesPossiveis) != 0)
			ataques |= boardAtaquesRaioX(board,occ, toIndex,removidos);
		ataquesPossiveis &= ~fromBB;
		fromBB = boardRecuperarAtacanteMaisBarato(board, ataques, 1 - cor, &peca);
		cor = 1 - cor;
	} while (fromBB > 0);
	while ((--d) > 0)
		ganho[d - 1] = -std::max(-ganho[d - 1], ganho[d]);
	return ganho[0];
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
	case MCAPENPASSANT:
	{
		boardAddPeca(board,move->bbFrom, move->peca, move->indiceDe);
		boardRemovePeca(board,move->bbTo, move->peca, move->indicePara);
		if ((int)move->peca % 2 == 0)
			boardAddPeca(board,move->bbTo << 8, move->pecaCap, move->indicePara + 8);
		else
			boardAddPeca(board,move->bbTo >> 8, move->pecaCap, move->indicePara - 8);
	}
	break;


	case MROQUEK:
	{
		int cor = (int)move->peca % 2;
		if (cor == 0)
		{
			boardAddPeca(board, I60, REI, 60);
			boardAddPeca(board, I63, TORRE, 63);
			boardRemovePeca(board, I62, REI, 62);
			boardRemovePeca(board,I61, TORRE, 61);
		}
		else
		{
			boardAddPeca(board,I04, KP, 4);
			boardAddPeca(board, I07, TP, 7);
			boardRemovePeca(board, I06, KP, 6);
			boardRemovePeca(board, I05, TP, 5);
		}
	}
	break;

	case MROQUEQ:
	{
		int cor = (int)move->peca % 2;
		if (cor == 0)
		{
			boardAddPeca(board, I60, REI, 60);
			boardAddPeca(board, I56, TORRE, 56);
			boardRemovePeca(board, I58, REI, 58);
			boardRemovePeca(board, I59, TORRE, 59);
		}
		else
		{
			boardAddPeca(board, I04, KP, 4);
			boardAddPeca(board, I00, TP, 0);
			boardRemovePeca(board, I02, KP, 2);
			boardRemovePeca(board, I03, TP, 3);
		}
	}
	break;
	default:
	{
		if ((int)move->tipo > (int)MPROMOCAP)
		{
			tipoPeca promo = (tipoPeca)((int)move->tipo - (int)MPROMOCAP);
			boardAddPeca(board, move->bbFrom, move->peca, move->indiceDe);
			boardAddPeca(board, move->bbTo, move->pecaCap, move->indicePara);
			boardRemovePeca(board, move->bbTo, promo, move->indicePara);
		}
		else
		{
			tipoPeca promo = (tipoPeca)((int)move->tipo - (int)MPROMO);
			boardAddPeca(board,move->bbFrom, move->peca, move->indiceDe);
			boardRemovePeca(board, move->bbTo, promo, move->indicePara);
		}

	}
	break;

	}
	board->corMover = 1 - board->corMover;

}

tipoPeca boardGetPecaPosicao(Board * board, unsigned long long posicao, int cor)
{
	int i;

	for (i = cor; i < PECAS; i += 2)
	{
		if ((board->bbs[i] & posicao) != 0)
		{
			return (tipoPeca)i;
		}
	}
	return NENHUMA;
}

void boardGenMovsTorre(Board * board, tipoPeca peca, bool capturas, ListaMovs & moves, bool quiet = false)
{

	unsigned long long torres = board->bbs[(int)peca + board->corMover];
	unsigned long long movs;
	unsigned long long pFrom, pTo;
	int iFrom, iTo;
	tipoPeca pecaAtacada;
	unsigned long long amigas = board->bbs[PECAS + board->corMover];
	unsigned long long inimigas = board->bbs[PECAS + 1 - board->corMover];
	unsigned long long todas = amigas | inimigas;
	unsigned long long occ;
	Move move;
	unsigned int index;

	if (capturas)
	{
		while (torres > 0)
		{
			pFrom = (unsigned long long)((long long)torres & -(long long)torres);
			iFrom = blackMagicIndex(pFrom);
			index = torre[iFrom].posicao;
			occ = torre[iFrom].mascara | todas;
			occ *= torre[iFrom].fator;
			occ >>= (64 - 12);
			index = index + (unsigned int )occ;

			movs = tabela[index] & inimigas;
			while (movs > 0)
			{
				pTo = (unsigned long long)((long long)movs & -(long long)movs);
				iTo = blackMagicIndex(pTo);
				pecaAtacada = boardGetPecaPosicao(board,pTo, 1 - board->corMover);
				move = Move(pFrom, pTo, MCAP,
					(tipoPeca)((int)peca + board->corMover),
					pecaAtacada,
					iFrom,
					iTo
				);
				move.score = boardSee(board,iTo, pTo, pecaAtacada, iFrom, pFrom, (tipoPeca)((int)peca + board->corMover));
				if ((!quiet) || (move.score >= 0))
					listAdd(moves, move);

				movs = movs & (movs - 1);
			}
			torres = torres & (torres - 1);
		}
	}
	else
	{
		while (torres > 0)
		{
			pFrom = (unsigned long long)((long long)torres & -(long long)torres);
			iFrom = blackMagicIndex(pFrom);

			index = torre[iFrom].posicao;
			occ = torre[iFrom].mascara | todas;
			occ *= torre[iFrom].fator;
			occ >>= (64 - 12);
			index = index + (unsigned int )occ;

			movs = tabela[index] & ~todas;
			while (movs > 0)
			{
				pTo = (unsigned long long)((long long)movs & -(long long)movs);
				iTo = blackMagicIndex(pTo);
				move = Move(pFrom, pTo, MNORMAL,
					(tipoPeca)((int)peca + board->corMover),
					NENHUMA,
					iFrom,
					iTo
				);
				listAdd(moves, move);
				movs = movs & (movs - 1);
			}
			torres = torres & (torres - 1);
		}
	}
}


void boardGenMovsPeao(Board * board, bool capturas, ListaMovs & moves, bool quiet = false)
{
	Move move;
	if (capturas)
	{
		unsigned long long peoes = board->bbs[(int)PEAO + board->corMover];
		unsigned long long inimigas = board->bbs[PECAS + 1 - board->corMover];
		unsigned long long pFrom;
		unsigned long long ataques;
		unsigned long long pTo;
		tipoPeca pAtacada;
		unsigned long long promos;
		int iFrom, iTo;
		int j;

		if (board->enPassant > -1)
			inimigas |= blackMagicGetBBIndex(board->enPassant);

		while (peoes != 0)
		{
			pFrom = (unsigned long long)((long long)peoes & -(long long)peoes);
			iFrom = blackMagicIndex(pFrom);
			ataques = aPeao[board->corMover][iFrom] & inimigas;
			if (board->corMover == 0)
				promos = ataques & R1;
			else
				promos = ataques & R8;

			ataques &= ~promos;

			while (promos != 0)
			{
				pTo = (unsigned long long)((long long)promos & -(long long)promos);
				iTo = blackMagicIndex(pTo);
				pAtacada = boardGetPecaPosicao(board, pTo, 1 - board->corMover);
				for (j = (int)RAINHA + board->corMover; j > (int)PP; j -= 2)
				{
					move = Move(pFrom, pTo, (tipoMovimento)((int)MPROMOCAP + j),
						(tipoPeca)((int)PEAO + board->corMover),
						pAtacada,
						iFrom,
						iTo);
					move.score = boardSee(board,iTo, pTo, pAtacada, iFrom, pFrom, (tipoPeca)j);
					if ((!quiet) || (move.score >= 0))
						listAdd(moves, move);
				}

				promos = promos & (promos - 1);
			}

			while (ataques != 0)
			{
				pTo = (unsigned long long)((long long)ataques & -(long long)ataques);
				iTo = blackMagicIndex(pTo);
				if (iTo == board->enPassant)
				{
					move = Move(pFrom, pTo, MCAPENPASSANT,
						(tipoPeca)((int)PEAO + board->corMover),
						(tipoPeca)((int)PEAO + 1 - board->corMover),
						iFrom,
						iTo);
					move.score = boardSee(board, iTo, pTo, (tipoPeca)((int)PEAO + board->corMover), iFrom, pFrom, (tipoPeca)((int)PEAO + board->corMover));

				}
				else
				{
					pAtacada = boardGetPecaPosicao(board, pTo, 1 - board->corMover);
					// if (pAtacada == tipoPeca.NENHUMA)
					//     this.print();
					move = Move(pFrom, pTo, MCAP,
						(tipoPeca)((int)PEAO + board->corMover),
						pAtacada,
						iFrom,
						iTo);
					move.score = boardSee(board,iTo, pTo, pAtacada, iFrom, pFrom, (tipoPeca)((int)PEAO + board->corMover));
				}
				if ((!quiet) || (move.score >= 0))
					listAdd(moves, move);
				ataques = ataques & (ataques - 1);
			}
			peoes = peoes & (peoes - 1);
		}
	}
	else
	{
		if (board->corMover == 0)
		{
			unsigned long long amigas = board->bbs[PECAS + board->corMover];
			unsigned long long inimigas = board->bbs[PECAS + 1 - board->corMover];
			unsigned long long todas = amigas | inimigas;

			unsigned long long movs = (board->bbs[(int)PEAO] >> 8) & ~todas;
			unsigned long long movsDuplos = ((movs & R6) >> 8) & ~todas;
			unsigned long long promos = movs & R1;
			unsigned long long pFrom;
			int iFrom, iTo, j;

			movs &= ~promos;

			while (promos != 0)
			{
				pFrom = (unsigned long long )(((long long)promos & -(long long)promos) << 8);
				iFrom = blackMagicIndex(pFrom);
				iTo = iFrom - 8;
				for (j = (int)RAINHA + board->corMover; j > (int)PP; j -= 2)
				{
					move = Move(pFrom, pFrom >> 8, (tipoMovimento)((int)MPROMO + j),
						(tipoPeca)((int)PEAO + board->corMover),
						NENHUMA,
						iFrom,
						iTo);
					move.score = j - (int)PEAO;
					listAdd(moves, move);
				}
				promos &= (promos - 1);
			}


			while (movsDuplos > 0)
			{
				pFrom = (unsigned long long)(((long long)movsDuplos & -(long long)movsDuplos) << 16);
				iFrom = blackMagicIndex(pFrom);
				iTo = iFrom - 16;
				move = Move(pFrom, pFrom >> 16, MDUPLO, PEAO,
					NENHUMA, iFrom, iTo);
				listAdd(moves, move);
				movsDuplos = movsDuplos & (movsDuplos - 1);
			}

			while (movs > 0)
			{
				pFrom = (unsigned long long)(((long long)movs & -(long long)movs) << 8);
				iFrom = blackMagicIndex(pFrom);
				iTo = iFrom - 8;
				move = Move(pFrom, pFrom >> 8, MNORMAL, PEAO,
					NENHUMA, iFrom, iTo);
				listAdd(moves, move);
				movs = movs & (movs - 1);
			}
		}
		else
		{
			unsigned long long amigas = board->bbs[PECAS + board->corMover];
			unsigned long long inimigas = board->bbs[PECAS + 1 - board->corMover];
			unsigned long long todas = amigas | inimigas;
			unsigned long long movs = (board->bbs[(int)PP] << 8) & ~todas;
			unsigned long long promos = movs & R8;
			unsigned long long movsDuplos = ((movs & R3) << 8) & ~todas;
			unsigned long long pFrom;
			int iFrom, iTo, j;

			movs &= ~promos;

			while (promos != 0)
			{
				pFrom = (unsigned long long)(((long long)promos & -(long long)promos) >> 8);
				iFrom = blackMagicIndex(pFrom);
				iTo = iFrom + 8;
				for (j = (int)RAINHA + board->corMover; j > (int)PP; j -= 2)
				{
					move = Move(pFrom, pFrom << 8, (tipoMovimento)((int)MPROMO + j),
						(tipoPeca)((int)PEAO + board->corMover),
						NENHUMA,
						iFrom,
						iTo);
					listAdd(moves, move);

				}
				promos &= (promos - 1);
			}

			while (movsDuplos > 0)
			{
				pFrom = (unsigned long long)((long long)movsDuplos & -(long long)movsDuplos) >> 16;
				iFrom = blackMagicIndex(pFrom);
				iTo = iFrom + 16;
				move = Move(pFrom, pFrom << 16, MDUPLO, PP,
					NENHUMA, iFrom, iTo);
				listAdd(moves, move);
				movsDuplos = movsDuplos & (movsDuplos - 1);
			}

			while (movs > 0)
			{
				pFrom = (unsigned long long)((long long)movs & -(long long)movs) >> 8;
				iFrom = blackMagicIndex(pFrom);
				iTo = iFrom + 8;
				move = Move(pFrom, pFrom << 8, MNORMAL, PP,
					NENHUMA, iFrom, iTo);
				listAdd(moves, move);
				movs = movs & (movs - 1);
			}
		}
	}
}

void boardGenMovsCavalo(Board * board, bool capturas, ListaMovs & moves, bool quiet = false)
{
	unsigned long long cavalos = board->bbs[(int)CAVALO + board->corMover];
	unsigned long long movs;
	unsigned long long pFrom, pTo;
	int iFrom, iTo;
	tipoPeca pecaAtacada;
	unsigned long long amigas = board->bbs[PECAS + board->corMover];
	unsigned long long  inimigas = board->bbs[PECAS + 1 - board->corMover];
	unsigned long long  todas = amigas | inimigas;
	Move move;

	if (capturas)
	{
		while (cavalos > 0)
		{
			pFrom = (unsigned long long)((long long)cavalos & -(long long)cavalos);
			iFrom = blackMagicIndex(pFrom);
			movs = mCavalo[iFrom] & inimigas;
			while (movs > 0)
			{
				pTo = (unsigned long long)((long long)movs & -(long long)movs);
				iTo = blackMagicIndex(pTo);
				pecaAtacada = boardGetPecaPosicao(board,pTo, 1 - board->corMover);
				move = Move(pFrom, pTo, MCAP,
					(tipoPeca)((int)CAVALO + board->corMover),
					pecaAtacada,
					iFrom,
					iTo
				);
				move.score = boardSee(board,iTo, pTo, pecaAtacada, iFrom, pFrom, (tipoPeca)((int)CAVALO + board->corMover));
				if ((!quiet) || (move.score >= 0))
					listAdd(moves, move);
				movs = movs & (movs - 1);
			}
			cavalos = cavalos & (cavalos - 1);
		}
	}
	else
	{
		while (cavalos > 0)
		{
			pFrom = (unsigned long long)((long long)cavalos & -(long long)cavalos);
			iFrom = blackMagicIndex(pFrom);
			movs = mCavalo[iFrom] & ~todas;
			while (movs > 0)
			{
				pTo = (unsigned long long)((long long)movs & -(long long)movs);
				iTo = blackMagicIndex(pTo);
				move = Move(pFrom, pTo, MNORMAL,
					(tipoPeca)((int)CAVALO + board->corMover),
					NENHUMA,
					iFrom,
					iTo
				);
				listAdd(moves, move);
				movs = movs & (movs - 1);
			}
			cavalos = cavalos & (cavalos - 1);
		}
	}

}


void boardGenMovsBispo(Board * board, tipoPeca peca, bool capturas, ListaMovs & moves, bool quiet = false)
{

	unsigned long long bispos = board->bbs[(int)peca + board->corMover];
	unsigned long long movs;
	unsigned long long pFrom, pTo;
	int iFrom, iTo;
	tipoPeca pecaAtacada;
	unsigned long long amigas = board->bbs[PECAS + board->corMover];
	unsigned long long inimigas = board->bbs[PECAS + 1 - board->corMover];
	unsigned long long todas = amigas | inimigas;
	unsigned long long occ;
	Move move;
	unsigned int index;

	if (capturas)
	{
		while (bispos > 0)
		{
			pFrom = (unsigned long long)((long long)bispos & -(long long)bispos);
			iFrom = blackMagicIndex(pFrom);
			index = bispo[iFrom].posicao;
			occ = bispo[iFrom].mascara | todas;
			occ *= bispo[iFrom].fator;
			occ >>= (64 - 9);
			index = index + (unsigned int)occ;

			movs = tabela[index] & inimigas;
			while (movs > 0)
			{
				pTo = (unsigned long long)((long long)movs & -(long long)movs);
				iTo = blackMagicIndex(pTo);
				pecaAtacada = boardGetPecaPosicao(board,pTo, 1 - board->corMover);
				move = Move(pFrom, pTo, MCAP,
					(tipoPeca)((int)peca + board->corMover),
					pecaAtacada,
					iFrom,
					iTo
				);
				move.score = boardSee(board,iTo, pTo, pecaAtacada, iFrom, pFrom, (tipoPeca)((int)peca + board->corMover));
				if ((!quiet) || (move.score > 0))
					listAdd(moves, move);
				movs = movs & (movs - 1);
			}
			bispos = bispos & (bispos - 1);
		}
	}
	else
	{
		while (bispos > 0)
		{
			pFrom = (unsigned long long)((long long)bispos & -(long long)bispos);
			iFrom = blackMagicIndex(pFrom);

			index = bispo[iFrom].posicao;
			occ = bispo[iFrom].mascara | todas;
			occ *= bispo[iFrom].fator;
			occ >>= (64 - 9);
			index = index + (unsigned int )occ;

			movs = tabela[index] & ~todas;
			while (movs > 0)
			{
				pTo = (unsigned long long)((long long)movs & -(long long)movs);
				iTo = blackMagicIndex(pTo);
				move = Move(pFrom, pTo, MNORMAL,
					(tipoPeca)((int)peca + board->corMover),
					NENHUMA,
					iFrom,
					iTo
				);
				listAdd(moves, move);
				movs = movs & (movs - 1);
			}
			bispos = bispos & (bispos - 1);
		}
	}
}

void boardGenMovsRei(Board * board, bool capturas, ListaMovs & moves)
{
	unsigned long long reis = board->bbs[(int)REI + board->corMover];
	unsigned long long movs;
	unsigned long long pTo;
	int iFrom, iTo;
	tipoPeca pecaAtacada;
	unsigned long long amigas = board->bbs[PECAS + board->corMover];
	unsigned long long inimigas = board->bbs[PECAS + 1 - board->corMover];
	unsigned long long todas = amigas | inimigas;
	Move move;

	if (capturas)
	{
		iFrom = blackMagicIndex(reis);
		movs = mRei[iFrom] & inimigas;
		while (movs > 0)
		{
			pTo = (unsigned long long)((long long)movs & -(long long)movs);
			iTo =blackMagicIndex(pTo);
			pecaAtacada = boardGetPecaPosicao(board,pTo, 1 - board->corMover);
			move = Move(reis, pTo, MCAP,
				(tipoPeca)((int)REI + board->corMover),
				pecaAtacada,
				iFrom,
				iTo
			);
			listAdd(moves, move);
			movs = movs & (movs - 1);
		}
	}
	else
	{
		iFrom = blackMagicIndex(reis);
		movs = mRei[iFrom] & ~todas;
		while (movs > 0)
		{
			pTo = (unsigned long long)((long long)movs & -(long long)movs);
			iTo = blackMagicIndex(pTo);
			move = Move(reis, pTo, MNORMAL,
				(tipoPeca)((int)REI + board->corMover),
				NENHUMA,
				iFrom,
				iTo
			);
			listAdd(moves, move);
			movs = movs & (movs - 1);
		}
		if ((board->corMover == 0) && (((board->potencialRoque & (ROQUE_RAINHA_BRANCO | ROQUE_REI_BRANCO)) != 0)))
		{
			if ((board->potencialRoque & ROQUE_REI_BRANCO) != 0)
			{
				if (((todas & (I62 | I61)) == 0)
					&& (!boardCasaAtacada(board,I60, 1))
					&& (!boardCasaAtacada(board,I61, 1))
					&& (!boardCasaAtacada(board,I62, 1)))
				{
					move = Move(reis, I63,
						MROQUEK,
						REI, NENHUMA,
						60, 63);
					listAdd(moves, move);
				}
			}
			if ((board->potencialRoque & ROQUE_RAINHA_BRANCO) != 0)
			{
				if (((todas & (I57 | I58 | I59)) == 0)
					&& (!boardCasaAtacada(board,I56, 1))
					&& (!boardCasaAtacada(board,I57, 1))
					&& (!boardCasaAtacada(board,I58, 1))
					&& (!boardCasaAtacada(board,I59, 1)))
				{
					move = Move(reis, I56,
						MROQUEQ,
						REI, NENHUMA,
						60, 56);
					listAdd(moves, move);
				}

			}
		}
		else if ((board->corMover != 0) && (((board->potencialRoque & (ROQUE_RAINHA_PRETO | ROQUE_REI_PRETO)) != 0)))
		{
			if ((board->potencialRoque & ROQUE_REI_PRETO) != 0)
			{
				if (((todas & (I05 | I06)) == 0)
					&& (!boardCasaAtacada(board,I04, 0))
					&& (!boardCasaAtacada(board, I05, 0))
					&& (!boardCasaAtacada(board,I06, 0)))
				{
					move = Move(reis, I07,
						MROQUEK,
						KP, NENHUMA,
						4, 7);
					listAdd(moves, move);
				}
			}
			if ((board->potencialRoque & ROQUE_RAINHA_PRETO) != 0)
			{
				if (((todas & (I03 | I02 | I01)) == 0)
					&& (!boardCasaAtacada(board,I00, 0))
					&& (!boardCasaAtacada(board,I01, 0))
					&& (!boardCasaAtacada(board,I02, 0))
					&& (!boardCasaAtacada(board,I03, 0)))
				{
					move = Move(reis, I56,
						MROQUEQ,
						KP, NENHUMA,
						4, 0);
					listAdd(moves, move);
				}

			}

		}
	}

}


void boardGerarMovimentos(Board * board, ListaMovs & moves, bool quiet)
{
	boardGenMovsPeao(board,true, moves, quiet);
	boardGenMovsCavalo(board,true, moves, quiet);
	boardGenMovsBispo(board,BISPO, true, moves, quiet);
	boardGenMovsTorre(board,TORRE, true, moves, quiet);
	boardGenMovsBispo(board,RAINHA, true, moves, quiet);
	boardGenMovsTorre(board,RAINHA, true, moves, quiet);
	boardGenMovsRei(board,true, moves);

	if (!quiet)
	{
		boardGenMovsPeao(board,false, moves);
		boardGenMovsCavalo(board,false, moves);
		boardGenMovsBispo(board,BISPO, false, moves);
		boardGenMovsTorre(board,TORRE, false, moves);
		boardGenMovsBispo(board,RAINHA, false, moves);
		boardGenMovsTorre(board,RAINHA, false, moves);
		boardGenMovsRei(board, false, moves);
	}
}

unsigned long long boardGetChave(Board * board)
{
	unsigned long long chavereal = board->chave;
	if ((board->potencialRoque & ROQUE_RAINHA_BRANCO) != 0)
		chavereal ^= transp->chavesRoque[0];
	if ((board->potencialRoque & ROQUE_REI_BRANCO) != 0)
		chavereal ^= transp->chavesRoque[1];
	if ((board->potencialRoque & ROQUE_RAINHA_PRETO) != 0)
		chavereal ^= transp->chavesRoque[2];
	if ((board->potencialRoque & ROQUE_REI_PRETO) != 0)
		chavereal ^= transp->chavesRoque[3];
	if ((board->enPassant != -1))
		chavereal ^= transp->chaves[PECAS][board->enPassant];
	if (board->corMover == 1)
		chavereal ^= transp->chaveWTM;
	return chavereal;
}