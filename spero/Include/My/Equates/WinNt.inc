



;-------------------------------------------------------------------------
PROCESS_TERMINATE                                uniq  0x0001
PROCESS_CREATE_THREAD                            uniq  0x0002
PROCESS_SET_SESSIONID                            uniq  0x0004
PROCESS_VM_OPERATION                             uniq  0x0008
PROCESS_VM_READ                                  uniq  0x0010
PROCESS_VM_WRITE                                 uniq  0x0020
PROCESS_DUP_HANDLE                               uniq  0x0040
PROCESS_CREATE_PROCESS                           uniq  0x0080
PROCESS_SET_QUOTA                                uniq  0x0100
PROCESS_SET_INFORMATION                          uniq  0x0200
PROCESS_QUERY_INFORMATION                        uniq  0x0400
PROCESS_SUSPEND_RESUME                           uniq  0x0800
PROCESS_QUERY_LIMITED_INFORMATION                uniq  0x1000
PROCESS_SET_LIMITED_INFORMATION                  uniq  0x2000

THREAD_TERMINATE                               uniq  0x0001
THREAD_SUSPEND_RESUME                          uniq  0x0002
THREAD_GET_CONTEXT                             uniq  0x0008
THREAD_SET_CONTEXT                             uniq  0x0010
THREAD_QUERY_INFORMATION                       uniq  0x0040
THREAD_SET_INFORMATION                         uniq  0x0020
THREAD_SET_THREAD_TOKEN                        uniq  0x0080
THREAD_IMPERSONATE                             uniq  0x0100
THREAD_DIRECT_IMPERSONATION                    uniq  0x0200
THREAD_SET_LIMITED_INFORMATION                 uniq  0x0400;winnt
THREAD_QUERY_LIMITED_INFORMATION               uniq  0x0800;winnt
THREAD_RESUME                                  uniq  0x1000;winnt

JOB_OBJECT_ASSIGN_PROCESS                         uniq  0x0001
JOB_OBJECT_SET_ATTRIBUTES                         uniq  0x0002
JOB_OBJECT_QUERY                                  uniq  0x0004
JOB_OBJECT_TERMINATE                              uniq  0x0008
JOB_OBJECT_SET_SECURITY_ATTRIBUTES                uniq  0x0010
JOB_OBJECT_IMPERSONATE                            uniq  0x0020
JOB_OBJECT_ALL_ACCESS                             uniq  STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 0x3F

;-------------------------------------------------------------------------

RTL_SRWLOCK_INIT                           uniq  0x0

;-------------------------------------------------------------------------
HEAP_NO_SERIALIZE                          uniq  0x00000001
HEAP_GROWABLE                              uniq  0x00000002
HEAP_GENERATE_EXCEPTIONS                   uniq  0x00000004
HEAP_ZERO_MEMORY                           uniq  0x00000008
HEAP_REALLOC_IN_PLACE_ONLY                 uniq  0x00000010
HEAP_TAIL_CHECKING_ENABLED                 uniq  0x00000020
HEAP_FREE_CHECKING_ENABLED                 uniq  0x00000040
HEAP_DISABLE_COALESCE_ON_FREE              uniq  0x00000080
HEAP_CREATE_ALIGN_16                       uniq  0x00010000
HEAP_CREATE_ENABLE_TRACING                 uniq  0x00020000
HEAP_CREATE_ENABLE_EXECUTE                 uniq  0x00040000
HEAP_MAXIMUM_TAG                           uniq  0x0FFF
HEAP_PSEUDO_TAG_FLAG                       uniq  0x8000
HEAP_TAG_SHIFT                             uniq  18
HEAP_CREATE_SEGMENT_HEAP                   uniq  0x00000100
HEAP_CREATE_HARDENED                       uniq  0x00000200

;Predefined Value Types.
;-------------------------------------------------------------------------
REG_NONE                            uniq  0 ; No value type
REG_SZ                              uniq  1 ; Unicode nul terminated string
REG_EXPAND_SZ                       uniq  2 ; Unicode nul terminated string
                                                  ; (with environment variable references)
REG_BINARY                          uniq  3 ; Free form binary
REG_DWORD                           uniq  4 ; 32-bit number
REG_DWORD_LITTLE_ENDIAN             uniq  4 ; 32-bit number (same as REG_DWORD)
REG_DWORD_BIG_ENDIAN                uniq  5 ; 32-bit number
REG_LINK                            uniq  6 ; Symbolic Link (unicode)
REG_MULTI_SZ                        uniq  7 ; Multiple Unicode strings
REG_RESOURCE_LIST                   uniq  8 ; Resource list in the resource map
REG_FULL_RESOURCE_DESCRIPTOR        uniq  9 ; Resource list in the hardware description
REG_RESOURCE_REQUIREMENTS_LIST      uniq  10
REG_QWORD                           uniq  11 ; 64-bit number
REG_QWORD_LITTLE_ENDIAN             uniq  11 ; 64-bit number (same as REG_QWORD)

;-------------------------------------------------------------------------
CREATE_EVENT_INITIAL_SET                 uniq  0x00000002
CREATE_EVENT_MANUAL_RESET                uniq  0x00000001

EVENT_MODIFY_STATE                       uniq  0x0002
EVENT_ALL_ACCESS                         uniq  STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 0x3

;Mutant Specific Access Rights
;-------------------------------------------------------------------------
MUTANT_QUERY_STATE                       uniq  0x0001

MUTANT_ALL_ACCESS                        uniq  STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or  MUTANT_QUERY_STATE

SEMAPHORE_MODIFY_STATE                   uniq  0x0002
SEMAPHORE_ALL_ACCESS                     uniq  STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 0x3


;Timer Specific Access Rights.
;-------------------------------------------------------------------------
TIMER_QUERY_STATE                        uniq   0x0001
TIMER_MODIFY_STATE                       uniq   0x0002

TIMER_ALL_ACCESS                         uniq STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or TIMER_QUERY_STATE or TIMER_MODIFY_STATE



; These are the generic rights.
;-------------------------------------------------------------------------
GENERIC_READ                  uniq   0x80000000
GENERIC_WRITE                 uniq   0x40000000
GENERIC_EXECUTE               uniq   0x20000000
GENERIC_ALL                   uniq   0x10000000

; Define access rights to files and directories
;-------------------------------------------------------------------------
; The FILE_READ_DATA and FILE_WRITE_DATA constants are also defined in
; devioctl.h as FILE_READ_ACCESS and FILE_WRITE_ACCESS. The values for these
; constants *MUST* always be in sync.
; The values are redefined in devioctl.h because they must be available to
; both DOS and NT.
;

FILE_READ_DATA              uniq  0x0001     ; file & pipe
FILE_LIST_DIRECTORY         uniq  0x0001     ; directory

FILE_WRITE_DATA             uniq  0x0002     ; file & pipe
FILE_ADD_FILE               uniq  0x0002     ; directory

FILE_APPEND_DATA            uniq  0x0004     ; file
FILE_ADD_SUBDIRECTORY       uniq  0x0004     ; directory
FILE_CREATE_PIPE_INSTANCE   uniq  0x0004     ; named pipe


FILE_READ_EA                uniq  0x0008     ; file & directory

FILE_WRITE_EA               uniq  0x0010     ; file & directory

