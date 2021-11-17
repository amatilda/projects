;Common dialog error return codes
;---------------------------
CDERR_DIALOGFAILURE                  uniq  0xFFFF
CDERR_GENERALCODES                   uniq  0x0000
CDERR_STRUCTSIZE                     uniq  0x0001
CDERR_INITIALIZATION                 uniq  0x0002
CDERR_NOTEMPLATE                     uniq  0x0003
CDERR_NOHINSTANCE                    uniq  0x0004
CDERR_LOADSTRFAILURE                 uniq  0x0005
CDERR_FINDRESFAILURE                 uniq  0x0006
CDERR_LOADRESFAILURE                 uniq  0x0007
CDERR_LOCKRESFAILURE                 uniq  0x0008
CDERR_MEMALLOCFAILURE                uniq  0x0009
CDERR_MEMLOCKFAILURE                 uniq  0x000A
CDERR_NOHOOK                         uniq  0x000B
CDERR_REGISTERMSGFAIL                uniq  0x000C

PDERR_PRINTERCODES                   uniq  0x1000
PDERR_SETUPFAILURE                   uniq  0x1001
PDERR_PARSEFAILURE                   uniq  0x1002
PDERR_RETDEFFAILURE                  uniq  0x1003
PDERR_LOADDRVFAILURE                 uniq  0x1004
PDERR_GETDEVMODEFAIL                 uniq  0x1005
PDERR_INITFAILURE                    uniq  0x1006
PDERR_NODEVICES                      uniq  0x1007
PDERR_NODEFAULTPRN                   uniq  0x1008
PDERR_DNDMMISMATCH                   uniq  0x1009
PDERR_CREATEICFAILURE                uniq  0x100A
PDERR_PRINTERNOTFOUND                uniq  0x100B
PDERR_DEFAULTDIFFERENT               uniq  0x100C

CFERR_CHOOSEFONTCODES                uniq  0x2000
CFERR_NOFONTS                        uniq  0x2001
CFERR_MAXLESSTHANMIN                 uniq  0x2002

FNERR_FILENAMECODES                  uniq  0x3000
FNERR_SUBCLASSFAILURE                uniq  0x3001
FNERR_INVALIDFILENAME                uniq  0x3002
FNERR_BUFFERTOOSMALL                 uniq  0x3003

FRERR_FINDREPLACECODES               uniq  0x4000
FRERR_BUFFERLENGTHZERO               uniq  0x4001

CCERR_CHOOSECOLORCODES               uniq  0x5000







ERROR_ALREADY_EXISTS         uniq    183




;Debugger printed exception on control C.
DBG_PRINTEXCEPTION_C                      uniq  0x40010006;winnt

;Debugger printed exception on control C.
DBG_PRINTEXCEPTION_WIDE_C                 uniq  0x4001000A; winnt