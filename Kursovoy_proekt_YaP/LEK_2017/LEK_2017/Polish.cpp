#include "stdafx.h"
#include "LexAnaliz.h"
#include <stack>
#include <queue>

using namespace std;

bool PolishNotation(int i, LT::LexTable& lextable, IT::IdTable& idtable)
{
	stack<LT::Entry> stack;		// ���� ��� ��������
	queue<LT::Entry> queue;		// ������� ��� ���������
	LT::Entry temp;		temp.idxTI = -1;	temp.lexema = '#';	temp.sn = -1;	// ����������� �������, ��� ������ �������� ����� ���������� �� ���
	LT::Entry func;		func.idxTI = -1;	func.lexema = '@';	func.sn = -1;	// ������� ��� ������ �������
	int countLex = 0;			// ���������� ��������������� ������
	int countParm = 0;			// ���������� ���������� �������
	int posLex = i;				// ���������� ����� ������� ����� ���������������
	bool findFunc = false;		// ���� ��� �������
	bool findComma = false;		// ���� ��� ������� (���-�� ���������� +2 �����)
	char* buf = new char[1];	// ����� ��� countParm � ��������� �������������

	for (int i = 0; i < lextable.size; i++)// ���������� ��������� ��� ������� ���������
	{
		if (lextable.table[i].lexema == LEX_LEFTTHESIS || lextable.table[i].lexema == LEX_RIGHTTHESIS)
			lextable.table[i].priority = 0;
		if (lextable.table[i].lexema == LEX_COMMA)
			lextable.table[i].priority = 1;
		if (lextable.table[i].lexema == LEX_PLUS || lextable.table[i].lexema == LEX_MINUS)
			lextable.table[i].priority = 2;
		if (lextable.table[i].lexema == LEX_STAR || lextable.table[i].lexema == LEX_DIRSLASH)
			lextable.table[i].priority = 3;
	}

	for (i; lextable.table[i].lexema != LEX_SEPARATOR; i++, countLex++)
	{
		switch (lextable.table[i].lexema)
		{
		case LEX_ID:	// ���� �������������
		{
			if (idtable.table[lextable.table[i].idxTI].idtype == IT::F)
				findFunc = true;
			queue.push(lextable.table[i]);
			continue;
		}
		case LEX_LITERAL:		// ���� �������
		{
			queue.push(lextable.table[i]);	// �������� � �������
			continue;
		}
		case LEX_LEFTTHESIS:		// ���� (
		{
			stack.push(lextable.table[i]); // �������� �� � ����
			continue;
		}
		case LEX_RIGHTTHESIS:	// ���� )
		{
			if (findFunc)		// ���� ��� ����� �������, �� ������� () ���������� �� @ � ���-�� ����������
			{
				if (findComma)
					countParm++;
				//lextable.table[i] = func;
				//queue.push(lextable.table[i]);		// ��������� � ������� ������� ������ �������
				//_itoa_s(countParm, buf, 2, 10);			// �������������� ����� countParm � ������
				//stack.top().lexema = buf[0];	stack.top().idxTI = -1; stack.top().sn = -1;	// ��������� �������, ����������� ���������� ���������� �������
				//queue.push(stack.top());				// ��������� � ������� ��� �������
				findFunc = false;
			}
			else {
				while (stack.top().lexema != LEX_LEFTTHESIS)	// ���� �� �������� (
				{
					queue.push(stack.top());	// ����������� �� ����� � �������
					stack.pop();

					if (stack.empty())
						return false;
				}
			}
			stack.pop();	// ���������� ( ��� �������, ����������� ���������� ���������� �������
			continue;
		}
		case LEX_PLUS: case LEX_MINUS: case LEX_STAR: case LEX_DIRSLASH:	// ���� ���� ���������
		{
			while (!stack.empty() && lextable.table[i].priority <= stack.top().priority)	// ���� ��������� �������� ��������� 
																							// ������ ��� ����� ���������� ��������� � ������� �����
			{
				queue.push(stack.top());	// ����������� �� ����� � �������� ������
				stack.pop();
			}
			stack.push(lextable.table[i]);
			continue;
		}
		case LEX_COMMA:	// ���� �������
		{
			findComma = true;
			countParm++;
		}
		}
	}
	while (!stack.empty())	// ���� ���� �� ������
	{
		if (stack.top().lexema == LEX_LEFTTHESIS || stack.top().lexema == LEX_RIGHTTHESIS)
			return false;
		queue.push(stack.top());	// ����������� ��� � �������
		stack.pop();
	}
	while (countLex != 0)		// ������ �������� ��������� � ������� ������ �� ��������� � �����
	{
		if (!queue.empty()) {
			lextable.table[posLex++] = queue.front();
			queue.pop();
		}
		else
		{
			lextable.table[posLex++] = temp;			
		}
		countLex--;
	}

	for (int i = 0; i < posLex; i++)		// �������������� �������� ������� ��������� � ������� ������ � ���������� �� ������� ���������������
	{
		if (lextable.table[i].lexema == LEX_LITERAL || lextable.table[i].lexema == LEX_ID)
			idtable.table[lextable.table[i].idxTI].idxfirstLE = i;
	}
	return true;
}

bool startPolishNotation(LT::LexTable& lextable, IT::IdTable& idtable)
{
	bool rc = false;
	for (int i = 0; i < lextable.size; i++)
	{
		if (lextable.table[i].lexema == LEX_EQUAL)		// ���� ����� �������� ������������
		{
			rc = PolishNotation(i + 1, lextable, idtable);		// �������� ����� ��������� �� ��� �������
			if (!rc)
				return rc;
		}
	}
	return rc;
}