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

	char* getScopeName(IT::IdTable idtable, char* prevword) // ��� ������� ������� ��������� (���� ��������� ������ �����)
	{
		if (strcmp(prevword, MAIN) == 0) //���� main
			return (char *)MAIN;//���������� m
		for (int i = 0; i <= idtable.size - 1; i++)
			if (idtable.table[i].idtype == IT::IDTYPE::F) //���� � ������� ��������������� ��� �������
				return idtable.table[i].id;// ���������� �������������
		return NULL; // �� ������� ��� �������
	}

	int getLiteralIndex(IT::IdTable ittable, char* value, IT::IDDATATYPE type) // �������� ������ ���������� ��������(�� ��������)
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
					for (unsigned j = 1; j < strlen(value) - 1; j++) // ��� �������
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

	// �������� ������� ��� ��������������
	IT::IDDATATYPE getType(char* curword, char* idtype)
	{
		if (!strcmp(TYPE_PROC, idtype))
			return IT::IDDATATYPE::PROC; // ���������
		if (!strcmp(TYPE_STR, idtype)) 
			return IT::IDDATATYPE::STR;  // ��������� ��
		if (!strcmp(TYPE_NUM, idtype)) 
			return IT::IDDATATYPE::INT;	 // ��������  ��

		if (isdigit(*curword) || *curword == LEX_MINUS)  return IT::IDDATATYPE::INT;				// �������� �������
		else if (*curword == IN_CODE_QUOTE)  return IT::IDDATATYPE::STR;	// ��������� �������

		return IT::IDDATATYPE::UNDEF;		// �������������� ���, ��������� ������
	}

	int getIndexInLT(LT::LexTable &lextable, int itTableIndex)					// ������ ������ ������� � ������� ������
	{
		if (itTableIndex == TI_NULLIDX)		// ���� ������������� ����������� �������
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

	char* getNextLiteralName()						// ������������� ��������� ��� ��������
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


	IT::Entry* getEntry(		// ��������� � ���������� ������ ��
		Lexer::LEX& tables,						// �� + ��
		char lex,								// �������
		char* id,								// �������������
		char* idtype,							// ���������� (���)
		bool isParam,							// ������� ��������� �������
		bool isFunc,							// ������� �������
		Log::LOG log,							// ��������
		int line,								// ������ � �������� ������
		bool& lex_ok)							// ���� ������(�� ������)
	{
		// ��� ������
		IT::IDDATATYPE type = getType(id, idtype);// �������� ������� ��� ��������������
		int index = IT::isId(tables.idtable, id);	// ������ ������������� �������������� ��� -1
		if (lex == LEX_LITERAL)
			index = getLiteralIndex(tables.idtable, id, type);// �������� ������ ���������� ��������(�� ��������)
		if (index != TI_NULLIDX)
			return NULL;	// ��� ����������

		IT::Entry *itentry = new IT::Entry;//������ ��������� ��
		itentry->iddatatype = type; // ��������� ��� ������

									// ������ ������ ������ � ������� ������ 
		itentry->idxfirstLE = getIndexInLT(tables.lextable, index);

		if (lex == LEX_LITERAL) // �������
		{
			bool int_ok = IT::SetValue(itentry, id);// ��������� �������� ����������
			if (!int_ok)
			{
				// �������� ������������ ������ ��������� ��������
				Log::writeError(log.stream, Error::GetError(313, line, 0));
				lex_ok = false;
			}
			if (itentry->iddatatype == IT::IDDATATYPE::STR && itentry->value.vstr.len == 0)
			{
				// ������ ��������� �������
				Log::writeError(log.stream, Error::GetError(310, line, 0));
				lex_ok = false;
			}
			strcpy_s(itentry->id, getNextLiteralName());
			itentry->idtype = IT::IDTYPE::L;
		}
		else // ������������� ��������� ����
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
					itentry->iddatatype = CATSTRING_TYPE;//��� ������������� ����-�
					itentry->value.params.count = CATSTRING_PARAMS_CNT;//���-�� ����������
					itentry->value.params.types = new IT::IDDATATYPE[CATSTRING_PARAMS_CNT];//���� ����������
					for (int k = 0; k < CATSTRING_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::CATSTRING_PARAMS[k];//������� ���� � ����
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
		int i = tables.lextable.size; // ������ � �� �������� ��

		if (i > 1 && itentry->idtype == IT::IDTYPE::V && tables.lextable.table[i - 2].lexema != LEX_NEW)
		{
			// � ���������� ����������� �������� ����� new
			Log::writeError(log.stream, Error::GetError(304, line, 0));
			lex_ok = false;
		}
		if (i > 1 && itentry->idtype == IT::IDTYPE::F && tables.lextable.table[i - 1].lexema != LEX_FUNCTION)
		{
			// � ���������� �� ������ ��� �������
			Log::writeError(log.stream, Error::GetError(303, line, 0));
			lex_ok = false;
		}
		if (itentry->iddatatype == IT::IDDATATYPE::UNDEF)
		{
			// ���������� ������������ ���
			Log::writeError(log.stream, Error::GetError(300, line, 0));
			lex_ok = false;
		}
		// --------------------------------------------------------
		return itentry;
	}

	bool analyze(LEX& tables, In::IN &in, Log::LOG &log, Parm::PARM &parm)
	{
		static bool lex_ok = true;
		tables.lextable = LT::Create(LT_MAXSIZE); //������ ������� ������
		tables.idtable = IT::Create(TI_MAXSIZE); //������ ������� ���������������

		bool isParam = false, isFunc = false;
		int enterPoint = NULL;//����� ����� (main)
		char curword[TI_STR_MAXSIZE], nextword[TI_STR_MAXSIZE]; // ������� � ���� �����
		int curline;// ������� ������
		std::stack <char*> scopes;			// ���� ��� �������� ����� ������� ������� ���������

		for (int i = 0; i < In::InWord::size; i++)
		{
			strcpy_s(curword, in.words[i].word);
			if (i < In::InWord::size - 1)
				strcpy_s(nextword, in.words[i + 1].word);
			curline = in.words[i].line;
			isFunc = false;
			int idxTI = TI_NULLIDX;//������� ������� �����-�

			for (int j = 0; j < N_GRAPHS; j++)
			{
				FST::FST fst(curword, graphs[j].graph);// ������������������� ����� ������� (���������� ��������� ��������, ������ ���������(���� ���������))
				if (FST::execute(fst))//���� ������� ����������
				{
					char lexema = graphs[j].lexema;//�������� �������
					switch (lexema)
					{
					case LEX_MAIN: 
						enterPoint++; 
						break;// ����� �����
					case LEX_SEPARATORS://�����������
					{
						switch (*curword)//������ �� �����
						{
						case LEX_LEFTTHESIS:		// ����������� ������ - ���������  �������
						{
							isParam = true;
							// ������ ������� ���������
							if (*nextword == LEX_RIGHTTHESIS || ISTYPE(nextword))//��� ), ��� ���
							{
								char* functionname = new char[ID_MAXSIZE];//��� ������� (�������� 9)
								char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);// ��� ������� ������� ��������� (���� ��������� ������ �����)
								if (scopename == NULL)  
									break; // �� ������� ��� �������
								strcpy_s(functionname, ID_MAXSIZE, scopename);//�������� ��� ������� � ��� �������
								scopes.push(functionname);//������� � ���� ������� ��� ���������
							}
							break;
						}
						case LEX_RIGHTTHESIS:	// ����������� ������
						{
							isParam = false;
							// ����� ������� ���������
							if (*in.words[i - 1].word == LEX_LEFTTHESIS || // ���� ����� ���� ( ���
								(i > 2 && (tables.lextable.table[tables.lextable.size - 2].lexema == LEX_ID_TYPE))) // -2 ���� ��� ���
								scopes.pop();//��������� �� ��������� �� �����
							break;
						}
						case LEX_LEFTBRACE:		// ������ ������� ���������
						{
							char* functionname = new char[ID_MAXSIZE];//��� ������� (�������� 9)
							char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);// ��� ������� ������� ��������� (���� ��������� ������ �����)
							if (scopename == NULL) 
								break;// �� ������� ��� �������
							strcpy_s(functionname, ID_MAXSIZE, scopename);//�������� ��� ������� � ��� �������
							scopes.push(functionname);//������� � ���� ������� ��� ���������
							break;
						}
						case LEX_BRACELET:		// ����� ������� ���������
						{
							// ������ � ���� ������ ��������� ������� ���������
							if (*in.words[i + 1].word == LEX_ID_TYPE || *in.words[i + 1].word == LEX_PROC || *in.words[i + 1].word == LEX_MAIN)
								// ���� ���� ����� ���, ��� proc, ��� main
							{
								if (!scopes.empty())  scopes.pop();//��������� �� ����� ��� ���������
							}
							break;
						}
						} // in switch

						lexema = *curword;
						break;
					} // case separatoes
					case LEX_ID:
					case LEX_LITERAL: //���� ������� ��� �������������
					{
						char id[TI_STR_MAXSIZE] = "";
						idxTI = LT_TI_NULLDX;  // ������ �������������� � ��

						if (*nextword == LEX_LEFTTHESIS)  isFunc = true;						// ������������� �������

						char* idtype = (isFunc && i > 1) ?	// ��� ��������������
							in.words[i - 2].word : in.words[i - 1].word;		// ���������� �������� ����� function

						if (!isFunc && !scopes.empty())
							strncpy_s(id, scopes.top(), ID_MAXSIZE);
						strncat(id, curword, ID_MAXSIZE);
						if (isLiteral(curword))  strcpy_s(id, curword); // ������� ���������� ����� ���������

						IT::Entry *itentry = getEntry(tables, lexema, id, idtype, isParam, isFunc, log, curline, lex_ok);
						if (itentry != nullptr) // ������ ������� - ����������
						{
							if (isFunc) // ���� ������� - ��������� ������ ����������
							{
								if (getStandFunction(id) == IT::STDFNC::F_NOT_STD) // ����������� ������� ��� ���
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
						else // ��������� ������������� (��� ����)
						{
							int i = tables.lextable.size - 1; // ������ ������� �������������� �������������
							if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_NEW || tables.lextable.table[i].lexema == LEX_NEW
								|| tables.lextable.table[i - 1].lexema == LEX_FUNCTION || tables.lextable.table[i].lexema == LEX_FUNCTION
								|| tables.lextable.table[i - 1].lexema == LEX_ID_TYPE || tables.lextable.table[i].lexema == LEX_ID_TYPE
								|| tables.lextable.table[i - 1].lexema == LEX_PROC || tables.lextable.table[i].lexema == LEX_PROC)
							{
								Log::writeError(log.stream, Error::GetError(305, curline, 0));
								lex_ok = false;
							}
							idxTI = IT::isId(tables.idtable, id);	// ������ ������������� ��������������
							if (lexema == LEX_LITERAL)
								idxTI = getLiteralIndex(tables.idtable, curword, getType(id, in.words[i - 1].word)); // ��� ��������
						}
					}
					break;
					} // end switch

					LT::Entry *ltentry = new LT::Entry(lexema, curline, idxTI);
					LT::Add(tables.lextable, *ltentry);
					break;
				} // execute true
				else if (j == N_GRAPHS - 1) // ������� �� ����������
				{
					Log::writeError(log.stream, Error::GetError(201, curline, 0));
					lex_ok = false;
				}
			}  // for j
		} // for i

		if (enterPoint == NULL) // �� ������� ����� �����
		{
			Log::writeError(log.stream, Error::GetError(301));
			lex_ok = false;
		}
		if (enterPoint > 1) //������ 1 ����� �����
		{
			Log::writeError(log.stream, Error::GetError(302));
			lex_ok = false;
		}

		return lex_ok;
	}
};