format PE GUI 4.0
;heap  0x20000,0x4000
entry Start
;stack 0x3840f0,3000h

;equGlAssembly equ Debug; покуда указана у нас отладочная версия
equGuardian   equ Guardian;Покуда указано действуют методы защиты некторорые через макросы и не только
;----------------------------------------------
include  'Include\Win32W.inc'

;----------------------------------------------
include  'Inc\ConstEquates.inc'
ArgNtSetDebugFilterState       uniq  NtSetDebugFilterState;Что бы чуток попугать в импорте будет но мной не используеться
ArgNtSetInformationProcess     uniq  NtSetInformationProcess;Что бы чуток попугать в импорте будет но мной не используеться
ArgNtSetInformationThread      uniq  NtSetInformationThread;Что бы чуток попугать в импорте будет но мной не используеться
ArgNtGetContextThread          uniq  NtGetContextThread;Что бы чуток попугать в импорте будет но мной не используеться
ArgNtQueryObject               uniq  NtQueryObject;Что бы чуток попугать в импорте будет но мной не используеться
ArgNtQuerySystemInformation    uniq  NtQuerySystemInformation ;Что бы чуток попугать в импорте будет но мной не используеться


constSetTimer                  uniq  SetTimer - Start.GetProcessHeap;Смещение до функции SetTimer  относительно Start тем самым  хоть както замаскируем
constLoadLibraryExW            uniq  LoadLibraryExW - Start.GetProcessHeap;Смещение до функции LoadLibraryExW  относительно Start тем самым  хоть както замаскируем
constFreeLibrary               uniq  FreeLibrary - Start.GetProcessHeap;Смещение до функции FreeLibrary  относительно Start тем самым  хоть както замаскируем
constTimerEmptyGuardian        uniq  TimerEmptyGuardian;Что бы существовала функция обманка


;----------------------------------------------
include  'Inc\Data.inc'

;----------------------------------------------
section '.text' code readable executable
              ; _VTuneMini
              ; _EndVTuneMini
proc  Start
  locals
   !ppszPath              dd  ?;Roaming
   !NumberOfBytesRead     dd  ?;ReadFile - size read
   !aSizeFile             !LARGE_INTEGER; сюда получаем размер ini файла
   !aAddTcItem            TCITEMW;add item for SysTab Control
   !aAddColum             LVCOLUMNW
   !aAddItem              LVITEMW
   !aCommonLVGROUP        LVGROUPW
   !aBufferString         du  tbKeylengthMax dup(?)
  endl


;SEH
;----------------------------------------------
           xor  ebx,ebx ;ebx = 0
        invoke  SetUnhandledExceptionFilter,UnhandledExceptionFilter;устанавливаем для что бы ловить все исключения;SetUnhandledExceptionFilter;AddVectoredExceptionHandler

;Чтобы не запустилось больше одного приложения в одной сесии
;ebx = 0
;----------------------------------------------
        invoke  CreateEventExW,ebx,szEventOneApp,ebx,SYNCHRONIZE or EVENT_MODIFY_STATE;ebx = 0
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [hEventThread],eax

        invoke  GetLastError
           cmp  eax,ERROR_ALREADY_EXISTS
           jne  .GetProcessHeap;ebx = 0


;Находим окно по зарегестированому классу и выводим его на передний план
;----------------------------------------------
        invoke  FindWindowExW,ebx,ebx,szEventOneApp,ebx;ebx = 0
           mov  ebp,eax;ebp = Хендл окна

        invoke  SetForegroundWindow,eax
        invoke  ShowWindow,ebp,SW_SHOW;ebp = Хендл окна
        .Exit:
        invoke  ExitProcess,eax;eax - роли не играет

;Получаем heap потока
;ebx = 0
;----------------------------------------------
.GetProcessHeap:
        invoke  GetProcessHeap
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [hheap],eax



;Иницилизурием COM  и заадно структуру SHELLEXECUTEINFOW
;ebx = 0
;----------------------------------------------
        invoke  CoInitializeEx,ebx,COINIT_APARTMENTTHREADED or COINIT_DISABLE_OLE1DDE;ebx = 0

;Получаем путь к каталогу Roaming и создаем там папки
;ebx = 0
;----------------------------------------------
           ;находим системную папку Roaming для хранения своих служебных файлов
           lea  eax,[!ppszPath]
        invoke  SHGetKnownFolderPath,aFolderidRoamingAppData,KF_FLAG_CREATE,ebx,eax;ebx = 0
                _LastErrorNull;Если ноль возращает функция то ошибки нет

           mov  ebp,[!ppszPath];ebp = нужно потом освободить память
           mov  ecx,-constMySizeofRoaming/2
           lea  edx,[ebp+constMySizeofRoaming]
           xor  eax,eax
           mov  esi,tbRoaming

           @@:
            or  ax,[edx+ecx*2]
            jz  @f
           mov  word[szRoamingAppData+constMySizeofRoaming+ecx*2],ax
           xor  eax,eax
           inc  ecx
           jnz  @b
           jmp  .SkipSetting

           @@:
           lea  edi,[szRoamingAppData+constMySizeofRoaming+ecx*2]
        invoke  CoTaskMemFree,ebp;освободили память полученого функцией SHGetKnownFolderPath

           mov  ecx,(sizeof.sHLCatalog) shr 1
           mov  ebp,szRoamingAppData
           cld
           rep  movsw
        invoke  CreateDirectoryW,ebp,ebx;ebx = 0 создаем папку в Roaming общую

           mov  ecx,(sizeof.sNameDirectory) shr 1
           cld
           rep  movsw
        invoke  CreateDirectoryW,ebp,ebx;ebx = 0 создаем папку в Roaming для нашей программы

           mov  ecx,sizeof.sMainIniFile shr 1
           cld
           rep  movsw;копируем имя нашего ini файла
           mov  [lpEndRoaming],edi

           mov  esi,[CreateFileW];esi = [CreateFileW]
           mov  edi,[ReadFile];edi = [ReadFile]

;Открываем ksi файл
;esi = [CreateFileW];edi = [ReadFile];ebp = Указатель на буфер с путем к Roaming и далее
;----------------------------------------------
       stdcall  esi,ebp,FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0;esi = [CreateFileW]
           mov  ebp,eax;ebp = хендл нашего ksi файла
           inc  eax;INVALID_HANDLE_VALUE;не удаеться открыть ini файл
            jz  .OpenFileKss;esi = [CreateFileW];edi = [ReadFile]



;читаем файл в память
;esi = [CreateFileW];edi = [ReadFile];ebp = хендл нашего ksi файла
;----------------------------------------------
           lea  eax,[!NumberOfBytesRead]
       stdcall  edi,ebp,aIniFileData,sizeof.INIFILEDATA,eax,0;edi = [ReadFile];ebp = хендл нашего ksi файла
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

        invoke  CloseHandle,ebp;ebp = хендл нашего ini файла
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;CRC32c - 0x82F63B78 -полином
;esi = [CreateFileW];edi = [ReadFile]
;----------------------------------------------
           mov  ebx,aIniFileData;ebx = aIniFileData
       stdcall  CRC32cHL,ebx,sizeof.INIFILEDATA - sizeof.INIFILEDATA.CRC32,-1;ebx = aIniFileData

           ;сравниваем с CRC32c сохраненым в ini файле
           cmp  eax,[ebx+INIFILEDATA.CRC32];ebx = aIniFileData
            je  .OpenFileKss;esi = [CreateFileW];edi = [ReadFile]

       ;если CRC32 не совпадает копируем настройки по дефолту
       stdcall  MoveMemoryHL,ebx,aInitilizDefault,sizeof.INITILIZDATA;ebx = aIniFileData

           ;Обнуляем так как CRC32 не совпадает, что бы не возникли баги иза не действительных данных
           mov  word[ebx+INIFILEDATA.szOpenExeBufer],0;ebx = aIniFileData
           mov  word[ebx+INIFILEDATA.szLaunchfileBufer],0;ebx = aIniFileData



