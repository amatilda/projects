macro  enumMenu num,[id,vkCode]
{ forward
   id uniq num + vkCode}

 ;������������ id;���� � ����� ���� �� ������������� �� �������������, �� � ���� � �� � �������� ����� ������� � �������� � +1
;----------------------------------------------
 enum  IDCANCEL+1,\;��������� �������� IDCANCEL - ��������� �� ��������� ����������� � ��������(��� �� ����� ���������...???)
       BUTTON_EXIT_Id,\
       BUTTON_OPTIONS_Id,\
       BUTTON_ACTIVSUSP_Id,\
       COMBOBOX_LANG_Id,\
       SYSTAB_CONTROL_Id,\
       LIST_VIEW_Id,\
       STATIC_HELP_Id,\
       DIALOG_NOTLICENSE_ONE,\
       DIALOG_NOTLICENSE_TWO,\
       DIALOG_NOTLICENSE_THREE,\
       DIALOG_NOTLICENSE_FOUR,\
       DIALOG_NOTLICENSE_REGINFO,\
       DIALOG_NOTLICENSE_LICENSE,\
       DIALOG_NOTLICENSE_COPYRIGHT,\
       DIALOG_NOTLICENSE_RANDOM,\
       DIALOG_KEYMAP_EDIT_SET_Id,\
       DIALOG_KEYMAP_EDIT_TO_Id,\
       DIALOG_KEYMAP_BUTTON_ADD_Id,\
       DIALOG_KEYMAP_STATIC_HELP_Id,\
       DIALOG_OPTIONS_BUTTON_AVTO_START_Id


 ;�������� � +1 ��� �� �� ���������� �������� c ���������� MENU_HELP_LAST_Id
 ;��� ��������� ��������� WM_MENUSELECT � ������ id ��� ����
;----------------------------------------------
 enum  MENU_HELP_LAST_Id+1,\;MENU_HELP_LAST_Id - ������ ������� - ��������� id ���� � ����������� �� ������� ��� ������ - ������������ ��� �� �� �������� ������ ��� ���� ��� ����
       MENU_SHOW_HIDE_Id,\
       MENU_ACTIVATE_Id,\
       MENU_EXIT_Id,\
       MENU_TABLE_ADD_Id;� ���� ������ ���� ����� ������ ��������� ������ ����

 ;������������ ���������� ��� ������ ������� ������ �������� �� �������� MENU_TABLE_KEY_Id + vkCode ��� ����� ��� ������� ���� ������� ���� vkCode ����� �������� ����� MENU_TABLE_KEY_Id
 enumMenu MENU_TABLE_KEY_Id,\;��������� ������� ��� ����  - ���� ��������� �� ����� ���
          MENU_ADD_2LVL_MOUSE_1Id,VK_LBUTTON,\
          MENU_ADD_2LVL_MOUSE_2Id,VK_RBUTTON,\
          MENU_ADD_2LVL_MOUSE_3Id,VK_MBUTTON,\
          MENU_ADD_2LVL_MOUSE_4Id,VK_XBUTTON1,\
          MENU_ADD_2LVL_MOUSE_5Id,VK_XBUTTON2,\
          MENU_ADD_2LVL_MOUSE_6Id,VK_MOUSEWHEEL_UP,\
          MENU_ADD_2LVL_MOUSE_7Id,VK_MOUSEWHEEL_DOWN,\
          MENU_ADD_2LVL_MOUSE_8Id,VK_MOUSEWHEEL_RIGHT,\
          MENU_ADD_2LVL_MOUSE_9Id,VK_MOUSEWHEEL_LEFT,\
          MENU_LAST_ID,VK_LAST;��������� id ���� �������� "�������"  ��������� ����� ��� ����������� ���� ���� �� ����� ������ ���������


;----------------------------------------------
 enum  0xFF+1,\;��������� �������� - ��� ����������� ���� ��� ������ vk ����������� � ���� ������� �������� �������� 0xFF+1, ��� �� ��� ���� � ��� �� �����������
       VK_MOUSEWHEEL_UP,\;(������
       VK_MOUSEWHEEL_DOWN,\;����
       VK_MOUSEWHEEL_RIGHT,\;������
       VK_MOUSEWHEEL_LEFT,\;�� ���������)
       VK_LAST,\;��� ��� ����� ������ ��������� � ���� ��� ���� �������� ��� ����� ������������ ��� �������  � ��� ��������
       VK_FILE_OPEN,\;����=vkCodeSubOne,����� vkCodeSubTwoThree = ��������� �� ��������� � ������ � �������,scanCodeSubOne = ������ � ������ ����������� ������� ������ �� ������ (0 ����),\
       VK_LOCK_KEY,\;scanCodeSubTwoThree = ������ ���� ���������(0 ���� + ����� ������ � ������)
       VK_SCRSAV,\;\----\
       VK_OFFUNIV,\;\----\
       VK_TESTCRC32;��� �������� ����������� ���������


;----------------------------------------------
 enum  WM_APP+1,\;��������� ��������
       WM_APP_NOTIFY_ICON,\
       WM_APP_LICENSE,\
       WM_APP_ACTION,\;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = lp Action Date - ���������� ����� ���� ������
       WM_APP_HOOK_Play,\
       WM_APP_HOOK_Stop,\
       WM_APP_HOOK_ONE_KEY,\;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW
       WM_APP_HOOK_ONE_KEY_RELEASED,\;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW
       WM_APP_HOOK_COMB_ONE_KEY_RELEASED,\;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = [ITEMLISTVIEW.scanCodeSubOne]
       WM_APP_HOOK_COMBINATION_KEY;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW


;MENU_NOTIFU_Id
;MENU_ADDITEM_Id

;SYSTAB_CONTROL_KEYCHANGE_Id
;SYSTAB_CONTROL_ABOUT_Id

;LIST_VIEW_COLUM_ORIGINAL_Id
;LIST_VIEW_COLUM_NEW_Id

;IMAGE_LIST_STATE_NOTCHECK_Id
;IMAGE_LIST_STATE_CHECK_Id
;IMAGE_LIST_STATE_QUES_Id



NOTIFY_ICON_Id                 uniq  WM_APP + 1


LIST_VIEW_GROUP_GLOBAL_Id      uniq  0

;----------------------------------------------
equCopyright                   equ  'Copyright ',0A9h,' 2018 Pavel Kulaga. All rights reserved.'
equNameApp                     equ  'Spero';�� ����� �� �� ������� �������� �����, �� ��� ��� ��������� � ������ ������� ����������
equVersionKsmRules             equ  'v1';����� ������� ������ ��������� �������� ���������� ������
equVersionKeyRules             equ  '1.0';����� ��������� ��� ������ ��� ����� �������������� �������� � ������ �������� ������� � ������ ��������� � ������ ������� ���������� �� ��������
equVersionLang                 equ  '1.0';����� ������ �������� ��������� ����� ������ ��� ��������
equVersionApp                  equ  '1.0'
equMailSupport                 equ  'support@hlapp.net'

constTimeGuardianMin           uniq  60000*30;30 min ;����� �� ����� ��������� �������� ������;60000 = 1 min

constNumberN                   uniq  0xB3A9E983;��� RSA; ����� ��� ����� ��� ����� ����������� ���������� ���� �� ������ ������������ ����
constNumberE                   uniq  257;��� RSA

constLengthKeyPressedMap       uniq  (VK_LAST+1+3)/4  ;+1 �� ������� ������;+3 ��� �� ��� ������� �� 4 �� �������� �����;���������� dword � ������   aKeyPressedMap

;----------------------------------------------
BUTTON_EXIT_Width              uniq  102
BUTTON_EXIT_Height             uniq  31

BUTTON_OPTIONS_Width           uniq  102
BUTTON_OPTIONS_Height          uniq  31

STATIC_HELP_Height             uniq  31

BUTTON_ACTIVSUSP_Width         uniq  102
BUTTON_ACTIVSUSP_Height        uniq  31

COMBOBOX_LANG_Width            uniq  100
COMBOBOX_LANG_Height           uniq  30

SYSTAB_CONTROL_Height          uniq  COMBOBOX_LANG_Height + 10

ABOUT_BMP_Width_Height         uniq  64;������ � ������ bmp ��� ABOUT � ��������
ABOUT_BORDER                   uniq  10;������ ��� �����
ABOUT_INDENT                   uniq  10;������ ��� ������

LIST_VIEW_STATE_Width_Height   uniq  32
LIST_VIEW_STATE_MIN_MAX        uniq  3

;----------------------------------------------
struct FILEALLSTRUCT
 iCRC32c                    dd  ?;������ ����������
 iVersionRules              dd  ?
 icountGroup                dd  ?
 icountItem                 dd  ?
