format PE GUI 4.0
entry Start

;equGlAssembly equ Debug; покуда указана у нас отладочная версия

include  'Include\Win32W.inc'
include  'Inc\ConstEquates.inc'

 enum  1,\;Начальное значение
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
_DebugDate; для отладки в релезе не сохраниться

aVectoredExceptionHandler               VectoredExceptionHandler ;структура для исключений

aKeyFile                     KEYFILEW;Структура для сохранения ключа в файл

aOpenKey                     OPENKEYW;структура для отображения N числа


hwnd                         ddA  ?;Окно которое являеться главным и для привязки когда вылет для отображения текста например при SEH
hwndEditSet                  ddA  ?
hwndEditOpenKey              ddA  ?

iNumberOfBytesWritten        ddA  ?
hInstance                    ddA  ?


numberN                      ddA  ?;Числа для RSA
numberP                      ddA  ?;Числа для RSA
numberQ                      ddA  ?;Числа для RSA
numberFN                     ddA  ?;Числа для RSA
;numberE                      ddA  ?;Числа для RSA
numberD                      ddA  ?;Числа для RSA


;----------------------------------------------
section '.text' code readable executable
              ; _VTuneMini
              ; _EndVTuneMini

Start:

;SEH
;----------------------------------------------
           xor  ebx,ebx;ebx = 0
        invoke  SetUnhandledExceptionFilter,UnhandledExceptionFilter;устанавливаем для что бы ловить все исключения

;Находим базу модуля - hInstance
;ebx = 0
;----------------------------------------------
        invoke  GetModuleHandleW,ebx;ebx = 0
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [hInstance],eax


;Запускаем диалог как главное окно
;ebx = 0
;----------------------------------------------
       stdcall  LoadResoursIndirectHL,tbRussian,RT_DLG,DIALOG_MAIN_ID
        invoke  DialogBoxIndirectParamW,[hInstance],eax,ebx,DlgProcMain,ecx;ecx = без разницы;ebx = 0
                _LastErrorNotSigned;Если не минус возращает функция то ошибки нет

        invoke  ExitProcess,eax



 proc DlgProcMain [esi edi ebp ebx],!HwndDlg,!Wmsg,!Wparam,!Lparam
                 mov  ebp,[!HwndDlg];ebp = Хендл диолога
                 mov  eax,[!Wmsg];eax = сообщение
                 mov  ecx,[!Wparam];ecx = !Wparam
                 mov  edx,[!Lparam];edx = !Lparam
                 xor  ebx,ebx;ebx = ноль


;ebp = Хендл диолога ;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------
                 cmp  eax,WM_COMMAND
                  je  .WM_COMMAND
                 cmp  eax,WM_INITDIALOG
                  je  .WM_INITDIALOG



             .False:
                 xor  eax,eax
                 ret



;Обрабатываем сообщения от Control, Accelerator, Menu
;ebp = Хендл диолога ;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------
        .WM_COMMAND:
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_MAIN_BUTTON_OK_Id;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .Exit;ebp = Хендл диолога ;ebx = ноль
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_MAIN_BUTTON_GENERATE_Id;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .DIALOG_MAIN_BUTTON_GENERATE_Id;ebp = Хендл диолога ;ebx = ноль

                 jmp  .True


