#include "stdafx.h"

#include "Gen.h"

using namespace std;

namespace Gener
{
	static int conditionnum = 0;

	string itoS(int x) { stringstream r;  r << x;  return r.str(); }
	string genCallFuncCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;

		IT::Entry e = ITENTRY(i); // идентификатор вызываемой функции
		stack <IT::Entry> temp;
		bool stnd = (e.idtype == IT::IDTYPE::S);//если стандартная функция

		for (int j = i + 1; LEXEMA(j) != LEX_RIGHTTHESIS; j++)//пока не )
		{
			if (LEXEMA(j) == LEX_ID || LEXEMA(j) == LEX_LITERAL)//если идентификатор или литерал
				temp.push(ITENTRY(j)); // // заполняем стек в прямом порядке	
		}
		str += "\n";

		// раскручиваем стек
		while (!temp.empty())//пока не пустой
		{
			if (temp.top().idtype == IT::IDTYPE::L && temp.top().iddatatype == IT::IDDATATYPE::STR)//если строковый литерал
				str = str + "push offset " + temp.top().id + "\n";//id[SCOPED_ID_MAXSIZE];	//идентификатор
			else   str = str + "push " + temp.top().id + "\n";
			temp.pop();
		}

		if (stnd)  str += "push offset buffer\n";//если стандартная функция
		str = str + "call " + string(e.id) + IN_CODE_ENDL;//вызываем нужную функцию
		// выравниваниe стека
		if (e.value.params.count != 0) str = str + "add esp, " + itoS(4 * e.value.params.count + 4) + "\n";//если количество параметров функции не 0, то очищаем стек
		return str;
	}
	string genEqualCode(Lexer::LEX& tables, Log::LOG& log, int i)
	{
		string str;
		IT::Entry e1 = ITENTRY(i - 1); // левый операнд

		switch (e1.iddatatype)
		{
		case IT::IDDATATYPE::INT:
		{
			bool first = true;
			for (int j = i + 1; LEXEMA(j) != LEX_SEPARATOR; j++)
			{
				switch (LEXEMA(j))
				{
				case LEX_LITERAL:
				case LEX_ID:
				{
					if (ITENTRY(j).idtype == IT::IDTYPE::F || ITENTRY(j).idtype == IT::IDTYPE::S) // если в выражении вызов функции
					{
						str = str + genCallFuncCode(tables, log, j); // функция возвращает результат в eax
						str = str + "push eax\n";				// результат выражения в стек для дальнейшего вычисления выражения
						while (LEXEMA(j) != LEX_RIGHTTHESIS) j++;
						break;
					}
					else  str = str + "push " + ITENTRY(j).id + "\n";
					break;
				}
				case LEX_PLUS:
					str = str + "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
				case LEX_MINUS:
					str = str + "pop ebx\npop eax\nsub eax, ebx\npush eax\n"; break;
				case LEX_STAR:
					str = str + "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
				case LEX_DIRSLASH:
					str = str + "pop ebx\npop eax\ncdq\nidiv ebx\npush eax\n"; break;
				}
			} // цикл вычисления

			str = str + "\npop ebx\nmov " + e1.id + ", ebx\n";			// вычисленное выражение в ebx 
			break;
		}
		case IT::IDDATATYPE::STR:// разрешить присваивать строкам только строки, литералы и вызовы функций
		{
			char lex = LEXEMA(i + 1);
			IT::Entry e2 = ITENTRY(i + 1);
			if (lex == LEX_ID && (e2.idtype == IT::IDTYPE::F || e2.idtype == IT::IDTYPE::S)) // вызов функции
			{
				str += genCallFuncCode(tables, log, i + 1);
				str = str + "mov " + e1.id + ", eax";
			}
			else if (lex == LEX_LITERAL) // литерал
			{
				str = str + "mov " + e1.id + ", offset " + e2.id;
			}
			else // ид(переменная) - через регистр
			{
				str = str + "mov ecx, " + e2.id + "\nmov " + e1.id + ", ecx";
			}
		}
		}

		return str;
	}
	string genFunctionCode(Lexer::LEX& tables, int i, string funcname, int pcount)
	{
		string str;
		IT::Entry e = ITENTRY(i + 1);
		IT::IDDATATYPE type = e.iddatatype;

		str = SEPSTR(funcname) + string(e.id) + string(" PROC,\n\t");
		//дальше параметры
		int j = i + 3; // начало - то что сразу после открывающей скобки
		while (LEXEMA(j) != LEX_RIGHTTHESIS) // пока параметры не кончатся
		{
			if (LEXEMA(j) == LEX_ID) // параметр
				str = str + string(ITENTRY(j).id) + (ITENTRY(j).iddatatype == IT::IDDATATYPE::INT ? " : sdword, " : " : dword, ");
			j++;
		}
		int f = str.rfind(',');
		if (f > 0)
			str[f] = IN_CODE_SPACE;

		str += "\n; --- save registers ---\npush ebx\npush edx\n; ----------------------";

		return str;
	}
	string genExitCode(Lexer::LEX& tables, int i, string funcname, int pcount)
	{
		string str = "; --- restore registers ---\npop edx\npop ebx\n; -------------------------\n";
		if (LEXEMA(i + 1) != LEX_SEPARATOR)	// выход из функции (вернуть значение)
		{
			str = str + "mov eax, " + string(ITENTRY(i + 1).id) + "\n";
		}
		if (pcount == NULL) 
			str += "ret\n";
		else str = str + "ret " + itoS(pcount * 4) + "\n";
		str += funcname + " ENDP" + SEPSTREMP;
		return str;
	}

	vector <string> startFillVector(Lexer::LEX& tables)
	{
		vector <string> v;
		v.push_back(BEGIN);
		v.push_back(EXTERN);

		vector <string> vlt;
		vlt.push_back(CONST);
		vector <string> vid;
		vid.push_back(DATA);

		for (int i = 0; i < tables.idtable.size; i++)// const, data
		{
			IT::Entry e = tables.idtable.table[i]; //массив строки таблицы идентификаторов
			string str = "\t\t" + string(e.id);

			if (tables.idtable.table[i].idtype == IT::IDTYPE::L)	// литерал - в .const
			{
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT:  str = str + " sdword " + itoS(e.value.vint);  break;
				case IT::IDDATATYPE::STR:  str = str + " byte '" + string(e.value.vstr.str) + "', 0";  break;
				}
				vlt.push_back(str);
			}
			else if (tables.idtable.table[i].idtype == IT::IDTYPE::V)// переменная - в .data
			{
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT: str = str + " sdword 0";  break;
				case IT::IDDATATYPE::STR: str = str + " dword ?";  break;
				}
				vid.push_back(str);
			}
		}


		v.insert(v.end(), vlt.begin(), vlt.end());
		v.insert(v.end(), vid.begin(), vid.end());
		v.push_back(CODE);
		return v;
	}

	void CodeGeneration(Lexer::LEX& tables, Parm::PARM& parm, Log::LOG& log)
	{
		vector <string> v = startFillVector(tables);
		ofstream ofile(parm.out);
		string funcname;	// имя текущей функции
		int pcount;			// количество параметров текущей функции
		string str;

		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (LEXEMA(i))
			{
			case LEX_MAIN:
			{
				str = str + SEPSTR("MAIN") + "main PROC";
				break;
			}
			case LEX_FUNCTION:
			{
				funcname = ITENTRY(i + 1).id;
				pcount = ITENTRY(i + 1).value.params.count;
				str = genFunctionCode(tables, i, funcname, pcount);
				break;
			}
			case LEX_END:
			{
				str = genExitCode(tables, i, funcname, pcount);
				break;
			}
			case LEX_ID: // вызов функции
			{
				if (LEXEMA(i + 1) == LEX_LEFTTHESIS && LEXEMA(i - 1) != LEX_FUNCTION) // не объявление, а вызов
					str = genCallFuncCode(tables, log, i);
				break;
			}
			case LEX_EQUAL: // присваивание (вычисление выражений)
			{
				str = genEqualCode(tables, log, i);
				while (LEXEMA(++i) != LEX_SEPARATOR);	// пропускаем выражение
				break;
			}
			case LEX_ENDL: // перевод строки 
			{
				str = str + "push offset endl\ncall printstr\nadd esp, 4\n";
				break;
			}
			case LEX_PRINT: // вывод
			{
				IT::Entry e = ITENTRY(i + 1);
				switch (e.iddatatype)
				{
				case IT::IDDATATYPE::INT:
					str = str + "\npush " + e.id + "\ncall printnum\n";
					break;
				case IT::IDDATATYPE::STR:
					if (e.idtype == IT::IDTYPE::L)  str = str + "\npush offset " + e.id + "\ncall printstr\n";
					else  str = str + "\npush " + e.id + "\ncall printstr\n";
					break;
				}
				str += "add esp, 4\n";
				break;
			}
			} // switch

			if (!str.empty()) v.push_back(str);
			str.clear();
		}

		v.push_back(END);
		// вывод в файл
		for (auto x : v) 
			ofile << x << endl;
		ofile.close();
	}
};