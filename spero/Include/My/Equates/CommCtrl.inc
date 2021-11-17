;//====== STATUS BAR CONTROL ===================================================
;--------------------------------------
SBARS_SIZEGRIP                     uniq  0x0100
SBARS_TOOLTIPS                     uniq  0x0800

;this is a status bar flag, preference to SBARS_TOOLTIPS
SBT_TOOLTIPS                       uniq  0x0800

SB_SETTEXTW                        uniq  WM_USER+11
SB_GETTEXTW                        uniq  WM_USER+13
SB_GETTEXTLENGTHW                  uniq  WM_USER+12

SB_SETPARTS                        uniq  WM_USER+4
SB_GETPARTS                        uniq  WM_USER+6
SB_GETBORDERS                      uniq  WM_USER+7
SB_SETMINHEIGHT                    uniq  WM_USER+8
SB_SIMPLE                          uniq  WM_USER+9
SB_GETRECT                         uniq  WM_USER+10
SB_ISSIMPLE                        uniq  WM_USER+14
SB_SETICON                         uniq  WM_USER+15
SB_SETTIPTEXTW                     uniq  WM_USER+17
SB_GETTIPTEXTW                     uniq  WM_USER+19
SB_GETICON                         uniq  WM_USER+20
SB_SETUNICODEFORMAT                uniq  CCM_SETUNICODEFORMAT
SB_GETUNICODEFORMAT                uniq  CCM_GETUNICODEFORMAT

SBT_OWNERDRAW                      uniq  0x1000
SBT_NOBORDERS                      uniq  0x0100
SBT_POPOUT                         uniq  0x0200
SBT_RTLREADING                     uniq  0x0400
SBT_NOTABPARSING                   uniq  0x0800

SB_SETBKCOLOR                      uniq  CCM_SETBKCOLOR;lParam = bkColor

;status bar notifications
SBN_SIMPLEMODECHANGE               uniq  SBN_FIRST - 0

;refers to the data saved for simple mode
SB_SIMPLEID                        uniq  0x00ff



;====================== Edit Control =============================
;--------------------------------------
EM_SETCUEBANNER             uniq  ECM_FIRST + 1; Set the cue banner with the lParm = LPCWSTR
EM_GETCUEBANNER             uniq  ECM_FIRST + 2; Set the cue banner with the lParm = LPCWSTR
EM_SHOWBALLOONTIP           uniq  ECM_FIRST + 3;Show a balloon tip associated to the edit control
EM_SETHILITE                uniq  ECM_FIRST + 5
EM_GETHILITE                uniq  ECM_FIRST + 6
EM_NOSETFOCUS               uniq  ECM_FIRST + 7
EM_TAKEFOCUS                uniq  ECM_FIRST + 8

; Generic WM_NOTIFY notification codes =================================
;--------------------------------------
NM_OUTOFMEMORY                 uniq  NM_FIRST-1
NM_CLICK                       uniq  NM_FIRST-2 ;uses NMCLICK struct
NM_DBLCLK                      uniq  NM_FIRST-3
NM_RETURN                      uniq  NM_FIRST-4
NM_RCLICK                      uniq  NM_FIRST-5 ;uses NMCLICK struct
NM_RDBLCLK                     uniq  NM_FIRST-6
NM_SETFOCUS                    uniq  NM_FIRST-7
NM_KILLFOCUS                   uniq  NM_FIRST-8
NM_CUSTOMDRAW                  uniq  NM_FIRST-12
NM_HOVER                       uniq  NM_FIRST-13
NM_NCHITTEST                   uniq  NM_FIRST-14;uses NMMOUSE struct
NM_KEYDOWN                     uniq  NM_FIRST-15;uses NMKEY struct
NM_RELEASEDCAPTURE             uniq  NM_FIRST-16
NM_SETCURSOR                   uniq  NM_FIRST-17;uses NMMOUSE struct
NM_CHAR                        uniq  NM_FIRST-18;uses NMCHAR struct
NM_TOOLTIPSCREATED             uniq  NM_FIRST-19;notify of when the tooltips window is create
NM_LDOWN                       uniq  NM_FIRST-20
NM_RDOWN                       uniq  NM_FIRST-21
NM_THEMECHANGED                uniq  NM_FIRST-22
NM_FONTCHANGED                 uniq  NM_FIRST-23
NM_CUSTOMTEXT                  uniq  NM_FIRST-24;uses NMCUSTOMTEXT struct
NM_TVSTATEIMAGECHANGING        uniq  NM_FIRST-24;uses NMTVSTATEIMAGECHANGING struct, defined after HTREEITEM

