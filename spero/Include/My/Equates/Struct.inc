
;----------------------------
struct POINT
 x                  dd  ?
 y                  dd  ?
ends

;----------------------------
struct RECT
 left               dd  ?
 top                dd  ?
 right              dd  ?
 bottom             dd  ?
ends

;----------------------------
struct NMHDR
 hwndFrom                      LpType  ?
 idFrom                        LpType  ?
 code                              dd  ?;// NM_ code
ends

;----------------------------
struct LVHITTESTINFO
 pt                 POINT
 flags              dd  ?
 iItem              dd  ?
 iSubItem           dd  ?;this is was NOT in win95.  valid only for LVM_SUBITEMHITTEST
 iGroup             dd  ?;readonly. index of group. only valid for owner data. supports single item in multiple groups.
ends


;----------------------------
struct MOUSEINPUT
 dx                         dd  ?
 dy                         dd  ?
 mouseData                  dd  ?
 dwFlags                    dd  ?
 time                       dd  ?
 dwExtraInfo                LpType  ?
ends

;----------------------------
struct KEYBDINPUT
 wVk                        dw  ?
 wScan                      dw  ?
 dwFlags                    dd  ?
 time                       dd  ?
 dwExtraInfo                LpType  ?
ends


;----------------------------
struct HARDWAREINPUT
 uMsg                       dd  ?
 wParamL                    dw  ?
 wParamH                    dw  ?
ends



;----------------------------
struct INPUT
 type                       dd  ?

 union
  mi                   MOUSEINPUT
  ki                   KEYBDINPUT
  hi                   HARDWAREINPUT
 ends
ends


;----------------------------
struct RTL_SRWLOCK
 Ptr                LpType  RTL_SRWLOCK_INIT
ends




;----------------------------
struct LVKEYDOWN
 hdr                      NMHDR
 wVKey                    dw  ?
 flags                    dd  ?
ends


;----------------------------
struct MENUITEMINFOW
 cbSize                   dd  ?
 fMask                    dd  ?
 fType                    dd  ?;used if MIIM_TYPE (4.0) or MIIM_FTYPE (>4.0)
 fState                   dd  ?;used if MIIM_STATE
 wID                      dd  ?;used if MIIM_ID
 hSubMenu             LpType  ?;used if MIIM_SUBMENU
 hbmpChecked          LpType  ?;used if MIIM_CHECKMARKS
 hbmpUnchecked        LpType  ?;used if MIIM_CHECKMARKS
 dwItemData           LpType  ?;used if MIIM_DATA
 dwTypeData           LpType  ?;used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
 cch                      dd  ?;// used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
 hbmpItem             LpType  ?;used if MIIM_BITMAP
ends

;--------------------------------------
struct NOTIFYICONDATAW;Данная версия структуры для Wiondows не раньше Vista
 cbSize                            dd  ?
 hWnd                              LpType  ?
 uID                               dd  ?
 uFlags                            dd  ?
 uCallbackMessage                  dd  ?
 hIcon                             LpType  ?
 szTip                             du  128 dup(?)
 dwState                           dd  ?
 dwStateMask                       dd  ?
 szInfo                            du  256 dup(?)

 union
  uTimeout                         dd  ?
  uVersion                         dd  ?;used with NIM_SETVERSION, values 0, 3 and 4
 ends

 szInfoTitle                       du  64  dup(?)
 dwInfoFlags                       dd  ?
 guidItem                          GUID
 hBalloonIcon                      LpType  ?
ends


;-------------------------------------------------------------------------
struct OSVERSIONINFOEXW
  dwOSVersionInfoSize          dd ?
  dwMajorVersion               dd ?
  dwMinorVersion               dd ?
  dwBuildNumber                dd ?
  dwPlatformId                 dd ?
  szCSDVersion                 du 128 dup(?);Maintenance string for PSS usage
  wServicePackMajor            dw ?
  wServicePackMinor            dw ?
  wSuiteMask                   dw ?
  wProductType                 db ?
  wReserved                    db ?
ends


EXCEPTION_MAXIMUM_PARAMETERS     uniq 15;maximum number of exception parameters
MAXIMUM_SUPPORTED_EXTENSION      uniq 512
SIZE_OF_80387_REGISTERS          uniq 80

