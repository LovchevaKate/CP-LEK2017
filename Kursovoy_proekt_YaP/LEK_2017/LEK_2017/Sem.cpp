#include "stdafx.h"

#include "Error.h"
#include "LexAnaliz.h"
#include "Sem.h"


namespace Semantic
{
	bool Semantic::semanticsCheck(Lexer::LEX & tables, Log::LOG & log)
	{
		bool sem_ok = true;

		for (int i = 0; i < tables.lextable.size; i++)//проходим по всей таблице лексем
		{
			switch (tables.lextable.table[i].lexema)//смотрим по лексеме
			{
			case LEX_EQUAL: // выражение(=)
			{
				if (i > 0 && tables.lextable.table[i - 1].idxTI != TI_NULLIDX) // левый операнд
				{
					IT::IDDATATYPE lefttype = tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype;//заносим соответ тип данных
					bool ignore = false;

					for (int k = i + 1; tables.lextable.table[k].lexema != LEX_SEPARATOR; k++)//пока не ;
					{
						if (k == tables.lextable.size) break; // синтакс ошибка - нет ;
						if (tables.lextable.table[k].idxTI != TI_NULLIDX) // если ид - проверить совпадение типов
						{
							if (!ignore)//true
							{
								IT::IDDATATYPE righttype = tables.idtable.table[tables.lextable.table[k].idxTI].iddatatype;//соответ тип данных правого оп-а
								if (lefttype != righttype) // типы данных в выражении не совпадают
								{
									Log::writeError(log.stream, Error::GetError(314, tables.lextable.table[k].sn, 0));
									// "***СЕМ*** Типы данных в выражении не совпадают
									sem_ok = false;
									break;
								}
							}
							// если лексема сразу после идентиф скобка - это вызов функции
							if (tables.lextable.table[k + 1].lexema == LEX_LEFTTHESIS)
							{
								ignore = true;
								continue;
							}
							// закрывающая скобка после списка параметров
							if (ignore && tables.lextable.table[k + 1].lexema == LEX_RIGHTTHESIS)
							{
								ignore = false;
								continue;
							}
						}
						if (lefttype == IT::IDDATATYPE::STR) // справа только литерал, ид или вызов строковой ф-ции
						{
							char l = tables.lextable.table[k].lexema;//после =
							if (l == LEX_PLUS || l == LEX_MINUS || l == LEX_STAR) // выражения недопустимы
							{
								Log::writeError(log.stream, Error::GetError(316, tables.lextable.table[k].sn, 0));
								//***СЕМ*** Недопустимое строковое выражение справа от знака =
								sem_ok = false;
								break;
							}
						}
					} // for
				}
				break;
			}
			case LEX_ID: // проверка типа возвращаемого значения  !по прототипу!
			{
				// проверка раз
				IT::Entry e = tables.idtable.table[tables.lextable.table[i].idxTI];

				if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_FUNCTION)
				{
					if (e.idtype == IT::IDTYPE::F && e.iddatatype != IT::IDDATATYPE::PROC) //функция, не процедура
					{
						for (int k = i + 1; ; k++)
						{
							char l = tables.lextable.table[k].lexema;
							if (l == LEX_END)
							{
								int next = tables.lextable.table[k + 1].idxTI; // след. за end
								if (next != TI_NULLIDX)
								{
									// тип функции и возвращаемого значения не совпадают
									if (tables.idtable.table[next].iddatatype != e.iddatatype)
									{
										Log::writeError(log.stream, Error::GetError(315, tables.lextable.table[k].sn, 0));
										sem_ok = false;
										break;
									}
								}
								break; 
							}

							if (k == tables.lextable.size) break;//проверка на конец таблицы
						} // for
					}
				}
				// проверка два
				if (tables.lextable.table[i + 1].lexema == LEX_LEFTTHESIS && tables.lextable.table[i - 1].lexema != LEX_FUNCTION) // именно вызов
				{
					if (e.idtype == IT::IDTYPE::F || e.idtype == IT::IDTYPE::S) // точно функция
					{
						int paramscount = NULL;
						// проверка передаваемых параметров
						for (int j = i + 1; tables.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)//пока не )
						{
							// проверка соответствия передаваемых параметров прототипам
							if (tables.lextable.table[j].lexema == LEX_ID || tables.lextable.table[j].lexema == LEX_LITERAL)
							{
								paramscount++;
								if (e.value.params.count == NULL) break;// количество параметров функции = 0
								IT::IDDATATYPE ctype = tables.idtable.table[tables.lextable.table[j].idxTI].iddatatype;
								if (ctype != e.value.params.types[paramscount - 1])
								{
									// Несовпадение типов передаваемых параметров
									Log::writeError(log.stream, Error::GetError(309, tables.lextable.table[i].sn, 0));
									sem_ok = false;
									break;
								}
							}
							if (j == tables.lextable.size) break;
						}
						if (paramscount != e.value.params.count)
						{
							// Количество передаваемых и принимаемых параметров не совпадает
							Log::writeError(log.stream, Error::GetError(308, tables.lextable.table[i].sn, 0));
							sem_ok = false;
						}
						if (paramscount > 3)
						{
							// Слишком много параметров в вызове
							Log::writeError(log.stream, Error::GetError(307, tables.lextable.table[i].sn, 0));
							sem_ok = false;
						}
					}
				}
				break;
			}
			
			} // switch
		} // for

		return sem_ok;
	}
};