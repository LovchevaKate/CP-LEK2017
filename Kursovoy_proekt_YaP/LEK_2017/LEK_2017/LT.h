#pragma once

#define LEX_SEPARATORS	 'S'	// �����������
#define	LEX_ID_TYPE    	 't'	// ���� ������		
#define	LEX_ID			 'i'	// ��������������		
#define	LEX_LITERAL		 'l'	// ��������		
#define	LEX_FUNCTION     'f'	// function		
#define	LEX_MAIN		 'm'	// main		
#define	LEX_SEPARATOR	 ';'	// ����������� ����������
#define	LEX_COMMA		 ','	// ����������� ���������� 		
#define	LEX_LEFTBRACE	 '['	// ������ �����		
#define	LEX_BRACELET	 ']'	// ����� �����		
#define	LEX_LEFTTHESIS	 '('	// ������ ������ ����������		
#define	LEX_RIGHTTHESIS	 ')'	// ����� ������ ����������		
#define	LEX_PLUS		 '+'	// ��������	
#define	LEX_MINUS		 '-'	// ���������
#define	LEX_STAR		 '*'	// ���������
#define LEX_DIRSLASH	 '/'	// �������
#define	LEX_EQUAL		 '='	// ������������				
#define LEX_PRINT	     'o'	// �����
#define LEX_ENDL		 '^'	// ������� ������
#define LEX_END		     'e'	// ����� �� ���������/�������
#define LEX_PROC		 'p'	// �������� ����� ���������
#define LEX_NEW			 'n'	// ���������� ����������


#define	LEXEMA_FIXSIZE   1	        //������ �������
#define	LT_MAXSIZE		 4096	    //������������ ���������� ����� � ��
#define	LT_TI_NULLDX	 0xffffffff	//��� �������� ��


namespace LT	//������� ������
{
	struct Entry	// ������ ������� ������
	{
		char lexema;	// �������
		int sn;							// ����� ������ � �������� ������
		int idxTI;						// ������ � ������� ��������������� ��� LT_TI_NULLIDX
		int priority;					// ���������
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

	struct LexTable						//��������� ������� ������
	{
		int maxsize;					//������� ������� ������
		int size;						//������� ������ ������� ������
		Entry* table;					//������ ����� ��
	};

	LexTable Create(int size);		            //������� < LT_MAXSIZE
	void Add(LexTable &lextable, Entry entry);	//��������� ��, ������ ��
};