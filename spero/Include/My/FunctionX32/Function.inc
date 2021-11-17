;--------------------------------------
HLproc HLRectInRect lprcSrc1,lprcSrc2,nRectSize
	   movd  mm7,esp
	   movd  mm6,ebx
	   movd  mm5,esi
	   movd  mm4,edi

	    mov  eax,[lprcSrc1]
	    mov  edi,[lprcSrc2]
	    mov  edx,[nRectSize]


	  mov  ebp,[eax+RECT.left]
	  mov  esi,[eax+RECT.top]
	  mov  esp,[eax+RECT.right]
	  mov  ebx,[eax+RECT.bottom]

       .loop:
	  cmp  ebp,[edi+RECT.right]
	 setl  al
	  cmp  esp,[edi+RECT.left]
	 setg  ah

	  cmp  esi,[edi+RECT.bottom]
	 setl  cl
	  cmp  ebx,[edi+RECT.top]
	 setg  ch
	  and  al,ah
	  and  cl,ch
	  and  al,cl
	  jnz  .proc_end

	  add  edi,16
	  sub  edx,16
	  jnz  .loop
	  xor  eax,eax

     .proc_end:
	   movd  esp,mm7
	   movd  ebx,mm6
	   movd  esi,mm5
	   movd  edi,mm4
	   emms
	       ret
endp
;--------------------------------------
;stdcall ExcludeRectOfRegion lpRgnData,nSize,lpRects,nCount
;функция исключает прямоугольники из региона описываемого структурой RGNDATA
;lpRgnData-указатель на структуру RGNDATA причём буфер должен быть достаточно большим так как возможна запись дополнительных прямоугольников
;nSize-размер в байтах буфера на который указывает lpRgnData
;lpRects-указатель на массив прямоугольников
;nCount-количество прямоугольников на которые указывает lpRects
;возвращаемое значение размер в байтах пряугольников следующих за RGNDATAHEADER
;если -1 значит произошло переполнение буфера

;mm7  - esp
;mm6  - nCount
;mm5  - lpRgnData максимальный
;mm4  - lpRects
;mm3  -
ExcludeRectOfRegion:
	 push  ebx edi esi ebp;сохраняем для востанновления

	  mov  ebp,esp
	 movd  mm7,esp;сохраняем для востанновления

	  mov  edi,[ebp+4*5];lpRgnData
	  mov  ecx,[ebp+4*6];nSize
	  mov  eax,[ebp+4*7];lpRects
	 movd  mm6,[ebp+4*8];nCount;сохраняем для дольнейшего использования
	  mov  edx,[edi+RGNDATAHEADER.nCount];заносим количество структур RECT в структуре RGNDATA

	  lea  ecx,[ecx+edi-sizeof.RECT*3];находим максимально дальний безопастный указатель, на который указывает lpRgnData
	  shl  edx,4
	 movd  mm5,ecx;сохраняем для дольнейшего использования
	  lea  edi,[edi+edx+sizeof.RGNDATAHEADER]; получаем указатель куда можно записовать новые структуры RECT
	 movd  mm4,eax;сохраняем для дольнейшего использования
	 movd  mm3,edi;сохраняем для дольнейшего использования
	  cmp  edi,ecx;провераем достачен ли размер буфера получаемый в  lpRgnData
	  jae .error_buffer

   .big_loop:
	  mov  ebp,[eax+RECT.left]
	  mov  esi,[eax+RECT.top]
	  mov  esp,[eax+RECT.right]
	  mov  ebx,[eax+RECT.bottom]


       .loop:
	  cmp  esi,[edi+RECT.top-sizeof.RECT];если неисключаемый меньше исключаемого 0 иначе 1
	setle  ch
	  cmp  ebx,[edi+RECT.top-sizeof.RECT];использования "равно" в таких местах позволяет избежать таких ситуаций как например два непересекающияся прямоугольники будут считаться таковыми
	setle  ah

	  cmp  esi,[edi+RECT.bottom-sizeof.RECT]
	 setl  al
	  add  ch,ah
	  cmp  ebx,[edi+RECT.bottom-sizeof.RECT]
	 setl  ah
	  add  ah,al
	  sub  ch,2
	  shl  ah,1;за счёт сдвига увеличываем разнобразие среди чисел
	  add  ch,ah

	  cmp  ch,4;в итоге получаем числа от 0 до 3 в любом другом случае значит прямоугольники не пересекаются
	  jae  .next

	  cmp  ebp,[edi+RECT.left-sizeof.RECT]
	setle  cl
	  cmp  esp,[edi+RECT.left-sizeof.RECT]
	setle  al

	  cmp  ebp,[edi+RECT.right-sizeof.RECT]
	 setl  ah
	  add  cl,al
	  cmp  esp,[edi+RECT.right-sizeof.RECT]
	 setl  al
	  add  ah,al
	  sub  cl,2
	  shl  ah,1
	  add  cl,ah

	  cmp  cl,4
	  jae  .next

	movzx  eax,ch
	movzx  ecx,cl
	 movq  mm0,qword[edi+RECT.left-sizeof.RECT]
	 movq  mm1,qword[edi+RECT.right-sizeof.RECT]
	  lea  ecx,[eax*4+ecx];упрощаем для использования таблицы переходов
	 movd  eax,mm3
	  jmp  dword[4*ecx+@f]

