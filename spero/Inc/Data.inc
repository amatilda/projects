constMySizeofRoaming   uniq  sizeof.szRoamingAppData - sizeof.tbRoaming - 0x2;0x2 - ��������� ����


struct  ABOUT_ARRAY_ZONA
  Rect                           RECT ABOUT_BORDER+ABOUT_BMP_Width_Height-ABOUT_INDENT*2;���� ��� � ������ ����� �� �  ���� RECT
                                 ;������ ����� ��� ���� ��� �� ���������� ����� ������������� ���� ������ �� ����������
  Point                          POINT ABOUT_BORDER+ABOUT_BMP_Width_Height-ABOUT_INDENT*2-1
  Any2                           POINT;�������� �� ���� ������� ��� ��� ��������� ������� � ��������� ExtTextOutW ��� ������� ���� ������� �� ������� ����� ��������������
  Any3                           POINT
  Any4                           POINT ABOUT_BORDER+ABOUT_BMP_Width_Height-ABOUT_INDENT*2-1
  Any5                           POINT ABOUT_BORDER+ABOUT_BMP_Width_Height-ABOUT_INDENT*2-1
  lpRectText                     dd  aAboutRectPaint.Email
  lpText                         dd  szInfoEmail
  iLeght                         dd  (sizeof.szInfoEmail-2)/2
  IdHelp                         dd  STR_HELP_Emeil;�������� �� ���� ������ ������
  Color                          dd  ?;���� ��� ���� �  ������� �������� ������ ����� � �������
  Adjustment                     dd  ?;�� ��� ��������� � ����� ��������� � Color - ��� ����������
  Indicator                      dd  ?;�������� �� ��� ���� ���������� �� ������ ��� ����� RECT � ��������� ����� ���� ������� ������ � ������������ ���� ����� ���� ����
  StringLeft                     dd  ?;���������� ���������� ������ ������ �� x � ������ �������
  Arrow                          POINT ?,ABOUT_BORDER+ABOUT_BMP_Width_Height
                                 db sizeof.POINT*9 dup(?)
ends


struct  ABOUT_RECT_ZONA
 Email                           ABOUT_ARRAY_ZONA;�������� �� ������ ������
 Registry                        ABOUT_ARRAY_ZONA <?,?,?,?>,<?,?>,<?,?>,<?,?>,<?,?>,<?,?>,aAboutRectPaint.Registry,szInfoRegUrl,\
                                                  (sizeof.szInfoRegUrl-2)/2,STR_HELP_Registry,?,?;�������� �� ������ �����

ends


struct  ABOUT_RECT_PAINT
 ;���������� ������������ NameVer � ������� About
 NameVer                        RECT  ABOUT_BORDER+ABOUT_BMP_Width_Height
 ;���������� ������������ Copyright � ������� About
 Copyright                      RECT ABOUT_BORDER+ABOUT_BMP_Width_Height,ABOUT_BORDER+ABOUT_BMP_Width_Height+SYSTAB_CONTROL_Height
 ;���������� ������������ Email � ������� About
 Email                          RECT ABOUT_BORDER+ABOUT_BMP_Width_Height
 ;���������� ������������ Registry � ������� About
 Registry                       RECT
ends


;��������� ini �����
;----------------------------------------------
  struct  INITILIZDATA
    aWindPlac                WINDOWPLACEMENT sizeof.WINDOWPLACEMENT,0,SW_SHOWNORMAL,<-1,-1>,<-1,-1>,<0,0,800,700>
    Language                 dd  COMBOBOX_ENGLISH
    idTabCurent              dd  SYSTAB_CONTROL_ABOUT_Id;idTabCurent  = Index item SysTab
    iRun                     dd  BST_UNCHECKED
    iSuspendPlayHook         dd  0;���� ���� �� image = Play � ���� �� �������
  ends


  struct  INIFILEDATA ;��������� ini ����� - ����������� ���������
    aIni                     INITILIZDATA;�� ������ ��������� ������ ���� ������
    szOpenExeBufer           du  MAX_PATH dup(?)
    szLaunchfileBufer        du  MAX_PATH dup(?)
    CRC32                    dd  ?;�� ������ ��������� ������ ���� ���������
  ends




;----------------------------------------------
section '.data' data readable writeable
_DebugDate; ��� ������� � ������ �� �����������

aRWKeyMap                               RTL_SRWLOCK              RTL_SRWLOCK_INIT;������������ ������ � ������ �������� ������ � ����� ���������� �������� ����

aVectoredExceptionHandler               VectoredExceptionHandler ;��������� ��� ����������
aWc                                     WNDCLASSEXW              sizeof.WNDCLASSEXW,CS_PARENTDC,WindowProc,0,0,?,?,0,COLOR_BTNFACE+1,0,szEventOneApp,?;���������� ��� �������� ����
aNotifyIconData                         NOTIFYICONDATAW_1        sizeof.NOTIFYICONDATAW,?,NOTIFY_ICON_Id,?,WM_APP_NOTIFY_ICON,?,?;�����������: uFlags � szTip

