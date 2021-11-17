;Binary raster ops */
;-------------------------------------
R2_BLACK                    uniq  1 ;  0       */
R2_NOTMERGEPEN              uniq  2 ; DPon     */
R2_MASKNOTPEN               uniq  3 ; DPna     */
R2_NOTCOPYPEN               uniq  4 ; PN       */
R2_MASKPENNOT               uniq  5 ; PDna     */
R2_NOT                      uniq  6 ; Dn       */
R2_XORPEN                   uniq  7 ; DPx      */
R2_NOTMASKPEN               uniq  8 ; DPan     */
R2_MASKPEN                  uniq  9 ; DPa      */
R2_NOTXORPEN                uniq  10; DPxn     */
R2_NOP                      uniq  11; D        */
R2_MERGENOTPEN              uniq  12; DPno     */
R2_COPYPEN                  uniq  13; P        */
R2_MERGEPENNOT              uniq  14; PDno     */
R2_MERGEPEN                 uniq  15; DPo      */
R2_WHITE                    uniq  16;  1       */
R2_LAST                     uniq  16

;Stock Logical Objects */
;-------------------------------------
WHITE_BRUSH                 uniq  0
LTGRAY_BRUSH                uniq  1
GRAY_BRUSH                  uniq  2
DKGRAY_BRUSH                uniq  3
BLACK_BRUSH                 uniq  4
NULL_BRUSH                  uniq  5
HOLLOW_BRUSH                uniq  NULL_BRUSH
WHITE_PEN                   uniq  6
BLACK_PEN                   uniq  7
NULL_PEN                    uniq  8
OEM_FIXED_FONT              uniq  10
ANSI_FIXED_FONT             uniq  11
ANSI_VAR_FONT               uniq  12
SYSTEM_FONT                 uniq  13
DEVICE_DEFAULT_FONT         uniq  14
DEFAULT_PALETTE             uniq  15
SYSTEM_FIXED_FONT           uniq  16
DEFAULT_GUI_FONT            uniq  17
DC_BRUSH                    uniq  18
DC_PEN                      uniq  19
STOCK_LAST                  uniq  19

;Background Modes */
;-------------------------------------
TRANSPARENT                 uniq  1
OPAQUE                      uniq  2
BKMODE_LAST                 uniq  2

;Text Alignment Options */
;--------------------------------------
TA_NOUPDATECP                       uniq  0
TA_UPDATECP                         uniq  1

TA_LEFT                             uniq  0
TA_RIGHT                            uniq  2
TA_CENTER                           uniq  6

TA_TOP                              uniq  0
TA_BOTTOM                           uniq  8
TA_BASELINE                         uniq  24
TA_RTLREADING                       uniq  256
TA_MASK                             uniq  TA_BASELINE+TA_CENTER+TA_UPDATECP+TA_RTLREADING

VTA_BASELINE                        uniq  TA_BASELINE
VTA_LEFT                            uniq  TA_BOTTOM
VTA_RIGHT                           uniq  TA_TOP
VTA_CENTER                          uniq  TA_CENTER
VTA_BOTTOM                          uniq  TA_RIGHT
VTA_TOP                             uniq  TA_LEFT

ETO_OPAQUE                          uniq  0x0002
ETO_CLIPPED                         uniq  0x0004
ETO_GLYPH_INDEX                     uniq  0x0010
ETO_RTLREADING                      uniq  0x0080
ETO_NUMERICSLOCAL                   uniq  0x0400
ETO_NUMERICSLATIN                   uniq  0x0800
ETO_IGNORELANGUAGE                  uniq  0x1000
ETO_PDY                             uniq  0x2000
ETO_REVERSE_INDEX_MAP               uniq  0x10000

ASPECT_FILTERING                    uniq  0x0001

;Ternary raster operations */
;--------------------------------------
SRCCOPY                          uniq  0x00CC0020;dest = source                   */
SRCPAINT                         uniq  0x00EE0086;dest = source OR dest           */
SRCAND                           uniq  0x008800C6;dest = source AND dest          */
SRCINVERT                        uniq  0x00660046;dest = source XOR dest          */
SRCERASE                         uniq  0x00440328;dest = source AND (NOT dest )   */
NOTSRCCOPY                       uniq  0x00330008;dest = (NOT source)             */
NOTSRCERASE                      uniq  0x001100A6;dest = (NOT src) AND (NOT dest) */
MERGECOPY                        uniq  0x00C000CA;dest = (source AND pattern)     */
MERGEPAINT                       uniq  0x00BB0226;dest = (NOT source) OR dest     */
PATCOPY                          uniq  0x00F00021;dest = pattern                  */
PATPAINT                         uniq  0x00FB0A09;dest = DPSnoo                   */
PATINVERT                        uniq  0x005A0049;dest = pattern XOR dest         */
DSTINVERT                        uniq  0x00550009;dest = (NOT dest)               */
BLACKNESS                        uniq  0x00000042;dest = BLACK                    */
WHITENESS                        uniq  0x00FF0062;dest = WHITE

