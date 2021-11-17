/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_42sh_define.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amatilda <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/17 12:06:23 by amatilda          #+#    #+#             */
/*   Updated: 2019/06/25 15:24:38 by amatilda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_42SH_DEFINE_H
# define FT_42SH_DEFINE_H

# include "pguitar/ft_42sh_pguitar_define.h"

# define PROG_42SH					"42sh"
# define CRIT_42SH					"\x1b[31m"
# define CRIT_PR_42SH				"\x1b[31m""42sh"
# define WAR_42SH					"\x1b[31m"
# define WAR_PR_42SH				"\x1b[31m""42sh"

# define VERSION_LIST_42SH			0x1
# define LIMIT_LIST_42SH			0xFFFF
# define BUFFER_READ_42SH			0x20//должен быть кратене sizeof(sizet) - не меньше , что бы при сохранении в листов структуру не паритьсяч с выравниванием
# define BUFFER_OUT_42SH			0x1000//Размер буферв для вывода команд и текста на терминал
# define BUFFER_ERR_42SH			0x100//Размер буферв для вывода сообщений о ошибках
# define BUFFER_LITTER_42SH			0x20//Буфер получающий ввод пользователя через read - управляющие клавиши отдельные буквы и также текст для вставок

# define FILE_PASSWD_42SH			"/etc/passwd"//Путь где храняться логины - имя каждого из юзеров;уникальный идентификационный номер;пароль ИД;ИД группы;название группы;оболочка домашнего каталога;номер домашнего каталога.
# define FILE_PASSWD_MIN_42SH		0x0//Минимальный размер
# define FILE_PASSWD_MAX_42SH		0x100000//Максимальный размер

# define STATUS_OK_42SH				0x0
# define STATUS_FALTURE_42SH		0x1
# define STATUS_FD_CLOSE_42SH		0x2
# define STATUS_TEST_ERR_42SH		0x2
# define STATUS_DEFINED_CMD_42SH	0x7E
# define STATUS_NOT_FIN_CMD_42SH	0x7F
# define STATUS_JOBS_STOP_42SH		0x92
# define STATUS_REZERVED_42SH		-1

# define DQ_HRDC_42SH				0x1

# define SHIELD_EXTERNALLY			0x1//Из внешнего источника и перед '\n' нет некаких лишних пробелов
# define SHIELD_INTERIOR			0x2//Указывает что перед '\n' стоит пробел который надо пропустить

# define SCRIPT_MAX_SIZE_42SH		0x10000

# define LOCATION_STANDART_42SH		0x1//Находимся в шелле LOCATION_STANDART_42SH и LOCATION_SCRIPT_42SH - основной тип только один из них может быть
# define LOCATION_SCRIPT_42SH		0x2//Находимся в Script
# define LOCATION_FORK_42SH			0x4//Находимся в Fork
# define LOCATION_NOT_DSP_ERR_42SH	0x8//Не обрабатываем ошибки ни как кроме возвращаем в ретурн
# define LOCATION_NOT_SET_42SH		0x10//Не изменяет никакие переменные среды

# define CRC32_DEPTH_42SH			0x1
# define CRC32_POLINOM_MIRROR_42SH	0x82F63B78
# define CRC32_START_MIRROR_42SH	0xFFFFFFFF

# define AUTO_TYPE_SET_42SH			0x0//Если только он установлен проверяес черз равно тогда только имеет смысл добавлять переменные среды и наче только дляпрограмм в fork
# define AUTO_TYPE_FUN_42SH			0x1
# define AUTO_TYPE_EXE_42SH			0x2
# define AUTO_TYPE_ALIAS_42SH		0x4
# define AUTO_TYPE_RUN_42SH			0x8//Запускать в асинхронам состоянии
# define AUTO_TYPE_SET_RUN_42SH		0x8//(0x0 | 0x8)//Когда запускаеться в ассинхроном установка локальных переменных
# define AUTO_TYPE_HASH_42SH		0x10
# define AUTO_TYPE_ERROR_42SH		0x20

# define JOBS_STOP_42SH				0x1
# define JOBS_MSG_42SH				0x2
# define JOBS_SUSPEND_42SH			0x4

# define JOBS_HRDC_42SH				0x1//Если в pipe pipe->lp_heredok_b равно JOBS_HRDC_42SH тогда в lp_heredok_spl указатель который нужно освободить
# define JOBS_HRDC_EMPTY_42SH		0x2//Говорит, что ничего освобождать не надо и нечего делать тоже ненадо
# define JOBS_HRDC_DQ_42SH			0x4//Говорит, что ничего освобождать не надо и были кавычки нужно доп обработка минимальная для удаления лишних пробелов

# define DQ_HRDC_42SH				0x1//Если установленна значит во время тестинга имя содержала кавычку
# define DQ_HRDC_ERR_42SH			0x2//Если установленна значит во время тестинга нашли синтаксическую ошибку

# define CD_LOGIC_42SH				0x1//Логически парсим
# define CD_PHYSICAL_42SH			0x2//Физически парсим
# define CD_VIEW_42SH				0x4//Вывидим результат работы на экран
# define CD_CANON_42SH				0x8//Нужно значит в случае логического канонизировать прежде чем записать

# define HASH_NOT_42SH				0x0
# define HASH_ALL_42SH				0x1
# define HASH_ONLY_42SH				0x2

# define EXIT_ANYWAY_42SH			0x1//Значит все арвно выходим

# define JOBS_MSG_PROC_42SH			0x1//Отображает имя оболочки
# define JOBS_MSG_ID_42SH			0x2//| JOBS_MSG_PLUS_42SH | JOBS_MSG_MINUS_42SH //Отображает id
# define JOBS_MSG_PLUS_42SH			0x4//Ищет текущий
# define JOBS_MSG_MINUS_42SH		0x8//Ищет предтекущий
# define JOBS_MSG_ID_PID_42SH		0x10//Отображает PID
# define JOBS_MSG_ID_PID_ALL_42SH	0x20//Отображает все PID
# define JOBS_MSG_ID_PID_GRUP_42SH	0x40//Отображает PID пака jobs
# define JOBS_MSG_JOBS_42SH			0x80//Отобразит подсказку jobs и отобразит указаный jobs 
# define JOBS_MSG_FG_42SH			0x100//Отобразит подсказку fg
# define JOBS_MSG_BG_42SH			0x200//Отобразит подсказку bg
# define JOBS_MSG_ID_RUN_42SH		0x400//Отображает id в строчку ввсех запущеных jobs в паке
# define JOBS_MSG_CONTINUED_42SH	0x800//Для приостановленных показывать надпись возобновили

# define PARSING_FALTURE_42SH		0xFF

# define PARSING_MODE_CMD_42SH		0x1//Перевод парсинга как для команд // PARSING_MODE_CMD_42SH PARSING_MODE_ARG_42SH
# define PARSING_MODE_ARG_42SH		0x2//Перевод парсинга как для аргументов
# define PARSING_MODE_HRDC_42SH		0x8//Позволяет игнорировать расширение переменных среды и расширение арифметики в случае с именем хередока//В случае стелом все кроме расширение переменных среды и расширение арифметики превращает в обычные буквы
# define PARSING_MODE_ERR_42SH		0x10//Говорит что надо сохранять спец ошибки ошибки
# define PARSING_MODE_TILD_42SH		0x20//Говорит что тильду не обрабатываем используеться в ft_42sh_replase и ft_42sh_replase_count

# define EXP_PATH_42SH				"PATH"//Для универсальности переменная среды с путями к исполняемым файлам
# define EXP_HOME_42SH				"HOME"//Для универсальности переменная среды домашний каталог
# define EXP_PWD_42SH				"PWD"//Для универсальности переменная текущий каталог
# define EXP_OLDPWD_42SH			"OLDPWD"//Для универсальности переменная предыдущий текущий каталог
# define EXP_CDPATH_42SH			"CDPATH"//Какие перебирать каталоги в cd если путь указан без предшетствуещего '/'
# define EXP_HOME_PATH_42SH			"/Users/"//Путь к домашней дериктории без имени пользователя но с слешам
# define EXP_PS2_42SH				"PS2"//Сожержит преветсвенное сообщение для хередока

# define EXP_LENG_42SH				0x1

# define EXP_TYPE_EVERIMENT_42SH	0x1//Основной тип
# define EXP_TYPE_LOCAL_42SH		0x2//Основной тип
# define EXP_TYPE_R_ONLY_42SH		0x4//Дополнительный тип
# define EXP_TYPE_NUMBER_42SH		0x8//Дополнительный тип
# define EXP_TYPE_CORECTION_42SH	0x10//Указывает нужно ли добавлять перед выводом на экран кавычки//Дополнительный тип

# define PIPE_READ_42SH				0//Смещение для fds[2] что бы не путаться
# define PIPE_WRITE_42SH			1//Смещение для fds[2] что бы не путаться

# define FD_MAX_STANDART_42SH		2//Максимальный стандартный fd
# define FD_MAX_SUPPORT_42SH		9//Максимальнй поддерживаемый по POSIX
# define FD_TERMINAL_42SH			10//В данном дескрипторе всегда в шелле терминал

# define MODE_SIGCHILD_42SH		0x1//Включает слеежени за дочерними через сигнал
# define MODE_GUARD_42SH		0x2//Защищает от всех паддений
# define MODE_VALG_42SH			0x4//Защиищает от валгринда
# define MODE_LAUNCH_42SH		0x8//Защиищает от запуска на другой машине
# define MODE_TASKMASTER_42SH	0x10//Добавляет режим работы таскмастера в шелл
# define MODE_DEBUG_42SH		0x20//Если в режиме отладки то учитываем это что vs code мог работать

# define MODE_GUARD_COUNT_42SH		0xA//Счетчик Когда уже можно выводить поскалку
# define MODE_GUARD_COUNT_MAX_42SH	0x14//Счетчик максимального повтора для запска при срабатывании защиты

# define ID_LAUNCH_42SH			0x01000000//Id винчестера к которому привяжемся MODE_LAUNCH_42SH

# define PIPE_CLOSE_42SH		0x1
# define PIPE_AND_42SH			0x2
# define PIPE_42SH				0x4
# define PIPE_LEFT_42SH			0x8
# define PIPE_LEFT2_42SH		0x18//(0x10 | 0x8)
# define PIPE_RIGHT_42SH		0x20
# define PIPE_RIGHT2_42SH		0x60//(0x40 | 0x20)
# define PIPE_ADD_ERR_42SH		0x80
# define PIPE_REPLASE_42SH		0x100//Значит уже заменен и заменять далее нету потребности

# define PIPE_FIRST_42SH		0x1
# define PIPE_CENTRAL_42SH		0x2
# define PIPE_LAST_42SH			0x4

# define E_CODE_42SH		0
# define E_MEM_CODE_42SH	1//Ошибки с памятью
# define E_MEM_TXT_42SH		": Not enough space.\n"
# define E_READ_CODE_42SH	2//ошибки считывания с файла
# define E_READ_TXT_42SH	": The error read attempt on a socket.\n"
# define E_EXE_CODE_42SH	3//ошибки запуска файла на исполнения
# define E_EXE_TXT_42SH		": Could not start file for execution.\n"
# define E_PIPE_CODE_42SH	4//ошибки pipe
# define E_PIPE_TXT_42SH	": Could not start pipe.\n"
# define E_CD_CODE_42SH		5//ошибки путей
# define E_CD_TXT_42SH		": Failed to change directory.\n"
# define E_IOTL_CODE_42SH	6//ошибки iotl
# define E_IOTL_TXT_42SH	": Failed to iotl.\n"
# define E_DUP_CODE_42SH	7//ошибки dup
# define E_DUP_TXT_42SH	": Failed to dup.\n"

# define E_FOOR_CODE_42SH	54//Превышен лимит на процессы
# define E_CANNON_CODE_42SH	55
# define E_CANON_TXT_42SH	": Error non-canonical mode of operation.\n"

# define MSG_42SH					"$> "//Стартовое сообщение после пути к текущей папке
# define MSG_SP_42SH				"   "//Для вывода сообщений не ведущих к выходу из программы
# define PRE_MSG_COUNT_42SH			3//Размер в байтах стартового сообщения
# define PRE_MSG_LITTER_42SH		3//Размер в буквах стартового сообщения
# define MSG_DQUOTE_42SH			"dquote> "
# define PRE_MSG_DQUOTE_LITTER_42SH	8//Размер в буквах сообщения
# define MSG_QUOTE_42SH				"quote> "
# define PRE_MSG_QUOTE_LITTER_42SH	7//Размер в буквах сообщения
# define MSG_HEREDOC_42SH			"heredoc> "
# define PRE_MSG_HEREDOC_LITTER_42SH 9//Размер в буквах сообщения
# define MSG_SUB_42SH				"sub> "
# define PRE_MSG_SUB_LITTER_42SH	5//Размер в буквах сообщения
# define MSG_CMD_42SH				"cmd> "
# define PRE_MSG_CMD_LITTER_42SH	5//Размер в буквах сообщения

# define MSG_JOBS_PROC_42SH			"42sh"": "
# define MSG_JOBS_CONTINUED_42SH	"continued             : "
# define MSG_JOBS_DONE_42SH			"done                  : "
# define MSG_JOBS_RUN_42SH			"runing                : "
# define MSG_JOBS_EXIT_42SH			"exit               "
# define MSG_JOBS_EXIT_ADD_42SH		": "
# define MSG_JOBS_NULL_42SH			"user defined signal   : "//Для неизвестного сигнала
# define MSG_JOBS_HUP_42SH			"hangup                : "
# define MSG_JOBS_INT_42SH			"interrupt             : "
# define MSG_JOBS_QUIT_42SH			"quit                  : "
# define MSG_JOBS_ILL_42SH			"illegal instruction   : "
# define MSG_JOBS_TRAP_42SH			"trace trap            : "
# define MSG_JOBS_ABRT_42SH			"abort                 : "
# define MSG_JOBS_EMT_42SH			"EMT instruction       : "
# define MSG_JOBS_FPE_42SH			"floating exception    : "
# define MSG_JOBS_KILL_42SH			"killed                : "
# define MSG_JOBS_BUS_42SH			"bus error             : "
# define MSG_JOBS_SEGV_42SH			"segmentation fault    : "
# define MSG_JOBS_SYS_42SH			"invalid system call   : "
# define MSG_JOBS_PIPE_42SH			"broken pipe           : "
# define MSG_JOBS_ALRM_42SH			"alarm                 : "
# define MSG_JOBS_TERM_42SH			"terminated            : "
# define MSG_JOBS_GURD_42SH			"urgent condition      : "
# define MSG_JOBS_STOP_42SH			"suspended (signal)    : "
# define MSG_JOBS_STR_42SH			"suspended             : "
# define MSG_JOBS_CONT_42SH			"continue after stop   : "
# define MSG_JOBS_CHLD_42SH			"child status changed  : "
# define MSG_JOBS_TTIN_42SH			"suspended (tty input) : "
# define MSG_JOBS_TTON_42SH			"suspended (tty output): "
# define MSG_JOBS_GIO_42SH			"I/O is possible       : "
# define MSG_JOBS_XCPU_42SH			"cpu limit exceeded    : "
# define MSG_JOBS_XFSZ_42SH			"file size limit       : "
# define MSG_JOBS_TALR_42SH			"virtual time alarm    : "
# define MSG_JOBS_PROF_42SH			"profile signal        : "
# define MSG_JOBS_WINC_42SH			"window size change    : "
# define MSG_JOBS_INFO_42SH			"status from keyboard  : "
# define MSG_JOBS_USR1_42SH			"user defined signal 1 : "
# define MSG_JOBS_USR2_42SH			"user defined signal 2 : "

# define MSG_SH_TEST_42SH			"*.sh"
# define MSG_SH_TEST_TYPE_42SH		"*/42sh"
# define MSG_SH_DEFINED_42SH		": script permission denied."
# define MSG_SH_OPEN_FALLED_42SH	": script falled open."
# define MSG_SH_INVALID_SIZE_42SH	": script invalid size."
# define MSG_SH_INVALID_MEMORY_42SH	": script not enough space."
# define MSG_SH_INVALID_READ_42SH	": script invalid read."
# define MSG_SH_UNKNOWN_42SH		": script unknown."