aListViewGroup                          GROUPLISTVIEW            LIST_VIEW_GROUP_GLOBAL_Id,0,szNameGroup,aListViewGroup,aListViewGroup

;(IMAGE_LIST_STATE_QUES_Id+1) shl 12; + 1 - ��� ��� ��� ���� ������  �� �� ���� � �� 1 � ������ �� List Image
aListViewItem                           ITEMLISTVIEW             (IMAGE_LIST_STATE_QUES_Id+1) shl 12,LIST_VIEW_GROUP_GLOBAL_Id,aListViewItem,aListViewItem

aOpenFileExe                            OPENFILENAMEW            sizeof.OPENFILENAMEW,?,0,?,0,0,0,?,sizeof.INIFILEDATA.szOpenExeBufer/2,0,0,0,0,\
                                                                 OFN_FILEMUSTEXIST or OFN_PATHMUSTEXIST or OFN_LONGNAMES or OFN_EXPLORER or OFN_HIDEREADONLY,0,0,0,0,0,0,0,0

aMenuStateChange                        MENUITEMINFOW        sizeof.MENUITEMINFOW;��� ��������� ���� ��������� ������;�������� ������ ������� - cbSize

aFont                                   LOGFONTW             30,0,0,0,FW_SEMIBOLD,0,0,0,DEFAULT_CHARSET,OUT_TT_ONLY_PRECIS,CLIP_DEFAULT_PRECIS,\
                                                              DEFAULT_QUALITY,DEFAULT_PITCH or FF_SWISS,<'Segoe UI',0>
;���������� ������������ bmp � ������� About
aAboutRectBmp                  RECT  ABOUT_BORDER,SYSTAB_CONTROL_Height+ABOUT_BORDER,ABOUT_BORDER+ABOUT_BMP_Width_Height,SYSTAB_CONTROL_Height+ABOUT_BORDER+ABOUT_BMP_Width_Height
aAboutRectPaint                ABOUT_RECT_PAINT;��� �������� � ����� ��������� - ��� RECT ������������ � About ���  ������
aAboutZonaButton               ABOUT_RECT_ZONA;��� �������� � ����� ��������� - ��� RECT ������������ � About ��� ��������� ����� ������ �����

lpMultiLanguage             ddA  tbMultiLanguage;��� �������� ������ �� ������;������� ������ ��� �� �� �������� ��� ������� �������� ��� ������� ���������� ������

iPlague                     ddA  1;������ ������ ���������;  ����� ���� ����� ��������� ��������; � �� ����� ���� ����� ��� ���������

iCountIndexItem             ddA  1;��� �������� ������  ����������\������� ������ �� ���� ������� ���������� �� ��� ��� �������� �� ���� ������ �� ��
                                  ;����� ����� ����� ������������� ������������ ����������  ����� ������������ ��� ���������� ��� �������� ������
;-----���������������� ������  - ������
aIniFileData               INIFILEDATA;���� ��������� ��������� c ini ����� � � ������ �� �� ���������� ��������� ���������
aKeySerial                 KEYFILEW;��� ���������
aMsg                       MSG;��������� ��� �������� ����� ��������� ���������.
aMsgForHook                MSG ;��� ��������� � ������ � ������
aShellExecute              SHELLEXECUTEINFOW;��� ���������� ������ � �������� ������ ����������� ������ ��� ������������ �����
aClientRect                RECT;������������ ���� ��� �� WM_WINDOWPOSCHANGED ����� ������ � ��������� - ������ �������� � ��� ��������� ������ ������ ��������� ����� �������� ����
aWindowRect                RECT;������������ � ��������� WM_WINDOWPOSCHANGED - ������ �������� � ��� ��������� ������ ������ �������� ����
aAboutRectPaintUnion       RECT;����� RECT ��� About ��� ��������������� �������� ��� �� ������ �� ��������� �������� RECT
aAboutFree                 ABOUT_FREE;��� �������� � ����� ��������� - �� ��� ����������� ����� '���������' ������� About
aTabCurrent                TAB_CURRENT;��� �������� � ����� ��������� - �������� �� ������ �������� � ������� �������
aSysStock                  SYS_STOCK;��� �������� � ����� ��������� - ������ ��������� ������� ���������
aLpAboutZonaButton         LP_ABOUT_ARRAY_ZONA;��� �������� � ����� ��������� - ������ �� ������� ����� ������ �������������� � �� ������� ������� ������������� � ������� About
aFlag                      FLAG;��� �������� � ����� ��������� - ������� ������ ��������� � ������ ��������� ���� -������
aMenuState                 MENU_STATE;��� �������� � ����� ��������� - ������� ��������� ���� � ������ ��������� ���� -������
aHookMinor                 HOOK_MINOR;��� �������� � ����� ��������� - ������� �������� ���������� ������ � ����������� ������ � ���������� � � ��� ���� ���������� � �������

iInputCount                ddA  ?;�������� �� ���������� �������� INPUT ������������ SendInput
lpaInputCurrent            ddA  ?;��������� �� ������� ���������  INPUT - aInput; ������������� � ���� � iInputCount
;������ �� 9 �������� INPUT, ��� �������� ����� ��������� 2 modif UP + 2 modif DOWN � 1 key DOWN + 2 modif UP � 1 key UP � 1 key (�� ���� �������)
aInput                     ddA  (sizeof.INPUT*9)/4 dup(?)

aSet                       KEYMAP;����� ��� ���������� ������� ������ � ������� �������� �������
aTo                        KEYMAP;����� ��� ���������� ������� ������ � ������� �������� �������

aHook                       HOOK;��� ������ ��� ���� ������� ������ ��������� ��������� ������� � ���������, ��� �������� ���������

aArrayHwndFont              HWNDFONT;��������� ��� ���� ������� ����  ������� ��������� �����

lpEndRoaming                ddA  ?;��������� �� ���� � szRoamingAppData
lpBufferDlgKeyMapScankode   ddA  ?;��������� ��  ��������� ����� � ����� ����� ��� ����� � Edit � ��� ��������� �������� ������ � �������� ���������� ��� ��� �������
lpaKeyPressedMapIn          ddA  ?;��������� �� ����� ��� Edit ��� ���������� ������� ������� �������������, ���� = 1 ������ ������������� ������ �� ������� vk,
lpBuyNameUser               ddA  ?;��������� �� ������������ ��������� ���������

iChangeCurrentListViewItemIndex  ddA  ?;id ����� ��������� ��� ��������������
lpChangeCurrentListViewItem      ddA  ?;������ �� ��������� ����� ��������� ��� ��������������

iBoolMessageReg                  ddA  ?;���� �� ����� ���� ����� ������� ������� ��������� ��� ����� ��������

iNumberNotLicense                ddA  ?;����� � ������� ����� �������� ������ ���� � ������� DlgProcNotLicense

iThreadIdHook                    ddA  ?;id ������� ������ � ������� �������� �������� ����

iIdForStatic               ddA  ?;ID ������ � �������� ��������;���� ��������� ������� �� ������ ������� ����� � ������� ������ �������� ��������� ��� ��� ��������

iTimerActivation           ddA  ?;���� ����� ���� - ������ �� ���������� ���� ���������� ���

msgTaskCreat               ddA  ?;���� �������� windows ��������������� ������ ��� ���������

hmenu                      ddA  ?;���� ��������
hmenuSubPopupNotify        ddA  ?;��� ���� ��� ������ � ����
hmenuSubAddItem            ddA  ?;��� ���� ��� List View

hicon                      ddA  ?;������ ��� ��������� ��������� ���� � ���� ��������
hiconPause                 ddA  ? ;���  list view ������
hiconPlay                  ddA  ?;���  list view ������
hiconAdd                   ddA  ?;������ ��� ������ ��� �������� ���������� ����������

hfontNotLicense            ddA  ?;��������� ����� ��� ������� DlgProcNotLicense - ��������������

hEventThread               ddA  ?;������� ������� �� ���� ����������� ������ ��� ���������� � � ������������� ��� �������� ������ �� ������ ����� ������� ���������
hInstance                  ddA  ?;���������� �������� ������
hheap                      ddA  ?;���������� ������ ����� ���������� ��� �������� ��������
hImageListState            ddA  ?;���� � �������� ��� list view

hwnd                       ddA  ?; ���������� �������� ����
hwndDeskop                 ddA  ?;���������� �������� �����
hwndButtonActivSusp        ddA  ?;������ ��������� ���������� ������
hwndShell_Tray             ddA  ?;������ ���� ����������� ������ � ����

hhookMouse                 ddA  ?;����� ���� ���� �������� �� ������ ������   ���������� ����������
hhookKeyboard              ddA  ?;����� ���� ������������� �������� �� ������ ������   ���������� ����������
hhookInValue               ddA  ?;����� ���� ��� ����������� ������� ������� ���������� ����������
hhookInValueMouse          ddA  ?;����� ���� ��� ����������� ������� ������� ���� ���������� ����������


;���� � Roaming � � ����� ������ � ���;������ ��������� ����� ���� ��������� ��� ��� ���� ���� ������� ��� ������������� � ���������� ��� �� ����� ��������
szRoamingAppData           duA  MAX_PATH dup(?)
