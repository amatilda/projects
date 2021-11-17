format PE GUI 4.0
;heap  0x20000,0x4000
entry Start
;stack 0x3840f0,3000h

;equGlAssembly equ Debug; ������ ������� � ��� ���������� ������
equGuardian   equ Guardian;������ ������� ��������� ������ ������ ���������� ����� ������� � �� ������
;----------------------------------------------
include  'Include\Win32W.inc'

;----------------------------------------------
include  'Inc\ConstEquates.inc'
ArgNtSetDebugFilterState       uniq  NtSetDebugFilterState;��� �� ����� �������� � ������� ����� �� ���� �� �������������
ArgNtSetInformationProcess     uniq  NtSetInformationProcess;��� �� ����� �������� � ������� ����� �� ���� �� �������������
ArgNtSetInformationThread      uniq  NtSetInformationThread;��� �� ����� �������� � ������� ����� �� ���� �� �������������
ArgNtGetContextThread          uniq  NtGetContextThread;��� �� ����� �������� � ������� ����� �� ���� �� �������������
ArgNtQueryObject               uniq  NtQueryObject;��� �� ����� �������� � ������� ����� �� ���� �� �������������
ArgNtQuerySystemInformation    uniq  NtQuerySystemInformation ;��� �� ����� �������� � ������� ����� �� ���� �� �������������


constSetTimer                  uniq  SetTimer - Start.GetProcessHeap;�������� �� ������� SetTimer  ������������ Start ��� �����  ���� ����� �����������
constLoadLibraryExW            uniq  LoadLibraryExW - Start.GetProcessHeap;�������� �� ������� LoadLibraryExW  ������������ Start ��� �����  ���� ����� �����������
constFreeLibrary               uniq  FreeLibrary - Start.GetProcessHeap;�������� �� ������� FreeLibrary  ������������ Start ��� �����  ���� ����� �����������
constTimerEmptyGuardian        uniq  TimerEmptyGuardian;��� �� ������������ ������� �������


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
   !aSizeFile             !LARGE_INTEGER; ���� �������� ������ ini �����
   !aAddTcItem            TCITEMW;add item for SysTab Control
   !aAddColum             LVCOLUMNW
   !aAddItem              LVITEMW
   !aCommonLVGROUP        LVGROUPW
   !aBufferString         du  tbKeylengthMax dup(?)
  endl


;SEH
;----------------------------------------------
           xor  ebx,ebx ;ebx = 0
        invoke  SetUnhandledExceptionFilter,UnhandledExceptionFilter;������������� ��� ��� �� ������ ��� ����������;SetUnhandledExceptionFilter;AddVectoredExceptionHandler

;����� �� ����������� ������ ������ ���������� � ����� �����
;ebx = 0
;----------------------------------------------
        invoke  CreateEventExW,ebx,szEventOneApp,ebx,SYNCHRONIZE or EVENT_MODIFY_STATE;ebx = 0
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [hEventThread],eax

        invoke  GetLastError
           cmp  eax,ERROR_ALREADY_EXISTS
           jne  .GetProcessHeap;ebx = 0


;������� ���� �� ����������������� ������ � ������� ��� �� �������� ����
;----------------------------------------------
        invoke  FindWindowExW,ebx,ebx,szEventOneApp,ebx;ebx = 0
           mov  ebp,eax;ebp = ����� ����

        invoke  SetForegroundWindow,eax
        invoke  ShowWindow,ebp,SW_SHOW;ebp = ����� ����
        .Exit:
        invoke  ExitProcess,eax;eax - ���� �� ������

;�������� heap ������
;ebx = 0
;----------------------------------------------
.GetProcessHeap:
        invoke  GetProcessHeap
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [hheap],eax



;������������� COM  � ������ ��������� SHELLEXECUTEINFOW
;ebx = 0
;----------------------------------------------
        invoke  CoInitializeEx,ebx,COINIT_APARTMENTTHREADED or COINIT_DISABLE_OLE1DDE;ebx = 0

;�������� ���� � �������� Roaming � ������� ��� �����
;ebx = 0
;----------------------------------------------
           ;������� ��������� ����� Roaming ��� �������� ����� ��������� ������
           lea  eax,[!ppszPath]
        invoke  SHGetKnownFolderPath,aFolderidRoamingAppData,KF_FLAG_CREATE,ebx,eax;ebx = 0
                _LastErrorNull;���� ���� ��������� ������� �� ������ ���

           mov  ebp,[!ppszPath];ebp = ����� ����� ���������� ������
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
        invoke  CoTaskMemFree,ebp;���������� ������ ���������� �������� SHGetKnownFolderPath

           mov  ecx,(sizeof.sHLCatalog) shr 1
           mov  ebp,szRoamingAppData
           cld
           rep  movsw
        invoke  CreateDirectoryW,ebp,ebx;ebx = 0 ������� ����� � Roaming �����

           mov  ecx,(sizeof.sNameDirectory) shr 1
           cld
           rep  movsw
        invoke  CreateDirectoryW,ebp,ebx;ebx = 0 ������� ����� � Roaming ��� ����� ���������

           mov  ecx,sizeof.sMainIniFile shr 1
           cld
           rep  movsw;�������� ��� ������ ini �����
           mov  [lpEndRoaming],edi

           mov  esi,[CreateFileW];esi = [CreateFileW]
           mov  edi,[ReadFile];edi = [ReadFile]

;��������� ksi ����
;esi = [CreateFileW];edi = [ReadFile];ebp = ��������� �� ����� � ����� � Roaming � �����
;----------------------------------------------
       stdcall  esi,ebp,FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0;esi = [CreateFileW]
           mov  ebp,eax;ebp = ����� ������ ksi �����
           inc  eax;INVALID_HANDLE_VALUE;�� �������� ������� ini ����
            jz  .OpenFileKss;esi = [CreateFileW];edi = [ReadFile]



;������ ���� � ������
;esi = [CreateFileW];edi = [ReadFile];ebp = ����� ������ ksi �����
;----------------------------------------------
           lea  eax,[!NumberOfBytesRead]
       stdcall  edi,ebp,aIniFileData,sizeof.INIFILEDATA,eax,0;edi = [ReadFile];ebp = ����� ������ ksi �����
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

        invoke  CloseHandle,ebp;ebp = ����� ������ ini �����
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

;CRC32c - 0x82F63B78 -�������
;esi = [CreateFileW];edi = [ReadFile]
;----------------------------------------------
           mov  ebx,aIniFileData;ebx = aIniFileData
       stdcall  CRC32cHL,ebx,sizeof.INIFILEDATA - sizeof.INIFILEDATA.CRC32,-1;ebx = aIniFileData

           ;���������� � CRC32c ���������� � ini �����
           cmp  eax,[ebx+INIFILEDATA.CRC32];ebx = aIniFileData
            je  .OpenFileKss;esi = [CreateFileW];edi = [ReadFile]

       ;���� CRC32 �� ��������� �������� ��������� �� �������
       stdcall  MoveMemoryHL,ebx,aInitilizDefault,sizeof.INITILIZDATA;ebx = aIniFileData

           ;�������� ��� ��� CRC32 �� ���������, ��� �� �� �������� ���� ��� �� �������������� ������
           mov  word[ebx+INIFILEDATA.szOpenExeBufer],0;ebx = aIniFileData
           mov  word[ebx+INIFILEDATA.szLaunchfileBufer],0;ebx = aIniFileData