# define MSG_STOP_42SH		"42sh"": can't set tty pgrp: interrupt\n"

# define MSG_EXP_QUEST_ERR_TXT_42SH	": "
# define MSG_EXP_QUEST_NOT_SET_TXT_42SH	" :parameter null or not set "

# define MSG_BAD_OPTION_TXT_42SH	": bad option: "

# define MSG_FOOR_TXT_42SH	": process limit exceeded."

# define MSG_EXE_DEFINE_42SH	": permission denied: "
# define MSG_EXE_NOT_CMD_42SH	": command not found: "
# define MSG_EXE_NOT_42SH 		": no such file or directory: "
# define MSG_IS_DIR_42SH 		": is a directory: "
# define MSG_PIPE_EQV_42SH		": file mode mismatch on fd: "
# define MSG_PIPE_MUL_FD_42SH	": multio failed, bad file descriptor fd: "
# define MSG_PIPE_BAD_FD_42SH	": bad file descriptor: "
# define MSG_PIPE_LIMIT_FD_42SH	": limit file descriptor"

# define MSG_EXPR_42SH		"export"
# define MSG_EXPR_VIEW_42SH	"export "
# define MSG_EXPR_SET_VIEW_42SH	"set "
# define MSG_EXPR_NONE_TXT_42SH "export"": Environment Variables - None."
# define MSG_EXPR_NOT_VALID_TXT_42SH "export"": not valid in this context: "
# define MSG_EXPR_NOT_FIND_TXT_42SH "export"": no such variable: "

