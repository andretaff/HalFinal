#include "pch.h"
#include "NegaThread.h"
#include <list>
#include "TimeControl.h"
#include "Avaliador.h"
#include <algorithm>

unsigned long nodes;
unsigned long hits;

void negarun(int id, Board * board, SharedQueue<NegaResult>* results, int maxply, int age)
{
	int nota;
	NegaResult result;
	NegaData * nega = new NegaData();
	nodes = 0;
	hits = 0;
	nega->board = new Board(*board);

	//this.tabuleiro.print();
	//Thread.Sleep(300);
	nega->id = id;
	result.nota = negamax(nega, -99999999, +99999999, maxply, 0);
	result.nodes = nodes;
	result.hits = hits;
	
	if (nega->move!=0)
		result.move = *(nega->move);
	
	//Console.Out.WriteLine("Q " + this.quiesNodes.ToString());
	if ((!timeShouldStop()) && (results->size() == 0))
		results->push_back(result);
}


int negaquies(NegaData * nega, int alfa, int beta, int ply, int depth)
{
	int alfaOriginal = alfa;
	int valor;
	ListaMovs moves;
	TranspItem retornoTabela;
	bool check;
	unsigned long long chaveLocal = boardGetChave(nega->board);
	bool moveu = false;
	Move move;
	moves.size = 0;

	if (timeShouldStop())

		return 0;


	nodes++;


	bool encontrouTabela = transp->recuperar(chaveLocal, 0, nega->age, retornoTabela);
	if (encontrouTabela)
	{
		hits++;
		if (retornoTabela.tipo == SCORE_EXATO)
		{
			return retornoTabela.score;
		}
		else if (retornoTabela.tipo == SCORE_UPPER)
		{
			if (beta > retornoTabela.score)
				beta = retornoTabela.score;
		}
		else if (retornoTabela.tipo == SCORE_LOWER)
		{
			if (alfa < retornoTabela.score)
				alfa = retornoTabela.score;
		}
		if (alfa > beta)
		{
			return retornoTabela.score;
		}
	}
	valor = -CHECKMATE;

	//nega->quiesNodes++;


	if (ply == 0)
	{
		return avaliar(nega->board);
	}
	check = boardIsChecked(nega->board);

	boardGerarMovimentos(nega->board, moves, !check);

	if ((moves.size == 0) && (!check))
	{
		return avaliar(nega->board);
	}
	else if ((moves.size == 0) && (check))
	{
		return -(CHECKMATE - depth);
	}
	//            Console.Out.WriteLine("-------------------------ANTES");
	//            foreach (Move move in moves)
	//                move.print();
	listSort(moves);

	//            Console.Out.WriteLine("-------------------------");
	//            foreach (Move move in moves)
	//               move.print();
   //tabuleiro.print();

	
	for (int i = 0; i<moves.size;i++)
	{
		move = moves.movs[i];
		if (timeShouldStop())
		{
			return 0;
		}
		boardMakeMove(nega->board, &move);
		if (!boardIsValid(nega->board))
		{
			//  this.tabuleiro.print();
			boardUnmakeMove(nega->board, &move);
		}
		else
		{
			moveu = true;
			valor = std::max(valor, -negaquies(nega, -beta, -alfa, ply - 1, depth + 1));

			if (valor > alfa)
				alfa = valor;
			if (alfa > beta)
			{
#if DEBUG
				//                        if (chaveLocal != tabuleiro.getChave())
				//                            chaveLocal = 0;
#endif

				boardUnmakeMove(nega->board, &move);
				break;
			}
			boardUnmakeMove(nega->board, &move);
#if DEBUG
			if (chaveLocal != tabuleiro.getChave())
			{
				Console.Out.WriteLine("Errooooo");
				move.print();
				tabuleiro.print();
			}
#endif

		}
	}
	if ((check) && (!moveu))
	{
		return -(CHECKMATE - depth);
	}

	else if (!moveu)
	{
		return avaliar(nega->board);
	}
	return alfa;
}



