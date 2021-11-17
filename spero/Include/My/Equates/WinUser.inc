

;Multimonitor API.
;---------------------------
;A physical monitor description is always an array of 128 characters.  Some
;of the characters may not be used.
PHYSICAL_MONITOR_DESCRIPTION_SIZE  uniq  128

MONITOR_DEFAULTTONULL              uniq  0x00000000
MONITOR_DEFAULTTOPRIMARY           uniq  0x00000001
MONITOR_DEFAULTTONEAREST           uniq  0x00000002

;---------------------------
MAPVK_VK_TO_VSC           uniq  0
MAPVK_VSC_TO_VK           uniq  1
MAPVK_VK_TO_CHAR          uniq  2
MAPVK_VSC_TO_VK_EX        uniq  3
MAPVK_VK_TO_VSC_EX        uniq  4


;flags for DrawFrameControl */
;---------------------------
DFC_CAPTION                           uniq  1
DFC_MENU                              uniq  2
DFC_SCROLL                            uniq  3
DFC_BUTTON                            uniq  4
DFC_POPUPMENU                         uniq  5

DFCS_CAPTIONCLOSE                     uniq  0x0000
DFCS_CAPTIONMIN                       uniq  0x0001
DFCS_CAPTIONMAX                       uniq  0x0002
DFCS_CAPTIONRESTORE                   uniq  0x0003
DFCS_CAPTIONHELP                      uniq  0x0004

DFCS_MENUARROW                        uniq  0x0000
DFCS_MENUCHECK                        uniq  0x0001
DFCS_MENUBULLET                       uniq  0x0002
DFCS_MENUARROWRIGHT                   uniq  0x0004
DFCS_SCROLLUP                         uniq  0x0000
DFCS_SCROLLDOWN                       uniq  0x0001
DFCS_SCROLLLEFT                       uniq  0x0002
DFCS_SCROLLRIGHT                      uniq  0x0003
DFCS_SCROLLCOMBOBOX                   uniq  0x0005
DFCS_SCROLLSIZEGRIP                   uniq  0x0008
DFCS_SCROLLSIZEGRIPRIGHT              uniq  0x0010

DFCS_BUTTONCHECK                      uniq  0x0000
DFCS_BUTTONRADIOIMAGE                 uniq  0x0001
DFCS_BUTTONRADIOMASK                  uniq  0x0002
DFCS_BUTTONRADIO                      uniq  0x0004
DFCS_BUTTON3STATE                     uniq  0x0008
DFCS_BUTTONPUSH                       uniq  0x0010

DFCS_INACTIVE                         uniq  0x0100
DFCS_PUSHED                           uniq  0x0200
DFCS_CHECKED                          uniq  0x0400

DFCS_TRANSPARENT                      uniq  0x0800
DFCS_HOT                              uniq  0x1000

DFCS_ADJUSTRECT                       uniq  0x2000
DFCS_FLAT                             uniq  0x4000
DFCS_MONO                             uniq  0x8000

;Window field offsets for GetWindowLong()
;---------------------------
GWL_STYLE                    uniq  -16
GWL_EXSTYLE                  uniq  -20
GWL_ID                       uniq  -12
GWLP_WNDPROC                 uniq  -4
GWLP_HINSTANCE               uniq  -6
GWLP_HWNDPARENT              uniq  -8
GWLP_USERDATA                uniq  -21
GWLP_ID                      uniq  -12

;Class field offsets for GetClassLong
;---------------------------
GCL_CBWNDEXTRA               uniq  -18
GCL_CBCLSEXTRA               uniq  -20
GCL_STYLE                    uniq  -26
GCW_ATOM                     uniq  -32
GCLP_MENUNAME                uniq  -8
GCLP_HBRBACKGROUND           uniq  -10
GCLP_HCURSOR                 uniq  -12
GCLP_HICON                   uniq  -14
GCLP_HMODULE                 uniq  -16
GCLP_WNDPROC                 uniq  -24
GCLP_HICONSM                 uniq  -34

;Dialog Box Command IDs
;---------------------------
IDOK                        uniq  1
IDCANCEL                    uniq  2
IDABORT                     uniq  3
IDRETRY                     uniq  4
IDIGNORE                    uniq  5
IDYES                       uniq  6
IDNO                        uniq  7
IDCLOSE                     uniq  8
IDHELP                      uniq  9
IDTRYAGAIN                  uniq  10
IDCONTINUE                  uniq  11
IDTIMEOUT                   uniq  32000

;---------------------------
INPUT_MOUSE         uniq  0
INPUT_KEYBOARD      uniq  1
INPUT_HARDWARE      uniq  2

WHEEL_DELTA         uniq  120

KEYEVENTF_EXTENDEDKEY           uniq  0x0001
KEYEVENTF_KEYUP                 uniq  0x0002
KEYEVENTF_UNICODE               uniq  0x0004
KEYEVENTF_SCANCODE              uniq  0x0008

MOUSEEVENTF_MOVE                    uniq  0x0001; mouse move */
MOUSEEVENTF_LEFTDOWN                uniq  0x0002; left button down */
MOUSEEVENTF_LEFTUP                  uniq  0x0004; left button up */
MOUSEEVENTF_RIGHTDOWN               uniq  0x0008; right button down */
MOUSEEVENTF_RIGHTUP                 uniq  0x0010; right button up */
MOUSEEVENTF_MIDDLEDOWN              uniq  0x0020; middle button down */
MOUSEEVENTF_MIDDLEUP                uniq  0x0040; middle button up */
MOUSEEVENTF_XDOWN                   uniq  0x0080; x button down */
MOUSEEVENTF_XUP                     uniq  0x0100; x button down */
MOUSEEVENTF_WHEEL                            uniq  0x0800; wheel button rolled */
MOUSEEVENTF_HWHEEL                           uniq  0x01000; hwheel button rolled */
MOUSEEVENTF_MOVE_NOCOALESCE                  uniq  0x2000; do not coalesce mouse moves */
MOUSEEVENTF_VIRTUALDESK                      uniq  0x4000; map to entire virtual desktop */
MOUSEEVENTF_ABSOLUTE                         uniq  0x8000; absolute move */


;Queue status flags for GetQueueStatus() and MsgWaitForMultipleObjects()
;---------------------------
QS_KEY                             uniq  0x0001
QS_MOUSEMOVE                       uniq  0x0002
QS_MOUSEBUTTON                     uniq  0x0004
QS_POSTMESSAGE                     uniq  0x0008
QS_TIMER                           uniq  0x0010
QS_PAINT                           uniq  0x0020
QS_SENDMESSAGE                     uniq  0x0040
QS_HOTKEY                          uniq  0x0080
QS_ALLPOSTMESSAGE                  uniq  0x0100
QS_RAWINPUT                        uniq  0x0400
QS_TOUCH                           uniq  0x0800
QS_POINTER                         uniq  0x1000
QS_MOUSE                           uniq  QS_MOUSEMOVE or QS_MOUSEBUTTON
;QS_ALLEVENTS                       uniq  QS_INPUT or QS_POSTMESSAGE or QS_TIMER or QS_PAINT or QS_HOTKEY
;QS_ALLINPUT                        uniq  QS_INPUT or QS_POSTMESSAGE or QS_TIMER or QS_PAINT or QS_HOTKEY or QS_SENDMESSAGE

;PeekMessage() Options
;---------------------------
PM_NOREMOVE              uniq  0x0000
PM_REMOVE                uniq  0x0001
PM_NOYIELD               uniq  0x0002
PM_QS_POSTMESSAGE        uniq  (QS_POSTMESSAGE or QS_HOTKEY or QS_TIMER) shl 16
PM_QS_PAINT              uniq  QS_PAINT shl 16
PM_QS_SENDMESSAGE        uniq  QS_SENDMESSAGE shl 16



;---------------------------
MIIM_STATE                 uniq  0x00000001
MIIM_ID                    uniq  0x00000002
MIIM_SUBMENU               uniq  0x00000004
MIIM_CHECKMARKS            uniq  0x00000008
MIIM_TYPE                  uniq  0x00000010
MIIM_DATA                  uniq  0x00000020
MIIM_STRING                uniq  0x00000040
MIIM_BITMAP                uniq  0x00000080
MIIM_FTYPE                 uniq  0x00000100

HBMMENU_CALLBACK           uniq -1
HBMMENU_SYSTEM             uniq  1
HBMMENU_MBAR_RESTORE       uniq  2
HBMMENU_MBAR_MINIMIZE      uniq  3
HBMMENU_MBAR_CLOSE         uniq  5
HBMMENU_MBAR_CLOSE_D       uniq  6
HBMMENU_MBAR_MINIMIZE_D    uniq  7
HBMMENU_POPUP_CLOSE        uniq  8
HBMMENU_POPUP_RESTORE      uniq  9
HBMMENU_POPUP_MAXIMIZE     uniq  10
HBMMENU_POPUP_MINIMIZE     uniq  11


;Flags for TrackPopupMenu
;----------------------------
TPM_LEFTBUTTON                uniq  0x0000
TPM_RIGHTBUTTON               uniq  0x0002
TPM_LEFTALIGN                 uniq  0x0000
TPM_CENTERALIGN               uniq  0x0004
TPM_RIGHTALIGN                uniq  0x0008
TPM_TOPALIGN                  uniq  0x0000
TPM_VCENTERALIGN              uniq  0x0010
TPM_BOTTOMALIGN               uniq  0x0020
TPM_HORIZONTAL                uniq  0x0000;Horz alignment matters more */
TPM_VERTICAL                  uniq  0x0040;Vert alignment matters more */
TPM_NONOTIFY                  uniq  0x0080;Don't send any notification msgs */
TPM_RETURNCMD                 uniq  0x0100
TPM_RECURSE                   uniq  0x0001
TPM_HORPOSANIMATION           uniq  0x0400
TPM_HORNEGANIMATION           uniq  0x0800
TPM_VERPOSANIMATION           uniq  0x1000
TPM_VERNEGANIMATION           uniq  0x2000
TPM_NOANIMATION               uniq  0x4000
TPM_LAYOUTRTL                 uniq  0x8000
TPM_WORKAREA                  uniq  0x10000


