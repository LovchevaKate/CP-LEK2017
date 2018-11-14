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
	int getIndexInLT(LT::LexTable &lextable, int itTableIndex);					// ������ ������ ������� � ������� ������

	IT::Entry* getEntry		// ��������� � ���������� ������ ��
	(
		Lexer::LEX& tables,						// �� + ��
		char lex,								// �������
		char* id,								// �������������
		char* idtype,							// ���������� (���)
		bool isParam,							// ������� ��������� �������
		bool isFunc,							// ������� �������
		Log::LOG log,							// ��������
		int line,								// ������ � �������� ������
		bool& rc_err							// ���� ������(�� ������)
	);

	struct ERROR_S									// ��� ���������� ��� throw ERROR_THROW | ERROR_THROW_IN
	{
		int id;
		char message[ERROR_MAXSIZE_MESSAGE];			// ���������			
		struct
		{
			short line = -1;						//����� ������ (0, 1, 2, ...)
			short cells = -1;						//����� ������� � ������(0, 1, 2, ...)
		} position;
	};
};