;-------------------------------------------------------------------------
struct EXCEPTION_POINTERS
  ExceptionRecord              LpType ?
  ContextRecord                LpType ?
ends

struct EXCEPTION_RECORD
  ExceptionCode                dd ?
  ExceptionFlags               dd ?
  ExceptionRecord              LpType ?
  ExceptionAddress             LpType ?
  NumberParameters             dd ?
  ExceptionInformation         LpType EXCEPTION_MAXIMUM_PARAMETERS dup(?)
ends

struct FLOATING_SAVE_AREA
  ControlWord                  dd ?
  StatusWord                   dd ?
  TagWord                      dd ?
  ErrorOffset                  dd ?
  ErrorSelector                dd ?
  DataOffset                   dd ?
  DataSelector                 dd ?
  RegisterArea                 db SIZE_OF_80387_REGISTERS dup(?)
  Spare0                       dd ?
ends

;Context Frame
;This frame has a several purposes: 1) it is used as an argument to NtContinue, 2) is is used to constuct a call frame for APC delivery,and
;3) it is used in the user level thread creation routines.;The layout of the record conforms to a standard call frame.
 struct CONTEXTx86
    ;The flags values within this flag control the contents of a CONTEXT record.
    ;If the context record is used as an input parameter, then for each portion of the context record controlled by a flag whose value is set, it is assumed that that portion of the
    ; context record contains valid context. If the context record is being used to modify a threads context, then only that portion of the threads context will be modified.
    ;If the context record is used as an IN OUT parameter to capture the context of a thread, then only those portions of the thread's context corresponding to set flags will be returned.
    ;The context record is never used as an OUT only parameter.

  ContextFlags                 dd ?
    ;This section is specified/returned if CONTEXT_DEBUG_REGISTERS is set in ContextFlags.  Note that CONTEXT_DEBUG_REGISTERS is NOT included in CONTEXT_FULL.
  Dr0                          dd ?
  Dr1                          dd ?
  Dr2                          dd ?
  Dr3                          dd ?
  Dr6                          dd ?
  Dr7                          dd ?
    ;This section is specified/returned if the ContextFlags word contians the flag CONTEXT_FLOATING_POINT.
  FloatSave                    FLOATING_SAVE_AREA
    ;This section is specified/returned if the  ContextFlags word contians the flag CONTEXT_SEGMENTS.
  SegGs                        dd ?
  SegFs                        dd ?
  SegEs                        dd ?
  SegDs                        dd ?
    ;This section is specified/returned if the ContextFlags word contians the flag CONTEXT_INTEGER.
  Edi                          dd ?
  Esi                          dd ?
  Ebx                          dd ?
  Edx                          dd ?
  Ecx                          dd ?
  Eax                          dd ?
    ;This section is specified/returned if the ContextFlags word contians the flag CONTEXT_CONTROL.
  Ebp                          dd ?
  Eip                          dd ?
  SegCs                        dd ?; MUST BE SANITIZED
  EFlags                       dd ?; MUST BE SANITIZED
  Esp                          dd ?
  SegSs                        dd ?
    ;This section is specified/returned if the ContextFlags word  contains the flag CONTEXT_EXTENDED_REGISTERS. The format and contexts are processor specific
  ExtendedRegisters            db MAXIMUM_SUPPORTED_EXTENSION dup(?)
ends

;-------------------------------------------------------------------------
struct LARGE_INTEGER
 union
  struct
   LowPart                      dd ?
   HighPart                     dd ?
  ends

  QuadPart                      dq ?
 ends
ends

;Logical Font */
LF_FACESIZE        uniq   32
;--------------------------------------
struct LOGFONTW
 lfHeight                      dd  ?
 lfWidth                       dd  ?
 lfEscapement                  dd  ?
 lfOrientation                 dd  ?
 lfWeight                      dd  ?
 lfItalic                      db  ?
 lfUnderline                   db  ?
 lfStrikeOut                   db  ?
 lfCharSet                     db  ?
 lfOutPrecision                db  ?
 lfClipPrecision               db  ?
 lfQuality                     db  ?
 lfPitchAndFamily              db  ?
 lfFaceName                    du LF_FACESIZE dup(?)
ends