;Virtual Keys, Standard Set
;----------------------------
VK_LBUTTON                    uniq  0x01
VK_RBUTTON                    uniq  0x02
VK_CANCEL                     uniq  0x03
VK_MBUTTON                    uniq  0x04 ; NOT contiguous with L & RBUTTON */
VK_XBUTTON1                   uniq  0x05 ; NOT contiguous with L & RBUTTON */
VK_XBUTTON2                   uniq  0x06 ; NOT contiguous with L & RBUTTON */
;0x07 : reserved
VK_BACK                       uniq  0x08
VK_TAB                        uniq  0x09
;0x0A - 0x0B : reserved
VK_CLEAR                      uniq  0x0C
VK_RETURN                     uniq  0x0D
;0x0E - 0x0F : unassigned
VK_SHIFT                      uniq  0x10
VK_CONTROL                    uniq  0x11
VK_MENU                       uniq  0x12
VK_PAUSE                      uniq  0x13
VK_CAPITAL                    uniq  0x14

VK_KANA                       uniq  0x15
VK_HANGEUL                    uniq  0x15;old name - should be here for compatibility */
VK_HANGUL                     uniq  0x15
;0x16 : unassigned
VK_JUNJA                      uniq  0x17
VK_FINAL                      uniq  0x18
VK_HANJA                      uniq  0x19
VK_KANJI                      uniq  0x19
;0x1A : unassigned
VK_ESCAPE                     uniq  0x1B
VK_CONVERT                    uniq  0x1C
VK_NONCONVERT                 uniq  0x1D
VK_ACCEPT                     uniq  0x1E
VK_MODECHANGE                 uniq  0x1F
VK_SPACE                      uniq  0x20
VK_PRIOR                      uniq  0x21
VK_NEXT                       uniq  0x22
VK_END                        uniq  0x23
VK_HOME                       uniq  0x24
VK_LEFT                       uniq  0x25
VK_UP                         uniq  0x26
VK_RIGHT                      uniq  0x27
VK_DOWN                       uniq  0x28
VK_SELECT                     uniq  0x29
VK_PRINT                      uniq  0x2A
VK_EXECUTE                    uniq  0x2B
VK_SNAPSHOT                   uniq  0x2C
VK_INSERT                     uniq  0x2D
VK_DELETE                     uniq  0x2E
VK_HELP                       uniq  0x2F

;VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
;0x3A - 0x40 : unassigned
;VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
VK_LWIN                       uniq  0x5B
VK_RWIN                       uniq  0x5C
VK_APPS                       uniq  0x5D
;0x5E : reserved
VK_SLEEP                      uniq  0x5F
VK_NUMPAD0                    uniq  0x60
VK_NUMPAD1                    uniq  0x61
VK_NUMPAD2                    uniq  0x62
VK_NUMPAD3                    uniq  0x63
VK_NUMPAD4                    uniq  0x64
VK_NUMPAD5                    uniq  0x65
VK_NUMPAD6                    uniq  0x66
VK_NUMPAD7                    uniq  0x67
VK_NUMPAD8                    uniq  0x68
VK_NUMPAD9                    uniq  0x69
VK_MULTIPLY                   uniq  0x6A
VK_ADD                        uniq  0x6B
VK_SEPARATOR                  uniq  0x6C
VK_SUBTRACT                   uniq  0x6D
VK_DECIMAL                    uniq  0x6E
VK_DIVIDE                     uniq  0x6F
VK_F1                         uniq  0x70
VK_F2                         uniq  0x71
VK_F3                         uniq  0x72
VK_F4                         uniq  0x73
VK_F5                         uniq  0x74
VK_F6                         uniq  0x75
VK_F7                         uniq  0x76
VK_F8                         uniq  0x77
VK_F9                         uniq  0x78
VK_F10                        uniq  0x79
VK_F11                        uniq  0x7A
VK_F12                        uniq  0x7B
VK_F13                        uniq  0x7C
VK_F14                        uniq  0x7D
VK_F15                        uniq  0x7E
VK_F16                        uniq  0x7F
VK_F17                        uniq  0x80
VK_F18                        uniq  0x81
VK_F19                        uniq  0x82
VK_F20                        uniq  0x83
VK_F21                        uniq  0x84
VK_F22                        uniq  0x85
VK_F23                        uniq  0x86
VK_F24                        uniq  0x87

;0x88 - 0x8F : UI navigation

VK_NAVIGATION_VIEW                 uniq  0x88 ;reserved
VK_NAVIGATION_MENU                 uniq  0x89 ;reserved
VK_NAVIGATION_UP                   uniq  0x8A ; reserved
VK_NAVIGATION_DOWN                 uniq  0x8B; reserved
VK_NAVIGATION_LEFT                 uniq  0x8C ; reserved
VK_NAVIGATION_RIGHT                uniq  0x8D ; reserved
VK_NAVIGATION_ACCEPT               uniq  0x8E ; reserved
VK_NAVIGATION_CANCEL               uniq  0x8F ; reserved

VK_NUMLOCK                    uniq  0x90
VK_SCROLL                     uniq  0x91

;NEC PC-9800 kbd definitions
VK_OEM_NEC_EQUAL              uniq  0x92 ; '=' key on numpad

;Fujitsu/OASYS kbd definitions
VK_OEM_FJ_JISHO               uniq  0x92 ;'Dictionary' key
VK_OEM_FJ_MASSHOU             uniq  0x93  ; 'Unregister word' key
VK_OEM_FJ_TOUROKU             uniq  0x94  ; 'Register word' key
VK_OEM_FJ_LOYA                uniq  0x95 ; 'Left OYAYUBI' key
VK_OEM_FJ_ROYA                uniq  0x96 ; 'Right OYAYUBI' key


;0x97 - 0x9F : unassigned

; VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
; Used only as parameters to GetAsyncKeyState() and GetKeyState().
; No other API or message will distinguish left and right keys in this way.
VK_LSHIFT                     uniq  0xA0
VK_RSHIFT                     uniq  0xA1
VK_LCONTROL                   uniq  0xA2
VK_RCONTROL                   uniq  0xA3
VK_LMENU                      uniq  0xA4
VK_RMENU                      uniq  0xA5
VK_BROWSER_BACK                    uniq  0xA6
VK_BROWSER_FORWARD                 uniq  0xA7
VK_BROWSER_REFRESH                 uniq  0xA8
VK_BROWSER_STOP                    uniq  0xA9
VK_BROWSER_SEARCH                  uniq  0xAA
VK_BROWSER_FAVORITES               uniq  0xAB
VK_BROWSER_HOME                    uniq  0xAC
VK_VOLUME_MUTE                     uniq  0xAD
VK_VOLUME_DOWN                     uniq  0xAE
VK_VOLUME_UP                       uniq  0xAF
VK_MEDIA_NEXT_TRACK                uniq  0xB0
VK_MEDIA_PREV_TRACK                uniq  0xB1
VK_MEDIA_STOP                      uniq  0xB2
VK_MEDIA_PLAY_PAUSE                uniq  0xB3
VK_LAUNCH_MAIL                     uniq  0xB4
VK_LAUNCH_MEDIA_SELECT             uniq  0xB5
VK_LAUNCH_APP1                     uniq  0xB6
VK_LAUNCH_APP2                     uniq  0xB7
;0xB8 - 0xB9 : reserved
VK_OEM_1                      uniq  0xBA ;';:' for US
VK_OEM_PLUS                   uniq  0xBB; '+' any country
VK_OEM_COMMA                  uniq  0xBC; ',' any country
VK_OEM_MINUS                  uniq  0xBD ; '-' any country
VK_OEM_PERIOD                 uniq  0xBE ; '.' any country
VK_OEM_2                      uniq  0xBF ; '/?' for US
VK_OEM_3                      uniq  0xC0 ; '`~' for US
;0xC1 - 0xC2 : reserved
;0xC3 - 0xDA : Gamepad input
VK_GAMEPAD_A                                     uniq  0xC3;reserved
VK_GAMEPAD_B                                     uniq  0xC4;reserved
VK_GAMEPAD_X                                     uniq  0xC5;reserved
VK_GAMEPAD_Y                                     uniq  0xC6;reserved
VK_GAMEPAD_RIGHT_SHOULDER                        uniq  0xC7;reserved
VK_GAMEPAD_LEFT_SHOULDER                         uniq  0xC8;reserved
VK_GAMEPAD_LEFT_TRIGGER                          uniq  0xC9;reserved
VK_GAMEPAD_RIGHT_TRIGGER                         uniq  0xCA;reserved
VK_GAMEPAD_DPAD_UP                               uniq  0xCB;reserved
VK_GAMEPAD_DPAD_DOWN                             uniq  0xCC;reserved
VK_GAMEPAD_DPAD_LEFT                             uniq  0xCD;reserved
VK_GAMEPAD_DPAD_RIGHT                            uniq  0xCE;reserved
VK_GAMEPAD_MENU                                  uniq  0xCF;reserved
VK_GAMEPAD_VIEW                                  uniq  0xD0;reserved
VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON                uniq  0xD1;reserved
VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON               uniq  0xD2;reserved
VK_GAMEPAD_LEFT_THUMBSTICK_UP                    uniq  0xD3;reserved
VK_GAMEPAD_LEFT_THUMBSTICK_DOWN                  uniq  0xD4;reserved
VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT                 uniq  0xD5;reserved
VK_GAMEPAD_LEFT_THUMBSTICK_LEFT                  uniq  0xD6;reserved
VK_GAMEPAD_RIGHT_THUMBSTICK_UP                   uniq  0xD7;reserved
VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN                 uniq  0xD8;reserved
VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT                uniq  0xD9;reserved
VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT                 uniq  0xDA;reserved

