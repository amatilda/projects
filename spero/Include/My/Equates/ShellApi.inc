
;Begin ShellExecuteEx and family
;--------------------------------------
;ShellExecute() and ShellExecuteEx() error codes */

;regular WinExec() codes */
SE_ERR_FNF                               uniq  2    ;file not found
SE_ERR_PNF                               uniq  3    ;path not found
SE_ERR_ACCESSDENIED                      uniq  5    ;access denied
SE_ERR_OOM                               uniq  8    ;out of memory
SE_ERR_DLLNOTFOUND                       uniq  32

;error values for ShellExecute() beyond the regular WinExec() codes */
SE_ERR_SHARE                             uniq  26
SE_ERR_ASSOCINCOMPLETE                   uniq  27
SE_ERR_DDETIMEOUT                        uniq  28
SE_ERR_DDEFAIL                           uniq  29
SE_ERR_DDEBUSY                           uniq  30
SE_ERR_NOASSOC                           uniq  31

;Note CLASSKEY overrides CLASSNAME
SEE_MASK_DEFAULT                         uniq  0x00000000
SEE_MASK_CLASSNAME                       uniq  0x00000001;SHELLEXECUTEINFO.lpClass is valid
SEE_MASK_CLASSKEY                        uniq  0x00000003;SHELLEXECUTEINFO.hkeyClass is valid
; Note SEE_MASK_INVOKEIDLIST(0xC) implies SEE_MASK_IDLIST(0x04)
SEE_MASK_IDLIST                          uniq  0x00000004;SHELLEXECUTEINFO.lpIDList is valid
SEE_MASK_INVOKEIDLIST                    uniq  0x0000000c;enable IContextMenu based verbs
SEE_MASK_HOTKEY                          uniq  0x00000020;SHELLEXECUTEINFO.dwHotKey is valid
SEE_MASK_NOCLOSEPROCESS                  uniq  0x00000040;SHELLEXECUTEINFO.hProcess
SEE_MASK_CONNECTNETDRV                   uniq  0x00000080;enables re-connecting disconnected network drives
SEE_MASK_NOASYNC                         uniq  0x00000100;block on the call until the invoke has completed, use for callers that exit after calling ShellExecuteEx()
SEE_MASK_FLAG_DDEWAIT                    uniq  SEE_MASK_NOASYNC;Use SEE_MASK_NOASYNC instead of SEE_MASK_FLAG_DDEWAIT as it more accuratly describes the behavior
SEE_MASK_DOENVSUBST                      uniq  0x00000200;indicates that SHELLEXECUTEINFO.lpFile contains env vars that should be expanded
SEE_MASK_FLAG_NO_UI                      uniq  0x00000400;disable UI including error messages
SEE_MASK_UNICODE                         uniq  0x00004000
SEE_MASK_NO_CONSOLE                      uniq  0x00008000
SEE_MASK_ASYNCOK                         uniq  0x00100000
SEE_MASK_HMONITOR                        uniq  0x00200000;SHELLEXECUTEINFO.hMonitor
SEE_MASK_NOZONECHECKS                    uniq  0x00800000
SEE_MASK_NOQUERYCLASSSTORE               uniq  0x01000000
SEE_MASK_WAITFORINPUTIDLE                uniq  0x02000000
SEE_MASK_FLAG_LOG_USAGE                  uniq  0x04000000

;When SEE_MASK_FLAG_HINST_IS_SITE is specified SHELLEXECUTEINFO.hInstApp is used as an
;In_ parameter and specifies a IUnknown* to be used as a site pointer. The site pointer
;is used to provide services to shell execute, the handler binding process and the verb handlers
;once they are invoked.
SEE_MASK_FLAG_HINST_IS_SITE              uniq  0x08000000

;--------------------------------------
NIN_SELECT                     uniq  WM_USER + 0
NINF_KEY                       uniq  0x1
NIN_KEYSELECT                  uniq  NIN_SELECT or NINF_KEY
NIN_BALLOONSHOW                uniq  WM_USER + 2
NIN_BALLOONHIDE                uniq  WM_USER + 3
NIN_BALLOONTIMEOUT             uniq  WM_USER + 4
NIN_BALLOONUSERCLICK           uniq  WM_USER + 5
NIN_POPUPOPEN                  uniq  WM_USER + 6
NIN_POPUPCLOSE                 uniq  WM_USER + 7

NIM_ADD                        uniq  0x00000000
NIM_MODIFY                     uniq  0x00000001
NIM_DELETE                     uniq  0x00000002
NIM_SETFOCUS                   uniq  0x00000003
NIM_SETVERSION                 uniq  0x00000004

;set NOTIFYICONDATA.uVersion with 0, 3 or 4
;please read the documentation on the behavior difference that the different versions imply
NOTIFYICON_VERSION             uniq  3
NOTIFYICON_VERSION_4           uniq  4

NIF_MESSAGE                    uniq  0x00000001
NIF_ICON                       uniq  0x00000002
NIF_TIP                        uniq  0x00000004
NIF_STATE                      uniq  0x00000008
NIF_INFO                       uniq  0x00000010
NIF_GUID                       uniq  0x00000020
NIF_REALTIME                   uniq  0x00000040
NIF_SHOWTIP                    uniq  0x00000080

NIS_HIDDEN                     uniq  0x00000001
NIS_SHAREDICON                 uniq  0x00000002

;says this is the source of a shared icon
;Notify Icon Infotip flags
NIIF_NONE              uniq  0x00000000
;icon flags are mutually exclusive
;and take only the lowest 2 bits
NIIF_INFO              uniq  0x00000001
NIIF_WARNING           uniq  0x00000002
NIIF_ERROR             uniq  0x00000003
NIIF_USER              uniq  0x00000004;also available in NTDDI_WS03SP1
NIIF_ICON_MASK         uniq  0x0000000F
NIIF_NOSOUND           uniq  0x00000010
NIIF_LARGE_ICON        uniq  0x00000020
NIIF_RESPECT_QUIET_TIME        uniq  0x00000080