;Попробуем с генерировать серийник и открытый ключ
;ebp = Хендл диолога ;ebx = ноль
;--------------------------------------
.DIALOG_MAIN_BUTTON_GENERATE_Id:

              ;Получим текст, если конечно он в виден
              ;ebp = Хендл диолога ;ebx = ноль
                 mov  esi,aKeyFile.szNameUser;esi = aKeyFile.szNameUser
              invoke  SendMessageW,[hwndEditSet],WM_GETTEXT,sizeof.KEYFILEW.szNameUser/2,esi;esi = aKeyFile.szNameUser
                 ;eax = leng text in simbols
                test  eax,eax
                  jz  .NotText;ebp = Хендл диолога ;ebx = ноль
                 cmp  eax,sizeof.KEYFILEW.szNameUser/2-4;Так как возвращаеться количество символах без учета нуля, поэтому поверяем если равно или больше значит что то не то
                 jae  .ErrorBuffer;ebp = Хендл диолога ;ebx = ноль



              ;Проверим соответствую ли введеные даные эталону
              ;esi = aKeyFile.szNameUser;ebx = ноль;eax = leng text in simbols;ebp = Хендл диолога
                 mov  edx,-2
                 xor  ecx,ecx

           ;Проверяем на наличие двух пробелов и  - , после второго тем самым косвенно введена ли фамилия с именем
           .LoopCmp:
                 xor  edi,edi
                  or  di,word[esi+ecx*2];esi = aKeyFile.szNameUser
                  jz  .ErrorNotFormatData;Если ноль выйдим с цикла - даные не соотвентствуют

                 cmp  di,' '
                  je  .SpaceCmp; если пробел значит нужно пометить это

                 inc  ecx
                 jmp  .LoopCmp

          .SpaceCmp:
                 inc  ecx

                 inc  edx; помечаем что пробел
                  jz  .EndLoopCmp

                 jmp  .LoopCmp

        .EndLoopCmp:
                 ; если после второго пробела не '-', значит не тот формат сообщаем
                 cmp  word[esi+ecx*2],'-';esi = aKeyFile.szNameUser
                 jne  .ErrorNotFormatData;ebp = Хендл диолога ;ebx = ноль

                 lea  eax,[ecx-1]
                 inc  ecx
                 mov  [aKeyFile.lengthszNameUser],eax;сохраняем длину в символах без нуля фамилии и имяни пользователя для сохранения в файл

              ;Проверим соответствую ли введеные даные эталону ввиден ли емайл
              ;esi = aKeyFile.szNameUser;ebx = ноль;eax = leng text in simbols;ebp = Хендл диолога
          .LoopCmp2:
                  ;edx = 0= - иначе бы сюда не попали бы
                  or  dx,word[esi+ecx*2];esi = aKeyFile.szNameUser
                  jz  .ErrorNotFormatData;Если ноль выйдим с цикла - даные не соотвентствуют

                 cmp  dx,'@'
                  je  .AllGood;если пробел значит нужно пометить это

                 xor  edx,edx
                 inc  ecx
                 jmp  .LoopCmp2




              ;Превратим в веденые пользователем даннные в одно число уникальное с помощью CRC32
              ;esi = aKeyFile.szNameUser;ebx = ноль;eax = leng text in simbols
           .AllGood:
                 shl  eax,1;eax = leng text in simbols
                 mov  edi,CRC32cHL;edi = CRC32cHL
             stdcall  edi,esi,eax,-1;eax = leng text in simbols;esi = aKeyFile.szNameUser;edi = CRC32cHL
                 mov  [aKeyFile.iCRC32ForKey],eax ;CRC32c - войдет в серийник и именно его подпишем RSA