# define MSG_EXP_SUB_BAD_42SH	": bad substitution: "
# define MSG_SINTAX_ERROR_42SH	": parse error near: "
# define MSG_CLOSING_ERROR_42SH	": closing brace expected"

# define MSG_JOBS_42SH		"jobs"
# define MSG_JOBS_NOT_TXT_42SH "jobs"": job not found: "
# define MSG_JOBS_NOT_CUR_TXT_42SH "jobs"": no current job"
# define MSG_JOBS_NOT_PREV_TXT_42SH "jobs"": no previous job"

# define MSG_JOBS_QUEST_42SH "42sh"": you have suspended jobs, exit?(y/n) \x7"
# define MSG_JOBS_QUEST_LEN_42SH 43 - 1//Сохраняем количество БУКВ // -1  на звук

# define MSG_FG_42SH		"fg"
# define MSG_FG_NOT_CONT_TXT_42SH "fg"": no job control in this shell."

# define MSG_CD_42SH		"cd"
# define MSG_CD_TOO_ARG_TXT_42SH "cd"": too many arguments"
# define MSG_CD_CHDIR_TXT_42SH "cd"": failed to change directory"
# define MSG_CD_NOT_VALID_TXT_42SH "cd"": no such file or directory: "
# define MSG_CD_FILE_TXT_42SH "cd"": not a directory: "
# define MSG_CD_DENIED_TXT_42SH "cd"": permission denied: "

