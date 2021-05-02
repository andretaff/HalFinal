#include "pch.h"
#include "TimeControl.h"
#include <chrono>
#include <thread>


bool parar;
bool pararThreads;
bool interromperExecucao;
std::chrono::steady_clock::time_point begin;
long long duracao;
int tipoTimer;

void timeStart(int tipo, unsigned long long milisecs)
{
	parar = false;
	begin = std::chrono::steady_clock::now();
	duracao = milisecs;
	interromperExecucao = false;
	tipoTimer = tipo;
}

unsigned long long timeEllapsed()
{
	return 0;
}

void timeStop()
{
	interromperExecucao = true;
}

void timeRun()
{
	std::chrono::steady_clock::time_point end;
	if (tipoTimer == TIME_LEVEL) {
		while ((!parar) && (!interromperExecucao)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			end = std::chrono::steady_clock::now();
			if (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() > duracao)
				parar = true;
		}
	}
	else
	{
		while (!interromperExecucao)
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

bool timeShouldStop()
{
	return parar or pararThreads or interromperExecucao;
}