VK_OEM_4                      uniq  0xDB ; '[{' for US
VK_OEM_5                      uniq  0xDC ; '\|' for US
VK_OEM_6                      uniq  0xDD ; ']}' for US
VK_OEM_7                      uniq  0xDE ; ''"' for US
VK_OEM_8                      uniq  0xDF
;0xE0 : reserved
;Various extended or enhanced keyboards
VK_OEM_AX                     uniq  0xE1 ; 'AX' key on Japanese AX kbd
VK_OEM_102                    uniq  0xE2 ; "<>" or "\|" on RT 102-key kbd.
VK_ICO_HELP                   uniq  0xE3 ; Help key on ICO
VK_ICO_00                     uniq  0xE4 ; 00 key on ICO
VK_PROCESS                    uniq  0xE5
VK_ICO_CLEAR                  uniq  0xE6
VK_PACKET                     uniq  0xE7
;0xE8 : unassigned
;Nokia/Ericsson definitions
VK_OEM_RESET                  uniq  0xE9
VK_OEM_JUMP                   uniq  0xEA
VK_OEM_PA1                    uniq  0xEB
VK_OEM_PA2                    uniq  0xEC
VK_OEM_PA3                    uniq  0xED
VK_OEM_WSCTRL                 uniq  0xEE
VK_OEM_CUSEL                  uniq  0xEF
VK_OEM_ATTN                   uniq  0xF0
VK_OEM_FINISH                 uniq  0xF1
VK_OEM_COPY                   uniq  0xF2
VK_OEM_AUTO                   uniq  0xF3
VK_OEM_ENLW                   uniq  0xF4
VK_OEM_BACKTAB                uniq  0xF5

VK_ATTN                       uniq  0xF6
VK_CRSEL                      uniq  0xF7
VK_EXSEL                      uniq  0xF8
VK_EREOF                      uniq  0xF9
VK_PLAY                       uniq  0xFA
VK_ZOOM                       uniq  0xFB
VK_NONAME                     uniq  0xFC
VK_PA1                        uniq  0xFD
VK_OEM_CLEAR                  uniq  0xFE



HWND_MESSAGE        uniq   -3

;Standard Icon IDs
;----------------------------
IDI_APPLICATION     uniq  32512
IDI_HAND            uniq  32513
IDI_QUESTION        uniq  32514
IDI_EXCLAMATION     uniq  32515
IDI_ASTERISK        uniq  32516
IDI_WINLOGO         uniq  32517
IDI_SHIELD          uniq  32518
IDI_WARNING         uniq  IDI_EXCLAMATION
IDI_ERROR           uniq  IDI_HAND
IDI_INFORMATION     uniq  IDI_ASTERISK


;Edit Control Styles
;----------------------------
ES_LEFT                          uniq  0x0000
ES_CENTER                        uniq  0x0001
ES_RIGHT                         uniq  0x0002
ES_MULTILINE                     uniq  0x0004
ES_UPPERCASE                     uniq  0x0008
ES_LOWERCASE                     uniq  0x0010
ES_PASSWORD                      uniq  0x0020
ES_AUTOVSCROLL                   uniq  0x0040
ES_AUTOHSCROLL                   uniq  0x0080
ES_NOHIDESEL                     uniq  0x0100
ES_OEMCONVERT                    uniq  0x0400
ES_READONLY                      uniq  0x0800
ES_WANTRETURN                    uniq  0x1000
ES_NUMBER                        uniq  0x2000

;Edit Control Notification Codes
EN_SETFOCUS                      uniq  0x0100
EN_KILLFOCUS                     uniq  0x0200
EN_CHANGE                        uniq  0x0300
EN_UPDATE                        uniq  0x0400
EN_ERRSPACE                      uniq  0x0500
EN_MAXTEXT                       uniq  0x0501
EN_HSCROLL                       uniq  0x0601
EN_VSCROLL                       uniq  0x0602
EN_ALIGN_LTR_EC                  uniq  0x0700
EN_ALIGN_RTL_EC                  uniq  0x0701
EN_BEFORE_PASTE                  uniq  0x0800
EN_AFTER_PASTE                   uniq  0x0801

;Edit control EM_SETMARGIN parameters */
EC_LEFTMARGIN                    uniq  0x0001
EC_RIGHTMARGIN                   uniq  0x0002
EC_USEFONTINFO                   uniq  0xffff
;wParam of EM_GET/SETIMESTATUS  */
EMSIS_COMPOSITIONSTRING                     uniq  0x0001

;lParam for EMSIS_COMPOSITIONSTRING  */
EIMES_GETCOMPSTRATONCE                      uniq  0x0001
EIMES_CANCELCOMPSTRINFOCUS                  uniq  0x0002
EIMES_COMPLETECOMPSTRKILLFOCUS              uniq  0x0004

;Edit Control Messages
EM_GETSEL                            uniq  0x00B0
EM_SETSEL                            uniq  0x00B1
EM_GETRECT                           uniq  0x00B2
EM_SETRECT                           uniq  0x00B3
EM_SETRECTNP                         uniq  0x00B4
EM_SCROLL                            uniq  0x00B5
EM_LINESCROLL                        uniq  0x00B6
EM_SCROLLCARET                       uniq  0x00B7
EM_GETMODIFY                         uniq  0x00B8
EM_SETMODIFY                         uniq  0x00B9
EM_GETLINECOUNT                      uniq  0x00BA
EM_LINEINDEX                         uniq  0x00BB
EM_SETHANDLE                         uniq  0x00BC
EM_GETHANDLE                         uniq  0x00BD
EM_GETTHUMB                          uniq  0x00BE
EM_LINELENGTH                        uniq  0x00C1
EM_REPLACESEL                        uniq  0x00C2
EM_GETLINE                           uniq  0x00C4
EM_LIMITTEXT                         uniq  0x00C5
EM_CANUNDO                           uniq  0x00C6
EM_UNDO                              uniq  0x00C7
EM_FMTLINES                          uniq  0x00C8
EM_LINEFROMCHAR                      uniq  0x00C9
EM_SETTABSTOPS                       uniq  0x00CB
EM_SETPASSWORDCHAR                   uniq  0x00CC
EM_EMPTYUNDOBUFFER                   uniq  0x00CD
EM_GETFIRSTVISIBLELINE               uniq  0x00CE
EM_SETREADONLY                       uniq  0x00CF
EM_SETWORDBREAKPROC                  uniq  0x00D0
EM_GETWORDBREAKPROC                  uniq  0x00D1
EM_GETPASSWORDCHAR                   uniq  0x00D2
EM_SETMARGINS                        uniq  0x00D3
EM_GETMARGINS                        uniq  0x00D4
EM_SETLIMITTEXT         uniq EM_LIMITTEXT; ;win40 Name change */
EM_GETLIMITTEXT                      uniq  0x00D5
EM_POSFROMCHAR                       uniq  0x00D6
EM_CHARFROMPOS                       uniq  0x00D7
EM_SETIMESTATUS                      uniq  0x00D8
EM_GETIMESTATUS                      uniq  0x00D9
EM_ENABLEFEATURE                     uniq  0x00DA

;EM_ENABLEFEATURE options
EDIT_CONTROL_FEATURE_ENTERPRISE_DATA_PROTECTION_PASTE_SUPPORT  uniq  0
EDIT_CONTROL_FEATURE_PASTE_NOTIFICATIONS                       uniq  1

;EDITWORDBREAKPROC code values
WB_LEFT            uniq  0
WB_RIGHT           uniq  1
WB_ISDELIMITER     uniq  2


;SetWindowsHook() codes
;----------------------------
WH_MSGFILTER                  uniq  -1
WH_JOURNALRECORD              uniq  0
WH_JOURNALPLAYBACK            uniq  1
WH_KEYBOARD                   uniq  2
WH_GETMESSAGE                 uniq  3
WH_CALLWNDPROC                uniq  4
WH_CBT                        uniq  5
WH_SYSMSGFILTER               uniq  6
WH_MOUSE                      uniq  7
WH_HARDWARE                   uniq  8
WH_DEBUG                      uniq  9
WH_SHELL                      uniq  10
WH_FOREGROUNDIDLE             uniq  11
WH_CALLWNDPROCRET             uniq  12
WH_KEYBOARD_LL                uniq  13
WH_MOUSE_LL                   uniq  14

;Hook Codes
HC_ACTION                     uniq  0
HC_GETNEXT                    uniq  1
HC_SKIP                       uniq  2
HC_NOREMOVE                   uniq  3
HC_NOREM                      uniq  HC_NOREMOVE
HC_SYSMODALON                 uniq  4
HC_SYSMODALOFF                uniq  5


FALSE                           uniq  0
NULL                            uniq  0
TRUE                            uniq  1


;Dialog Styles
;----------------------------
DS_ABSALIGN                     uniq  0x01
DS_SYSMODAL                     uniq  0x02
DS_LOCALEDIT                    uniq  0x20 ;Edit items get Local storage. */
DS_SETFONT                      uniq  0x40 ;User specified font for Dlg controls */
DS_MODALFRAME                   uniq  0x80 ; Can be combined with WS_CAPTION  */
DS_NOIDLEMSG                    uniq  0x100 ; WM_ENTERIDLE message will not be sent */
DS_SETFOREGROUND                uniq  0x200  ;not in win3.1 */
DS_3DLOOK                       uniq  0x0004
DS_FIXEDSYS                     uniq  0x0008
DS_NOFAILCREATE                 uniq  0x0010
DS_CONTROL                      uniq  0x0400
DS_CENTER                       uniq  0x0800
DS_CENTERMOUSE                  uniq  0x1000
DS_CONTEXTHELP                  uniq  0x2000
DS_SHELLFONT                    uniq  DS_SETFONT or DS_FIXEDSYS

DM_GETDEFID                     uniq  WM_USER+0
DM_SETDEFID                     uniq  WM_USER+1
DM_REPOSITION                   uniq  WM_USER+2

;Returned in HIWORD() of DM_GETDEFID result if msg is supported
DC_HASDEFID                     uniq  0x534B

; Dialog Codes
;----------------------------
DLGC_WANTARROWS                 uniq  0x0001   ;Control wants arrow keys         */
DLGC_WANTTAB                    uniq  0x0002   ;Control wants tab keys           */
DLGC_WANTALLKEYS                uniq  0x0004   ;Control wants all keys           */
DLGC_WANTMESSAGE                uniq  0x0004   ;Pass message to control          */
DLGC_HASSETSEL                  uniq  0x0008   ;Understands EM_SETSEL message    */
DLGC_DEFPUSHBUTTON              uniq  0x0010   ;Default pushbutton               */
DLGC_UNDEFPUSHBUTTON            uniq  0x0020  ;Non-default pushbutton           */
DLGC_RADIOBUTTON                uniq  0x0040   ;Radio button                     */
DLGC_WANTCHARS                  uniq  0x0080   ;Want WM_CHAR messages            */
DLGC_STATIC                     uniq  0x0100   ;Static item: don't include       */
DLGC_BUTTON                     uniq  0x2000   ;Button item: can be checked      */