struct TEXTMETRICW
 tmHeight                       dd  ?
 tmAscent                       dd  ?
 tmDescent                      dd  ?
 tmInternalLeading              dd  ?
 tmExternalLeading              dd  ?
 tmAveCharWidth                 dd  ?
 tmMaxCharWidth                 dd  ?
 tmWeight                       dd  ?
 tmOverhang                     dd  ?
 tmDigitizedAspectX             dd  ?
 tmDigitizedAspectY             dd  ?
 tmFirstChar                    dw  ?
 tmLastChar                     dw  ?
 tmDefaultChar                  dw  ?
 tmBreakChar                    dw  ?
 tmItalic                       db  ?
 tmUnderlined                   db  ?
 tmStruckOut                    db  ?
 tmPitchAndFamily               db  ?
 tmCharSet                      db  ?
ends


;----------------------------
struct WNDCLASSEXW
 cbSize                      dd  ?
 style                       dd  ?
 lpfnWndProc             LpType  ?
 cbClsExtra                  dd  ?
 cbWndExtra                  dd  ?
 hInstance               LpType  ?
 hIcon                   LpType  ?
 hCursor                 LpType  ?
 hbrBackground           LpType  ?
 lpszMenuName                dd  ?
 lpszClassName               dd  ?
 hIconSm                 LpType  ?
ends

;Message structure
;----------------------------
struct MSG
 hwnd                    LpType  ?
 message                     dd  ?
 wParam                      dd  ?
 lParam                      dd  ?
 time                        dd  ?
 pt                       POINT
ends

;----------------------------
struct PAINTSTRUCT
 hdc                     LpType  ?
 fErase                      dd  ?
 rcPaint                   RECT
 fRestore                    dd  ?
 fIncUpdate                  dd  ?
 rgbReserved                 db  32 dup(?)
ends

;----------------------------
struct WINDOWPLACEMENT
 length                      dd  ?
 flags                       dd  ?
 showCmd                     dd  ?
 ptMinPosition               POINT
 ptMaxPosition               POINT
 rcNormalPosition            RECT
ends

;----------------------------
struct WINDOWPOS
 hwnd                       dd  ?
 hwndInsertAfter            dd  ?
 x                          dd  ?
 y                          dd  ?
 cx                         dd  ?
 cy                         dd  ?
 flags                      dd  ?
ends




;NMITEMACTIVATE is used instead of NMLISTVIEW in IE >= 0x400
;therefore all the fields are the same except for extra uKeyFlags
;they are used to store key flags at the time of the single click with
;delayed activation - because by the time the timer goes off a user may
;not hold the keys (shift, ctrl) any more
;----------------------------
struct NMITEMACTIVATE
 hdr                               NMHDR
 iItem                             dd  ?
 iSubItem                          dd  ?
 uNewState                         dd  ?
 uOldState                         dd  ?
 uChanged                          dd  ?
 ptAction                          POINT
 lParam                        LpType  ?
 uKeyFlags                         dd  ?
ends

;----------------------------
struct LVITEMW
 mask                              dd  ?
 iItem                             dd  ?
 iSubItem                          dd  ?
 state                             dd  ?
 stateMask                         dd  ?
 pszText                       LpType  ?
 cchTextMax                        dd  ?
 iImage                            dd  ?
 lParam                        LpType  ?
 iIndent                           dd  ?
 iGroupId                          dd  ?
 cColumns                          dd  ?; // tile view columns
 puColumns                     LpType  ?
 piColFmt                          dd  ?
 iGroup                            dd  ?; // readonly. only valid for owner data.
ends

;----------------------------
struct LVGROUPW
 cbSize                            dd  ?
 mask                              dd  ?
 pszHeader                     LpType  ?
 cchHeader                         dd  ?
 pszFooter                     LpType  ?
 cchFooter                         dd  ?
 iGroupId                          dd  ?
 stateMask                         dd  ?
 state                             dd  ?
 uAlign                            dd  ?
 pszSubtitle                   LpType  ?
 cchSubtitle                       dd  ?
 pszTask                       LpType  ?
 cchTask                           dd  ?
 pszDescriptionTop             LpType  ?
 cchDescriptionTop                 dd  ?
 pszDescriptionBottom          LpType  ?
 cchDescriptionBottom              dd  ?
 iTitleImage                       dd  ?
 iExtendedImage                    dd  ?
 iFirstItem                        dd  ?    ;// Read only
 cItems                            dd  ?    ; // Read only
 pszSubsetTitle                LpType  ?  ;  // NULL if group is not subset
 cchSubsetTitle                    dd  ?
