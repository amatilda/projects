struc size_t args:?&
	if x86.mode = 16
		. dw args
	else if x86.mode = 32
		. dd args
	else if x86.mode = 64
		. dq args
	else
		err "Error x86.mode"
	end if
end struc

macro size_t args:?&
	if x86.mode = 16
		dw args
	else if x86.mode = 32
		dd args
	else if x86.mode = 64
		dq args
	else
		err "Error x86.mode"
	end if
end macro

macro enum enclosing
	counter = 0
	macro item name
			name := counter
			counter = counter + 1
	end macro
	macro enclosing
			purge item,enclosing
	end macro
end macro

; Добавляет sizeof к текстовым сообщениям
struc (name) db args:?&
	name db args
	name.sizeof = $ - .
end struc

struc uint64_t args:?&
	align 0x8
	. dq args
end struc
struc uint32_t args:?&
	align 0x4
	. dd args
end struc
struc uint16_t args:?&
	align 0x2
	. dw args
end struc
struc uint8_t args:?&
	. db args
end struc
macro uint64_t args:?&
	align 0x8
	dq args
end macro
macro uint32_t args:?&
	align 0x4
	dd args
end macro
macro uint16_t args:?&
	align 0x2
	dw args
end macro
macro uint8_t args:?&
	db args
end macro

macro ends?!
	mvmacro struct?.collect, ?
	mvstruc struct?.collect, ?

	match name, struct?.pname

		virtual at 0
			namespace name
				name.__body
			end namespace
			sizeof.name := $
			label name: sizeof.name at 0
		end virtual

		calminstruction breakifndef? sym
				transform sym
				jyes ok
				asm break
			ok:
		end calminstruction

		local alignment, maxalignment
		maxalignment = 1
		while 1
			breakifndef name.__statement#%
			match label, name.__label#%
				if defined name.label
					local alignment
					alignment = 1
					if sizeof name.label > 0
						alignment = 1 shl (bsf sizeof name.label)
					end if
					match name type, name.__definition#% ; adjust for the change in the base macro
						if defined type.__alignment
							alignment = type.__alignment
						end if
					end match
					if alignment > maxalignment
						maxalignment = alignment
					end if
				end if
			end match
		end while
		name.__alignment := maxalignment

		local sname
		define sname name

		calminstruction (instance) name values&
			arrange values, =struct?.=instantiate instance, sname, values
			assemble values
		end calminstruction

		calminstruction name values&
			local ic
			init ic
			compute ic, ic + 1
			arrange values, =struct?.=instantiate sname#ic, sname, values
			assemble values
		end calminstruction

	end match
end macro

macro NumToStrM num,args
   local loc,mods,count,Error
   count = args
   loc	 = num
   if num < 0xA & args = 0xA
     display num + 0x30
    else if num < 0x10 & args = 0x10
     if num <= 0xF
       display 0x30
     end if
     if num >= 0xA
       display 0x41 + num - 0xA
      else
       display num + 0x30
     end if
    else if args = 0xA | args = 0x10
     repeat 64
      loc = loc / args
      if loc < args
	break
      end if
      count = count * args
     end repeat

     loc = num
     repeat 64
      mods = loc mod count
      loc  = loc / count
       if loc >= 0xA & args = 0x10
	 display 0x41 + loc - 0xA
	else
	 display loc + 0x30
       end if

      if count = 1
	break
      end if
      count = count / args
      loc   = mods
     end repeat
    else
     display 'NumToStrM,num,?',0xD,0xA
     error@
   end if
end macro

macro displayM text,bool

      display text,'0x'
      NumToStrM bool,0x10
      display ' '
      NumToStrM bool,0xA
      display  0xD,0xA
end macro

; Macroinstructions for making resource section
;----------------------------------------------
macro crc32_generate_table_M countTabl,val;val = полином для таблицы
	local loc,arg,locmini,temp,big
	big = 0

	while big < countTabl
	loc = 0; Расчитать значения для всех 256 байт
	big = big + 1
		while loc < 256;большой цикл для crc32 = 8 * 32
			locmini = 0
			arg = loc
			while  locmini < 8 * big; малый цикл
				temp = arg
				arg = arg shr 1
				if (temp and 1) <> 0; Проверить четность байта
					arg = arg xor val; XOR выполняется только если байт нечетный
				end if
				locmini = locmini + 1
			end while ; Следующий бит
			dd arg  ; Записать полученный dword в таблицу
			loc = loc + 1
		end while;Следующий байт
	end while
end macro

macro crc32_zlib_stream_M  name*, Source*, Length*, FirstStep*
	local x,y,z,count
	count = 0
	y = FirstStep

	virtual at 0
		table::
		crc32_generate_table_M 1,0xEDB88320
	end virtual

	repeat  Length
		load x byte from Source + count
		x = (x xor y) and 0xFF
		y = y shr 8
		load z dword from table:x*4
		y = y xor z
		count = count + 1
	end repeat
	name := not y
end macro