# define MSG_TEST_42SH			"test"
# define MSG_TEST_TOO_ARG_TXT_42SH "test"": too many arguments"
# define MSG_TEST_UNK_COND_TXT_42SH "test"": unknown condition: "
# define MSG_TEST_COND_TXT_42SH	"test"": condition expected: "
# define MSG_TEST_INT_TXT_42SH	"test"": integer expression expected: "

# define MSG_HASH_42SH		"hash"
# define MSG_HASH_PRE_42SH	"hits       command\n"
# define HASH_HITS_LEN_42SH	19 - 8//LEN_(MSG_HASH_PRE_42SH) - LEN_("command\n")
# define MSG_HASH_TOO_ARG_TXT_42SH "hash"": too many arguments"
# define MSG_HASH_NOT_FIND_TXT_42SH "hash"": no such command:"

# define MSG_BG_42SH		"bg"
# define MSG_BG_NOT_CONT_TXT_42SH "bg"": no job control in this shell."
# define MSG_BG_ALREDY_TXT_42SH "bg"": job already in background"

# define MSG_EXIT_42SH		"exit"
# define MSG_EXIT_TOO_TXT_42SH	"exit"": too many arguments"
# define MSG_EXIT_NUM_TXT_42SH	"exit"": numeric argument required"

# define MSG_TYPE_42SH			"type"
# define MSG_TYPE_NOT_42SH		" not found"
# define MSG_TYPE_FUN_42SH		" is a shell builtin"
# define MSG_TYPE_EXE_42SH		" is "
# define MSG_TYPE_HASH_42SH		" is hashed to "
# define MSG_TYPE_ALIAS_42SH	" is an alias for "