;Static Control Constants
;----------------------------
SS_LEFT                   uniq  0x00000000
SS_CENTER                 uniq  0x00000001
SS_RIGHT                  uniq  0x00000002
SS_ICON                   uniq  0x00000003
SS_BLACKRECT              uniq  0x00000004
SS_GRAYRECT               uniq  0x00000005
SS_WHITERECT              uniq  0x00000006
SS_BLACKFRAME             uniq  0x00000007
SS_GRAYFRAME              uniq  0x00000008
SS_WHITEFRAME             uniq  0x00000009
SS_USERITEM               uniq  0x0000000A
SS_SIMPLE                 uniq  0x0000000B
SS_LEFTNOWORDWRAP         uniq  0x0000000C
SS_OWNERDRAW              uniq  0x0000000D
SS_BITMAP                 uniq  0x0000000E
SS_ENHMETAFILE            uniq  0x0000000F
SS_ETCHEDHORZ             uniq  0x00000010
SS_ETCHEDVERT             uniq  0x00000011
SS_ETCHEDFRAME            uniq  0x00000012
SS_TYPEMASK               uniq  0x0000001F
SS_REALSIZECONTROL        uniq  0x00000040
SS_NOPREFIX               uniq  0x00000080; Don't do "&" character translation */
SS_NOTIFY                 uniq  0x00000100
SS_CENTERIMAGE            uniq  0x00000200
SS_RIGHTJUST              uniq  0x00000400
SS_REALSIZEIMAGE          uniq  0x00000800
SS_SUNKEN                 uniq  0x00001000
SS_EDITCONTROL            uniq  0x00002000
SS_ENDELLIPSIS            uniq  0x00004000
SS_PATHELLIPSIS           uniq  0x00008000
SS_WORDELLIPSIS           uniq  0x0000C000
SS_ELLIPSISMASK           uniq  0x0000C000

;Static Control Mesages
STM_SETICON               uniq  0x0170
STM_GETICON               uniq  0x0171
STM_SETIMAGE              uniq  0x0172
STM_GETIMAGE              uniq  0x0173
STN_CLICKED               uniq  0
STN_DBLCLK                uniq  1
STN_ENABLE                uniq  2
STN_DISABLE               uniq  3
STM_MSGMAX                uniq  0x0174



;System Menu Command Values
;----------------------------
SC_SIZE                    uniq  0xF000
SC_MOVE                    uniq  0xF010
SC_MINIMIZE                uniq  0xF020
SC_MAXIMIZE                uniq  0xF030
SC_NEXTWINDOW              uniq  0xF040
SC_PREVWINDOW              uniq  0xF050
SC_CLOSE                   uniq  0xF060
SC_VSCROLL                 uniq  0xF070
SC_HSCROLL                 uniq  0xF080
SC_MOUSEMENU               uniq  0xF090
SC_KEYMENU                 uniq  0xF100
SC_ARRANGE                 uniq  0xF110
SC_RESTORE                 uniq  0xF120
SC_TASKLIST                uniq  0xF130
SC_SCREENSAVE              uniq  0xF140
SC_HOTKEY                  uniq  0xF150
SC_DEFAULT                 uniq  0xF160
SC_MONITORPOWER            uniq  0xF170
SC_CONTEXTHELP             uniq  0xF180
SC_SEPARATOR               uniq  0xF00F

;GetWindow() Constants
;----------------------------
GW_HWNDFIRST         uniq  0
GW_HWNDLAST          uniq  1
GW_HWNDNEXT          uniq  2
GW_HWNDPREV          uniq  3
GW_OWNER             uniq  4
GW_CHILD             uniq  5
GW_ENABLEDPOPUP      uniq  6
GW_MAX               uniq  6


;;win40  -- A lot of MF_* flags have been renamed as MFT_* and MFS_* flags */
; Menu flags for Add/Check/EnableMenuItem()
;----------------------------
MF_INSERT                  uniq  0x00000000
MF_CHANGE                  uniq  0x00000080
MF_APPEND                  uniq  0x00000100
MF_DELETE                  uniq  0x00000200
MF_REMOVE                  uniq  0x00001000

MF_BYCOMMAND               uniq  0x00000000
MF_BYPOSITION              uniq  0x00000400

MF_SEPARATOR               uniq  0x00000800

MF_ENABLED                 uniq  0x00000000
MF_GRAYED                  uniq  0x00000001
MF_DISABLED                uniq  0x00000002

MF_UNCHECKED               uniq  0x00000000
MF_CHECKED                 uniq  0x00000008
MF_USECHECKBITMAPS         uniq  0x00000200

MF_STRING                  uniq  0x00000000
MF_BITMAP                  uniq  0x00000004
MF_OWNERDRAW               uniq  0x00000100

MF_POPUP                   uniq  0x00000010
MF_MENUBARBREAK            uniq  0x00000020
MF_MENUBREAK               uniq  0x00000040

MF_UNHILITE                uniq  0x00000000
MF_HILITE                  uniq  0x00000080

MF_DEFAULT                 uniq  0x00001000
MF_SYSMENU                 uniq  0x00002000
MF_HELP                    uniq  0x00004000
MF_RIGHTJUSTIFY            uniq  0x00004000

MF_MOUSESELECT             uniq  0x00008000
MF_END                     uniq  0x00000080 ;Obsolete -- only used by old RES files */
MFR_POPUP                  uniq  0x0001
MFR_END                    uniq  MF_END


MFT_STRING                 uniq  MF_STRING
MFT_BITMAP                 uniq  MF_BITMAP
MFT_MENUBARBREAK           uniq  MF_MENUBARBREAK
MFT_MENUBREAK              uniq  MF_MENUBREAK
MFT_OWNERDRAW              uniq  MF_OWNERDRAW
MFT_RADIOCHECK             uniq  0x00000200
MFT_SEPARATOR              uniq  MF_SEPARATOR
MFT_RIGHTORDER             uniq  0x00002000
MFT_RIGHTJUSTIFY           uniq  MF_RIGHTJUSTIFY

; Menu flags for Add/Check/EnableMenuItem() */
MFS_GRAYED                 uniq  0x00000003
MFS_DISABLED               uniq  MFS_GRAYED
MFS_CHECKED                uniq  MF_CHECKED
MFS_HILITE                 uniq  MF_HILITE
MFS_ENABLED                uniq  MF_ENABLED
MFS_UNCHECKED              uniq  MF_UNCHECKED
MFS_UNHILITE               uniq  MF_UNHILITE
MFS_DEFAULT                uniq  MF_DEFAULT



;Combo Box return Values
;----------------------------
CB_OKAY             uniq   0
CB_ERR              uniq  -1
CB_ERRSPACE         uniq  -2

;Combo Box Notification Codes
;----------------------------
CBN_ERRSPACE        uniq  -1
CBN_SELCHANGE       uniq   1
CBN_DBLCLK          uniq   2
CBN_SETFOCUS        uniq   3
CBN_KILLFOCUS       uniq   4
CBN_EDITCHANGE      uniq   5
CBN_EDITUPDATE      uniq   6
CBN_DROPDOWN        uniq   7
CBN_CLOSEUP         uniq   8
CBN_SELENDOK        uniq   9
CBN_SELENDCANCEL    uniq  10

;Combo Box styles
;----------------------------
CBS_SIMPLE                  uniq  0x0001
CBS_DROPDOWN                uniq  0x0002
CBS_DROPDOWNLIST            uniq  0x0003
CBS_OWNERDRAWFIXED          uniq  0x0010
CBS_OWNERDRAWVARIABLE       uniq  0x0020
CBS_AUTOHSCROLL             uniq  0x0040
CBS_OEMCONVERT              uniq  0x0080
CBS_SORT                    uniq  0x0100
CBS_HASSTRINGS              uniq  0x0200
CBS_NOINTEGRALHEIGHT        uniq  0x0400
CBS_DISABLENOSCROLL         uniq  0x0800
CBS_UPPERCASE               uniq  0x2000
CBS_LOWERCASE               uniq  0x4000

;Combo Box messages
;----------------------------
CB_GETEDITSEL                     uniq  0x0140
CB_LIMITTEXT                      uniq  0x0141
CB_SETEDITSEL                     uniq  0x0142
CB_ADDSTRING                      uniq  0x0143
CB_DELETESTRING                   uniq  0x0144
CB_DIR                            uniq  0x0145
CB_GETCOUNT                       uniq  0x0146
CB_GETCURSEL                      uniq  0x0147
CB_GETLBTEXT                      uniq  0x0148
CB_GETLBTEXTLEN                   uniq  0x0149
CB_INSERTSTRING                   uniq  0x014A
CB_RESETCONTENT                   uniq  0x014B
CB_FINDSTRING                     uniq  0x014C
CB_SELECTSTRING                   uniq  0x014D
CB_SETCURSEL                      uniq  0x014E
CB_SHOWDROPDOWN                   uniq  0x014F
CB_GETITEMDATA                    uniq  0x0150
CB_SETITEMDATA                    uniq  0x0151
CB_GETDROPPEDCONTROLRECT          uniq  0x0152
CB_SETITEMHEIGHT                  uniq  0x0153
CB_GETITEMHEIGHT                  uniq  0x0154
CB_SETEXTENDEDUI                  uniq  0x0155
CB_GETEXTENDEDUI                  uniq  0x0156
CB_GETDROPPEDSTATE                uniq  0x0157
CB_FINDSTRINGEXACT                uniq  0x0158
CB_SETLOCALE                      uniq  0x0159
CB_GETLOCALE                      uniq  0x015A
CB_GETTOPINDEX                    uniq  0x015b
CB_SETTOPINDEX                    uniq  0x015c
CB_GETHORIZONTALEXTENT            uniq  0x015d
CB_SETHORIZONTALEXTENT            uniq  0x015e
CB_GETDROPPEDWIDTH                uniq  0x015f
CB_SETDROPPEDWIDTH                uniq  0x0160
CB_INITSTORAGE                    uniq  0x0161
CB_MULTIPLEADDSTRING              uniq  0x0163
CB_GETCOMBOBOXINFO                uniq  0x0164

