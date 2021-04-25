#pragma once
#include "BoardConstants.h"
#include "BlackMagic.h"

enum tipoMovimento {
	MNORMAL = 0,
	MDUPLO = 1,
	MCAP = 2,
	MCAPENPASSANT = 3,
	MROQUEQ = 4,
	MROQUEK = 5,
	MPROMO = 10,
	MPROMON = 11,
	MPROMOB = 12,
	MPROMOR = 13,
	MPROMOQ = 14,
	MPROMOCAP = 50,
	MPROMOCAPN = 51,
	MPROMOCAPB = 52,
	MPROMOCAPR = 53,
	MPROMOCAPQ = 54,
	MOVNENHUM = 100
};

struct Move
{
	unsigned long long bbFrom;
	unsigned long long bbTo;
	tipoPeca peca;
	tipoPeca pecaCap;
	tipoMovimento tipo;
	int indiceDe;
	int indicePara;
	unsigned int potencialRoque;
	int enPassant;
	int score;

	Move();

	Move(unsigned long long bbFrom, unsigned long long bbTo, tipoMovimento tipo, tipoPeca peca, tipoPeca pecaCap, int indiceDe, int indicePara);

	std::string ToAlgebra();
};



