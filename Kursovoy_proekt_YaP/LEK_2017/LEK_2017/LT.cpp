#include "stdafx.h"

namespace LT
{

	LexTable Create(int size)
	{
		if (size > LT_MAXSIZE)
			throw ERROR_THROW(202);
		LexTable lextable;
		lextable.table = new Entry[lextable.maxsize = size];
		lextable.size = NULL;
		return  lextable;
	}

	void Add(LexTable &lextable, Entry entry)
	{
		if (lextable.size >= lextable.maxsize)
			throw ERROR_THROW(202);
		lextable.table[lextable.size++] = entry;
	}
}