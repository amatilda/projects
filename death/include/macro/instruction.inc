macro push reglist*&
	iterate reg, reglist
		push reg
	end iterate
end macro


macro pop reglist*&
	iterate reg, reglist
		indx %%-%+1
		pop reg
	end iterate
end macro


; r9    ; 6th param
; r8    ; 5th param
; r10   ; 4th param
; rdx   ; 3rd param
; rsi   ; 2nd param
; rdi   ; 1st param
; eax   ; syscall_number (регистры RCX и R11 будут уничтожены)
; syscall
macro syscall args&
	local				value
	r1 equ rax
	rd1 equ eax

	r2 equ rdi
	rd2 equ edi

	r3 equ rsi
	rd3 equ esi

	r4 equ rdx
	rd4 equ edx

	r5 equ r10
	rd5 equ r10d

	r6 equ r8
	rd6 equ r8d

	r7 equ r9
	rd7 equ r9d

	iterate arg, args
		match =qword [val], arg
			type = 'qword'
			redefine value val
		else match =dword [val], arg
			type = 'dword'
			redefine value val
		else match =word [val], arg
			type = 'word'
			redefine value val
		else match =byte [val], arg
			type = 'byte'
			redefine value val
		else
			type = 'rest'
			redefine value arg
			x86.parse_operand @src, arg
		end match
		if type = 'rest'
			if @src.type = 'imm'
				if value = (value and 0xFFFFFFFF)
					mov r#%, value
				else if value = 0x0
					xor rd#%, rd#%
				else
					mov rd#%, value
				end if
			else if @src.type = 'reg'
				if ~(value eq r#% | value eq rd#%)
					if @src.size = 0x8
						mov r#%, value
					else if @src.size = 0x4
						mov rd#%, value
					else
						movzx rd#%, value
					end if
				end if
			else if @src.type = 'mem'
				lea r#%, value
			else
				err @src.type, `arg
			end if
		else if type = 'qword'
			mov r#%, qword[value]
		else if type = 'dword'
			mov rd#%, dword[value]
		else if type = 'word'
			movzx rd#%, word[value]
		else if type = 'byte'
			movzx rd#%, byte[value]
		else
			err "Type invalid"
		end if
		if %% > 7
			err 'Invalid argument count max',`arg
		end if 
	end iterate
	syscall
end macro
