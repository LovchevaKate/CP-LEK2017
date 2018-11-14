#include "stdafx.h"
#include "Error.h"
using namespace Error;
namespace Lexer
{
	Graph graphs[N_GRAPHS] =
	{
		{ LEX_SEPARATORS, FST::FST(GRAPH_SEPARATORS) },
	    { LEX_LITERAL, FST::FST(GRAPH_INT_LITERAL) },
	    { LEX_LITERAL, FST::FST(GRAPH_STRING_LITERAL) },
	    { LEX_NEW, FST::FST(GRAPH_NEW) },
	    { LEX_MAIN, FST::FST(GRAPH_MAIN) },
	    { LEX_ID_TYPE, FST::FST(GRAPH_NUMBER) },
	    { LEX_ID_TYPE, FST::FST(GRAPH_STRING) },
	    { LEX_FUNCTION, FST::FST(GRAPH_FUNCTION) },
	    { LEX_PROC, FST::FST(GRAPH_PROC) },
	    { LEX_END, FST::FST(GRAPH_END) },
	    { LEX_PRINT, FST::FST(GRAPH_PRINT) },
	    { LEX_ENDL, FST::FST(GRAPH_ENDL) },
	    { LEX_ID, FST::FST(GRAPH_ID) }
	};

	char* getScopeName(IT::IdTable idtable, char* prevword) // имя текущей области видимости (если находимся внутри блока)
	{
		if (strcmp(prevword, MAIN) == 0) //если main
			return (char *)MAIN;//возвращаем m
		for (int i = 0; i <= idtable.size - 1; i++)
			if (idtable.table[i].idtype == IT::IDTYPE::F) //если в таблице идентификаторов это функция
				return idtable.table[i].id;// возвращаем идентификатор
		return NULL; // не найдено имя функции
	}

	int getLiteralIndex(IT::IdTable ittable, char* value, IT::IDDATATYPE type) // получаем индекс повторного литерала(по значению)
	{
		for (int i = 0; i < ittable.size; i++)
		{
			if (ittable.table[i].idtype == IT::IDTYPE::L && ittable.table[i].iddatatype == type)
			{
				switch (type)
				{
				case IT::IDDATATYPE::INT:
					if (ittable.table[i].value.vint == atoi(value))	
						return i;
					break;
				case IT::IDDATATYPE::STR:
					char buf[TI_STR_MAXSIZE];
					for (unsigned j = 1; j < strlen(value) - 1; j++) // без кавычек
						buf[j - 1] = value[j];
					buf[strlen(value) - 2] = IN_CODE_NULL;

					if (strcmp(ittable.table[i].value.vstr.str, buf) == 0)
						return i;
					break;
				}
			}
		}
		return TI_NULLIDX;
	}

	// получить текущий тип идентификатора
	IT::IDDATATYPE getType(char* curword, char* idtype)
	{
		if (!strcmp(TYPE_PROC, idtype))
			return IT::IDDATATYPE::PROC; // процедуры
		if (!strcmp(TYPE_STR, idtype)) 
			return IT::IDDATATYPE::STR;  // строковый ид
		if (!strcmp(TYPE_NUM, idtype)) 
			return IT::IDDATATYPE::INT;	 // числовой  ид

		if (isdigit(*curword) || *curword == LEX_MINUS)  return IT::IDDATATYPE::INT;				// числовой литерал
		else if (*curword == IN_CODE_QUOTE)  return IT::IDDATATYPE::STR;	// строковый литерал

		return IT::IDDATATYPE::UNDEF;		// неопределенный тип, индикатор ошибки
	}

	int getIndexInLT(LT::LexTable &lextable, int itTableIndex)					// индекс первой встречи в таблице лексем
	{
		if (itTableIndex == TI_NULLIDX)		// если идентификатор встречается впервые
			return lextable.size;
		for (int i = 0; i < lextable.size; i++)
			if (itTableIndex == lextable.table[i].idxTI)  return i;
		return LT_TI_NULLDX;
	}

	bool isLiteral(char* id)
	{
		if (isdigit(*id) || *id == IN_CODE_QUOTE || *id == LEX_MINUS) return true;
		return false;
	}

