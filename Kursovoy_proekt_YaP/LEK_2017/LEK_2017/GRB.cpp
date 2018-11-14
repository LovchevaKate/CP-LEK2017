#include "stdafx.h"
#include <cstring>
#include "Rules.h"
#include "GRB.h"

namespace GRB
{

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)	// конструктор
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i) nt[i] = (GRBALPHABET)p[i];
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) // конструктор
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	};

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...) // конструктор
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	};

	Greibach getGreibach() { return greibach; };

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) // получить правило, возвращающая номер правила или -1
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
			k++;
		if (k < size)
			prule = rules[rc = k];
		return rc;
	};

	Rule Greibach::getRule(short n)	// // получить правило по номеру
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain)		// полчить правило в виде N->цепочка
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) // получить следующую за j подходящую цепочку, вернуть её номер или -1
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t) ++j;		// цикл по всем цепочкам в правиле
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	};

	char* Rule::Chain::getCChain(char* b)	// получить правую сторону правила (цепочку)
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};

}