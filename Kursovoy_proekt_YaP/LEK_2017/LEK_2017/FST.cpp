#include "stdafx.h"

namespace FST
{
	RELATION::RELATION(char c = NULL, short ns = NULL) // �����: ������ -> ������� ������ ��������� ��
	{
		symbol = c; // ������ ��������
		nnode = ns; // ����� ���������
	};

	NODE::NODE()//������� ����� ���������
	{
		n_relation = NULL;//���������� ������������ �����
		relations = NULL;//������������ �����
	};

	NODE::NODE(short n, RELATION rel, ...)//���������� ������������ �����, ������ �����
	{
		RELATION* temp = &rel;
		relations = new RELATION[n];//������������ �����
		n_relation = n;//���������� ������������ �����
		for (short i = 0; i < n; i++)
			relations[i] = *(temp + i);
	};

	FST::FST(short ns, NODE n, ...) //(������)���������� ��������� ��������, ������ ���������(���� ���������)
	{
		node = new NODE[ns];//(������)���������� ��������� ��������
		NODE* temp = &n;// ������ ���������(���� ���������)
		nstates = ns;//���������� ��������� ��������
		rstates = new short[ns];//��������� ��������� �������� �� ������ �������
		for (short i = 0; i < ns; i++)
			node[i] = *(temp + i);
	}

	FST::FST(char* s, FST& fst)// ���������� ��������� ��������, ������ ���������(���� ���������)
	{
		node = new NODE[fst.nstates];//���� ��������� = ���-�� ��������� ��������
		NODE* temp = fst.node;//������� = ���������� ���������
		string = s;//�����
		nstates = fst.nstates;//���-�� ��������� ��������
		rstates = new short[nstates]; //��������� ��������� �������� �� ������ �������
		for (short i = 0; i < nstates; i++)
			node[i] = *(temp + i);
	}

	bool execute(FST& fst)//������� ������������� �������
	{
		int i, j;
		memset(fst.rstates, -1, fst.nstates * sizeof(short)); // ��������� fst.rstates(//��������� ��������� �������� �� ������ �������) ������
		                                                      // -1-��� ��������  fst.nstates(//���������� ��������� ��������) * sizeof(short)
		for (fst.rstates[0] = 0, fst.position = 0; fst.position < (signed)(strlen(fst.string)); fst.position++)//(���� ������� �� ����������(������, ����������� 0�00))
		{
			for (i = 0; i < fst.nstates; i++)//���� �� ���������� ���������� ��������� ��������
			{
				if (fst.rstates[i] == fst.position)//��������� ��������� �������� �� ������ ������� == ������� ������� � �������
				{
					for (j = 0; j < fst.node[i].n_relation; j++) //��������� (���� �� ���������� ����������� ����)
					{
						if (fst.node[i].relations[j].symbol == fst.string[fst.position])//������ ����� ����� = ������� � �������
						{
							fst.rstates[fst.node[i].relations[j].nnode] = fst.position + 1;//������������ �� ��������� ������� (�������)
						}
					}
				}
			}
		}
		return (fst.rstates[fst.nstates - 1] == (strlen(fst.string))); // ��������� �� �������� ����� � ������ ������
	}
};