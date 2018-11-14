#pragma once

#define LEX_SEPARATORS	 'S'	// разделители
#define	LEX_ID_TYPE    	 't'	// типы данных		
#define	LEX_ID			 'i'	// идентификаторы		
#define	LEX_LITERAL		 'l'	// литералы		
#define	LEX_FUNCTION     'f'	// function		
#define	LEX_MAIN		 'm'	// main		
#define	LEX_SEPARATOR	 ';'	// разделитель инструкций
#define	LEX_COMMA		 ','	// разделитель параметров 		
#define	LEX_LEFTBRACE	 '['	// начало блока		
#define	LEX_BRACELET	 ']'	// конец блока		
#define	LEX_LEFTTHESIS	 '('	// начало списка параметров		
#define	LEX_RIGHTTHESIS	 ')'	// конец списка параметров		
#define	LEX_PLUS		 '+'	// сложение	
#define	LEX_MINUS		 '-'	// вычитание
#define	LEX_STAR		 '*'	// умножение
#define LEX_DIRSLASH	 '/'	// деление
#define	LEX_EQUAL		 '='	// присваивание				
#define LEX_PRINT	     'o'	// вывод
#define LEX_ENDL		 '^'	// перевод строки
#define LEX_END		     'e'	// выход из процедуры/функции
#define LEX_PROC		 'p'	// ключевое слово процедуры
#define LEX_NEW			 'n'	// объявления переменных


#define	LEXEMA_FIXSIZE   1	        //размер лексемы
#define	LT_MAXSIZE		 4096	    //максимальное количество строк в ТЛ
#define	LT_TI_NULLDX	 0xffffffff	//нет элемента ТИ


namespace LT	//таблица лексем
{
	struct Entry	// строка таблицы лексем
	{
		char lexema;	// лексема
		int sn;							// номер строки в исходном тексте
		int idxTI;						// индекс в таблице идентификаторов или LT_TI_NULLIDX
		int priority;					// приоритет
		Entry(char l, int sn_1, int idxTI_1)
		{
			this->lexema = l;
			this->sn = sn_1;
			this->idxTI = idxTI_1;
		}
		Entry()
		{

		}
	};

	struct LexTable						//экземпляр таблицы лексем
	{
		int maxsize;					//ёмкость таблицы лексем
		int size;						//текущий размер таблицы лексем
		Entry* table;					//массив строк ТЛ
	};

	LexTable Create(int size);		            //ёмкость < LT_MAXSIZE
	void Add(LexTable &lextable, Entry entry);	//экземпляр ТЛ, строка ТЛ
};