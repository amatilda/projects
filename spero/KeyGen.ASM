format PE GUI 4.0
entry Start

;equGlAssembly equ Debug; ������ ������� � ��� ���������� ������

include  'Include\Win32W.inc'
include  'Inc\ConstEquates.inc'

 enum  1,\;��������� ��������
       DIALOG_MAIN_BUTTON_OK_Id,\
       DIALOG_MAIN_EDIT_OPENKEY_Id,\
       DIALOG_MAIN_BUTTON_GENERATE_Id,\
       DIALOG_MAIN_EDIT_SET_Id


struct  OPENKEYW
                     du  'N 0x'
 MessageN            du  8 dup(' '),0
ends

;----------------------------------------------
section '.data' data readable writeable
_DebugDate; ��� ������� � ������ �� �����������

aVectoredExceptionHandler               VectoredExceptionHandler ;��������� ��� ����������

aKeyFile                     KEYFILEW;��������� ��� ���������� ����� � ����

aOpenKey                     OPENKEYW;��������� ��� ����������� N �����


hwnd                         ddA  ?;���� ������� ��������� ������� � ��� �������� ����� ����� ��� ����������� ������ �������� ��� SEH
hwndEditSet                  ddA  ?
hwndEditOpenKey              ddA  ?

iNumberOfBytesWritten        ddA  ?
hInstance                    ddA  ?


numberN                      ddA  ?;����� ��� RSA
numberP                      ddA  ?;����� ��� RSA
numberQ                      ddA  ?;����� ��� RSA
numberFN                     ddA  ?;����� ��� RSA
;numberE                      ddA  ?;����� ��� RSA
numberD                      ddA  ?;����� ��� RSA


;----------------------------------------------
section '.text' code readable executable
              ; _VTuneMini
              ; _EndVTuneMini

Start:

;SEH
;----------------------------------------------
           xor  ebx,ebx;ebx = 0
        invoke  SetUnhandledExceptionFilter,UnhandledExceptionFilter;������������� ��� ��� �� ������ ��� ����������

;������� ���� ������ - hInstance
;ebx = 0
;----------------------------------------------
        invoke  GetModuleHandleW,ebx;ebx = 0
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [hInstance],eax


;��������� ������ ��� ������� ����
;ebx = 0
;----------------------------------------------
       stdcall  LoadResoursIndirectHL,tbRussian,RT_DLG,DIALOG_MAIN_ID
        invoke  DialogBoxIndirectParamW,[hInstance],eax,ebx,DlgProcMain,ecx;ecx = ��� �������;ebx = 0
                _LastErrorNotSigned;���� �� ����� ��������� ������� �� ������ ���

        invoke  ExitProcess,eax



 proc DlgProcMain [esi edi ebp ebx],!HwndDlg,!Wmsg,!Wparam,!Lparam
                 mov  ebp,[!HwndDlg];ebp = ����� �������
                 mov  eax,[!Wmsg];eax = ���������
                 mov  ecx,[!Wparam];ecx = !Wparam
                 mov  edx,[!Lparam];edx = !Lparam
                 xor  ebx,ebx;ebx = ����


;ebp = ����� ������� ;ebx = ����;ecx = !Wparam ;edx = !Lparam
;--------------------------------------
                 cmp  eax,WM_COMMAND
                  je  .WM_COMMAND
                 cmp  eax,WM_INITDIALOG
                  je  .WM_INITDIALOG



             .False:
                 xor  eax,eax
                 ret



;������������ ��������� �� Control, Accelerator, Menu
;ebp = ����� ������� ;ebx = ����;ecx = !Wparam ;edx = !Lparam
;--------------------------------------
        .WM_COMMAND:
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_MAIN_BUTTON_OK_Id;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .Exit;ebp = ����� ������� ;ebx = ����
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_MAIN_BUTTON_GENERATE_Id;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .DIALOG_MAIN_BUTTON_GENERATE_Id;ebp = ����� ������� ;ebx = ����

                 jmp  .True


