#pragma once

#include "stdafx.h"
#include "Parm.h"
#include "Error.h"

namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE];
		std::ofstream *stream;
		LOG() : stream(NULL)
		{
			wcscpy(logfile, L"");
		}
	};

	LOG getstream(wchar_t logfile[]);
	void writeLog(LOG &log);
	void writeLine(std::ostream *stream, char* c, ...);
	void writeParm(LOG &log, Parm::PARM &parm);
	void writeIn(std::ostream *stream, In::IN &in);
	void writeWords(std::ostream *stream, In::InWord* words);
	void writeError(std::ostream *stream, Error::ERROR error);
	void writeLexemsOnLines(LT::LexTable &lextable);
	void Close(LOG &log);
	void writeLT(LT::LexTable& lextable);
	void writeIT(IT::IdTable& idtable);
};
