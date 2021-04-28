#pragma once
#include <string>
enum tipoPeca
{
	PEAO = 0,
	PP = 1,
	CAVALO = 2,
	CP = 3,
	BISPO = 4,
	BP = 5,
	TORRE = 6,
	TP = 7,
	RAINHA = 8,
	RP = 9,
	REI = 10,
	KP = 11,
	NENHUMA = 15
};

	 const int todosBBs = 16;
	 const int ATB = 14;
	 const int ATP = 15;
	 const int PECAS = 12;
	 const std::string sPecas = "PpNnBbRrQqKk";

	 const unsigned int MAXPLY = 15;
	 const unsigned int MAXTHREADS = 4;

	 const int CHECKMATE = 999999;

	 const unsigned int ROQUE_REI_BRANCO = 1;    //01b
	 const  unsigned int ROQUE_RAINHA_BRANCO = 2; //10b
	 const  unsigned int ROQUE_REI_PRETO = 4;
	 const  unsigned int ROQUE_RAINHA_PRETO = 8;
	 const  unsigned int ROQUE_BRANCO = 16;
	 const  unsigned int ROQUE_PRETO = 32;

	 const unsigned int SCORE_MOVE_HASH = 700;


	 const unsigned long long R1 = 0x00000000000000FF;
	 const unsigned long long R2 = 0x000000000000FF00;
	 const unsigned long long R3 = 0x0000000000FF0000;
	 const unsigned long long R4 = 0x00000000FF000000;
	 const unsigned long long R5 = 0x000000FF00000000;
	 const unsigned long long R6 = 0x0000FF0000000000;
	 const unsigned long long R7 = 0x00FF000000000000;
	 const unsigned long long R8 = 0xFF00000000000000;

	 const unsigned long long C1 = 0x0101010101010101;
	 const unsigned long long C2 = 0x0202020202020202;
	 const unsigned long long C3 = 0x0404040404040404;
	 const unsigned long long C4 = 0x0808080808080808;
	 const unsigned long long C5 = 0x1010101010101010;
	 const unsigned long long C6 = 0x2020202020202020;
	 const unsigned long long C7 = 0x4040404040404040;
	 const unsigned long long C8 = 0x8080808080808080;

	 const unsigned long long I00 = C1 & R1;
	 const unsigned long long I01 = C2 & R1;
	 const unsigned long long I02 = C3 & R1;
	 const unsigned long long I03 = C4 & R1;
	 const unsigned long long I04 = C5 & R1;
	 const unsigned long long I05 = C6 & R1;
	 const unsigned long long I06 = C7 & R1;
	 const unsigned long long I07 = C8 & R1;
	 const unsigned long long I55 = C8 & R7;

	 const unsigned long long I56 = C1 & R8;
	 const unsigned long long I57 = C2 & R8;
	 const unsigned long long I58 = C3 & R8;
	 const unsigned long long I59 = C4 & R8;
	 const unsigned long long I60 = C5 & R8;
	 const unsigned long long I61 = C6 & R8;
	 const unsigned long long I62 = C7 & R8;
	 const unsigned long long I63 = C8 & R8;
