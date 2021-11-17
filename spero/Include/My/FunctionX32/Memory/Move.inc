struct HEAP
 sizeofHeap         LpType  ?
 flProtect          LpType  ?
 nextlpHeap         LpType  ?
 prevlpHeap         LpType  ?
ends

struct ALLOCHEAP
 sizeofAllocHeap    LpType  ?
                    LpType  ?
ends

struct SPACEHEAP
 sizeofSpace        LpType  ?
 nextlpSpace        LpType  ?
 prevlpSpace        LpType  ?
                    LpType  ?
ends

struct MAINHEAP
 aSRWLock           LpType  ?;RTL_SRWLOCK  RTL_SRWLOCK_INIT;RTL_SRWLOCK_INIT = 0 ;не меняем положение
 minSpace           LpType  ?
 maxSpace           LpType  ?
 lpSpaceHeap        LpType  ?
 aHEAP              HEAP
ends

proc HeapAllocHL [ebx esi edi ebp],!hHeap,!dwBytes
                 mov  ebp,[!hHeap];ebp = MAINHEAP
                 mov  ebx,[!dwBytes]

              invoke  AcquireSRWLockExclusive,ebp;Так как эти даные могут читаться вторым потоком

                 add  ebx,(sizeof.ALLOCHEAP+7) and 0xFFFFFFF8

                 xor  edx,edx
                 cmp  [ebp+MAINHEAP.aSRWLock],ebx
                seta  al
                  or  edx,[ebp+MAINHEAP.lpSpaceHeap]
                setz  ah
                  or  al,ah
                  jz  @f

                 int 3
                 @@:


              invoke  ReleaseSRWLockExclusive,ebp;Так как эти даные могут читаться вторым потоком
                 ret
endp


proc HeapCreateHL [ebp ebx],!dwInitialSize,!flProtect
                  mov  ebp,[!dwInitialSize]
                  mov  ebx,[!flProtect]
                  add  ebp,0xFFF
                  and  ebp,0xFFFFF000

                  invoke  VirtualAlloc,0,ebp,MEM_COMMIT or MEM_RESERVE,ebx
                    test  eax,eax
                      jz  .Errror

                     lea  ecx,[ebp-sizeof.MAINHEAP]
                     lea  edx,[eax+sizeof.MAINHEAP]
                     mov  [eax+MAINHEAP.minSpace],ecx
                     mov  [eax+MAINHEAP.maxSpace],ecx
                     mov  [eax+MAINHEAP.lpSpaceHeap],edx
                     mov  [eax+MAINHEAP.aHEAP.sizeofHeap],ebp
                     lea  ebp,[eax+MAINHEAP.aHEAP]
                     mov  [eax+MAINHEAP.aHEAP.flProtect],ebx
                     mov  [eax+MAINHEAP.aHEAP.nextlpHeap],ebp
                     mov  [eax+MAINHEAP.aHEAP.prevlpHeap],ebp

                     mov  [edx+SPACEHEAP.sizeofSpace],ecx
                     mov  [edx+SPACEHEAP.nextlpSpace],edx
                     mov  [edx+SPACEHEAP.prevlpSpace],edx


                .Errror:
                     ret
endp







;На входе:;[!Destination] - Место назначения;[!Source] - Источник;[!Length] - Обьем копирования в байтах
proc MoveMemoryHL [ebx esi edi],!Destination,!Source,!Length;поддерживает при перемещение данных alingn 16 4 1, нужна поддержка SSE - сейчас не найти процессор без него так что наличие его не проверяем.
            mov  edi,[!Destination];Место назначения
            mov  esi,[!Source];Источник
            mov  eax,edi
            mov  ecx,[!Length];Обьем копирования в байтах
             or  eax,esi

           test  eax,15;2 and 3
           setz  bh
            and  eax,3;1
           setz  al
            cmp  ecx,4;1
          setae  bl
            cmp  ecx,32;2
          setae  dl
            cmp  ecx,256;3
          setae  dh

            and  al,bl
            and  dl,bh
            and  dh,bh
            add  al,dl
            cld
            add  al,dh
            mov  ebx,ecx
            jmp  dword[.Table+eax*4]



      .LoopMax:;3
            shr  ebx,7

          align  4
            @@:
         movdqa  xmm0,[esi]
         movdqa  xmm1,[esi+16]
         movdqa  xmm2,[esi+32]
         movdqa  xmm3,[esi+48]
         movdqa  [edi],xmm0
         movdqa  [edi+16],xmm1
         movdqa  [edi+32],xmm2
         movdqa  [edi+48],xmm3

         movdqa  xmm4,[esi+64]
         movdqa  xmm5,[esi+80]
         movdqa  xmm6,[esi+96]
         movdqa  xmm7,[esi+112]
         movdqa  [edi+64],xmm4
         movdqa  [edi+80],xmm5
         movdqa  [edi+96],xmm6
         movdqa  [edi+112],xmm7
            add  esi,128
            add  edi,128
            dec  ebx
            jnz  @b

            and  ecx,127
            mov  ebx,ecx
            cmp  ecx,4
          setae  al
            cmp  ecx,32
          setae  dl
            add  al,dl
            jmp  dword[.Table+eax*4]

   .LoopMovaps:;2
            shr  ebx,5

          align  4
            @@:
         movdqa  xmm0,[esi]
         movdqa  xmm1,[esi+16]
         movdqa  [edi],xmm0
         movdqa  [edi+16],xmm1
            add  esi,32
            add  edi,32
            dec  ebx
            jnz  @b

            and  ecx,31
            mov  ebx,ecx
            cmp  ecx,4
             jb  .RepMovsb

     .RepMovsd:;1
            shr  ecx,2
            and  ebx,3
            rep  movsd

     .RepMovsb:;0
            mov  ecx,ebx
            rep  movsb
            ret

align 4
.Table:
       dd  .RepMovsb;0
       dd  .RepMovsd;1
       dd  .LoopMovaps;2
       dd  .LoopMax;3
endp