;Все теперь можно начинать генерировать серийный номерь на основании ;eax = CRC32c и sKeyRules(версия программы к которой делаеться серийник)
;Благодоря этим двум данным серийник будет уникален как для пользователя так и для программы каждой по отдельности и также версии, при изменении ее само собой
;RSA-32 - используеться, для моего уровня теперешнего самое то, так как всеравно программу через отладчик можно взламать
;--------------------------------------
             ;edi = CRC32cHL
             ;Получим на основании sKeyRules - CRC32 на основании котого получим число n - numberN
             stdcall  edi,sKeyRules,sizeof.sKeyRules,-1;eax = leng text in simbols;esi = aKeyFile.szNameUser;edi = CRC32cHL
                 mov  esi,eax;esi = сохраняем numberN

                ;Так как версия RSA простоя провирать тожде будем по простому на простоту деля все числа на  числа от  3  до 2001
                ;Находим число numberP
                 mov  ecx,eax
                 shr  ecx,16
                 mov  edi,3
                  or  ecx,1;делаем нечетным numberP

                 ;Проверяем на простоту
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
                 jbe  .ErrorSimple;ebp = Хендл диолога ;ebx = ноль
                 mov  [numberP],ecx;numberP


                ;Находим число numberQ
                 mov  eax,esi;esi = сохраняем numberN
                 xor  edx,edx
                 mov  esi,ecx;esi = numberP
                 div  ecx
                 mov  edi,3
                 mov  ecx,eax;numberQ
                  or  ecx,1;numberQ

                 ;Проверяем на простоту
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
                 jbe  .ErrorSimple;ebp = Хендл диолога ;ebx = ноль
                 mov  [numberQ],ecx;numberQ

              ;На основании numberQ и numberP получим истинное numberN
              ;esi = numberP;ecx = numberQ;ebp = Хендл диолога ;ebx = ноль
                 mov  eax,ecx;ecx = numberQ
                 mul  esi
                 mov  [numberN],eax
                test  edx,edx
                 jnz  .ErrornumberN;ebp = Хендл диолога ;ebx = ноль

              ;На основании numberQ и numberP получим  numberFN
              ;esi = numberP;ecx = numberQ;ebp = Хендл диолога ;ebx = ноль
                 lea  eax,[esi-1];esi = numberP
                 dec  ecx;ecx = numberQ
                 mul  ecx
                 mov  [numberFN],eax
                 mov  esi,eax;esi = numberFN

              ;Найдем numberD на основании тупово перебора
              ;eax = numberFN ;esi = numberFN;numberE = equNumberE;ebp = Хендл диолога ;ebx = ноль
                 mov  edi,constNumberE;edi = equNumberE
                 div  edi
                  or  eax,1
                 mov  ecx,eax;ecx = numberD

                 @@:
                 mul  edi;edi = constNumberE
                 div  esi;esi = numberFN
                 cmp  edx,1
                  je  @f;Нашли то что надо супер;eax = numberD

                 add  ecx,2
                 mov  eax,ecx
                 cmp  ecx,0xFFFFFFFC
                 jae  .ErrornumberD;ebp = Хендл диолога ;ebx = ноль
                 jmp  @b

                 @@:
                 mov  [numberD],ecx;ecx = numberD
                 mov  esi,[numberN];esi = [numberN]
                 dec  ecx;ecx = столько раз надо умножать

              ;Подпишем iCRC32ForKey цифровой подписью возведя iCRC32ForKey в степень  numberD по модолю numberN
              ;Подписвать будет один word за один раз, что бы навернека взятое число не было больше чем numberN
              ;ecx = столько раз надо умножать ;esi = [numberN]
               movzx  ebx,word[aKeyFile.iCRC32ForKey]
                 mov  edi,ecx;edi = столько раз надо умножать
                 mov  eax,ebx;ebx = word[iCRC32ForKey]

                 @@:
                 mul  ebx;ebx = word[iCRC32ForKey]
                 div  esi;esi = [numberN]
                 mov  eax,edx
                 dec  ecx
                 jnz  @b

                 mov  [aKeyFile.iDigitalSignatureLow],eax
               movzx  ebx,word[aKeyFile.iCRC32ForKey+2]
                 mov  ecx,edi;edi = столько раз надо умножать
                 mov  eax,ebx

                 @@:
                 mul  ebx;ebx = word[iCRC32ForKey+2]
                 div  esi;esi = [numberN]
                 mov  eax,edx
                 dec  ecx
                 jnz  @b

                 mov  [aKeyFile.iDigitalSignatureHiw],eax


                ;Преобразим публичный ключ для отображения на экране и отобразим его
                 mov  ebx,[numberN];ebx = [numberN]
             stdcall  HexToUnicodHL,ebx,aOpenKey.MessageN;ebx = [numberN]

              invoke  SendMessageW,[hwndEditOpenKey],WM_SETTEXT,eax,aOpenKey;eax - роли не играет
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


                ;Проксорим имя и фамилию через [numberN], что бы не было явно видно и в вадило в путаницу, а так же затрем все даные после них, т.е. имейл затрем
                ;ebx = [numberN]
                 mov  esi,aKeyFile;esi = aKeyFile
                 mov  eax,sizeof.KEYFILEW.szNameUser/2
                 mov  ecx,[esi+KEYFILEW.lengthszNameUser]
                 lea  edx,[esi+KEYFILEW.szNameUser]
                 sub  eax,ecx;eax = размер в символах который надо затереть

                 @@:;ксорим
                 xor  word[edx],bx
                 add  edx,2
                 dec  ecx
                 jnz  @b

                 xor  ebx,ebx;ebx = 0

                 @@:;затираем нулем
                 mov  word[edx],bx
                 add  edx,2
                 dec  eax
                 jnz  @b

                ;Подсчитаем CRC32c файла с серийника для потвержения того что он не был изменен
                ;esi = aKeyFile;ebx = 0;ebp = Хендл диолога
                 lea  eax,[esi+sizeof.KEYFILEW.iCRC32File];esi = aKeyFile
             stdcall  CRC32cHL,eax,sizeof.KEYFILEW - sizeof.KEYFILEW.iCRC32File,-1
                 mov  [esi+KEYFILEW.iCRC32File],eax;esi = aKeyFile



                ;Все - все готово для того что бы сохранить серийник в виде в виде спец файла
                ;esi = aKeyFile;ebx = 0;ebp = Хендл диолога
              invoke  CreateFileW,szFileName,FILE_GENERIC_WRITE or FILE_GENERIC_READ,ebx,ebx,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,ebx;ebx = 0
                 mov  edi,eax;edi = хендл файла
                 inc  eax;INVALID_HANDLE_VALUE
                  jz  .ErrorFileSave;не удаеться создать файл;ebp = Хендл диолога;ebx = ноль


              invoke  WriteFile,edi,esi,sizeof.KEYFILEW,iNumberOfBytesWritten,ebx;edi = хендл файла;ebx = ноль;esi = aKeyFile
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 cmp  [iNumberOfBytesWritten],sizeof.KEYFILEW
                 jne .ErrorFileSave;не совпадает размер с тем который должен быть записан в файл;ebp = Хендл диолога;ebx = ноль

                 jmp  .True