;��������� ���� �����������
;esi = [CreateFileW];edi = [ReadFile]
;----------------------------------------------
 .OpenFileKss:
       stdcall  esi,szKeyFile,FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0 ;esi = [CreateFileW]
           ;�������� ���������� ��� ���� ����� ������

           mov  ebp,eax;ebp = ����� ������ Kss �����
           inc  eax;INVALID_HANDLE_VALUE;�� �������� �������
            jz  .OpenFileKsm;esi = [CreateFileW];edi = [ReadFile]

           lea  edx,[!NumberOfBytesRead]
           mov  ebx,aKeySerial;ebx = KEYFILEW
       stdcall  edi,ebp,ebx,sizeof.KEYFILEW,edx,0;edi = [ReadFile];ebp = ����� �����;ebx = KEYFILEW
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

        invoke  CloseHandle,ebp;ebp = �����  �����
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���


          ;�������� CRC32c �� ����������� �����
          ;esi = [CreateFileW];edi = [ReadFile];ebx = KEYFILEW
           lea  eax,[ebx+sizeof.KEYFILEW.iCRC32File];ebx = KEYFILEW
       stdcall  CRC32cHL,eax,sizeof.KEYFILEW - sizeof.KEYFILEW.iCRC32File,-1
           cmp  [ebx+KEYFILEW.iCRC32File],eax;ebx = KEYFILEW
           jne  .OpenFileKsm;esi = [CreateFileW];edi = [ReadFile]

          ;��������� ��� � ������� ������������ ��������� �����
          ;esi = [CreateFileW];edi = [ReadFile];ebx = KEYFILEW
           lea  edx,[ebx+KEYFILEW.szNameUser]
           mov  ecx,[ebx+KEYFILEW.lengthszNameUser];ebx = KEYFILEW
           mov  [lpBuyNameUser],edx

           @@:
           xor  word[edx],constNumberN and 0xFFFF
           add  edx,2
           dec  ecx
           jnz  @b



;��������� ���� ����� ������
;�������� ������ ������ ���� ������ � ����� �������, ��� �� �� ���������� ���������� ��� ���������� ��� ���� �������� ���
;esi = [CreateFileW];edi = [ReadFile]
;----------------------------------------------
 .OpenFileKsm:
           mov  edx,[lpEndRoaming];edx = [lpEndRoaming]
           mov  word[edx-2],'m';edx =  [lpEndRoaming]
       stdcall  esi,szRoamingAppData,FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0;esi = [CreateFileW]
           mov  ebp,eax;ebp = ����� ������ ksm �����
           inc  eax;INVALID_HANDLE_VALUE
            jz  .SkipSetting

           ;������ ������, ��� �� ����� ������� �������� ������
           lea  eax,[!aSizeFile]
        invoke  GetFileSizeEx,ebp,eax;ebp = �����  �����
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

           ;�������� ������
           mov  esi,[!aSizeFile.LowPart];esi = [!aSizeFile.LowPart]
        invoke  HeapAlloc,[hheap],0,esi;esi = [!aSizeFile.LowPart]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  ebx,eax;ebx = FILEALLSTRUCT

       ;������ ����
       ;ebx = FILEALLSTRUCT;esi = [!aSizeFile.LowPart];ebp = ����� ������ ksm �����;edi = [ReadFile]
           lea  edx,[!NumberOfBytesRead]
       stdcall  edi,ebp,ebx,esi,edx,0;edi = [ReadFile];ebp = ����� �����;ebx = FILEALLSTRUCT;esi = [!aSizeFile.LowPart]

        invoke  CloseHandle,ebp;ebp = �����  �����
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

          ;�������� CRC32c �� ����������� �����
          ;ebx = FILEALLSTRUCT;esi = [!aSizeFile.LowPart]
           sub  esi,sizeof.FILEALLSTRUCT.iCRC32c;esi = [!aSizeFile.LowPart]
            js  .FreeAlloc;ebx = FILEALLSTRUCT;�� ������ ���� ���� ��� �������
           lea  edx,[ebx+sizeof.FILEALLSTRUCT.iCRC32c];ebx = FILEALLSTRUCT
       stdcall  CRC32cHL,edx,esi,-1;ebx = FILEALLSTRUCT

           ;��������� ������� CRC32c ��� ���
           xor  ecx,ecx
           cmp  [ebx+FILEALLSTRUCT.iCRC32c],eax;ebx = FILEALLSTRUCT;esi = [!aSizeFile.LowPart]
        cmovne  ecx,esp

           ;�������� ��������� ������ ������ ��� ���
           cmp  [ebx+FILEALLSTRUCT.iVersionRules],equVersionKsmRules;ebx = lpFILEALLSTRUCT
        cmovne  ecx,esp

          ;���� �� ����� ���� ������ ����������� ������ �� ����������
          test  ecx,ecx
           jnz  .FreeAlloc;ebx = FILEALLSTRUCT

          ;���������� ��������� ����� � ���������
          ;ebx = FILEALLSTRUCT
           mov  ebp,[ebx+FILEALLSTRUCT.icountGroup]
           lea  esi,[ebx+sizeof.FILEALLSTRUCT];esi = fileGROUPLISTVIEW

  .LoopGrooup:
           dec  ebp;ebp = [icountGroup]
            js  .BildItemStruct

           mov  eax,[esi+GROUPLISTVIEW.lengthNameGroup];esi = fileGROUPLISTVIEW
           lea  edi,[eax*2+sizeof.GROUPLISTVIEW];edi = sizeof.full.GROUPLISTVIEW

        invoke  HeapAlloc,[hheap],0,edi;edi = sizeof.full.GROUPLISTVIEW
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

          xchg  eax,edi;edi = newGROUPLISTVIEW;eax = sizeof.full.GROUPLISTVIEW
           mov  edx,esi;esi = fileGROUPLISTVIEW
           add  esi,eax;esi = fileGROUPLISTVIEW;eax = sizeof.full.GROUPLISTVIEW

       stdcall  MoveMemoryHL,edi,edx,eax;edi = newGROUPLISTVIEW;eax = sizeof.full.GROUPLISTVIEW

          ;����������� ������ � ����� � ������ ������ ������������
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



          ;���������� ��������� ������  � ���������
          ;ebx = FILEALLSTRUCT;esi = fileITEMLISTVIEW
.BildItemStruct:
           mov  ebp,[ebx+FILEALLSTRUCT.icountItem]

    .LoopItem:
           dec  ebp;ebp = [icountItem]
            js  .FreeAlloc

           xor  eax,eax
           mov  edi,sizeof.ITEMLISTVIEW
           cmp  [esi+ITEMLISTVIEW.vkCodeSubOne],VK_LAST;esi = fileITEMLISTVIEW
         cmova  eax,[esi+ITEMLISTVIEW.scanCodeSubTwoThree];esi = fileITEMLISTVIEW ;������ ��� ��������� � ������
           add  edi,eax;edi = sizeof.full.ITEMLISTVIEW

        invoke  HeapAlloc,[hheap],0,edi;edi = sizeof.full.ITEMLISTVIEW
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

          xchg  eax,edi;edi = newITEMLISTVIEW;eax = sizeof.full.ITEMLISTVIEW
           mov  edx,esi;esi = fileITEMLISTVIEW
           lea  ecx,[edi+sizeof.ITEMLISTVIEW]
           cmp  eax,sizeof.ITEMLISTVIEW;eax = sizeof.full.ITEMLISTVIEW
         cmove  ecx,[edx+ITEMLISTVIEW.vkCodeSubTwoThree];���� vkCodeSubOne ������ ��� VK_LAST, ����� vkCodeSubTwoThree ��������� �� ��� ������ - �����������
           add  esi,eax;esi = fileITEMLISTVIEW;eax = sizeof.full.ITEMLISTVIEW
           mov  [edx+ITEMLISTVIEW.vkCodeSubTwoThree],ecx

       stdcall  MoveMemoryHL,edi,edx,eax;edi = newITEMLISTVIEW;eax = sizeof.full.ITEMLISTVIEW

          ;����������� ������ � ����� � ������ ������ ������������
          ;edi = newITEMLISTVIEW;esi = fileITEMLISTVIEW;ebx = FILEALLSTRUCT;ebp = [icountItem]
           mov  edx,aListViewItem
           mov  ecx,[edx+ITEMLISTVIEW.lpPrevItem]
           mov  [edx+ITEMLISTVIEW.lpPrevItem],edi;edi = newITEMLISTVIEW
           mov  [ecx+ITEMLISTVIEW.lpNextItem],edi;edi = newITEMLISTVIEW
           mov  [edi+ITEMLISTVIEW.lpPrevItem],ecx;edi = newITEMLISTVIEW
           mov  [edi+ITEMLISTVIEW.lpNextItem],edx;edi = newITEMLISTVIEW
           jmp  .LoopItem

    ;����������� �� ������ ������
    ;ebx = FILEALLSTRUCT
   .FreeAlloc:
        invoke  HeapFree,[hheap],0,ebx;ebx = FILEALLSTRUCT
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

