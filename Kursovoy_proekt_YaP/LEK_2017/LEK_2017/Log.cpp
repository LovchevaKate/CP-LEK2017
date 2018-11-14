#include "stdafx.h"

#define W(x, y)  << setw(x) << (y) <<
#define STR(n, line, type, id)\
	"|" W(4,n) " |  " W(5,line) "    |" W(17,type) " |  " W(SCOPED_ID_MAXSIZE, id) " |"


namespace Log
{
	LOG getstream(wchar_t  logfile[])
	{
		LOG stream;
		stream.stream = new ofstream;
		stream.stream->open(logfile);
		if (!stream.stream->is_open())
			throw ERROR_THROW(103); // ошибка при создании файла протокола
		wcscpy_s(stream.logfile, logfile);
		return stream;
	}

	void writeLog(LOG &log)
	{
		char buffer[80];
		time_t seconds = time(NULL);
		tm* timeinfo = localtime(&seconds);
		char* format = (char*)"%d.%m.%Y %H:%M:%S";
		strftime(buffer, 80, format, timeinfo);
		*log.stream << "\n----------- Протокол ------------ Дата: " << buffer << " ------------ \n\n";
	}

	void writeLine(ostream *stream, char* c, ...)		// вывести в протокол конкатенацию строк
	{
		*stream << c << endl;
		cout << c << endl;
	}

	void writeParm(LOG& log, Parm::PARM& parm)
	{
		char inTxt[PARM_MAX_SIZE],
			outTxt[PARM_MAX_SIZE],
			logTxt[PARM_MAX_SIZE];
		wcstombs(inTxt, parm.in, wcslen(parm.in) + 1);
		wcstombs(outTxt, parm.out, wcslen(parm.out) + 1);
		wcstombs(logTxt, parm.log, wcslen(parm.log) + 1);
		*log.stream << "\n----- Параметры --------";
		*log.stream << "\n-in: " << inTxt
			<< "\n-out: " << outTxt
			<< "\n-log: " << logTxt << "\n\n";
	}

	void writeIn(ostream* stream, In::IN& in)
	{
		*stream << "\n**** Исходные данные ****";
		*stream << "\nКоличество символов: " << setw(3) << in.size
			<< "\nПроигнорировано: " << setw(3) << in.ignor
			<< "\nКоличество строк: " << setw(3) << in.lines << "\n\n";
	}

	void writeError(ostream *stream, Error::ERROR e)
	{
		if (stream == NULL)
		{
			if (e.position.cells == -1 || e.position.line == -1)
				cout << endl << "Ошибка N" << e.id << ": " << e.message << endl;
			else if (e.position.cells == NULL)
				cout << endl << "Ошибка N" << e.id << ": " << e.message
				<< " Строка: " << e.position.line << endl;
			else
				cout << endl << "Ошибка N" << e.id << ": " << e.message
				<< " Строка: " << e.position.line
				<< " Позиция в строке: " << e.position.cells << endl;
			system("pause");
		}
		else
		{
			if (e.position.cells == -1 || e.position.line == -1)
				*stream << endl << "Ошибка N" << e.id << ": " << e.message;
			else if (e.position.cells == NULL)
				*stream << endl << "Ошибка N" << e.id << ": " << e.message
				<< " Строка: " << e.position.line;
			else
				*stream << endl << "Ошибка N" << e.id << ": " << e.message
				<< " Строка: " << e.position.line
				<< " Позиция в строке: " << e.position.cells;
			*stream << endl;
		}
	}

	void writeLT(LT::LexTable& lextable)
	{
		ofstream fout("LT.txt", ios_base::app);
		if (!fout.is_open()) // если файл не открыт
			cout << "Файл не может быть открыт!\n"; // сообщить об этом
		char buffer[80];
		time_t seconds = time(NULL);
		tm* timeinfo = localtime(&seconds);
		char* format = (char*)"%d.%m.%Y %H:%M:%S";
		strftime(buffer, 80, format, timeinfo);
		fout << "\n-------------- ТАБЛИЦА ЛЕКСЕМ  -------------------- Дата: " << buffer << " ------------ \n\n";
		fout << "|  № | Лексема | Строка  | Индекс в ТИ |" << endl;
		for (int i = 0; i < lextable.size; i++)
		{
			fout << "|" << setw(3) << i << " | " << setw(4) << lextable.table[i].lexema << "    |   " << setw(3)
				<< lextable.table[i].sn << "   |";
			if (lextable.table[i].idxTI == -1)
				fout << "             |" << endl;
			else
				fout << setw(8) << lextable.table[i].idxTI << "     |" << endl;
		}
	}

