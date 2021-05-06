#include "pch.h"
#include "Game.h"

Game::Game()
{
	blackMagicIniciar();
	transp = new TranspTable(50000);
	tabuleiro = new Board();
	timer = 0;
	negaclass = new NegaClass();
	fen = new FEN();
}


Game::~Game()
{
	delete tabuleiro;
	delete transp;
	delete fen;
	delete negaclass;

}

void Game::run()
{
	bestmove = negaclass->go(*tabuleiro);
	uciEnviarComandoParaInterface("bestmove " + bestmove->ToAlgebra());
	if (timer != 0) {
		timeStop();
		timer->join();
		delete timer;
		timer = 0;
	}
	uciEnviarSinalThreadParou();
}

void Game::stop()
{
	if (timer != 0) {
		timeStop();
	}
	
}

void Game::setFenPosition(std::string fenStr)
{
	fen->lerFen(tabuleiro, fenStr);
}

void Game::newGame()
{
		fen->tabuleiroPadrao(tabuleiro);
}

void Game::start(int tipo, unsigned long long milisecs)
{
	timeStart(tipo, milisecs);
	if (timer != 0)
		throw std::exception("timer já está rodando");

	timer = new std::thread(timeRun);
}

void Game::makeHumanMoves(std::string moves)
{
	std::string move;
	Move movimento;
	int pos;
	bool achou;
	ListaMovs movel;
	std::string move1, move2;

	trim(moves);
	while (moves != "")
	{
		pos = moves.find(" ");
		if (pos == std::string::npos)
			pos = moves.length();
		move = moves.substr(0, pos);
		move1 = move;
		caps(move1);
		moves = moves.substr(move.length(), moves.length() - move.length());
		trim(moves);
		trim(move);
		boardGerarMovimentos(tabuleiro, movel, false);
		achou = false;

		for (int i= 0; i< movel.size; i++)
		{
			movimento = movel.movs[i];
			move2 = movimento.ToAlgebra();
			caps(move2);
			if (move1 == move2)
			{
				achou = true;
				boardMakeMove(tabuleiro, &movimento);
				break;
			}
		}
		if (!achou)
		{
			throw std::exception("Movimento não encontrado");
		}
	}
}