	IT::STDFNC getStandFunction(char* id)
	{
		if (!strcmp(CATSTRING, id)) 
			return IT::STDFNC::F_CATSTRING;
		if (!strcmp(PLENGTH, id)) 
			return IT::STDFNC::F_PLENGTH;
		return IT::STDFNC::F_NOT_STD;
	}

	char* getNextLiteralName()						// сгенерировать следующее имя литерала
	{
		static int literalNumber;
		char *buf = new char[ID_MAXSIZE];
		*buf = 'L';
		char num[4];
		_itoa_s(literalNumber++, num, 10);
		for (unsigned i = 0; i <= strlen(num); i++)
			buf[i + 1] = num[i];
		return buf;
	}


	IT::Entry* getEntry(		// формирует и возвращает строку ТИ
		Lexer::LEX& tables,						// ТЛ + ТИ
		char lex,								// лексема
		char* id,								// идентификатор
		char* idtype,							// предыдущая (тип)
		bool isParam,							// признак параметра функции
		bool isFunc,							// признак функции
		Log::LOG log,							// протокол
		int line,								// строка в исходном тексте
		bool& lex_ok)							// флаг ошибки(по ссылке)
	{
		// тип данных
		IT::IDDATATYPE type = getType(id, idtype);// получить текущий тип идентификатора
		int index = IT::isId(tables.idtable, id);	// индекс существующего идентификатора или -1
		if (lex == LEX_LITERAL)
			index = getLiteralIndex(tables.idtable, id, type);// получаем индекс повторного литерала(по значению)
		if (index != TI_NULLIDX)
			return NULL;	// уже существует

		IT::Entry *itentry = new IT::Entry;//создаём экземпляр ТИ
		itentry->iddatatype = type; // сохраняем тип данных

									// индекс первой строки в таблице лексем 
		itentry->idxfirstLE = getIndexInLT(tables.lextable, index);

		if (lex == LEX_LITERAL) // литерал
		{
			bool int_ok = IT::SetValue(itentry, id);// установка значения переменной
			if (!int_ok)
			{
				// превышен максимальный размер числового литерала
				Log::writeError(log.stream, Error::GetError(313, line, 0));
				lex_ok = false;
			}
			if (itentry->iddatatype == IT::IDDATATYPE::STR && itentry->value.vstr.len == 0)
			{
				// пустой строковый литерал
				Log::writeError(log.stream, Error::GetError(310, line, 0));
				lex_ok = false;
			}
			strcpy_s(itentry->id, getNextLiteralName());
			itentry->idtype = IT::IDTYPE::L;
		}
		else // идентификатор начальная иниц
		{
			switch (type)
			{
			case IT::IDDATATYPE::STR:
				strcpy_s(itentry->value.vstr.str, "");
				itentry->value.vstr.len = TI_STR_DEFAULT;
				break;
			case IT::IDDATATYPE::INT:
				itentry->value.vint = TI_INT_DEFAULT;
				break;
			}

			if (isFunc)
			{
				switch (getStandFunction(id))
				{
				case IT::STDFNC::F_CATSTRING:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = CATSTRING_TYPE;//тип возвращающего знач-я
					itentry->value.params.count = CATSTRING_PARAMS_CNT;//кол-во параметров
					itentry->value.params.types = new IT::IDDATATYPE[CATSTRING_PARAMS_CNT];//типы параметров
					for (int k = 0; k < CATSTRING_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::CATSTRING_PARAMS[k];//заносим типы в типы
					break;
				}
				case IT::STDFNC::F_PLENGTH:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = PLENGTH_TYPE;
					itentry->value.params.count = PLENGTH_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[PLENGTH_PARAMS_CNT];
					for (int k = 0; k < PLENGTH_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::PLENGTH_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_NOT_STD:  itentry->idtype = IT::IDTYPE::F;  break;
				}
			}
			else if (isParam) itentry->idtype = IT::IDTYPE::P;
			else itentry->idtype = IT::IDTYPE::V;

			strncpy_s(itentry->id, id, SCOPED_ID_MAXSIZE);
		}
		// -------------------------------------------------------
		int i = tables.lextable.size; // индекс в ТЛ текущего ИД

		if (i > 1 && itentry->idtype == IT::IDTYPE::V && tables.lextable.table[i - 2].lexema != LEX_NEW)
		{
			// в объявлении отсутствует ключевое слово new
			Log::writeError(log.stream, Error::GetError(304, line, 0));
			lex_ok = false;
		}
		if (i > 1 && itentry->idtype == IT::IDTYPE::F && tables.lextable.table[i - 1].lexema != LEX_FUNCTION)
		{
			// в объявлении не указан тип функции
			Log::writeError(log.stream, Error::GetError(303, line, 0));
			lex_ok = false;
		}
		if (itentry->iddatatype == IT::IDDATATYPE::UNDEF)
		{
			// невозможно определелить тип
			Log::writeError(log.stream, Error::GetError(300, line, 0));
			lex_ok = false;
		}
		// --------------------------------------------------------
		return itentry;
	}

	bool analyze(LEX& tables, In::IN &in, Log::LOG &log, Parm::PARM &parm)
	{
		static bool lex_ok = true;
		tables.lextable = LT::Create(LT_MAXSIZE); //создаём таблицу лексем
		tables.idtable = IT::Create(TI_MAXSIZE); //создаём таблицу идентификаторов

		bool isParam = false, isFunc = false;
		int enterPoint = NULL;//точка входа (main)
		char curword[TI_STR_MAXSIZE], nextword[TI_STR_MAXSIZE]; // текущее и след слово
		int curline;// текущяя строка
		std::stack <char*> scopes;			// стек для хранения имени текущей области видимости

		for (int i = 0; i < In::InWord::size; i++)
		{
			strcpy_s(curword, in.words[i].word);
			if (i < In::InWord::size - 1)
				strcpy_s(nextword, in.words[i + 1].word);
			curline = in.words[i].line;
			isFunc = false;
			int idxTI = TI_NULLIDX;//элемент таблицы идент-в

			for (int j = 0; j < N_GRAPHS; j++)
			{
				FST::FST fst(curword, graphs[j].graph);// недетерминированный конеч автомат (количество состояний автомата, список состояний(граф переходов))
				if (FST::execute(fst))//если цепочка распознана
				{
					char lexema = graphs[j].lexema;//получаем лексему
					switch (lexema)
					{
					case LEX_MAIN: 
						enterPoint++; 
						break;// точка входа
					case LEX_SEPARATORS://разделители
					{
						switch (*curword)//смотри по слову
						{
						case LEX_LEFTTHESIS:		// открывающая скобка - параметры  функции
						{
							isParam = true;
							// начало области видимости
							if (*nextword == LEX_RIGHTTHESIS || ISTYPE(nextword))//или ), или тип
							{
								char* functionname = new char[ID_MAXSIZE];//имя функции (размером 9)
								char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);// имя текущей области видимости (если находимся внутри блока)
								if (scopename == NULL)  
									break; // не найдено имя функции
								strcpy_s(functionname, ID_MAXSIZE, scopename);//копируем имя функции в имя функции
								scopes.push(functionname);//заносим в стек текущую обл видимости
							}
							break;
						}
						case LEX_RIGHTTHESIS:	// закрывающая скобка
						{
							isParam = false;
							// конец области видимости
							if (*in.words[i - 1].word == LEX_LEFTTHESIS || // если перед было ( или
								(i > 2 && (tables.lextable.table[tables.lextable.size - 2].lexema == LEX_ID_TYPE))) // -2 знач был тип
								scopes.pop();//извлекаем об видимости из стека
							break;
						}
						case LEX_LEFTBRACE:		// начало области видимости
						{
							char* functionname = new char[ID_MAXSIZE];//имя функции (размером 9)
							char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);// имя текущей области видимости (если находимся внутри блока)
							if (scopename == NULL) 
								break;// не найдено имя функции
							strcpy_s(functionname, ID_MAXSIZE, scopename);//копируем имя функции в имя функции
							scopes.push(functionname);//заносим в стек текущую обл видимости
							break;
						}
						case LEX_BRACELET:		// конец области видимости
						{
							// только в этом случае закрываем область видимости
							if (*in.words[i + 1].word == LEX_ID_TYPE || *in.words[i + 1].word == LEX_PROC || *in.words[i + 1].word == LEX_MAIN)
								// если след слово тип, или proc, или main
							{
								if (!scopes.empty())  scopes.pop();//извлекаем из стека обл видимости
							}
							break;
						}
						} // in switch

						lexema = *curword;
						break;
					} // case separatoes
					case LEX_ID:
					case LEX_LITERAL: //если литерал или идентификатор
					{
						char id[TI_STR_MAXSIZE] = "";
						idxTI = LT_TI_NULLDX;  // индекс идентификатора в ТИ

						if (*nextword == LEX_LEFTTHESIS)  isFunc = true;						// идентификатор функции

						char* idtype = (isFunc && i > 1) ?	// тип идентификатора
							in.words[i - 2].word : in.words[i - 1].word;		// пропускаем ключевое слово function

						if (!isFunc && !scopes.empty())
							strncpy_s(id, scopes.top(), ID_MAXSIZE);
						strncat(id, curword, ID_MAXSIZE);
						if (isLiteral(curword))  strcpy_s(id, curword); // литерал заменяется своим значением

						IT::Entry *itentry = getEntry(tables, lexema, id, idtype, isParam, isFunc, log, curline, lex_ok);
						if (itentry != nullptr) // первая встреча - объявление
						{
							if (isFunc) // если функция - сохранить список параметров
							{
								if (getStandFunction(id) == IT::STDFNC::F_NOT_STD) // стандартная функция или нет
								{
									itentry->value.params.count = NULL;
									itentry->value.params.types = new IT::IDDATATYPE[MAX_PARAMS_COUNT];

									for (int k = i; in.words[k].word[0] != LEX_RIGHTTHESIS; k++)
									{
										if (k == In::InWord::size || in.words[k].word[0] == LEX_SEPARATOR)  break;
										if (ISTYPE(in.words[k].word))
										{
											if (itentry->value.params.count == MAX_PARAMS_COUNT)
											{
												Log::writeError(log.stream, Error::GetError(306, curline, 0));
												lex_ok = false;
												break;
											}
											itentry->value.params.types[itentry->value.params.count++] = getType(NULL, in.words[k].word);
										}
									} // for
								}
							}
							IT::Add(tables.idtable, *itentry);
							idxTI = tables.idtable.size - 1;
						}
						else // повторный идентификатор (уже есть)
						{
							int i = tables.lextable.size - 1; // жалкие попытки переопределить идентификатор
							if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_NEW || tables.lextable.table[i].lexema == LEX_NEW
								|| tables.lextable.table[i - 1].lexema == LEX_FUNCTION || tables.lextable.table[i].lexema == LEX_FUNCTION
								|| tables.lextable.table[i - 1].lexema == LEX_ID_TYPE || tables.lextable.table[i].lexema == LEX_ID_TYPE
								|| tables.lextable.table[i - 1].lexema == LEX_PROC || tables.lextable.table[i].lexema == LEX_PROC)
							{
								Log::writeError(log.stream, Error::GetError(305, curline, 0));
								lex_ok = false;
							}
							idxTI = IT::isId(tables.idtable, id);	// индекс существующего идентификатора
							if (lexema == LEX_LITERAL)
								idxTI = getLiteralIndex(tables.idtable, curword, getType(id, in.words[i - 1].word)); // или литерала
						}
					}
					break;
					} // end switch

					LT::Entry *ltentry = new LT::Entry(lexema, curline, idxTI);
					LT::Add(tables.lextable, *ltentry);
					break;
				} // execute true
				else if (j == N_GRAPHS - 1) // цепочка не распознана
				{
					Log::writeError(log.stream, Error::GetError(201, curline, 0));
					lex_ok = false;
				}
			}  // for j
		} // for i

		if (enterPoint == NULL) // не найдена точка входа
		{
			Log::writeError(log.stream, Error::GetError(301));
			lex_ok = false;
		}
		if (enterPoint > 1) //больше 1 точки входа
		{
			Log::writeError(log.stream, Error::GetError(302));
			lex_ok = false;
		}

		return lex_ok;
	}
};