;загружаем файл регестрации
;esi = [CreateFileW];edi = [ReadFile]
;----------------------------------------------
 .OpenFileKss:
       stdcall  esi,szKeyFile,FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0 ;esi = [CreateFileW]
           ;Изменяем расширение для файл карты клавиш

           mov  ebp,eax;ebp = хендл нашего Kss файла
           inc  eax;INVALID_HANDLE_VALUE;не удаеться открыть
            jz  .OpenFileKsm;esi = [CreateFileW];edi = [ReadFile]

           lea  edx,[!NumberOfBytesRead]
           mov  ebx,aKeySerial;ebx = KEYFILEW
       stdcall  edi,ebp,ebx,sizeof.KEYFILEW,edx,0;edi = [ReadFile];ebp = хендл файла;ebx = KEYFILEW
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

        invoke  CloseHandle,ebp;ebp = хендл  файла
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


          ;Проверим CRC32c на целостность файла
          ;esi = [CreateFileW];edi = [ReadFile];ebx = KEYFILEW
           lea  eax,[ebx+sizeof.KEYFILEW.iCRC32File];ebx = KEYFILEW
       stdcall  CRC32cHL,eax,sizeof.KEYFILEW - sizeof.KEYFILEW.iCRC32File,-1
           cmp  [ebx+KEYFILEW.iCRC32File],eax;ebx = KEYFILEW
           jne  .OpenFileKsm;esi = [CreateFileW];edi = [ReadFile]

          ;Расксорим имя и фамилию пользователя купившего прогу
          ;esi = [CreateFileW];edi = [ReadFile];ebx = KEYFILEW
           lea  edx,[ebx+KEYFILEW.szNameUser]
           mov  ecx,[ebx+KEYFILEW.lengthszNameUser];ebx = KEYFILEW
           mov  [lpBuyNameUser],edx

           @@:
           xor  word[edx],constNumberN and 0xFFFF
           add  edx,2
           dec  ecx
           jnz  @b



;загружаем файл карты клавиш
;Загрузка файлов далжно быть всегда в таком порядке, что бы не попутались расширения для сохранения или надо изменить это
;esi = [CreateFileW];edi = [ReadFile]
;----------------------------------------------
 .OpenFileKsm:
           mov  edx,[lpEndRoaming];edx = [lpEndRoaming]
           mov  word[edx-2],'m';edx =  [lpEndRoaming]
       stdcall  esi,szRoamingAppData,FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0;esi = [CreateFileW]
           mov  ebp,eax;ebp = хендл нашего ksm файла
           inc  eax;INVALID_HANDLE_VALUE
            jz  .SkipSetting

           ;Узнаем размер, что бы знать сколько выделить памяти
           lea  eax,[!aSizeFile]
        invoke  GetFileSizeEx,ebp,eax;ebp = хендл  файла
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

           ;Выделяем память
           mov  esi,[!aSizeFile.LowPart];esi = [!aSizeFile.LowPart]
        invoke  HeapAlloc,[hheap],0,esi;esi = [!aSizeFile.LowPart]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  ebx,eax;ebx = FILEALLSTRUCT

       ;Читаем файл
       ;ebx = FILEALLSTRUCT;esi = [!aSizeFile.LowPart];ebp = хендл нашего ksm файла;edi = [ReadFile]
           lea  edx,[!NumberOfBytesRead]
       stdcall  edi,ebp,ebx,esi,edx,0;edi = [ReadFile];ebp = хендл файла;ebx = FILEALLSTRUCT;esi = [!aSizeFile.LowPart]

        invoke  CloseHandle,ebp;ebp = хендл  файла
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          ;Проверим CRC32c на целостность файла
          ;ebx = FILEALLSTRUCT;esi = [!aSizeFile.LowPart]
           sub  esi,sizeof.FILEALLSTRUCT.iCRC32c;esi = [!aSizeFile.LowPart]
            js  .FreeAlloc;ebx = FILEALLSTRUCT;На случай если файл был нулевой
           lea  edx,[ebx+sizeof.FILEALLSTRUCT.iCRC32c];ebx = FILEALLSTRUCT
       stdcall  CRC32cHL,edx,esi,-1;ebx = FILEALLSTRUCT

           ;Проверяем совпала CRC32c или нет
           xor  ecx,ecx
           cmp  [ebx+FILEALLSTRUCT.iCRC32c],eax;ebx = FILEALLSTRUCT;esi = [!aSizeFile.LowPart]
        cmovne  ecx,esp

           ;Проверим совпадает версия правил или нет
           cmp  [ebx+FILEALLSTRUCT.iVersionRules],equVersionKsmRules;ebx = lpFILEALLSTRUCT
        cmovne  ecx,esp

          ;Если не равно нулю значит сохранеения клавиш не подходящие
          test  ecx,ecx
           jnz  .FreeAlloc;ebx = FILEALLSTRUCT

          ;Подготовим структуры групп и перенесем
          ;ebx = FILEALLSTRUCT
           mov  ebp,[ebx+FILEALLSTRUCT.icountGroup]
           lea  esi,[ebx+sizeof.FILEALLSTRUCT];esi = fileGROUPLISTVIEW

  .LoopGrooup:
           dec  ebp;ebp = [icountGroup]
            js  .BildItemStruct

           mov  eax,[esi+GROUPLISTVIEW.lengthNameGroup];esi = fileGROUPLISTVIEW
           lea  edi,[eax*2+sizeof.GROUPLISTVIEW];edi = sizeof.full.GROUPLISTVIEW

        invoke  HeapAlloc,[hheap],0,edi;edi = sizeof.full.GROUPLISTVIEW
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          xchg  eax,edi;edi = newGROUPLISTVIEW;eax = sizeof.full.GROUPLISTVIEW
           mov  edx,esi;esi = fileGROUPLISTVIEW
           add  esi,eax;esi = fileGROUPLISTVIEW;eax = sizeof.full.GROUPLISTVIEW

       stdcall  MoveMemoryHL,edi,edx,eax;edi = newGROUPLISTVIEW;eax = sizeof.full.GROUPLISTVIEW

          ;Каректируем ссылки в групе с учетам нового расположения
          ;edi = newGROUPLISTVIEW;esi = fileGROUPLISTVIEW;ebx = FILEALLSTRUCT;ebp = [icountGroup]
           lea  eax,[edi+sizeof.GROUPLISTVIEW];edi = newGROUPLISTVIEW
           mov  edx,aListViewGroup
           mov  ecx,[edx+GROUPLISTVIEW.lpPrevGroup]
           mov  [edx+GROUPLISTVIEW.lpPrevGroup],edi;edi = newGROUPLISTVIEW
           mov  [ecx+GROUPLISTVIEW.lpNextGroup],edi;edi = newGROUPLISTVIEW
           mov  [edi+GROUPLISTVIEW.lpszNameGroup],eax;edi = newGROUPLISTVIEW
           mov  [edi+GROUPLISTVIEW.lpPrevGroup],ecx;edi = newGROUPLISTVIEW
           mov  [edi+GROUPLISTVIEW.lpNextGroup],edx;edi = newGROUPLISTVIEW
           jmp  .LoopGrooup



          ;Подготовим структуры итемав  и перенесем
          ;ebx = FILEALLSTRUCT;esi = fileITEMLISTVIEW
.BildItemStruct:
           mov  ebp,[ebx+FILEALLSTRUCT.icountItem]

    .LoopItem:
           dec  ebp;ebp = [icountItem]
            js  .FreeAlloc

           xor  eax,eax
           mov  edi,sizeof.ITEMLISTVIEW
           cmp  [esi+ITEMLISTVIEW.vkCodeSubOne],VK_LAST;esi = fileITEMLISTVIEW
         cmova  eax,[esi+ITEMLISTVIEW.scanCodeSubTwoThree];esi = fileITEMLISTVIEW ;размер доп структуры в байтах
           add  edi,eax;edi = sizeof.full.ITEMLISTVIEW

        invoke  HeapAlloc,[hheap],0,edi;edi = sizeof.full.ITEMLISTVIEW
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          xchg  eax,edi;edi = newITEMLISTVIEW;eax = sizeof.full.ITEMLISTVIEW
           mov  edx,esi;esi = fileITEMLISTVIEW
           lea  ecx,[edi+sizeof.ITEMLISTVIEW]
           cmp  eax,sizeof.ITEMLISTVIEW;eax = sizeof.full.ITEMLISTVIEW
         cmove  ecx,[edx+ITEMLISTVIEW.vkCodeSubTwoThree];Если vkCodeSubOne больше чем VK_LAST, тогда vkCodeSubTwoThree указатель на доп данные - каректируем
           add  esi,eax;esi = fileITEMLISTVIEW;eax = sizeof.full.ITEMLISTVIEW
           mov  [edx+ITEMLISTVIEW.vkCodeSubTwoThree],ecx

       stdcall  MoveMemoryHL,edi,edx,eax;edi = newITEMLISTVIEW;eax = sizeof.full.ITEMLISTVIEW

          ;Каректируем ссылки в итеме с учетам нового расположения
          ;edi = newITEMLISTVIEW;esi = fileITEMLISTVIEW;ebx = FILEALLSTRUCT;ebp = [icountItem]
           mov  edx,aListViewItem
           mov  ecx,[edx+ITEMLISTVIEW.lpPrevItem]
           mov  [edx+ITEMLISTVIEW.lpPrevItem],edi;edi = newITEMLISTVIEW
           mov  [ecx+ITEMLISTVIEW.lpNextItem],edi;edi = newITEMLISTVIEW
           mov  [edi+ITEMLISTVIEW.lpPrevItem],ecx;edi = newITEMLISTVIEW
           mov  [edi+ITEMLISTVIEW.lpNextItem],edx;edi = newITEMLISTVIEW
           jmp  .LoopItem

    ;Освобождаем не нужную память
    ;ebx = FILEALLSTRUCT
   .FreeAlloc:
        invoke  HeapFree,[hheap],0,ebx;ebx = FILEALLSTRUCT
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;загружаем языковые файлы
;----------------------------------------------
 .SkipSetting:
           mov  esi,aIniFileData.aIni.Language;esi = aIniFileData.aIni.Language
           xor  ebx,ebx;ebx = 0

       ;За гружает нужный язык по !idlang - id языка для загрузки например COMBOBOX_ENGLISH
       ;Если не удаеться загрузить то eax = 0
       stdcall  LoadLang,[esi];esi = aIniFileData.aIni.Language
          test  eax,eax
           jnz  @f

           ;Если неудаеться загрузить файл языкавой устанавливаем дефолтный
           ;esi = aIniFileData.aIni.Language
           mov  dword[esi],COMBOBOX_ENGLISH;esi = aIniFileData.aIni.Language
           mov  [lpMultiLanguage],tbMultiLanguage

           @@:

