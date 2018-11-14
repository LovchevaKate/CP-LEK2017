#pragma once

#include "IT.h"
#include "LexAnaliz.h"
#include "LT.h"
#include "In.h"
#include "Log.h"


namespace Semantic
{
	bool semanticsCheck(Lexer::LEX& tables, Log::LOG& log);
};