;��������� � ������������ �������� � �������� ����
;ebp = ����� ������� ;ebx = ����
;--------------------------------------
.DIALOG_MAIN_BUTTON_GENERATE_Id:

              ;������� �����, ���� ������� �� � �����
              ;ebp = ����� ������� ;ebx = ����
                 mov  esi,aKeyFile.szNameUser;esi = aKeyFile.szNameUser
              invoke  SendMessageW,[hwndEditSet],WM_GETTEXT,sizeof.KEYFILEW.szNameUser/2,esi;esi = aKeyFile.szNameUser
                 ;eax = leng text in simbols
                test  eax,eax
                  jz  .NotText;ebp = ����� ������� ;ebx = ����
                 cmp  eax,sizeof.KEYFILEW.szNameUser/2-4;��� ��� ������������� ���������� �������� ��� ����� ����, ������� �������� ���� ����� ��� ������ ������ ��� �� �� ��
                 jae  .ErrorBuffer;ebp = ����� ������� ;ebx = ����



              ;�������� ������������ �� �������� ����� �������
              ;esi = aKeyFile.szNameUser;ebx = ����;eax = leng text in simbols;ebp = ����� �������
                 mov  edx,-2
                 xor  ecx,ecx

           ;��������� �� ������� ���� �������� �  - , ����� ������� ��� ����� �������� ������� �� ������� � ������
           .LoopCmp:
                 xor  edi,edi
                  or  di,word[esi+ecx*2];esi = aKeyFile.szNameUser
                  jz  .ErrorNotFormatData;���� ���� ������ � ����� - ����� �� ��������������

                 cmp  di,' '
                  je  .SpaceCmp; ���� ������ ������ ����� �������� ���

                 inc  ecx
                 jmp  .LoopCmp

          .SpaceCmp:
                 inc  ecx

                 inc  edx; �������� ��� ������
                  jz  .EndLoopCmp

                 jmp  .LoopCmp

        .EndLoopCmp:
                 ; ���� ����� ������� ������� �� '-', ������ �� ��� ������ ��������
                 cmp  word[esi+ecx*2],'-';esi = aKeyFile.szNameUser
                 jne  .ErrorNotFormatData;ebp = ����� ������� ;ebx = ����

                 lea  eax,[ecx-1]
                 inc  ecx
                 mov  [aKeyFile.lengthszNameUser],eax;��������� ����� � �������� ��� ���� ������� � ����� ������������ ��� ���������� � ����

              ;�������� ������������ �� �������� ����� ������� ������ �� �����
              ;esi = aKeyFile.szNameUser;ebx = ����;eax = leng text in simbols;ebp = ����� �������
          .LoopCmp2:
                  ;edx = 0= - ����� �� ���� �� ������ ��
                  or  dx,word[esi+ecx*2];esi = aKeyFile.szNameUser
                  jz  .ErrorNotFormatData;���� ���� ������ � ����� - ����� �� ��������������

                 cmp  dx,'@'
                  je  .AllGood;���� ������ ������ ����� �������� ���

                 xor  edx,edx
                 inc  ecx
                 jmp  .LoopCmp2




              ;��������� � ������� ������������� ������� � ���� ����� ���������� � ������� CRC32
              ;esi = aKeyFile.szNameUser;ebx = ����;eax = leng text in simbols
           .AllGood:
                 shl  eax,1;eax = leng text in simbols
                 mov  edi,CRC32cHL;edi = CRC32cHL
             stdcall  edi,esi,eax,-1;eax = leng text in simbols;esi = aKeyFile.szNameUser;edi = CRC32cHL
                 mov  [aKeyFile.iCRC32ForKey],eax ;CRC32c - ������ � �������� � ������ ��� �������� RSA