;SetWindowPos Flags
;----------------------------
SWP_NOSIZE              uniq  0x0001
SWP_NOMOVE              uniq  0x0002
SWP_NOZORDER            uniq  0x0004
SWP_NOREDRAW            uniq  0x0008
SWP_NOACTIVATE          uniq  0x0010
SWP_FRAMECHANGED        uniq  0x0020; The frame changed: send WM_NCCALCSIZE */
SWP_SHOWWINDOW          uniq  0x0040
SWP_HIDEWINDOW          uniq  0x0080
SWP_NOCOPYBITS          uniq  0x0100
SWP_NOOWNERZORDER       uniq  0x0200;Don't do owner Z ordering */
SWP_NOSENDCHANGING      uniq  0x0400;Don't send WM_WINDOWPOSCHANGING */
SWP_DRAWFRAME           uniq  SWP_FRAMECHANGED
SWP_NOREPOSITION        uniq  SWP_NOOWNERZORDER
SWP_DEFERERASE          uniq  0x2000
SWP_ASYNCWINDOWPOS      uniq  0x4000


;Standard Cursor IDs
;----------------------------
IDC_ARROW               uniq  32512
IDC_IBEAM               uniq  32513
IDC_WAIT                uniq  32514
IDC_CROSS               uniq  32515
IDC_UPARROW             uniq  32516
IDC_SIZE                uniq  32640;OBSOLETE: use IDC_SIZEALL */
IDC_ICON                uniq  32641;OBSOLETE: use IDC_ARROW */
IDC_SIZENWSE            uniq  32642
IDC_SIZENESW            uniq  32643
IDC_SIZEWE              uniq  32644
IDC_SIZENS              uniq  32645
IDC_SIZEALL             uniq  32646
IDC_NO                  uniq  32648;not in win3.1 */
IDC_HAND                uniq  32649
IDC_APPSTARTING         uniq  32650;not in win3.1 */
IDC_HELP                uniq  32651


;3D border styles */
;----------------------------
BDR_RAISEDOUTER     uniq  0x0001
BDR_SUNKENOUTER     uniq  0x0002
BDR_RAISEDINNER     uniq  0x0004
BDR_SUNKENINNER     uniq  0x0008
BDR_OUTER           uniq  BDR_RAISEDOUTER or BDR_SUNKENOUTER
BDR_INNER           uniq  BDR_RAISEDINNER or BDR_SUNKENINNER
BDR_RAISED          uniq  BDR_RAISEDOUTER or BDR_RAISEDINNER
BDR_SUNKEN          uniq  BDR_SUNKENOUTER or BDR_SUNKENINNER
EDGE_RAISED         uniq  BDR_RAISEDOUTER or BDR_RAISEDINNER
EDGE_SUNKEN         uniq  BDR_SUNKENOUTER or BDR_SUNKENINNER
EDGE_ETCHED         uniq  BDR_SUNKENOUTER or BDR_RAISEDINNER
EDGE_BUMP           uniq  BDR_RAISEDOUTER or BDR_SUNKENINNER

;Border flags */
;----------------------------
BF_LEFT             uniq  0x0001
BF_TOP              uniq  0x0002
BF_RIGHT            uniq  0x0004
BF_BOTTOM           uniq  0x0008
BF_TOPLEFT          uniq  BF_TOP or BF_LEFT
BF_TOPRIGHT         uniq  BF_TOP or BF_RIGHT
BF_BOTTOMLEFT       uniq  BF_BOTTOM or BF_LEFT
BF_BOTTOMRIGHT      uniq  BF_BOTTOM or BF_RIGHT
BF_RECT             uniq  BF_LEFT or BF_TOP or BF_RIGHT or BF_BOTTOM
 BF_DIAGONAL         uniq  0x0010

;For diagonal lines, the BF_RECT flags specify the end point of the
;vector bounded by the rectangle parameter.
;----------------------------
BF_DIAGONAL_ENDTOPRIGHT         uniq  BF_DIAGONAL or BF_TOP or BF_RIGHT
BF_DIAGONAL_ENDTOPLEFT          uniq  BF_DIAGONAL or BF_TOP or BF_LEFT
BF_DIAGONAL_ENDBOTTOMLEFT       uniq  BF_DIAGONAL or BF_BOTTOM or BF_LEFT
BF_DIAGONAL_ENDBOTTOMRIGHT      uniq  BF_DIAGONAL or BF_BOTTOM or BF_RIGHT
BF_MIDDLE           uniq  0x0800; Fill in the middle */
BF_SOFT             uniq  0x1000;For softer buttons */
BF_ADJUST           uniq  0x2000;Calculate the space left over */
BF_FLAT             uniq  0x4000;For flat rather than 3D borders */
BF_MONO             uniq  0x8000;For monochrome borders */

;Button Control Styles
;----------------------------
BS_PUSHBUTTON      uniq  0x00000000
BS_DEFPUSHBUTTON   uniq  0x00000001
BS_CHECKBOX        uniq  0x00000002
BS_AUTOCHECKBOX    uniq  0x00000003
BS_RADIOBUTTON     uniq  0x00000004
BS_3STATE          uniq  0x00000005
BS_AUTO3STATE      uniq  0x00000006
BS_GROUPBOX        uniq  0x00000007
BS_USERBUTTON      uniq  0x00000008
BS_AUTORADIOBUTTON uniq  0x00000009
BS_PUSHBOX         uniq  0x0000000A
BS_OWNERDRAW       uniq  0x0000000B
BS_TYPEMASK        uniq  0x0000000F
BS_LEFTTEXT        uniq  0x00000020
BS_TEXT            uniq  0x00000000
BS_ICON            uniq  0x00000040
BS_BITMAP          uniq  0x00000080
BS_LEFT            uniq  0x00000100
BS_RIGHT           uniq  0x00000200
BS_CENTER          uniq  0x00000300
BS_TOP             uniq  0x00000400
BS_BOTTOM          uniq  0x00000800
BS_VCENTER         uniq  0x00000C00
BS_PUSHLIKE        uniq  0x00001000
BS_MULTILINE       uniq  0x00002000
BS_NOTIFY          uniq  0x00004000
BS_FLAT            uniq  0x00008000
BS_RIGHTBUTTON     uniq   BS_LEFTTEXT

;User Button Notification Codes
;----------------------------
BN_CLICKED         uniq  0
BN_PAINT           uniq  1
BN_HILITE          uniq  2
BN_UNHILITE        uniq  3
BN_DISABLE         uniq  4
BN_DOUBLECLICKED   uniq  5
BN_PUSHED          uniq  BN_HILITE
BN_UNPUSHED        uniq  BN_UNHILITE
BN_DBLCLK          uniq  BN_DOUBLECLICKED
BN_SETFOCUS        uniq  6
BN_KILLFOCUS       uniq  7

;Button Control Messages
;----------------------------
BM_GETCHECK       uniq  0x00F0
BM_SETCHECK       uniq  0x00F1
BM_GETSTATE       uniq  0x00F2
BM_SETSTATE       uniq  0x00F3
BM_SETSTYLE       uniq  0x00F4
BM_CLICK          uniq  0x00F5
BM_GETIMAGE       uniq  0x00F6
BM_SETIMAGE       uniq  0x00F7
BM_SETDONTCLICK   uniq  0x00F8
BST_UNCHECKED     uniq  0x0000
BST_CHECKED       uniq  0x0001
BST_INDETERMINATE uniq  0x0002
BST_PUSHED        uniq  0x0004
BST_FOCUS         uniq  0x0008


;Window Messages
;----------------------------
WM_NULL                            uniq  0x0000
WM_CREATE                          uniq  0x0001
WM_DESTROY                         uniq  0x0002
WM_MOVE                            uniq  0x0003
WM_SIZE                            uniq  0x0005

WM_ACTIVATE                        uniq  0x0006
;WM_ACTIVATE state values
    WA_INACTIVE     uniq 0
    WA_ACTIVE       uniq 1
    WA_CLICKACTIVE  uniq 2

WM_SETFOCUS                        uniq  0x0007
WM_KILLFOCUS                       uniq  0x0008
WM_ENABLE                          uniq  0x000A
WM_SETREDRAW                       uniq  0x000B
WM_SETTEXT                         uniq  0x000C
WM_GETTEXT                         uniq  0x000D
WM_GETTEXTLENGTH                   uniq  0x000E
WM_PAINT                           uniq  0x000F
WM_CLOSE                           uniq  0x0010
WM_QUERYENDSESSION                 uniq  0x0011
WM_QUERYOPEN                       uniq  0x0013
WM_ENDSESSION                      uniq  0x0016
WM_QUIT                            uniq  0x0012
WM_ERASEBKGND                      uniq  0x0014
WM_SYSCOLORCHANGE                  uniq  0x0015
WM_SHOWWINDOW                      uniq  0x0018
WM_WININICHANGE                    uniq  0x001A
WM_SETTINGCHANGE                   uniq  WM_WININICHANGE
WM_DEVMODECHANGE                   uniq  0x001B
WM_ACTIVATEAPP                     uniq  0x001C
WM_FONTCHANGE                      uniq  0x001D
WM_TIMECHANGE                      uniq  0x001E
WM_CANCELMODE                      uniq  0x001F
WM_SETCURSOR                       uniq  0x0020
WM_MOUSEACTIVATE                   uniq  0x0021
WM_CHILDACTIVATE                   uniq  0x0022
WM_QUEUESYNC                       uniq  0x0023
WM_GETMINMAXINFO                   uniq  0x0024
WM_PAINTICON                       uniq  0x0026
WM_ICONERASEBKGND                  uniq  0x0027
WM_NEXTDLGCTL                      uniq  0x0028
WM_SPOOLERSTATUS                   uniq  0x002A
WM_DRAWITEM                        uniq  0x002B
WM_MEASUREITEM                     uniq  0x002C
WM_DELETEITEM                      uniq  0x002D
WM_VKEYTOITEM                      uniq  0x002E
WM_CHARTOITEM                      uniq  0x002F
WM_SETFONT                         uniq  0x0030
WM_GETFONT                         uniq  0x0031
WM_SETHOTKEY                       uniq  0x0032
WM_GETHOTKEY                       uniq  0x0033
WM_QUERYDRAGICON                   uniq  0x0037
WM_COMPAREITEM                     uniq  0x0039
WM_GETOBJECT                       uniq  0x003D
WM_COMPACTING                      uniq  0x0041
WM_COMMNOTIFY                      uniq  0x0044  ;/* no longer suported */
WM_WINDOWPOSCHANGING               uniq  0x0046
WM_WINDOWPOSCHANGED                uniq  0x0047

