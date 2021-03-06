#include "stdafx.h"
#include <iostream>

extern "C"
{

	int __stdcall plength(char* buffer, char* str)
	{
		// буфер не используется
		if (str == nullptr) 
			return 0;
		int len = 0;
		for (int i = 0; i < 256; i++)
			if (str[i] == '\0')
			{
				len = i;
				break;
			}
		return len;
	}

	char*  __stdcall catstring(char* buffer, char* str1, char* str2)
	{
		int i = 0, len1 = 0, len2 = 0;
		for (; str1[len1] != '\0'; len1++);
		for (; str2[len2] != '\0'; len2++);

		for (int j = 0; str1[j] != '\0'; j++)
		{
			if (i == 255) break;
			buffer[i++] = str1[j];
		}
		for (int j = 0; str2[j] != '\0'; j++)
		{
			if (i == 255) break;
			buffer[i++] = str2[j];
		}
		buffer[i] = '\0';
		return buffer;
	}

	int __stdcall printstr(unsigned char* value)
	{
		std::cout << value << std::endl;
		return 0;
	}

	int __stdcall printnum(unsigned int value)
	{
		std::cout << value << std::endl;
		return 0;
	}

}