;��� ������ ����� �������� ������������ �������� ������ �� ��������� ;eax = CRC32c � sKeyRules(������ ��������� � ������� ��������� ��������)
;��������� ���� ���� ������ �������� ����� �������� ��� ��� ������������ ��� � ��� ��������� ������ �� ����������� � ����� ������, ��� ��������� �� ���� �����
;RSA-32 - �������������, ��� ����� ������ ����������� ����� ��, ��� ��� �������� ��������� ����� �������� ����� ��������
;--------------------------------------
             ;edi = CRC32cHL
             ;������� �� ��������� sKeyRules - CRC32 �� ��������� ������ ������� ����� n - numberN
             stdcall  edi,sKeyRules,sizeof.sKeyRules,-1;eax = leng text in simbols;esi = aKeyFile.szNameUser;edi = CRC32cHL
                 mov  esi,eax;esi = ��������� numberN

                ;��� ��� ������ RSA ������� ��������� ����� ����� �� �������� �� �������� ���� ��� ����� ��  ����� ��  3  �� 2001
                ;������� ����� numberP
                 mov  ecx,eax
                 shr  ecx,16
                 mov  edi,3
                  or  ecx,1;������ �������� numberP

                 ;��������� �� ��������
                 @@:
                 xor  edx,edx
                 mov  eax,ecx
                 div  edi
                 add  edi,2
                test  edx,edx
                  jz  .NotSimple
                 cmp  edi,2001
                 jae  .Simple
                 jmp  @b

         .NotSimple:
                 sub  ecx,2;numberP
                 mov  edi,3
                 jmp  @b

            .Simple:
                 cmp  ecx,2001
                 jbe  .ErrorSimple;ebp = ����� ������� ;ebx = ����
                 mov  [numberP],ecx;numberP


                ;������� ����� numberQ
                 mov  eax,esi;esi = ��������� numberN
                 xor  edx,edx
                 mov  esi,ecx;esi = numberP
                 div  ecx
                 mov  edi,3
                 mov  ecx,eax;numberQ
                  or  ecx,1;numberQ

                 ;��������� �� ��������
                 @@:
                 xor  edx,edx
                 mov  eax,ecx
                 div  edi
                 add  edi,2
                test  edx,edx
                  jz  .NotSimple2
                 cmp  edi,2001
                 jae  .Simple2
                 jmp  @b

        .NotSimple2:
                 sub  ecx,2;numberP
                 mov  edi,3
                 jmp  @b

           .Simple2:
                 cmp  ecx,2001
                 jbe  .ErrorSimple;ebp = ����� ������� ;ebx = ����
                 mov  [numberQ],ecx;numberQ

              ;�� ��������� numberQ � numberP ������� �������� numberN
              ;esi = numberP;ecx = numberQ;ebp = ����� ������� ;ebx = ����
                 mov  eax,ecx;ecx = numberQ
                 mul  esi
                 mov  [numberN],eax
                test  edx,edx
                 jnz  .ErrornumberN;ebp = ����� ������� ;ebx = ����

              ;�� ��������� numberQ � numberP �������  numberFN
              ;esi = numberP;ecx = numberQ;ebp = ����� ������� ;ebx = ����
                 lea  eax,[esi-1];esi = numberP
                 dec  ecx;ecx = numberQ
                 mul  ecx
                 mov  [numberFN],eax
                 mov  esi,eax;esi = numberFN

              ;������ numberD �� ��������� ������ ��������
              ;eax = numberFN ;esi = numberFN;numberE = equNumberE;ebp = ����� ������� ;ebx = ����
                 mov  edi,constNumberE;edi = equNumberE
                 div  edi
                  or  eax,1
                 mov  ecx,eax;ecx = numberD

                 @@:
                 mul  edi;edi = constNumberE
                 div  esi;esi = numberFN
                 cmp  edx,1
                  je  @f;����� �� ��� ���� �����;eax = numberD

                 add  ecx,2
                 mov  eax,ecx
                 cmp  ecx,0xFFFFFFFC
                 jae  .ErrornumberD;ebp = ����� ������� ;ebx = ����
                 jmp  @b

                 @@:
                 mov  [numberD],ecx;ecx = numberD
                 mov  esi,[numberN];esi = [numberN]
                 dec  ecx;ecx = ������� ��� ���� ��������

              ;�������� iCRC32ForKey �������� �������� ������� iCRC32ForKey � �������  numberD �� ������ numberN
              ;���������� ����� ���� word �� ���� ���, ��� �� ��������� ������ ����� �� ���� ������ ��� numberN
              ;ecx = ������� ��� ���� �������� ;esi = [numberN]
               movzx  ebx,word[aKeyFile.iCRC32ForKey]
                 mov  edi,ecx;edi = ������� ��� ���� ��������
                 mov  eax,ebx;ebx = word[iCRC32ForKey]

                 @@:
                 mul  ebx;ebx = word[iCRC32ForKey]
                 div  esi;esi = [numberN]
                 mov  eax,edx
                 dec  ecx
                 jnz  @b

                 mov  [aKeyFile.iDigitalSignatureLow],eax
               movzx  ebx,word[aKeyFile.iCRC32ForKey+2]
                 mov  ecx,edi;edi = ������� ��� ���� ��������
                 mov  eax,ebx

                 @@:
                 mul  ebx;ebx = word[iCRC32ForKey+2]
                 div  esi;esi = [numberN]
                 mov  eax,edx
                 dec  ecx
                 jnz  @b

                 mov  [aKeyFile.iDigitalSignatureHiw],eax


                ;���������� ��������� ���� ��� ����������� �� ������ � ��������� ���
                 mov  ebx,[numberN];ebx = [numberN]
             stdcall  HexToUnicodHL,ebx,aOpenKey.MessageN;ebx = [numberN]

              invoke  SendMessageW,[hwndEditOpenKey],WM_SETTEXT,eax,aOpenKey;eax - ���� �� ������
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���


                ;��������� ��� � ������� ����� [numberN], ��� �� �� ���� ���� ����� � � ������ � ��������, � ��� �� ������ ��� ����� ����� ���, �.�. ����� ������
                ;ebx = [numberN]
                 mov  esi,aKeyFile;esi = aKeyFile
                 mov  eax,sizeof.KEYFILEW.szNameUser/2
                 mov  ecx,[esi+KEYFILEW.lengthszNameUser]
                 lea  edx,[esi+KEYFILEW.szNameUser]
                 sub  eax,ecx;eax = ������ � �������� ������� ���� ��������

                 @@:;������
                 xor  word[edx],bx
                 add  edx,2
                 dec  ecx
                 jnz  @b

                 xor  ebx,ebx;ebx = 0

                 @@:;�������� �����
                 mov  word[edx],bx
                 add  edx,2
                 dec  eax
                 jnz  @b

                ;���������� CRC32c ����� � ��������� ��� ����������� ���� ��� �� �� ��� �������
                ;esi = aKeyFile;ebx = 0;ebp = ����� �������
                 lea  eax,[esi+sizeof.KEYFILEW.iCRC32File];esi = aKeyFile
             stdcall  CRC32cHL,eax,sizeof.KEYFILEW - sizeof.KEYFILEW.iCRC32File,-1
                 mov  [esi+KEYFILEW.iCRC32File],eax;esi = aKeyFile



                ;��� - ��� ������ ��� ���� ��� �� ��������� �������� � ���� � ���� ���� �����
                ;esi = aKeyFile;ebx = 0;ebp = ����� �������
              invoke  CreateFileW,szFileName,FILE_GENERIC_WRITE or FILE_GENERIC_READ,ebx,ebx,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,ebx;ebx = 0
                 mov  edi,eax;edi = ����� �����
                 inc  eax;INVALID_HANDLE_VALUE
                  jz  .ErrorFileSave;�� �������� ������� ����;ebp = ����� �������;ebx = ����


              invoke  WriteFile,edi,esi,sizeof.KEYFILEW,iNumberOfBytesWritten,ebx;edi = ����� �����;ebx = ����;esi = aKeyFile
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

                 cmp  [iNumberOfBytesWritten],sizeof.KEYFILEW
                 jne .ErrorFileSave;�� ��������� ������ � ��� ������� ������ ���� ������� � ����;ebp = ����� �������;ebx = ����

                 jmp  .True