;Иницилизурием комон котролы
;ebx = 0
;----------------------------------------------
.InitCommonControls:
           ;Подготовим
           mov  esi,[lpEndRoaming];esi = [lpEndRoaming]
        invoke  InitCommonControlsEx,aInitControlEx


;Создадим файл для проверки отладки
;ebx = 0;esi = [lpEndRoaming]
;----------------------------------------------
           ;Скаректируем расширение  файла
           ;ebx = 0;esi = [lpEndRoaming]
           mov  word[esi-6],'t';esi = [lpEndRoaming]
           mov  word[esi-4],'m';esi = [lpEndRoaming]
           mov  word[esi-2],'p';esi = [lpEndRoaming]

        ;Создадим файл
        ;ebx = ноль
        invoke  CreateFileW,szRoamingAppData,FILE_GENERIC_WRITE or FILE_GENERIC_READ,ebx,ebx,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,ebx;ebx = 0
           mov  ebp,eax;ebp = хендл файла
           inc  eax;INVALID_HANDLE_VALUE
            jz  .SkipTmpFile;ebx = 0

           ;Запишим файл
           lea  eax,[!NumberOfBytesRead]
        invoke  WriteFile,ebp,aGuardianDll,sizeof.aGuardianDll,eax,ebx;ebp = хендл файла;ebx = ноль
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

        ;Закроем хендл файла
        invoke  CloseHandle,ebp;ebp = хендл  файла
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Находим базу модуля - hInstance
;ebx = 0
;----------------------------------------------
 .SkipTmpFile:
        invoke  GetModuleHandleW,ebx;ebx = 0
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

           mov  [hInstance],eax
           mov  ebp,eax;ebp = hInstance

;Создаем дополнительный поток для работы хуков
;!!!Дожен быть создан после того как загружен язык, файл с ключем, файл с настройками и файл с настройкай KeyMap!!!
;ebp = hInstance;ebx = 0
;----------------------------------------------
        invoke  CreateThread,ebx,ebx,ThreadProcForHook,ebx,ebx,iThreadIdHook;ebx = 0
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

        invoke  CloseHandle,eax
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Подготавливаем иконки для ListView
;ebp = hInstance
;----------------------------------------------
           ;Создаем ImageList для состояний итемов
        invoke  ImageList_Create,LIST_VIEW_STATE_Width_Height,LIST_VIEW_STATE_Width_Height,ILC_COLOR32,LIST_VIEW_STATE_MIN_MAX,LIST_VIEW_STATE_MIN_MAX
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [hImageListState],eax
           mov  esi,eax;esi = hImageListState


index 0
          ; Загружаем иконки и устанавливаем в ImageList для состояний итемов
           xor  eax,eax
           mov  edi,[LoadImageW];edi = [LoadImageW]
        indexM  IMAGE_LIST_STATE_NOTCHECK_Id
       stdcall  edi,ebp,IDR_ICON_GROUP_NOTCHECK,IMAGE_ICON,eax,eax,eax;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
          push  eax;для DestroyIcon

           mov  ebx,[ImageList_ReplaceIcon];ebx = [ImageList_ReplaceIcon]
       stdcall  ebx,esi,-1,eax;esi = hImageListState  ;ebx = [ImageList_ReplaceIcon]
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

           xor  eax,eax
        indexM  IMAGE_LIST_STATE_CHECK_Id
       stdcall  edi,ebp,IDR_ICON_GROUP_CHECK,IMAGE_ICON,eax,eax,eax;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
          push  eax;для DestroyIcon

       stdcall  ebx,esi,-1,eax;esi = hImageListState ;ebx = [ImageList_ReplaceIcon]
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

           xor  eax,eax
        indexM  IMAGE_LIST_STATE_QUES_Id
       stdcall  edi,ebp,IDR_ICON_GROUP_QUES,IMAGE_ICON,eax,eax,eax;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
          push  eax ;для DestroyIcon

       stdcall  ebx,esi,-1,eax;esi = hImageListState ;ebx = [ImageList_ReplaceIcon]
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет
endi

         ;Освобождаем ненужные дескрипторы иконок
           mov  esi,[DestroyIcon];esi = [DestroyIcon]
           xor  ebx,ebx;ebx = 0
          call  esi;esi = [DestroyIcon]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          call  esi;esi = [DestroyIcon]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          call  esi;esi = [DestroyIcon]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


;Загружаем иконки для   кнопки приостановить/активировать
;ebx = 0;ebp = hInstance;edi = [LoadImageW]
;----------------------------------------------
        stdcall  edi,ebp,IDR_ICON_GROUP_PAUSE,IMAGE_ICON,ebx,ebx,ebx;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW];ebx = 0
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
            mov  [hiconPause],eax

        stdcall  edi,ebp,IDR_ICON_GROUP_PLAY,IMAGE_ICON,ebx,ebx,ebx;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW];ebx = 0
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
            mov  [hiconPlay],eax
            mov  [aMenuState.ActivSusp],eax;Установим значение для меню MENU_ACTIVATE_Id

;Загружаем иконки для окна,панели задач  и стандартный курсор
;ebx = 0;ebp = hInstance;edi = [LoadImageW]
;----------------------------------------------

           mov  esi,aWc;WNDCLASSEXW
       stdcall  edi,ebp,IDR_ICON_GROUP,IMAGE_ICON,ebx,ebx,LR_DEFAULTCOLOR or LR_DEFAULTSIZE;edi = [LoadImageW];ebp = hInstance;ebx = 0
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

           mov  [esi+WNDCLASSEXW.hInstance],ebp;ebp = hInstance
           mov  [esi+WNDCLASSEXW.hIcon],eax;hIcon
           mov  [esi+WNDCLASSEXW.hIconSm],eax;hIcon
           mov  [aNotifyIconData.hIcon],eax;hIcon  - сохраняем иконку для трея
           mov  [hicon],eax

       ;Курсор стандартный
       stdcall  edi,ebx,IDC_ARROW,IMAGE_CURSOR,ebx,ebx,LR_DEFAULTCOLOR or LR_DEFAULTSIZE or LR_SHARED;edi = [LoadImageW];ebp = hInstance;ebx = 0
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [esi+WNDCLASSEXW.hCursor],eax


;Регистрируем класс основного окна и все необходимые  окна
;ebx = 0;ebp = hInstance;esi = WNDCLASSEXW
;----------------------------------------------
        invoke  RegisterClassExW,esi;esi = WNDCLASSEXW
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                ;eax = атом
           ;Подготовим
           mov  esi,[CreateWindowExW];esi = CreateWindowExW

;основное окно ;esi = CreateWindowExW
       stdcall  esi,WS_EX_ACCEPTFILES or WS_EX_CONTROLPARENT,eax,ebx,WS_CLIPCHILDREN or WS_MAXIMIZEBOX or WS_SYSMENU or WS_OVERLAPPED or WS_SIZEBOX,\
                                ebx,ebx,ebx,ebx,ebx,ebx,ebp,ebx;ebx = 0;ebp = hInstance;eax = атом
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  edi,eax;edi = hwnd
           mov  edx,aShellExecute;edx = aShellExecute
           mov  [hwnd],eax
           mov  [aNotifyIconData.hWnd],eax; - сохраняем хендл окна для трея

           ;Иницилизурием  структуру SHELLEXECUTEINFOW
           ;edx = aShellExecute;edi = hwnd
           mov  [edx+SHELLEXECUTEINFOW.cbSize],sizeof.SHELLEXECUTEINFOW;edx = aShellExecute
           mov  [edx+SHELLEXECUTEINFOW.fMask],SEE_MASK_ASYNCOK;edx = aShellExecute
           mov  [edx+SHELLEXECUTEINFOW.hwnd],edi;edx = aShellExecute;edi = hwnd
           mov  [edx+SHELLEXECUTEINFOW.nShow],SW_SHOWNORMAL;edx = aShellExecute

