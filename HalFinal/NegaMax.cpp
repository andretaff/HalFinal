#include "pch.h"
#include "NegaMax.h"
#include "NegaThread.h"
#include "Board.h"


NegaMax::NegaMax()
{
	this->age = 0;
}


NegaMax::~NegaMax()
{
}

Move * NegaMax::go(Board board)
{
	int numberOfthreads = MAXTHREADS;
	int profundidade = 1;
	std::list<std::thread*> threads;
	SharedQueue<class NegaResult> results; 
	bool bAchou = false;
	age++;

	while (!timeShouldStop())
	{
		profundidade++;
		for (int i = 0; i < numberOfthreads;i++)
		{
			threads.push_back(new std::thread(negarun, i, board, results, profundidade, age));
			threads.back()->join();
		}
		while (!bAchou)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			
			if (!results.empty()) {

			}
		}
	}

	
	
}