;не удаеться создать файл
;ebp = Хендл диолога ;ebx = ноль
     .ErrorFileSave:
                 mov  esi,szErrorFileSave
                 jmp  .MessageBox

;ErrornumberD - вышло за границы 32 бита
;ebp = Хендл диолога ;ebx = ноль
      .ErrornumberD:
                 mov  esi,szErrorErrornumberD
                 jmp  .MessageBox

;ErrornumberN - вышло за границы 32 бита
;ebp = Хендл диолога ;ebx = ноль
      .ErrornumberN:
                 mov  esi,szErrorErrornumberN
                 jmp  .MessageBox


;Упс простое число не было найдено ...
;ebp = Хендл диолога ;ebx = ноль
        .ErrorSimple:
                 mov  esi,szErrorSimple
                 jmp  .MessageBox


;Не правильный формат ввиденых данных
;ebp = Хендл диолога ;ebx = ноль
.ErrorNotFormatData:
                 mov  esi,szBanerEdiSet
                 mov  ebx,szErrorTextNotFormatData
                 jmp  .MessageBox


       ;Не было в видино данных
       ;ebp = Хендл диолога ;ebx = ноль
           .NotText:
                 mov  esi,szErrorTextNot
                 jmp  .MessageBox

       ;Слишком много данных в видино
       ;ebp = Хендл диолога ;ebx = ноль
       .ErrorBuffer:
                 mov  esi,szErrorTextLong

        .MessageBox:
              invoke  MessageBoxExW,ebp,esi,ebx,MB_OK or MB_ICONINFORMATION,ebx;ebp = Хендл диолога ;ebx = ноль или сообщение; esi = сообщение
                 jmp  .True

