#include "stdafx.h"
#include "Polish.h"
#include "GRB.h"
#include "MFST.h"
#include "Gen.h"
#include "Sem.h"
using namespace std;

#define ERROR_LEX      "\nОшибки в ЛЕКСИЧЕСКОМ АНАЛИЗЕ\n"
#define ERROR_SYNT     "\nОшибки в СИНТАКСИЧЕСКОМ АНАЛИЗЕ\n"
#define ERROR_SEM	   "\nСЕМАНТИЧЕСКИЕ ОШИБКИ\n"
#define ERROR_POLISH   "\nОшибка при попытке преобразования выражения\n"
#define CLOSE		   " !!!Выполнение программы остановлено!!! "
#define OK		       "\nПрограмма завершена!"


int _tmain(int argc, wchar_t *argv[])
{
	setlocale(0, "");
	Log::LOG log;

	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getstream(parm.log);
		Log::writeLog(log);
		Log::writeParm(log, parm);


		In::IN in = In::getin(parm.in, log.stream);
		Log::writeIn(log.stream, in);
		in.words = In::getWordsTable(log.stream, in.text, in.code, in.size);
		Log::writeWords(log.stream, in.words);

		Lexer::LEX tables;
		bool analyze = Lexer::analyze(tables, in, log, parm);
		if (!analyze)
		{
			Log::writeLine(log.stream, (char *)ERROR_LEX, "");
			cout << "ERROR::LEX" << endl;
			return 0;
		}

		MFST_TRACE_START
		MFST::Mfst mfst(tables, GRB::getGreibach());
		mfst.start();
		mfst.savededucation();
		mfst.printrules();

		bool sem_ok = Semantic::semanticsCheck(tables, log);
		if (!sem_ok)
			cout << "ERROR::SEM" << endl;

		bool polish_ok = startPolishNotation(tables.lextable, tables.idtable);
		if (!polish_ok)
			cout<< "ERROR::POLISH"<<endl;
		else cout << "Польская запись построена" << endl;
		Log:: writeLT(tables.lextable);
		Log:: writeIT(tables.idtable);
		Log::writeLexemsOnLines(tables.lextable);


		Gener::CodeGeneration(tables, parm, log);
		Log::writeLine(log.stream, (char *)OK, "");
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		Log::writeError(log.stream, e);
	}
}