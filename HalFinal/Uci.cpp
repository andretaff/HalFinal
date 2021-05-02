#include "pch.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "Uci.h"
#include "SharedQueue.cpp"

SharedQueue<std::string> * filain;
SharedQueue<std::string> * filaout;
Game game;
std::thread * gameThread;

static bool threadParou;

void uciApresenta()
{
	uciEnviarComandoParaInterface("id name: HAL");
	uciEnviarComandoParaInterface("id author: André Taffarello");
	uciEnviarComandoParaInterface("uciok");
}

void ucigo(std::string comando) {
	if (gameThread != 0) {
		throw std::exception("Engine já iniciada");
	}
	if ((comando.find("infinite") != std::string::npos) || (comando == "go")) {
		game.start(TIME_INFINITO,0);
	}
	if (comando.find("movetime") != std::string::npos) {
		unsigned int msegundos = std::stoi(comando.substr(comando.find("movetime") + 9));
		game.start(TIME_LEVEL, msegundos);
	}
	gameThread = new std::thread(&Game::run,&game);
}

void cUciPosition(std::string comando)
{
	int movePos = comando.find("moves") - 1;
	std::string moveStr;
	if (comando.find("startpos") != std::string::npos)
	{
		game.newGame();
	}
	else
	{
		if (movePos < 0)
			movePos = comando.length() - 9;
		std::string fenstr = comando.substr(9, movePos);

		game.setFenPosition(fenstr);
		movePos = comando.find("moves") - 1;
	}
	if (movePos < 0)
	{
		movePos = comando.length();
		moveStr = "";
	}
	else
	{
		moveStr = comando.substr(movePos + 7, comando.length() - movePos - 7);
	}
	game.makeHumanMoves(moveStr);
}

void uciEliminarThread() {
	if (gameThread != 0) {
		gameThread->join();
		delete gameThread;
		gameThread = 0;
	}
}

void uciEnviarSinalThreadParou() {
	threadParou = true;
}


void ucirun() 
{
	std::string comando;
	filain = new SharedQueue<std::string>();
	filaout = new SharedQueue < std::string>();
	filain->push_back("ucinewgame");
	threadParou = false;
	while (true) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (threadParou)
		{
			threadParou = false;
			uciEliminarThread();
		}
		if (filain->size() > 0)
		{
			comando = filain->pop_front();
			//filaout->push_back(comando);
			if (comando == "quit")
			{
				game.stop();
				break;
			}

			if (comando == "ucinewgame")
			{
				game.newGame();
			}
			if (comando == "stop")
			{
				game.stop();
			}

			if (comando.find("position") != std::string::npos) {
				cUciPosition(comando);
			}

			if (comando.find("go") == 0)
				ucigo(comando);

			if (comando == "uci")
				uciApresenta();
		}
		if (filaout->size() > 0)
		{
			std::cout << filaout->pop_front().c_str()<<"\n";
		}
	}
	
}

void uciEnviarComandoParaEngine(std::string comando)
{
	filain->push_back(comando);
}

void uciEnviarComandoParaInterface(std::string comando)
{
	filaout->push_back(comando);
}
