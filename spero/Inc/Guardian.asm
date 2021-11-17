
format PE GUI 4.0 DLL
entry DllEntryPoint

include  '..\Include\Win32W.inc'

section '.text' code readable executable

proc DllEntryPoint [ebp],!hinstDLL,!fdwReason,!lpvReserved
  locals
   !szModuleFileName         du  MAX_PATH*4 dup(?)
  endl

        ;������� ���� � dll
        lea  ebp,[!szModuleFileName];ebp = !szModuleFileName
     invoke  GetModuleFileNameW,[!hinstDLL],ebp,MAX_PATH*4;ebp = !szModuleFileName

        ;������� ����
        xor  eax,eax;eax = 0
     invoke  CreateFileW,ebp,GENERIC_READ,eax,eax,OPEN_EXISTING,eax,eax;ebp = !szModuleFileName;eax = 0
        mov  ebp,eax;ebp = ����� �����

        ;���� �� ������ ������� ���� ������ ��� �������� ���������
        inc  eax;INVALID_HANDLE_VALUE
        jnz  @f

        ;������� ��������  dll
        ret

        ;������� ����� ����� � �� ����� ��������� �������� dll
        @@:
     invoke  CloseHandle,ebp;ebp = ����� �����
        mov     eax,1
        ret
endp


section '.idata' import data readable writeable
include '..\Include\My\Import.inc'



section '.reloc' data fixups readable