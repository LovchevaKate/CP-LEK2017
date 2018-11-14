#pragma once

#define ID_MAXSIZE	9				//���� ����� �������� ��������������
#define SCOPED_ID_MAXSIZE   ID_MAXSIZE*2 //���� ����� �������� ������������� + ������� ���������
#define TI_MAXSIZE		4096			//���� ����� ���������� ����� � ������� ���������������
#define TI_INT_DEFAULT	0x00000000	//�������� �� ��������� ��� int
#define TI_STR_DEFAULT	0x00		//�������� �� ��������� ��� sting
#define TI_NULLIDX		0xffffffff	//��� �������� ������� ���������������
#define TI_STR_MAXSIZE	255
#define TI_NUM_MAXSIZE   8388607
#define TI_NUM_MINSIZE  -8388608

#define MAX_PARAMS_COUNT 3
#define CATSTRING_PARAMS_CNT 2
#define PLENGTH_PARAMS_CNT 1
#define CATSTRING_TYPE IT::IDDATATYPE::STR
#define PLENGTH_TYPE IT::IDDATATYPE::INT


namespace IT
{	//���� ������ ���������������: ��������, ���������, ��� ����(��� ��������), ��������������
	enum IDDATATYPE { INT = 1, STR = 2, PROC = 3, UNDEF };
	//���� ���������������: ����������, �������, ��������, �������, ����������� �������
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5 };
	//����������� �������
	enum STDFNC { F_CATSTRING, F_PLENGTH, F_NOT_STD };

	static const IDDATATYPE CATSTRING_PARAMS[] = { IT::IDDATATYPE::STR, IT::IDDATATYPE::STR };
	static const IDDATATYPE PLENGTH_PARAMS[] = { IT::IDDATATYPE::STR };

	struct Entry//��������� ������� ���������������
	{
		int			idxfirstLE;				//������ � ������� ������		
		char		id[SCOPED_ID_MAXSIZE];	//�������������
		IDDATATYPE	iddatatype;				//��� ������
		IDTYPE		idtype;					//��� ��������������
		union
		{
			int	vint;            	//�������� integer
			struct
			{
				int len;							//���������� ��������
				char str[TI_STR_MAXSIZE - 1];		//�������
			} vstr;	//�������� ������

			struct
			{
				int count;			// ���������� ���������� �������
				IDDATATYPE *types;	//���� ���������� �������
			} params;
		} value;	//�������� ��������������

		Entry()
		{
			value.vint = TI_INT_DEFAULT;
			value.vstr.len = NULL;
			value.params.count = NULL;
		};
	};
	struct IdTable		//��������� ������� ���������������
	{
		int maxsize;	//������� ������� ��������������� < TI_MAXSIZE
		int size;		//������� ������ ������� ��������������� < maxsize
		Entry* table;	//������ ����� ������� ���������������
	};

	IdTable Create(int size = NULL);	//������� ������� ��������������� < TI_MAXSIZE

	void Add(					//�������� ������ � ������� ���������������
		IdTable &idtable,		//��������� ������� ���������������
		Entry entry);			//������ ������� ��������������� 

	int isId(					//�������: ����� ������(���� ����), TI_NULLIDX(���� ����)
		IdTable &idtable,		//��������� ������� ���������������
		char id[SCOPED_ID_MAXSIZE]);	//�������������

	bool SetValue(IT::Entry* entry, char* value);	//������ �������� ��������������
	bool SetValue(IT::IdTable& idtable, int index, char* value);
};