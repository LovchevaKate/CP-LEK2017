#pragma once

#define ID_MAXSIZE	9				//макс число символов идентификатора
#define SCOPED_ID_MAXSIZE   ID_MAXSIZE*2 //макс число символов идентификатор + область видимости
#define TI_MAXSIZE		4096			//макс число количество строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000	//значение по умолчанию для int
#define TI_STR_DEFAULT	0x00		//значение по умолчанию для sting
#define TI_NULLIDX		0xffffffff	//нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255
#define TI_NUM_MAXSIZE   8388607
#define TI_NUM_MINSIZE  -8388608

#define MAX_PARAMS_COUNT 3
#define CATSTRING_PARAMS_CNT 2
#define PLENGTH_PARAMS_CNT 1
#define CATSTRING_TYPE IT::IDDATATYPE::STR
#define PLENGTH_TYPE IT::IDDATATYPE::INT


namespace IT
{	//типы данных идентификаторов: числовой, строковый, без типа(для процедур), неопределенный
	enum IDDATATYPE { INT = 1, STR = 2, PROC = 3, UNDEF };
	//типы идентификаторов: переменная, функция, параметр, литерал, стандартная функция
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };
	//стандартные функции
	enum STDFNC { F_CATSTRING, F_PLENGTH, F_NOT_STD };

	static const IDDATATYPE CATSTRING_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };
	static const IDDATATYPE PLENGTH_PARAMS[] = { IT::IDDATATYPE::STR };

	struct Entry//экземпляр таблицы идентификаторов
	{
		int			idxfirstLE;				//индекс в таблице лексем		
		char		id[SCOPED_ID_MAXSIZE];	//идентификатор
		IDDATATYPE	iddatatype;				//тип данных
		IDTYPE		idtype;					//тип идентификатора
		union
		{
			int	vint;            	//значение integer
			struct
			{
				int len;							//количество символов
				char str[TI_STR_MAXSIZE - 1];		//символы
			} vstr;	//значение строки

			struct
			{
				int count;			// количество параметров функции
				IDDATATYPE *types;	//типы параметров функции
			} params;
		} value;	//значение идентификатора

		Entry()
		{
			value.vint = TI_INT_DEFAULT;
			value.vstr.len = NULL;
			value.params.count = NULL;
		};
	};
	struct IdTable		//экземпляр таблицы идентификаторов
	{
		int maxsize;	//ёмкость таблицы идентификаторов < TI_MAXSIZE
		int size;		//текущий размер таблицы идентификаторов < maxsize
		Entry* table;	//массив строк таблицы идентификаторов
	};

	IdTable Create(int size = NULL);	//ёмкость таблицы идентификаторов < TI_MAXSIZE

	void Add(					//добавить строку в таблицу идентификаторов
		IdTable &idtable,		//экземпляр таблицы идентификаторов
		Entry entry);			//строка таблицы идентификаторов 

	int isId(					//возврат: номер строки(если есть), TI_NULLIDX(если есть)
		IdTable &idtable,		//экземпляр таблицы идентификаторов
		char id[SCOPED_ID_MAXSIZE]);	//идентификатор

	bool SetValue(IT::Entry* entry, char* value);	//задать значение идентификатора
	bool SetValue(IT::IdTable& idtable, int index, char* value);
};