WM_POWER                           uniq  0x0048
;wParam for WM_POWER window message and DRV_POWER driver notification
     PWR_OK              uniq  1
     PWR_FAIL            uniq  -1
     PWR_SUSPENDREQUEST  uniq  1
     PWR_SUSPENDRESUME   uniq  2
     PWR_CRITICALRESUME  uniq  3

WM_COPYDATA                        uniq  0x004A
WM_CANCELJOURNAL                   uniq  0x004B
WM_NOTIFY                          uniq  0x004E
WM_INPUTLANGCHANGEREQUEST          uniq  0x0050
WM_INPUTLANGCHANGE                 uniq  0x0051
WM_TCARD                           uniq  0x0052
WM_HELP                            uniq  0x0053
WM_USERCHANGED                     uniq  0x0054
WM_NOTIFYFORMAT                    uniq  0x0055

NFR_ANSI                          uniq   1
NFR_UNICODE                       uniq   2
NF_QUERY                          uniq   3
NF_REQUERY                        uniq   4

WM_CONTEXTMENU                     uniq  0x007B
WM_STYLECHANGING                   uniq  0x007C
WM_STYLECHANGED                    uniq  0x007D
WM_DISPLAYCHANGE                   uniq  0x007E
WM_GETICON                         uniq  0x007F
WM_SETICON                         uniq  0x0080
WM_NCCREATE                        uniq  0x0081
WM_NCDESTROY                       uniq  0x0082
WM_NCCALCSIZE                      uniq  0x0083
WM_NCHITTEST                       uniq  0x0084
WM_NCPAINT                         uniq  0x0085
WM_NCACTIVATE                      uniq  0x0086
WM_GETDLGCODE                      uniq  0x0087
WM_SYNCPAINT                       uniq  0x0088
WM_NCMOUSEMOVE                     uniq  0x00A0
WM_NCLBUTTONDOWN                   uniq  0x00A1
WM_NCLBUTTONUP                     uniq  0x00A2
WM_NCLBUTTONDBLCLK                 uniq  0x00A3
WM_NCRBUTTONDOWN                   uniq  0x00A4
WM_NCRBUTTONUP                     uniq  0x00A5
WM_NCRBUTTONDBLCLK                 uniq  0x00A6
WM_NCMBUTTONDOWN                   uniq  0x00A7
WM_NCMBUTTONUP                     uniq  0x00A8
WM_NCMBUTTONDBLCLK                 uniq  0x00A9
WM_NCXBUTTONDOWN                   uniq  0x00AB
WM_NCXBUTTONUP                     uniq  0x00AC
WM_NCXBUTTONDBLCLK                 uniq  0x00AD
WM_INPUT_DEVICE_CHANGE             uniq  0x00FE
WM_INPUT                           uniq  0x00FF
WM_KEYFIRST                        uniq  0x0100
WM_KEYDOWN                         uniq  0x0100
WM_KEYUP                           uniq  0x0101
WM_CHAR                            uniq  0x0102
WM_DEADCHAR                        uniq  0x0103
WM_SYSKEYDOWN                      uniq  0x0104
WM_SYSKEYUP                        uniq  0x0105
WM_SYSCHAR                         uniq  0x0106
WM_SYSDEADCHAR                     uniq  0x0107
WM_UNICHAR                         uniq  0x0109
;WM_KEYLAST                         uniq  0x0109
UNICODE_NOCHAR                     uniq  0xFFFF
;WM_KEYLAST                         uniq  0x0108
WM_IME_STARTCOMPOSITION            uniq  0x010D
WM_IME_ENDCOMPOSITION              uniq  0x010E
WM_IME_COMPOSITION                 uniq  0x010F
WM_IME_KEYLAST                     uniq  0x010F
WM_INITDIALOG                      uniq  0x0110
WM_COMMAND                         uniq  0x0111
WM_SYSCOMMAND                      uniq  0x0112
WM_TIMER                           uniq  0x0113
WM_HSCROLL                         uniq  0x0114
WM_VSCROLL                         uniq  0x0115
WM_INITMENU                        uniq  0x0116
WM_INITMENUPOPUP                   uniq  0x0117
WM_GESTURE                         uniq  0x0119
WM_GESTURENOTIFY                   uniq  0x011A
WM_MENUSELECT                      uniq  0x011F
WM_MENUCHAR                        uniq  0x0120
WM_ENTERIDLE                       uniq  0x0121
WM_MENURBUTTONUP                   uniq  0x0122
WM_MENUDRAG                        uniq  0x0123
WM_MENUGETOBJECT                   uniq  0x0124
WM_UNINITMENUPOPUP                 uniq  0x0125
WM_MENUCOMMAND                     uniq  0x0126
WM_CHANGEUISTATE                   uniq  0x0127
WM_UPDATEUISTATE                   uniq  0x0128
WM_QUERYUISTATE                    uniq  0x0129

;LOWORD(wParam) values in WM_*UISTATE*
      UIS_SET                       uniq 1
      UIS_CLEAR                     uniq 2
      UIS_INITIALIZE                uniq 3

;HIWORD(wParam) values in WM_*UISTATE*
      UISF_HIDEFOCUS                uniq  0x1
      UISF_HIDEACCEL                uniq  0x2
      UISF_ACTIVE                   uniq  0x4

WM_CTLCOLORMSGBOX                  uniq  0x0132
WM_CTLCOLOREDIT                    uniq  0x0133
WM_CTLCOLORLISTBOX                 uniq  0x0134
WM_CTLCOLORBTN                     uniq  0x0135
WM_CTLCOLORDLG                     uniq  0x0136
WM_CTLCOLORSCROLLBAR               uniq  0x0137
WM_CTLCOLORSTATIC                  uniq  0x0138
MN_GETHMENU                        uniq  0x01E1
WM_MOUSEFIRST                      uniq  0x0200
WM_MOUSEMOVE                       uniq  0x0200
WM_LBUTTONDOWN                     uniq  0x0201
WM_LBUTTONUP                       uniq  0x0202
WM_LBUTTONDBLCLK                   uniq  0x0203
WM_RBUTTONDOWN                     uniq  0x0204
WM_RBUTTONUP                       uniq  0x0205
WM_RBUTTONDBLCLK                   uniq  0x0206
WM_MBUTTONDOWN                     uniq  0x0207
WM_MBUTTONUP                       uniq  0x0208
WM_MBUTTONDBLCLK                   uniq  0x0209
WM_MOUSEWHEEL                      uniq  0x020A
WM_XBUTTONDOWN                     uniq  0x020B
WM_XBUTTONUP                       uniq  0x020C
WM_XBUTTONDBLCLK                   uniq  0x020D
WM_MOUSEHWHEEL                     uniq  0x020E
WM_MOUSELAST                       uniq  0x020E
WM_PARENTNOTIFY                    uniq  0x0210
WM_ENTERMENULOOP                   uniq  0x0211
WM_EXITMENULOOP                    uniq  0x0212
WM_NEXTMENU                        uniq  0x0213
WM_SIZING                          uniq  0x0214
WM_CAPTURECHANGED                  uniq  0x0215
WM_MOVING                          uniq  0x0216
WM_POWERBROADCAST                  uniq  0x0218
WM_DEVICECHANGE                    uniq  0x0219
WM_MDICREATE                       uniq  0x0220
WM_MDIDESTROY                      uniq  0x0221
WM_MDIACTIVATE                     uniq  0x0222
WM_MDIRESTORE                      uniq  0x0223
WM_MDINEXT                         uniq  0x0224
WM_MDIMAXIMIZE                     uniq  0x0225
WM_MDITILE                         uniq  0x0226
WM_MDICASCADE                      uniq  0x0227
WM_MDIICONARRANGE                  uniq  0x0228
WM_MDIGETACTIVE                    uniq  0x0229
WM_MDISETMENU                      uniq  0x0230
WM_ENTERSIZEMOVE                   uniq  0x0231
WM_EXITSIZEMOVE                    uniq  0x0232
WM_DROPFILES                       uniq  0x0233
WM_MDIREFRESHMENU                  uniq  0x0234
WM_POINTERDEVICECHANGE             uniq  0x238
WM_POINTERDEVICEINRANGE            uniq  0x239
WM_POINTERDEVICEOUTOFRANGE         uniq  0x23A
WM_TOUCH                           uniq  0x0240
WM_NCPOINTERUPDATE                 uniq  0x0241
WM_NCPOINTERDOWN                   uniq  0x0242
WM_NCPOINTERUP                     uniq  0x0243
WM_POINTERUPDATE                   uniq  0x0245
WM_POINTERDOWN                     uniq  0x0246
WM_POINTERUP                       uniq  0x0247
WM_POINTERENTER                    uniq  0x0249
WM_POINTERLEAVE                    uniq  0x024A
WM_POINTERACTIVATE                 uniq  0x024B
WM_POINTERCAPTURECHANGED           uniq  0x024C
WM_TOUCHHITTESTING                 uniq  0x024D
WM_POINTERWHEEL                    uniq  0x024E
WM_POINTERHWHEEL                   uniq  0x024F
DM_POINTERHITTEST                  uniq  0x0250
WM_POINTERROUTEDTO                 uniq  0x0251
WM_POINTERROUTEDAWAY               uniq  0x0252
WM_POINTERROUTEDRELEASED           uniq  0x0253
WM_IME_SETCONTEXT                  uniq  0x0281
WM_IME_NOTIFY                      uniq  0x0282
WM_IME_CONTROL                     uniq  0x0283
WM_IME_COMPOSITIONFULL             uniq  0x0284
WM_IME_SELECT                      uniq  0x0285
WM_IME_CHAR                        uniq  0x0286
WM_IME_REQUEST                     uniq  0x0288
WM_IME_KEYDOWN                     uniq  0x0290
WM_IME_KEYUP                       uniq  0x0291
WM_MOUSEHOVER                      uniq  0x02A1
WM_MOUSELEAVE                      uniq  0x02A3
WM_NCMOUSEHOVER                    uniq  0x02A0
WM_NCMOUSELEAVE                    uniq  0x02A2
WM_WTSSESSION_CHANGE               uniq  0x02B1
WM_TABLET_FIRST                    uniq  0x02c0
WM_TABLET_LAST                     uniq  0x02df
WM_DPICHANGED                      uniq  0x02E0
WM_DPICHANGED_BEFOREPARENT         uniq  0x02E2
WM_DPICHANGED_AFTERPARENT          uniq  0x02E3
WM_GETDPISCALEDSIZE                uniq  0x02E4
WM_CUT                             uniq  0x0300
WM_COPY                            uniq  0x0301
WM_PASTE                           uniq  0x0302
WM_CLEAR                           uniq  0x0303
WM_UNDO                            uniq  0x0304
WM_RENDERFORMAT                    uniq  0x0305
WM_RENDERALLFORMATS                uniq  0x0306
WM_DESTROYCLIPBOARD                uniq  0x0307
WM_DRAWCLIPBOARD                   uniq  0x0308
WM_PAINTCLIPBOARD                  uniq  0x0309
WM_VSCROLLCLIPBOARD                uniq  0x030A
WM_SIZECLIPBOARD                   uniq  0x030B
WM_ASKCBFORMATNAME                 uniq  0x030C
WM_CHANGECBCHAIN                   uniq  0x030D
WM_HSCROLLCLIPBOARD                uniq  0x030E
WM_QUERYNEWPALETTE                 uniq  0x030F
WM_PALETTEISCHANGING               uniq  0x0310
WM_PALETTECHANGED                  uniq  0x0311
WM_HOTKEY                          uniq  0x0312
WM_PRINT                           uniq  0x0317
WM_PRINTCLIENT                     uniq  0x0318
WM_APPCOMMAND                      uniq  0x0319
WM_THEMECHANGED                    uniq  0x031A
WM_CLIPBOARDUPDATE                 uniq  0x031D
WM_DWMCOMPOSITIONCHANGED           uniq  0x031E
WM_DWMNCRENDERINGCHANGED           uniq  0x031F
WM_DWMCOLORIZATIONCOLORCHANGED     uniq  0x0320
WM_DWMWINDOWMAXIMIZEDCHANGE        uniq  0x0321
WM_DWMSENDICONICTHUMBNAIL              uniq  0x0323
WM_DWMSENDICONICLIVEPREVIEWBITMAP      uniq  0x0326
WM_GETTITLEBARINFOEX               uniq  0x033F
WM_HANDHELDFIRST                   uniq  0x0358
WM_HANDHELDLAST                    uniq  0x035F
WM_AFXFIRST                        uniq  0x0360
WM_AFXLAST                         uniq  0x037F
WM_PENWINFIRST                     uniq  0x0380
WM_PENWINLAST                      uniq  0x038F
WM_APP                             uniq  0x8000

