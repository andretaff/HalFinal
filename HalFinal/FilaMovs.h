#pragma once
#include "Move.h"
#include <algorithm>
struct ListaMovs {
	int size;
	Move movs[258];
	ListaMovs()
	{
		size = 0;
	}
};

inline void listAdd(ListaMovs &list, const Move & move)
{
	list.movs[list.size++] = move;
}
inline void listClear(ListaMovs &list) {
	list.size = 0;
}
inline void listSort(ListaMovs & list) {
	std::sort(list.movs, list.movs + list.size, compareMoves);
}
