#include "pch.h"
#include "NegaClass.h"

#include <stack>


std::string NegaClass::printPV(Board * board, int depth)
{
	std::stack<Move> movs;
	std::string resultado = "";
	bool encontrouTabela;
	TranspItem retorno;

	//boardPrint(board);
	

	while (depth > 0) {
		encontrouTabela = transp->recuperar(boardGetChave(board), 0, 0, retorno);
		if ((encontrouTabela) && (retorno.tipo != MOVNENHUM)) {
			resultado.append(retorno.move.ToAlgebra());
			resultado.append(" ");
			depth--;
			boardMakeMove(board, &retorno.move);
			movs.push(retorno.move);

		}
		else
			break;
	}
	while (!movs.empty()) {
		Move movimento = movs.top();
		boardUnmakeMove(board, &movimento);
		movs.pop();
	}
	return resultado;
}

NegaClass::NegaClass()
{
	this->age = 0;
}


NegaClass::~NegaClass()
{
}

Move * NegaClass::go(Board board)
{
	int numberOfthreads = MAXTHREADS;
	int profundidade = 1;
	NegaResult resultadoTemp;
	NegaResult resultado;
	std::deque<std::thread*> threads;
	SharedQueue<class NegaResult> results; 
	bool bAchou = false;
	std::deque<std::thread *>::iterator it;
	Board * boards[MAXTHREADS];
	int i;
	age++;
	pararThreads = false;

	while (!timeShouldStop())
	{
		bAchou = false;
		profundidade++;
		results.clear();
		
		for (i = 0; i < numberOfthreads;i++)
		{
			boards[i] = new Board(board);

			//std::this_thread::sleep_for(std::chrono::milliseconds(25));
			threads.push_back(new std::thread(negarun, i, boards[i], &results, profundidade, age));
		}
		
		
		while ((!bAchou) && (!timeShouldStop()))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			
			if (!results.empty()) {
				resultadoTemp = results.pop_front();
				bAchou = true;
				transp->reiniciarMovsBuscados();
				pararThreads = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				pararThreads = false;
			}
		}
		i = 0;
		for (it = threads.begin();it != threads.end();++it)
		{
			(*it)->join();
			delete *it;
			delete boards[i++];
		}

		threads.clear();

		if (!timeShouldStop())
		{
			std::string saida;
			resultado = resultadoTemp;
			boardMakeMove(&board, &resultado.move);
			saida.append("info ");
			saida.append("depth ");
			saida.append(std::to_string(profundidade));
			saida.append(" score cp ");
			saida.append(std::to_string(resultado.nota));
			saida.append(" nodes ");
			saida.append(std::to_string(resultado.nodes));
			saida.append(" time ");
			saida.append(std::to_string(timeEllapsed()));
			saida.append(" pv ");
			saida.append(resultado.move.ToAlgebra());
			saida.append(" ");
			saida.append(this->printPV(&board,10));
			boardUnmakeMove(&board, &resultado.move);
			uciEnviarComandoParaInterface(saida);
			
		}
	}

	return new Move(resultado.move);
	
	
}