FILE_EXECUTE                uniq  0x0020     ; file
FILE_TRAVERSE               uniq  0x0020     ; directory

FILE_DELETE_CHILD           uniq  0x0040     ; directory

FILE_READ_ATTRIBUTES        uniq  0x0080     ; all

FILE_WRITE_ATTRIBUTES       uniq  0x0100     ; all

FILE_ALL_ACCESS uniq STANDARD_RIGHTS_REQUIRED or SYNCHRONIZE or 0x1FF

FILE_GENERIC_READ         uniq STANDARD_RIGHTS_READ or  FILE_READ_DATA or FILE_READ_ATTRIBUTES or FILE_READ_EA or SYNCHRONIZE


FILE_GENERIC_WRITE        uniq STANDARD_RIGHTS_WRITE or FILE_WRITE_DATA or FILE_WRITE_ATTRIBUTES or FILE_WRITE_EA or FILE_APPEND_DATA or SYNCHRONIZE


FILE_GENERIC_EXECUTE      uniq STANDARD_RIGHTS_EXECUTE or FILE_READ_ATTRIBUTES or FILE_EXECUTE or SYNCHRONIZE


FILE_SHARE_READ                   uniq  0x00000001
FILE_SHARE_WRITE                  uniq  0x00000002
FILE_SHARE_DELETE                 uniq  0x00000004
FILE_ATTRIBUTE_READONLY               uniq  0x00000001
FILE_ATTRIBUTE_HIDDEN                 uniq  0x00000002
FILE_ATTRIBUTE_SYSTEM                 uniq  0x00000004
FILE_ATTRIBUTE_DIRECTORY              uniq  0x00000010
FILE_ATTRIBUTE_ARCHIVE                uniq  0x00000020
FILE_ATTRIBUTE_DEVICE                 uniq  0x00000040
FILE_ATTRIBUTE_NORMAL                 uniq  0x00000080
FILE_ATTRIBUTE_TEMPORARY              uniq  0x00000100
FILE_ATTRIBUTE_SPARSE_FILE            uniq  0x00000200
FILE_ATTRIBUTE_REPARSE_POINT          uniq  0x00000400
FILE_ATTRIBUTE_COMPRESSED             uniq  0x00000800
FILE_ATTRIBUTE_OFFLINE                uniq  0x00001000
FILE_ATTRIBUTE_NOT_CONTENT_INDEXED    uniq  0x00002000
FILE_ATTRIBUTE_ENCRYPTED              uniq  0x00004000
FILE_ATTRIBUTE_INTEGRITY_STREAM       uniq  0x00008000
FILE_ATTRIBUTE_VIRTUAL                uniq  0x00010000
FILE_ATTRIBUTE_NO_SCRUB_DATA          uniq  0x00020000
FILE_ATTRIBUTE_EA                     uniq  0x00040000
FILE_ATTRIBUTE_PINNED                 uniq  0x00080000
FILE_ATTRIBUTE_UNPINNED               uniq  0x00100000
FILE_ATTRIBUTE_RECALL_ON_OPEN         uniq  0x00040000
FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS   uniq  0x00400000
FILE_NOTIFY_CHANGE_FILE_NAME      uniq  0x00000001
FILE_NOTIFY_CHANGE_DIR_NAME       uniq  0x00000002
FILE_NOTIFY_CHANGE_ATTRIBUTES     uniq  0x00000004
FILE_NOTIFY_CHANGE_SIZE           uniq  0x00000008
FILE_NOTIFY_CHANGE_LAST_WRITE     uniq  0x00000010
FILE_NOTIFY_CHANGE_LAST_ACCESS    uniq  0x00000020
FILE_NOTIFY_CHANGE_CREATION       uniq  0x00000040
FILE_NOTIFY_CHANGE_SECURITY       uniq  0x00000100
FILE_ACTION_ADDED                     uniq  0x00000001
FILE_ACTION_REMOVED                   uniq  0x00000002
FILE_ACTION_MODIFIED                  uniq  0x00000003
FILE_ACTION_RENAMED_OLD_NAME          uniq  0x00000004
FILE_ACTION_RENAMED_NEW_NAME          uniq  0x00000005
FILE_CASE_SENSITIVE_SEARCH            uniq  0x00000001
FILE_CASE_PRESERVED_NAMES             uniq  0x00000002
FILE_UNICODE_ON_DISK                  uniq  0x00000004
FILE_PERSISTENT_ACLS                  uniq  0x00000008
FILE_FILE_COMPRESSION                 uniq  0x00000010
FILE_VOLUME_QUOTAS                    uniq  0x00000020
FILE_SUPPORTS_SPARSE_FILES            uniq  0x00000040
FILE_SUPPORTS_REPARSE_POINTS          uniq  0x00000080
FILE_SUPPORTS_REMOTE_STORAGE          uniq  0x00000100
FILE_RETURNS_CLEANUP_RESULT_INFO      uniq  0x00000200

FILE_VOLUME_IS_COMPRESSED             uniq  0x00008000
FILE_SUPPORTS_OBJECT_IDS              uniq  0x00010000
FILE_SUPPORTS_ENCRYPTION              uniq  0x00020000
FILE_NAMED_STREAMS                    uniq  0x00040000
FILE_READ_ONLY_VOLUME                 uniq  0x00080000
FILE_SEQUENTIAL_WRITE_ONCE            uniq  0x00100000
FILE_SUPPORTS_TRANSACTIONS            uniq  0x00200000
FILE_SUPPORTS_HARD_LINKS              uniq  0x00400000
FILE_SUPPORTS_EXTENDED_ATTRIBUTES     uniq  0x00800000
FILE_SUPPORTS_OPEN_BY_FILE_ID         uniq  0x01000000
FILE_SUPPORTS_USN_JOURNAL             uniq  0x02000000
FILE_SUPPORTS_INTEGRITY_STREAMS       uniq  0x04000000
FILE_SUPPORTS_BLOCK_REFCOUNTING       uniq  0x08000000
FILE_SUPPORTS_SPARSE_VDL              uniq  0x10000000
FILE_DAX_VOLUME                       uniq  0x20000000
FILE_SUPPORTS_GHOSTING                uniq  0x40000000