;��������� �������� �����
;----------------------------------------------
 .SkipSetting:
           mov  esi,aIniFileData.aIni.Language;esi = aIniFileData.aIni.Language
           xor  ebx,ebx;ebx = 0

       ;�� ������� ������ ���� �� !idlang - id ����� ��� �������� �������� COMBOBOX_ENGLISH
       ;���� �� �������� ��������� �� eax = 0
       stdcall  LoadLang,[esi];esi = aIniFileData.aIni.Language
          test  eax,eax
           jnz  @f

           ;���� ���������� ��������� ���� �������� ������������� ���������
           ;esi = aIniFileData.aIni.Language
           mov  dword[esi],COMBOBOX_ENGLISH;esi = aIniFileData.aIni.Language
           mov  [lpMultiLanguage],tbMultiLanguage

           @@:

;������������� ����� �������
;ebx = 0
;----------------------------------------------
.InitCommonControls:
           ;����������
           mov  esi,[lpEndRoaming];esi = [lpEndRoaming]
        invoke  InitCommonControlsEx,aInitControlEx


;�������� ���� ��� �������� �������
;ebx = 0;esi = [lpEndRoaming]
;----------------------------------------------
           ;������������ ����������  �����
           ;ebx = 0;esi = [lpEndRoaming]
           mov  word[esi-6],'t';esi = [lpEndRoaming]
           mov  word[esi-4],'m';esi = [lpEndRoaming]
           mov  word[esi-2],'p';esi = [lpEndRoaming]

        ;�������� ����
        ;ebx = ����
        invoke  CreateFileW,szRoamingAppData,FILE_GENERIC_WRITE or FILE_GENERIC_READ,ebx,ebx,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,ebx;ebx = 0
           mov  ebp,eax;ebp = ����� �����
           inc  eax;INVALID_HANDLE_VALUE
            jz  .SkipTmpFile;ebx = 0

           ;������� ����
           lea  eax,[!NumberOfBytesRead]
        invoke  WriteFile,ebp,aGuardianDll,sizeof.aGuardianDll,eax,ebx;ebp = ����� �����;ebx = ����
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

        ;������� ����� �����
        invoke  CloseHandle,ebp;ebp = �����  �����
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

;������� ���� ������ - hInstance
;ebx = 0
;----------------------------------------------
 .SkipTmpFile:
        invoke  GetModuleHandleW,ebx;ebx = 0
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

           mov  [hInstance],eax
           mov  ebp,eax;ebp = hInstance

;������� �������������� ����� ��� ������ �����
;!!!����� ���� ������ ����� ���� ��� �������� ����, ���� � ������, ���� � ����������� � ���� � ���������� KeyMap!!!
;ebp = hInstance;ebx = 0
;----------------------------------------------
        invoke  CreateThread,ebx,ebx,ThreadProcForHook,ebx,ebx,iThreadIdHook;ebx = 0
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

        invoke  CloseHandle,eax
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

;�������������� ������ ��� ListView
;ebp = hInstance
;----------------------------------------------
           ;������� ImageList ��� ��������� ������
        invoke  ImageList_Create,LIST_VIEW_STATE_Width_Height,LIST_VIEW_STATE_Width_Height,ILC_COLOR32,LIST_VIEW_STATE_MIN_MAX,LIST_VIEW_STATE_MIN_MAX
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [hImageListState],eax
           mov  esi,eax;esi = hImageListState


index 0
          ; ��������� ������ � ������������� � ImageList ��� ��������� ������
           xor  eax,eax
           mov  edi,[LoadImageW];edi = [LoadImageW]
        indexM  IMAGE_LIST_STATE_NOTCHECK_Id
       stdcall  edi,ebp,IDR_ICON_GROUP_NOTCHECK,IMAGE_ICON,eax,eax,eax;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
          push  eax;��� DestroyIcon

           mov  ebx,[ImageList_ReplaceIcon];ebx = [ImageList_ReplaceIcon]
       stdcall  ebx,esi,-1,eax;esi = hImageListState  ;ebx = [ImageList_ReplaceIcon]
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���

           xor  eax,eax
        indexM  IMAGE_LIST_STATE_CHECK_Id
       stdcall  edi,ebp,IDR_ICON_GROUP_CHECK,IMAGE_ICON,eax,eax,eax;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
          push  eax;��� DestroyIcon

       stdcall  ebx,esi,-1,eax;esi = hImageListState ;ebx = [ImageList_ReplaceIcon]
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���

           xor  eax,eax
        indexM  IMAGE_LIST_STATE_QUES_Id
       stdcall  edi,ebp,IDR_ICON_GROUP_QUES,IMAGE_ICON,eax,eax,eax;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
          push  eax ;��� DestroyIcon

       stdcall  ebx,esi,-1,eax;esi = hImageListState ;ebx = [ImageList_ReplaceIcon]
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���
endi

         ;����������� �������� ����������� ������
           mov  esi,[DestroyIcon];esi = [DestroyIcon]
           xor  ebx,ebx;ebx = 0
          call  esi;esi = [DestroyIcon]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

          call  esi;esi = [DestroyIcon]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

          call  esi;esi = [DestroyIcon]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���


;��������� ������ ���   ������ �������������/������������
;ebx = 0;ebp = hInstance;edi = [LoadImageW]
;----------------------------------------------
        stdcall  edi,ebp,IDR_ICON_GROUP_PAUSE,IMAGE_ICON,ebx,ebx,ebx;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW];ebx = 0
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
            mov  [hiconPause],eax

        stdcall  edi,ebp,IDR_ICON_GROUP_PLAY,IMAGE_ICON,ebx,ebx,ebx;ebp = hInstance LR_DEFAULTCOLOR = 0 ;edi = [LoadImageW];ebx = 0
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
            mov  [hiconPlay],eax
            mov  [aMenuState.ActivSusp],eax;��������� �������� ��� ���� MENU_ACTIVATE_Id

;��������� ������ ��� ����,������ �����  � ����������� ������
;ebx = 0;ebp = hInstance;edi = [LoadImageW]
;----------------------------------------------

           mov  esi,aWc;WNDCLASSEXW
       stdcall  edi,ebp,IDR_ICON_GROUP,IMAGE_ICON,ebx,ebx,LR_DEFAULTCOLOR or LR_DEFAULTSIZE;edi = [LoadImageW];ebp = hInstance;ebx = 0
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

           mov  [esi+WNDCLASSEXW.hInstance],ebp;ebp = hInstance
           mov  [esi+WNDCLASSEXW.hIcon],eax;hIcon
           mov  [esi+WNDCLASSEXW.hIconSm],eax;hIcon
           mov  [aNotifyIconData.hIcon],eax;hIcon  - ��������� ������ ��� ����
           mov  [hicon],eax

       ;������ �����������
       stdcall  edi,ebx,IDC_ARROW,IMAGE_CURSOR,ebx,ebx,LR_DEFAULTCOLOR or LR_DEFAULTSIZE or LR_SHARED;edi = [LoadImageW];ebp = hInstance;ebx = 0
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [esi+WNDCLASSEXW.hCursor],eax


