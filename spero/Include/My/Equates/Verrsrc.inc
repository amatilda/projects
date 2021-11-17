;Version Resource definitions


;Version resource
;----------------------------
;/* ----- Symbols ----- */
VS_FILE_INFO                         uniq RT_VERSION
VS_VERSION_INFO                      uniq 0x1
VS_USER_DEFINED                      uniq 0x64

;/* ----- VS_VERSION.dwFileFlags ----- */
VS_FFI_SIGNATURE                     uniq 0xBD04EFFE
VS_FFI_STRUCVERSION                  uniq 0x10000
VS_FFI_FILEFLAGSMASK                 uniq 0x3F

;/* ----- VS_VERSION.dwFileFlags ----- */
VS_FF_DEBUG                          uniq 0x1
VS_FF_PRERELEASE                     uniq 0x2
VS_FF_PATCHED                        uniq 0x4
VS_FF_PRIVATEBUILD                   uniq 0x8
VS_FF_INFOINFERRED                   uniq 0x10
VS_FF_SPECIALBUILD                   uniq 0x20

;/* ----- VS_VERSION.dwFileOS ----- */
VOS_UNKNOWN                          uniq 0x0
VOS_DOS                              uniq 0x10000
VOS_OS216                            uniq 0x20000
VOS_OS232                            uniq 0x30000
VOS_NT                               uniq 0x40000
VOS_WINCE                            uniq 0x50000

VOS__BASE                            uniq 0x0
VOS__WINDOWS16                       uniq 0x1
VOS__PM16                            uniq 0x2
VOS__PM32                            uniq 0x3
VOS__WINDOWS32                       uniq 0x4

VOS_DOS_WINDOWS16                    uniq 0x10001
VOS_DOS_WINDOWS32                    uniq 0x10004
VOS_OS216_PM16                       uniq 0x20002
VOS_OS232_PM32                       uniq 0x30003
VOS_NT_WINDOWS32                     uniq 0x40004

;/* ----- VS_VERSION.dwFileType ----- */
VFT_UNKNOWN                          uniq 0x0
VFT_APP                              uniq 0x1
VFT_DLL                              uniq 0x2
VFT_DRV                              uniq 0x3
VFT_FONT                             uniq 0x4
VFT_VXD                              uniq 0x5
VFT_STATIC_LIB                       uniq 0x7

;/* ----- VS_VERSION.dwFileSubtype for VFT_WINDOWS_DRV ----- */
VFT2_UNKNOWN                         uniq 0x0
VFT2_DRV_PRINTER                     uniq 0x1
VFT2_DRV_KEYBOARD                    uniq 0x2
VFT2_DRV_LANGUAGE                    uniq 0x3
VFT2_DRV_DISPLAY                     uniq 0x4
VFT2_DRV_MOUSE                       uniq 0x5
VFT2_DRV_NETWORK                     uniq 0x6
VFT2_DRV_SYSTEM                      uniq 0x7
VFT2_DRV_INSTALLABLE                 uniq 0x8
VFT2_DRV_SOUND                       uniq 0x9
VFT2_DRV_COMM                        uniq 0xA
VFT2_DRV_INPUTMETHOD                 uniq 0xB
VFT2_DRV_VERSIONED_PRINTER           uniq 0xC

;/* ----- VS_VERSION.dwFileSubtype for VFT_WINDOWS_FONT ----- */
VFT2_FONT_RASTER                     uniq 0x1
VFT2_FONT_VECTOR                     uniq 0x2
VFT2_FONT_TRUETYPE                   uniq 0x3
;----------------------------