;кнопка выхода ;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szButtonClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or BS_PUSHBUTTON or BS_CENTER or BS_TEXT,\
                                        ebx,ebx,BUTTON_EXIT_Width,BUTTON_EXIT_Height,edi,BUTTON_EXIT_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [aArrayHwndFont.hwndButtonExit],eax

;кнопка опций ;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szButtonClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or BS_PUSHBUTTON or BS_CENTER or BS_TEXT,\
                                        ebx,ebx,BUTTON_OPTIONS_Width,BUTTON_OPTIONS_Height,edi,BUTTON_OPTIONS_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [aArrayHwndFont.hwndButtonOptions],eax

;кнопка приостановить/активировать ;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szButtonClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or BS_PUSHBUTTON or BS_CENTER or BS_ICON,\
                                        ebx,ebx,BUTTON_ACTIVSUSP_Width,BUTTON_ACTIVSUSP_Height,edi,BUTTON_ACTIVSUSP_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [hwndButtonActivSusp],eax


;ComboBox для выбора языка ;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szComboBoxClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or CBS_DROPDOWNLIST,\
                                        ebx,ebx,COMBOBOX_LANG_Width,ebx,edi,COMBOBOX_LANG_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [aArrayHwndFont.hwndComboBoxLang],eax

;Sys Tab;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szSysTabClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or TCS_HOTTRACK or TCS_BUTTONS or TCS_FLATBUTTONS,\
                                        ebx,2,ebx,ebx,edi,SYSTAB_CONTROL_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [aArrayHwndFont.hwndSysTab],eax

;List View;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szListViewClass,ebx,WS_CHILD or WS_CLIPSIBLINGS or LVS_REPORT or LVS_SHOWSELALWAYS or\
                                         LVS_SINGLESEL or LVS_ALIGNTOP,ebx,ebx,ebx,ebx,edi,LIST_VIEW_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [aArrayHwndFont.hwndListView],eax

;Static для подсказок;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szStatic,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or SS_SUNKEN or SS_NOPREFIX or SS_CENTER,\
                                        ebx,ebx,COMBOBOX_LANG_Width,ebx,edi,STATIC_HELP_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
           mov  [aArrayHwndFont.hwndStatic],eax

;Получаем размер клиентской части главного окна
;ebx = 0;edi = hwnd;ebp = hInstance
;----------------------------------------------
         invoke  GetClientRect,edi,aClientRect;edi = hwnd
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Установим для всего класса диалог иконку
;ebx = 0;edi = hwnd;ebp = hInstance
;----------------------------------------------
         invoke  CreateDialogIndirectParamW,ebp,aDialogEmpty,edi,DlgProcEmpty,ebx;ebx = 0;edi = hwnd;ebp = hInstance
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            mov  esi,eax;esi = [HwndDlg]

         invoke  SetClassLongW,esi,GCLP_HICON,[hicon];esi = [HwndDlg]

         invoke  DestroyWindow,esi;esi = [HwndDlg]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Получим хендл рабочего стола
;ebx = 0;edi = hwnd
;----------------------------------------------
         invoke  GetDesktopWindow
            mov  [hwndDeskop],eax

;Подсчитаем сколько будет будет занимать место текст в About с большими шрифтом
;ebx = 0;edi = hwnd
;----------------------------------------------
         ;Создадим нужный шрифт -временный освободим сразу как не нужен будет
         invoke  CreateFontIndirectW,aFont
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            ;Подготовим
            mov  ebp,eax;ebp = hfont

         ;Получим DC окна главного
         invoke  GetDC,edi;edi = hwnd
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            ;Подготовим
            mov  ebx,eax;ebx = hDc current
            mov  esi,[GetTextExtentPoint32W];esi = [GetTextExtentPoint32W]
            mov  edi,[SelectObject];edi = [SelectObject]

        ;Выбирим шрифт в DC
        ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = hfont
        stdcall  edi,ebx,ebp;edi = [SelectObject];ebx = hDc current;ebp = hfont
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

        ;Подсчитаем ширину и высоту нужную для строки
        ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = hfont
        stdcall  esi,ebx,sNameMainWin,(sizeof.sNameMainWin-4)/2,aAboutRectPaint.NameVer.right;esi = [GetTextExtentPoint32W];ebx = hDc current
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            ;Каректируем
            mov  edx,aAboutRectPaint.NameVer;edx = aAboutRectPaint.NameVer
            mov  eax,ABOUT_BMP_Width_Height
            mov  ecx,[edx+RECT.bottom];edx = aAboutRectPaint.NameVer
            add  [edx+RECT.right],ABOUT_BORDER+ABOUT_BMP_Width_Height;edx = aAboutRectPaint.NameVer
            sub  eax,ecx
            shr  eax,1
            add  eax,SYSTAB_CONTROL_Height+ABOUT_BORDER
            add  ecx,eax
            mov  [edx+RECT.top],eax;edx = aAboutRectPaint.NameVer
            mov  [edx+RECT.bottom],ecx;edx = aAboutRectPaint.NameVer

        ;Подсчитаем ширину и высоту нужную для строки
        ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = hfont
        stdcall  esi,ebx,sCopyright,sizeof.sCopyright/2,aAboutRectPaint.Copyright.right;esi = [GetTextExtentPoint32W];ebx = hDc current
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            ;Каректируем
            mov  edx,aAboutRectPaint.Copyright;edx = aAboutRectPaint.Copyright
            add  [edx+RECT.right],ABOUT_BORDER+ABOUT_BMP_Width_Height;edx = aAboutRectPaint.Copyright
            add  [edx+RECT.bottom],ABOUT_BORDER+ABOUT_BMP_Width_Height+SYSTAB_CONTROL_Height;edx = aAboutRectCopyright

         ;Освобождаем ненужный шрифт
         ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = hfont
         invoke  DeleteObject,ebp;ebp = hfont
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


