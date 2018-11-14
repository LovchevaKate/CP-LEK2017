#include "stdafx.h"

namespace FST
{
	RELATION::RELATION(char c = NULL, short ns = NULL) // ребро: символ -> вершина графов переходов КА
	{
		symbol = c; // символ перехода
		nnode = ns; // новое состояние
	};

	NODE::NODE()//вершина графа переходов
	{
		n_relation = NULL;//количество инциндентных ребер
		relations = NULL;//инциндентные ребра
	};

	NODE::NODE(short n, RELATION rel, ...)//количество инциндентных ребер, список ребер
	{
		RELATION* temp = &rel;
		relations = new RELATION[n];//инциндентные ребра
		n_relation = n;//количество инциндентных ребер
		for (short i = 0; i < n; i++)
			relations[i] = *(temp + i);
	};

	FST::FST(short ns, NODE n, ...) //(массив)количество состояний автомата, список состояний(граф переходов)
	{
		node = new NODE[ns];//(массив)количество состояний автомата
		NODE* temp = &n;// список состояний(граф переходов)
		nstates = ns;//количество состояний автомата
		rstates = new short[ns];//возможные состояния автомата на данной позиции
		for (short i = 0; i < ns; i++)
			node[i] = *(temp + i);
	}

	FST::FST(char* s, FST& fst)// количество состояний автомата, список состояний(граф переходов)
	{
		node = new NODE[fst.nstates];//граф переходов = кол-ву состояний автомата
		NODE* temp = fst.node;//вершина = начальному состоянию
		string = s;//слово
		nstates = fst.nstates;//кол-во состояний автомата
		rstates = new short[nstates]; //возможные состояния автомата на данной позиции
		for (short i = 0; i < nstates; i++)
			node[i] = *(temp + i);
	}

	bool execute(FST& fst)//функция распознавания цепочки
	{
		int i, j;
		memset(fst.rstates, -1, fst.nstates * sizeof(short)); // Заполняем fst.rstates(//возможные состояния автомата на данной позиции) массив
		                                                      // -1-ами размером  fst.nstates(//количество состояний автомата) * sizeof(short)
		for (fst.rstates[0] = 0, fst.position = 0; fst.position < (signed)(strlen(fst.string)); fst.position++)//(пока цепочка не закончится(строка, завершается 0х00))
		{
			for (i = 0; i < fst.nstates; i++)//пока не закончатся количество состояний автомата
			{
				if (fst.rstates[i] == fst.position)//возможные состояния автомата на данной позиции == текущая позиция в цепочке
				{
					for (j = 0; j < fst.node[i].n_relation; j++) //ррелэшены (пока не закончатся инцидентные рёбра)
					{
						if (fst.node[i].relations[j].symbol == fst.string[fst.position])//символ инцид ребра = символу в цепочке
						{
							fst.rstates[fst.node[i].relations[j].nnode] = fst.position + 1;//перемещаемся на следующую позицию (вершину)
						}
					}
				}
			}
		}
		return (fst.rstates[fst.nstates - 1] == (strlen(fst.string))); // Совпадает ли конечное позия с длиной строки
	}
};