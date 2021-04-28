#include "pch.h"
#include "NegaThread.h"
#include <list>
#include "TimeControl.h"
#include "Avaliador.h"

void run(int id, Board board, SharedQueue<NegaResult>* results, int maxply, TranspTable * tabela, int age)
{
	int nota;
	NegaResult result;
	NegaData * nega = new NegaData();
	nega->nodes = 0;
	nega->hits = 0;

	//this.tabuleiro.print();
	//Thread.Sleep(300);
	nota = negamax(nega, -99999999, +99999999, maxply, 0);
	result.nota = nota;
	result.move = *(nega->move);
	result.nodes = nega->nodes;
	result.hits = nega->hits;
	
	//Console.Out.WriteLine("Q " + this.quiesNodes.ToString());
	if ((!timeShouldStop()) && (results->size() > 0))
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

	if (timeShouldStop())
		return 0;

	
	nega->nodes++;


	bool encontrouTabela = transp->recuperar(chaveLocal, 0, nega->age, retornoTabela);
	if (encontrouTabela)
	{
		nega->hits++;
		if (retornoTabela.tipo == SCORE_EXATO)
		{
			return retornoTabela..score;
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
	valor = -99999999;

	//nega->quiesNodes++;



	if (ply == 0)
		return avaliar(nega->board);
	check = boardIsChecked(nega->board);

	boardGerarMovimentos(nega->board, moves,!check);

	if ((moves->size() == 0) && (!check))
		return avaliar(nega->board);
	else if ((moves->size() == 0) && (check))
		return -(CHECKMATE - depth);
	//            Console.Out.WriteLine("-------------------------ANTES");
	//            foreach (Move move in moves)
	//                move.print();

	moves->sort()
	//            Console.Out.WriteLine("-------------------------");
	//            foreach (Move move in moves)
	//               move.print();
   //tabuleiro.print();
	foreach(Move move in moves)
	{
		//   move.print();
		unsafe
		{
			if ((!temporizador.IsAlive) || (*bParar))
			{
				return 0;
			}
		}
		this.tabuleiro.makeMove(move);
		if (!tabuleiro.isValido())
		{
			//  this.tabuleiro.print();
			this.tabuleiro.unmakeMove(move);
#if DEBUG
			if (chaveLocal != tabuleiro.getChave())
				chaveLocal = 0;
#endif
		}
		else
		{
			moveu = true;
			valor = Math.Max(valor, -NegaQuiet(-beta, -alfa, ply - 1, depth + 1));

			if (valor > alfa)
				alfa = valor;
			if (alfa > beta)
			{
#if DEBUG
				//                        if (chaveLocal != tabuleiro.getChave())
				//                            chaveLocal = 0;
#endif

				tabuleiro.unmakeMove(move);

				break;
			}
			tabuleiro.unmakeMove(move);
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
		return -(RESULT_CHECKMATE - depth);
	else if (!moveu)
	{
		return this.avaliador.avaliar(tabuleiro);
	}
	return alfa;
}