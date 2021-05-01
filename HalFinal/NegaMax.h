#pragma once
#include "Uci.h"
#include "Move.h"
#include "Board.h"
#include "TimeControl.h"
class NegaMax
{
private:
	int age;

public:
	NegaMax();
	~NegaMax();

	Move * go(Board * board);

};

