
format PE GUI 4.0 DLL
entry DllEntryPoint

include  '..\Include\Win32W.inc'

section '.text' code readable executable

proc DllEntryPoint [ebp],!hinstDLL,!fdwReason,!lpvReserved
  locals
   !szModuleFileName         du  MAX_PATH*4 dup(?)
  endl

        ;Получим путь к dll
        lea  ebp,[!szModuleFileName];ebp = !szModuleFileName
     invoke  GetModuleFileNameW,[!hinstDLL],ebp,MAX_PATH*4;ebp = !szModuleFileName

        ;Откроем файл
        xor  eax,eax;eax = 0
     invoke  CreateFileW,ebp,GENERIC_READ,eax,eax,OPEN_EXISTING,eax,eax;ebp = !szModuleFileName;eax = 0
        mov  ebp,eax;ebp = хендл файла

        ;Если не смогли открыть файл значит под отладкой находимся
        inc  eax;INVALID_HANDLE_VALUE
        jnz  @f

        ;Прервем загрузку  dll
        ret

        ;Закроем хендл файла и не будем прерывать загрузку dll
        @@:
     invoke  CloseHandle,ebp;ebp = хендл файла
        mov     eax,1
        ret
endp


section '.idata' import data readable writeable
include '..\Include\My\Import.inc'



section '.reloc' data fixups readable