;��� ���� ������� ���������� ��������� �����, � ����� ������
;������ ������ ��������� ���� ����� ����� ���������
;....................

ends

struct FLAG
 Activate                 dd  ?;��������� ������� ������� ���� ��� ���, ���� ����� ���� ������ �� �������
ends

struct MENU_STATE
 Item                    dd  ?;���������� ��������� � ������� ���������� ������ ���� ������������ � ������ � ���� �� �� ������ �� ��������, ���� = 0 ������ ���� ������������
 Group                   dd  ?;���������� ��������� � ������� ���������� ������ ���� ������������ � ������� � ���� �� �� ������ �� ��������, ���� = 0 ������ ���� ������������
 NotifySnowHide          dd  ?;������������ ���� ��� shellnotify - ���������� ����� Snow ��� Hide;���� ����� ���� ����� Snow
 ActivSusp               dd  ?;������������ ����� ���� MENU_ACTIVATE_Id

 iForStaticMenu          dd  ?;��������� �������� ��������� WM_MENUSELECT �� ���� ���������� �������� ���������
 hmenuAddAction          dd  ?;���� ��� ������� ��� �������� ��������� ��� �������� ������� ����������
 hmenuSubAddAction0Lvl   dd  ?;��� ���� ��� ��� TrackPopupMenuEx �� �������� � ����
 hmenuSubAddManual1Lvl   dd  ?;��� ���� ������� ������ ���� �� ����� ����� ������ ��������� ��� ��� ����
 hmenuSubAddMonitor1Lvl  dd  ?;��� ���� ������� ������ ���� �� ����� ����� ������ ��������� ��� ��� ����
 SubAddActionModif_1     dd  ?;��� �������� ����� ������������ ����� ������������ ��� ���������� ��������� ������������� ������
 SubAddActionModif_2     dd  ?;��� �������� ����� ������������ ����� ������������ ��� ���������� ��������� ������������� ������
 hwndAddAddKeysStatic    dd  ?;�������� ���������� ������ ������� �� ������� DlgProcAddKeys
ends

struct  SYS_STOCK
 rgbBTNFACE               dd  ?;���� ������ �������� ����
 rgbBTNTEXT               dd  ?;���� ������ �������� ����
 rgbHOTLIGHT              dd  ?;���� �����������
 rgb3DLIGHT               dd  ?;���� ����� ��� ������ � �������

 Pen                      dd  ?;����������� pen  ����������� ������ ���� ����
 Brush                    dd  ?;����������� Brush  ����������� ������ ���� ����

 hfont                     dd  ?;�������� ����� ������� �� ��������������
ends

struct  LP_ABOUT_ARRAY_ZONA
 Point                 dd  ?;��������� �� ������� ABOUT_ARRAY_ZONA ���������� �� ����� ������ ������ �� ������� About
 UpDown                dd  ?;��������� �� ������� ABOUT_ARRAY_ZONA ���������� �� �������� ������������� ������� ��� ������� ������� About
ends

struct  TAB_CURRENT;��������� �� ������� ����������� ���������� �� ������� �������
 Move             dd  ?
 Paint            dd  ?
 MouseMove        dd  ?
 MouseLeftUpDown  dd  ?
 Activate         dd  ?
ends

struct  ABOUT_FREE
 hbmpAbout                  dd  ?
 hfont30SEMIBOLD            dd  ?;��������� ����� ��� ������� About - ��������������
ends


struct KEYFILEW
 iCRC32File                   dd  ?;����� ��������� �RC32� ����� � ���������� ������� ������� ������ � ��������
 iDigitalSignatureLow         dd  ?;������ ����� �������� �������
 iCRC32ForKey                 dd  ?;��� ����� ������� � �������� ��������
 iDigitalSignatureHiw         dd  ?;������� ����� �������� �������
 lengthszNameUser             dd  ?;����� � �������� ��� ���� ������� � ����� ������������
 szNameUser                   du  50 dup(?);����� ������ ������ ��������� �� ������������ - ��� � �������
ends



struct  GROUPLISTVIEW
 idGroup                    dd  ?; = id ������ � �������� �������������� ����� ����������� � ���� ������
 lengthNameGroup            dd  ?;����� ����� ������ � �������� ������� ����
 lpszNameGroup              dd  ?; ��������� �� ��� ������
 lpPrevGroup                dd  ?
 lpNextGroup                dd  ?
ends


