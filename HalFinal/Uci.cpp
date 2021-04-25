#include "pch.h"
#include <iostream>;
#include <thread>
#include <chrono>
#include "Uci.h"
#include "SharedQueue.cpp"

SharedQueue<std::string> * filain;
SharedQueue<std::string> * filaout;

void ucirun() 
{
	std::string comando;
	filain = new SharedQueue<std::string>();
	filaout = new SharedQueue < std::string>();
	filain->push_back("ucinewgame");
	while (true) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (filain->size() > 0)
		{
			comando = filain->pop_front();
			filaout->push_back(comando);
			if (comando == "quit")
				break;
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
