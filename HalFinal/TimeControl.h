#pragma once
#include "pch.h"

const int TIME_LEVEL = 0;
const int TIME_INFINITO = 1;
extern bool pararThreads;
extern bool interromperExecucao;

void timeStart(int tipo, unsigned long long milisecs);
unsigned long long timeEllapsed();
void timeStop();
void timeRun();
bool timeShouldStop();