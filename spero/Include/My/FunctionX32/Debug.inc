 proc _DebugMessageProc  !Offset;[esi edi ebp ebx]

     stdcall  HexToUnicodHL,eax,_aDebugMessage.MessageEaxHex

      invoke  GetLastError
     stdcall  HexToUnicodHL,eax,_aDebugMessage.MessageLastErrHex

     stdcall  HexToUnicodHL,[!Offset],_aDebugMessage.MessageOffsetHex

         xor  ebx,ebx
      invoke  MessageBoxExW,ebx,_aDebugMessage,ebx,MB_OK or MB_ICONSTOP or MB_APPLMODAL,ebx

      invoke  ExitProcess,eax
        ;ret
 endp