# define MSG_SET_42SH		"set"
# define MSG_SET_NONE_TXT_42SH "set"": Environment Variables - None."

# define MSG_UNSET_42SH		"unset"
# define MSG_UNSET_R_ONLY_TXT_42SH "unset"": read-only variable: "
# define MSG_UNSET_NOT_ARG_TXT_42SH "unset"": not enough arguments"

# define MSG_TAB_LIMIT_42SH		"\n""do you wish to see all "//ИСПОЛЬЗУЕТЬСЯ ТО ЧТО ОДНА БУКВА РАВНА ОДНО БАЙТУ - в ином случае отследить и изменить
# define MSG_TAB_LIM_A_42SH		" possibilities? "//ИСПОЛЬЗУЕТЬСЯ ТО ЧТО ОДНА БУКВА РАВНА ОДНО БАЙТУ - в ином случае отследить и изменить
# define MSG_TAB_LIM_L_42SH		24 + 16 - 1//Сохраняем количество БУКВ //-1 что бы не учитывать "\n" LEN_(MSG_TAB_LIMIT_42SH)+LEN_(MSG_TAB_LIM_A_42SH)-1

# define KEY_UP_42SH 				0x415B1B//Стрелка в верх - перемещает каретку на одну стрку в верх если есть перевод строк или если есть история то загружает из истории
# define KEY_DOWN_42SH 				0x425B1B//Стрелка в низ - перемещает каретку на одну стрку в вниз если есть перевод строк или если есть история то загружает из истории
# define KEY_RIGHT_42SH 			0x435B1B//Стрелка вправо - перемещает каретку на одну символ впрвао
# define KEY_LEFT_42SH 				0x445B1B//Стрелка влево - перемещает каретку на одну символ влево
# define KEY_HOME_42SH 				0x1//ctr + A- перемещает каретку в самое начало всей строки
# define KEY_END_42SH 				0x5//ctr + E- перемещает каретку в самое конец всей строки
# define KEY_HOME_FULL_42SH 		0x485B1B//home- перемещает каретку в самое начало 
# define KEY_END_FULL_42SH 			0x465B1B//end- перемещает каретку в самое конец
# define KEY_DELETE_42SH			0x7E335B1B//Удаляет справо от коретки символ
# define KEY_BACKSPACE_42SH			0x7F//Удаляет слево от коретки символ
# define KEY_TAB_42SH				0x9//Автодаполнение команд путей и переменных сред
# define KEY_ENTER_42SH				0xA//Команду на выполнение
# define KEY_ENTER2_42SH			0xD//Команду на выполнение
# define KEY_EOF_42SH				0x4//ctr + D = прекращает текущую программу если ее не ттогда выходит с шелл
# define KEY_CLEAR_42SH				0xC//ctr + L = очищает весь экран
# define KEY_WORD_LEFT_42SH			0x2//ctr + F = сдвигает картетку на одно слово в лево
# define KEY_WORD_RIGHT_42SH		0x6//opt + B = сдвигает картетку на одно слово в право
# define KEY_SELECT_LEFT_42SH		0x44323B315B1B//shift + Left = выделяет текст в лево
# define KEY_SELECT_RIGHT_42SH		0x43323B315B1B//shift + Right = выделяет текст в право
# define KEY_STR_DELETE_42SH		0x15//ctr + U = если выделена строка то удаляет выделеную иначе удаляет всю строку
# define KEY_STR_COPY_42SH			0xB//ctr + K = если выделена строка то копирует выделеную иначе копирует всю строку
# define KEY_STR_CUT_42SH			0x18//ctr + X = если выделена строка то копирует выделеную иначе копирует всю строку
# define KEY_STR_PASTE_42SH			0x10//ctr + P = если выделена строка то копирует выделеную иначе копирует всю строку
# define KEY_INT_42SH				0x3//ctr + C Убиваем все запущенные в данный момент процессы или прерываем свои текущие действия

# define TAB_LIMIT_42SH				0x30

#endif