;����������� ����� vkCodeItemOne � vkCodeSubOne ������������ ���� ����� �� ������ �������� ��� ��� ��� ������������� ��� ������������ � ������� MakeModifeter
;������ ����� ����� ���� �������� ������������
struct  ITEMLISTVIEW
 stateItem                  dd  ?;= ��������� �������� ��� ��������� ��� ����� ��� ���, ��� �� ��� ����������� �����
 idGroup                    dd  ?; = id ������ ����� ���� ���� �������������
 lpPrevItem                 dd  ?;������ �� ��������� ���������� �����
 lpNextItem                 dd  ?;������ �� ��������� ����������� �����
 vkCodeItemOne              dd  ?; = 0 - �������� �� ��������� ����, ���� �� ���� ����� = virtual-key code
 vkCodeItemTwoThree         dd  ?;���� �� ���� ����� = virtual-key code ���� ������ ������������� � ������� � ������� ���������,� vkCodeItemOne = virtual-key ��������� ������� � ����������
 scanCodeItemOne            dd  ?
 scanCodeItemTwoThree       dd  ?
 vkCodeSubOne               dd  ?;���������� ��� � ���� vkCodeItemOne � vkCodeItemTwoThree, �� ���� vkCodeSubOne > VK_LAST ����� � ��������� "Sub" ���� ���������,\
 vkCodeSubTwoThree          dd  ?;��������� ������ ����� ��������� �� �������� �  vkCodeSubOne
 scanCodeSubOne             dd  ?; ���� vkCodeSubOne = virtual-key code, ����� = ������� �������������� virtual-key code � ������� �����, � �������  ���� 1 ����� ���������� �������
 scanCodeSubTwoThree        dd  ?; ���� vkCodeSubOne = virtual-key code, ����� = ������� �������������� virtual-key code � ������� �����, � �������  ���� 1 ����� ���������� �������
ends

struct  HOOK_MINOR
 iKeyModif                  dd  ?;��� ���� ��� �� ��������� ������������ ��������� � ����������� ��������� - ��� �� �������� ������� ������������ ��� ����� �� ���������� ���� �����������
 iKeyModif2                 dd  ?;��� ���� ��� �� ��������� ������������ ��������� � ����������� ��������� - ��� �� �������� ������� ������������ ��� ����� �� ���������� ���� �����������
ends

struct  HOOK
union
 iCountKeyPressDD           dd  ?
 struct
  iCountKeyPress            db  ?;������������ ���������� ������ ������������ �� ���������� � ����
  iCountKeyPressModif       db  ?;������������ ���������� ������ ������������� ������������ �� ����������
 ends
ends

aCombi                      ITEMLISTVIEW;��� ���������� ������� ����������

 aKeyPressedMap             dd  constLengthKeyPressedMap  dup(?);����� ���������� ������� ������� �������������, ���� = 1 ������ ������������� ������ �� ������� vk,
 aKeyPressedMapColizion     dd  constLengthKeyPressedMap  dup(?);����� ���������� ������� ������� �������������, ���� = 1 ������ ������������� ������ �� ������� vk,
ends



struct  KEYMAP
 hwnd                       dd  ?;����� ���� Edit � �������� ������������� ����� ���������
 vkCodeOne                  dd  ?; = 0 - �������� �� ��������� ����, ���� �� ���� ����� = virtual-key code
 vkCodeTwoThree             dd  ?;���� �� ���� ����� = virtual-key code ���� ������ ������������� � ������� � ������� ���������,� vkCodeItemOne = virtual-key ��������� ������� � ����������
 scanCodeSubOne             dd  ?; ���� vkCodeSubOne = virtual-key code, ����� = ������� �������������� virtual-key code � ������� �����, � �������  ���� 1 ����� ���������� �������
 scanCodeSubTwoThree        dd  ?; ���� vkCodeSubOne = virtual-key code, ����� = ������� �������������� virtual-key code � ������� �����, � �������  ���� 1 ����� ���������� �������
ends


struct HWNDFONT;��������� ��� ���� ������� ����  ������� ��������� �����
 hwndButtonExit             dd  ?
 hwndButtonOptions          dd  ?
 hwndComboBoxLang           dd  ?
 hwndSysTab                 dd  ?
 hwndListView               dd  ?
 hwndStatic                 dd  ?
ends