;�� �������� ������� ����
;ebp = ����� ������� ;ebx = ����
     .ErrorFileSave:
                 mov  esi,szErrorFileSave
                 jmp  .MessageBox

;ErrornumberD - ����� �� ������� 32 ����
;ebp = ����� ������� ;ebx = ����
      .ErrornumberD:
                 mov  esi,szErrorErrornumberD
                 jmp  .MessageBox

;ErrornumberN - ����� �� ������� 32 ����
;ebp = ����� ������� ;ebx = ����
      .ErrornumberN:
                 mov  esi,szErrorErrornumberN
                 jmp  .MessageBox


;��� ������� ����� �� ���� ������� ...
;ebp = ����� ������� ;ebx = ����
        .ErrorSimple:
                 mov  esi,szErrorSimple
                 jmp  .MessageBox


;�� ���������� ������ �������� ������
;ebp = ����� ������� ;ebx = ����
.ErrorNotFormatData:
                 mov  esi,szBanerEdiSet
                 mov  ebx,szErrorTextNotFormatData
                 jmp  .MessageBox


       ;�� ���� � ������ ������
       ;ebp = ����� ������� ;ebx = ����
           .NotText:
                 mov  esi,szErrorTextNot
                 jmp  .MessageBox

       ;������� ����� ������ � ������
       ;ebp = ����� ������� ;ebx = ����
       .ErrorBuffer:
                 mov  esi,szErrorTextLong

        .MessageBox:
              invoke  MessageBoxExW,ebp,esi,ebx,MB_OK or MB_ICONINFORMATION,ebx;ebp = ����� ������� ;ebx = ���� ��� ���������; esi = ���������
                 jmp  .True

;������������� ������
;ebp = ����� ������� ;ebx = ����
;--------------------------------------
     .WM_INITDIALOG:
              ;��������� �����(�����) ������������ � ��������� � ������� ������ ���� Edit
              ;ebp = ����� ������� ;ebx = ����
                 mov  esi,[GetDlgItem];esi = [GetDlgItem]
             stdcall  esi,ebp,DIALOG_MAIN_EDIT_SET_Id;ebp = ����� �������;esi = [GetDlgItem]
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
                 mov  [hwndEditSet],eax
                 mov  [hwnd],ebp

               invoke  SendMessageW,eax,EM_SETCUEBANNER,ebx,szBanerEdiSet;ebx = ����
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���


             stdcall  esi,ebp,DIALOG_MAIN_EDIT_OPENKEY_Id ;ebp = ����� �������;esi = [GetDlgItem]
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
                 mov  [hwndEditOpenKey],eax

                 jmp  .True


