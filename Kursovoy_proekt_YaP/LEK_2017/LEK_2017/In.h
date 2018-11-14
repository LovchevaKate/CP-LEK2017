#pragma once

#define IN_MAX_LEN_TEXT 1024*1024
#define MAX_LEN_BUFFER  2048
#define IN_CODE_ENDL  '\n'
#define IN_CODE_SPACE  ' '
#define IN_CODE_NULL  '\0'
#define IN_CODE_QUOTE '\"'


// 28 29  ( )
// 3c 3e  < >
// 5b 5d  [ ]
// 7b 7d  { }
// 22 "

// одинарные кавычки запрещены
#define IN_CODE_TABLE	{\
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::P, IN::N, IN::F,  IN::F, IN::F, IN::F, IN::F,\
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,\
		IN::P, IN::S, IN::Q, IN::S,  IN::T, IN::T, IN::S, IN::F,   IN::S, IN::S, IN::S, IN::S,  IN::S, IN::S, IN::T, IN::T,\
		IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T, IN::T,   IN::T, IN::T, IN::T, IN::S,  IN::S, IN::S, IN::S, IN::T,\
		IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T, IN::T,   IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T, IN::T,\
		IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T, IN::T,   IN::T, IN::T, IN::T, IN::S,  IN::T, IN::S, IN::T, IN::T,\
		IN::F, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T, IN::T,   IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T, IN::T,\
		IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T, IN::T,   IN::T, IN::T, IN::T, IN::T,  IN::T, IN::T, IN::T, IN::T,\
																														   \
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,\
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,\
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,\
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,\
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,\
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,\
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,\
		IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F,   IN::F, IN::F, IN::F, IN::F,  IN::F, IN::F, IN::F, IN::F \
}


namespace In
{
	struct InWord
	{
		char word[MAX_LEN_BUFFER];	// слово
		int line;		// строка в исходном тексте
		static int size;// количество слов
	};

	struct IN
	{
		/*
		S - ( ) [ ] # < > ! & * + - = , ;
		P - пробел, табуляция
		N - новая строка
		F - запрещённый
		T - разрешённый
		I - игнорируемый
		Q - "
		*/
		enum { T = 1024, F = 2048, I = 4096, S, Q, P, N };
		int code[256] = IN_CODE_TABLE;
		int size = 0;
		int lines = 1;
		int ignor = 0;
		unsigned char *text;
		InWord* words;
	};

	IN getin(wchar_t infile[], std::ostream *stream);
	void addWord(InWord* words, char* word, int line);
	InWord* getWordsTable(std::ostream* stream, unsigned char* text, int* code, int textSize);
};