section '.rsrc' resource data readable
  directory RT_VERSION,version,\
            RT_GROUP_ICON,group_icons,\
            RT_BITMAP,bitmaps,\
            RT_ICON,icons,\
            RT_MANIFEST,manifest



;----------------------------------------------

  resource version,\
           Anonymous,LANG_NEUTRAL,version_data

  resource group_icons,\
           IDR_ICON_GROUP,LANG_NEUTRAL,icon_groups_butterfly,\
           IDR_ICON_GROUP_NOTCHECK,LANG_NEUTRAL,icon_groups_notcheck,\
           IDR_ICON_GROUP_CHECK,LANG_NEUTRAL,icon_groups_check ,\
           IDR_ICON_GROUP_QUES,LANG_NEUTRAL,icon_groups_ques,\
           IDR_ICON_GROUP_PAUSE,LANG_NEUTRAL,icon_groups_pause,\
           IDR_ICON_GROUP_PLAY,LANG_NEUTRAL,icon_groups_play,\
           IDR_ICON_GROUP_ADD,LANG_NEUTRAL,icon_groups_add

  resource icons,\
           Anonymous,LANG_NEUTRAL,icon_data_butterfly1,\
           Anonymous,LANG_NEUTRAL,icon_data_butterfly2,\
           Anonymous,LANG_NEUTRAL,icon_data_butterfly3,\
           Anonymous,LANG_NEUTRAL,icon_data_butterfly4,\
           Anonymous,LANG_NEUTRAL,icon_data_notcheck,\
           Anonymous,LANG_NEUTRAL,icon_data_check,\
           Anonymous,LANG_NEUTRAL,icon_data_ques,\
           Anonymous,LANG_NEUTRAL,icon_data_pause,\
           Anonymous,LANG_NEUTRAL,icon_data_play,\
           Anonymous,LANG_NEUTRAL,icon_data_add

  resource manifest,\
           Anonymous,LANG_NEUTRAL,manifest_data

  resource bitmaps,\
           IDR_BITMAP_ABOUT,LANG_NEUTRAL,assign_about

;----------------------------------------------
  versioninfo version_data ,VOS_NT_WINDOWS32,VFT_APP,VFT2_UNKNOWN,LANG_ENGLISH,1200,\
              'OriginalFilename',<equNameApp,'.exe'>,\
              'ProductName',equNameApp,\
              'FileDescription',equNameApp,\
              'LegalCopyright',<equCopyright>,\
              'FileVersion',equVersionApp,\
              'ProductVersion',equVersionApp


 icon icon_groups_add,\
 icon_data_add,'Inc\Ico\add32x32x32.ico'

 icon icon_groups_play,\
 icon_data_play,'Inc\Ico\play32x32x32.ico'

 icon icon_groups_pause,\
 icon_data_pause,'Inc\Ico\pause32x32x32.ico'

 icon icon_groups_notcheck,\
 icon_data_notcheck,'Inc\Ico\notcheck32x32x32.ico'

 icon icon_groups_check,\
 icon_data_check,'Inc\Ico\check32x32x32.ico'

 icon icon_groups_ques,\
 icon_data_ques,'Inc\Ico\quest32x32x32.ico'

 icon icon_groups_butterfly,\
 icon_data_butterfly1,'Inc\Ico\256x256x32.ico',\
 icon_data_butterfly2,'Inc\Ico\48x48x32.ico',\
 icon_data_butterfly3,'Inc\Ico\32x32x32.ico',\
 icon_data_butterfly4,'Inc\Ico\16x16x32.ico'

 bitmap assign_about,'Inc\Ico\64x64x32.bmp'

  resdata manifest_data
db   '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>',13,10
db   '<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">',13,10
db      "<dependency>",13,10
db        "<dependentAssembly>",13,10
db         "<assemblyIdentity type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*' />",13,10
db        "</dependentAssembly>",13,10
db      "</dependency>",13,10

