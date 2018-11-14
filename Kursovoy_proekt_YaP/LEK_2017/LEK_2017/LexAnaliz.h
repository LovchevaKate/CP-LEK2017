#pragma once

#include "IT.h"
#include "LT.h"
#include "In.h"
#include "Log.h"
#include "FST.h"
#include "Error.h"

#define TYPE_NUM	"number"
#define TYPE_STR	"string"
#define TYPE_PROC	"proc"
#define MAIN		"main"
#define CATSTRING	"catstring"
#define PLENGTH		"plength"


#define ISTYPE(str) ( !strcmp(str, TYPE_NUM) || !strcmp(str, TYPE_STR) )

namespace Lexer
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable	idtable;

		LEX() {}
	};
	struct Graph
	{
		char lexema;
		FST::FST graph;
	};

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm);
	int getIndexInLT(LT::LexTable &lextable, int itTableIndex);					// индекс первой встречи в таблице лексем

	IT::Entry* getEntry		// формирует и возвращает строку ТИ
	(
		Lexer::LEX& tables,						// ТЛ + ТИ
		char lex,								// лексема
		char* id,								// идентификатор
		char* idtype,							// предыдущая (тип)
		bool isParam,							// признак параметра функции
		bool isFunc,							// признак функции
		Log::LOG log,							// протокол
		int line,								// строка в исходном тексте
		bool& rc_err							// флаг ошибки(по ссылке)
	);

	struct ERROR_S									// тип исключения для throw ERROR_THROW | ERROR_THROW_IN
	{
		int id;
		char message[ERROR_MAXSIZE_MESSAGE];			// сообщение			
		struct
		{
			short line = -1;						//номер строки (0, 1, 2, ...)
			short cells = -1;						//номер позиции в строке(0, 1, 2, ...)
		} position;
	};
};