;--------------------------------------
OUT_DEFAULT_PRECIS          uniq  0
OUT_STRING_PRECIS           uniq  1
OUT_CHARACTER_PRECIS        uniq  2
OUT_STROKE_PRECIS           uniq  3
OUT_TT_PRECIS               uniq  4
OUT_DEVICE_PRECIS           uniq  5
OUT_RASTER_PRECIS           uniq  6
OUT_TT_ONLY_PRECIS          uniq  7
OUT_OUTLINE_PRECIS          uniq  8
OUT_SCREEN_OUTLINE_PRECIS   uniq  9
OUT_PS_ONLY_PRECIS          uniq  10

CLIP_DEFAULT_PRECIS     uniq  0
CLIP_CHARACTER_PRECIS   uniq  1
CLIP_STROKE_PRECIS      uniq  2
CLIP_MASK               uniq  0xf
CLIP_LH_ANGLES          uniq  1 shl 4
CLIP_TT_ALWAYS          uniq  2 shl 4
CLIP_DFA_DISABLE        uniq  4 shl 4
CLIP_EMBEDDED           uniq  8 shl 4

DEFAULT_QUALITY         uniq  0
DRAFT_QUALITY           uniq  1
PROOF_QUALITY           uniq  2
NONANTIALIASED_QUALITY  uniq  3
ANTIALIASED_QUALITY     uniq  4

CLEARTYPE_QUALITY       uniq  5
CLEARTYPE_NATURAL_QUALITY    uniq  6

DEFAULT_PITCH           uniq  0
FIXED_PITCH             uniq  1
VARIABLE_PITCH          uniq  2
MONO_FONT               uniq  8

ANSI_CHARSET            uniq  0
DEFAULT_CHARSET         uniq  1
SYMBOL_CHARSET          uniq  2
SHIFTJIS_CHARSET        uniq  128
HANGEUL_CHARSET         uniq  129
HANGUL_CHARSET          uniq  129
GB2312_CHARSET          uniq  134
CHINESEBIG5_CHARSET     uniq  136
OEM_CHARSET             uniq  255
JOHAB_CHARSET           uniq  130
HEBREW_CHARSET          uniq  177
ARABIC_CHARSET          uniq  178
GREEK_CHARSET           uniq  161
TURKISH_CHARSET         uniq  162
VIETNAMESE_CHARSET      uniq  163
THAI_CHARSET            uniq  222
EASTEUROPE_CHARSET      uniq  238
RUSSIAN_CHARSET         uniq  204

MAC_CHARSET             uniq  77
BALTIC_CHARSET          uniq  186

FS_LATIN1                 uniq  0x00000001
FS_LATIN2                 uniq  0x00000002
FS_CYRILLIC               uniq  0x00000004
FS_GREEK                  uniq  0x00000008
FS_TURKISH                uniq  0x00000010
FS_HEBREW                 uniq  0x00000020
FS_ARABIC                 uniq  0x00000040
FS_BALTIC                 uniq  0x00000080
FS_VIETNAMESE             uniq  0x00000100
FS_THAI                   uniq  0x00010000
FS_JISJAPAN               uniq  0x00020000
FS_CHINESESIMP            uniq  0x00040000
FS_WANSUNG                uniq  0x00080000
FS_CHINESETRAD            uniq  0x00100000
FS_JOHAB                  uniq  0x00200000
FS_SYMBOL                 uniq  0x80000000

;Font Families */
FF_DONTCARE         uniq  0 shl 4  ; Don't care or don't know.
FF_ROMAN            uniq  1 shl 4  ; Variable stroke width, serifed. Times Roman, Century Schoolbook, etc.
FF_SWISS            uniq  2 shl 4  ; Variable stroke width, sans-serifed. Helvetica, Swiss, etc.
FF_MODERN           uniq  3 shl 4  ; Constant stroke width, serifed or sans-serifed. Pica, Elite, Courier, etc.
FF_SCRIPT           uniq  4 shl 4  ; Cursive, etc.
FF_DECORATIVE       uniq  5 shl 4  ;Old English, etc.

;Font Weights */
FW_DONTCARE         uniq  0
FW_THIN             uniq  100
FW_EXTRALIGHT       uniq  200
FW_LIGHT            uniq  300
FW_NORMAL           uniq  400
FW_MEDIUM           uniq  500
FW_SEMIBOLD         uniq  600
FW_BOLD             uniq  700
FW_EXTRABOLD        uniq  800
FW_HEAVY            uniq  900
FW_ULTRALIGHT       uniq  FW_EXTRALIGHT
FW_REGULAR          uniq  FW_NORMAL
FW_DEMIBOLD         uniq  FW_SEMIBOLD
FW_ULTRABOLD        uniq  FW_EXTRABOLD
FW_BLACK            uniq  FW_HEAVY