	void writeIT(IT::IdTable& idtable)
	{
		ofstream fout("IT.txt", ios_base::app);
		if (!fout.is_open()) // если файл не открыт
			cout << "Файл не может быть открыт!\n"; // сообщить об этом
		char buffer[80];
		time_t seconds = time(NULL);
		tm* timeinfo = localtime(&seconds);
		char* format = (char*)"%d.%m.%Y %H:%M:%S";
		strftime(buffer, 80, format, timeinfo);
		fout << "\n-------------- ТАБЛИЦА ИДЕНТИФИКАТОРОВ  -------------------- Дата: " << buffer << " ------------ \n\n";
		fout << "|  №  |Строка в ТЛ| Тип Идентификатора |        Имя        | Значение (Параметры)" << std::endl;
		for (int i = 0; i < idtable.size; i++)
		{
			IT::Entry *e = &idtable.table[i];
			char type[50] = "";

			switch (e->iddatatype) {
			case IT::IDDATATYPE::INT: strcat(type, "  number "); break;
			case IT::IDDATATYPE::STR: strcat(type, "  string "); break;
			case IT::IDDATATYPE::PROC: strcat(type, "  proc   "); break;
			case IT::IDDATATYPE::UNDEF: strcat(type, "UNDEFINED"); break;
			}
			switch (e->idtype) {
			case IT::IDTYPE::V: strcat(type, "  variable"); break;
			case IT::IDTYPE::F: strcat(type, "  function"); break;
			case IT::IDTYPE::P: strcat(type, " parameter"); break;
			case IT::IDTYPE::L: strcat(type, "   literal"); break;
			case IT::IDTYPE::S: strcat(type, "  LIB FUNC"); break;
			default: strcat(type, "UNDEFINED "); break;
			}

			fout << STR(i, e->idxfirstLE, type, e->id);
			if (e->idtype == IT::IDTYPE::L || e->idtype == IT::IDTYPE::V && e->iddatatype != IT::IDDATATYPE::UNDEF)
			{
				if (e->iddatatype == IT::IDDATATYPE::INT) fout << e->value.vint;
				else fout << "[" << (int)e->value.vstr.len << "]" << e->value.vstr.str;
			}
			if (e->idtype == IT::IDTYPE::F || e->idtype == IT::IDTYPE::S)
			{
				for (int i = 0; i < e->value.params.count; i++)
				{
					fout << " P" << i << ":";
					switch (e->value.params.types[i]) {
					case IT::IDDATATYPE::INT: fout << "NM |"; break;
					case IT::IDDATATYPE::STR: fout << "LN |"; break;
					case IT::IDDATATYPE::PROC:
					case IT::IDDATATYPE::UNDEF: fout << "UNDEFINED"; break;
					}
				}
			}
			fout << endl;
		}
		fout << "\n***********************************************************************\n\n";
	}

	void writeLexemsOnLines(LT::LexTable& lextable)
	{
		ofstream fout("LT1.txt", ios_base::app);
		if (!fout.is_open()) // если файл не открыт
			cout << "Файл не может быть открыт!\n"; // сообщить об этом
		fout << "\n********  Лексемы, Соответствующие Исходному Коду ********\n" << endl;
		for (int i = 0; i < lextable.size; )
		{
			int line = lextable.table[i].sn;
			fout << setw(3) << line << " | ";
			while (lextable.table[i].sn == line || lextable.table[i].sn == -1)
			{
				fout << lextable.table[i].lexema;
				/*if (lextable.table[i].idxTI != LT_TI_NULLDX)
					fout << "[" << lextable.table[i].idxTI << "]";*/
				i++;
			}
			fout << endl;
		}
		fout << "*******************************************************************************\n\n";
	}

	void writeWords(ostream* stream, In::InWord* words)
	{
		*stream << " ******** ТАБЛИЦА СЛОВ ********" << endl;
		*stream << words->size << endl;
		for (int i = 0; i < words->size; i++)
			*stream << setw(2) << i << setw(3) << words[i].line << " |  " << words[i].word << endl;
		*stream << "\n****************************************************************************\n\n";
	}


	void Close(LOG& log)
	{
		(*log.stream).close();
	}
};