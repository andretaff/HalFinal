#include "pch.h"
#include <fstream>
#include "TranspTable.h"


TranspTable::TranspTable(unsigned long size)
{
	this->size = size;
	this->read();
	this->tabela = new TranspItem[size];
	this->mtx = new std::mutex[size / 1000];
	lockMove.lock();
	for (int i = 0; i < MAXTHREADS; i++)
	{
		for (int j = 0; j < MAXPLY; j++)
		{
			movBusc * mov = new movBusc();
			mov->hash = 0;
			this->movsBuscados[i].push_back(mov);
		}
			
	}
	lockMove.unlock();
}

TranspTable::~TranspTable()
{
	delete tabela;
	delete mtx;
	lockMove.lock();
	for (int i = 0; i <4; i++)
		while (this->movsBuscados->size() > 0)
		{
			movBusc * mov = this->movsBuscados->back();
			this->movsBuscados->pop_back();
			delete mov;
		}

	lockMove.unlock();
}

TranspItem::TranspItem()
{
	this->idade = 0;
	this->move.tipo = MOVNENHUM;
	this->chave = 0;
}
void TranspTable::reiniciarMovsBuscados()
{
	lockMove.lock();
	for (int i =0; i<MAXTHREADS; i++)
		for (unsigned int j = 0; j < this->movsBuscados->size(); j++)
		{
			movBusc * move = this->movsBuscados[i].at(j);
			move->hash = 0;
		}
	lockMove.unlock();
}

bool TranspTable::verificaSeBuscado(int threadId, unsigned long long hash,unsigned long long moveHash, int ply)
{
	int i;
	for (i = 0; i<MAXTHREADS; i++)
		if (i != threadId)
		{
			if (movsBuscados[i][ply]->hash == 0)
				break;
			if ((movsBuscados[i][ply]->hash == hash) && (movsBuscados[i][ply]->moveHash == moveHash))
				return false;
		}
	lockMove.lock();
	movBusc * mov = movsBuscados[threadId][ply];
	mov->hash = hash;
	mov->moveHash = moveHash;
	lockMove.unlock();
	return true;
}

void TranspTable::retiraMov(int threadId,int ply)
{
	this->movsBuscados[threadId][ply]->hash = 0;
}

void TranspTable::clear()
{
	for (unsigned int i = 0; i < size; i++)
		this->tabela[i].chave = 0;
}

void TranspTable::read()
{
	std::ifstream arquivo("e:\\chaves.txt");
	std::string buffer;
	for (int i = 0; i<PECAS;i++)
		for (int j = 0; j < 64;j++)
		{
			std::getline(arquivo,buffer);
			chaves[i][j] = (unsigned long long) std::stoull(buffer.c_str());

		}
	for (int i = 0;i < 4;i++)
	{
		std::getline(arquivo,buffer);
		chavesRoque[i]= (unsigned long long) std::stoull(buffer.c_str());

	}
	std::getline(arquivo, buffer);
	chaveWTM = (unsigned long long) std::stoull(buffer.c_str());
	arquivo.close();
}


void TranspTable::armazenar(TranspItem item)
{
	unsigned int pos = (unsigned int)(item.chave % (unsigned long )size);
	unsigned int posLock = pos % 1000;

	this->mtx[posLock].lock();
	tabela[pos] = item;
	this->mtx[posLock].unlock();
}

bool TranspTable::recuperar(unsigned long chave, int ply, unsigned int idade, TranspItem * retorno)
{
	TranspItem item;
	unsigned int pos = (unsigned int)(chave % (unsigned long)size);
	unsigned int posLock = pos % 1000;
	bool ok = false;
	this->mtx[posLock].lock();
	item = tabela[pos];
	if (item.chave == 0)
	{
		retorno = 0;
		return false;
	}
	if ((chave == item.chave) &&
		(ply <= item.ply) &&
		(idade <= item.idade + 1))
	{
		retorno = new TranspItem(item);
		ok = true;
	}
	else if (chave == item.chave)
	{
		ok = false;
		retorno = new TranspItem(item);
	}
	else
	{
		retorno = 0;
		ok = false;
	}
	return ok;
}