;-------------------------------------------------------------------------
PAGE_NOACCESS                uniq 0x01
PAGE_READONLY                uniq 0x02
PAGE_READWRITE               uniq 0x04
PAGE_WRITECOPY               uniq 0x08
PAGE_EXECUTE                 uniq 0x10
PAGE_EXECUTE_READ            uniq 0x20
PAGE_EXECUTE_READWRITE       uniq 0x40
PAGE_EXECUTE_WRITECOPY       uniq 0x80
PAGE_GUARD                   uniq 0x100
PAGE_NOCACHE                 uniq 0x200
PAGE_WRITECOMBINE            uniq 0x400
PAGE_REVERT_TO_FILE_MAP      uniq 0x80000000
PAGE_ENCLAVE_THREAD_CONTROL  uniq 0x80000000
PAGE_TARGETS_NO_UPDATE       uniq 0x40000000
PAGE_TARGETS_INVALID         uniq 0x40000000
PAGE_ENCLAVE_UNVALIDATED     uniq 0x20000000
MEM_COMMIT                   uniq 0x00001000
MEM_RESERVE                  uniq 0x00002000
MEM_DECOMMIT                 uniq 0x00004000
MEM_RELEASE                  uniq 0x00008000
MEM_FREE                     uniq 0x00010000
MEM_PRIVATE                  uniq 0x00020000
MEM_MAPPED                   uniq 0x00040000
MEM_RESET                    uniq 0x00080000
MEM_TOP_DOWN                 uniq 0x00100000
MEM_WRITE_WATCH              uniq 0x00200000
MEM_PHYSICAL                 uniq 0x00400000
MEM_ROTATE                   uniq 0x00800000
MEM_DIFFERENT_IMAGE_BASE_OK  uniq 0x00800000
MEM_RESET_UNDO               uniq 0x01000000
MEM_LARGE_PAGES              uniq 0x20000000
MEM_4MB_PAGES                uniq 0x80000000
MEM_64K_PAGES                uniq MEM_LARGE_PAGES or MEM_PHYSICAL
SEC_64K_PAGES                uniq 0x00080000
SEC_FILE                     uniq 0x00800000
SEC_IMAGE                    uniq 0x01000000
SEC_PROTECTED_IMAGE          uniq 0x02000000
SEC_RESERVE                  uniq 0x04000000
SEC_COMMIT                   uniq 0x08000000
SEC_NOCACHE                  uniq 0x10000000
SEC_WRITECOMBINE             uniq 0x40000000
SEC_LARGE_PAGES              uniq 0x80000000
SEC_IMAGE_NO_EXECUTE         uniq SEC_IMAGE or SEC_NOCACHE


;Language IDs.
;----------------------------
;Note that the named locale APIs (eg GetLocaleInfoEx) are preferred.

;Not all locales have unique Language IDs

;The following two combinations of primary language ID and
;sublanguage ID have special semantics:

;     Primary Language ID   Sublanguage ID      Result
;     -------------------   ---------------     ------------------------
;     LANG_NEUTRAL          SUBLANG_NEUTRAL     Language neutral
;     LANG_NEUTRAL          SUBLANG_DEFAULT     User default language
;     LANG_NEUTRAL          SUBLANG_SYS_DEFAULT System default language
;     LANG_INVARIANT        SUBLANG_NEUTRAL     Invariant locale

;It is recommended that applications test for locale names instead of
;Language IDs / LCIDs.

;Primary language IDs.

;WARNING: These aren't always unique.  Bosnian, Serbian & Croation for example.

;It is recommended that applications test for locale names or actual LCIDs.

;Note that the LANG, SUBLANG construction is not always consistent.
;The named locale APIs (eg GetLocaleInfoEx) are recommended.

LANG_NEUTRAL                         uniq 0x00
LANG_INVARIANT                       uniq 0x7f

LANG_AFRIKAANS                       uniq 0x36
LANG_ALBANIAN                        uniq 0x1c
LANG_ALSATIAN                        uniq 0x84
LANG_AMHARIC                         uniq 0x5e
LANG_ARABIC                          uniq 0x01
LANG_ARMENIAN                        uniq 0x2b
LANG_ASSAMESE                        uniq 0x4d
LANG_AZERI                           uniq 0x2c;for Azerbaijani, LANG_AZERBAIJANI is preferred
LANG_AZERBAIJANI                     uniq 0x2c
LANG_BANGLA                          uniq 0x45
LANG_BASHKIR                         uniq 0x6d
LANG_BASQUE                          uniq 0x2d
LANG_BELARUSIAN                      uniq 0x23
LANG_BENGALI                         uniq 0x45;Some prefer to use LANG_BANGLA
LANG_BRETON                          uniq 0x7e
LANG_BOSNIAN                         uniq 0x1a; Use with SUBLANG_BOSNIAN_* Sublanguage IDs
LANG_BOSNIAN_NEUTRAL                 uniq 0x781a;Use with the ConvertDefaultLocale function
LANG_BULGARIAN                       uniq 0x02
LANG_CATALAN                         uniq 0x03
LANG_CENTRAL_KURDISH                 uniq 0x92
LANG_CHEROKEE                        uniq 0x5c
LANG_CHINESE                         uniq 0x04;Use with SUBLANG_CHINESE_* Sublanguage IDs
LANG_CHINESE_SIMPLIFIED              uniq 0x04;Use with the ConvertDefaultLocale function
LANG_CHINESE_TRADITIONAL             uniq 0x7c04;Use with the ConvertDefaultLocale function
LANG_CORSICAN                        uniq 0x83
LANG_CROATIAN                        uniq 0x1a
LANG_CZECH                           uniq 0x05
LANG_DANISH                          uniq 0x06
LANG_DARI                            uniq 0x8c
LANG_DIVEHI                          uniq 0x65
LANG_DUTCH                           uniq 0x13
LANG_ENGLISH                         uniq 0x09
LANG_ESTONIAN                        uniq 0x25
LANG_FAEROESE                        uniq 0x38
LANG_FARSI                           uniq 0x29;Deprecated: use LANG_PERSIAN instead
LANG_FILIPINO                        uniq 0x64
LANG_FINNISH                         uniq 0x0b
LANG_FRENCH                          uniq 0x0c
LANG_FRISIAN                         uniq 0x62
LANG_FULAH                           uniq 0x67
LANG_GALICIAN                        uniq 0x56
LANG_GEORGIAN                        uniq 0x37
LANG_GERMAN                          uniq 0x07
LANG_GREEK                           uniq 0x08
LANG_GREENLANDIC                     uniq 0x6f
LANG_GUJARATI                        uniq 0x47
LANG_HAUSA                           uniq 0x68
LANG_HAWAIIAN                        uniq 0x75
LANG_HEBREW                          uniq 0x0d
LANG_HINDI                           uniq 0x39
LANG_HUNGARIAN                       uniq 0x0e
LANG_ICELANDIC                       uniq 0x0f
LANG_IGBO                            uniq 0x70
LANG_INDONESIAN                      uniq 0x21
LANG_INUKTITUT                       uniq 0x5d
LANG_IRISH                           uniq 0x3c;Use with the SUBLANG_IRISH_IRELAND Sublanguage ID
LANG_ITALIAN                         uniq 0x10
LANG_JAPANESE                        uniq 0x11
LANG_KANNADA                         uniq 0x4b
LANG_KASHMIRI                        uniq 0x60
LANG_KAZAK                           uniq 0x3f
LANG_KHMER                           uniq 0x53
LANG_KICHE                           uniq 0x86
LANG_KINYARWANDA                     uniq 0x87
LANG_KONKANI                         uniq 0x57
LANG_KOREAN                          uniq 0x12
LANG_KYRGYZ                          uniq 0x40
LANG_LAO                             uniq 0x54
LANG_LATVIAN                         uniq 0x26
LANG_LITHUANIAN                      uniq 0x27
LANG_LOWER_SORBIAN                   uniq 0x2e
LANG_LUXEMBOURGISH                   uniq 0x6e
LANG_MACEDONIAN                      uniq 0x2f;the Former Yugoslav Republic of Macedonia
LANG_MALAY                           uniq 0x3e
LANG_MALAYALAM                       uniq 0x4c
LANG_MALTESE                         uniq 0x3a
LANG_MANIPURI                        uniq 0x58
LANG_MAORI                           uniq 0x81
LANG_MAPUDUNGUN                      uniq 0x7a
LANG_MARATHI                         uniq 0x4e
LANG_MOHAWK                          uniq 0x7c
LANG_MONGOLIAN                       uniq 0x50
LANG_NEPALI                          uniq 0x61
LANG_NORWEGIAN                       uniq 0x14
LANG_OCCITAN                         uniq 0x82
LANG_ODIA                            uniq 0x48
LANG_ORIYA                           uniq 0x48;Deprecated: use LANG_ODIA, instead.
LANG_PASHTO                          uniq 0x63
LANG_PERSIAN                         uniq 0x29
LANG_POLISH                          uniq 0x15
LANG_PORTUGUESE                      uniq 0x16
LANG_PULAR                           uniq 0x67;Deprecated: use LANG_FULAH instead
LANG_PUNJABI                         uniq 0x46
LANG_QUECHUA                         uniq 0x6b
LANG_ROMANIAN                        uniq 0x18
LANG_ROMANSH                         uniq 0x17
LANG_RUSSIAN                         uniq 0x19
LANG_SAKHA                           uniq 0x85
LANG_SAMI                            uniq 0x3b
LANG_SANSKRIT                        uniq 0x4f
LANG_SCOTTISH_GAELIC                 uniq 0x91
LANG_SERBIAN                         uniq 0x1a;Use with the SUBLANG_SERBIAN_* Sublanguage IDs
LANG_SERBIAN_NEUTRAL                 uniq 0x7c1a;Use with the ConvertDefaultLocale function
LANG_SINDHI                          uniq 0x59
LANG_SINHALESE                       uniq 0x5b
LANG_SLOVAK                          uniq 0x1b
LANG_SLOVENIAN                       uniq 0x24
LANG_SOTHO                           uniq 0x6c
LANG_SPANISH                         uniq 0x0a
LANG_SWAHILI                         uniq 0x41
LANG_SWEDISH                         uniq 0x1d
LANG_SYRIAC                          uniq 0x5a
LANG_TAJIK                           uniq 0x28
LANG_TAMAZIGHT                       uniq 0x5f
LANG_TAMIL                           uniq 0x49
LANG_TATAR                           uniq 0x44
LANG_TELUGU                          uniq 0x4a
LANG_THAI                            uniq 0x1e
LANG_TIBETAN                         uniq 0x51
LANG_TIGRIGNA                        uniq 0x73
LANG_TIGRINYA                        uniq 0x73;Preferred spelling in locale
LANG_TSWANA                          uniq 0x32
LANG_TURKISH                         uniq 0x1f
LANG_TURKMEN                         uniq 0x42
LANG_UIGHUR                          uniq 0x80
LANG_UKRAINIAN                       uniq 0x22
LANG_UPPER_SORBIAN                   uniq 0x2e
LANG_URDU                            uniq 0x20
LANG_UZBEK                           uniq 0x43
LANG_VALENCIAN                       uniq 0x03
LANG_VIETNAMESE                      uniq 0x2a
LANG_WELSH                           uniq 0x52
LANG_WOLOF                           uniq 0x88
LANG_XHOSA                           uniq 0x34
LANG_YAKUT                           uniq 0x85;Deprecated: use LANG_SAKHA,instead
LANG_YI                              uniq 0x78
LANG_YORUBA                          uniq 0x6a
LANG_ZULU                            uniq 0x35
;----------------------------

