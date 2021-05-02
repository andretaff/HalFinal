#include "pch.h"
#include "NegaThread.h"
#include <list>
#include "TimeControl.h"
#include "Avaliador.h"
#include <algorithm>

void liberarListaMovs(std::list<Move*> * moves) {
	std::list<Move*>::iterator it;
	for (it = moves->begin(); it != moves->end(); ++it)
		delete *it;
	delete moves;
}

void negarun(int id, Board * board, SharedQueue<NegaResult>* results, int maxply, int age)
{
	int nota;
	NegaResult result;
	NegaData * nega = new NegaData();
	nega->nodes = 0;
	nega->hits = 0;
	nega->board = new Board(*board);

	//this.tabuleiro.print();
	//Thread.Sleep(300);
	nota = negamax(nega, -99999999, +99999999, maxply, 0);
	result.nota = nota;
	result.move = *(nega->move);
	result.nodes = nega->nodes;
	result.hits = nega->hits;
	
	//Console.Out.WriteLine("Q " + this.quiesNodes.ToString());
	if ((!timeShouldStop()) && (results->size() == 0))
		results->push_back(result);
}


int negaquies(NegaData * nega, int alfa, int beta, int ply, int depth)
{
	int alfaOriginal = alfa;
	int valor;
	std::list<Move *> * moves = new std::list<Move*>();
	TranspItem retornoTabela;
	bool check;
	unsigned long long chaveLocal = boardGetChave(nega->board);
	bool moveu = false;
	Move * move;

	if (timeShouldStop())
		return 0;


	nega->nodes++;


	bool encontrouTabela = transp->recuperar(chaveLocal, 0, nega->age, retornoTabela);
	if (encontrouTabela)
	{
		nega->hits++;
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
		return avaliar(nega->board);
	check = boardIsChecked(nega->board);

	boardGerarMovimentos(nega->board, moves, !check);

	if ((moves->size() == 0) && (!check))
	{
		delete moves;
		return avaliar(nega->board);
	}
	else if ((moves->size() == 0) && (check))
	{
		delete moves;
		return -(CHECKMATE - depth);
	}
	//            Console.Out.WriteLine("-------------------------ANTES");
	//            foreach (Move move in moves)
	//                move.print();

	moves->sort(compareMoves);
	//            Console.Out.WriteLine("-------------------------");
	//            foreach (Move move in moves)
	//               move.print();
   //tabuleiro.print();

	std::list<Move*>::iterator it;
	for (it = moves->begin(); it != moves->end();++it)
	{
		move = *it;
		if (timeShouldStop())
			return 0;
		boardMakeMove(nega->board, move);
		if (!boardIsValid(nega->board))
		{
			//  this.tabuleiro.print();
			boardUnmakeMove(nega->board, move);
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

				boardUnmakeMove(nega->board, move);
				break;
			}
			boardUnmakeMove(nega->board, move);
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
		liberarListaMovs(moves);
		return -(CHECKMATE - depth);
	}

	else if (!moveu)
	{
		liberarListaMovs(moves);
		return avaliar(nega->board);
	}
	liberarListaMovs(moves);
	return alfa;
}



int negamax(NegaData * nega, int alpha, int beta, int ply, int depth)
{
	int alfaOriginal = alpha;
	int valor;
	int melhorValor;
	Move * melhorMov = 0;
	std::list<Move*> * moves = new std::list<Move*>();
	std::list<Move*> *atrasados = new std::list<Move*>();
	unsigned long long moveHash = 0;
	bool check;
	bool visitado = false;
	unsigned long long chaveLocal = boardGetChave(nega->board);


	if (timeShouldStop())
	{
		return 0;
	}

	nega->nodes++;
	bool encontrou;
	TranspItem retornoTabela;

	encontrou = transp->recuperar(chaveLocal, ply, nega->age, retornoTabela);

	if (encontrou)
	{
		nega->hits++;
		if (depth == 0)
		{
			melhorMov = new Move(retornoTabela.move);
			nega->move = melhorMov;
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
			alpha = std::max(alpha,retornoTabela.score);
		}
		if (alpha > beta)
		{
			return retornoTabela.score;
		}
		if  (retornoTabela.move.tipo != MOVNENHUM)
		{
			retornoTabela.move.score = SCORE_MOVE_HASH;
			moves->push_front(new Move(retornoTabela.move));
			moveHash = transp->moveHash(&retornoTabela.move);
		}
	}
	else if (retornoTabela.move.tipo != MOVNENHUM)
	{
		retornoTabela.move.score = SCORE_MOVE_HASH;
		moves->push_front(new Move(retornoTabela.move));
		moveHash = transp->moveHash(&retornoTabela.move);
	}
	valor = -99999999;
	melhorValor = -99999999;
	check = boardIsChecked(nega->board);
	if (check)
		ply++;
	//            if (ply == 0)
	//                return avaliador.avaliar(tabuleiro);

	boardGerarMovimentos(nega->board,moves, false);
	moves->sort(compareMoves);


	do
	{
		if (atrasados->size() != 0)
		{
			moves->clear();

			while (atrasados->size() > 0)
			{
				moves->push_back(atrasados->back());
				atrasados->pop_back();
			}
		}
		std::list<Move*>::iterator it;
		Move * move;
		for (it = moves->begin(); it != moves->end();++it)
		{
			move = *it;
			if ((visitado) && (moveHash == transp->moveHash(move)))
				continue;

			if (timeShouldStop())
			{
				liberarListaMovs(moves);
				delete atrasados;
				return 0;
			}
			if ((!transp->verificaSeBuscado(nega->id,chaveLocal,transp->moveHash(move),ply)))
			{
				atrasados->push_back(move);
				continue;
			}

			boardMakeMove(nega->board, move);
			//tabuleiro.print();
			if (!boardIsValid(nega->board))
			{
				boardUnmakeMove(nega->board,move);
				transp->retiraMov(nega->id, ply);
			}
			else
			{
				if (ply > 1)
					valor = -negamax(nega, -beta, -alpha, ply - 1, depth + 1);
				else
					valor = -negaquies(nega,-beta, -alpha, MAXQUIES, depth + 1);
				if (valor > melhorValor)
				{
					melhorMov = move;
					melhorValor = valor;
				}
				if (valor > alpha)
					alpha = valor;
				if (alpha > beta)
				{

					boardUnmakeMove(nega->board,move);
					transp->retiraMov(nega->id,ply);
					
					   if (chaveLocal != boardGetChave(nega->board))
					   {
						   throw std::exception("erro");
					   }
					break;
				}
				boardUnmakeMove(nega->board,move);
				transp->retiraMov(nega->id, ply);
				// if (chaveLocal != tabuleiro.getChave())
				// {
				//     tabuleiro.print();
				//     move.print();
				// }
			}
			visitado = visitado || transp->moveHash(move) == moveHash;
		}
	} while (atrasados->size() != 0);


	if (melhorMov == 0)
	{
		liberarListaMovs(moves);
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
		if (!timeShouldStop())
		{
			nega->move = new Move(*melhorMov);
		}
		else
		{ }
	}
	if (check)
		ply--;

	TranspItem itemT ;
	itemT.move = *melhorMov;
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
	liberarListaMovs(moves);

	return melhorValor;

}