;NOTE: All Message Numbers below    uniq  0x0400 are RESERVED.
;Private Window Messages Start Here:
WM_USER                            uniq  0x0400

;wParam for WM_SIZING message  */
    WMSZ_LEFT          uniq  1
    WMSZ_RIGHT         uniq  2
    WMSZ_TOP           uniq  3
    WMSZ_TOPLEFT       uniq  4
    WMSZ_TOPRIGHT      uniq  5
    WMSZ_BOTTOM        uniq  6
    WMSZ_BOTTOMLEFT    uniq  7
    WMSZ_BOTTOMRIGHT   uniq  8

;WM_NCHITTEST and MOUSEHOOKSTRUCT Mouse Position Codes
HTERROR            uniq  -2
HTTRANSPARENT      uniq  -1
HTNOWHERE          uniq  0
HTCLIENT           uniq  1
HTCAPTION          uniq  2
HTSYSMENU          uniq  3
HTGROWBOX          uniq  4
HTSIZE             uniq  HTGROWBOX
HTMENU             uniq  5
HTHSCROLL          uniq  6
HTVSCROLL          uniq  7
HTMINBUTTON        uniq  8
HTMAXBUTTON        uniq  9
HTLEFT             uniq  10
HTRIGHT            uniq  11
HTTOP              uniq  12
HTTOPLEFT          uniq  13
HTTOPRIGHT         uniq  14
HTBOTTOM           uniq  15
HTBOTTOMLEFT       uniq  16
HTBOTTOMRIGHT      uniq  17
HTBORDER           uniq  18
HTREDUCE           uniq  HTMINBUTTON
HTZOOM             uniq  HTMAXBUTTON
HTSIZEFIRST        uniq  HTLEFT
HTSIZELAST         uniq  HTBOTTOMRIGHT
HTOBJECT           uniq  19
HTCLOSE            uniq  20
HTHELP             uniq  21

;SendMessageTimeout values

SMTO_NORMAL            uniq  0x0000
SMTO_BLOCK             uniq  0x0001
SMTO_ABORTIFHUNG       uniq  0x0002
SMTO_NOTIMEOUTIFNOTHUNG    uniq  0x0008
SMTO_ERRORONEXIT       uniq  0x0020

;WM_MOUSEACTIVATE Return Codes
MA_ACTIVATE         uniq 1
MA_ACTIVATEANDEAT   uniq 2
MA_NOACTIVATE       uniq 3
MA_NOACTIVATEANDEAT uniq 4

;WM_SETICON / WM_GETICON Type Codes
ICON_SMALL         uniq 0
ICON_BIG           uniq 1
ICON_SMALL2        uniq 2

;WM_SIZE message wParam values
SIZE_RESTORED      uniq 0
SIZE_MINIMIZED     uniq 1
SIZE_MAXIMIZED     uniq 2
SIZE_MAXSHOW       uniq 3
SIZE_MAXHIDE       uniq 4

;Obsolete constant names
SIZENORMAL         uniq SIZE_RESTORED
SIZEICONIC         uniq SIZE_MINIMIZED
SIZEFULLSCREEN     uniq SIZE_MAXIMIZED
SIZEZOOMSHOW       uniq SIZE_MAXSHOW
SIZEZOOMHIDE       uniq SIZE_MAXHIDE

;WM_NCCALCSIZE "window valid rect" return values
WVR_ALIGNTOP           uniq  0x0010
WVR_ALIGNLEFT          uniq  0x0020
WVR_ALIGNBOTTOM        uniq  0x0040
WVR_ALIGNRIGHT         uniq  0x0080
WVR_HREDRAW            uniq  0x0100
WVR_VREDRAW            uniq  0x0200
WVR_REDRAW          uniq  WVR_HREDRAW or  WVR_VREDRAW
WVR_VALIDRECTS         uniq  0x0400

;Key State Masks for Mouse Messages
MK_LBUTTON             uniq  0x0001
MK_RBUTTON             uniq  0x0002
MK_SHIFT               uniq  0x0004
MK_CONTROL             uniq  0x0008
MK_MBUTTON             uniq  0x0010
MK_XBUTTON1            uniq  0x0020
MK_XBUTTON2            uniq  0x0040

TME_HOVER          uniq  0x00000001
TME_LEAVE          uniq  0x00000002
TME_NONCLIENT      uniq  0x00000010
TME_QUERY          uniq  0x40000000
TME_CANCEL         uniq  0x80000000


HOVER_DEFAULT      uniq  0xFFFFFFFF


;ShowWindow() Commands
;----------------------------
SW_HIDE            uniq 0
SW_SHOWNORMAL      uniq 1
SW_NORMAL          uniq 1
SW_SHOWMINIMIZED   uniq 2
SW_SHOWMAXIMIZED   uniq 3
SW_MAXIMIZE        uniq 3
SW_SHOWNOACTIVATE  uniq 4
SW_SHOW            uniq 5
SW_MINIMIZE        uniq 6
SW_SHOWMINNOACTIVE uniq 7
SW_SHOWNA          uniq 8
SW_RESTORE         uniq 9
SW_SHOWDEFAULT     uniq 10
SW_FORCEMINIMIZE   uniq 11
SW_MAX             uniq 11

;----------------------------
CW_USEDEFAULT           uniq  0x80000000

;----------------------------

IMAGE_BITMAP            uniq  0
IMAGE_ICON              uniq  1
IMAGE_CURSOR            uniq  2
IMAGE_ENHMETAFILE       uniq  3

LR_DEFAULTCOLOR         uniq  0x00000000
LR_MONOCHROME           uniq  0x00000001
LR_COLOR                uniq  0x00000002
LR_COPYRETURNORG        uniq  0x00000004
LR_COPYDELETEORG        uniq  0x00000008
LR_LOADFROMFILE         uniq  0x00000010
LR_LOADTRANSPARENT      uniq  0x00000020
LR_DEFAULTSIZE          uniq  0x00000040
LR_VGACOLOR             uniq  0x00000080
LR_LOADMAP3DCOLORS      uniq  0x00001000
LR_CREATEDIBSECTION     uniq  0x00002000
LR_COPYFROMRESOURCE     uniq  0x00004000
LR_SHARED               uniq  0x00008000


;Color Types
;----------------------------
CTLCOLOR_MSGBOX         uniq  0
CTLCOLOR_EDIT           uniq  1
CTLCOLOR_LISTBOX        uniq  2
CTLCOLOR_BTN            uniq  3
CTLCOLOR_DLG            uniq  4
CTLCOLOR_SCROLLBAR      uniq  5
CTLCOLOR_STATIC         uniq  6
CTLCOLOR_MAX            uniq  7

COLOR_SCROLLBAR         uniq  0
COLOR_BACKGROUND        uniq  1
COLOR_ACTIVECAPTION     uniq  2
COLOR_INACTIVECAPTION   uniq  3
COLOR_MENU              uniq  4
COLOR_WINDOW            uniq  5
COLOR_WINDOWFRAME       uniq  6
COLOR_MENUTEXT          uniq  7
COLOR_WINDOWTEXT        uniq  8
COLOR_CAPTIONTEXT       uniq  9
COLOR_ACTIVEBORDER      uniq  10
COLOR_INACTIVEBORDER    uniq  11
COLOR_APPWORKSPACE      uniq  12
COLOR_HIGHLIGHT         uniq  13
COLOR_HIGHLIGHTTEXT     uniq  14
COLOR_BTNFACE           uniq  15
COLOR_BTNSHADOW         uniq  16
COLOR_GRAYTEXT          uniq  17
COLOR_BTNTEXT           uniq  18
COLOR_INACTIVECAPTIONTEXT uniq  19
COLOR_BTNHIGHLIGHT      uniq  20

