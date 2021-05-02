#include "pch.h"
#include "FEN.h"
#include <cctype>


FEN::FEN()
{
}


FEN::~FEN()
{
}

void FEN::tabuleiroPadrao(Board * board)
{

	lerFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
}

void FEN::lerFen(Board * board, std::string fenString)
{
	int tipo = 0;
	unsigned int i, j;
	char c;

	boardReset(board);

	i = 0;
	int posicao = 0;

	while (i < fenString.size())
	{
		c = fenString[i++];
		if (c == ' ')
		{
			tipo++;
			continue;
		}
		if (tipo == 0)
		{
			if (c == '/')
				continue;

			if (std::isdigit(c))
				posicao += ((int) c - '0');
			else
			{
				for (j = 0; j < PECAS; j++)
					if (sPecas[j] == c)
						break;
				boardAddPecaHumana(board, (tipoPeca)j, posicao);
				posicao++;
			}
		}
		if (tipo == 1) //roques
		{
			if (c == 'w')
				board->corMover = 0;
			else
				board->corMover = 1;
		}
		if (tipo == 2)
		{
			switch (c)
			{
			case 'q':
				board->potencialRoque |= ROQUE_RAINHA_PRETO;
				break;
			case 'Q':
				board->potencialRoque |= ROQUE_RAINHA_BRANCO;
				break;
			case 'k':
				board->potencialRoque |= ROQUE_REI_PRETO;
				break;
			case 'K':
				board->potencialRoque |= ROQUE_REI_BRANCO;
				break;
			case '-':
				break;
			}
		}
	}
}