;Sublanguage IDs.
;----------------------------
;The name immediately following SUBLANG_ dictates which primary
;language ID that sublanguage ID can be combined with to form a
;valid language ID.

;Note that the LANG, SUBLANG construction is not always consistent.
;The named locale APIs (eg GetLocaleInfoEx) are recommended.

SUBLANG_NEUTRAL                             uniq 0x00 shl 0xA;language neutral
SUBLANG_DEFAULT                             uniq 0x01 shl 0xA;user default
SUBLANG_SYS_DEFAULT                         uniq 0x02 shl 0xA;system default
SUBLANG_CUSTOM_DEFAULT                      uniq 0x03 shl 0xA;default custom language/locale
SUBLANG_CUSTOM_UNSPECIFIED                  uniq 0x04 shl 0xA;custom language/locale
SUBLANG_UI_CUSTOM_DEFAULT                   uniq 0x05 shl 0xA;Default custom MUI language/locale

SUBLANG_AFRIKAANS_SOUTH_AFRICA              uniq 0x01 shl 0xA;Afrikaans (South Africa) 0x0436 af-ZA
SUBLANG_ALBANIAN_ALBANIA                    uniq 0x01 shl 0xA;Albanian (Albania) 0x041c sq-AL
SUBLANG_ALSATIAN_FRANCE                     uniq 0x01 shl 0xA;Alsatian (France) 0x0484
SUBLANG_AMHARIC_ETHIOPIA                    uniq 0x01 shl 0xA;Amharic (Ethiopia) 0x045e
SUBLANG_ARABIC_SAUDI_ARABIA                 uniq 0x01 shl 0xA;Arabic (Saudi Arabia)
SUBLANG_ARABIC_IRAQ                         uniq 0x02 shl 0xA;Arabic (Iraq)
SUBLANG_ARABIC_EGYPT                        uniq 0x03 shl 0xA;Arabic (Egypt)
SUBLANG_ARABIC_LIBYA                        uniq 0x04 shl 0xA;Arabic (Libya)
SUBLANG_ARABIC_ALGERIA                      uniq 0x05 shl 0xA;Arabic (Algeria)
SUBLANG_ARABIC_MOROCCO                      uniq 0x06 shl 0xA;Arabic (Morocco)
SUBLANG_ARABIC_TUNISIA                      uniq 0x07 shl 0xA;Arabic (Tunisia)
SUBLANG_ARABIC_OMAN                         uniq 0x08 shl 0xA;Arabic (Oman)
SUBLANG_ARABIC_YEMEN                        uniq 0x09 shl 0xA;Arabic (Yemen)
SUBLANG_ARABIC_SYRIA                        uniq 0x0a shl 0xA;Arabic (Syria)
SUBLANG_ARABIC_JORDAN                       uniq 0x0b shl 0xA;Arabic (Jordan)
SUBLANG_ARABIC_LEBANON                      uniq 0x0c shl 0xA;Arabic (Lebanon)
SUBLANG_ARABIC_KUWAIT                       uniq 0x0d shl 0xA;Arabic (Kuwait)
SUBLANG_ARABIC_UAE                          uniq 0x0e shl 0xA;Arabic (U.A.E)
SUBLANG_ARABIC_BAHRAIN                      uniq 0x0f shl 0xA;Arabic (Bahrain)
SUBLANG_ARABIC_QATAR                        uniq 0x10 shl 0xA;Arabic (Qatar)
SUBLANG_ARMENIAN_ARMENIA                    uniq 0x01 shl 0xA;Armenian (Armenia) 0x042b hy-AM
SUBLANG_ASSAMESE_INDIA                      uniq 0x01 shl 0xA;Assamese (India) 0x044d
SUBLANG_AZERI_LATIN                         uniq 0x01 shl 0xA;Azeri (Latin) - for Azerbaijani, SUBLANG_AZERBAIJANI_AZERBAIJAN_LATIN preferred
SUBLANG_AZERI_CYRILLIC                      uniq 0x02 shl 0xA;Azeri (Cyrillic) - for Azerbaijani, SUBLANG_AZERBAIJANI_AZERBAIJAN_CYRILLIC preferred
SUBLANG_AZERBAIJANI_AZERBAIJAN_LATIN        uniq 0x01 shl 0xA;Azerbaijani (Azerbaijan, Latin)
SUBLANG_AZERBAIJANI_AZERBAIJAN_CYRILLIC     uniq 0x02 shl 0xA;Azerbaijani (Azerbaijan, Cyrillic)
SUBLANG_BANGLA_INDIA                        uniq 0x01 shl 0xA;Bangla (India)
SUBLANG_BANGLA_BANGLADESH                   uniq 0x02 shl 0xA;Bangla (Bangladesh)
SUBLANG_BASHKIR_RUSSIA                      uniq 0x01 shl 0xA;Bashkir (Russia) 0x046d ba-RU
SUBLANG_BASQUE_BASQUE                       uniq 0x01 shl 0xA;Basque (Basque) 0x042d eu-ES
SUBLANG_BELARUSIAN_BELARUS                  uniq 0x01 shl 0xA;Belarusian (Belarus) 0x0423 be-BY
SUBLANG_BENGALI_INDIA                       uniq 0x01 shl 0xA;Bengali (India) - Note some prefer SUBLANG_BANGLA_INDIA
SUBLANG_BENGALI_BANGLADESH                  uniq 0x02 shl 0xA;Bengali (Bangladesh) - Note some prefer SUBLANG_BANGLA_BANGLADESH
SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_LATIN    uniq 0x05 shl 0xA;Bosnian (Bosnia and Herzegovina - Latin) 0x141a bs-BA-Latn
SUBLANG_BOSNIAN_BOSNIA_HERZEGOVINA_CYRILLIC uniq 0x08 shl 0xA;Bosnian (Bosnia and Herzegovina - Cyrillic) 0x201a bs-BA-Cyrl
SUBLANG_BRETON_FRANCE                       uniq 0x01 shl 0xA;Breton (France) 0x047e
SUBLANG_BULGARIAN_BULGARIA                  uniq 0x01 shl 0xA;Bulgarian (Bulgaria) 0x0402
SUBLANG_CATALAN_CATALAN                     uniq 0x01 shl 0xA;Catalan (Catalan) 0x0403
SUBLANG_CENTRAL_KURDISH_IRAQ                uniq 0x01 shl 0xA;Central Kurdish (Iraq) 0x0492 ku-Arab-IQ
SUBLANG_CHEROKEE_CHEROKEE                   uniq 0x01 shl 0xA;Cherokee (Cherokee) 0x045c chr-Cher-US
SUBLANG_CHINESE_TRADITIONAL                 uniq 0x01 shl 0xA;Chinese (Taiwan) 0x0404 zh-TW
SUBLANG_CHINESE_SIMPLIFIED                  uniq 0x02 shl 0xA;Chinese (PR China) 0x0804 zh-CN
SUBLANG_CHINESE_HONGKONG                    uniq 0x03 shl 0xA;Chinese (Hong Kong S.A.R., P.R.C.) 0x0c04 zh-HK
SUBLANG_CHINESE_SINGAPORE                   uniq 0x04 shl 0xA;Chinese (Singapore) 0x1004 zh-SG
SUBLANG_CHINESE_MACAU                       uniq 0x05 shl 0xA;Chinese (Macau S.A.R.) uniq 0x1404 zh-MO
SUBLANG_CORSICAN_FRANCE                     uniq 0x01 shl 0xA;Corsican (France) 0x0483
SUBLANG_CZECH_CZECH_REPUBLIC                uniq 0x01 shl 0xA;Czech (Czech Republic) 0x0405
SUBLANG_CROATIAN_CROATIA                    uniq 0x01 shl 0xA;Croatian (Croatia)
SUBLANG_CROATIAN_BOSNIA_HERZEGOVINA_LATIN   uniq 0x04 shl 0xA;Croatian (Bosnia and Herzegovina - Latin) uniq 0x101a hr-BA
SUBLANG_DANISH_DENMARK                      uniq 0x01 shl 0xA;Danish (Denmark) 0x0406
SUBLANG_DARI_AFGHANISTAN                    uniq 0x01 shl 0xA;Dari (Afghanistan)
SUBLANG_DIVEHI_MALDIVES                     uniq 0x01 shl 0xA;Divehi (Maldives) 0x0465 div-MV
SUBLANG_DUTCH                               uniq 0x01 shl 0xA;Dutch
SUBLANG_DUTCH_BELGIAN                       uniq 0x02 shl 0xA;Dutch (Belgian)
SUBLANG_ENGLISH_US                          uniq 0x01 shl 0xA;English (USA)
SUBLANG_ENGLISH_UK                          uniq 0x02 shl 0xA;English (UK)
SUBLANG_ENGLISH_AUS                         uniq 0x03 shl 0xA;English (Australian)
SUBLANG_ENGLISH_CAN                         uniq 0x04 shl 0xA;English (Canadian)
SUBLANG_ENGLISH_NZ                          uniq 0x05 shl 0xA;English (New Zealand)
SUBLANG_ENGLISH_EIRE                        uniq 0x06 shl 0xA;English (Irish)
SUBLANG_ENGLISH_SOUTH_AFRICA                uniq 0x07 shl 0xA;English (South Africa)
SUBLANG_ENGLISH_JAMAICA                     uniq 0x08 shl 0xA;English (Jamaica)
SUBLANG_ENGLISH_CARIBBEAN                   uniq 0x09 shl 0xA;English (Caribbean)
SUBLANG_ENGLISH_BELIZE                      uniq 0x0a shl 0xA;English (Belize)
SUBLANG_ENGLISH_TRINIDAD                    uniq 0x0b shl 0xA;English (Trinidad)
SUBLANG_ENGLISH_ZIMBABWE                    uniq 0x0c shl 0xA;English (Zimbabwe)
SUBLANG_ENGLISH_PHILIPPINES                 uniq 0x0d shl 0xA;English (Philippines)
SUBLANG_ENGLISH_INDIA                       uniq 0x10 shl 0xA;English (India)
SUBLANG_ENGLISH_MALAYSIA                    uniq 0x11 shl 0xA;English (Malaysia)
SUBLANG_ENGLISH_SINGAPORE                   uniq 0x12 shl 0xA;English (Singapore)
SUBLANG_ESTONIAN_ESTONIA                    uniq 0x01 shl 0xA;Estonian (Estonia) 0x0425 et-EE
SUBLANG_FAEROESE_FAROE_ISLANDS              uniq 0x01 shl 0xA;Faroese (Faroe Islands) 0x0438 fo-FO
SUBLANG_FILIPINO_PHILIPPINES                uniq 0x01 shl 0xA;Filipino (Philippines) 0x0464 fil-PH
SUBLANG_FINNISH_FINLAND                     uniq 0x01 shl 0xA;Finnish (Finland) 0x040b
SUBLANG_FRENCH                              uniq 0x01 shl 0xA;French
SUBLANG_FRENCH_BELGIAN                      uniq 0x02 shl 0xA;French (Belgian)
SUBLANG_FRENCH_CANADIAN                     uniq 0x03 shl 0xA;French (Canadian)
SUBLANG_FRENCH_SWISS                        uniq 0x04 shl 0xA;French (Swiss)
SUBLANG_FRENCH_LUXEMBOURG                   uniq 0x05 shl 0xA;French (Luxembourg)
SUBLANG_FRENCH_MONACO                       uniq 0x06 shl 0xA;French (Monaco)
SUBLANG_FRISIAN_NETHERLANDS                 uniq 0x01 shl 0xA;Frisian (Netherlands) 0x0462 fy-NL
SUBLANG_FULAH_SENEGAL                       uniq 0x02 shl 0xA;Fulah (Senegal) 0x0867 ff-SN
SUBLANG_GALICIAN_GALICIAN                   uniq 0x01 shl 0xA;Galician (Galician) 0x0456 gl-ES
SUBLANG_GEORGIAN_GEORGIA                    uniq 0x01 shl 0xA;Georgian (Georgia) 0x0437 ka-GE
SUBLANG_GERMAN                              uniq 0x01 shl 0xA;German
SUBLANG_GERMAN_SWISS                        uniq 0x02 shl 0xA;German (Swiss)
SUBLANG_GERMAN_AUSTRIAN                     uniq 0x03 shl 0xA;German (Austrian)
SUBLANG_GERMAN_LUXEMBOURG                   uniq 0x04 shl 0xA;German (Luxembourg)
SUBLANG_GERMAN_LIECHTENSTEIN                uniq 0x05 shl 0xA;German (Liechtenstein)
SUBLANG_GREEK_GREECE                        uniq 0x01 shl 0xA;Greek (Greece)
SUBLANG_GREENLANDIC_GREENLAND               uniq 0x01 shl 0xA;Greenlandic (Greenland) 0x046f kl-GL
SUBLANG_GUJARATI_INDIA                      uniq 0x01 shl 0xA;Gujarati (India (Gujarati Script)) 0x0447 gu-IN
SUBLANG_HAUSA_NIGERIA_LATIN                 uniq 0x01 shl 0xA;Hausa (Latin, Nigeria) 0x0468 ha-NG-Latn
SUBLANG_HAWAIIAN_US                         uniq 0x01 shl 0xA;Hawiian (US) 0x0475 haw-US
SUBLANG_HEBREW_ISRAEL                       uniq 0x01 shl 0xA;Hebrew (Israel) 0x040d
SUBLANG_HINDI_INDIA                         uniq 0x01 shl 0xA;Hindi (India) 0x0439 hi-IN
SUBLANG_HUNGARIAN_HUNGARY                   uniq 0x01 shl 0xA;Hungarian (Hungary) 0x040e
SUBLANG_ICELANDIC_ICELAND                   uniq 0x01 shl 0xA;Icelandic (Iceland) 0x040f
SUBLANG_IGBO_NIGERIA                        uniq 0x01 shl 0xA;Igbo (Nigeria) 0x0470 ig-NG
SUBLANG_INDONESIAN_INDONESIA                uniq 0x01 shl 0xA;Indonesian (Indonesia) 0x0421 id-ID
SUBLANG_INUKTITUT_CANADA                    uniq 0x01 shl 0xA;Inuktitut (Syllabics) (Canada) 0x045d iu-CA-Cans
SUBLANG_INUKTITUT_CANADA_LATIN              uniq 0x02 shl 0xA;Inuktitut (Canada - Latin)
SUBLANG_IRISH_IRELAND                       uniq 0x02 shl 0xA;Irish (Ireland)
SUBLANG_ITALIAN                             uniq 0x01 shl 0xA;Italian
SUBLANG_ITALIAN_SWISS                       uniq 0x02 shl 0xA;Italian (Swiss)
SUBLANG_JAPANESE_JAPAN                      uniq 0x01 shl 0xA;Japanese (Japan) 0x0411
SUBLANG_KANNADA_INDIA                       uniq 0x01 shl 0xA;Kannada (India (Kannada Script)) 0x044b kn-IN
SUBLANG_KASHMIRI_SASIA                      uniq 0x02 shl 0xA;Kashmiri (South Asia)
SUBLANG_KASHMIRI_INDIA                      uniq 0x02 shl 0xA;For app compatibility only
SUBLANG_KAZAK_KAZAKHSTAN                    uniq 0x01 shl 0xA;Kazakh (Kazakhstan) 0x043f kk-KZ
SUBLANG_KHMER_CAMBODIA                      uniq 0x01 shl 0xA;Khmer (Cambodia) 0x0453 kh-KH
SUBLANG_KICHE_GUATEMALA                     uniq 0x01 shl 0xA;K'iche (Guatemala)
SUBLANG_KINYARWANDA_RWANDA                  uniq 0x01 shl 0xA;Kinyarwanda (Rwanda) 0x0487 rw-RW
SUBLANG_KONKANI_INDIA                       uniq 0x01 shl 0xA;Konkani (India) 0x0457 kok-IN
SUBLANG_KOREAN                              uniq 0x01 shl 0xA;Korean (Extended Wansung)
SUBLANG_KYRGYZ_KYRGYZSTAN                   uniq 0x01 shl 0xA;Kyrgyz (Kyrgyzstan) 0x0440 ky-KG
SUBLANG_LAO_LAO                             uniq 0x01 shl 0xA;Lao (Lao PDR) 0x0454 lo-LA
SUBLANG_LATVIAN_LATVIA                      uniq 0x01 shl 0xA;Latvian (Latvia) 0x0426 lv-LV
SUBLANG_LITHUANIAN                          uniq 0x01 shl 0xA;Lithuanian
SUBLANG_LOWER_SORBIAN_GERMANY               uniq 0x02 shl 0xA;Lower Sorbian (Germany) 0x082e wee-DE
SUBLANG_LUXEMBOURGISH_LUXEMBOURG            uniq 0x01 shl 0xA;Luxembourgish (Luxembourg) 0x046e lb-LU
SUBLANG_MACEDONIAN_MACEDONIA                uniq 0x01 shl 0xA;Macedonian (Macedonia (FYROM)) 0x042f mk-MK
SUBLANG_MALAY_MALAYSIA                      uniq 0x01 shl 0xA;Malay (Malaysia)
SUBLANG_MALAY_BRUNEI_DARUSSALAM             uniq 0x02 shl 0xA;Malay (Brunei Darussalam)
SUBLANG_MALAYALAM_INDIA                     uniq 0x01 shl 0xA;Malayalam (India (Malayalam Script) ) 0x044c ml-IN
SUBLANG_MALTESE_MALTA                       uniq 0x01 shl 0xA;Maltese (Malta) 0x043a mt-MT
SUBLANG_MAORI_NEW_ZEALAND                   uniq 0x01 shl 0xA;Maori (New Zealand) 0x0481 mi-NZ
SUBLANG_MAPUDUNGUN_CHILE                    uniq 0x01 shl 0xA;Mapudungun (Chile) 0x047a arn-CL
SUBLANG_MARATHI_INDIA                       uniq 0x01 shl 0xA;Marathi (India) 0x044e mr-IN
SUBLANG_MOHAWK_MOHAWK                       uniq 0x01 shl 0xA;Mohawk (Mohawk) 0x047c moh-CA
SUBLANG_MONGOLIAN_CYRILLIC_MONGOLIA         uniq 0x01 shl 0xA;Mongolian (Cyrillic, Mongolia)
SUBLANG_MONGOLIAN_PRC                       uniq 0x02 shl 0xA;Mongolian (PRC)
SUBLANG_NEPALI_INDIA                        uniq 0x02 shl 0xA;Nepali (India)
SUBLANG_NEPALI_NEPAL                        uniq 0x01 shl 0xA;Nepali (Nepal) 0x0461 ne-NP
SUBLANG_NORWEGIAN_BOKMAL                    uniq 0x01 shl 0xA;Norwegian (Bokmal)
SUBLANG_NORWEGIAN_NYNORSK                   uniq 0x02 shl 0xA;Norwegian (Nynorsk)
SUBLANG_OCCITAN_FRANCE                      uniq 0x01 shl 0xA;Occitan (France) 0x0482 oc-FR
SUBLANG_ODIA_INDIA                          uniq 0x01 shl 0xA;Odia (India (Odia Script)) 0x0448 or-IN
SUBLANG_ORIYA_INDIA                         uniq 0x01 shl 0xA;Deprecated: use SUBLANG_ODIA_INDIA instead
SUBLANG_PASHTO_AFGHANISTAN                  uniq 0x01 shl 0xA;Pashto (Afghanistan)
SUBLANG_PERSIAN_IRAN                        uniq 0x01 shl 0xA;Persian (Iran) 0x0429 fa-IR
SUBLANG_POLISH_POLAND                       uniq 0x01 shl 0xA;Polish (Poland) 0x0415
SUBLANG_PORTUGUESE                          uniq 0x02 shl 0xA;Portuguese
SUBLANG_PORTUGUESE_BRAZILIAN                uniq 0x01 shl 0xA;Portuguese (Brazil)
SUBLANG_PULAR_SENEGAL                       uniq 0x02 shl 0xA;Deprecated: Use SUBLANG_FULAH_SENEGAL instead
SUBLANG_PUNJABI_INDIA                       uniq 0x01 shl 0xA;Punjabi (India (Gurmukhi Script)) 0x0446 pa-IN
SUBLANG_PUNJABI_PAKISTAN                    uniq 0x02 shl 0xA;Punjabi (Pakistan (Arabic Script)) 0x0846 pa-Arab-PK
SUBLANG_QUECHUA_BOLIVIA                     uniq 0x01 shl 0xA;Quechua (Bolivia)
SUBLANG_QUECHUA_ECUADOR                     uniq 0x02 shl 0xA;Quechua (Ecuador)
SUBLANG_QUECHUA_PERU                        uniq 0x03 shl 0xA;Quechua (Peru)
SUBLANG_ROMANIAN_ROMANIA                    uniq 0x01 shl 0xA;Romanian (Romania) 0x0418
SUBLANG_ROMANSH_SWITZERLAND                 uniq 0x01 shl 0xA;Romansh (Switzerland) 0x0417 rm-CH
SUBLANG_RUSSIAN_RUSSIA                      uniq 0x01 shl 0xA;Russian (Russia) 0x0419
SUBLANG_SAKHA_RUSSIA                        uniq 0x01 shl 0xA;Sakha (Russia) 0x0485 sah-RU
SUBLANG_SAMI_NORTHERN_NORWAY                uniq 0x01 shl 0xA;Northern Sami (Norway)
SUBLANG_SAMI_NORTHERN_SWEDEN                uniq 0x02 shl 0xA;Northern Sami (Sweden)
SUBLANG_SAMI_NORTHERN_FINLAND               uniq 0x03 shl 0xA;Northern Sami (Finland)
SUBLANG_SAMI_LULE_NORWAY                    uniq 0x04 shl 0xA;Lule Sami (Norway)
SUBLANG_SAMI_LULE_SWEDEN                    uniq 0x05 shl 0xA;Lule Sami (Sweden)
SUBLANG_SAMI_SOUTHERN_NORWAY                uniq 0x06 shl 0xA;Southern Sami (Norway)
SUBLANG_SAMI_SOUTHERN_SWEDEN                uniq 0x07 shl 0xA;Southern Sami (Sweden)
SUBLANG_SAMI_SKOLT_FINLAND                  uniq 0x08 shl 0xA;Skolt Sami (Finland)
SUBLANG_SAMI_INARI_FINLAND                  uniq 0x09 shl 0xA;Inari Sami (Finland)
SUBLANG_SANSKRIT_INDIA                      uniq 0x01 shl 0xA;Sanskrit (India) 0x044f sa-IN
SUBLANG_SCOTTISH_GAELIC                     uniq 0x01 shl 0xA;Scottish Gaelic (United Kingdom) 0x0491 gd-GB
SUBLANG_SERBIAN_BOSNIA_HERZEGOVINA_LATIN    uniq 0x06 shl 0xA;Serbian (Bosnia and Herzegovina - Latin)
SUBLANG_SERBIAN_BOSNIA_HERZEGOVINA_CYRILLIC uniq 0x07 shl 0xA;Serbian (Bosnia and Herzegovina - Cyrillic)
SUBLANG_SERBIAN_MONTENEGRO_LATIN            uniq 0x0b shl 0xA;Serbian (Montenegro - Latn)
SUBLANG_SERBIAN_MONTENEGRO_CYRILLIC         uniq 0x0c shl 0xA;Serbian (Montenegro - Cyrillic)
SUBLANG_SERBIAN_SERBIA_LATIN                uniq 0x09 shl 0xA;Serbian (Serbia - Latin)
SUBLANG_SERBIAN_SERBIA_CYRILLIC             uniq 0x0a shl 0xA;Serbian (Serbia - Cyrillic)
SUBLANG_SERBIAN_CROATIA                     uniq 0x01 shl 0xA;Croatian (Croatia) 0x041a hr-HR
SUBLANG_SERBIAN_LATIN                       uniq 0x02 shl 0xA;Serbian (Latin)
SUBLANG_SERBIAN_CYRILLIC                    uniq 0x03 shl 0xA;Serbian (Cyrillic)
SUBLANG_SINDHI_INDIA                        uniq 0x01 shl 0xA;Sindhi (India) reserved 0x0459
SUBLANG_SINDHI_PAKISTAN                     uniq 0x02 shl 0xA;Sindhi (Pakistan) 0x0859 sd-Arab-PK
SUBLANG_SINDHI_AFGHANISTAN                  uniq 0x02 shl 0xA;For app compatibility only
SUBLANG_SINHALESE_SRI_LANKA                 uniq 0x01 shl 0xA;Sinhalese (Sri Lanka)
SUBLANG_SOTHO_NORTHERN_SOUTH_AFRICA         uniq 0x01 shl 0xA;Northern Sotho (South Africa)
SUBLANG_SLOVAK_SLOVAKIA                     uniq 0x01 shl 0xA;Slovak (Slovakia) 0x041b sk-SK
SUBLANG_SLOVENIAN_SLOVENIA                  uniq 0x01 shl 0xA;Slovenian (Slovenia) 0x0424 sl-SI
SUBLANG_SPANISH                             uniq 0x01 shl 0xA;Spanish (Castilian)
SUBLANG_SPANISH_MEXICAN                     uniq 0x02 shl 0xA;Spanish (Mexico)
SUBLANG_SPANISH_MODERN                      uniq 0x03 shl 0xA;Spanish (Modern)
SUBLANG_SPANISH_GUATEMALA                   uniq 0x04 shl 0xA;Spanish (Guatemala)
SUBLANG_SPANISH_COSTA_RICA                  uniq 0x05 shl 0xA;Spanish (Costa Rica)
SUBLANG_SPANISH_PANAMA                      uniq 0x06 shl 0xA;Spanish (Panama)
SUBLANG_SPANISH_DOMINICAN_REPUBLIC          uniq 0x07 shl 0xA;Spanish (Dominican Republic)
SUBLANG_SPANISH_VENEZUELA                   uniq 0x08 shl 0xA;Spanish (Venezuela)
SUBLANG_SPANISH_COLOMBIA                    uniq 0x09 shl 0xA;Spanish (Colombia)
SUBLANG_SPANISH_PERU                        uniq 0x0a shl 0xA;Spanish (Peru)
SUBLANG_SPANISH_ARGENTINA                   uniq 0x0b shl 0xA;Spanish (Argentina)
SUBLANG_SPANISH_ECUADOR                     uniq 0x0c shl 0xA;Spanish (Ecuador)
SUBLANG_SPANISH_CHILE                       uniq 0x0d shl 0xA;Spanish (Chile)
SUBLANG_SPANISH_URUGUAY                     uniq 0x0e shl 0xA;Spanish (Uruguay)
SUBLANG_SPANISH_PARAGUAY                    uniq 0x0f shl 0xA;Spanish (Paraguay)
SUBLANG_SPANISH_BOLIVIA                     uniq 0x10 shl 0xA;Spanish (Bolivia)
SUBLANG_SPANISH_EL_SALVADOR                 uniq 0x11 shl 0xA;Spanish (El Salvador)
SUBLANG_SPANISH_HONDURAS                    uniq 0x12 shl 0xA;Spanish (Honduras)
SUBLANG_SPANISH_NICARAGUA                   uniq 0x13 shl 0xA;Spanish (Nicaragua)
SUBLANG_SPANISH_PUERTO_RICO                 uniq 0x14 shl 0xA;Spanish (Puerto Rico)
SUBLANG_SPANISH_US                          uniq 0x15 shl 0xA;Spanish (United States)
SUBLANG_SWAHILI_KENYA                       uniq 0x01 shl 0xA;Swahili (Kenya) 0x0441 sw-KE
SUBLANG_SWEDISH                             uniq 0x01 shl 0xA;Swedish
SUBLANG_SWEDISH_FINLAND                     uniq 0x02 shl 0xA;Swedish (Finland)
SUBLANG_SYRIAC_SYRIA                        uniq 0x01 shl 0xA;Syriac (Syria) 0x045a syr-SY
SUBLANG_TAJIK_TAJIKISTAN                    uniq 0x01 shl 0xA;Tajik (Tajikistan) 0x0428 tg-TJ-Cyrl
SUBLANG_TAMAZIGHT_ALGERIA_LATIN             uniq 0x02 shl 0xA;Tamazight (Latin, Algeria) 0x085f tzm-Latn-DZ
SUBLANG_TAMAZIGHT_MOROCCO_TIFINAGH          uniq 0x04 shl 0xA;Tamazight (Tifinagh) 0x105f tzm-Tfng-MA
SUBLANG_TAMIL_INDIA                         uniq 0x01 shl 0xA;Tamil (India)
SUBLANG_TAMIL_SRI_LANKA                     uniq 0x02 shl 0xA;Tamil (Sri Lanka) 0x0849 ta-LK
SUBLANG_TATAR_RUSSIA                        uniq 0x01 shl 0xA;Tatar (Russia) 0x0444 tt-RU
SUBLANG_TELUGU_INDIA                        uniq 0x01 shl 0xA;Telugu (India (Telugu Script)) 0x044a te-IN
SUBLANG_THAI_THAILAND                       uniq 0x01 shl 0xA;Thai (Thailand) 0x041e th-TH
SUBLANG_TIBETAN_PRC                         uniq 0x01 shl 0xA;Tibetan (PRC)
SUBLANG_TIGRIGNA_ERITREA                    uniq 0x02 shl 0xA;Tigrigna (Eritrea)
SUBLANG_TIGRINYA_ERITREA                    uniq 0x02 shl 0xA;Tigrinya (Eritrea) 0x0873 ti-ER (preferred spelling)
SUBLANG_TIGRINYA_ETHIOPIA                   uniq 0x01 shl 0xA;Tigrinya (Ethiopia) 0x0473 ti-ET
SUBLANG_TSWANA_BOTSWANA                     uniq 0x02 shl 0xA;Setswana / Tswana (Botswana) 0x0832 tn-BW
SUBLANG_TSWANA_SOUTH_AFRICA                 uniq 0x01 shl 0xA;Setswana / Tswana (South Africa) 0x0432 tn-ZA
SUBLANG_TURKISH_TURKEY                      uniq 0x01 shl 0xA;Turkish (Turkey) 0x041f tr-TR
SUBLANG_TURKMEN_TURKMENISTAN                uniq 0x01 shl 0xA;Turkmen (Turkmenistan) 0x0442 tk-TM
SUBLANG_UIGHUR_PRC                          uniq 0x01 shl 0xA;Uighur (PRC) 0x0480 ug-CN
SUBLANG_UKRAINIAN_UKRAINE                   uniq 0x01 shl 0xA;Ukrainian (Ukraine) 0x0422 uk-UA
SUBLANG_UPPER_SORBIAN_GERMANY               uniq 0x01 shl 0xA;Upper Sorbian (Germany) 0x042e wen-DE
SUBLANG_URDU_PAKISTAN                       uniq 0x01 shl 0xA;Urdu (Pakistan)
SUBLANG_URDU_INDIA                          uniq 0x02 shl 0xA;Urdu (India)
SUBLANG_UZBEK_LATIN                         uniq 0x01 shl 0xA;Uzbek (Latin)
SUBLANG_UZBEK_CYRILLIC                      uniq 0x02 shl 0xA;Uzbek (Cyrillic)
SUBLANG_VALENCIAN_VALENCIA                  uniq 0x02 shl 0xA;Valencian (Valencia) 0x0803 ca-ES-Valencia
SUBLANG_VIETNAMESE_VIETNAM                  uniq 0x01 shl 0xA;Vietnamese (Vietnam) 0x042a vi-VN
SUBLANG_WELSH_UNITED_KINGDOM                uniq 0x01 shl 0xA;Welsh (United Kingdom) 0x0452 cy-GB
SUBLANG_WOLOF_SENEGAL                       uniq 0x01 shl 0xA;Wolof (Senegal)
SUBLANG_XHOSA_SOUTH_AFRICA                  uniq 0x01 shl 0xA;isiXhosa / Xhosa (South Africa) 0x0434 xh-ZA
SUBLANG_YAKUT_RUSSIA                        uniq 0x01 shl 0xA;Deprecated: use SUBLANG_SAKHA_RUSSIA instead
SUBLANG_YI_PRC                              uniq 0x01 shl 0xA;Yi (PRC)) 0x0478
SUBLANG_YORUBA_NIGERIA                      uniq 0x01 shl 0xA;Yoruba (Nigeria) 046a yo-NG
SUBLANG_ZULU_SOUTH_AFRICA                   uniq 0x01 shl 0xA;isiZulu / Zulu (South Africa) 0x0435 zu-ZA


