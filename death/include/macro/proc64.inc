macro cdecl name*, args&
	fastcall name, args
end macro

define fastcall fastcall

fastcall.frame = -1

macro frame
	local size
	define fastcall.frame_size size
	fastcall.frame =: 0
	if size <> 0x0
		sub rsp,size
	end if
end macro

macro end.frame
	match size, fastcall.frame_size
		size := fastcall.frame
		if size <> 0x0
			add rsp,size
		end if
	end match
	restore fastcall.frame,fastcall.frame_size
end macro

; r9    ; 6th param
; r8    ; 5th param
; rcx   ; 4th param
; rdx   ; 3rd param
; rsi   ; 2nd param
; rdi   ; 1st param
macro fastcall name*, args&
	r1					equ rdi
	rd1					equ edi

	r2					equ rsi
	rd2					equ esi

	r3					equ rdx
	rd3					equ edx

	r4					equ rcx
	rd4					equ ecx

	r5					equ r8
	rd5					equ r8d

	r6					equ r9
	rd6					equ r9d

	local				value, offset, framesize

	if framesize & fastcall.frame < 0
		sub rsp,framesize
	end if
	offset = 0
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
		if %% >= 7
			err 'Invalid argument count max',`arg
		end if 
		offset = offset + 8
	end iterate
	framesize = (offset + 0xF) and 0xFFFFFFFFFFFFFFF0
	call name
	if framesize & fastcall.frame < 0
		add rsp, framesize
	else if fastcall.frame >= 0 & framesize > fastcall.frame
		fastcall.frame = framesize
	end if
end macro

define pcountsuffix %

prologue@proc equ prologuedef

macro prologuedef procname,flag,parmbytes,localbytes,reglist
	local loc, fill, size, size_reg

	define fastcall.frame_size size
	fastcall.frame =: 0
	loc = (localbytes+15) and (not 15)
	parmbase@proc equ rsp + 16 + size_reg + fill + loc + (size + 0x8)
	localbase@proc equ rsp + (size + 0x8)
	match any, reglist
		iterate reg, reglist
			push reg
			if % = %%
				size_reg := 0x8 * %
				fill := 8*(% and 1)
			end if
		end iterate
	else
		size_reg := 0
		fill := 0
	end match
	sub rsp,loc + fill + (size + 0x8)
end macro

epilogue@proc equ epiloguedef

macro epiloguedef procname,flag,parmbytes,localbytes,reglist
	local loc, fill, size

	add rsp,loc + fill + (size + 0x8)
	loc = (localbytes+15) and (not 15)
	match any, fastcall.frame_size
		any := fastcall.frame
		size := any
	end match
	restore fastcall.frame,fastcall.frame_size
	match any, reglist
		iterate reg, reglist
			indx %%-%+1
			pop reg
			if % = %%
				fill := 8*(% and 1)
			end if
		end iterate
	else
		fill := 0
	end match
	retn
end macro

close@proc equ

macro proc? statement&

	local _local,params,flag,regs,parmbytes,localbytes,current,tmp,initlocal

	macro endp?!

			localbytes = current

			purge ret?,locals?,endl?,proclocal?

			match close:reglist, close@proc,<regs>
				close name,flag,parmbytes,localbytes,reglist
			end match

		end match
		end namespace
		end if
		end match

		purge endp?

	end macro

	match name declaration, statement :
	if used name
	name:
	namespace name
	outscope match local?, proclocal

		match =stdcall? args :, declaration
			define params args
			flag = 11b
		else match =stdcall? :, declaration
			define params
			flag = 11b
		else match =c? args :, declaration
			define params args
			flag = 10001b
		else match =c? :, declaration
			define params
			flag = 10001b
		else match args :, declaration
			define params args
			flag = 0
		else
			define params
			flag = 0
		end match
		define regs
		match =uses? list, params
			define params list
			while 1
				match =, tail, params
					define params tail
					break
				else match reg tail, params&
					match more&, tail
						define params more
					else
						define params
					end match
					if % = 1
						regs equ reg
					else
						regs equ regs,reg
					end if
				else
					break
				end match
			end while
		else match =, tail, params
			define params tail
		end match

		match prologue:reglist, prologue@proc:<regs>
			prologue name,flags,parmbytes,localbytes,reglist
		end match

		virtual at parmbase@proc
			namespace name
				match args, params
					iterate arg, args
						match argname:type, arg
							label argname:type
							rb type
						else
							?arg dq ?
						end match
					end iterate
				end match
				parmbytes := $-(parmbase@proc)
				match p, pcountsuffix
					name#p = parmbytes/8
				end match
			end namespace
		end virtual

		macro ret? operand
			match any, operand
				retn operand
			else
				match epilogue:reglist, epilogue@proc:<regs>
					epilogue name,flag,parmbytes,localbytes,reglist
				end match
			end match
		end macro

		current = 0

		macro initlocal
			local area,pointer,length,value
			area::
			pointer = localbase@proc+current
			length = $@ - (localbase@proc) - current
			current = $ - (localbase@proc)
			end virtual
			while length > 0
				if length < 2
					load value:byte from area:pointer
					mov byte [pointer],value
					pointer = pointer + 1
					length = length - 1
				else if length < 4
					load value:word from area:pointer
					mov word [pointer],value
					pointer = pointer + 2
					length = length - 2
				else if length < 8
					load value:dword from area:pointer
					mov dword [pointer],value
					pointer = pointer + 4
					length = length - 4
				else
					load value:qword from area:pointer
					if  value < 80000000h | value >= 1 shl 64 - 80000000h
						mov qword [pointer],value
						pointer = pointer + 8
						length = length - 8
					else
						mov dword [pointer],value and 0FFFFFFFFh
						pointer = pointer + 4
						length = length - 4
					end if
				end if
			end while
			virtual at localbase@proc+current
		end macro

		macro locals?
			macro align value*
				align.assume localbase@proc+current, value
			end macro
			virtual at localbase@proc+current
			iterate dword, dword,qword
				macro dword? value
					if value relativeto 0
						emit dword: value
					else
						initlocal
						local pointer
						pointer := $
						end virtual
						mov dword [pointer],value
						virtual at pointer+4
						current = $ - (localbase@proc)
					end if
				end macro
			end iterate
			macro ? line&
				line
				if $ > $@
					initlocal
				end if
			end macro
		end macro

		macro endl?
			purge ?, dword?,qword?
			purge align
			initlocal
			end virtual
		end macro

		macro proclocal? args&
			locals
				iterate arg, args
					match varname[count]:type, arg
						?varname dbx type:count dup ?
					else match varname:type, arg
						?varname dbx type, ?
					else match varname[count], arg
						?varname rq count
					else match varname type, arg
						?varname type
					else
						?arg dq ?
					end match
				end iterate
			endl
		end macro

end macro