;��������� ������
;ebp = ����� ������� ;ebx = ����
;--------------------------------------
              .Exit:
              invoke  EndDialog,ebp,ebx;ebp = ����� ������� ;ebx = ���� -��� �������� ���� �� ������
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

              .True:
                 xor  eax,eax
                 inc  eax
                 ret

endp








include  'Inc\Proc.inc'
;----------------------------------------------
section '.idata' import data readable writeable
include 'Include\My\Import.inc'

;----------------------------------------------
section '.rsrc' resource data readable
  directory RT_MANIFEST,manifest


  resource manifest,\
           Anonymous,LANG_NEUTRAL,manifest_data




  resdata manifest_data
db   '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>',13,10
db   '<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">',13,10
db      "<dependency>",13,10
db        "<dependentAssembly>",13,10
db         "<assemblyIdentity type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*' />",13,10
db        "</dependentAssembly>",13,10
db      "</dependency>",13,10

db   '<compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1">',13,10
  db   '<application>',13,10
       ; <!--This Id value indicates the application supports Windows Vista functionality -->
         db   '<supportedOS Id="{e2011457-1546-43c5-a5fe-008deee3d3f0}"/>',13,10
        ;<!--This Id value indicates the application supports Windows 7 functionality-->
         db   '<supportedOS Id="{35138b9a-5d96-4fbd-8e2d-a2440225f93a}"/>',13,10
        ;<!--This Id value indicates the application supports Windows 8 functionality-->
         db   '<supportedOS Id="{4a2f28e3-53b9-4441-ba9c-d69d4a4a6e38}"/>',13,10
        ;<!--This Id value indicates the application supports Windows 8.1 functionality-->
         db   '<supportedOS Id="{1f676c76-80e1-4239-95bb-83d0f6d0da78}"/>',13,10
        ;<!--This Id value indicates the application supports Windows 10 functionality-->
         db   '<supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}"/>',13,10
  db   '</application>',13,10
db   '</compatibility>',13,10
db   '</assembly>',13,10
  endres



;������ ����
;----------------------------------------------
dirIndirect tbRussian,\
            RT_DLG,Dialog

resIndirect Dialog,\
            DIALOG_MAIN_ID,dialog_main

 dialogIndirect dialog_main,<'��������� ����� ���: ',equNameApp>,0,0,400,100,WS_CAPTION or WS_POPUP or WS_DLGFRAME or DS_MODALFRAME or WS_CLIPCHILDREN,0,0,'Segoe UI',12
  dialogitemIndirect 'BUTTON','Ok',DIALOG_MAIN_BUTTON_OK_Id,360,87,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON or WS_GROUP
  dialogitemIndirect 'BUTTON','�����',DIALOG_MAIN_BUTTON_GENERATE_Id,360,70,40,13,WS_VISIBLE or WS_TABSTOP  or BS_PUSHBUTTON
  dialogitemIndirect 'EDIT','',DIALOG_MAIN_EDIT_SET_Id,1,70,188,13,WS_VISIBLE or WS_TABSTOP or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'EDIT','',DIALOG_MAIN_EDIT_OPENKEY_Id,190,70,169,13,WS_VISIBLE or WS_BORDER  or ES_CENTER or ES_AUTOHSCROLL or ES_AUTOVSCROLL or ES_READONLY
  dialogitemIndirect 'STATIC','������ ��� ��������� ���������:',NULL,3,57,160,13,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','��������� ����:',NULL,192,57,80,13,WS_VISIBLE  or SS_LEFT
 enddialogIndirect

PolinomCRC32 8,0x82F63B78

sKeyRules                    duA  equVersionKeyRules,equNameApp

szFileName                   duA  equNameApp,'.kss',0

szBanerEdiSet                duA  '���� �������� - ivan@example.com ',0

szErrorFileSave              duA  '�� �������� ��������� �������������� ���� � ����',0
szErrorErrornumberD          duA  '����� D ����� �� ������� 32 ����',0
szErrorErrornumberN          duA  '����� N ����� �� ������� 32 ����',0
szErrorSimple                duA  '��� ������� ����� �� ���� ������� ...',0
szErrorTextLong              duA  '������� ����� ������, � ������ ������',0
szErrorTextNot               duA  '������� ������',0
szErrorTextNotFormatData     duA  '��� ������ ����������� �����'

section '.reloc' data fixups readable


