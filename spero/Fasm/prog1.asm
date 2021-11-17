; Example of 64-bit PE program

format PE64 GUI
entry start

 RC6_P16   equ              0xB7E1
 RC6_Q16    equ             0x9E37
 RC6_P32    equ             0xB7E15163
 RC6_Q32    equ             0x9E3779B9
 RC6_P64    equ             0xB7E151628AED2A6B
 RC6_Q64    equ             0x9E3779B97F4A7C15

 RC6_W      equ                     (0x20)
 RC6_KEY_MAX   equ                  (0x100 / 0x8)
 RC6_R         equ                  (0x14)
 RC6_B         equ                  (0x10)
 RC6_T         equ                  (0x2 * RC6_R + 0x4)


         RC6_WORD       equ         dword
         RC6_WORD_SIZE  equ 0x4
         RC6_WORD_SHIFT equ 0x2
         RC6_WORD_RAX   equ eax
         RC6_WORD_RCX   equ ecx
         RC6_WORD_RDX   equ edx
         RC6_WORD_RBX   equ ebx
         RC6_P             equ      RC6_P32
         RC6_Q              equ     RC6_Q32
         RC6_LG              equ    0x5




section '.text' code readable executable



  start:
        sub     rsp,8*5         ; reserve stack for API use and make stack dqword aligned

        mov     r9d,0
        lea     r8,[_caption]
        lea     rdx,[_message]
        mov     rcx,0
        call    [MessageBoxA]

        mov     ecx,eax
        call    [ExitProcess]



section '.data' data readable writeable
  _key     db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  _caption db 'Win64 assembly program',0
  _message db 'Hello World!',0

section '.idata' import data readable writeable

  dd 0,0,0,RVA kernel_name,RVA kernel_table
  dd 0,0,0,RVA user_name,RVA user_table
  dd 0,0,0,0,0

  kernel_table:
    ExitProcess dq RVA _ExitProcess
    dq 0
  user_table:
    MessageBoxA dq RVA _MessageBoxA
    dq 0

  kernel_name db 'KERNEL32.DLL',0
  user_name db 'USER32.DLL',0

  _ExitProcess dw 0
    db 'ExitProcess',0
  _MessageBoxA dw 0
    db 'MessageBoxA',0 