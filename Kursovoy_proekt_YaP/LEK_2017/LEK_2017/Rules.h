#pragma once
#include "GRB.h"
#define GRB_ERROR_SERIES 600
#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)

namespace GRB
{
	Greibach greibach(NS('S'), TS('$'), 11,

		Rule(NS('S'), GRB_ERROR_SERIES, 3,						// �������� ��������� ���������	
			Rule::Chain(6, TS('t'), TS('f'), TS('i'), NS('P'), NS('T'), NS('S')),
			Rule::Chain(6, TS('p'), TS('f'), TS('i'), NS('P'), NS('G'), NS('S')),
			Rule::Chain(4, TS('m'), TS('['), NS('L'), TS(']'))
		),

		Rule(NS('P'), GRB_ERROR_SERIES + 1, 2,					// �� ������ ������ ���������� �������	
			Rule::Chain(2, TS('('), TS(')')),
			Rule::Chain(3, TS('('), NS('E'), TS(')'))
		),

		Rule(NS('T'), GRB_ERROR_SERIES + 2, 2,					// ������ � ���� �������
			Rule::Chain(5, TS('['), TS('e'), NS('V'), TS(';'), TS(']')),
			Rule::Chain(6, TS('['), NS('L'), TS('e'), NS('V'), TS(';'), TS(']'))
		),

		Rule(NS('G'), GRB_ERROR_SERIES + 3, 2,					// ������ � ���� ���������
			Rule::Chain(4, TS('['), TS('e'), TS(';'), TS(']')),
			Rule::Chain(5, TS('['), NS('L'), TS('e'), TS(';'), TS(']'))
		),

		Rule(NS('E'), GRB_ERROR_SERIES + 4, 2,					// ������ � ������ ���������� �������						
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('E'))
		),

		Rule(NS('F'), GRB_ERROR_SERIES + 5, 2,					// ������ � ������ �������						
			Rule::Chain(2, TS('('), TS(')')),
			Rule::Chain(3, TS('('), NS('N'), TS(')'))
		),

		Rule(NS('N'), GRB_ERROR_SERIES + 6, 4,					// ������ � ������ ���������� �������		
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('N')),
			Rule::Chain(3, TS('l'), TS(','), NS('N'))
		),

		Rule(NS('A'), GRB_ERROR_SERIES + 7, 4,					// �������� �������������� ��������
			Rule::Chain(1, TS('+')),
			Rule::Chain(1, TS('-')),
			Rule::Chain(1, TS('*')),
			Rule::Chain(1, TS('/'))
		),

		Rule(NS('V'), GRB_ERROR_SERIES + 8, 2,					// �������� ���������. ��������� ������ �������������� � ��������
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),

		Rule(NS('W'), GRB_ERROR_SERIES + 9, 8,					// ������ � ������������� ���������
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('W'), TS(')')),
			Rule::Chain(5, TS('('), NS('W'), TS(')'), NS('A'), NS('W')),
			Rule::Chain(2, TS('i'), NS('F')),
			Rule::Chain(3, TS('i'), NS('A'), NS('W')),
			Rule::Chain(3, TS('l'), NS('A'), NS('W')),
			Rule::Chain(4, TS('i'), NS('F'), NS('A'), NS('W'))
		),

		Rule(NS('L'), GRB_ERROR_SERIES + 10, 10,				// ������������ �������������� �����������

			Rule::Chain(5, TS('n'), TS('t'), TS('i'), TS(';'), NS('L')),	// ����������
			Rule::Chain(5, TS('i'), TS('='), NS('W'), TS(';'), NS('L')),	// ������������
			Rule::Chain(4, TS('o'), NS('V'), TS(';'), NS('L')),				// �����
			Rule::Chain(3, TS('^'), TS(';'), NS('L')),						// ������� ������
			Rule::Chain(4, TS('i'), NS('F'), TS(';'), NS('L')),				// ����� �������

			Rule::Chain(4, TS('i'), TS('='), NS('W'), TS(';')),			// ������������
			Rule::Chain(4, TS('n'), TS('t'), TS('i'), TS(';')),			// ����������
			Rule::Chain(3, TS('o'), NS('V'), TS(';')),					// �����
			Rule::Chain(2, TS('^'), TS(';')),							// ������� ������
			Rule::Chain(3, TS('i'), NS('F'), TS(';'))					// ����� �������
		)


	);

}