#include "stdafx.h"

#include "Error.h"
#include "LexAnaliz.h"
#include "Sem.h"


namespace Semantic
{
	bool Semantic::semanticsCheck(Lexer::LEX & tables, Log::LOG & log)
	{
		bool sem_ok = true;

		for (int i = 0; i < tables.lextable.size; i++)//�������� �� ���� ������� ������
		{
			switch (tables.lextable.table[i].lexema)//������� �� �������
			{
			case LEX_EQUAL: // ���������(=)
			{
				if (i > 0 && tables.lextable.table[i - 1].idxTI != TI_NULLIDX) // ����� �������
				{
					IT::IDDATATYPE lefttype = tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype;//������� ������� ��� ������
					bool ignore = false;

					for (int k = i + 1; tables.lextable.table[k].lexema != LEX_SEPARATOR; k++)//���� �� ;
					{
						if (k == tables.lextable.size) break; // ������� ������ - ��� ;
						if (tables.lextable.table[k].idxTI != TI_NULLIDX) // ���� �� - ��������� ���������� �����
						{
							if (!ignore)//true
							{
								IT::IDDATATYPE righttype = tables.idtable.table[tables.lextable.table[k].idxTI].iddatatype;//������� ��� ������ ������� ��-�
								if (lefttype != righttype) // ���� ������ � ��������� �� ���������
								{
									Log::writeError(log.stream, Error::GetError(314, tables.lextable.table[k].sn, 0));
									// "***���*** ���� ������ � ��������� �� ���������
									sem_ok = false;
									break;
								}
							}
							// ���� ������� ����� ����� ������� ������ - ��� ����� �������
							if (tables.lextable.table[k + 1].lexema == LEX_LEFTTHESIS)
							{
								ignore = true;
								continue;
							}
							// ����������� ������ ����� ������ ����������
							if (ignore && tables.lextable.table[k + 1].lexema == LEX_RIGHTTHESIS)
							{
								ignore = false;
								continue;
							}
						}
						if (lefttype == IT::IDDATATYPE::STR) // ������ ������ �������, �� ��� ����� ��������� �-���
						{
							char l = tables.lextable.table[k].lexema;//����� =
							if (l == LEX_PLUS || l == LEX_MINUS || l == LEX_STAR) // ��������� �����������
							{
								Log::writeError(log.stream, Error::GetError(316, tables.lextable.table[k].sn, 0));
								//***���*** ������������ ��������� ��������� ������ �� ����� =
								sem_ok = false;
								break;
							}
						}
					} // for
				}
				break;
			}
			case LEX_ID: // �������� ���� ������������� ��������  !�� ���������!
			{
				// �������� ���
				IT::Entry e = tables.idtable.table[tables.lextable.table[i].idxTI];

				if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_FUNCTION)
				{
					if (e.idtype == IT::IDTYPE::F && e.iddatatype != IT::IDDATATYPE::PROC) //�������, �� ���������
					{
						for (int k = i + 1; ; k++)
						{
							char l = tables.lextable.table[k].lexema;
							if (l == LEX_END)
							{
								int next = tables.lextable.table[k + 1].idxTI; // ����. �� end
								if (next != TI_NULLIDX)
								{
									// ��� ������� � ������������� �������� �� ���������
									if (tables.idtable.table[next].iddatatype != e.iddatatype)
									{
										Log::writeError(log.stream, Error::GetError(315, tables.lextable.table[k].sn, 0));
										sem_ok = false;
										break;
									}
								}
								break; 
							}

							if (k == tables.lextable.size) break;//�������� �� ����� �������
						} // for
					}
				}
				// �������� ���
				if (tables.lextable.table[i + 1].lexema == LEX_LEFTTHESIS && tables.lextable.table[i - 1].lexema != LEX_FUNCTION) // ������ �����
				{
					if (e.idtype == IT::IDTYPE::F || e.idtype == IT::IDTYPE::S) // ����� �������
					{
						int paramscount = NULL;
						// �������� ������������ ����������
						for (int j = i + 1; tables.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)//���� �� )
						{
							// �������� ������������ ������������ ���������� ����������
							if (tables.lextable.table[j].lexema == LEX_ID || tables.lextable.table[j].lexema == LEX_LITERAL)
							{
								paramscount++;
								if (e.value.params.count == NULL) break;// ���������� ���������� ������� = 0
								IT::IDDATATYPE ctype = tables.idtable.table[tables.lextable.table[j].idxTI].iddatatype;
								if (ctype != e.value.params.types[paramscount - 1])
								{
									// ������������ ����� ������������ ����������
									Log::writeError(log.stream, Error::GetError(309, tables.lextable.table[i].sn, 0));
									sem_ok = false;
									break;
								}
							}
							if (j == tables.lextable.size) break;
						}
						if (paramscount != e.value.params.count)
						{
							// ���������� ������������ � ����������� ���������� �� ���������
							Log::writeError(log.stream, Error::GetError(308, tables.lextable.table[i].sn, 0));
							sem_ok = false;
						}
						if (paramscount > 3)
						{
							// ������� ����� ���������� � ������
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