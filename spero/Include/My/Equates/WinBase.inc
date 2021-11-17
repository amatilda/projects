;-------------------------------------------------------------------------

MAX_PATH                           uniq  260


;------------------------
NOPARITY                   uniq  0
ODDPARITY                  uniq  1
EVENPARITY                 uniq  2
MARKPARITY                 uniq  3
SPACEPARITY                uniq  4

ONESTOPBIT                 uniq  0
ONE5STOPBITS               uniq  1
TWOSTOPBITS                uniq  2

IGNORE                     uniq  0;Ignore signal
INFINITE                   uniq  0xFFFFFFFF; Infinite timeout

;Process dwCreationFlag values
;------------------------
DEBUG_PROCESS                      uniq 0x00000001
DEBUG_ONLY_THIS_PROCESS            uniq 0x00000002
CREATE_SUSPENDED                   uniq 0x00000004
DETACHED_PROCESS                   uniq 0x00000008

CREATE_NEW_CONSOLE                 uniq 0x00000010
NORMAL_PRIORITY_CLASS              uniq 0x00000020
IDLE_PRIORITY_CLASS                uniq 0x00000040
HIGH_PRIORITY_CLASS                uniq 0x00000080

REALTIME_PRIORITY_CLASS            uniq 0x00000100
CREATE_NEW_PROCESS_GROUP           uniq 0x00000200
CREATE_UNICODE_ENVIRONMENT         uniq 0x00000400
CREATE_SEPARATE_WOW_VDM            uniq 0x00000800

CREATE_SHARED_WOW_VDM              uniq 0x00001000
CREATE_FORCEDOS                    uniq 0x00002000
BELOW_NORMAL_PRIORITY_CLASS        uniq 0x00004000
ABOVE_NORMAL_PRIORITY_CLASS        uniq 0x00008000

INHERIT_PARENT_AFFINITY            uniq 0x00010000
INHERIT_CALLER_PRIORITY            uniq 0x00020000    ;Deprecated
CREATE_PROTECTED_PROCESS           uniq 0x00040000
EXTENDED_STARTUPINFO_PRESENT       uniq 0x00080000

PROCESS_MODE_BACKGROUND_BEGIN      uniq 0x00100000
PROCESS_MODE_BACKGROUND_END        uniq 0x00200000

CREATE_BREAKAWAY_FROM_JOB          uniq 0x01000000
CREATE_PRESERVE_CODE_AUTHZ_LEVEL   uniq 0x02000000
CREATE_DEFAULT_ERROR_MODE          uniq 0x04000000
CREATE_NO_WINDOW                   uniq 0x08000000

PROFILE_USER                       uniq 0x10000000
PROFILE_KERNEL                     uniq 0x20000000
PROFILE_SERVER                     uniq 0x40000000
CREATE_IGNORE_SYSTEM_DEFAULT       uniq 0x8000000000

;-------------------------------------------------------------------------