ends



;----------------------------
struct LVCOLUMNW
 mask                              dd  ?
 fmt                               dd  ?
 cx                                dd  ?
 pszText                       LpType  ?
 cchTextMax                        dd  ?
 iSubItem                          dd  ?
 iImage                            dd  ?
 iOrder                            dd  ?
 cxMin                             dd  ?;min snap point
 cxDefault                         dd  ?; default snap point
 cxIdeal                           dd  ?; read only. ideal may not eqaul current width if auto sized (LVS_EX_AUTOSIZECOLUMNS) to a lesser width.
ends



;----------------------------
struct INITCOMMONCONTROLSEX
 dwSize                            dd  ?;size of this structure
 dwICC                             dd  ?;flags indicating which classes to be initialized
ends


;----------------------------
struct TCITEMW
 mask                              dd  ?
 dwState                           dd  ?
 dwStateMask                       dd  ?
 pszText                       LpType  ?
 cchTextMax                        dd  ?
 iImage                            dd  ?
 lParam                        LpType  ?
ends


;----------------------------
struct KBDLLHOOKSTRUCT
 vkCode                            dd  ?
 scanCode                          dd  ?
 flags                             dd  ?
 time                              dd  ?
 dwExtraInfo                   LpType  ?
ends;

;----------------------------
struct MSLLHOOKSTRUCT
 pt                                POINT
 mouseData                         dd  ?
 flags                             dd  ?
 time                              dd  ?
 dwExtraInfo                   LpType  ?
ends

;----------------------------
struct OPENFILENAMEW
 lStructSize                          dd  ?
 hwndOwner                        LpType  ?
 hInstance                        LpType  ?
 lpstrFilter                      LpType  ?
 lpstrCustomFilter                LpType  ?
 nMaxCustFilter                       dd  ?
 nFilterIndex                         dd  ?
 lpstrFile                        LpType  ?
 nMaxFile                             dd  ?
 lpstrFileTitle                   LpType  ?
 nMaxFileTitle                        dd  ?
 lpstrInitialDir                  LpType  ?
 lpstrTitle                       LpType  ?
 Flags                                dd  ?
 nFileOffset                          dw  ?
 nFileExtension                       dw  ?
 lpstrDefExt                      LpType  ?
 lCustData                        LpType  ?
 lpfnHook                         LpType  ?
 lpTemplateName                   LpType  ?
 pvReserved                       LpType  ?
 dwReserved                           dd  ?
 FlagsEx                              dd  ?
ends

;----------------------------
struct PHYSICAL_MONITOR
 hPhysicalMonitor                LpType  ?
 szPhysicalMonitorDescription    dw PHYSICAL_MONITOR_DESCRIPTION_SIZE dup(?)
ends

;----------------------------
struct SHELLEXECUTEINFOW
 cbSize                               dd  ?;in, required, sizeof of this structure
 fMask                                dd  ?;in, SEE_MASK_XXX values
 hwnd                             LpType  ?; in, optional
 lpVerb                           LpType  ?;in, optional when unspecified the default verb is choosen
 lpFile                           LpType  ?; in, either this value or lpIDList must be specified
 lpParameters                     LpType  ?;in, optional
 lpDirectory                      LpType  ?;in, optional
 nShow                                dd  ?;in, required
 hInstApp                         LpType  ?;out when SEE_MASK_NOCLOSEPROCESS is specified
 lpIDList                         LpType  ?;in, valid when SEE_MASK_IDLIST is specified, PCIDLIST_ABSOLUTE, for use with SEE_MASK_IDLIST & SEE_MASK_INVOKEIDLIST
 lpClass                          LpType  ?; in, valid when SEE_MASK_CLASSNAME is specified
 hkeyClass                        LpType  ?;in, valid when SEE_MASK_CLASSKEY is specified
 dwHotKey                             dd  ?;in, valid when SEE_MASK_HOTKEY is specified

 union
  hIcon                           LpType  ?;not used
  hMonitor                        LpType  ?;in, valid when SEE_MASK_HMONITOR specified
 ends

  hProcess                        LpType  ?;out, valid when SEE_MASK_NOCLOSEPROCESS
ends
