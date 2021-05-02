#pragma once
class NegaClass;
#include "Uci.h"
#include "Move.h"
#include "Board.h"
#include "TimeControl.h"
#include "NegaThread.h"
class NegaClass
{
private:
	int age;

	std::string printPV(Board * board, int depth);

public:
	NegaClass();
	~NegaClass();

	Move * go(Board board);

};

