#include "pch.h"
#include "NegaClass.h"

#include <stack>


std::string NegaClass::printPV(Board * board, int depth)
{
	std::stack<Move> movs;
	std::string resultado = "";
	bool encontrouTabela;
	TranspItem retorno;
	

	while (depth > 0) {
		encontrouTabela = transp->recuperar(boardGetChave(board), 0, age, retorno);
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
	std::list<std::thread*> threads;
	SharedQueue<class NegaResult> results; 
	bool bAchou = false;
	std::list<std::thread *>::iterator it;
	age++;
	pararThreads = false;


	while (!timeShouldStop())
	{
		profundidade++;
		
		for (int i = 0; i < numberOfthreads;i++)
		{
			threads.push_back(new std::thread(negarun, i, &board, &results, profundidade, age));
		}
		
		
		while (!bAchou)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			
			if (!results.empty()) {
				resultadoTemp = results.pop_front();
				bAchou = true;
				transp->reiniciarMovsBuscados();
				pararThreads = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				pararThreads = false;
			}
		}
		for (it = threads.begin();it != threads.end();++it)
		{
			(*it)->join();
			delete *it;
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
			boardMakeMove(&board, &resultado.move);
			saida.append(resultado.move.ToAlgebra());
			saida.append(" ");
			saida.append(this->printPV(&board,10));
			boardUnmakeMove(&board, &resultado.move);
			uciEnviarComandoParaInterface(saida);
			
		}
	}

	return new Move(resultado.move);
	
	
}
