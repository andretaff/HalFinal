#include "pch.h"
#include "TimeControl.h"
#include <chrono>
#include <thread>


bool parar;
std::chrono::steady_clock::time_point begin;
unsigned long long duracao;

void timeStart(int tipo, unsigned long long milisecs)
{
	parar = false;
	begin = std::chrono::steady_clock::now();
	duracao = milisecs;
}

unsigned long long timeEllapsed()
{
	return 0;
}

void timeStop()
{
	parar = true;
}

void timeRun()
{
	std::chrono::steady_clock::time_point end;
	while (!parar) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		end = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() > duracao)
			parar = true;
	}
}

bool timeShouldStop()
{
	return parar;
}