;Подсчитаем сколько будет будет занимать место текст в About с обычным шрифтом
;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W]
;----------------------------------------------
            ;Подготовим
            mov  edx,aFont;edx = LOGFONTW
            mov  [edx+LOGFONTW.lfHeight],21;edx = LOGFONTW ;Высота - 0x15, средняя ширина- 0x9;Под этот размер подобрана ширина и длина Combox
            mov  [edx+LOGFONTW.lfWeight],FW_NORMAL;edx = LOGFONTW

         ;Создадим основной шрифт - будет использован на постоянной основе
         ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W]
         invoke  CreateFontIndirectW,edx;edx = LOGFONTW
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
            mov  [aSysStock.hfont],eax;eax = [hfont]

        ;Выбирим шрифт в DC
        ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W]
        stdcall  edi,ebx,eax;edi = [SelectObject];ebx = hDc current;eax = [hfont]
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

        ;Подсчитаем ширину и высоту нужную для строки
        ;ebx = hDc current;esi = [GetTextExtentPoint32W]
        stdcall  esi,ebx,szInfoEmail,(sizeof.szInfoEmail-2)/2,aAboutRectPaint.Email.right;esi = [GetTextExtentPoint32W];ebx = hDc current
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            ;Каректируем
            mov  edx,aAboutRectPaint.Email;edx = aAboutRectPaint.Email
            mov  eax,[aAboutRectPaint.Copyright.bottom]
            mov  ecx,[edx+RECT.bottom];edx = aAboutRectPaint.Email
            mov  edi,aAboutZonaButton.Email.Rect;edi =aAboutZonaButton.Email.Rect
            add  eax,ABOUT_INDENT+5;Что бы по вертикали между строчками была пространство для пррисовки рамки
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow],ecx;edi =aAboutZonaButton.Email.Rect;Дляя стрелки сохраняем
            lea  ecx,[ecx*2+ABOUT_BORDER+ABOUT_BMP_Width_Height]
            mov  [edx+RECT.top],eax;edx = aAboutRectPaint.Email
            add  [edx+RECT.right],ecx;edx = aAboutRectPaint.Email
            add  [edx+RECT.bottom],eax;edx = aAboutRectPaint.Email

            ;Дляя стрелки сохраняем
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow+8],eax;edi =aAboutZonaButton.Email.Rect

            ;Подготовим координаты для рамок текста
            mov  ebp,eax;ebp = aAboutRectPaint.Email.top
            mov  ecx,[edx+RECT.right];edx = aAboutRectPaint.Email
            mov  edx,[edx+RECT.bottom];edx = aAboutRectPaint.Email
            sub  eax,ABOUT_INDENT
            add  ecx,ABOUT_INDENT*2
            add  edx,ABOUT_INDENT
            mov  [edi+RECT.top],eax;edi =aAboutZonaButton.Email.Rect
            mov  [edi+RECT.right],ecx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+RECT.bottom],edx;edi =aAboutZonaButton.Email.Rect

            ;Уменьшим на одну единицу так как некоторые функции в частности ExtTextOutW при заливки фона залазят на верхние линии прямойгольника
            dec  eax

            ;Заполним массив точек
            mov  [edi+sizeof.RECT+POINT.y],eax;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.x+sizeof.POINT],ecx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT],eax;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.x+sizeof.POINT*2],ecx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT*2],edx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT*3],edx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT*4],eax

        ;Подсчитаем ширину и высоту нужную для строки
        ;ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = aAboutRectPaint.Email.top
        stdcall  esi,ebx,szInfoRegUrl,(sizeof.szInfoRegUrl-2)/2,aAboutRectPaint.Registry.right;esi = [GetTextExtentPoint32W];ebx = hDc current
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            ;Каректируем
            ;ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = aAboutRectPaint.Email.top
            mov  edx,aAboutRectPaint.Registry;edx = aAboutRectPaint.Registry
            mov  edi,aAboutZonaButton.Registry.Rect;edi =aAboutZonaButton.Registry.Rect
            mov  ecx,[edx+RECT.bottom];edx = aAboutRectPaint.Registry
            mov  eax,[aAboutRectPaint.Email.right]
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow],ecx;edi =aAboutZonaButton.Registry.Rect;Дляя стрелки сохраняем
            add  eax,ABOUT_INDENT*2*2*2;Что бы по горизонтали между строчками была пространство для пррисовки рамки и учитываем еще
            mov  [edx+RECT.top],ebp;edx = aAboutRectPaint.Registry;ebp = aAboutRectPaint.Email.top
            lea  ecx,[ecx*2+eax]
            add  [edx+RECT.bottom],ebp;edx = aAboutRectPaint.Registry;ebp = aAboutRectPaint.Email.top
            add  [edx+RECT.right],ecx;edx = aAboutRectPaint.Registry
            mov  [edx+RECT.left],eax;edx = aAboutRectPaint.Registry

            ;Дляя стрелки сохраняем
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow+4],eax;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow+8],ebp;edi =aAboutZonaButton.Registry.Rect

            ;Подготовим координаты для рамок текста
            ;ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = aAboutRectPaint.Registry.top;eax = [aAboutRectPaint.Registry.left];edi =aAboutZonaButton.Registry.Rect
            mov  ecx,[edx+RECT.right];edx = aAboutRectPaint.Registry
            mov  edx,[edx+RECT.bottom];edx = aAboutRectPaint.Registry
            sub  ebp,ABOUT_INDENT;ebp = aAboutRectPaint.Registry.top
            sub  eax,ABOUT_INDENT*2;eax = [aAboutRectPaint.Registry.left]
            add  ecx,ABOUT_INDENT*2
            add  edx,ABOUT_INDENT
            mov  [edi+RECT.left],eax;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+RECT.top],ebp;edi =aAboutZonaButton.Registry.Rect;ebp = aAboutRectPaint.Registry.top
            mov  [edi+RECT.right],ecx;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+RECT.bottom],edx;edi =aAboutZonaButton.Registry.Rect

            ;Уменьшим на одну единицу так как некоторые функции в частности ExtTextOutW при заливки фона залазят на верхние линии прямойгольника
            dec  eax
            dec  ebp

            ;Заполним массив точек
            mov  [edi+sizeof.RECT+POINT.x],eax;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+sizeof.RECT+POINT.y],ebp;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+sizeof.RECT+POINT.x+sizeof.POINT],ecx;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT],ebp;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+sizeof.RECT+POINT.x+sizeof.POINT*2],ecx;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT*2],edx;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+sizeof.RECT+POINT.x+sizeof.POINT*3],eax;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT*3],edx;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+sizeof.RECT+POINT.x+sizeof.POINT*4],eax;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT*4],ebp;edi =aAboutZonaButton.Registry.Rect

        ;Получим обедененый найменьший RECT
        ;!lprcDst - указатель на RECT получающий обьедененого;!lprcSrc1 - указатель на RECT;!lprcSrc2 - указатель на  массив RECT;!nCount - количество RECT в массиве
        ;ebx = hDc current
        stdcall  UnionRectHL,aAboutRectPaintUnion,aAboutRectBmp,aAboutRectPaint,sizeof.ABOUT_RECT_PAINT/sizeof.RECT

         ;Освободим не нужный DC
         ;ebx = hDc current
         invoke  ReleaseDC,[hwnd],ebx;ebx = hDc current
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Посчитаем координаты для срелочек в About
;----------------------------------------------
       ;Подготовим
       mov  esi,aAboutZonaButton;esi = ABOUT_RECT_ZONA
       mov  ebp,sizeof.ABOUT_RECT_ZONA/sizeof.ABOUT_ARRAY_ZONA;ebp = count

       @@:
       mov  ebx,[esi+ABOUT_ARRAY_ZONA.Arrow]
       mov  edi,[esi+ABOUT_ARRAY_ZONA.Arrow+4];edi = left point
       shr  ebx,3;ebx = depth line half
       mov  edx,[esi+ABOUT_ARRAY_ZONA.Arrow+8];edx = top string

       add  edx,ebx
       lea  ecx,[edi+ebx*8]
       lea  eax,[edx+ebx*4]
       lea  ecx,[ecx+ebx*2]
       lea  edx,[edx+ebx*2]
       add  ecx,ebx
       add  edx,ebx;edx = top point

       ;Острие стрелки
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*4],ecx
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*4],eax

       lea  ecx,[ecx+ebx*4]

       ;Начальная и конечная точка стрелки
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*0],edi
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*0],edx
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*9],edi
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*9],edx


       ;Сохраняем для строки новые координаты по x откуда начинать рисовать
       mov  [esi+ABOUT_ARRAY_ZONA.StringLeft],ecx

       lea  ecx,[edx+ebx*2];ecx = bottom point
       lea  eax,[edi+ebx*8]

       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*8],edi
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*8],ecx
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*1],eax
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*1],edx
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*7],eax
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*7],ecx

       lea  edi,[edi+ebx*4]
       lea  eax,[ebx*2+ebx]
       add  edi,ebx
       sub  edx,eax
       add  ecx,eax

       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*2],edi
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*2],edx
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*6],edi
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*6],ecx

       lea  eax,[edi+ebx]
       sub  edx,ebx
       add  ecx,ebx

       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*3],eax
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*3],edx
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*5],eax
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*5],ecx

       add  esi,sizeof.ABOUT_ARRAY_ZONA;esi = ABOUT_RECT_ZONA
       dec  ebp
       jnz  @b


;Получим значения системных цветов
;----------------------------------------------
       mov  esi,[GetSysColor];esi = [GetSysColor]
       mov  edi,aSysStock;edi = SYS_STOCK

   stdcall  esi,COLOR_BTNFACE;esi = [GetSysColor]
       mov  [edi+SYS_STOCK.rgbBTNFACE],eax;edi = SYS_STOCK

   stdcall  esi,COLOR_BTNTEXT;esi = [GetSysColor]
       mov  [edi+SYS_STOCK.rgbBTNTEXT],eax;edi = SYS_STOCK

   stdcall  esi,COLOR_HOTLIGHT;esi = [GetSysColor]
       mov  [edi+SYS_STOCK.rgbHOTLIGHT],eax;edi = SYS_STOCK

   stdcall  esi,COLOR_3DLIGHT;esi = [GetSysColor]
       mov  [edi+SYS_STOCK.rgb3DLIGHT],eax;edi = SYS_STOCK

;Получим значения Стандартные системные обьекты рисования
;edi = SYS_STOCK
;----------------------------------------------
       mov  esi,[GetStockObject];esi = [GetStockObject]

   stdcall  esi,DC_PEN;esi = [GetStockObject]
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       mov  [edi+SYS_STOCK.Pen],eax;edi = SYS_STOCK

   stdcall  esi,DC_BRUSH;esi = [GetStockObject]
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       mov  [edi+SYS_STOCK.Brush],eax;edi = SYS_STOCK


            ;Подготовим
            xor  ebx,ebx

;Находим хендл окна панели задач
;ebx = 0
;----------------------------------------------
        invoke  FindWindowExW,ebx,ebx,szShell_TrayWnd,ebx;ebx = 0
           mov  [hwndShell_Tray],eax

