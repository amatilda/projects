proc  CRC32cHL [esi edi ebx ],!Source,!Length,!FirstStep;0x82F63B78 таблица генерируеться макросом отдельно -  PolinomCRC32 8,0x82F63B78

            mov  esi,[!Source];Источник
            mov  ecx,[!Length];Обьем для которгого нужно посчитать CRC32
            mov  eax,[!FirstStep];Начало для CRC32 зачастую -1


           test  esi,3
           setz  dl
            cmp  ecx,11
          setae  dh
            shl  dl,1
            add  dl,dh
          movzx  ebx,dl
            jmp  dword[.Table + ebx*4]

 .PredBiglLoop:
            @@:
          movzx  edx,byte[esi]
            xor  dl,al
            inc  esi
            shr  eax,8
            dec  ecx
            xor  eax,[CRC32_0x82F63B78+edx*4]
           test  esi,3
            jnz  @b


     .BiglLoop:
            sub  ecx,8
          align  4
            @@:
            mov  ebx,eax
            mov  edx,[esi+4]
            xor  ebx,[esi]

          movzx  edi,dl
            mov  eax,[edi*4 + CRC32_0x82F63B78 + 1024*3]
          movzx  edi,dh
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*2]
            shr  edx,16
          movzx  edi,dl
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*1]
          movzx  edi,dh
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*0]

          movzx  edi,bl
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*7]
          movzx  edi,bh
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*6]
            shr  ebx,16
          movzx  edi,bl
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*5]
          movzx  edi,bh
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*4]


            add  esi,8
            sub  ecx,8
            jae  @b

            add  ecx,8
             jz  .Finish

    .SmallLoop:
            @@:
          movzx  edx,byte[esi]
            xor  dl,al
            inc  esi
            shr  eax,8
            xor  eax,[CRC32_0x82F63B78+edx*4]
            dec  ecx
            jnz  @b

       .Finish:
            not  eax
            ret

align 4
.Table:
       dd  .SmallLoop;0
       dd  .PredBiglLoop;1
       dd  .SmallLoop;2
       dd  .BiglLoop;3
endp

proc  CRC32HL2 [esi],!Source,!Length
            xor  eax,eax
            mov  esi,[!Source];Источник
            mov  ecx,[!Length];Обьем для которгого нужно посчитать CRC32
            dec  eax

          align  4
            @@:
          movzx  edx,byte[esi]
            xor  dl,al
            inc  esi
            shr  eax,8
            xor  eax,[CRC32_0x82F63B78+edx*4]
            dec  ecx
            jnz  @b

            not  eax
            ret

endp