;--------------------------------------
LVM_INSERTGROUP                uniq  LVM_FIRST + 145
LVM_SETGROUPINFO               uniq  LVM_FIRST + 147
LVM_GETGROUPINFO               uniq  LVM_FIRST + 149
LVM_REMOVEGROUP                uniq  LVM_FIRST + 150
LVM_MOVEGROUP                  uniq  LVM_FIRST + 151
LVM_GETGROUPCOUNT              uniq  LVM_FIRST + 152
LVM_GETGROUPINFOBYINDEX        uniq  LVM_FIRST + 153
LVM_MOVEITEMTOGROUP            uniq  LVM_FIRST + 154
LVM_SETGROUPMETRICS            uniq  LVM_FIRST + 155
LVM_GETGROUPMETRICS            uniq  LVM_FIRST + 156
LVM_ENABLEGROUPVIEW            uniq  LVM_FIRST + 157
LVM_SORTGROUPS                 uniq  LVM_FIRST + 158


LVGGR_GROUP                     uniq  0 ;Entire expanded group
LVGGR_HEADER                    uniq  1 ;Header only (collapsed group
LVGGR_LABEL                     uniq  2 ; Label only
LVGGR_SUBSETLINK                uniq  3 ;subset link only

LVM_GETGROUPRECT                uniq  LVM_FIRST + 98

LVGMF_NONE                      uniq  0x00000000
LVGMF_BORDERSIZE                uniq  0x00000001
LVGMF_BORDERCOLOR               uniq  0x00000002
LVGMF_TEXTCOLOR                 uniq  0x00000004

;--------------------------------------
LVGF_NONE                       uniq  0x00000000
LVGF_HEADER                     uniq  0x00000001
LVGF_FOOTER                     uniq  0x00000002
LVGF_STATE                      uniq  0x00000004
LVGF_ALIGN                      uniq  0x00000008
LVGF_GROUPID                    uniq  0x00000010
LVGF_SUBTITLE                       uniq  0x00000100;pszSubtitle is valid
LVGF_TASK                           uniq  0x00000200;pszTask is valid
LVGF_DESCRIPTIONTOP                 uniq  0x00000400;pszDescriptionTop is valid
LVGF_DESCRIPTIONBOTTOM              uniq  0x00000800;pszDescriptionBottom is valid
LVGF_TITLEIMAGE                     uniq  0x00001000;iTitleImage is valid
LVGF_EXTENDEDIMAGE                  uniq  0x00002000;iExtendedImage is valid
LVGF_ITEMS                          uniq  0x00004000;iFirstItem and cItems are valid
LVGF_SUBSET                         uniq  0x00008000;pszSubsetTitle is valid
LVGF_SUBSETITEMS                    uniq  0x00010000;readonly, cItems holds count of items in visible subset, iFirstItem is valid

LVGS_NORMAL                         uniq  0x00000000
LVGS_COLLAPSED                      uniq  0x00000001
LVGS_HIDDEN                         uniq  0x00000002
LVGS_NOHEADER                       uniq  0x00000004
LVGS_COLLAPSIBLE                    uniq  0x00000008
LVGS_FOCUSED                        uniq  0x00000010
LVGS_SELECTED                       uniq  0x00000020
LVGS_SUBSETED                       uniq  0x00000040
LVGS_SUBSETLINKFOCUSED              uniq  0x00000080

LVGA_HEADER_LEFT                uniq  0x00000001
LVGA_HEADER_CENTER              uniq  0x00000002
LVGA_HEADER_RIGHT               uniq  0x00000004;Don't forget to validate exclusivity
LVGA_FOOTER_LEFT                uniq  0x00000008
LVGA_FOOTER_CENTER              uniq  0x00000010
LVGA_FOOTER_RIGHT               uniq  0x00000020;Don't forget to validate exclusivity


;--------------------------------------
CLR_NONE                        uniq  0xFFFFFFFF
CLR_DEFAULT                     uniq  0xFF000000

ILC_MASK                        uniq  0x00000001
ILC_COLOR                       uniq  0x00000000
ILC_COLORDDB                    uniq  0x000000FE
ILC_COLOR4                      uniq  0x00000004
ILC_COLOR8                      uniq  0x00000008
ILC_COLOR16                     uniq  0x00000010
ILC_COLOR24                     uniq  0x00000018
ILC_COLOR32                     uniq  0x00000020
ILC_PALETTE                     uniq  0x00000800;(not implemented)
ILC_MIRROR                      uniq  0x00002000;Mirror the icons contained, if the process is mirrored
ILC_PERITEMMIRROR               uniq  0x00008000;Causes the mirroring code to mirror each item when inserting a set of images, verses the whole strip
ILC_ORIGINALSIZE                uniq  0x00010000;Imagelist should accept smaller than set images and apply OriginalSize based on image added
ILC_HIGHQUALITYSCALE            uniq  0x00020000;Imagelist should enable use of the high quality scaler.




;BUTTON STYLES
;--------------------------------------
BS_SPLITBUTTON                 uniq  0x0000000C
BS_DEFSPLITBUTTON              uniq  0x0000000D
BS_COMMANDLINK                 uniq  0x0000000E
BS_DEFCOMMANDLINK              uniq  0x0000000F


; LISTVIEW CONTROL =====================================================


LVIR_BOUNDS                        uniq  0
LVIR_ICON                          uniq  1
LVIR_LABEL                         uniq  2
LVIR_SELECTBOUNDS                  uniq  3

LVNI_ALL                           uniq  0x0000
LVNI_FOCUSED                       uniq  0x0001
LVNI_SELECTED                      uniq  0x0002
LVNI_CUT                           uniq  0x0004
LVNI_DROPHILITED                   uniq  0x0008
LVNI_STATEMASK                     uniq  LVNI_FOCUSED or LVNI_SELECTED or LVNI_CUT or LVNI_DROPHILITED
LVNI_VISIBLEORDER                  uniq  0x0010
LVNI_PREVIOUS                      uniq  0x0020
LVNI_VISIBLEONLY                   uniq  0x0040
LVNI_SAMEGROUPONLY                 uniq  0x0080
LVNI_ABOVE                         uniq  0x0100
LVNI_BELOW                         uniq  0x0200
LVNI_TOLEFT                        uniq  0x0400
LVNI_TORIGHT                       uniq  0x0800
LVNI_DIRECTIONMASK                 uniq  LVNI_ABOVE or LVNI_BELOW or LVNI_TOLEFT or LVNI_TORIGHT


LVN_ITEMCHANGING              uniq  LVN_FIRST-0
LVN_ITEMCHANGED               uniq  LVN_FIRST-1
LVN_INSERTITEM                uniq  LVN_FIRST-2
LVN_DELETEITEM                uniq  LVN_FIRST-3
LVN_DELETEALLITEMS            uniq  LVN_FIRST-4
LVN_BEGINLABELEDITW           uniq  LVN_FIRST-75
LVN_ENDLABELEDITW             uniq  LVN_FIRST-76
LVN_COLUMNCLICK               uniq  LVN_FIRST-8
LVN_BEGINDRAG                 uniq  LVN_FIRST-9
LVN_BEGINRDRAG                uniq  LVN_FIRST-11
LVN_ODCACHEHINT               uniq  LVN_FIRST-13
LVN_ODFINDITEMW               uniq  LVN_FIRST-79
LVN_ITEMACTIVATE              uniq  LVN_FIRST-14
LVN_ODSTATECHANGED            uniq  LVN_FIRST-15
LVN_HOTTRACK                  uniq  LVN_FIRST-21
LVN_GETDISPINFOW              uniq  LVN_FIRST-77
LVN_SETDISPINFOW              uniq  LVN_FIRST-78
LVN_KEYDOWN                   uniq  LVN_FIRST-55

LVHT_NOWHERE                     uniq  0x00000001
LVHT_ONITEMICON                  uniq  0x00000002
LVHT_ONITEMLABEL                 uniq  0x00000004
LVHT_ONITEMSTATEICON             uniq  0x00000008
LVHT_ONITEM                      uniq  LVHT_ONITEMICON or LVHT_ONITEMLABEL or LVHT_ONITEMSTATEICON

LVHT_ABOVE                       uniq  0x00000008
LVHT_BELOW                       uniq  0x00000010
LVHT_TORIGHT                     uniq  0x00000020
LVHT_TOLEFT                      uniq  0x00000040


LVHT_EX_GROUP_HEADER                uniq  0x10000000
LVHT_EX_GROUP_FOOTER                uniq  0x20000000
LVHT_EX_GROUP_COLLAPSE              uniq  0x40000000
LVHT_EX_GROUP_BACKGROUND            uniq  0x80000000
LVHT_EX_GROUP_STATEICON             uniq  0x01000000
LVHT_EX_GROUP_SUBSETLINK            uniq  0x02000000
LVHT_EX_GROUP                       uniq  LVHT_EX_GROUP_BACKGROUND or LVHT_EX_GROUP_COLLAPSE or LVHT_EX_GROUP_FOOTER or LVHT_EX_GROUP_HEADER or \
                                          LVHT_EX_GROUP_STATEICON or LVHT_EX_GROUP_SUBSETLINK
LVHT_EX_ONCONTENTS                  uniq  0x04000000;On item AND not on the background
LVHT_EX_FOOTER                      uniq  0x08000000


LVS_ICON                        uniq  0x0000
LVS_REPORT                      uniq  0x0001
LVS_SMALLICON                   uniq  0x0002
LVS_LIST                        uniq  0x0003
LVS_TYPEMASK                    uniq  0x0003
LVS_SINGLESEL                   uniq  0x0004
LVS_SHOWSELALWAYS               uniq  0x0008
LVS_SORTASCENDING               uniq  0x0010
LVS_SORTDESCENDING              uniq  0x0020
LVS_SHAREIMAGELISTS             uniq  0x0040
LVS_NOLABELWRAP                 uniq  0x0080
LVS_AUTOARRANGE                 uniq  0x0100
LVS_EDITLABELS                  uniq  0x0200
LVS_OWNERDATA                   uniq  0x1000
LVS_NOSCROLL                    uniq  0x2000

LVS_TYPESTYLEMASK               uniq  0xfc00

LVS_ALIGNTOP                    uniq  0x0000
LVS_ALIGNLEFT                   uniq  0x0800
LVS_ALIGNMASK                   uniq  0x0c00

LVS_OWNERDRAWFIXED              uniq  0x0400
LVS_NOCOLUMNHEADER              uniq  0x4000
LVS_NOSORTHEADER                uniq  0x8000

LVM_SETUNICODEFORMAT            uniq  CCM_SETUNICODEFORMAT
LVM_GETUNICODEFORMAT            uniq  CCM_GETUNICODEFORMAT

LVM_GETBKCOLOR                  uniq  LVM_FIRST + 0
LVM_SETBKCOLOR                  uniq  LVM_FIRST + 1
LVM_GETIMAGELIST                uniq  LVM_FIRST + 2

LVSIL_NORMAL                    uniq  0
LVSIL_SMALL                     uniq  1
LVSIL_STATE                     uniq  2
LVSIL_GROUPHEADER               uniq  3

LVM_GETITEMRECT                 uniq  LVM_FIRST + 14
LVM_GETNEXTITEM                 uniq  LVM_FIRST + 12
LVM_SETIMAGELIST                uniq  LVM_FIRST + 3
LVM_GETITEMCOUNT                uniq  LVM_FIRST + 4

LVIF_TEXT                       uniq  0x00000001
LVIF_IMAGE                      uniq  0x00000002
LVIF_PARAM                      uniq  0x00000004
LVIF_STATE                      uniq  0x00000008
LVIF_INDENT                     uniq  0x00000010
LVIF_NORECOMPUTE                uniq  0x00000800
LVIF_GROUPID                    uniq  0x00000100
LVIF_COLUMNS                    uniq  0x00000200
LVIF_COLFMT                     uniq  0x00010000 ;The piColFmt member is valid in addition to puColumns


LVIS_FOCUSED                    uniq  0x0001
LVIS_SELECTED                   uniq  0x0002
LVIS_CUT                        uniq  0x0004
LVIS_DROPHILITED                uniq  0x0008
LVIS_GLOW                       uniq  0x0010
LVIS_ACTIVATING                 uniq  0x0020

LVIS_OVERLAYMASK                uniq  0x0F00
LVIS_STATEIMAGEMASK             uniq  0xF000

LVCF_FMT                          uniq  0x0001
LVCF_WIDTH                        uniq  0x0002
LVCF_TEXT                         uniq  0x0004
LVCF_SUBITEM                      uniq  0x0008
LVCF_IMAGE                        uniq  0x0010
LVCF_ORDER                        uniq  0x0020
LVCF_MINWIDTH                     uniq  0x0040
LVCF_DEFAULTWIDTH                 uniq  0x0080
LVCF_IDEALWIDTH                   uniq  0x0100

;LVCFMT_ flags up to FFFF are shared with the header control (HDF_ flags).
;Flags above FFFF are listview-specific.

LVCFMT_LEFT                           uniq  0x0000;Same as HDF_LEFT
LVCFMT_RIGHT                          uniq  0x0001;Same as HDF_RIGHT
LVCFMT_CENTER                         uniq  0x0002;Same as HDF_CENTER
LVCFMT_JUSTIFYMASK                    uniq  0x0003;Same as HDF_JUSTIFYMASK

LVCFMT_IMAGE                          uniq  0x0800;Same as HDF_IMAGE
LVCFMT_BITMAP_ON_RIGHT                uniq  0x1000;Same as HDF_BITMAP_ON_RIGHT
LVCFMT_COL_HAS_IMAGES                 uniq  0x8000;Same as HDF_OWNERDRAW

LVCFMT_FIXED_WIDTH                    uniq  0x00100 ;Can't resize the column; same as HDF_FIXEDWIDTH
LVCFMT_NO_DPI_SCALE                   uniq  0x40000 ;If not set, CCM_DPISCALE will govern scaling up fixed width
LVCFMT_FIXED_RATIO                    uniq  0x80000 ;Width will augment with the row height

;The following flags
LVCFMT_LINE_BREAK                    uniq  0x100000;Move to the top of the next list of columns
LVCFMT_FILL                          uniq  0x200000;Fill the remainder of the tile area. Might have a title.
LVCFMT_WRAP                          uniq  0x400000;This sub-item can be wrapped.
LVCFMT_NO_TITLE                      uniq  0x800000 ;This sub-item doesn't have an title.
LVCFMT_TILE_PLACEMENTMASK            uniq  LVCFMT_LINE_BREAK or LVCFMT_FILL
LVCFMT_SPLITBUTTON                   uniq  0x1000000;Column is a split button; same as HDF_SPLITBUTTON

LVM_GETCOLUMNW                     uniq  LVM_FIRST + 95
LVM_SETCOLUMNW                     uniq  LVM_FIRST + 96
LVM_INSERTCOLUMNW                  uniq  LVM_FIRST + 97
LVM_DELETECOLUMN                   uniq  LVM_FIRST + 28
LVM_GETCOLUMNWIDTH                 uniq  LVM_FIRST + 29

LVSCW_AUTOSIZE                     uniq   -1
LVSCW_AUTOSIZE_USEHEADER           uniq   -2


LVM_DELETEITEM                     uniq  LVM_FIRST + 8
LVM_DELETEALLITEMS                 uniq  LVM_FIRST + 9
LVM_GETCALLBACKMASK                uniq  LVM_FIRST + 10
LVM_SETCALLBACKMASK                uniq  LVM_FIRST + 11

LVM_SETCOLUMNWIDTH                 uniq  LVM_FIRST + 30
LVM_GETHEADER                      uniq  LVM_FIRST + 31
LVM_CREATEDRAGIMAGE                uniq  LVM_FIRST + 33
LVM_GETVIEWRECT                    uniq  LVM_FIRST + 34
LVM_GETTEXTCOLOR                   uniq  LVM_FIRST + 35
LVM_SETTEXTCOLOR                   uniq  LVM_FIRST + 36
LVM_GETTEXTBKCOLOR                 uniq  LVM_FIRST + 37
LVM_SETTEXTBKCOLOR                 uniq  LVM_FIRST + 38
LVM_GETTOPINDEX                    uniq  LVM_FIRST + 39
LVM_GETCOUNTPERPAGE                uniq  LVM_FIRST + 40
LVM_GETORIGIN                      uniq  LVM_FIRST + 41
LVM_UPDATE                         uniq  LVM_FIRST + 42
LVM_SETITEMSTATE                   uniq  LVM_FIRST + 43
LVM_GETITEMSTATE                   uniq  LVM_FIRST + 44
LVM_GETITEMTEXTW                   uniq  LVM_FIRST + 115
LVM_SETITEMTEXTW                   uniq  LVM_FIRST + 116
LVM_SETITEMW                       uniq  LVM_FIRST + 76

;these flags only apply to LVS_OWNERDATA listviews in report or list mode
LVSICF_NOINVALIDATEALL             uniq  0x00000001
LVSICF_NOSCROLL                    uniq  0x00000002

LVM_SETITEMCOUNT                   uniq  LVM_FIRST + 47
LVM_SORTITEMS                      uniq  LVM_FIRST + 48
LVM_SETITEMPOSITION32              uniq  LVM_FIRST + 49
LVM_GETSELECTEDCOUNT               uniq  LVM_FIRST + 50
LVM_GETITEMSPACING                 uniq  LVM_FIRST + 51
LVM_GETISEARCHSTRINGW              uniq  LVM_FIRST + 117
LVM_SETICONSPACING                 uniq  LVM_FIRST + 53
LVM_SETEXTENDEDLISTVIEWSTYLE       uniq  LVM_FIRST + 54  ;optional wParam == mask
LVM_GETEXTENDEDLISTVIEWSTYLE       uniq  LVM_FIRST + 55

LVS_EX_GRIDLINES                  uniq  0x00000001
LVS_EX_SUBITEMIMAGES              uniq  0x00000002
LVS_EX_CHECKBOXES                 uniq  0x00000004
LVS_EX_TRACKSELECT                uniq  0x00000008
LVS_EX_HEADERDRAGDROP             uniq  0x00000010
LVS_EX_FULLROWSELECT              uniq  0x00000020;applies to report mode only
LVS_EX_ONECLICKACTIVATE           uniq  0x00000040
LVS_EX_TWOCLICKACTIVATE           uniq  0x00000080
LVS_EX_FLATSB                     uniq  0x00000100
LVS_EX_REGIONAL                   uniq  0x00000200
LVS_EX_INFOTIP                    uniq  0x00000400;listview does InfoTips for you
LVS_EX_UNDERLINEHOT               uniq  0x00000800
LVS_EX_UNDERLINECOLD              uniq  0x00001000
LVS_EX_MULTIWORKAREAS             uniq  0x00002000
LVS_EX_LABELTIP                   uniq  0x00004000;listview unfolds partly hidden labels if it does not have infotip text
LVS_EX_BORDERSELECT               uniq  0x00008000;border selection style instead of highlight
LVS_EX_DOUBLEBUFFER               uniq  0x00010000
LVS_EX_HIDELABELS                 uniq  0x00020000
LVS_EX_SINGLEROW                  uniq  0x00040000
LVS_EX_SNAPTOGRID                 uniq  0x00080000 ;Icons automatically snap to grid.
LVS_EX_SIMPLESELECT               uniq  0x00100000 ;Also changes overlay rendering to top right for icon mode.
LVS_EX_JUSTIFYCOLUMNS             uniq  0x00200000 ;Icons are lined up in columns that use up the whole view area.
LVS_EX_TRANSPARENTBKGND           uniq  0x00400000 ;Background is painted by the parent via WM_PRINTCLIENT
LVS_EX_TRANSPARENTSHADOWTEXT      uniq  0x00800000 ;Enable shadow text on transparent backgrounds only (useful with bitmaps)
LVS_EX_AUTOAUTOARRANGE            uniq  0x01000000 ;Icons automatically arrange if no icon positions have been set
LVS_EX_HEADERINALLVIEWS           uniq  0x02000000 ;Display column header in all view modes
LVS_EX_AUTOCHECKSELECT            uniq  0x08000000
LVS_EX_AUTOSIZECOLUMNS            uniq  0x10000000
LVS_EX_COLUMNSNAPPOINTS           uniq  0x40000000
LVS_EX_COLUMNOVERFLOW             uniq  0x80000000

LVM_GETSUBITEMRECT                 uniq  LVM_FIRST + 56
LVM_SUBITEMHITTEST                 uniq  LVM_FIRST + 57
LVM_SETCOLUMNORDERARRAY            uniq  LVM_FIRST + 58
LVM_GETCOLUMNORDERARRAY            uniq  LVM_FIRST + 59
LVM_SETHOTITEM                     uniq  LVM_FIRST + 60
LVM_GETHOTITEM                     uniq  LVM_FIRST + 61
LVM_SETHOTCURSOR                   uniq  LVM_FIRST + 62
LVM_GETHOTCURSOR                   uniq  LVM_FIRST + 63
LVM_APPROXIMATEVIEWRECT            uniq  LVM_FIRST + 64

LV_MAX_WORKAREAS                    uniq  16
LVM_SETWORKAREAS                    uniq  LVM_FIRST + 65
LVM_GETWORKAREAS                    uniq  LVM_FIRST + 70
LVM_GETNUMBEROFWORKAREAS            uniq  LVM_FIRST + 73
LVM_GETSELECTIONMARK                uniq  LVM_FIRST + 66
LVM_SETSELECTIONMARK                uniq  LVM_FIRST + 67
LVM_SETHOVERTIME                    uniq  LVM_FIRST + 71
LVM_GETHOVERTIME                    uniq  LVM_FIRST + 72
LVM_SETTOOLTIPS                     uniq  LVM_FIRST + 74
LVM_INSERTITEMW                     uniq  LVM_FIRST + 77
LVM_GETTOOLTIPS                     uniq  LVM_FIRST + 78
LVM_SORTITEMSEX                     uniq  LVM_FIRST + 81



; WM_NOTIFY codes (NMHDR.code values) ==================================
;--------------------------------------
NM_FIRST                              uniq  0 ; generic to all controls
NM_LAST                               uniq  0- 99

LVN_FIRST                             uniq  0-100 ; listview
LVN_LAST                              uniq  0-199

;Property sheet reserved      (0U-200 -  (0U-299 - see prsht.h
HDN_FIRST                             uniq  0-300 ; header
HDN_LAST                              uniq  0-399

TVN_FIRST                             uniq  0-400 ; treeview
TVN_LAST                              uniq  0-499

TTN_FIRST                             uniq  0-520 ; tooltips
TTN_LAST                              uniq  0-549

TCN_FIRST                             uniq  0-550 ; tab control
TCN_LAST                              uniq  0-580

;Shell reserved               (0U-580 -  (0U-589
CDN_FIRST                             uniq  0-601 ; common dialog (new)
CDN_LAST                              uniq  0-699

TBN_FIRST                             uniq  0-700 ; toolbar
TBN_LAST                              uniq  0-720

UDN_FIRST                             uniq  0-721  ; updown
UDN_LAST                              uniq  0-729
DTN_FIRST                             uniq  0-740 ; datetimepick
DTN_LAST                              uniq  0-745 ; DTN_FIRST - 5

MCN_FIRST                             uniq  0-746 ; monthcal
MCN_LAST                              uniq  0-752 ; MCN_FIRST - 6

DTN_FIRST2                            uniq  0-753 ; datetimepick2
DTN_LAST2                             uniq  0-799

CBEN_FIRST                            uniq  0-800 ; combo box ex
CBEN_LAST                             uniq  0-830

RBN_FIRST                             uniq  0-831 ; rebar
RBN_LAST                              uniq  0-859

IPN_FIRST                             uniq  0-860 ; internet address
IPN_LAST                              uniq  0-879 ; internet address

SBN_FIRST                             uniq  0-880 ; status bar
SBN_LAST                              uniq  0-899

PGN_FIRST                             uniq  0-900 ; Pager Control
PGN_LAST                              uniq  0-950

WMN_FIRST                             uniq  0-1000
WMN_LAST                              uniq  0-1200

BCN_FIRST                             uniq  0-1250
BCN_LAST                              uniq  0-1350

TRBN_FIRST                            uniq  0-1501 ; trackbar
TRBN_LAST                             uniq  0-1519

MSGF_COMMCTRL_BEGINDRAG               uniq  0x4200
MSGF_COMMCTRL_SIZEHEADER              uniq  0x4201
MSGF_COMMCTRL_DRAGSELECT              uniq  0x4202
MSGF_COMMCTRL_TOOLBARCUST             uniq  0x4203


;Ranges for control message IDs =======================================
;--------------------------------------
LVM_FIRST                               uniq  0x1000 ;ListView messages
TV_FIRST                                uniq  0x1100 ;TreeView messages
HDM_FIRST                               uniq  0x1200 ;Header messages
TCM_FIRST                               uniq  0x1300 ;Tab control messages

PGM_FIRST                               uniq  0x1400 ;Pager control messages


ECM_FIRST                               uniq  0x1500 ;Edit control messages
BCM_FIRST                               uniq  0x1600 ;Button control messages
CBM_FIRST                               uniq  0x1700 ;Combobox control messages

CCM_FIRST                               uniq  0x2000 ;Common control shared messages
CCM_LAST                                uniq  CCM_FIRST + 0x200
CCM_SETBKCOLOR                          uniq  CCM_FIRST + 1;lParam is bkColor
CCM_SETCOLORSCHEME                      uniq  CCM_FIRST + 2;lParam is color scheme
CCM_GETCOLORSCHEME                      uniq  CCM_FIRST + 3;fills in COLORSCHEME pointed to by lParam
CCM_GETDROPTARGET                       uniq  CCM_FIRST + 4
CCM_SETUNICODEFORMAT                    uniq  CCM_FIRST + 5
CCM_GETUNICODEFORMAT                    uniq  CCM_FIRST + 6
CCM_SETVERSION                          uniq  CCM_FIRST + 0x7
CCM_GETVERSION                          uniq  CCM_FIRST + 0x8
CCM_SETNOTIFYWINDOW                     uniq  CCM_FIRST + 0x9;wParam == hwndParent.
CCM_SETWINDOWTHEME                      uniq  CCM_FIRST + 0xb
CCM_DPISCALE                            uniq  CCM_FIRST + 0xc ;wParam == Awareness


;TAB CONTROL
;--------------------------------------
;begin_r_commctrl
TCS_SCROLLOPPOSITE                      uniq  0x0001;assumes multiline tab
TCS_BOTTOM                              uniq  0x0002
TCS_RIGHT                               uniq  0x0002
TCS_MULTISELECT                         uniq  0x0004;allow multi-select in button mode
TCS_FLATBUTTONS                         uniq  0x0008
TCS_FORCEICONLEFT                       uniq  0x0010
TCS_FORCELABELLEFT                      uniq  0x0020
TCS_HOTTRACK                            uniq  0x0040
TCS_VERTICAL                            uniq  0x0080
TCS_TABS                                uniq  0x0000
TCS_BUTTONS                             uniq  0x0100
TCS_SINGLELINE                          uniq  0x0000
TCS_MULTILINE                           uniq  0x0200
TCS_RIGHTJUSTIFY                        uniq  0x0000
TCS_FIXEDWIDTH                          uniq  0x0400
TCS_RAGGEDRIGHT                         uniq  0x0800
TCS_FOCUSONBUTTONDOWN                   uniq  0x1000
TCS_OWNERDRAWFIXED                      uniq  0x2000
TCS_TOOLTIPS                            uniq  0x4000
TCS_FOCUSNEVER                          uniq  0x8000

;EX styles for use with TCM_SETEXTENDEDSTYLE
TCS_EX_FLATSEPARATORS                   uniq  0x00000001
TCS_EX_REGISTERDROP                     uniq  0x00000002

TCM_GETIMAGELIST                        uniq  TCM_FIRST + 2
TCM_SETIMAGELIST                        uniq  TCM_FIRST + 3
TCM_GETITEMCOUNT                        uniq  TCM_FIRST + 4

TCIF_TEXT                               uniq  0x0001
TCIF_IMAGE                              uniq  0x0002
TCIF_RTLREADING                         uniq  0x0004
TCIF_PARAM                              uniq  0x0008
TCIF_STATE                              uniq  0x0010


TCIS_BUTTONPRESSED                      uniq  0x0001
TCIS_HIGHLIGHTED                        uniq  0x0002

TCM_GETITEMW                            uniq  TCM_FIRST + 60
TCM_SETITEMW                            uniq  TCM_FIRST + 61
TCM_INSERTITEMW                         uniq  TCM_FIRST + 62
TCM_DELETEITEM                          uniq  TCM_FIRST + 8
TCM_DELETEALLITEMS                      uniq  TCM_FIRST + 9
TCM_GETITEMRECT                         uniq  TCM_FIRST + 10
TCM_GETCURSEL                           uniq  TCM_FIRST + 11
TCM_SETCURSEL                           uniq  TCM_FIRST + 12

TCHT_NOWHERE                            uniq  0x0001
TCHT_ONITEMICON                         uniq  0x0002
TCHT_ONITEMLABEL                        uniq  0x0004
TCHT_ONITEM                             uniq  TCHT_ONITEMICON or TCHT_ONITEMLABEL

TCM_HITTEST                             uniq  TCM_FIRST + 13
TCM_SETITEMEXTRA                        uniq  TCM_FIRST + 14
TCM_ADJUSTRECT                          uniq  TCM_FIRST + 40
TCM_SETITEMSIZE                         uniq  TCM_FIRST + 41
TCM_REMOVEIMAGE                         uniq  TCM_FIRST + 42
TCM_SETPADDING                          uniq  TCM_FIRST + 43
TCM_GETROWCOUNT                         uniq  TCM_FIRST + 44
TCM_GETTOOLTIPS                         uniq  TCM_FIRST + 45
TCM_SETTOOLTIPS                         uniq  TCM_FIRST + 46
TCM_GETCURFOCUS                         uniq  TCM_FIRST + 47
TCM_SETCURFOCUS                         uniq  TCM_FIRST + 48
TCM_SETMINTABWIDTH                      uniq  TCM_FIRST + 49
TCM_DESELECTALL                         uniq  TCM_FIRST + 50
TCM_HIGHLIGHTITEM                       uniq  TCM_FIRST + 51
TCM_SETEXTENDEDSTYLE                    uniq  TCM_FIRST + 52;optional wParam == mask
TCM_GETEXTENDEDSTYLE                    uniq  TCM_FIRST + 53
TCM_SETUNICODEFORMAT                    uniq  CCM_SETUNICODEFORMAT
TCM_GETUNICODEFORMAT                    uniq  CCM_GETUNICODEFORMAT
TCN_KEYDOWN                             uniq  TCN_FIRST - 0
TCN_SELCHANGE                           uniq  TCN_FIRST - 1
TCN_SELCHANGING                         uniq  TCN_FIRST - 2
TCN_GETOBJECT                           uniq  TCN_FIRST - 3
TCN_FOCUSCHANGE                         uniq  TCN_FIRST - 4


;----------------------------
TTS_ALWAYSTIP              uniq  0x01
TTS_NOPREFIX               uniq  0x02
TTS_NOANIMATE              uniq  0x10
TTS_NOFADE                 uniq  0x20
TTS_BALLOON                uniq  0x40
TTS_CLOSE                  uniq  0x80
TTS_USEVISUALSTYLE         uniq  0x100;Use themed hyperlinks

;----------------------------
ICC_LISTVIEW_CLASSES         uniq  0x00000001;listview, header
ICC_TREEVIEW_CLASSES         uniq  0x00000002;treeview, tooltips
ICC_BAR_CLASSES              uniq  0x00000004;toolbar, statusbar, trackbar, tooltips
ICC_TAB_CLASSES              uniq  0x00000008;tab, tooltips
ICC_UPDOWN_CLASS             uniq  0x00000010;updown
ICC_PROGRESS_CLASS           uniq  0x00000020;progress
ICC_HOTKEY_CLASS             uniq  0x00000040;hotkey
ICC_ANIMATE_CLASS            uniq  0x00000080;animate
ICC_WIN95_CLASSES            uniq  0x000000FF
ICC_DATE_CLASSES             uniq  0x00000100;month picker, date picker, time picker, updown
ICC_USEREX_CLASSES           uniq  0x00000200;comboex
ICC_COOL_CLASSES             uniq  0x00000400;rebar (coolbar control
ICC_INTERNET_CLASSES         uniq  0x00000800
ICC_PAGESCROLLER_CLASS       uniq  0x00001000;page scroller
ICC_NATIVEFNTCTL_CLASS       uniq  0x00002000;native font control
ICC_STANDARD_CLASSES         uniq  0x00004000
ICC_LINK_CLASS               uniq  0x00008000



