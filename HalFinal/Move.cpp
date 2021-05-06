#include "pch.h"
#include "Move.h"


Move::Move()
{
	this->enPassant = -1;
	this->potencialRoque = 0;
}

Move::Move(unsigned long long bbFrom, unsigned long long bbTo, tipoMovimento tipo, tipoPeca peca, tipoPeca pecaCap, int indiceDe, int indicePara)
{
	this->bbFrom = bbFrom;
	this->bbTo = bbTo;
	this->tipo = tipo;
	this->peca = peca;
	this->pecaCap = pecaCap;
	this->indiceDe = indiceDe;
	this->indicePara = indicePara;
	this->score = 0;
	this->enPassant = -1;
	this->potencialRoque = 0;
}

std::string Move::ToAlgebra()
{
	std::string sb ="";
	tipoPeca pecaPromo = NENHUMA;
	if (!((tipo == MROQUEK) || (tipo == MROQUEQ)))
	{
		sb += blackMagicBBToString(this->bbFrom);
		sb += blackMagicBBToString(this->bbTo);
		if (tipo > MPROMOCAP)
		{
			pecaPromo = (tipoPeca)((int)tipo - (int)MPROMOCAP);
		}
		else if (tipo > MPROMO)
		{
			pecaPromo = (tipoPeca)((int)tipo - (int)MPROMO);
		}
		if (pecaPromo != NENHUMA)
		{
			sb += (sPecas[(int)pecaPromo]);
		}
	}
	else
	{
		if (tipo == MROQUEK) {
			if ((int)peca % 2 == 0)
			{
				sb +=("e1g1");
			}
			else
			{
				sb+=("e8g8");
			}
		}
		else
		{
			if ((int)peca % 2 == 0)
			{
				sb+= ("e1c1");
			}
			else
			{
				sb += ("e8c8");
			}
		}
	}
	return sb;
}

bool compareMoves(const Move move1, const Move move2)
{
	return move1.score>move2.score;
}