align 4
@@:
dd  .x2_0
dd  .x2_1
dd  .x2_2
dd  .x2_3
dd  .x2_4
dd  .x2_5
dd  .x2_6
dd  .x2_7
dd  .x2_8
dd  .x2_9
dd  .x2_10
dd  .x2_11
dd  .x2_12
dd  .x2_13
dd  .x2_14
dd  .x2_15


       .x2_0:
	psrlq  mm0,32
	  mov  [edi+RECT.right-sizeof.RECT],ebp

	  mov  [eax+RECT.left],ebp
	 movd  [eax+RECT.top],mm0
	 movd  [eax+RECT.right],mm1
	  mov  [eax+RECT.bottom],esi

	  add  eax,sizeof.RECT
	  jmp  .next_sav_eax

       .x2_1:
	  mov  [edi+RECT.bottom-sizeof.RECT],esi
	  jmp  .next

       .x2_2:
	psrlq  mm0,32
	  mov  [edi+RECT.right-sizeof.RECT],ebp

	  mov  [eax+RECT.left],ebp
	 movd  [eax+RECT.top],mm0
	  mov  [eax+RECT.right],esp
	  mov  [eax+RECT.bottom],esi

	  mov  [eax+RECT.left+sizeof.RECT],esp
	 movd  [eax+RECT.top+sizeof.RECT],mm0
	 movq  qword[eax+RECT.right+sizeof.RECT],mm1

	  add  eax,sizeof.RECT*2
	  jmp  .next_sav_eax

       .x2_3:
	  mov  [edi+RECT.left-sizeof.RECT],esp

	 movq  qword[eax+RECT.left],mm0
	 movd  [eax+RECT.right],mm1
	  mov  [eax+RECT.bottom],esi

	  add  eax,sizeof.RECT
	  jmp  .next_sav_eax

       .x2_4:
	  mov  [edi+RECT.right-sizeof.RECT],ebp
	  jmp  .next

       .x2_5:;поглощается
	  mov  ecx,edi
	  sub  eax,sizeof.RECT
	  sub  ecx,edx
	  cmp  ecx,eax
	   je  @f
	 movq  mm0,qword[eax+RECT.left]
	 movq  mm1,qword[eax+RECT.right]
	 movq  mm0,qword[edi+RECT.left-sizeof.RECT]
	 movq  mm1,qword[edi+RECT.right-sizeof.RECT]
	  jmp  .next_sav_eax
	       @@:
	       xor  eax,eax
	       mov  [ecx-24],eax;RGNDATAHEADER.nCount
	       jmp  .ret

       .x2_6:
	psrlq  mm0,32
	  mov  [edi+RECT.right-sizeof.RECT],ebp

	  mov  [eax+RECT.left],esp
	 movd  [eax+RECT.top],mm0
	 movq  qword[eax+RECT.right],mm1

	  add  eax,sizeof.RECT
	  jmp  .next_sav_eax

       .x2_7:
	  mov  [edi+RECT.left-sizeof.RECT],esp
	  jmp  .next

       .x2_8:
	psrlq  mm0,32
	  mov  [edi+RECT.right-sizeof.RECT],ebp

	  mov  [eax+RECT.left],ebp
	 movd  [eax+RECT.top],mm0
	 movd  [eax+RECT.right],mm1
	  mov  [eax+RECT.bottom],esi

	  mov  [eax+RECT.left+sizeof.RECT],ebp
	  mov  [eax+RECT.top+sizeof.RECT],ebx
	 movq  qword[eax+RECT.right+sizeof.RECT],mm1

	  add  eax,sizeof.RECT*2
	  jmp  .next_sav_eax

       .x2_9:
	  mov  [edi+RECT.bottom-sizeof.RECT],esi

	 movd  [eax+RECT.left],mm0
	  mov  [eax+RECT.top],ebx
	 movq  qword[eax+RECT.right],mm1

	  add  eax,sizeof.RECT
	  jmp  .next_sav_eax

      .x2_10:
	  mov  [edi+RECT.bottom-sizeof.RECT],esi

	 movd  [eax+RECT.left],mm0
	  mov  [eax+RECT.top],ebx
	 movq  qword[eax+RECT.right],mm1

	 movd  [eax+RECT.left+sizeof.RECT],mm0
	  mov  [eax+RECT.top+sizeof.RECT],esi
	  mov  [eax+RECT.right+sizeof.RECT],ebp
	  mov  [eax+RECT.bottom+sizeof.RECT],ebx

	  mov  [eax+RECT.left+sizeof.RECT*2],esp
	  mov  [eax+RECT.top+sizeof.RECT*2],esi
	 movd  [eax+RECT.right+sizeof.RECT*2],mm1
	  mov  [eax+RECT.bottom+sizeof.RECT*2],ebx

	  add  eax,sizeof.RECT*3
	  jmp  .next_sav_eax

      .x2_11:
	  mov  [edi+RECT.bottom-sizeof.RECT],esi

	  mov  [eax+RECT.left],esp
	  mov  [eax+RECT.top],esi
	 movd  [eax+RECT.right],mm1
	  mov  [eax+RECT.bottom],ebx

	 movd  [eax+RECT.left+sizeof.RECT],mm0
	  mov  [eax+RECT.top+sizeof.RECT],ebx
	 movq  qword[eax+RECT.right+sizeof.RECT],mm1

	  add  eax,sizeof.RECT*2
	  jmp  .next_sav_eax

      .x2_12:
	  mov  [edi+RECT.right-sizeof.RECT],ebp

	  mov  [eax+RECT.left],ebp
	  mov  [eax+RECT.top],ebx
	 movq  qword[eax+RECT.right],mm1

	  add  eax,sizeof.RECT
	  jmp  .next_sav_eax

      .x2_13:
	  mov  [edi+RECT.top-sizeof.RECT],ebx
	  jmp  .next

      .x2_14:
	  mov  [edi+RECT.top-sizeof.RECT],ebx

	 movq  qword[eax+RECT.left],mm0
	  mov  [eax+RECT.right],ebp
	  mov  [eax+RECT.bottom],ebx

	psrlq  mm0,32
	  mov  [eax+RECT.left+sizeof.RECT],esp
	 movd  [eax+RECT.top+sizeof.RECT],mm0
	 movd  [eax+RECT.right+sizeof.RECT],mm1
	  mov  [eax+RECT.bottom+sizeof.RECT],ebx

	  add  eax,sizeof.RECT*2
	  jmp  .next_sav_eax

      .x2_15:
	psrlq  mm0,32
	  add  eax,sizeof.RECT
	  mov  [edi+RECT.top-sizeof.RECT],ebx

	  mov  [eax+RECT.left-sizeof.RECT],esp
	 movd  [eax+RECT.top-sizeof.RECT],mm0
	 movd  [eax+RECT.right-sizeof.RECT],mm1
	  mov  [eax+RECT.bottom-sizeof.RECT],ebx

.next_sav_eax:
	 movd  ecx,mm5
	 movd  mm3,eax
	  cmp  eax,ecx
	  jae .error_buffer

       .next:
	  sub  edi,sizeof.RECT
	  sub  edx,sizeof.RECT
	  jnz  .loop

	 movd  edx,mm3
	 movd  esi,mm6
	 movd  eax,mm4
	  sub  edx,edi
	 movd  edi,mm3

	  add  eax,sizeof.RECT
	  sub  esi,1
	 movd  mm6,esi
	 movd  mm4,eax
	  jnz  .big_loop

	  sub  edi,edx
	  mov  eax,edx
	  shr  edx,4
	  mov  [edi-24],edx;RGNDATAHEADER.nCount
	.ret:
	 movd  esp,mm7
	 emms

	  pop  ebp esi edi ebx
	  ret  16

.error_buffer:
	   mov	eax,-1
	   jmp	.ret

;--------------------------------------