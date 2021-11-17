

;RRF - Registry Routine Flags (for RegGetValue)
RRF_RT_REG_NONE             uniq  0x00000001;restrict type to REG_NONE      (other data types will not return ERROR_SUCCESS) (automatically converts REG_EXPAND_SZ to
RRF_RT_REG_SZ               uniq  0x00000002;restrict type to REG_SZ        (other data types will not return ERROR_SUCCESS)  REG_SZ unless RRF_NOEXPAND is specified)
RRF_RT_REG_EXPAND_SZ        uniq  0x00000004;restrict type to REG_EXPAND_SZ (other data types will not return ERROR_SUCCESS) (must specify RRF_NOEXPAND or RegGetValue )
RRF_RT_REG_BINARY           uniq  0x00000008;restrict type to REG_BINARY    (other data types will not return ERROR_SUCCESS)  will fail with ERROR_INVALID_PARAMETER
RRF_RT_REG_DWORD            uniq  0x00000010;restrict type to REG_DWORD     (other data types will not return ERROR_SUCCESS)
RRF_RT_REG_MULTI_SZ         uniq  0x00000020;restrict type to REG_MULTI_SZ  (other data types will not return ERROR_SUCCESS)
RRF_RT_REG_QWORD            uniq  0x00000040;restrict type to REG_QWORD     (other data types will not return ERROR_SUCCESS)

RRF_RT_DWORD                uniq  RRF_RT_REG_BINARY or RRF_RT_REG_DWORD; restrict type to *32-bit* RRF_RT_REG_BINARY or RRF_RT_REG_DWORD (other data types will not return ERROR_SUCCESS)
RRF_RT_QWORD                uniq  RRF_RT_REG_BINARY or RRF_RT_REG_QWORD; restrict type to *64-bit* RRF_RT_REG_BINARY or RRF_RT_REG_DWORD (other data types will not return ERROR_SUCCESS)
RRF_RT_ANY                  uniq  0x0000ffff                             ; no type restriction


;Reserved Key Handles.
HKEY_CLASSES_ROOT                   uniq  0x80000000
HKEY_CURRENT_USER                   uniq  0x80000001
HKEY_LOCAL_MACHINE                  uniq  0x80000002
HKEY_USERS                          uniq  0x80000003
HKEY_PERFORMANCE_DATA               uniq  0x80000004
HKEY_PERFORMANCE_TEXT               uniq  0x80000050
HKEY_PERFORMANCE_NLSTEXT            uniq  0x80000060


HKEY_CURRENT_CONFIG                 uniq  0x80000005
HKEY_DYN_DATA                       uniq  0x80000006
HKEY_CURRENT_USER_LOCAL_SETTINGS    uniq  0x80000007