struct NOTIFYICONDATAW_1;������ ������ ��������� ��� Windows  ������ XP -��� ��� ��� ����������� �� ����� �������������� � ��� �� �� ��������������....
 cbSize                            dd  ?
 hWnd                              LpType  ?
 uID                               dd  ?
 uFlags                            dd  ?
 uCallbackMessage                  dd  ?
 hIcon                             LpType  ?
 szTip                             du  64 dup(?)

ends


struct MODIFITER
 db  VK_SHIFT
 db  VK_CONTROL
 db  VK_MENU
 db  VK_LSHIFT
 db  VK_RSHIFT
 db  VK_LCONTROL
 db  VK_RCONTROL
 db  VK_LMENU
 db  VK_RMENU
ends


  struct  VectoredExceptionHandler
    MessageEng               du  'Version: ',equVersionApp,13,10
                             du  'Save the screenshot and send it with a description of the actions that led to the error on the:',equMailSupport,13,10,13,10

                             du  'Exception Address: 0x'
    ExceptionAddress         du  8 dup(?),13,10
                             du  'Exception Record: 0x'
    ExceptionRecord          du  8 dup(?),13,10
                             du  'Exception Flags: 0x'
    ExceptionFlags           du  8 dup(?),13,10
                             du  'Exception Code: 0x'
    ExceptionCode            du  8 dup(?),13,10
                             du  'Base of module: 0x'
    BaseOfModule             du  8 dup('0'),13,10
                             du  'Base of app: 0x'
    BaseOfApp                du  8 dup(?),13,10,13,10

                             du  'Eax=0x'
    MessageEax               du  8 dup(?),' Ecx=0x'
    MessageEcx               du  8 dup(?),' Edx=0x'
    MessageEdx               du  8 dup(?),13,10,'Ebx=0x'
    MessageEbx               du  8 dup(?),' Esi=0x'
    MessageEsi               du  8 dup(?),' Edi=0x'
    MessageEdi               du  8 dup(?),13,10,'Ebp=0x'
    MessageEbp               du  8 dup(?),' Esp=0x'
    MessageEsp               du  8 dup(?),13,10,13,10

  ends



struct !LARGE_INTEGER
   LowPart                      dd ?
   HighPart                     dd ?
ends


macro arraybVCPCode  name,[id,str]
{ common
   local max,loc
   max  = 0

  forward
   if  id > max
    max = id
   end if

  common
   align 2
   name:
   dw (max+1)*2 dup(?)

  forward
   loc = $
   du str
   store word loc - name at name + id*2*2
   store word $ - loc at name + id*2*2 + 2
   du 0}


macro arrayChangeTab  name,[id,label]
{ common
   local max
   count = 1
   align 4
   name:
  forward
   count = count + 1

  common
   repeat count
  forward
   if (id+1) = %
     dd  label
   end if

  common
  end repeat }


macro  arrayKey name,[id,str]
{ common
   local max,any,maxi
   maxi = 0
   max = 0
  forward
   if id > max
     max = id
   end if
  common
   align 4
   name dd szEmptyItem
   repeat max
  forward
    if id = %
     local loc
     if  name+ 4 + %*4 > $
      dd  (name+  %*4 - $)/4 dup any
     end if
     dd loc
    end if

  common
   end repeat
   repeat max
  forward
   if id = %
     loc du str,0
     if ($ - loc - 2)/2 > maxi
       maxi = ($ - loc - 2)/2
     end if
   end if
  common
   end repeat
   any  du 'Unknown',0
   siz = $ - any - 2
   name#lengthMax uniq maxi*3+3*2+1+8 };maxi*3 = ��� ������� ����� ����;3*2 = ' + ';+1 = �� 0;+8 - ��� ��� �����



;----------------------------------------------
macro langtableDU  name,[id,namelng,pathlng]
{ common
   local count
   count = 0
   align 4
   name:
  forward
   local any
   id uniq count
   dd any
   count = count + 1

  forward
   lp#id  du namelng,0
   any  du pathlng,0  }


;----------------------------------------------
macro  enumMenuJmp num,last,[id,arg]
{ common
   local count
   count = num
   align 2
   Jmp#num:

  forward
   id uniq count
   count  = count + 1
   dw  arg-Start
  common
   last uniq count }


;----------------------------------------------
macro  arrayDU  name,[lab,val]
{ common
  align 2
  name:

  forward
  if lab eq Anonymous
    duA val
   else
    lab duA val
  end if


  common
  sizeof.#name uniq $ - name}
