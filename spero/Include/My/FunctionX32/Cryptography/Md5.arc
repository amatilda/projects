
;«ависимости: MoveMemoryAlingDwordHL,ZeroMemory32HL
;!InBuffer - указатель на данные(align 4) дл€ которых нужно подсчитать хеш
;!InBufferLength - размер подсчитываемых данных в byte
;!OutBuffer - указатель на буфер куда будет записан результат sizeof.!OutBuffer = sizeof.Md5HL_OutS
Md5HL_All	 = 0x0
Md5HL_Blok	 = 0x1
Md5HL_Final	 = 0x2

struct Md5HL_OutS
 Md5			       dd ?,?,?,?
 Size			       dd ?,?
ends

procM Md5HL [uses ebx esi edi ebp],\
	    !InBuffer,!InBufferLength,!OutBuffer,!Flag

	locals
	 ;------Ы
	 !outA				   dd ?
	 !outB				   dd ?
	 !outC				   dd ?
	 !outD				   dd ?
	 ;------Ы

	 !InAddCount			   dd ?
	 !InCount			   dd ?
	 !AddBuffer			   db aling[4] 128 dup (?)
	endl

	   ;phase I Ј padding
	   mov	eax,[!InBufferLength]
	   mov	ebx,[!InBuffer]

	   mov	ecx,eax
	   lea	edi,[!AddBuffer]
	   and	eax,-64
	   mov	ebp,64
	   lea	esi,[ebx+eax]
	   sub	ecx,eax
	    jz	@f

	   mov	edx,ebp
	   mov	eax,ebp
	   sub	edx,ecx
	   add	eax,ecx

	   cmp	ecx,55
	 cmova	ebp,eax
	cmovbe	ebp,edx
	  call	MoveMemoryAlingDwordHL;Destination = edi,Source = esi,countRepeat[byte] = ecx

	   @@:
	   mov	byte[edi],128
	   inc	edi
	   lea	ecx,[ebp-9]
	   sub	esi,ebx
	  call	ZeroMemory32HL;Destination = edi,countRepeat[byte] = ecx;Exit: eax = 0

	  shld	eax,esi,3
	   mov	ecx,esi
	   shl	esi,3
	   lea	edx,[!AddBuffer]
	   lea	ebp,[edi+8]
	   mov	[edi],esi
	   mov	[edi+4],eax

	   xor	eax,eax
	   sub	ebp,edx
	   and	ecx,-64
	 cmovz	ecx,ebp
	 cmovz	ebp,eax
	 cmovz	ebx,edx
	   mov	[!InAddCount],ebp
	   mov	[!InCount],ecx

	   ;phase II Ј chaining variables initialization
	   mov	[!outA],0x67452301
	   mov	[!outB],0xEFCDAB89
	   mov	[!outC],0x98BADCFE
	   mov	[!outD],0x10325476


	   ;phase III Ј hashing
    .HashLoop:
	   mov	edx,[!outA];dta
	   mov	ebp,[!outB];dtb
	   mov	esi,[!outC];dtc
	   mov	edi,[!outD];dtd

	   ;round 1
	   ;procM FF !a,!b,!c,!d,!x,!s,!t
	   ;a = b + ((a + F(b,c,d) + x + t) << s );<< = rol
	   ;F(b,c,d) = (b and c) or ((not b) and d)
	   ;F(b,c,d) = (( (c xor d) and b) xor d)
	   ;-------------------------------------
       ;invoke  FF,dta,dtb,dtc,dtd,[ebx+0*4],7,0xD76AA478
	   mov	eax,edx;a
	   mov	ecx,esi;c

	   add	eax,0xD76AA478
	   xor	ecx,edi

	   add	eax,[ebx+0*4]
	   and	ecx,ebp

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,7
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  FF,dtd,dta,dtb,dtc,[ebx+1*4],12,0xE8C7b756
	   mov	eax,edi;a
	   mov	ecx,ebp;c

	   add	eax,0xE8C7b756
	   xor	ecx,esi

	   add	eax,[ebx+1*4]
	   and	ecx,edx

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,12
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  FF,dtc,dtd,dta,dtb,[ebx+2*4],17,0x242070DB
	   mov	eax,esi;a
	   mov	ecx,edx;c

	   add	eax,0x242070DB
	   xor	ecx,ebp

	   add	eax,[ebx+2*4]
	   and	ecx,edi

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,17
	   lea	esi,[eax+edi];dtc
	 ; mov  dtc,eax

       ;invoke  FF,dtb,dtc,dtd,dta,[ebx+3*4],22,0xC1BDCEEE
	   mov	eax,ebp;a
	   mov	ecx,edi;c

	   add	eax,0xC1BDCEEE
	   xor	ecx,edx

	   add	eax,[ebx+3*4]
	   and	ecx,esi

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,22
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  FF,dta,dtb,dtc,dtd,[ebx+4*4],7,0xF57C0FAF
	   mov	eax,edx;a
	   mov	ecx,esi;c

	   add	eax,0xF57C0FAF
	   xor	ecx,edi

	   add	eax,[ebx+4*4]
	   and	ecx,ebp

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,7
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  FF,dtd,dta,dtb,dtc,[ebx+5*4],12,0x4787C62A
	   mov	eax,edi;a
	   mov	ecx,ebp;c

	   add	eax,0x4787C62A
	   xor	ecx,esi

	   add	eax,[ebx+5*4]
	   and	ecx,edx

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,12
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  FF,dtc,dtd,dta,dtb,[ebx+6*4],17,0xA8304613
	   mov	eax,esi;a
	   mov	ecx,edx;c

	   add	eax,0xA8304613
	   xor	ecx,ebp

	   add	eax,[ebx+6*4]
	   and	ecx,edi

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,17
	   lea	esi,[eax+edi];dtc
	 ; mov  dtc,eax

       ;invoke  FF,dtb,dtc,dtd,dta,[ebx+7*4],22,0xFD469501
	   mov	eax,ebp;a
	   mov	ecx,edi;c

	   add	eax,0xFD469501
	   xor	ecx,edx

	   add	eax,[ebx+7*4]
	   and	ecx,esi

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,22
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  FF,dta,dtb,dtc,dtd,[ebx+8*4],7,0x698098D8
	   mov	eax,edx;a
	   mov	ecx,esi;c

	   add	eax,0x698098D8
	   xor	ecx,edi

	   add	eax,[ebx+8*4]
	   and	ecx,ebp

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,7
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  FF,dtd,dta,dtb,dtc,[ebx+9*4],12,0x8B44f7AF
	   mov	eax,edi;a
	   mov	ecx,ebp;c

	   add	eax,0x8B44f7AF
	   xor	ecx,esi

	   add	eax,[ebx+9*4]
	   and	ecx,edx

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,12
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  FF,dtc,dtd,dta,dtb,[ebx+10*4],17,0xFFFF5BB1
	   mov	eax,esi;a
	   mov	ecx,edx;c

	   add	eax,0xFFFF5BB1
	   xor	ecx,ebp

	   add	eax,[ebx+10*4]
	   and	ecx,edi

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,17
	   lea	esi,[eax+edi];dtc
	 ; mov  dtc,eax

       ;invoke  FF,dtb,dtc,dtd,dta,[ebx+11*4],22,0x895CD7BE
	   mov	eax,ebp;a
	   mov	ecx,edi;c

	   add	eax,0x895CD7BE
	   xor	ecx,edx

	   add	eax,[ebx+11*4]
	   and	ecx,esi

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,22
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  FF,dta,dtb,dtc,dtd,[ebx+12*4],7,0x6B901122
	   mov	eax,edx;a
	   mov	ecx,esi;c

	   add	eax,0x6B901122
	   xor	ecx,edi

	   add	eax,[ebx+12*4]
	   and	ecx,ebp

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,7
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  FF,dtd,dta,dtb,dtc,[ebx+13*4],12,0xFD987193
	   mov	eax,edi;a
	   mov	ecx,ebp;c

	   add	eax,0xFD987193
	   xor	ecx,esi

	   add	eax,[ebx+13*4]
	   and	ecx,edx

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,12
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  FF,dtc,dtd,dta,dtb,[ebx+14*4],17,0xA679438E
	   mov	eax,esi;a
	   mov	ecx,edx;c

	   add	eax,0xA679438E
	   xor	ecx,ebp

	   add	eax,[ebx+14*4]
	   and	ecx,edi

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,17
	   lea	esi,[eax+edi];dtc
	 ; mov  dtc,eax

       ;invoke  FF,dtb,dtc,dtd,dta,[ebx+15*4],22,0x49B40821
	   mov	eax,ebp;a
	   mov	ecx,edi;c

	   add	eax,0x49B40821
	   xor	ecx,edx

	   add	eax,[ebx+15*4]
	   and	ecx,esi

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,22
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------

	   ;round 2
	   ;procM GG !a,!b,!c,!d,!x,!s,!t
	   ;a = b + ((a + G(b,c,d) + x + t) << s);<< = rol
	   ;G(b,c,d) = (b and d) or (c and (not d))
	   ;G(b,c,d) = (( (b xor c) and d) xor c)
	   ;-------------------------------------
       ;invoke  GG,dta,dtb,dtc,dtd,[ebx+1*4],5,0xF61E2562
	   mov	eax,edx;a
	   mov	ecx,ebp;b

	   add	eax,0xF61E2562
	   xor	ecx,esi

	   add	eax,[ebx+1*4]
	   and	ecx,edi

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,5
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  GG,dtd,dta,dtb,dtc,[ebx+6*4],9,0xC040B340
	   mov	eax,edi;a
	   mov	ecx,edx;b

	   add	eax,0xC040B340
	   xor	ecx,ebp

	   add	eax,[ebx+6*4]
	   and	ecx,esi

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,9
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  GG,dtc,dtd,dta,dtb,[ebx+11*4],14,0x265E5A51
	   mov	eax,esi;a
	   mov	ecx,edi;b

	   add	eax,0x265E5A51
	   xor	ecx,edx

	   add	eax,[ebx+11*4]
	   and	ecx,ebp

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,14
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  GG,dtb,dtc,dtd,dta,[ebx+0*4],20,0xE9B6C7AA
	   mov	eax,ebp;a
	   mov	ecx,esi;b

	   add	eax,0xE9B6C7AA
	   xor	ecx,edi

	   add	eax,[ebx+0*4]
	   and	ecx,edx

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,20
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  GG,dta,dtb,dtc,dtd,[ebx+5*4],5,0xD62F105D
	   mov	eax,edx;a
	   mov	ecx,ebp;b

	   add	eax,0xD62F105D
	   xor	ecx,esi

	   add	eax,[ebx+5*4]
	   and	ecx,edi

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,5
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  GG,dtd,dta,dtb,dtc,[ebx+10*4],9,0x2441453
	   mov	eax,edi;a
	   mov	ecx,edx;b

	   add	eax,0x2441453
	   xor	ecx,ebp

	   add	eax,[ebx+10*4]
	   and	ecx,esi

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,9
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  GG,dtc,dtd,dta,dtb,[ebx+15*4],14,0xD8A1E681
	   mov	eax,esi;a
	   mov	ecx,edi;b

	   add	eax,0xD8A1E681
	   xor	ecx,edx

	   add	eax,[ebx+15*4]
	   and	ecx,ebp

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,14
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  GG,dtb,dtc,dtd,dta,[ebx+4*4],20,0xE7D3FBC8
	   mov	eax,ebp;a
	   mov	ecx,esi;b

	   add	eax,0xE7D3FBC8
	   xor	ecx,edi

	   add	eax,[ebx+4*4]
	   and	ecx,edx

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,20
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  GG,dta,dtb,dtc,dtd,[ebx+9*4],5,0x21E1CDE6
	   mov	eax,edx;a
	   mov	ecx,ebp;b

	   add	eax,0x21E1CDE6
	   xor	ecx,esi

	   add	eax,[ebx+9*4]
	   and	ecx,edi

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,5
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  GG,dtd,dta,dtb,dtc,[ebx+14*4],9,0xC33707D6
	   mov	eax,edi;a
	   mov	ecx,edx;b

	   add	eax,0xC33707D6
	   xor	ecx,ebp

	   add	eax,[ebx+14*4]
	   and	ecx,esi

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,9
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  GG,dtc,dtd,dta,dtb,[ebx+3*4],14,0xF4D50D87
	   mov	eax,esi;a
	   mov	ecx,edi;b

	   add	eax,0xF4D50D87
	   xor	ecx,edx

	   add	eax,[ebx+3*4]
	   and	ecx,ebp

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,14
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  GG,dtb,dtc,dtd,dta,[ebx+8*4],20,0x455A14ED
	   mov	eax,ebp;a
	   mov	ecx,esi;b

	   add	eax,0x455A14ED
	   xor	ecx,edi

	   add	eax,[ebx+8*4]
	   and	ecx,edx

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,20
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  GG,dta,dtb,dtc,dtd,[ebx+13*4],5,0xA9E3E905
	   mov	eax,edx;a
	   mov	ecx,ebp;b

	   add	eax,0xA9E3E905
	   xor	ecx,esi

	   add	eax,[ebx+13*4]
	   and	ecx,edi

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,5
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  GG,dtd,dta,dtb,dtc,[ebx+2*4],9,0xFCEFA3F8
	   mov	eax,edi;a
	   mov	ecx,edx;b

	   add	eax,0xFCEFA3F8
	   xor	ecx,ebp

	   add	eax,[ebx+2*4]
	   and	ecx,esi

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,9
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  GG,dtc,dtd,dta,dtb,[ebx+7*4],14,0x676F02D9
	   mov	eax,esi;a
	   mov	ecx,edi;b

	   add	eax,0x676F02D9
	   xor	ecx,edx

	   add	eax,[ebx+7*4]
	   and	ecx,ebp

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,14
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  GG,dtb,dtc,dtd,dta,[ebx+12*4],20,0x8D2A4C8A
	   mov	eax,ebp;a
	   mov	ecx,esi;b

	   add	eax,0x8D2A4C8A
	   xor	ecx,edi

	   add	eax,[ebx+12*4]
	   and	ecx,edx

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,20
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------

	   ;round 3
	   ;procM HH !a,!b,!c,!d,!x,!s,!t
	   ;a = b + ((a + H(b,c,d) + x + t) << s);<< = rol
	   ;H(b,c,d) = b xor c xor d
	   ;-------------------------------------
       ;invoke  HH,dta,dtb,dtc,dtd,[ebx+5*4],4,0xFFFA3942
	   mov	eax,edx;a
	   mov	ecx,ebp;b

	   add	eax,0xFFFA3942
	   xor	ecx,esi

	   add	eax,[ebx+5*4]
	   xor	ecx,edi

	   add	eax,ecx
	   rol	eax,4
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  HH,dtd,dta,dtb,dtc,[ebx+8*4],11,0x8771F681
	   mov	eax,edi;a
	   mov	ecx,edx;b

	   add	eax,0x8771F681
	   xor	ecx,ebp

	   add	eax,[ebx+8*4]
	   xor	ecx,esi

	   add	eax,ecx
	   rol	eax,11
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  HH,dtc,dtd,dta,dtb,[ebx+11*4],16,0x6D9D6122
	   mov	eax,esi;a
	   mov	ecx,edi;b

	   add	eax,0x6D9D6122
	   xor	ecx,edx

	   add	eax,[ebx+11*4]
	   xor	ecx,ebp

	   add	eax,ecx
	   rol	eax,16
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  HH,dtb,dtc,dtd,dta,[ebx+14*4],23,0xFDE5380C
	   mov	eax,ebp;a
	   mov	ecx,esi;b

	   add	eax,0xFDE5380C
	   xor	ecx,edi

	   add	eax,[ebx+14*4]
	   xor	ecx,edx

	   add	eax,ecx
	   rol	eax,23
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  HH,dta,dtb,dtc,dtd,[ebx+1*4],4,0xA4BEEA44
	   mov	eax,edx;a
	   mov	ecx,ebp;b

	   add	eax,0xA4BEEA44
	   xor	ecx,esi

	   add	eax,[ebx+1*4]
	   xor	ecx,edi

	   add	eax,ecx
	   rol	eax,4
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  HH,dtd,dta,dtb,dtc,[ebx+4*4],11,0x4BDECFA9
	   mov	eax,edi;a
	   mov	ecx,edx;b

	   add	eax,0x4BDECFA9
	   xor	ecx,ebp

	   add	eax,[ebx+4*4]
	   xor	ecx,esi

	   add	eax,ecx
	   rol	eax,11
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  HH,dtc,dtd,dta,dtb,[ebx+7*4],16,0xF6BB4B60
	   mov	eax,esi;a
	   mov	ecx,edi;b

	   add	eax,0xF6BB4B60
	   xor	ecx,edx

	   add	eax,[ebx+7*4]
	   xor	ecx,ebp

	   add	eax,ecx
	   rol	eax,16
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  HH,dtb,dtc,dtd,dta,[edi+10*4],23,0xBEBFBC70
	   mov	eax,ebp;a
	   mov	ecx,esi;b

	   add	eax,0xBEBFBC70
	   xor	ecx,edi

	   add	eax,[ebx+10*4]
	   xor	ecx,edx

	   add	eax,ecx
	   rol	eax,23
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  HH,dta,dtb,dtc,dtd,[ebx+13*4],4,0x289B7EC6
	   mov	eax,edx;a
	   mov	ecx,ebp;b

	   add	eax,0x289B7EC6
	   xor	ecx,esi

	   add	eax,[ebx+13*4]
	   xor	ecx,edi

	   add	eax,ecx
	   rol	eax,4
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  HH,dtd,dta,dtb,dtc,[ebx+0*4],11,0xEAA127FA
	   mov	eax,edi;a
	   mov	ecx,edx;b

	   add	eax,0xEAA127FA
	   xor	ecx,ebp

	   add	eax,[ebx+0*4]
	   xor	ecx,esi

	   add	eax,ecx
	   rol	eax,11
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  HH,dtc,dtd,dta,dtb,[ebx+3*4],16,0xD4EF3085
	   mov	eax,esi;a
	   mov	ecx,edi;b

	   add	eax,0xD4EF3085
	   xor	ecx,edx

	   add	eax,[ebx+3*4]
	   xor	ecx,ebp

	   add	eax,ecx
	   rol	eax,16
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  HH,dtb,dtc,dtd,dta,[ebx+6*4],23,0x4881D05
	   mov	eax,ebp;a
	   mov	ecx,esi;b

	   add	eax,0x4881D05
	   xor	ecx,edi

	   add	eax,[ebx+6*4]
	   xor	ecx,edx

	   add	eax,ecx
	   rol	eax,23
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  HH,dta,dtb,dtc,dtd,[ebx+9*4],4,0xD9D4D039
	   mov	eax,edx;a
	   mov	ecx,ebp;b

	   add	eax,0xD9D4D039
	   xor	ecx,esi

	   add	eax,[ebx+9*4]
	   xor	ecx,edi

	   add	eax,ecx
	   rol	eax,4
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  HH,dtd,dta,dtb,dtc,[ebx+12*4],11,0xE6DB99E5
	   mov	eax,edi;a
	   mov	ecx,edx;b

	   add	eax,0xE6DB99E5
	   xor	ecx,ebp

	   add	eax,[ebx+12*4]
	   xor	ecx,esi

	   add	eax,ecx
	   rol	eax,11
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  HH,dtc,dtd,dta,dtb,[ebx+15*4],16,0x1FA27CF8
	   mov	eax,esi;a
	   mov	ecx,edi;b

	   add	eax,0x1FA27CF8
	   xor	ecx,edx

	   add	eax,[ebx+15*4]
	   xor	ecx,ebp

	   add	eax,ecx
	   rol	eax,16
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  HH,dtb,dtc,dtd,dta,[ebx+2*4],23,0xC4AC5665
	   mov	eax,ebp;a
	   mov	ecx,esi;b

	   add	eax,0xC4AC5665
	   xor	ecx,edi

	   add	eax,[ebx+2*4]
	   xor	ecx,edx

	   add	eax,ecx
	   rol	eax,23
	   lea	ebp,[eax+esi];dtb
	  ;mov  dtb,eax
	  ;-------------

	   ;round 4
	   ;procM II !a,!b,!c,!d,!x,!s,!t
	   ;;a = b + ((a + I(b,c,d) + x + t) << s);<< = rol
	   ;;I(b,c,d) = c xor (b or (not d))
	   ;-------------------------------------
       ;invoke  II,dta,dtb,dtc,dtd,[ebx+0*4],6,0xF4292244
	   mov	eax,edx;a
	   mov	ecx,edi;d

	   add	eax,0xF4292244
	   not	ecx

	   add	eax,[ebx+0*4]
	    or	ecx,ebp

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,6
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  II,dtd,dta,dtb,dtc,[ebx+7*4],10,0x432AFF97
	   mov	eax,edi;a
	   mov	ecx,esi;d

	   add	eax,0x432AFF97
	   not	ecx

	   add	eax,[ebx+7*4]
	    or	ecx,edx

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,10
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  II,dtc,dtd,dta,dtb,[ebx+14*4],15,0xAB9423A7
	   mov	eax,esi;a
	   mov	ecx,ebp;d

	   add	eax,0xAB9423A7
	   not	ecx

	   add	eax,[ebx+14*4]
	    or	ecx,edi

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,15
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  II,dtb,dtc,dtd,dta,[ebx+5*4],21,0xFC93A039
	   mov	eax,ebp;a
	   mov	ecx,edx;d

	   add	eax,0xFC93A039
	   not	ecx

	   add	eax,[ebx+5*4]
	    or	ecx,esi

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,21
	   lea	ebp,[eax+esi];dtc
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  II,dta,dtb,dtc,dtd,[ebx+12*4],6,0x655B59C3
	   mov	eax,edx;a
	   mov	ecx,edi;d

	   add	eax,0x655B59C3
	   not	ecx

	   add	eax,[ebx+12*4]
	    or	ecx,ebp

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,6
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  II,dtd,dta,dtb,dtc,[ebx+3*4],10,0x8F0CCC92
	   mov	eax,edi;a
	   mov	ecx,esi;d

	   add	eax,0x8F0CCC92
	   not	ecx

	   add	eax,[ebx+3*4]
	    or	ecx,edx

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,10
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  II,dtc,dtd,dta,dtb,[ebx+10*4],15,0xFFEFF47D
	   mov	eax,esi;a
	   mov	ecx,ebp;d

	   add	eax,0xFFEFF47D
	   not	ecx

	   add	eax,[ebx+10*4]
	    or	ecx,edi

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,15
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  II,dtb,dtc,dtd,dta,[ebx+1*4],21,0x85845DD1
	   mov	eax,ebp;a
	   mov	ecx,edx;d

	   add	eax,0x85845DD1
	   not	ecx

	   add	eax,[ebx+1*4]
	    or	ecx,esi

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,21
	   lea	ebp,[eax+esi];dtc
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  II,dta,dtb,dtc,dtd,[ebx+8*4],6,0x6FA87E4F
	   mov	eax,edx;a
	   mov	ecx,edi;d

	   add	eax,0x6FA87E4F
	   not	ecx

	   add	eax,[ebx+8*4]
	    or	ecx,ebp

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,6
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  II,dtd,dta,dtb,dtc,[ebx+15*4],10,0xFE2CE6E0
	   mov	eax,edi;a
	   mov	ecx,esi;d

	   add	eax,0xFE2CE6E0
	   not	ecx

	   add	eax,[ebx+15*4]
	    or	ecx,edx

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,10
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  II,dtc,dtd,dta,dtb,[ebx+6*4],15,0xA3014314
	   mov	eax,esi;a
	   mov	ecx,ebp;d

	   add	eax,0xA3014314
	   not	ecx

	   add	eax,[ebx+6*4]
	    or	ecx,edi

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,15
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  II,dtb,dtc,dtd,dta,[ebx+13*4],21,0x4E0811A1
	   mov	eax,ebp;a
	   mov	ecx,edx;d

	   add	eax,0x4E0811A1
	   not	ecx

	   add	eax,[ebx+13*4]
	    or	ecx,esi

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,21
	   lea	ebp,[eax+esi];dtc
	  ;mov  dtb,eax
	  ;-------------
       ;invoke  II,dta,dtb,dtc,dtd,[ebx+4*4],6,0xF7537E82
	   mov	eax,edx;a
	   mov	ecx,edi;d

	   add	eax,0xF7537E82
	   not	ecx

	   add	eax,[ebx+4*4]
	    or	ecx,ebp

	   xor	ecx,esi
	   add	eax,ecx
	   rol	eax,6
	   lea	edx,[eax+ebp];dta
	  ;mov  dta,eax

       ;invoke  II,dtd,dta,dtb,dtc,[ebx+11*4],10,0xBD3AF235
	   mov	eax,edi;a
	   mov	ecx,esi;d

	   add	eax,0xBD3AF235
	   not	ecx

	   add	eax,[ebx+11*4]
	    or	ecx,edx

	   xor	ecx,ebp
	   add	eax,ecx
	   rol	eax,10
	   lea	edi,[eax+edx];dtd
	  ;mov  dtd,eax

       ;invoke  II,dtc,dtd,dta,dtb,[ebx+2*4],15,0x2AD7D2BB
	   mov	eax,esi;a
	   mov	ecx,ebp;d

	   add	eax,0x2AD7D2BB
	   not	ecx

	   add	eax,[ebx+2*4]
	    or	ecx,edi

	   xor	ecx,edx
	   add	eax,ecx
	   rol	eax,15
	   lea	esi,[eax+edi];dtc
	  ;mov  dtc,eax

       ;invoke  II,dtb,dtc,dtd,dta,[ebx+9*4],21,0xEB86D391
	   mov	eax,ebp;a
	   mov	ecx,edx;d

	   add	eax,0xEB86D391
	   not	ecx

	   add	eax,[ebx+9*4]
	    or	ecx,esi

	   xor	ecx,edi
	   add	eax,ecx
	   rol	eax,21
	   lea	ebp,[eax+esi];dtc
	  ;mov  dtb,eax
	  ;-------------


	   add	[!outA],edx;dta
	   add	[!outB],ebp;dtb
	   add	[!outC],esi;dtc
	   add	[!outD],edi;dtd

	   add	ebx,64
	   sub	[!InCount],64
	   jnz	.HashLoop

	   xor	eax,eax
	   lea	ecx,[!AddBuffer]
	    or	eax,[!InAddCount]
	    jz	@f

	   mov	ebx,ecx
	   xor	[!InAddCount],eax
	   mov	[!InCount],eax
	   jmp	.HashLoop

	   @@:
	   ; phase IV Ј results
	   inc	eax
	   lea	esi,[!outA]
	   inc	eax
	   mov	edi,[!OutBuffer]


	   @@:
	   mov	ecx,[esi]
	   mov	edx,[esi+4]
	  xchg	cl,ch
	  xchg	dl,dh
	   rol	ecx,16
	   rol	edx,16
	  xchg	cl,ch
	  xchg	dl,dh
	   mov	[edi],ecx
	   mov	[edi+4],edx

	   add	esi,8
	   dec	eax
	   lea	edi,[edi+8]
	   jnz	@b

	   ret
endpM