int negamax(NegaData * nega, int alpha, int beta, int ply, int depth)
{
	int alfaOriginal = alpha;
	int valor;
	int melhorValor;
	Move melhorMov;
	ListaMovs moves;
	ListaMovs atrasados;
	unsigned long long moveHash = 0;
	bool check;
	bool visitado = false;
	unsigned long long chaveLocal = boardGetChave(nega->board);


	if (timeShouldStop())
	{
		return 0;
	}


	bool encontrou;
	TranspItem retornoTabela;
	nodes++;
	encontrou = transp->recuperar(chaveLocal, ply, nega->age, retornoTabela);

	if (encontrou)
	{
		hits++;
		if (depth == 0)
		{
			melhorMov = retornoTabela.move;
			nega->move = new Move(melhorMov);
		}
		if (retornoTabela.tipo == SCORE_EXATO)
		{
			return retornoTabela.score;
		}
		else if (retornoTabela.tipo == SCORE_UPPER)
		{
			beta = std::min(beta, retornoTabela.score);

		}
		else if (retornoTabela.tipo == SCORE_LOWER)
		{
			alpha = std::max(alpha, retornoTabela.score);
		}
		if (alpha > beta)
		{
			return retornoTabela.score;
		}
		if (retornoTabela.move.tipo != MOVNENHUM)
		{
			retornoTabela.move.score = SCORE_MOVE_HASH;
			listAdd(moves, retornoTabela.move);
			moveHash = transp->moveHash(&retornoTabela.move);
		}
	}
	else if (retornoTabela.move.tipo != MOVNENHUM)
	{
		retornoTabela.move.score = SCORE_MOVE_HASH;
		listAdd(moves, retornoTabela.move);
		moveHash = transp->moveHash(&retornoTabela.move);
	}
	valor = -99999999;
	melhorValor = -99999999;
	check = boardIsChecked(nega->board);
	if (check)
		ply++;
	//            if (ply == 0)
	//                return avaliador.avaliar(tabuleiro);

	boardGerarMovimentos(nega->board, moves, false);
	listSort(moves);


	do
	{
		if (timeShouldStop())
			break;
		if (atrasados.size != 0)
		{
			listClear(moves);
			for (int i = 0; i < atrasados.size;i++)
			{
				listAdd(moves, atrasados.movs[i]);
			}
			listClear(atrasados);
		}
		Move move;
		for (int i=0;i<moves.size; i++)
		{
			move = moves.movs[i];
			if ((visitado) && (moveHash == transp->moveHash(&move)))
				continue;

			if (timeShouldStop())
				break;
			if (depth==0)
				if ((!transp->verificaSeBuscado(nega->id, chaveLocal, transp->moveHash(&move), ply)))
				{
					listAdd(atrasados, move);
					continue;
				}

			boardMakeMove(nega->board, &move);
			//tabuleiro.print();
			if (!boardIsValid(nega->board))
			{
				boardUnmakeMove(nega->board, &move);
				transp->retiraMov(nega->id, ply);
			}
			else
			{
				if (ply > 1)
					valor = -negamax(nega, -beta, -alpha, ply - 1, depth + 1);
				else
					valor = -negaquies(nega, -beta, -alpha, MAXQUIES, depth + 1);
				if (valor > melhorValor)
				{
					melhorMov = move;
					melhorValor = valor;

				}
				if (valor > alpha)
					alpha = valor;
				if (alpha > beta)
				{

					boardUnmakeMove(nega->board, &move);
					transp->retiraMov(nega->id, ply);

					if (chaveLocal != boardGetChave(nega->board))
					{
						throw std::exception("erro");
					}
					break;
				}
				boardUnmakeMove(nega->board, &move);
				transp->retiraMov(nega->id, ply);
				// if (chaveLocal != tabuleiro.getChave())
				// {
				//     tabuleiro.print();
				//     move.print();
				// }
			}
			visitado = visitado || transp->moveHash(&move) == moveHash;
		}
	} while (atrasados.size != 0);


	if (melhorMov.tipo == MOVNENHUM)
	{
		
		if (check)
		{
			return -(CHECKMATE - depth);
		}
		else
		{
			return STALEMATE;
		}
	}

	if (depth == 0)
	{
		if (melhorMov.tipo!=MOVNENHUM)
		{
			nega->move = new Move(melhorMov);
		}
	}
	if (check)
		ply--;

	if (!timeShouldStop())
	{

		TranspItem itemT;
		itemT.move = melhorMov;
		itemT.idade = nega->age;
		itemT.score = melhorValor;
		itemT.chave = chaveLocal;
		itemT.ply = ply;

		if (alpha <= alfaOriginal)
			itemT.tipo = SCORE_UPPER;
		else if (alpha > beta)
			itemT.tipo = SCORE_LOWER;
		else
			itemT.tipo = SCORE_EXATO;
		transp->armazenar(itemT);
	}
	return melhorValor;

}

