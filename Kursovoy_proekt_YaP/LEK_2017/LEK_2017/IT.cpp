#include "stdafx.h"

#define W(x, y)\
		<< std::setw(x) << (y) <<
#define STR(n, line, type, id)\
		"|" W(4,n) " |  " W(5,line) "    |" W(17,type) " |  " W(SCOPED_ID_MAXSIZE, id) " |"


namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE)
			throw ERROR_THROW(203);
		IdTable idtable;
		idtable.table = new Entry[idtable.maxsize = size];
		idtable.size = NULL;
		return idtable;
	}

	void Add(IdTable &idtable, Entry entry)
	{
		if (idtable.size >= idtable.maxsize)
			throw ERROR_THROW(203);
		idtable.table[idtable.size++] = entry;
	}

	// возврат: номер строки(если есть), TI_NULLIDX(если нет)
	int isId(IdTable& idtable, char id[SCOPED_ID_MAXSIZE])
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (strcmp(idtable.table[i].id, id) == 0)
				return i;
		}
		return TI_NULLIDX;
	}

	bool SetValue(IT::IdTable& idtable, int index, char* value)
	{
		return SetValue(&(idtable.table[index]), value);
	}

	bool SetValue(IT::Entry* entry, char* value) // установка значения переменной
	{
		bool rc = true;
		if (entry->iddatatype == INT)
		{
			int temp = atoi(value);
			if (temp > TI_NUM_MAXSIZE || temp < TI_NUM_MINSIZE)
			{
				if (temp > TI_NUM_MAXSIZE) 
					temp = TI_NUM_MAXSIZE;
				if (temp < TI_NUM_MINSIZE)  
					temp = TI_NUM_MINSIZE;
				rc = false;
			}
			entry->value.vint = temp;
		}
		else
		{
			for (unsigned i = 1; i < strlen(value) - 1; i++)	// без кавычек
				entry->value.vstr.str[i - 1] = value[i];
			entry->value.vstr.str[strlen(value) - 2] = '\0';
			entry->value.vstr.len = strlen(value) - 2;
		}
		return rc;
	}
};