;Проводим иницилизацию  ComboBox для выбора языка
;Нужно учитывать тот фак что индекс присваиваеться итемам именно в том порядке каком они добавляються причем начинаеться он от нуля
;Так что в каком тут вызвана  - порядок просто так не надо менять а то легко могут появиться баги
;ebx = 0
;----------------------------------------------
            mov  ebp,[aArrayHwndFont.hwndComboBoxLang];ebp =[hwndComboBoxLang]
            mov  esi,[SendMessageW];esi = [SendMessageW]
        stdcall  esi,ebp,CB_ADDSTRING,ebx,lpCOMBOBOX_BELARUSSIAN;ebx = 0;ebp =[hwndComboBoxLang];esi = [SendMessageW];Index = 0
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

        stdcall  esi,ebp,CB_ADDSTRING,ebx,lpCOMBOBOX_ENGLISH;ebx = 0;ebp =[hwndComboBoxLang];esi = [SendMessageW];Index = 0
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

        stdcall  esi,ebp,CB_ADDSTRING,ebx,lpCOMBOBOX_RUSSIAN;ebx = 0;ebp =[hwndComboBoxLang];esi = [SendMessageW];Index = 1
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

        stdcall  esi,ebp,CB_SETCURSEL,[aIniFileData.aIni.Language],ebx;ebx = 0;ebp =[hwndComboBoxLang];esi = [SendMessageW]
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

;Проводим иницилизацию  SysTab
;Нужно учитывать тот фак что индекс присваиваеться итемам именно в том порядке каком они добавляються причем начинаеться он от нуля
;Так что в каком тут вызвана  - порядок просто так не надо менять а то легко могут появиться баги
;esi = [SendMessageW] ;ebx = 0
;----------------------------------------------
;Создаем вкладки
            lea  edi,[!aAddTcItem];edi =[!aAddTcItem]
            mov  ebp,[aArrayHwndFont.hwndSysTab];ebp =[hwndSysTab]
            mov  [edi+TCITEMW.mask],ebx;ebx = 0;так как иницилизируем сначала без текста то поэтому в ноль ставим
index 0
          indexM  SYSTAB_CONTROL_KEYCHANGE_Id
        stdcall  esi,ebp,TCM_INSERTITEMW,SYSTAB_CONTROL_KEYCHANGE_Id,edi;ebp =[hwndSysTab];esi = [SendMessageW]edi =[!aAddTcItem]
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет


          indexM  SYSTAB_CONTROL_ABOUT_Id
        stdcall  esi,ebp,TCM_INSERTITEMW,SYSTAB_CONTROL_ABOUT_Id ,edi;ebp =[hwndSysTab];esi = [SendMessageW]edi =[!aAddTcItem]
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет
endi
;устанавливаем текущую вкладку

            mov  edi,[aIniFileData.aIni.idTabCurent];edi = [aIniFileData.aIni.idTabCurent] - индекс вкладки которая станет текущей
        stdcall  esi,ebp,TCM_SETCURSEL,edi,ebx;ebp =[hwndSysTab];esi = [SendMessageW];ebx = 0 edi = [aIniFileData.aIni.idTabCurent]
                 _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

;Устанавливаем изабражение на Button приостановить/активировать
;esi = [SendMessageW] ;ebx = 0
;----------------------------------------------

           ;Прежде чем начнем взаимодействовать с потоком проверим а готов он или нет, если не готов - ждем
        invoke  WaitForSingleObject,[hEventThread],INFINITE
                _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

           mov  edi,aIniFileData;edi = aIniFileData
           mov  ecx,[hiconPlay]
           mov  edx,[hiconPause]
           cmp  [edi+INIFILEDATA.aIni.iSuspendPlayHook],ebx;ebx = 0
         cmove  ebp,ecx;ebp = хенд иконки
         cmove  ecx,edx
        cmovne  ebp,edx;ebp = хенд иконки
           mov  [edi+INIFILEDATA.aIni.iSuspendPlayHook],ecx;edi = aIniFileData
            je  .NotHook

        invoke  PostThreadMessageW,[iThreadIdHook],WM_APP_HOOK_Play,ebx,ebx;ebx = ноль
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

     .NotHook:

       stdcall  esi,[hwndButtonActivSusp],BM_SETIMAGE,IMAGE_ICON,ebp;esi = [SendMessageW];ebp = хенд иконки

;Проводим иницилизацию  ListView
;esi = [SendMessageW] ;ebx = 0
;----------------------------------------------
;Устанавливаем расширеные стили
            mov  ebp,[aArrayHwndFont.hwndListView];ebp = [aArrayHwndFont.hwndListView]
        stdcall  esi,ebp,LVM_SETEXTENDEDLISTVIEWSTYLE,ebx,LVS_EX_GRIDLINES or LVS_EX_FULLROWSELECT ;esi = [SendMessageW];ebp = [aArrayHwndFont.hwndListView];ebx = 0


;Устанавливаем ImageList  для состояний элементов
        stdcall  esi,ebp,LVM_SETIMAGELIST,LVSIL_STATE,[hImageListState] ;esi = [SendMessageW];ebp = [aArrayHwndFont.hwndListView];ebx = 0

;Вставляем столбцы
            lea  edi,[!aAddColum]
            mov  [edi+LVCOLUMNW.mask],LVCF_FMT  or LVCF_TEXT or  LVCF_WIDTH
            mov  [edi+LVCOLUMNW.fmt],LVCFMT_CENTER
            mov  [edi+LVCOLUMNW.cx],100;Потом  будет при перемещении меняться, иначе если не установить то не создадуться столбцы
            mov  [edi+LVCOLUMNW.pszText],szButtonClass;Текст болванка - потом замениться


index 0
          indexM  LIST_VIEW_COLUM_ORIGINAL_Id
         stdcall  esi,ebp,LVM_INSERTCOLUMNW,LIST_VIEW_COLUM_ORIGINAL_Id,edi;ebp = [aArrayHwndFont.hwndListView];esi = [SendMessageW]
                  _LastErrorNotSigned;Если не минус возращает функция то ошибки нет

          indexM  LIST_VIEW_COLUM_NEW_Id
         stdcall  esi,ebp,LVM_INSERTCOLUMNW,LIST_VIEW_COLUM_NEW_Id,edi;ebp = [aArrayHwndFont.hwndListView];esi = [SendMessageW]
                  _LastErrorNotSigned;Если не минус возращает функция то ошибки нет


;Вставляем столбец для того что бы потом удалить и получить у двух столбцов выравнивание по центру
          indexM  LIST_VIEW_COLUM_EMPTU_Id
         stdcall  esi,ebp,LVM_INSERTCOLUMNW,LIST_VIEW_COLUM_EMPTU_Id,edi;ebp = [aArrayHwndFont.hwndListView];esi = [SendMessageW]
                  _LastErrorNotSigned;Если не минус возращает функция то ошибки нет
endi
                  ;индекс = ноль так как удаляем крайний левый, а крайний левый всегда = 0
         stdcall  esi,ebp,LVM_DELETECOLUMN,ebx,ebx;ebp = [aArrayHwndFont.hwndListView];esi = [SendMessageW] ;ebx = 0
                  _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

         ;включаем группы
         stdcall  esi,ebp,LVM_ENABLEGROUPVIEW,1,ebx;ebx = 0
                  _LastErrorNotSigned;Если не минус возращает функция то ошибки нет