;������������ ����� ��������� ���� � ��� �����������  ����
;ebx = 0;ebp = hInstance;esi = WNDCLASSEXW
;----------------------------------------------
        invoke  RegisterClassExW,esi;esi = WNDCLASSEXW
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
                ;eax = ����
           ;����������
           mov  esi,[CreateWindowExW];esi = CreateWindowExW

;�������� ���� ;esi = CreateWindowExW
       stdcall  esi,WS_EX_ACCEPTFILES or WS_EX_CONTROLPARENT,eax,ebx,WS_CLIPCHILDREN or WS_MAXIMIZEBOX or WS_SYSMENU or WS_OVERLAPPED or WS_SIZEBOX,\
                                ebx,ebx,ebx,ebx,ebx,ebx,ebp,ebx;ebx = 0;ebp = hInstance;eax = ����
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  edi,eax;edi = hwnd
           mov  edx,aShellExecute;edx = aShellExecute
           mov  [hwnd],eax
           mov  [aNotifyIconData.hWnd],eax; - ��������� ����� ���� ��� ����

           ;�������������  ��������� SHELLEXECUTEINFOW
           ;edx = aShellExecute;edi = hwnd
           mov  [edx+SHELLEXECUTEINFOW.cbSize],sizeof.SHELLEXECUTEINFOW;edx = aShellExecute
           mov  [edx+SHELLEXECUTEINFOW.fMask],SEE_MASK_ASYNCOK;edx = aShellExecute
           mov  [edx+SHELLEXECUTEINFOW.hwnd],edi;edx = aShellExecute;edi = hwnd
           mov  [edx+SHELLEXECUTEINFOW.nShow],SW_SHOWNORMAL;edx = aShellExecute

;������ ������ ;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szButtonClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or BS_PUSHBUTTON or BS_CENTER or BS_TEXT,\
                                        ebx,ebx,BUTTON_EXIT_Width,BUTTON_EXIT_Height,edi,BUTTON_EXIT_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [aArrayHwndFont.hwndButtonExit],eax

;������ ����� ;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szButtonClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or BS_PUSHBUTTON or BS_CENTER or BS_TEXT,\
                                        ebx,ebx,BUTTON_OPTIONS_Width,BUTTON_OPTIONS_Height,edi,BUTTON_OPTIONS_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [aArrayHwndFont.hwndButtonOptions],eax

;������ �������������/������������ ;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szButtonClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or BS_PUSHBUTTON or BS_CENTER or BS_ICON,\
                                        ebx,ebx,BUTTON_ACTIVSUSP_Width,BUTTON_ACTIVSUSP_Height,edi,BUTTON_ACTIVSUSP_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [hwndButtonActivSusp],eax


;ComboBox ��� ������ ����� ;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szComboBoxClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or CBS_DROPDOWNLIST,\
                                        ebx,ebx,COMBOBOX_LANG_Width,ebx,edi,COMBOBOX_LANG_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [aArrayHwndFont.hwndComboBoxLang],eax

;Sys Tab;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szSysTabClass,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or TCS_HOTTRACK or TCS_BUTTONS or TCS_FLATBUTTONS,\
                                        ebx,2,ebx,ebx,edi,SYSTAB_CONTROL_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [aArrayHwndFont.hwndSysTab],eax

;List View;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szListViewClass,ebx,WS_CHILD or WS_CLIPSIBLINGS or LVS_REPORT or LVS_SHOWSELALWAYS or\
                                         LVS_SINGLESEL or LVS_ALIGNTOP,ebx,ebx,ebx,ebx,edi,LIST_VIEW_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [aArrayHwndFont.hwndListView],eax

;Static ��� ���������;esi = CreateWindowExW
       stdcall  esi,WS_EX_NOPARENTNOTIFY,szStatic,ebx,WS_CHILD or WS_VISIBLE or WS_CLIPSIBLINGS or SS_SUNKEN or SS_NOPREFIX or SS_CENTER,\
                                        ebx,ebx,COMBOBOX_LANG_Width,ebx,edi,STATIC_HELP_Id,ebp,ebx;ebx = 0;ebp = hInstance;edi = hwnd
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
           mov  [aArrayHwndFont.hwndStatic],eax

;�������� ������ ���������� ����� �������� ����
;ebx = 0;edi = hwnd;ebp = hInstance
;----------------------------------------------
         invoke  GetClientRect,edi,aClientRect;edi = hwnd
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

;��������� ��� ����� ������ ������ ������
;ebx = 0;edi = hwnd;ebp = hInstance
;----------------------------------------------
         invoke  CreateDialogIndirectParamW,ebp,aDialogEmpty,edi,DlgProcEmpty,ebx;ebx = 0;edi = hwnd;ebp = hInstance
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

            mov  esi,eax;esi = [HwndDlg]

         invoke  SetClassLongW,esi,GCLP_HICON,[hicon];esi = [HwndDlg]

         invoke  DestroyWindow,esi;esi = [HwndDlg]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

;������� ����� �������� �����
;ebx = 0;edi = hwnd
;----------------------------------------------
         invoke  GetDesktopWindow
            mov  [hwndDeskop],eax

;���������� ������� ����� ����� �������� ����� ����� � About � �������� �������
;ebx = 0;edi = hwnd
;----------------------------------------------
         ;�������� ������ ����� -��������� ��������� ����� ��� �� ����� �����
         invoke  CreateFontIndirectW,aFont
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

            ;����������
            mov  ebp,eax;ebp = hfont

         ;������� DC ���� ��������
         invoke  GetDC,edi;edi = hwnd
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

            ;����������
            mov  ebx,eax;ebx = hDc current
            mov  esi,[GetTextExtentPoint32W];esi = [GetTextExtentPoint32W]
            mov  edi,[SelectObject];edi = [SelectObject]

        ;������� ����� � DC
        ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = hfont
        stdcall  edi,ebx,ebp;edi = [SelectObject];ebx = hDc current;ebp = hfont
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

        ;���������� ������ � ������ ������ ��� ������
        ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = hfont
        stdcall  esi,ebx,sNameMainWin,(sizeof.sNameMainWin-4)/2,aAboutRectPaint.NameVer.right;esi = [GetTextExtentPoint32W];ebx = hDc current
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

            ;�����������
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

        ;���������� ������ � ������ ������ ��� ������
        ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = hfont
        stdcall  esi,ebx,sCopyright,sizeof.sCopyright/2,aAboutRectPaint.Copyright.right;esi = [GetTextExtentPoint32W];ebx = hDc current
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

            ;�����������
            mov  edx,aAboutRectPaint.Copyright;edx = aAboutRectPaint.Copyright
            add  [edx+RECT.right],ABOUT_BORDER+ABOUT_BMP_Width_Height;edx = aAboutRectPaint.Copyright
            add  [edx+RECT.bottom],ABOUT_BORDER+ABOUT_BMP_Width_Height+SYSTAB_CONTROL_Height;edx = aAboutRectCopyright

         ;����������� �������� �����
         ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = hfont
         invoke  DeleteObject,ebp;ebp = hfont
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���


;���������� ������� ����� ����� �������� ����� ����� � About � ������� �������
;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W]
;----------------------------------------------
            ;����������
            mov  edx,aFont;edx = LOGFONTW
            mov  [edx+LOGFONTW.lfHeight],21;edx = LOGFONTW ;������ - 0x15, ������� ������- 0x9;��� ���� ������ ��������� ������ � ����� Combox
            mov  [edx+LOGFONTW.lfWeight],FW_NORMAL;edx = LOGFONTW

         ;�������� �������� ����� - ����� ����������� �� ���������� ������
         ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W]
         invoke  CreateFontIndirectW,edx;edx = LOGFONTW
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
            mov  [aSysStock.hfont],eax;eax = [hfont]

        ;������� ����� � DC
        ;edi = [SelectObject];ebx = hDc current;esi = [GetTextExtentPoint32W]
        stdcall  edi,ebx,eax;edi = [SelectObject];ebx = hDc current;eax = [hfont]
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

        ;���������� ������ � ������ ������ ��� ������
        ;ebx = hDc current;esi = [GetTextExtentPoint32W]
        stdcall  esi,ebx,szInfoEmail,(sizeof.szInfoEmail-2)/2,aAboutRectPaint.Email.right;esi = [GetTextExtentPoint32W];ebx = hDc current
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

            ;�����������
            mov  edx,aAboutRectPaint.Email;edx = aAboutRectPaint.Email
            mov  eax,[aAboutRectPaint.Copyright.bottom]
            mov  ecx,[edx+RECT.bottom];edx = aAboutRectPaint.Email
            mov  edi,aAboutZonaButton.Email.Rect;edi =aAboutZonaButton.Email.Rect
            add  eax,ABOUT_INDENT+5;��� �� �� ��������� ����� ��������� ���� ������������ ��� ��������� �����
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow],ecx;edi =aAboutZonaButton.Email.Rect;���� ������� ���������
            lea  ecx,[ecx*2+ABOUT_BORDER+ABOUT_BMP_Width_Height]
            mov  [edx+RECT.top],eax;edx = aAboutRectPaint.Email
            add  [edx+RECT.right],ecx;edx = aAboutRectPaint.Email
            add  [edx+RECT.bottom],eax;edx = aAboutRectPaint.Email

            ;���� ������� ���������
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow+8],eax;edi =aAboutZonaButton.Email.Rect

            ;���������� ���������� ��� ����� ������
            mov  ebp,eax;ebp = aAboutRectPaint.Email.top
            mov  ecx,[edx+RECT.right];edx = aAboutRectPaint.Email
            mov  edx,[edx+RECT.bottom];edx = aAboutRectPaint.Email
            sub  eax,ABOUT_INDENT
            add  ecx,ABOUT_INDENT*2
            add  edx,ABOUT_INDENT
            mov  [edi+RECT.top],eax;edi =aAboutZonaButton.Email.Rect
            mov  [edi+RECT.right],ecx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+RECT.bottom],edx;edi =aAboutZonaButton.Email.Rect

            ;�������� �� ���� ������� ��� ��� ��������� ������� � ��������� ExtTextOutW ��� ������� ���� ������� �� ������� ����� ��������������
            dec  eax

            ;�������� ������ �����
            mov  [edi+sizeof.RECT+POINT.y],eax;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.x+sizeof.POINT],ecx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT],eax;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.x+sizeof.POINT*2],ecx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT*2],edx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT*3],edx;edi =aAboutZonaButton.Email.Rect
            mov  [edi+sizeof.RECT+POINT.y+sizeof.POINT*4],eax

        ;���������� ������ � ������ ������ ��� ������
        ;ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = aAboutRectPaint.Email.top
        stdcall  esi,ebx,szInfoRegUrl,(sizeof.szInfoRegUrl-2)/2,aAboutRectPaint.Registry.right;esi = [GetTextExtentPoint32W];ebx = hDc current
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

            ;�����������
            ;ebx = hDc current;esi = [GetTextExtentPoint32W];ebp = aAboutRectPaint.Email.top
            mov  edx,aAboutRectPaint.Registry;edx = aAboutRectPaint.Registry
            mov  edi,aAboutZonaButton.Registry.Rect;edi =aAboutZonaButton.Registry.Rect
            mov  ecx,[edx+RECT.bottom];edx = aAboutRectPaint.Registry
            mov  eax,[aAboutRectPaint.Email.right]
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow],ecx;edi =aAboutZonaButton.Registry.Rect;���� ������� ���������
            add  eax,ABOUT_INDENT*2*2*2;��� �� �� ����������� ����� ��������� ���� ������������ ��� ��������� ����� � ��������� ���
            mov  [edx+RECT.top],ebp;edx = aAboutRectPaint.Registry;ebp = aAboutRectPaint.Email.top
            lea  ecx,[ecx*2+eax]
            add  [edx+RECT.bottom],ebp;edx = aAboutRectPaint.Registry;ebp = aAboutRectPaint.Email.top
            add  [edx+RECT.right],ecx;edx = aAboutRectPaint.Registry
            mov  [edx+RECT.left],eax;edx = aAboutRectPaint.Registry

            ;���� ������� ���������
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow+4],eax;edi =aAboutZonaButton.Registry.Rect
            mov  [edi+ABOUT_ARRAY_ZONA.Arrow+8],ebp;edi =aAboutZonaButton.Registry.Rect

            ;���������� ���������� ��� ����� ������
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

            ;�������� �� ���� ������� ��� ��� ��������� ������� � ��������� ExtTextOutW ��� ������� ���� ������� �� ������� ����� ��������������
            dec  eax
            dec  ebp

            ;�������� ������ �����
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

        ;������� ���������� ���������� RECT
        ;!lprcDst - ��������� �� RECT ���������� ������������;!lprcSrc1 - ��������� �� RECT;!lprcSrc2 - ��������� ��  ������ RECT;!nCount - ���������� RECT � �������
        ;ebx = hDc current
        stdcall  UnionRectHL,aAboutRectPaintUnion,aAboutRectBmp,aAboutRectPaint,sizeof.ABOUT_RECT_PAINT/sizeof.RECT

         ;��������� �� ������ DC
         ;ebx = hDc current
         invoke  ReleaseDC,[hwnd],ebx;ebx = hDc current
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

;��������� ���������� ��� �������� � About
;----------------------------------------------
       ;����������
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

       ;������ �������
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*4],ecx
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*4],eax

       lea  ecx,[ecx+ebx*4]

       ;��������� � �������� ����� �������
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*0],edi
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*0],edx
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.x+sizeof.POINT*9],edi
       mov  [esi+ABOUT_ARRAY_ZONA.Arrow.y+sizeof.POINT*9],edx


       ;��������� ��� ������ ����� ���������� �� x ������ �������� ��������
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


;������� �������� ��������� ������
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

;������� �������� ����������� ��������� ������� ���������
;edi = SYS_STOCK
;----------------------------------------------
       mov  esi,[GetStockObject];esi = [GetStockObject]

   stdcall  esi,DC_PEN;esi = [GetStockObject]
            _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

       mov  [edi+SYS_STOCK.Pen],eax;edi = SYS_STOCK

   stdcall  esi,DC_BRUSH;esi = [GetStockObject]
            _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

       mov  [edi+SYS_STOCK.Brush],eax;edi = SYS_STOCK


            ;����������
            xor  ebx,ebx

;������� ����� ���� ������ �����
;ebx = 0
;----------------------------------------------
        invoke  FindWindowExW,ebx,ebx,szShell_TrayWnd,ebx;ebx = 0
           mov  [hwndShell_Tray],eax

;�������� ������������  ComboBox ��� ������ �����
;����� ��������� ��� ��� ��� ������ �������������� ������ ������ � ��� ������� ����� ��� ������������ ������ ����������� �� �� ����
;��� ��� � ����� ��� �������  - ������� ������ ��� �� ���� ������ � �� ����� ����� ��������� ����
;ebx = 0
;----------------------------------------------
            mov  ebp,[aArrayHwndFont.hwndComboBoxLang];ebp =[hwndComboBoxLang]
            mov  esi,[SendMessageW];esi = [SendMessageW]
        stdcall  esi,ebp,CB_ADDSTRING,ebx,lpCOMBOBOX_BELARUSSIAN;ebx = 0;ebp =[hwndComboBoxLang];esi = [SendMessageW];Index = 0
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���

        stdcall  esi,ebp,CB_ADDSTRING,ebx,lpCOMBOBOX_ENGLISH;ebx = 0;ebp =[hwndComboBoxLang];esi = [SendMessageW];Index = 0
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���

        stdcall  esi,ebp,CB_ADDSTRING,ebx,lpCOMBOBOX_RUSSIAN;ebx = 0;ebp =[hwndComboBoxLang];esi = [SendMessageW];Index = 1
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���

        stdcall  esi,ebp,CB_SETCURSEL,[aIniFileData.aIni.Language],ebx;ebx = 0;ebp =[hwndComboBoxLang];esi = [SendMessageW]
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���

;�������� ������������  SysTab
;����� ��������� ��� ��� ��� ������ �������������� ������ ������ � ��� ������� ����� ��� ������������ ������ ����������� �� �� ����
;��� ��� � ����� ��� �������  - ������� ������ ��� �� ���� ������ � �� ����� ����� ��������� ����
;esi = [SendMessageW] ;ebx = 0
;----------------------------------------------
;������� �������
            lea  edi,[!aAddTcItem];edi =[!aAddTcItem]
            mov  ebp,[aArrayHwndFont.hwndSysTab];ebp =[hwndSysTab]
            mov  [edi+TCITEMW.mask],ebx;ebx = 0;��� ��� ������������� ������� ��� ������ �� ������� � ���� ������
index 0
          indexM  SYSTAB_CONTROL_KEYCHANGE_Id
        stdcall  esi,ebp,TCM_INSERTITEMW,SYSTAB_CONTROL_KEYCHANGE_Id,edi;ebp =[hwndSysTab];esi = [SendMessageW]edi =[!aAddTcItem]
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���


          indexM  SYSTAB_CONTROL_ABOUT_Id
        stdcall  esi,ebp,TCM_INSERTITEMW,SYSTAB_CONTROL_ABOUT_Id ,edi;ebp =[hwndSysTab];esi = [SendMessageW]edi =[!aAddTcItem]
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���
endi
;������������� ������� �������

            mov  edi,[aIniFileData.aIni.idTabCurent];edi = [aIniFileData.aIni.idTabCurent] - ������ ������� ������� ������ �������
        stdcall  esi,ebp,TCM_SETCURSEL,edi,ebx;ebp =[hwndSysTab];esi = [SendMessageW];ebx = 0 edi = [aIniFileData.aIni.idTabCurent]
                 _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���

;������������� ����������� �� Button �������������/������������
;esi = [SendMessageW] ;ebx = 0
;----------------------------------------------

           ;������ ��� ������ ����������������� � ������� �������� � ����� �� ��� ���, ���� �� ����� - ����
        invoke  WaitForSingleObject,[hEventThread],INFINITE
                _LastErrorNotSigned;;���� �� ����� ��������� ������� �� ������ ���

           mov  edi,aIniFileData;edi = aIniFileData
           mov  ecx,[hiconPlay]
           mov  edx,[hiconPause]
           cmp  [edi+INIFILEDATA.aIni.iSuspendPlayHook],ebx;ebx = 0
         cmove  ebp,ecx;ebp = ���� ������
         cmove  ecx,edx
        cmovne  ebp,edx;ebp = ���� ������
           mov  [edi+INIFILEDATA.aIni.iSuspendPlayHook],ecx;edi = aIniFileData
            je  .NotHook

        invoke  PostThreadMessageW,[iThreadIdHook],WM_APP_HOOK_Play,ebx,ebx;ebx = ����
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

     .NotHook:

       stdcall  esi,[hwndButtonActivSusp],BM_SETIMAGE,IMAGE_ICON,ebp;esi = [SendMessageW];ebp = ���� ������

;�������� ������������  ListView
;esi = [SendMessageW] ;ebx = 0
;----------------------------------------------
;������������� ���������� �����
            mov  ebp,[aArrayHwndFont.hwndListView];ebp = [aArrayHwndFont.hwndListView]
        stdcall  esi,ebp,LVM_SETEXTENDEDLISTVIEWSTYLE,ebx,LVS_EX_GRIDLINES or LVS_EX_FULLROWSELECT ;esi = [SendMessageW];ebp = [aArrayHwndFont.hwndListView];ebx = 0


;������������� ImageList  ��� ��������� ���������
        stdcall  esi,ebp,LVM_SETIMAGELIST,LVSIL_STATE,[hImageListState] ;esi = [SendMessageW];ebp = [aArrayHwndFont.hwndListView];ebx = 0

;��������� �������
            lea  edi,[!aAddColum]
            mov  [edi+LVCOLUMNW.mask],LVCF_FMT  or LVCF_TEXT or  LVCF_WIDTH
            mov  [edi+LVCOLUMNW.fmt],LVCFMT_CENTER
            mov  [edi+LVCOLUMNW.cx],100;�����  ����� ��� ����������� ��������, ����� ���� �� ���������� �� �� ����������� �������
            mov  [edi+LVCOLUMNW.pszText],szButtonClass;����� �������� - ����� ����������


index 0
          indexM  LIST_VIEW_COLUM_ORIGINAL_Id
         stdcall  esi,ebp,LVM_INSERTCOLUMNW,LIST_VIEW_COLUM_ORIGINAL_Id,edi;ebp = [aArrayHwndFont.hwndListView];esi = [SendMessageW]
                  _LastErrorNotSigned;���� �� ����� ��������� ������� �� ������ ���

          indexM  LIST_VIEW_COLUM_NEW_Id
         stdcall  esi,ebp,LVM_INSERTCOLUMNW,LIST_VIEW_COLUM_NEW_Id,edi;ebp = [aArrayHwndFont.hwndListView];esi = [SendMessageW]
                  _LastErrorNotSigned;���� �� ����� ��������� ������� �� ������ ���


;��������� ������� ��� ���� ��� �� ����� ������� � �������� � ���� �������� ������������ �� ������
          indexM  LIST_VIEW_COLUM_EMPTU_Id
         stdcall  esi,ebp,LVM_INSERTCOLUMNW,LIST_VIEW_COLUM_EMPTU_Id,edi;ebp = [aArrayHwndFont.hwndListView];esi = [SendMessageW]
                  _LastErrorNotSigned;���� �� ����� ��������� ������� �� ������ ���
endi
                  ;������ = ���� ��� ��� ������� ������� �����, � ������� ����� ������ = 0
         stdcall  esi,ebp,LVM_DELETECOLUMN,ebx,ebx;ebp = [aArrayHwndFont.hwndListView];esi = [SendMessageW] ;ebx = 0
                  _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

         ;�������� ������
         stdcall  esi,ebp,LVM_ENABLEGROUPVIEW,1,ebx;ebx = 0
                  _LastErrorNotSigned;���� �� ����� ��������� ������� �� ������ ���


;��������� ������ ����� � ���-����� � �����
;esi = [SendMessageW] ;ebp = [aArrayHwndFont.hwndListView]

             ;��������� ������
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
                  _LastErrorNotSigned;���� �� ����� ��������� ������� �� ������ ���

             cmp  ebx,aListViewGroup;ebx = GROUPLISTVIEW
             jne  .LoopGroup


             xor  eax,eax
             lea  edi,[!aAddItem];edi =  !aAddItem
             dec  eax
             mov  ebx,aListViewItem ;ebx = ITEMLISTVIEW
             mov  [edi+LVITEMW.iItem],eax;-1
             mov  [edi+LVITEMW.stateMask],eax;-1


  .LoopItemText:;��������� ����� � ��� ����� ������

             lea  eax,[!aBufferString]
             mov  ecx,[ebx+ITEMLISTVIEW.stateItem]
             mov  edx,[ebx+ITEMLISTVIEW.idGroup]
             mov  [edi+LVITEMW.mask],LVIF_TEXT or LVIF_STATE or LVIF_GROUPID
             inc  [edi+LVITEMW.iItem]
             mov  [edi+LVITEMW.iSubItem],0;  - ������ ���� �������� � �������� Item ��������� � ����� ������ �� ������ = 0 ��� ��� � ������ ��������� ��� ������ = 0
             mov  [edi+LVITEMW.state],ecx
             mov  [edi+LVITEMW.pszText],eax
             mov  [edi+LVITEMW.iGroupId],edx

         ;��������� ��� �������� ������� ���������� ��������������� ������ �� ��������� vkCode
         stdcall  VkCodeInString,[ebx+ITEMLISTVIEW.vkCodeItemOne],[ebx+ITEMLISTVIEW.vkCodeItemTwoThree],eax

         stdcall  esi,ebp,LVM_INSERTITEMW,0,edi;esi = [SendMessageW] ;ebp = [aArrayHwndFont.hwndListView];edi =  !aAddItem
                  _LastErrorNotSigned;���� �� ����� ��������� ������� �� ������ ���


             mov  [edi+LVITEMW.mask],LVIF_TEXT; or LVIF_STATE
             mov  [edi+LVITEMW.iSubItem],1;LIST_VIEW_COLUM_NEW_Id  - ������ ���� �������� � �������� SubItem ��������� � ����� ������ �� ������ = 1 ��� ��� 2 ������ �������� ����

             mov  ecx,[ebx+ITEMLISTVIEW.vkCodeSubOne]
             mov  edx,[ebx+ITEMLISTVIEW.vkCodeSubTwoThree]
             cmp  ecx,VK_LAST
              ja  .VK_LAST


         ;��������� ��� �������� ������� ���������� ��������������� ������ �� ��������� vkCode
             lea  eax,[!aBufferString]
         stdcall  VkCodeInString,ecx,edx,eax
             jmp  .SetSubText

       .VK_LAST:
             add  edx,[ebx+ITEMLISTVIEW.scanCodeSubOne]
             mov  [edi+LVITEMW.pszText],edx

    .SetSubText:
             mov  ebx,[ebx+ITEMLISTVIEW.lpNextItem];ebx = ITEMLISTVIEW
         stdcall  esi,ebp,LVM_SETITEMW,0,edi;esi = [SendMessageW] ;ebp = [aArrayHwndFont.hwndListView] ;edi =  !aAddItem
                  _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���


             cmp  ebx,aListViewItem;ebx = ITEMLISTVIEW
             jne  .LoopItemText

             mov  eax,[edi+LVITEMW.iItem]
             add  [iCountIndexItem],eax

;������������� ����� ��� �������� ����
;esi = [SendMessageW]
;----------------------------------------------
            mov  ebp,[aSysStock.hfont];ebp = Font
            mov  ebx,-(sizeof.HWNDFONT/4);� ����� ������������� ���� �������� ����� ������� ���������� �  HWNDFONT ��������� �� ��� ����� ���������� ����
            mov  edi,aArrayHwndFont+ sizeof.HWNDFONT

            @@:
        stdcall  esi,[edi+ebx*4],WM_SETFONT,ebp,0;ebp = Font;esi = [SendMessageW]
            inc  ebx
            jnz  @b


;������������� �����
;----------------------------------------------
        stdcall  SetText

;������ �������� �� ���� ������� ������� � ������� ��������  ��� ������� ��� �� ���������� ������� ����������� ���� �������
;----------------------------------------------
            mov  eax,[aIniFileData.aIni.idTabCurent];eax = [aIniFileData.aIni.idTabCurent]
           push  SW_SHOW
           call  dword[tbChangeTab+eax*4];eax = [aIniFileData.aIni.idTabCurent] - ������� �� ������� * 4 - � ������� � ���������� �� ������ �������

;��������� ����� ��������� �������� ����, ������ � ���������
;----------------------------------------------
            ;����������
            xor  ebx,ebx;ebx = 0
            mov  edi,[hwnd];edi = hwnd
            mov  esi,[PostMessageW];esi = PostMessageW

         invoke  SetWindowPlacement,edi,aIniFileData.aIni.aWindPlac;edi = hwnd
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

;�������� ��������� �������� ���� ��� �������� ������� ��������� ��� ���
;ebx = 0;edi = hwnd;esi = PostMessageW
;----------------------------------------------
        .lpEmptyGuardianStart:;������ ������� ������ ��������
        stdcall  esi,edi,WM_APP_LICENSE,ebx,ebx;esi = PostMessageW;edi = hwnd;ebx = 0
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

;�������� ��������� �������� ���� ��� �������� ����������� ���������
;ebx = 0;edi = hwnd;esi = PostMessageW
;----------------------------------------------
        stdcall  esi,edi,WM_APP_ACTION,VK_TESTCRC32,ebx;esi = PostMessageW;edi = hwnd;ebx = 0
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
                 freeJmp;��� �� �� ���� ret, �� ����������� ���� ���� ����...
endp

;���� ���������� �� ��������� ���� ��������� �������� ������
;ebx = 0
;----------------------------------------------
            ;����������
            mov  esi,aMsg;esi = aMsg

           @@:
        invoke  GetMessageW,esi,ebx,ebx,ebx;ebx = 0;esi = aMsg
          test  eax,eax;���� eax  = 0, ������ �������� ��������� WM_QUIT - ������� �� ����������
            jz  ExitAppss;ebx = 0

        invoke  DispatchMessageW,esi;esi = aMsg
           jmp   @b

;������� ���, ��� ���� ������� ����� ������� �� ����������
;ebx = 0
;----------------------------------------------
proc    ExitAppss
  locals
   !iNumberOfBytesWritten     dd  ?
   !iSizeof.FILEALLSTRUCT     dd  ?
  endl

;��������� ���������� ���� ��� ��� ��� ��������� ��������� � ���� ����� ��� ����� ������� ������ �� ������������ ���������� ��� ������� � ���������� ������
;ebx = 0
;----------------------------------------------
           mov  esi,[UnhookWindowsHookEx];esi = [UnhookWindowsHookEx]
            or  ebx,[hhookKeyboard]
            jz  .UnhookInValue

       stdcall  esi,ebx;esi = [UnhookWindowsHookEx]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

       stdcall  esi,[hhookMouse];esi = [UnhookWindowsHookEx]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

.UnhookInValue:
           xor  ebx,ebx
            or  ebx,[hhookInValueMouse]
            jz  .SaveFileKsm

       stdcall  esi,ebx;esi = [UnhookWindowsHookEx]
                _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

       stdcall  esi,[hhookInValue];esi = [UnhookWindowsHookEx]
                 _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���


;�������� � ���� ��� �������� ������� � ��������
;----------------------------------------------
       .SaveFileKsm:
         ;���������� ������� �������� ���������� �����
                 mov  ecx,[lpEndRoaming];ecx = [lpEndRoaming]
                 mov  esi,aListViewGroup;esi = aListViewGroup
                 mov  edi,aListViewItem;edi = aListViewItem
                 mov  edx,esi
                 mov  ebp,-(sizeof.GROUPLISTVIEW+sizeof.ITEMLISTVIEW)+sizeof.FILEALLSTRUCT;ebp = sizeof.FILEALLSTRUCT
                 xor  eax,eax

                 ;������������ ����������  �����
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

         ;���������� ������� �������� ���������� ������
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
                 cmp  ebp,sizeof.FILEALLSTRUCT;������ ������ ��������� �� ����, �� ���� ��� �������� ���������� ��� - ���� ������� ���
                  je  .DeleteFileKsi

         ;������� ������ ��� sizeof.allstruct
         ;edi = aListViewItem;esi = aListViewGroup;ebp = sizeof.FILEALLSTRUCT
                 mov  [!iSizeof.FILEALLSTRUCT],ebp
              invoke  HeapAlloc,[hheap],0,ebp;ebp = sizeof.FILEALLSTRUCT
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
                 mov  ebx,eax;ebx = lpFILEALLSTRUCT
                 lea  ebp,[eax+sizeof.FILEALLSTRUCT];ebp = curent FILEALLSTRUCT

                 ;�������� ������ ������
                 mov  [ebx+FILEALLSTRUCT.iVersionRules],equVersionKsmRules;ebx = lpFILEALLSTRUCT

         ;��������� � ��������� ����� � ������ ��� ������������ ���������� � ����
         ;esi = aListViewGroup;ebx = lpFILEALLSTRUCT;ebp = curent FILEALLSTRUCT
                 xor  edi,edi;edi = icountGroup

                 @@:
                 ;esi = aListViewGroup
                 mov  esi,[esi+GROUPLISTVIEW.lpNextGroup]
                 cmp  esi,aListViewGroup
                  je  @f

                 mov  eax,[esi+GROUPLISTVIEW.lengthNameGroup];����� � �������� ������� ���� ����� ���������
                 mov  edx,ebp;ebp = curent lpallstruct
                 lea  ecx,[eax*2+sizeof.GROUPLISTVIEW]
                 lea  ebp,[ebp+eax*2+sizeof.GROUPLISTVIEW];ebp = curent FILEALLSTRUCT
                 inc  edi;edi = icountGroup

             stdcall  MoveMemoryHL,edx,esi,ecx;�������� ���������
                 jmp  @b

                 @@:
                 mov  [ebx+FILEALLSTRUCT.icountGroup],edi;ebx = lpFILEALLSTRUCT;edi = icountGroup ;��������� ���������� ��������
                 mov  esi,aListViewItem;esi = aListViewItem

         ;��������� � ��������� ������ � ������ ��� ������������ ���������� � ����
         ;esi = aListViewItem;ebx = lpFILEALLSTRUCT;ebp = curent FILEALLSTRUCT
                 xor  edi,edi;edi = icountItem

                 @@:
                 ;esi = aListViewItem
                 mov  esi,[esi+ITEMLISTVIEW.lpNextItem]
                 cmp  esi,aListViewItem
                  je  @f

                 xor  eax,eax
                 cmp  [esi+ITEMLISTVIEW.vkCodeSubOne],VK_LAST
               cmova  eax,[esi+ITEMLISTVIEW.scanCodeSubTwoThree];������ ��� ��������� � ������
                 mov  edx,ebp;ebp = curent lpallstruct
                 lea  ecx,[eax+sizeof.ITEMLISTVIEW]
                 lea  ebp,[ebp+eax+sizeof.ITEMLISTVIEW];ebp = curent FILEALLSTRUCT
                 inc  edi;edi = icountItem

             stdcall  MoveMemoryHL,edx,esi,ecx;�������� ���������
                 jmp  @b

                 @@:
                 mov  [ebx+FILEALLSTRUCT.icountItem],edi;ebx = lpFILEALLSTRUCT;edi = icountItem ;��������� ���������� ��������
                 xor  ebp,ebp;ebp = 0
                 mov  edi,[!iSizeof.FILEALLSTRUCT];edi = ������ ����������� ��������

              ;������ ���� .ksm ��� ���������� ����� ������
              ;ebx = lpFILEALLSTRUCT;edi = ������ ����������� ��������;ebp = 0
              invoke  CreateFileW,szRoamingAppData,FILE_GENERIC_WRITE or FILE_GENERIC_READ,ebp,ebp,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,ebp;ebp = 0
                 mov  esi,eax;esi = ����� �����
                 inc  eax;INVALID_HANDLE_VALUE
                  jz  .SaveFileKsi;�� �������� ������� ����

                 ;��� ������� ����� ��������� CRC32c
                 lea  eax,[edi-sizeof.FILEALLSTRUCT.iCRC32c];edi = ������ ����������� ��������
                 lea  ecx,[ebx+sizeof.FILEALLSTRUCT.iCRC32c];ebx = lpFILEALLSTRUCT
             stdcall  CRC32cHL,ecx,eax,-1
                 mov  [ebx+FILEALLSTRUCT.iCRC32c],eax

                 lea  eax,[!iNumberOfBytesWritten]
              invoke  WriteFile,esi,ebx,edi,eax,ebp;esi = ����� �����;ebp = ����;ebx = lpFILEALLSTRUCT;edi = ������ ����������� ��������
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���
                 jmp  .SaveFileKsi

     .DeleteFileKsi:
              invoke  DeleteFileW,szRoamingAppData

;�������� ��������� � .ksm ����
;----------------------------------------------
       .SaveFileKsi:;���������� ������ ������ ���� ������ � ����� �������, ��� �� �� ���������� ���������� ��� ���� �������� ���
                 xor  ebx,ebx;ebx = ����
                 mov  edi,aIniFileData;edi =  aIniFileData
                 mov  esi,[lpEndRoaming];esi = [lpEndRoaming]

               ;�������� iSuspendPlayHook � ������ ���� ����� ����  �� ������ ���������� ������ = [hiconPlay], ������ ������ iSuspendPlayHook= 0 � ���� �� �������
               ;ebx = ����;edi =  aIniFileData;esi = [lpEndRoaming]
                 mov  eax,[hiconPlay]
                 cmp  [edi+INIFILEDATA.aIni.iSuspendPlayHook],eax
              cmovne  eax,ebx;ebx = ����
                 mov  [edi+INIFILEDATA.aIni.iSuspendPlayHook],eax

                 ;������������ ����������  �����
                 mov  word[esi-2],'i';esi = [lpEndRoaming]

              ;��������� ��������� � ����
              ;ebx = ����;edi =  aIniFileData
              invoke  CreateFileW,szRoamingAppData,FILE_GENERIC_WRITE or FILE_GENERIC_READ,ebx,ebx,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,ebx;ebx = 0
                 mov  esi,eax;esi = ����� ������ ini �����
                 inc  eax;INVALID_HANDLE_VALUE
                  jz  .Exit;�� �������� ������� ini ����;ebp = ����� ����;ebx = ����

                 ;��� ������� ����� ��������� CRC32c
             stdcall  CRC32cHL,edi,sizeof.INIFILEDATA - sizeof.INIFILEDATA.CRC32,-1
                 mov  [edi+INIFILEDATA.CRC32],eax

                 lea  eax,[!iNumberOfBytesWritten]
              invoke  WriteFile,esi,edi,sizeof.INIFILEDATA,eax,ebx;esi = ����� ������ ini �����;ebx = ����;edi =  aIniFileData
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���


;��� ��� ����� �������  ������� �� ���������
;----------------------------------------------
          .Exit:
                 ;����������
                 xor  ebx,ebx;ebx = 0

                 ;�������� ���� �� ������� ��������� � ����������� �����
                 cmp  [iBoolMessageReg],ebx;ebx = 0
                  je  .NotMessage

              ;������� ���������
             stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_HACKING
              invoke  MessageBoxExW,ebx,eax,ebx,MB_OK or MB_ICONWARNING or MB_SYSTEMMODAL,ebx;ebx = 0
                      _LastErrorNotNull;���� �� ���� ��������� ������� �� ������ ���

        .NotMessage:
              invoke  ExitProcess,eax;eax - ���� �� ������

            ;ret - �������� � ������ �� �����
endp

;----------------------------------------------
include  'Inc\WindowsProc.inc'
include  'Inc\WindowsProcOther.inc'
include  'Inc\Proc.inc'


;----------------------------------------------
;��� ���������� ���������� �������
lpGuardianEnd:

;������� CRC32c ����������� �������
CRC32cInMacro  constCRC32cGuardian,Start,lpGuardianEnd - Start,0xFFFFFFFF;lpGuardianEnd - Start

;������� CRC32c ����������� ������� ��������
CRC32cInMacro  constCRC32cGuardianEmpty,Start.lpEmptyGuardianStart,WindowProc.lpEmptyGuardianEnd - Start.lpEmptyGuardianStart,\
               0xFFFFFFFF;WindowProc.lpEmptyGuardianEnd - Start.lpEmptyGuardianStart

;----------------------------------------------
section '.idata' import data readable writeable
include 'Include\My\Import.inc'

;----------------------------------------------
include  'Inc\Resource.inc'

;section '.reloc' data fixups readable

