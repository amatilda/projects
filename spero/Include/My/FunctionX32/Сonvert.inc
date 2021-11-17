proc HexToUnicodHL !Hex,!Destination
     ;Подготовим
     mov  eax,0x30;eax = 0x30
     mov  edx,[!Destination];edx = !Destination
     mov  ecx,[!Hex];ecx = !Hex

     ;Сначала обнулим полученую память
     ;ecx = !Hex;edx = !Destination;eax = 0
     mov  [edx],ax;eax = 0x30;edx = !Destination
     mov  [edx+2],ax;eax = 0x30;edx = !Destination
     mov  [edx+4],ax;eax = 0x30;edx = !Destination
     mov  [edx+6],ax;eax = 0x30;edx = !Destination
     mov  [edx+8],ax;eax = 0x30;edx = !Destination
     mov  [edx+10],ax;eax = 0x30;edx = !Destination
     mov  [edx+12],ax;eax = 0x30;edx = !Destination

     ;Каректируем что бы указвало на самый конец !Destination
     ;ecx = !Hex;edx = !Destination
     add  edx,16;edx = !Destination

     ;Начинаем цикл преобразования цифер в юникоде в системе исчесления Hex
     ;ecx = !Hex;edx = !Destination
     @@:
     mov  al,cl
     sub  edx,2
     and  al,0x0F
     cmp  al,0xA
     sbb  al,0x69
     das
     shr  ecx,4;ecx = !Hex
     mov  [edx],ax
     jnz  @b
     ret
endp