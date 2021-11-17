if defined RC6_W
	RC6_P16						:= 0xB7E1
	RC6_Q16						:= 0x9E37
	RC6_P32						:= 0xB7E15163
	RC6_Q32						:= 0x9E3779B9
	RC6_P64						:= 0xB7E151628AED2A6B
	RC6_Q64						:= 0x9E3779B97F4A7C15

	RC6_KEY_MAX					:= (0x100 / 0x8)
	RC6_R						:= (0x14)
	RC6_B						:= (0x10)
	RC6_T						:= (0x2 * RC6_R + 0x4)

	RC6_PRE_PROC				:= 0x38

	if RC6_W = 0x10
		RC6_WORD				:= word
		RC6_WORD_SIZE			:= 0x2
		RC6_WORD_MAX			:= 0xFFFF
		RC6_WORD_SHIFT			:= 0x1
		RC6_WORD_RAX			:= ax
		RC6_WORD_RCX			:= cx
		RC6_WORD_RDX			:= dx
		RC6_WORD_RBX			:= bx
		RC6_WORD_RBP			:= bp
		RC6_P					:= RC6_P16
		RC6_Q					:= RC6_Q16
		RC6_LG					:= 0x4
	else if RC6_W = 0x20
		RC6_WORD				equ dword
		RC6_WORD_SIZE			:= 0x4
		RC6_WORD_MAX			:= 0xFFFFFFFF
		RC6_WORD_SHIFT			:= 0x2
		RC6_WORD_RAX			:= eax
		RC6_WORD_RCX			:= ecx
		RC6_WORD_RDX			:= edx
		RC6_WORD_RBX			:= ebx
		RC6_WORD_RBP			:= ebp
		RC6_P					:= RC6_P32
		RC6_Q					:= RC6_Q32
		RC6_LG					:= 0x5
	else if RC6_W = 0x40
		RC6_WORD				:= qword
		RC6_WORD_SIZE			:= 0x8
		RC6_WORD_MAX			:= 0xFFFFFFFFFFFFFFFF
		RC6_WORD_SHIFT			:= 0x3
		RC6_WORD_RAX			:= rax
		RC6_WORD_RCX			:= rcx
		RC6_WORD_RDX			:= rdx
		RC6_WORD_RBX			:= rbx
		RC6_WORD_RBP			:= rbp
		RC6_P					:= RC6_P64
		RC6_Q					:= RC6_Q64
		RC6_LG					:= 0x6
	else
		err 'RC6_W'
	end if

	macro rol_M name*, val*, n*
		local				number
		local				shift

		number = val and RC6_WORD_MAX
		shift = n mod RC6_W
		name = (((number) shl shift) or ((number) shr (RC6_W - shift))) and RC6_WORD_MAX
	end macro

	macro ror_M name*, val*, n*
		local				number
		local				shift

		number = val and RC6_WORD_MAX
		shift = n mod RC6_W
		name = (((number) shr shift) or ((number) shl (RC6_W - shift))) and RC6_WORD_MAX
	end macro

	macro rc6_init_M key*, len*
		local						x;
		local						y;
		local						k;
		local						n;
		local						i;
		local						j;
		local						c;
		local						s;
		local						tempos;

		if len > RC6_KEY_MAX
			len = RC6_KEY_MAX
		end if
		virtual at 0
			L::
			db key
			db RC6_KEY_MAX - len dup (0x0)
		end virtual
		if len = 0
			c = 1;
		else
			c = ((len + (RC6_WORD_SIZE - 1)) and (0 - RC6_WORD_SIZE)) / RC6_WORD_SIZE;
		end if
		s = $
		db RC6_T * RC6_WORD_SIZE dup (0x0)
		store RC6_P : RC6_WORD at s
		i = 1;
		while i <= (RC6_T - 1)
			load tempos RC6_WORD from s + ((i - 1) * RC6_WORD_SIZE)
			store ((tempos + RC6_Q) and RC6_WORD_MAX) : RC6_WORD at s + (i * RC6_WORD_SIZE)
			i = i + 1
		end while
		x = 0;
		y = 0;
		i = 0;
		j = 0;
		k = 0;
		if RC6_T > c
			n = RC6_T;
		else
			n = c;
		end if
		n = n * 3;
		while k < n
			load tempos RC6_WORD from s + (i * RC6_WORD_SIZE)
			rol_M x, (tempos + x + y), 0x3
			store x: RC6_WORD at s + (i * RC6_WORD_SIZE)
			load tempos RC6_WORD from L: 0x0 + (j * RC6_WORD_SIZE)
			rol_M y, (tempos + x + y), (x + y)
			store y: RC6_WORD at L: 0x0 + (j * RC6_WORD_SIZE)
			i = (i + 1) mod RC6_T;
			j = (j + 1) mod c;
			k = k + 1;
		end while
	end macro
	macro rc6_encrypt_M s*, src*, dst*, num_block*
		local						a;
		local						b;
		local						c;
		local						d;
		local						t;
		local						x;
		local						u;
		local						i;
		local						block;
		local						tempos;
		local						src_tmp;
		local						dst_tmp;

		block = num_block
		src_tmp = src
		dst_tmp = dst
		while block <> 0
			load a RC6_WORD from src_tmp + (0x0 * RC6_WORD_SIZE)
			load tempos RC6_WORD from s + (0x0 * RC6_WORD_SIZE)
			load b RC6_WORD from src_tmp + (0x1 * RC6_WORD_SIZE)
			b = (b + tempos) and RC6_WORD_MAX
			load c RC6_WORD from src_tmp + (0x2 * RC6_WORD_SIZE)
			load tempos RC6_WORD from s + (0x1 * RC6_WORD_SIZE)
			load d RC6_WORD from src_tmp + (0x3 * RC6_WORD_SIZE)
			d = (d + tempos) and RC6_WORD_MAX
			i = 2
			while i <= (2 * RC6_R)
				rol_M t, (b * (2 * b + 1)), RC6_LG
				rol_M u, (d * (2 * d + 1)), RC6_LG
				load tempos RC6_WORD from s + (i * RC6_WORD_SIZE)
				rol_M a, (a xor t), u
				a = (a + tempos) and RC6_WORD_MAX
				load tempos RC6_WORD from s + ((i + 1) * RC6_WORD_SIZE)
				rol_M c, (c xor u), t
				c = (c + tempos) and RC6_WORD_MAX
				x = a;
				a = b;
				b = c;
				c = d;
				d = x;
				i = i + 2
			end while
			load tempos RC6_WORD from s + ((0x2 * RC6_R + 0x2) * RC6_WORD_SIZE)
			tempos = (a + tempos) and RC6_WORD_MAX
			store tempos: RC6_WORD at dst_tmp + (0x0 * RC6_WORD_SIZE)
			store b: RC6_WORD at dst_tmp + (0x1 * RC6_WORD_SIZE)
			load tempos RC6_WORD from s + ((0x2 * RC6_R + 0x3) * RC6_WORD_SIZE)
			tempos = (c + tempos) and RC6_WORD_MAX
			store tempos: RC6_WORD at dst_tmp + (0x2 * RC6_WORD_SIZE)
			store d: RC6_WORD at dst_tmp + (0x3 * RC6_WORD_SIZE)
			block = block - 1
			src_tmp = src_tmp + (0x4 * RC6_WORD_SIZE)
			dst_tmp = dst_tmp + (0x4 * RC6_WORD_SIZE)
		end while
	end macro
	macro rc6_decrypt_M s*, src*, dst*, num_block*
		local						a;
		local						b;
		local						c;
		local						d;
		local						t;
		local						x;
		local						u;
		local						i;
		local						block;
		local						tempos;
		local						src_tmp;
		local						dst_tmp;

		block = num_block
		src_tmp = src
		dst_tmp = dst
		while block <> 0
			load a RC6_WORD from src_tmp + (0x0 * RC6_WORD_SIZE)
			load tempos RC6_WORD from s + ((0x2 * RC6_R + 0x2) * RC6_WORD_SIZE)
			a = a - tempos
			load b RC6_WORD from src_tmp + (0x1 * RC6_WORD_SIZE)
			load c RC6_WORD from src_tmp + (0x2 * RC6_WORD_SIZE)
			load tempos RC6_WORD from s + ((0x2 * RC6_R + 0x3) * RC6_WORD_SIZE)
			load d RC6_WORD from src_tmp + (0x3 * RC6_WORD_SIZE)
			c = c - tempos
			i = (2 * RC6_R);
			while i >= 2
				x = d;
				d = c;
				c = b;
				b = a;
				a = x;
				rol_M u, (d * (2 * d + 1)), RC6_LG
				rol_M t, (b * (2 * b + 1)), RC6_LG
				load tempos RC6_WORD from s + ((i + 1) * RC6_WORD_SIZE)
				ror_M c, (c - tempos), t
				c = c xor u
				load tempos RC6_WORD from s + (i * RC6_WORD_SIZE)
				ror_M a, (a - tempos), u
				a = a xor t
				i = i - 2
			end while
			store a: RC6_WORD at dst_tmp + (0x0 * RC6_WORD_SIZE)
			load tempos RC6_WORD from s + (0x0 * RC6_WORD_SIZE)
			tempos = b - tempos
			store tempos: RC6_WORD at dst_tmp + (0x1 * RC6_WORD_SIZE)
			store c: RC6_WORD at dst_tmp + (0x2 * RC6_WORD_SIZE)
			load tempos RC6_WORD from s + (0x1 * RC6_WORD_SIZE)
			tempos = d - tempos
			store tempos: RC6_WORD at dst_tmp + (0x3 * RC6_WORD_SIZE)
			block = block - 1
			src_tmp = src_tmp + (0x4 * RC6_WORD_SIZE)
			dst_tmp = dst_tmp + (0x4 * RC6_WORD_SIZE)
		end while
	end macro
end if