db   '<compatibility xmlns="urn:schemas-microsoft-com:compatibility.v1">',13,10
  db   '<application>',13,10
       ; <!--This Id value indicates the application supports Windows Vista functionality -->
         db   '<supportedOS Id="{e2011457-1546-43c5-a5fe-008deee3d3f0}"/>',13,10
        ;<!--This Id value indicates the application supports Windows 7 functionality-->
         db   '<supportedOS Id="{35138b9a-5d96-4fbd-8e2d-a2440225f93a}"/>',13,10
        ;<!--This Id value indicates the application supports Windows 8 functionality-->
         db   '<supportedOS Id="{4a2f28e3-53b9-4441-ba9c-d69d4a4a6e38}"/>',13,10
        ;<!--This Id value indicates the application supports Windows 8.1 functionality-->
         db   '<supportedOS Id="{1f676c76-80e1-4239-95bb-83d0f6d0da78}"/>',13,10
        ;<!--This Id value indicates the application supports Windows 10 functionality-->
         db   '<supportedOS Id="{8e0f7a12-bfb3-4fe8-b9a5-48fd50a15a9a}"/>',13,10
  db   '</application>',13,10
db   '</compatibility>',13,10
db   '</assembly>',13,10
  endres


;Файл пустышка для защиты
;----------------------------------------------
align 16
aGuardianDll:
file 'inc\Guardian.dll'
sizeof.aGuardianDll  uniq $ - aGuardianDll

;----------------------------------------------
PolinomCRC32 8,0x82F63B78


aFolderidRoamingAppData                 GUID                 FOLDERID_RoamingAppData; гуид для поиска Roaming
aInitControlEx                          INITCOMMONCONTROLSEX sizeof.INITCOMMONCONTROLSEX,ICC_STANDARD_CLASSES or ICC_TAB_CLASSES or \
                                                             ICC_LISTVIEW_CLASSES;структура для инитилизатии комон контрол 6.0

aInitilizDefault                        INITILIZDATA ;начальные настройки по дефолту если нет ini файла


 dialogIndirect aDialogEmpty,'',0,0,0,0,0
 enddialogIndirect

iconstCRC32cGuardian        ddA  constCRC32cGuardian;Хранит CRC32 всего кода

arrayDU tbRoaming,\
        sHLCatalog,<'\HL'>,\; наша общая папка в Roaming для всех будущих программ
        sNameDirectory,<'\',equNameApp>,\; папка в Roaming нашей программы
        sMainIniFile,<'\',equNameApp,'.ksi'>; Будем хранить в этом файле все параметры программы


szStatic                 duA  'Static',0
szToolTipClass           duA  'tooltips_class32',0
szButtonClass            duA  'BUTTON',0
szComboBoxClass          duA  'ComboBox',0
szSysTabClass            duA  'SysTabControl32',0
szListViewClass          duA  'SysListView32',0
szEventOneApp            duA  'Local\AE0C4DB4-4DC1-4B2B-8A02-7CA524F6166E',0;Чтобы не запустилось больше одного приложения в одной сесии и имя скласса окна
szShell_TrayWnd          duA  'Shell_TrayWnd',0
szMsgTaskCreat           duA  'TaskbarCreated',0
szFileLicense            duA  'License.txt',0
szInfoRegUrl             duA  'https://hlapp.net',0
szNameGroup              duA  'Global',0
szEmptyItem              duA  '....',0
szSubKeyRun              duA  'Software\Microsoft\Windows\CurrentVersion\Run',0
szKeyRun                 duA  equNameApp,0
szKeyFile                duA  equNameApp,'.kss',0
szInfoEmail              duA   equMailSupport,0
szInfoEmailTo            duA  'mailto:',equMailSupport,'?subject=',equNameApp,' ',equVersionApp,0;Что бы ShellExecuteExW могла вызвать почтовый клиент по умолчанию

szLockKey                duA  'Lock',0;VK_LOCK_KEY
szScreensaver            duA  'Screensaver',0;VK_SCRSAV
szOffUniv                duA  'Turn off - universal',0

sNameMainWin             duA  equNameApp,' ',equVersionApp,' - '
sCopyright               duA  equCopyright


langtableDU     tbLanguageTable,\;должен сахраняться тот же порядак в каком устанавливаються в ComboBox итемы и тогда id совпадают - COMBOBOX_ENGLISH = выдаваемый ComboBox
                COMBOBOX_BELARUSSIAN,'Belarusian','Lng\Belarusian.lng',\
                COMBOBOX_ENGLISH,'English','E',\ ;Lng\English.lng
                COMBOBOX_RUSSIAN,'Russian','Lng\Russian.lng'


arrayChangeTab  tbChangeTab,\
                SYSTAB_CONTROL_KEYCHANGE_Id,ControlVisibleKeyChange,\
                SYSTAB_CONTROL_ABOUT_Id,ControlVisibleAbout