;Добавляем группы итемы и суб-итемы в цикле
;esi = [SendMessageW] ;ebp = [aArrayHwndFont.hwndListView]

             ;добавляем группу
             lea  edi,[!aCommonLVGROUP]
             mov  ebx,aListViewGroup;ebx = GROUPLISTVIEW
             mov  [edi+LVGROUPW.cbSize],sizeof.LVGROUPW
             mov  [edi+LVGROUPW.mask],LVGF_HEADER or LVGF_GROUPID



     .LoopGroup:
             mov  eax,[ebx+GROUPLISTVIEW.lpszNameGroup]
             mov  ecx,[ebx+GROUPLISTVIEW.idGroup]
             mov  [edi+LVGROUPW.pszHeader],eax
             mov  [edi+LVGROUPW.iGroupId ],ecx

             mov  ebx,[ebx+GROUPLISTVIEW.lpNextGroup];ebx = GROUPLISTVIEW
         stdcall  esi,ebp,LVM_INSERTGROUP,-1,edi;esi = [SendMessageW] ;ebp = [aArrayHwndFont.hwndListView]
                  _LastErrorNotSigned;Если не минус возращает функция то ошибки нет

             cmp  ebx,aListViewGroup;ebx = GROUPLISTVIEW
             jne  .LoopGroup


             xor  eax,eax
             lea  edi,[!aAddItem];edi =  !aAddItem
             dec  eax
             mov  ebx,aListViewItem ;ebx = ITEMLISTVIEW
             mov  [edi+LVITEMW.iItem],eax;-1
             mov  [edi+LVITEMW.stateMask],eax;-1


  .LoopItemText:;добавляем итемы и суб итемы группы

             lea  eax,[!aBufferString]
             mov  ecx,[ebx+ITEMLISTVIEW.stateItem]
             mov  edx,[ebx+ITEMLISTVIEW.idGroup]
             mov  [edi+LVITEMW.mask],LVIF_TEXT or LVIF_STATE or LVIF_GROUPID
             inc  [edi+LVITEMW.iItem]
             mov  [edi+LVITEMW.iSubItem],0;  - индекс того столбика в которрый Item вставляем в нашем случае он всегда = 0 так как в первый вставляем чей индекс = 0
             mov  [edi+LVITEMW.state],ecx
             mov  [edi+LVITEMW.pszText],eax
             mov  [edi+LVITEMW.iGroupId],edx

         ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
         stdcall  VkCodeInString,[ebx+ITEMLISTVIEW.vkCodeItemOne],[ebx+ITEMLISTVIEW.vkCodeItemTwoThree],eax

         stdcall  esi,ebp,LVM_INSERTITEMW,0,edi;esi = [SendMessageW] ;ebp = [aArrayHwndFont.hwndListView];edi =  !aAddItem
                  _LastErrorNotSigned;Если не минус возращает функция то ошибки нет


             mov  [edi+LVITEMW.mask],LVIF_TEXT; or LVIF_STATE
             mov  [edi+LVITEMW.iSubItem],1;LIST_VIEW_COLUM_NEW_Id  - индекс того столбика в которрый SubItem вставляем в нашем случае он всегда = 1 так как 2 только столбика есть

             mov  ecx,[ebx+ITEMLISTVIEW.vkCodeSubOne]
             mov  edx,[ebx+ITEMLISTVIEW.vkCodeSubTwoThree]
             cmp  ecx,VK_LAST
              ja  .VK_LAST


         ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
             lea  eax,[!aBufferString]
         stdcall  VkCodeInString,ecx,edx,eax
             jmp  .SetSubText

       .VK_LAST:
             add  edx,[ebx+ITEMLISTVIEW.scanCodeSubOne]
             mov  [edi+LVITEMW.pszText],edx

    .SetSubText:
             mov  ebx,[ebx+ITEMLISTVIEW.lpNextItem];ebx = ITEMLISTVIEW
         stdcall  esi,ebp,LVM_SETITEMW,0,edi;esi = [SendMessageW] ;ebp = [aArrayHwndFont.hwndListView] ;edi =  !aAddItem
                  _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


             cmp  ebx,aListViewItem;ebx = ITEMLISTVIEW
             jne  .LoopItemText

             mov  eax,[edi+LVITEMW.iItem]
             add  [iCountIndexItem],eax

;Устанавлифаем шрифт для дочерних окон
;esi = [SendMessageW]
;----------------------------------------------
            mov  ebp,[aSysStock.hfont];ebp = Font
            mov  ebx,-(sizeof.HWNDFONT/4);В цикле устанавливаем всем дочерним окнам которые находяться в  HWNDFONT структуре ну или позже добавяться туда
            mov  edi,aArrayHwndFont+ sizeof.HWNDFONT

            @@:
        stdcall  esi,[edi+ebx*4],WM_SETFONT,ebp,0;ebp = Font;esi = [SendMessageW]
            inc  ebx
            jnz  @b


;Устанавливает текст
;----------------------------------------------
        stdcall  SetText

;Делаем видимыми те окна которые связаны с текущей вкладкой  эти функции так же каректирую функции перемещения окан детских
;----------------------------------------------
            mov  eax,[aIniFileData.aIni.idTabCurent];eax = [aIniFileData.aIni.idTabCurent]
           push  SW_SHOW
           call  dword[tbChangeTab+eax*4];eax = [aIniFileData.aIni.idTabCurent] - считаем по индексу * 4 - с макраса с переходами на нужные функции

;Установим место положение главного окна, размер и видимость
;----------------------------------------------
            ;Подготовим
            xor  ebx,ebx;ebx = 0
            mov  edi,[hwnd];edi = hwnd
            mov  esi,[PostMessageW];esi = PostMessageW

         invoke  SetWindowPlacement,edi,aIniFileData.aIni.aWindPlac;edi = hwnd
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Отправим сообщение главному окну для проверки куплена программа или нет
;ebx = 0;edi = hwnd;esi = PostMessageW
;----------------------------------------------
        .lpEmptyGuardianStart:;Начало участка псевдо проверки
        stdcall  esi,edi,WM_APP_LICENSE,ebx,ebx;esi = PostMessageW;edi = hwnd;ebx = 0
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Отправим сообщение главному окну для проверки Целостности программы
;ebx = 0;edi = hwnd;esi = PostMessageW
;----------------------------------------------
        stdcall  esi,edi,WM_APP_ACTION,VK_TESTCRC32,ebx;esi = PostMessageW;edi = hwnd;ebx = 0
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 freeJmp;что бы не было ret, но востановить стек ведь надо...
endp

;Цикл отвечающий за получение всех сообщений главного потока
;ebx = 0
;----------------------------------------------
            ;Подготовим
            mov  esi,aMsg;esi = aMsg

           @@:
        invoke  GetMessageW,esi,ebx,ebx,ebx;ebx = 0;esi = aMsg
          test  eax,eax;Если eax  = 0, значит получили сообщение WM_QUIT - выходим из приложения
            jz  ExitAppss;ebx = 0

        invoke  DispatchMessageW,esi;esi = aMsg
           jmp   @b

;Сделаем все, что надо сделать перед выходом из приложения
;ebx = 0
;----------------------------------------------
proc    ExitAppss
  locals
   !iNumberOfBytesWritten     dd  ?
   !iSizeof.FILEALLSTRUCT     dd  ?
  endl

;Остановим работующие хуки так как они действуют глобально и плюс чтобы иза хукав второго потока не использовать блокировку для доступа к структурам итемав
;ebx = 0
;----------------------------------------------
           mov  esi,[UnhookWindowsHookEx];esi = [UnhookWindowsHookEx]
            or  ebx,[hhookKeyboard]
            jz  .UnhookInValue

       stdcall  esi,ebx;esi = [UnhookWindowsHookEx]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       stdcall  esi,[hhookMouse];esi = [UnhookWindowsHookEx]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

.UnhookInValue:
           xor  ebx,ebx
            or  ebx,[hhookInValueMouse]
            jz  .SaveFileKsm

       stdcall  esi,ebx;esi = [UnhookWindowsHookEx]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       stdcall  esi,[hhookInValue];esi = [UnhookWindowsHookEx]
                 _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