;Иницилизируем диалог
;ebp = Хендл диолога ;ebx = ноль
;--------------------------------------
     .WM_INITDIALOG:
              ;Установим банер(текст) мотивирующий к действиям и получим хендлы всех Edit
              ;ebp = Хендл диолога ;ebx = ноль
                 mov  esi,[GetDlgItem];esi = [GetDlgItem]
             stdcall  esi,ebp,DIALOG_MAIN_EDIT_SET_Id;ebp = Хендл диолога;esi = [GetDlgItem]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 mov  [hwndEditSet],eax
                 mov  [hwnd],ebp

               invoke  SendMessageW,eax,EM_SETCUEBANNER,ebx,szBanerEdiSet;ebx = ноль
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


             stdcall  esi,ebp,DIALOG_MAIN_EDIT_OPENKEY_Id ;ebp = Хендл диолога;esi = [GetDlgItem]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 mov  [hwndEditOpenKey],eax

                 jmp  .True


;Закрываем диолог
;ebp = Хендл диолога ;ebx = ноль
;--------------------------------------
              .Exit:
              invoke  EndDialog,ebp,ebx;ebp = Хендл диолога ;ebx = ноль -Код вазврата роли не играет
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

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



;Руский язык
;----------------------------------------------
dirIndirect tbRussian,\
            RT_DLG,Dialog

resIndirect Dialog,\
            DIALOG_MAIN_ID,dialog_main

 dialogIndirect dialog_main,<'Генератор ключа для: ',equNameApp>,0,0,400,100,WS_CAPTION or WS_POPUP or WS_DLGFRAME or DS_MODALFRAME or WS_CLIPCHILDREN,0,0,'Segoe UI',12
  dialogitemIndirect 'BUTTON','Ok',DIALOG_MAIN_BUTTON_OK_Id,360,87,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON or WS_GROUP
  dialogitemIndirect 'BUTTON','Старт',DIALOG_MAIN_BUTTON_GENERATE_Id,360,70,40,13,WS_VISIBLE or WS_TABSTOP  or BS_PUSHBUTTON
  dialogitemIndirect 'EDIT','',DIALOG_MAIN_EDIT_SET_Id,1,70,188,13,WS_VISIBLE or WS_TABSTOP or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'EDIT','',DIALOG_MAIN_EDIT_OPENKEY_Id,190,70,169,13,WS_VISIBLE or WS_BORDER  or ES_CENTER or ES_AUTOHSCROLL or ES_AUTOVSCROLL or ES_READONLY
  dialogitemIndirect 'STATIC','Данные для генерации серийника:',NULL,3,57,160,13,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','Публичный ключ:',NULL,192,57,80,13,WS_VISIBLE  or SS_LEFT
 enddialogIndirect

PolinomCRC32 8,0x82F63B78

sKeyRules                    duA  equVersionKeyRules,equNameApp

szFileName                   duA  equNameApp,'.kss',0

szBanerEdiSet                duA  'Иван Иванович - ivan@example.com ',0

szErrorFileSave              duA  'Не удаеться сохранить сгенерированый ключ в файл',0
szErrorErrornumberD          duA  'Число D вышло за границы 32 бита',0
szErrorErrornumberN          duA  'Число N вышло за границы 32 бита',0
szErrorSimple                duA  'Упс простое число не было найдено ...',0
szErrorTextLong              duA  'Слишком много данных, в видите меньше',0
szErrorTextNot               duA  'Введите данные',0
szErrorTextNotFormatData     duA  'Вот пример правильного ввода'

section '.reloc' data fixups readable


