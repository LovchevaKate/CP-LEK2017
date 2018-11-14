#include "stdafx.h"

namespace Error
{
	/*
	100 - системные ошибки
	200 - лексические ошибки
	300 - семантические ошибки
	600 - синтаксические ошибки
	*/
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "***СИСТЕМНАЯ*** Недопустимый код ошибки"),		
		ERROR_ENTRY(1, "***СИСТЕМНАЯ*** Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40),
		ERROR_ENTRY_NODEF10(50), ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "***СИСТЕМНАЯ*** Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "***СИСТЕМНАЯ*** Превышена длина входного параметра"),
		ERROR_ENTRY(102, "***СИСТЕМНАЯ*** Ошибка при открытии файла с исходным кодом(-in)"),
		ERROR_ENTRY(103, "***СИСТЕМНАЯ*** Ошибка при создании файла протокола(-log)"),
		ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY_NODEF10(110), ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140),
		ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY(200, "***ЛА*** Недопустимый символ в исходном файле(-in)"),		
		ERROR_ENTRY(201, "***ЛА*** Неизвестная последовательность символов"),		
		ERROR_ENTRY(202, "***ЛА*** Превышен размер таблицы лексем"),
		ERROR_ENTRY(203, "***ЛА*** Превышен размер таблицы идентификаторов"),		
		ERROR_ENTRY_NODEF(204), ERROR_ENTRY_NODEF(205), ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240),
		ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY(300, "***СЕМ*** Необъявленный идентификатор"),
		ERROR_ENTRY(301, "***СЕМ*** Отсутствует точка входа main"),
		ERROR_ENTRY(302, "***СЕМ*** Обнаружено несколько точек входа main"),
		ERROR_ENTRY(303, "***СЕМ*** В объявлении не указан тип идентификатора"),
		ERROR_ENTRY(304, "***СЕМ*** В объявлении отсутствует ключевое слово new"),
		ERROR_ENTRY(305, "***СЕМ*** Попытка переопределения идентификатора"),
		ERROR_ENTRY(306, "***СЕМ*** Превышено максимальное количество параметров функции"),
		ERROR_ENTRY(307, "***СЕМ*** Слишком много параметров в вызове"),
		ERROR_ENTRY(308, "***СЕМ*** Кол-во ожидаемыех функцией и передаваемых параметров не совпадают"),
		ERROR_ENTRY(309, "***СЕМ*** Несовпадение типов передаваемых параметров"),
		ERROR_ENTRY(310, "***СЕМ*** Использование пустого строкового литерала недопустимо"),
		ERROR_ENTRY(311, "***СЕМ*** Обнаружен символ \'\"\'. Возможно, не закрыт строковый литерал"),
		ERROR_ENTRY(312, "***СЕМ*** Превышен размер строкового литерала"),
		ERROR_ENTRY(313, "***СЕМ*** Недопустимый целочисленный литерал"),
		ERROR_ENTRY(314, "***СЕМ*** Типы данных в выражении не совпадают"),
		ERROR_ENTRY(315, "***СЕМ*** Тип функции и возвращаемого значения не совпадают"),
		ERROR_ENTRY(316, "***СЕМ*** Недопустимое строковое выражение справа от знака \'=\'"),
		ERROR_ENTRY_NODEF(317),	ERROR_ENTRY_NODEF(318), ERROR_ENTRY_NODEF(319),
		ERROR_ENTRY_NODEF10(320),ERROR_ENTRY_NODEF10(330),ERROR_ENTRY_NODEF10(340),ERROR_ENTRY_NODEF10(350),
		ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390),
		ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "***СИНТАКСИС*** Ошибка. Неверная структура программы"),
		ERROR_ENTRY(601, "***СИНТАКСИС*** Ошибка. Не найден список параметров функции"),
		ERROR_ENTRY(602, "***СИНТАКСИС*** Ошибка в теле функции"),
		ERROR_ENTRY(603, "***СИНТАКСИС*** Ошибка в теле процедуры"),
		ERROR_ENTRY(604, "***СИНТАКСИС*** Ошибка в списке параметров функции"),
		ERROR_ENTRY(605, "***СИНТАКСИС*** Ошибка в вызове функции/выражении"),
		ERROR_ENTRY(606, "***СИНТАКСИС*** Ошибка в списке фактических параметров функции"),
		ERROR_ENTRY(607, "***СИНТАКСИС*** Ошибка. Неверный арифметический оператор"),
		ERROR_ENTRY(608, "***СИНТАКСИС*** Ошибка. Неверное выражение. Ожидаются только идентификаторы/литералы"),
		ERROR_ENTRY(609, "***СИНТАКСИС*** Ошибка в арифметическом выражении"),
		ERROR_ENTRY(610, "***СИНТАКСИС*** Ошибка. Недопустимая синтаксическая конструкция"),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};

	ERROR GetError(int id)
	{
		if (id < 0 || id > ERROR_MAX_ENTRY)
			return errors[0];
		else
			return errors[id];
	}

	ERROR GetError(int id, int line, int cells)
	{
		ERROR err = GetError(id);
		err.position.cells = cells;
		err.position.line = line;
		return err;
	}
};