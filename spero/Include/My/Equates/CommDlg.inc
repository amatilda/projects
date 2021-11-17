
;--------------------------------------
OFN_READONLY                                uniq  0x00000001
OFN_OVERWRITEPROMPT                         uniq  0x00000002
OFN_HIDEREADONLY                            uniq  0x00000004
OFN_NOCHANGEDIR                             uniq  0x00000008
OFN_SHOWHELP                                uniq  0x00000010
OFN_ENABLEHOOK                              uniq  0x00000020
OFN_ENABLETEMPLATE                          uniq  0x00000040
OFN_ENABLETEMPLATEHANDLE                    uniq  0x00000080
OFN_NOVALIDATE                              uniq  0x00000100
OFN_ALLOWMULTISELECT                        uniq  0x00000200
OFN_EXTENSIONDIFFERENT                      uniq  0x00000400
OFN_PATHMUSTEXIST                           uniq  0x00000800
OFN_FILEMUSTEXIST                           uniq  0x00001000
OFN_CREATEPROMPT                            uniq  0x00002000
OFN_SHAREAWARE                              uniq  0x00004000
OFN_NOREADONLYRETURN                        uniq  0x00008000
OFN_NOTESTFILECREATE                        uniq  0x00010000
OFN_NONETWORKBUTTON                         uniq  0x00020000
OFN_NOLONGNAMES                             uniq  0x00040000;force no long names for 4.x modules
OFN_EXPLORER                                uniq  0x00080000;new look commdlg
OFN_NODEREFERENCELINKS                      uniq  0x00100000
OFN_LONGNAMES                               uniq  0x00200000;force long names for 3.x modules
;OFN_ENABLEINCLUDENOTIFY and OFN_ENABLESIZING require
;Windows 2000 or higher to have any effect.
OFN_ENABLEINCLUDENOTIFY                     uniq  0x00400000;send include message to callback
OFN_ENABLESIZING                            uniq  0x00800000
OFN_DONTADDTORECENT                         uniq  0x02000000
OFN_FORCESHOWHIDDEN                         uniq  0x10000000;Show All files including System and hidden files

;FlagsEx Values
 OFN_EX_NOPLACESBAR                        uniq  0x00000001
;Return values for the registered message sent to the hook function
;when a sharing violation occurs.  OFN_SHAREFALLTHROUGH allows the
;filename to be accepted, OFN_SHARENOWARN rejects the name but puts
;up no warning (returned when the app has already put up a warning
;message), and OFN_SHAREWARN puts up the default warning message
;for sharing violations.
;Note:  Undefined return values map to OFN_SHAREWARN, but are
;       reserved for future use.

OFN_SHAREFALLTHROUGH                    uniq  0x2
OFN_SHARENOWARN                         uniq  0x1
OFN_SHAREWARN                           uniq  0x0