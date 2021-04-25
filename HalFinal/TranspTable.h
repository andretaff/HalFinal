#pragma 
#include "Move.h"
#include <mutex>
#include <vector>
enum tipoTranspItem { SCORE_NENHUM, SCORE_EXATO, SCORE_UPPER, SCORE_LOWER };

struct TranspItem {
	tipoTranspItem tipo;
	int score;
	unsigned long long chave;
	unsigned int idade;
	int ply;
	Move move;
	TranspItem();
};

class TranspTable
{
private:
	struct movBusc
	{
		unsigned long long moveHash;
		unsigned long long hash;
	};
	unsigned long long chaves[12][64];
	unsigned long long chavesRoque[4];
	unsigned long long chaveWTM;
	unsigned long size;
	std::mutex * mtx;
	std::mutex lockMove;
	TranspItem * tabela;
	std::vector<movBusc*> movsBuscados[MAXTHREADS];

	void read();

public:
	TranspTable(unsigned long size);
	~TranspTable();

	void reiniciarMovsBuscados();
	bool verificaSeBuscado(int threadId, unsigned long long hash, unsigned long long moveHash, int ply);
	void armazenar(TranspItem item);
	bool recuperar(unsigned long chave, int ply, unsigned int idade, TranspItem * retorno);
	void retiraMov(int threadId, int ply);
	void clear();
};