tbMODIFITER        MODIFITER

arrayKey tbKey,\
         VK_LBUTTON,'Left mouse',\
         VK_RBUTTON,'Right mouse',\
         VK_CANCEL,'Break',\
         VK_MBUTTON,'Middle mouse',\
         VK_XBUTTON1,'X1 mouse',\
         VK_XBUTTON2,'X2 mouse',\
         VK_BACK,'BackSpace',\
         VK_TAB,'Tab',\
         VK_CLEAR,'Clear',\
         VK_RETURN,'Enter',\
         VK_SHIFT,'Shift',\
         VK_CONTROL,'Ctrl',\
         VK_MENU,'Alt',\
         VK_PAUSE,'Pause',\
         VK_CAPITAL,'Caps Lock',\
         VK_KANA,'IME Kana',\
         VK_HANGUL,'IME Hangul',\
         VK_JUNJA,'IME Junja',\
         VK_FINAL,'IME final',\
         VK_HANJA,'IME Hanja',\
         VK_KANJI,'IME Kanji',\
         VK_ESCAPE,'Esc',\
         VK_CONVERT,'IME convert',\
         VK_NONCONVERT,'IME nonconvert',\
         VK_ACCEPT,'IME accept',\
         VK_MODECHANGE,'IME mode change request',\
         VK_SPACE,'Space',\
         VK_PRIOR,'PageUp',\
         VK_NEXT,'PageDown',\
         VK_END,'End',\
         VK_HOME,'Home',\
         VK_LEFT,'Left Arrow',\
         VK_UP,'Up Arrow',\
         VK_RIGHT,'Right Arrow',\
         VK_DOWN,'Down Arrow',\
         VK_SELECT,'Select',\
         VK_PRINT,'Print',\
         VK_EXECUTE,'Execute',\
         VK_SNAPSHOT,'PrintScreen',\
         VK_INSERT,'Ins',\
         VK_DELETE,'Del',\
         VK_HELP,'Help',\
         0x30,'0',\
         0x31,'1',\
         0x32,'2',\
         0x33,'3',\
         0x34,'4',\
         0x35,'5',\
         0x36,'6',\
         0x37,'7',\
         0x38,'8',\
         0x39,'9',\
         0x41,'A',\
         0x42,'B',\
         0x43,'C',\
         0x44,'D',\
         0x45,'E',\
         0x46,'F',\
         0x47,'G',\
         0x48,'H',\
         0x49,'I',\
         0x4A,'J',\
         0x4B,'K',\
         0x4C,'L',\
         0x4D,'M',\
         0x4E,'N',\
         0x4F,'O',\
         0x50,'P',\
         0x51,'Q',\
         0x52,'R',\
         0x53,'S',\
         0x54,'T',\
         0x55,'U',\
         0x56,'V',\
         0x57,'W',\
         0x58,'X',\
         0x59,'Y',\
         0x5A,'Z',\
         VK_LWIN,'Left Win',\
         VK_RWIN,'Right Win',\
         VK_APPS,'Menu',\
         VK_SLEEP,'Computer Sleep',\
         VK_NUMPAD0,'Num 0',\
         VK_NUMPAD1,'Num 1',\
         VK_NUMPAD2,'Num 2',\
         VK_NUMPAD3,'Num 3',\
         VK_NUMPAD4,'Num 4',\
         VK_NUMPAD5,'Num 5',\
         VK_NUMPAD6,'Num 6',\
         VK_NUMPAD7,'Num 7',\
         VK_NUMPAD8,'Num 8',\
         VK_NUMPAD9,'Num 9',\
         VK_MULTIPLY,'Num *',\
         VK_ADD,'Num +',\
         VK_SEPARATOR,'Num Enter',\
         VK_SUBTRACT,'Num -',\
         VK_DECIMAL,'Num .',\
         VK_DIVIDE,'Num /',\
         VK_F1,'F1',\
         VK_F2,'F2',\
         VK_F3,'F3',\
         VK_F4,'F4',\
         VK_F5,'F5',\
         VK_F6,'F6',\
         VK_F7,'F7',\
         VK_F8,'F8',\
         VK_F9,'F9',\
         VK_F10,'F10',\
         VK_F11,'F11',\
         VK_F12,'F12',\
         VK_F13,'F13',\
         VK_F14,'F14',\
         VK_F15,'F15',\
         VK_F16,'F16',\
         VK_F17,'F17',\
         VK_F18,'F18',\
         VK_F19,'F19',\
         VK_F20,'F20',\
         VK_F21,'F21',\
         VK_F22,'F22',\
         VK_F23,'F23',\
         VK_F24,'F24',\
         VK_NAVIGATION_VIEW,'Nav View',\
         VK_NAVIGATION_MENU,'Nav Menu',\
         VK_NAVIGATION_UP,'Nav Up',\
         VK_NAVIGATION_DOWN,'Nav Down',\
         VK_NAVIGATION_LEFT,'Nav Left',\
         VK_NAVIGATION_RIGHT,'Nav Right',\
         VK_NAVIGATION_ACCEPT,'Nav Accept',\
         VK_NAVIGATION_CANCEL,'Nav Cancel',\
         VK_NUMLOCK,'Num Lock',\
         VK_SCROLL,'Scroll Lock',\
         VK_OEM_NEC_EQUAL,'Num =',\
         VK_OEM_FJ_JISHO,'Dictionary',\
         VK_OEM_FJ_MASSHOU,'Unregister word',\
         VK_OEM_FJ_TOUROKU,'Register word',\
         VK_OEM_FJ_LOYA,'Left OYAYUBI',\
         VK_OEM_FJ_ROYA,'Right OYAYUBI',\
         VK_LSHIFT,'Left Shift',\
         VK_RSHIFT,'Right Shift',\
         VK_LCONTROL,'Left Ctrl',\
         VK_RCONTROL,'Right Ctrl',\
         VK_LMENU,'Left Alt',\
         VK_RMENU,'Right Alt',\
         VK_BROWSER_BACK,'Browser Back',\
         VK_BROWSER_FORWARD,'Browser Forward',\
         VK_BROWSER_REFRESH,'Browser Refresh',\
         VK_BROWSER_STOP,'Browser Stop',\
         VK_BROWSER_SEARCH,'Browser Search',\
         VK_BROWSER_FAVORITES,'Browser Favorites',\
         VK_BROWSER_HOME,'Browser Start and Home',\
         VK_VOLUME_MUTE,'Volume Mute',\
         VK_VOLUME_DOWN,'Volume Down',\
         VK_VOLUME_UP,'Volume Up',\
         VK_MEDIA_NEXT_TRACK,'Next Track',\
         VK_MEDIA_PREV_TRACK,'Previous Track',\
         VK_MEDIA_STOP,'Stop Media',\
         VK_MEDIA_PLAY_PAUSE,'Play/Pause Media',\
         VK_LAUNCH_MAIL,'Start Mail',\
         VK_LAUNCH_MEDIA_SELECT,'Select Media',\
         VK_LAUNCH_APP1,'Start Application 1',\
         VK_LAUNCH_APP2,'Start Application 2',\
         VK_OEM_1,';:',\
         VK_OEM_PLUS,'+',\
         VK_OEM_COMMA,',',\
         VK_OEM_MINUS,'-',\
         VK_OEM_PERIOD,'.',\
         VK_OEM_2,'/?',\
         VK_OEM_3,'`~',\
         VK_OEM_4,'[{',\
         VK_OEM_5,'\|',\
         VK_OEM_6,']}',\
         VK_OEM_7,<0x22,0x27>,\
         VK_OEM_AX,'AX',\
         VK_OEM_102,'<> or \|',\
         VK_ICO_HELP,'Help',\
         VK_ICO_00,'00',\
         VK_PROCESS,'IME Process',\
         VK_ICO_CLEAR,'Clear',\
         VK_ATTN,'Attn',\
         VK_CRSEL,'CrSel',\
         VK_EXSEL,'ExSel',\
         VK_EREOF,'Erase Eof',\
         VK_PLAY,'Play',\
         VK_ZOOM,'Zoom',\
         VK_PA1,'PA1',\
         VK_OEM_CLEAR,'Clear',\
         VK_MOUSEWHEEL_UP,'Mouse Wheel Up',\
         VK_MOUSEWHEEL_DOWN,'Mouse Wheel Down',\
         VK_MOUSEWHEEL_RIGHT,'Mouse Wheel Right',\
         VK_MOUSEWHEEL_LEFT,'Mouse Wheel Left'

iconstCRC32cGuardianEmpty   ddA  constCRC32cGuardianEmpty;Хранит CRC32 приманки

include  '..\Lng\English.asm'