COLOR_3DDKSHADOW        uniq  21
COLOR_3DLIGHT           uniq  22
COLOR_INFOTEXT          uniq  23
COLOR_INFOBK            uniq  24

COLOR_HOTLIGHT          uniq  26
COLOR_GRADIENTACTIVECAPTION uniq  27
COLOR_GRADIENTINACTIVECAPTION uniq  28

COLOR_MENUHILIGHT       uniq  29
COLOR_MENUBAR           uniq  30

COLOR_DESKTOP           uniq  COLOR_BACKGROUND
COLOR_3DFACE            uniq  COLOR_BTNFACE
COLOR_3DSHADOW          uniq  COLOR_BTNSHADOW
COLOR_3DHIGHLIGHT       uniq  COLOR_BTNHIGHLIGHT
COLOR_3DHILIGHT         uniq  COLOR_BTNHIGHLIGHT
COLOR_BTNHILIGHT        uniq  COLOR_BTNHIGHLIGHT


;Window Styles
WS_OVERLAPPED                uniq  0x00000000
WS_POPUP                     uniq  0x80000000
WS_CHILD                     uniq  0x40000000
WS_MINIMIZE                  uniq  0x20000000
WS_VISIBLE                   uniq  0x10000000
WS_DISABLED                  uniq  0x08000000
WS_CLIPSIBLINGS              uniq  0x04000000
WS_CLIPCHILDREN              uniq  0x02000000
WS_MAXIMIZE                  uniq  0x01000000
WS_CAPTION                   uniq  0x00C00000;WS_BORDER | WS_DLGFRAME
WS_BORDER                    uniq  0x00800000
WS_DLGFRAME                  uniq  0x00400000
WS_VSCROLL                   uniq  0x00200000
WS_HSCROLL                   uniq  0x00100000
WS_SYSMENU                   uniq  0x00080000
WS_THICKFRAME                uniq  0x00040000
WS_GROUP                     uniq  0x00020000
WS_TABSTOP                   uniq  0x00010000

WS_MINIMIZEBOX               uniq  0x00020000
WS_MAXIMIZEBOX               uniq  0x00010000

WS_TILED                       uniq  WS_OVERLAPPED
WS_ICONIC                      uniq  WS_MINIMIZE
WS_SIZEBOX                     uniq  WS_THICKFRAME
WS_TILEDWINDOW                 uniq  WS_OVERLAPPEDWINDOW

;Common Window Styles
WS_OVERLAPPEDWINDOW            uniq  WS_OVERLAPPED or WS_CAPTION or WS_SYSMENU or WS_THICKFRAME or WS_MINIMIZEBOX or WS_MAXIMIZEBOX
WS_POPUPWINDOW                 uniq  WS_POPUP or WS_BORDER or WS_SYSMENU
WS_CHILDWINDOW                 uniq  WS_CHILD

;Extended Window Styles
WS_EX_DLGMODALFRAME              uniq  0x00000001
WS_EX_NOPARENTNOTIFY             uniq  0x00000004
WS_EX_TOPMOST                    uniq  0x00000008
WS_EX_ACCEPTFILES                uniq  0x00000010
WS_EX_TRANSPARENT                uniq  0x00000020
WS_EX_MDICHILD                   uniq  0x00000040
WS_EX_TOOLWINDOW                 uniq  0x00000080
WS_EX_WINDOWEDGE                 uniq  0x00000100
WS_EX_CLIENTEDGE                 uniq  0x00000200
WS_EX_CONTEXTHELP                uniq  0x00000400

WS_EX_RIGHT                      uniq  0x00001000
WS_EX_LEFT                       uniq  0x00000000
WS_EX_RTLREADING                 uniq  0x00002000
WS_EX_LTRREADING                 uniq  0x00000000
WS_EX_LEFTSCROLLBAR              uniq  0x00004000
WS_EX_RIGHTSCROLLBAR             uniq  0x00000000

WS_EX_CONTROLPARENT              uniq  0x00010000
WS_EX_STATICEDGE                 uniq  0x00020000
WS_EX_APPWINDOW                  uniq  0x00040000


WS_EX_OVERLAPPEDWINDOW         uniq  WS_EX_WINDOWEDGE or WS_EX_CLIENTEDGE
WS_EX_PALETTEWINDOW            uniq  WS_EX_WINDOWEDGE or WS_EX_TOOLWINDOW or WS_EX_TOPMOST



; Class styles
;----------------------------
CS_VREDRAW             uniq  0x0001
CS_HREDRAW             uniq  0x0002
CS_DBLCLKS             uniq  0x0008
CS_OWNDC               uniq  0x0020
CS_CLASSDC             uniq  0x0040
CS_PARENTDC            uniq  0x0080
CS_NOCLOSE             uniq  0x0200
CS_SAVEBITS            uniq  0x0800
CS_BYTEALIGNCLIENT     uniq  0x1000
CS_BYTEALIGNWINDOW     uniq  0x2000
CS_GLOBALCLASS         uniq  0x4000

;Resource types
;----------------------------
RT_CURSOR                           uniq 1
RT_BITMAP                           uniq 2
RT_ICON                             uniq 3
RT_MENU                             uniq 4
RT_DIALOG                           uniq 5
RT_STRING                           uniq 6
RT_FONTDIR                          uniq 7
RT_FONT                             uniq 8
RT_ACCELERATOR                      uniq 9
RT_RCDATA                           uniq 10
RT_MESSAGETABLE                     uniq 11
RT_GROUP_CURSOR                     uniq 12
RT_GROUP_ICON                       uniq 14
RT_VERSION                          uniq 16
RT_DLGINCLUDE                       uniq 17
RT_PLUGPLAY                         uniq 19
RT_VXD                              uniq 20
RT_ANICURSOR                        uniq 21
RT_ANIICON                          uniq 22
RT_HTML                             uniq 23
RT_MANIFEST                         uniq 24


;MessageBox() Flags
;----------------------------
MB_OK                            uniq  0x00000000
MB_OKCANCEL                      uniq  0x00000001
MB_ABORTRETRYIGNORE              uniq  0x00000002
MB_YESNOCANCEL                   uniq  0x00000003
MB_YESNO                         uniq  0x00000004
MB_RETRYCANCEL                   uniq  0x00000005

MB_CANCELTRYCONTINUE             uniq  0x00000006



MB_ICONHAND                      uniq  0x00000010
MB_ICONQUESTION                  uniq  0x00000020
MB_ICONEXCLAMATION               uniq  0x00000030
MB_ICONASTERISK                  uniq  0x00000040


MB_USERICON                      uniq  0x00000080
MB_ICONWARNING                   uniq MB_ICONEXCLAMATION
MB_ICONERROR                     uniq MB_ICONHAND


MB_ICONINFORMATION               uniq MB_ICONASTERISK
MB_ICONSTOP                      uniq MB_ICONHAND

MB_DEFBUTTON1                    uniq  0x00000000
MB_DEFBUTTON2                    uniq  0x00000100
MB_DEFBUTTON3                    uniq  0x00000200

MB_DEFBUTTON4                    uniq  0x00000300


MB_APPLMODAL                     uniq  0x00000000
MB_SYSTEMMODAL                   uniq  0x00001000
MB_TASKMODAL                     uniq  0x00002000

MB_HELP                          uniq  0x00004000; Help Button


MB_NOFOCUS                       uniq  0x00008000
MB_SETFOREGROUND                 uniq  0x00010000
MB_DEFAULT_DESKTOP_ONLY          uniq  0x00020000


MB_TOPMOST                       uniq  0x00040000
MB_RIGHT                         uniq  0x00080000
MB_RTLREADING                    uniq  0x00100000

MB_SERVICE_NOTIFICATION          uniq  0x00200000


MB_TYPEMASK                      uniq  0x0000000F
MB_ICONMASK                      uniq  0x000000F0
MB_DEFMASK                       uniq  0x00000F00
MB_MODEMASK                      uniq  0x00003000
MB_MISCMASK                      uniq  0x0000C000


;The following are masks for the predefined standard access types
;----------------------------
DELETE                           uniq 0x00010000
READ_CONTROL                     uniq 0x00020000
WRITE_DAC                        uniq 0x00040000
WRITE_OWNER                      uniq 0x00080000
SYNCHRONIZE                      uniq 0x00100000
STANDARD_RIGHTS_REQUIRED         uniq 0x000F0000
STANDARD_RIGHTS_READ             uniq READ_CONTROL
STANDARD_RIGHTS_WRITE            uniq READ_CONTROL
STANDARD_RIGHTS_EXECUTE          uniq READ_CONTROL
STANDARD_RIGHTS_ALL              uniq 0x001F0000
SPECIFIC_RIGHTS_ALL              uniq 0x0000FFFF

;Registry Specific Access Rights.
;----------------------------
KEY_QUERY_VALUE                  uniq 0x0001
KEY_SET_VALUE                    uniq 0x0002
KEY_CREATE_SUB_KEY               uniq 0x0004
KEY_ENUMERATE_SUB_KEYS           uniq 0x0008
KEY_NOTIFY                       uniq 0x0010
KEY_CREATE_LINK                  uniq 0x0020
KEY_WOW64_32KEY                  uniq 0x0200
KEY_WOW64_64KEY                  uniq 0x0100
KEY_WOW64_RES                    uniq 0x0300
KEY_READ                         uniq ((STANDARD_RIGHTS_READ or KEY_QUERY_VALUE or KEY_ENUMERATE_SUB_KEYS or KEY_NOTIFY) and (not SYNCHRONIZE))
KEY_WRITE                        uniq ((STANDARD_RIGHTS_WRITE or KEY_SET_VALUE or KEY_CREATE_SUB_KEY) and (not SYNCHRONIZE))
KEY_EXECUTE                      uniq ((KEY_READ) and (not SYNCHRONIZE))
KEY_ALL_ACCESS                   uniq ((STANDARD_RIGHTS_ALL or KEY_QUERY_VALUE or KEY_SET_VALUE or KEY_CREATE_SUB_KEY or KEY_ENUMERATE_SUB_KEYS or KEY_NOTIFY or KEY_CREATE_LINK)\
                                       and (not SYNCHRONIZE))