;Сохраним в файл все выбраные клавиши и действия
;----------------------------------------------
       .SaveFileKsm:
         ;Подсчитаем сколько занимают структурры групп
                 mov  ecx,[lpEndRoaming];ecx = [lpEndRoaming]
                 mov  esi,aListViewGroup;esi = aListViewGroup
                 mov  edi,aListViewItem;edi = aListViewItem
                 mov  edx,esi
                 mov  ebp,-(sizeof.GROUPLISTVIEW+sizeof.ITEMLISTVIEW)+sizeof.FILEALLSTRUCT;ebp = sizeof.FILEALLSTRUCT
                 xor  eax,eax

                 ;Скаректируем расширение  файла
                 mov  word[ecx-6],'k';ecx = [lpEndRoaming]
                 mov  word[ecx-4],'s';ecx = [lpEndRoaming]
                 mov  word[ecx-2],'m';ecx = [lpEndRoaming]

                 @@:
                 mov  edx,[edx+GROUPLISTVIEW.lpNextGroup]
                 lea  ebp,[ebp+eax*2+sizeof.GROUPLISTVIEW]
                 cmp  edx,esi
                  je  @f

                 mov  eax,[edx+GROUPLISTVIEW.lengthNameGroup]
                 jmp  @b

                 @@:

         ;Подсчитаем сколько занимают структурры итемав
         ;edi = aListViewItem;esi = aListViewGroup;ebp = sizeof.FILEALLSTRUCT
                 mov  edx,edi;edi = aListViewItem
                 xor  eax,eax

                 @@:
                 mov  edx,[edx+ITEMLISTVIEW.lpNextItem]
                 lea  ebp,[ebp+eax+sizeof.ITEMLISTVIEW]
                 cmp  edx,edi;edi = aListViewItem
                  je  @f

                 xor  eax,eax
                 cmp  [edx+ITEMLISTVIEW.vkCodeSubOne],VK_LAST
               cmova  eax,[edx+ITEMLISTVIEW.scanCodeSubTwoThree]
                 jmp  @b

                 @@:
                 cmp  ebp,sizeof.FILEALLSTRUCT;Значит нечего сохранять не надо, но файл мог остаться предытущий еще - надо удалить его
                  je  .DeleteFileKsi

         ;Выделим память под sizeof.allstruct
         ;edi = aListViewItem;esi = aListViewGroup;ebp = sizeof.FILEALLSTRUCT
                 mov  [!iSizeof.FILEALLSTRUCT],ebp
              invoke  HeapAlloc,[hheap],0,ebp;ebp = sizeof.FILEALLSTRUCT
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 mov  ebx,eax;ebx = lpFILEALLSTRUCT
                 lea  ebp,[eax+sizeof.FILEALLSTRUCT];ebp = curent FILEALLSTRUCT

                 ;Сохраним версию правил
                 mov  [ebx+FILEALLSTRUCT.iVersionRules],equVersionKsmRules;ebx = lpFILEALLSTRUCT

         ;Переносим в структуры групп в память для последующего сохранения в файл
         ;esi = aListViewGroup;ebx = lpFILEALLSTRUCT;ebp = curent FILEALLSTRUCT
                 xor  edi,edi;edi = icountGroup

                 @@:
                 ;esi = aListViewGroup
                 mov  esi,[esi+GROUPLISTVIEW.lpNextGroup]
                 cmp  esi,aListViewGroup
                  je  @f

                 mov  eax,[esi+GROUPLISTVIEW.lengthNameGroup];длина в символах включая ноль имени структуры
                 mov  edx,ebp;ebp = curent lpallstruct
                 lea  ecx,[eax*2+sizeof.GROUPLISTVIEW]
                 lea  ebp,[ebp+eax*2+sizeof.GROUPLISTVIEW];ebp = curent FILEALLSTRUCT
                 inc  edi;edi = icountGroup

             stdcall  MoveMemoryHL,edx,esi,ecx;Копируем структуру
                 jmp  @b

                 @@:
                 mov  [ebx+FILEALLSTRUCT.icountGroup],edi;ebx = lpFILEALLSTRUCT;edi = icountGroup ;сохраняем количество структур
                 mov  esi,aListViewItem;esi = aListViewItem

         ;Переносим в структуры итемав в память для последующего сохранения в файл
         ;esi = aListViewItem;ebx = lpFILEALLSTRUCT;ebp = curent FILEALLSTRUCT
                 xor  edi,edi;edi = icountItem

                 @@:
                 ;esi = aListViewItem
                 mov  esi,[esi+ITEMLISTVIEW.lpNextItem]
                 cmp  esi,aListViewItem
                  je  @f

                 xor  eax,eax
                 cmp  [esi+ITEMLISTVIEW.vkCodeSubOne],VK_LAST
               cmova  eax,[esi+ITEMLISTVIEW.scanCodeSubTwoThree];размер доп структуры в байтах
                 mov  edx,ebp;ebp = curent lpallstruct
                 lea  ecx,[eax+sizeof.ITEMLISTVIEW]
                 lea  ebp,[ebp+eax+sizeof.ITEMLISTVIEW];ebp = curent FILEALLSTRUCT
                 inc  edi;edi = icountItem

             stdcall  MoveMemoryHL,edx,esi,ecx;Копируем структуру
                 jmp  @b

                 @@:
                 mov  [ebx+FILEALLSTRUCT.icountItem],edi;ebx = lpFILEALLSTRUCT;edi = icountItem ;сохраняем количество структур
                 xor  ebp,ebp;ebp = 0
                 mov  edi,[!iSizeof.FILEALLSTRUCT];edi = размер сохраняемых структур

              ;Окрыем файл .ksm для сохранения карты клавиш
              ;ebx = lpFILEALLSTRUCT;edi = размер сохраняемых структур;ebp = 0
              invoke  CreateFileW,szRoamingAppData,FILE_GENERIC_WRITE or FILE_GENERIC_READ,ebp,ebp,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,ebp;ebp = 0
                 mov  esi,eax;esi = хендл файла
                 inc  eax;INVALID_HANDLE_VALUE
                  jz  .SaveFileKsi;не удаеться создать файл

                 ;Для целости даных посчитаем CRC32c
                 lea  eax,[edi-sizeof.FILEALLSTRUCT.iCRC32c];edi = размер сохраняемых структур
                 lea  ecx,[ebx+sizeof.FILEALLSTRUCT.iCRC32c];ebx = lpFILEALLSTRUCT
             stdcall  CRC32cHL,ecx,eax,-1
                 mov  [ebx+FILEALLSTRUCT.iCRC32c],eax

                 lea  eax,[!iNumberOfBytesWritten]
              invoke  WriteFile,esi,ebx,edi,eax,ebp;esi = хендл файла;ebp = ноль;ebx = lpFILEALLSTRUCT;edi = размер сохраняемых структур
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 jmp  .SaveFileKsi

     .DeleteFileKsi:
              invoke  DeleteFileW,szRoamingAppData

;Сохраним настройки в .ksm файл
;----------------------------------------------
       .SaveFileKsi:;Сохранение файлов далжно быть всегда в таком порядке, что бы не попутались расширения или надо изменить это
                 xor  ebx,ebx;ebx = ноль
                 mov  edi,aIniFileData;edi =  aIniFileData
                 mov  esi,[lpEndRoaming];esi = [lpEndRoaming]

               ;Приводим iSuspendPlayHook к такому виду когда если  на кнопке изображена иконка = [hiconPlay], значит должно iSuspendPlayHook= 0 и хуки не влючены
               ;ebx = ноль;edi =  aIniFileData;esi = [lpEndRoaming]
                 mov  eax,[hiconPlay]
                 cmp  [edi+INIFILEDATA.aIni.iSuspendPlayHook],eax
              cmovne  eax,ebx;ebx = ноль
                 mov  [edi+INIFILEDATA.aIni.iSuspendPlayHook],eax

                 ;Скаректируем расширение  файла
                 mov  word[esi-2],'i';esi = [lpEndRoaming]

              ;Сохраняем настройки в файл
              ;ebx = ноль;edi =  aIniFileData
              invoke  CreateFileW,szRoamingAppData,FILE_GENERIC_WRITE or FILE_GENERIC_READ,ebx,ebx,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,ebx;ebx = 0
                 mov  esi,eax;esi = хендл нашего ini файла
                 inc  eax;INVALID_HANDLE_VALUE
                  jz  .Exit;не удаеться создать ini файл;ebp = Хендл окна;ebx = ноль

                 ;Для целости даных посчитаем CRC32c
             stdcall  CRC32cHL,edi,sizeof.INIFILEDATA - sizeof.INIFILEDATA.CRC32,-1
                 mov  [edi+INIFILEDATA.CRC32],eax

                 lea  eax,[!iNumberOfBytesWritten]
              invoke  WriteFile,esi,edi,sizeof.INIFILEDATA,eax,ebx;esi = хендл нашего ini файла;ebx = ноль;edi =  aIniFileData
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


;Все что нужно сделали  выходим из программы
;----------------------------------------------
          .Exit:
                 ;Подготовим
                 xor  ebx,ebx;ebx = 0

                 ;Проверим надо ли вывести сообщение о нелегальной копии
                 cmp  [iBoolMessageReg],ebx;ebx = 0
                  je  .NotMessage

              ;Вывидим сообщение
             stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_HACKING
              invoke  MessageBoxExW,ebx,eax,ebx,MB_OK or MB_ICONWARNING or MB_SYSTEMMODAL,ebx;ebx = 0
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

        .NotMessage:
              invoke  ExitProcess,eax;eax - роли не играет

            ;ret - Выходить с потока не будем
endp

;----------------------------------------------
include  'Inc\WindowsProc.inc'
include  'Inc\WindowsProcOther.inc'
include  'Inc\Proc.inc'


;----------------------------------------------
;Тут кончаеться защищаемый участок
lpGuardianEnd:

;Считаем CRC32c защищаемого участка
CRC32cInMacro  constCRC32cGuardian,Start,lpGuardianEnd - Start,0xFFFFFFFF;lpGuardianEnd - Start

;Считаем CRC32c защищаемого участка приманки
CRC32cInMacro  constCRC32cGuardianEmpty,Start.lpEmptyGuardianStart,WindowProc.lpEmptyGuardianEnd - Start.lpEmptyGuardianStart,\
               0xFFFFFFFF;WindowProc.lpEmptyGuardianEnd - Start.lpEmptyGuardianStart

;----------------------------------------------
section '.idata' import data readable writeable
include 'Include\My\Import.inc'

;----------------------------------------------
include  'Inc\Resource.inc'

;section '.reloc' data fixups readable

