macro  enumMenu num,[id,vkCode]
{ forward
   id uniq num + vkCode}

 ;Перечесление id;Ноль и минус один не используеться не акселератарах, не в меню и не в дочерних окнах поэтому и начинаем с +1
;----------------------------------------------
 enum  IDCANCEL+1,\;Начальное значение IDCANCEL - последнее по умолчанию определеная в диалогах(или не самое последнее...???)
       BUTTON_EXIT_Id,\
       BUTTON_OPTIONS_Id,\
       BUTTON_ACTIVSUSP_Id,\
       COMBOBOX_LANG_Id,\
       SYSTAB_CONTROL_Id,\
       LIST_VIEW_Id,\
       STATIC_HELP_Id,\
       DIALOG_NOTLICENSE_ONE,\
       DIALOG_NOTLICENSE_TWO,\
       DIALOG_NOTLICENSE_THREE,\
       DIALOG_NOTLICENSE_FOUR,\
       DIALOG_NOTLICENSE_REGINFO,\
       DIALOG_NOTLICENSE_LICENSE,\
       DIALOG_NOTLICENSE_COPYRIGHT,\
       DIALOG_NOTLICENSE_RANDOM,\
       DIALOG_KEYMAP_EDIT_SET_Id,\
       DIALOG_KEYMAP_EDIT_TO_Id,\
       DIALOG_KEYMAP_BUTTON_ADD_Id,\
       DIALOG_KEYMAP_STATIC_HELP_Id,\
       DIALOG_OPTIONS_BUTTON_AVTO_START_Id


 ;Начинаем с +1 что бы не пересечься случайно c придыдущим MENU_HELP_LAST_Id
 ;Для обработки сообщения WM_MENUSELECT и заадно id для меню
;----------------------------------------------
 enum  MENU_HELP_LAST_Id+1,\;MENU_HELP_LAST_Id - первая таблица - Последний id меню с подсказками не включая его самого - кантралирует что бы не пригнуть дальше чем надо чем надо
       MENU_SHOW_HIDE_Id,\
       MENU_ACTIVATE_Id,\
       MENU_EXIT_Id,\
       MENU_TABLE_ADD_Id;В этот список выше этого нельзя добавлять только ниже

 ;Перечесление специально для ручной добавки клавиш основано на сложении MENU_TABLE_KEY_Id + vkCode тем самым при нажатии этих пунктов меню vkCode легко получить отняв MENU_TABLE_KEY_Id
 enumMenu MENU_TABLE_KEY_Id,\;Последняя таблица для меню  - если добавлять то перед ней
          MENU_ADD_2LVL_MOUSE_1Id,VK_LBUTTON,\
          MENU_ADD_2LVL_MOUSE_2Id,VK_RBUTTON,\
          MENU_ADD_2LVL_MOUSE_3Id,VK_MBUTTON,\
          MENU_ADD_2LVL_MOUSE_4Id,VK_XBUTTON1,\
          MENU_ADD_2LVL_MOUSE_5Id,VK_XBUTTON2,\
          MENU_ADD_2LVL_MOUSE_6Id,VK_MOUSEWHEEL_UP,\
          MENU_ADD_2LVL_MOUSE_7Id,VK_MOUSEWHEEL_DOWN,\
          MENU_ADD_2LVL_MOUSE_8Id,VK_MOUSEWHEEL_RIGHT,\
          MENU_ADD_2LVL_MOUSE_9Id,VK_MOUSEWHEEL_LEFT,\
          MENU_LAST_ID,VK_LAST;Последний id меню созданый "вручную"  остальные будут уже создаваться если надо во время работы программы


;----------------------------------------------
 enum  0xFF+1,\;Начальное значение - все стандартные коды для клавиш vk вмесчаються в байт поэтому начально значение 0xFF+1, что бы мои коды с ими не пересеклись
       VK_MOUSEWHEEL_UP,\;(должно
       VK_MOUSEWHEEL_DOWN,\;быть
       VK_MOUSEWHEEL_RIGHT,\;вместе
       VK_MOUSEWHEEL_LEFT,\;не разделять)
       VK_LAST,\;это для кодов клавиш последний и идут уже коды действий тем самым контролируем где клавиши  а где действия
       VK_FILE_OPEN,\;Если=vkCodeSubOne,тогда vkCodeSubTwoThree = указатель на структуру с даными и строкой,scanCodeSubOne = размер в байтах занимаемыми другими даными до строки (0 байт),\
       VK_LOCK_KEY,\;scanCodeSubTwoThree = размер всей структуры(0 байт + длина строки в байтах)
       VK_SCRSAV,\;\----\
       VK_OFFUNIV,\;\----\
       VK_TESTCRC32;Для проверки целостности программы


;----------------------------------------------
 enum  WM_APP+1,\;Начальное значение
       WM_APP_NOTIFY_ICON,\
       WM_APP_LICENSE,\
       WM_APP_ACTION,\;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = lp Action Date - освободить потом надо память
       WM_APP_HOOK_Play,\
       WM_APP_HOOK_Stop,\
       WM_APP_HOOK_ONE_KEY,\;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW
       WM_APP_HOOK_ONE_KEY_RELEASED,\;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW
       WM_APP_HOOK_COMB_ONE_KEY_RELEASED,\;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = [ITEMLISTVIEW.scanCodeSubOne]
       WM_APP_HOOK_COMBINATION_KEY;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW


;MENU_NOTIFU_Id
;MENU_ADDITEM_Id

;SYSTAB_CONTROL_KEYCHANGE_Id
;SYSTAB_CONTROL_ABOUT_Id

;LIST_VIEW_COLUM_ORIGINAL_Id
;LIST_VIEW_COLUM_NEW_Id

;IMAGE_LIST_STATE_NOTCHECK_Id
;IMAGE_LIST_STATE_CHECK_Id
;IMAGE_LIST_STATE_QUES_Id



NOTIFY_ICON_Id                 uniq  WM_APP + 1


LIST_VIEW_GROUP_GLOBAL_Id      uniq  0

;----------------------------------------------
equCopyright                   equ  'Copyright ',0A9h,' 2018 Pavel Kulaga. All rights reserved.'
equNameApp                     equ  'Spero';От этого то же зависит серийный номер, но это уже актуально в случае другого приложения
equVersionKsmRules             equ  'v1';Когда меняеть значит меняються струтуры сохраненых клавиш
equVersionKeyRules             equ  '1.0';Когда меняеться это значит что будет генерироваться серийник с новыми входными данными и старые серийники к новыми версиям приложения не подойдут
equVersionLang                 equ  '1.0';Когда строки языкавые меняються нужно менять это значение
equVersionApp                  equ  '1.0'
equMailSupport                 equ  'support@hlapp.net'

constTimeGuardianMin           uniq  60000*30;30 min ;Время на какое поставлен защитный таймер;60000 = 1 min

constNumberN                   uniq  0xB3A9E983;для RSA; также как число для хукав позволяющее определить была ли кнопка симулирована мной
constNumberE                   uniq  257;для RSA

constLengthKeyPressedMap       uniq  (VK_LAST+1+3)/4  ;+1 на нулевой индекс;+3 что бы при делении на 4 не потирать байты;количество dword в буфере   aKeyPressedMap

;----------------------------------------------
BUTTON_EXIT_Width              uniq  102
BUTTON_EXIT_Height             uniq  31

BUTTON_OPTIONS_Width           uniq  102
BUTTON_OPTIONS_Height          uniq  31

STATIC_HELP_Height             uniq  31

BUTTON_ACTIVSUSP_Width         uniq  102
BUTTON_ACTIVSUSP_Height        uniq  31

COMBOBOX_LANG_Width            uniq  100
COMBOBOX_LANG_Height           uniq  30

SYSTAB_CONTROL_Height          uniq  COMBOBOX_LANG_Height + 10

ABOUT_BMP_Width_Height         uniq  64;Высота и ширина bmp для ABOUT в пикселях
ABOUT_BORDER                   uniq  10;Отступ для рамки
ABOUT_INDENT                   uniq  10;Отступ для кнопки

LIST_VIEW_STATE_Width_Height   uniq  32
LIST_VIEW_STATE_MIN_MAX        uniq  3

;----------------------------------------------
struct FILEALLSTRUCT
 iCRC32c                    dd  ?;Нельзя перемещать
 iVersionRules              dd  ?
 icountGroup                dd  ?
 icountItem                 dd  ?
;тут идут сначала сохраненые структуры групп, а потом итемав
;причем каждая структура идет сразу после следующей
;....................

ends

struct FLAG
 Activate                 dd  ?;Указывает активно главное окно или нет, если равно нулю значит не активно
ends

struct MENU_STATE
 Item                    dd  ?;Определяет состояние в котором находяться пункты меню относящегося к итемам и надо ли их делать не рабочими, если = 0 пункты меню активированы
 Group                   dd  ?;Определяет состояние в котором находяться пункты меню относящегося к группам и надо ли их делать не рабочими, если = 0 пункты меню активированы
 NotifySnowHide          dd  ?;Контролирует меню для shellnotify - установлен текст Snow или Hide;Если равно ноль тогда Snow
 ActivSusp               dd  ?;Контролирует итеме меню MENU_ACTIVATE_Id

 iForStaticMenu          dd  ?;Указывает получали сообщения WM_MENUSELECT от меню требуещего выводить подсказки
 hmenuAddAction          dd  ?;меню для добавки доп действий временное при закритие диалога удаляеться
 hmenuSubAddAction0Lvl   dd  ?;Суб меню так как TrackPopupMenuEx не работает с меню
 hmenuSubAddManual1Lvl   dd  ?;Суб меню первого уровня чтто бы знать когда давать подсказки для суб меню
 hmenuSubAddMonitor1Lvl  dd  ?;Суб меню первого уровня чтто бы знать когда давать подсказки для суб меню
 SubAddActionModif_1     dd  ?;Для контрлоя какие мадификаторы будут использованы при добавлении физически отсутствуюзих клавиш
 SubAddActionModif_2     dd  ?;Для контрлоя какие мадификаторы будут использованы при добавлении физически отсутствуюзих клавиш
 hwndAddAddKeysStatic    dd  ?;Временое сохранение хендла статика от диалога DlgProcAddKeys
ends

struct  SYS_STOCK
 rgbBTNFACE               dd  ?;Цвет текста главнога окна
 rgbBTNTEXT               dd  ?;Цвет текста главнога окна
 rgbHOTLIGHT              dd  ?;Цвет гиперссылак
 rgb3DLIGHT               dd  ?;Цвет рамки еле видной в диалоге

 Pen                      dd  ?;Стандартный pen  позволяющий менять свой цвет
 Brush                    dd  ?;Стандартный Brush  позволяющий менять свой цвет

 hfont                     dd  ?;Основной шрифт обычный не освобождаеться
ends

struct  LP_ABOUT_ARRAY_ZONA
 Point                 dd  ?;Указатель на текущий ABOUT_ARRAY_ZONA отвечающий за рамки вокруг текста на вкладке About
 UpDown                dd  ?;Указатель на текущий ABOUT_ARRAY_ZONA отвечающий за выглидит прямоугольник отжатым или нажатым вкладке About
ends

struct  TAB_CURRENT;Указатели на функцию перемещения отвечающую за текущую вкладку
 Move             dd  ?
 Paint            dd  ?
 MouseMove        dd  ?
 MouseLeftUpDown  dd  ?
 Activate         dd  ?
ends

struct  ABOUT_FREE
 hbmpAbout                  dd  ?
 hfont30SEMIBOLD            dd  ?;Временный шрифт для вкладки About - освобождаеться
ends


struct KEYFILEW
 iCRC32File                   dd  ?;Здесь храниться СRC32с файла с серийникам поэтому смещать нельзя и изменять
 iDigitalSignatureLow         dd  ?;Нижняя часть цифровой подпеси
 iCRC32ForKey                 dd  ?;что будем сверать с цифровой подпесью
 iDigitalSignatureHiw         dd  ?;Верхняя часть цифровой подпеси
 lengthszNameUser             dd  ?;длину в символах без нуля фамилии и имяни пользователя
 szNameUser                   du  50 dup(?);Буфер хранит данные полученые от пользователя - Имя и фамилию
ends



struct  GROUPLISTVIEW
 idGroup                    dd  ?; = id группы к каторому привязываються итемы относящиеся к этой группе
 lengthNameGroup            dd  ?;Длина имени группы в символах включая ноль
 lpszNameGroup              dd  ?; Указатель на имя группы
 lpPrevGroup                dd  ?
 lpNextGroup                dd  ?
ends


;Соотношение между vkCodeItemOne и vkCodeSubOne относительно друг друга не должно меняться так как это используеться для подставления в функцию MakeModifeter
;Тоесть никто между ними недолжен вклиневаться
struct  ITEMLISTVIEW
 stateItem                  dd  ?;= состояние включена эта структура для замен или нет, так же вид отображения какой
 idGroup                    dd  ?; = id группы какой этот итем ссответствует
 lpPrevItem                 dd  ?;ссылка на структуру следующего итема
 lpNextItem                 dd  ?;ссылка на структуру предыдущего итема
 vkCodeItemOne              dd  ?; = 0 - отвечает за начальный итем, если не ноль тогда = virtual-key code
 vkCodeItemTwoThree         dd  ?;если не ноль тогда = virtual-key code двух клавиш модификаторов в младшей и страшей половинке,а vkCodeItemOne = virtual-key последней клавиши в комбинации
 scanCodeItemOne            dd  ?
 scanCodeItemTwoThree       dd  ?
 vkCodeSubOne               dd  ?;аналогично как в двух vkCodeItemOne и vkCodeItemTwoThree, но если vkCodeSubOne > VK_LAST тогда в остальных "Sub" этой структуры,\
 vkCodeSubTwoThree          dd  ?;храняться другие даные зависящие от значения в  vkCodeSubOne
 scanCodeSubOne             dd  ?; если vkCodeSubOne = virtual-key code, тогда = сканкод соответсвующий virtual-key code в младчей части, в старшей  если 1 тогда расширеная клавиша
 scanCodeSubTwoThree        dd  ?; если vkCodeSubOne = virtual-key code, тогда = сканкод соответсвующий virtual-key code в младчей части, в старшей  если 1 тогда расширеная клавиша
ends

struct  HOOK_MINOR
 iKeyModif                  dd  ?;Для того что бы правильно определялись сочитания с мультимедиа клавишами - они не блокирую клавиши модификаторы тем самым не правильное идет определение
 iKeyModif2                 dd  ?;Для того что бы правильно определялись сочитания с мультимедиа клавишами - они не блокирую клавиши модификаторы тем самым не правильное идет определение
ends

struct  HOOK
union
 iCountKeyPressDD           dd  ?
 struct
  iCountKeyPress            db  ?;Контролируем количество клавиш удерживаемых на клавиатуре и мыше
  iCountKeyPressModif       db  ?;Контролируем количество клавиш модификаторов удерживаемых на клавиатуре
 ends
ends

aCombi                      ITEMLISTVIEW;для сохранения нажатай комбинации

 aKeyPressedMap             dd  constLengthKeyPressedMap  dup(?);здесь отмечаются клавиши которые удерживаються, если = 1 значит удерживаеться доступ по индексу vk,
 aKeyPressedMapColizion     dd  constLengthKeyPressedMap  dup(?);здесь отмечаются клавиши которые удерживаються, если = 1 значит удерживаеться доступ по индексу vk,
ends



struct  KEYMAP
 hwnd                       dd  ?;хендл окна Edit к которому ссответствует даная структура
 vkCodeOne                  dd  ?; = 0 - отвечает за начальный итем, если не ноль тогда = virtual-key code
 vkCodeTwoThree             dd  ?;если не ноль тогда = virtual-key code двух клавиш модификаторов в младшей и страшей половинке,а vkCodeItemOne = virtual-key последней клавиши в комбинации
 scanCodeSubOne             dd  ?; если vkCodeSubOne = virtual-key code, тогда = сканкод соответсвующий virtual-key code в младчей части, в старшей  если 1 тогда расширеная клавиша
 scanCodeSubTwoThree        dd  ?; если vkCodeSubOne = virtual-key code, тогда = сканкод соответсвующий virtual-key code в младчей части, в старшей  если 1 тогда расширеная клавиша
ends


struct HWNDFONT;структура для всех детских окон  которым меняеться шрифт
 hwndButtonExit             dd  ?
 hwndButtonOptions          dd  ?
 hwndComboBoxLang           dd  ?
 hwndSysTab                 dd  ?
 hwndListView               dd  ?
 hwndStatic                 dd  ?
ends


struct NOTIFYICONDATAW_1;Данная версия структуры для Windows  раньше XP -так как все вазможности не будут использоваться и что бы не замарачиваться....
 cbSize                            dd  ?
 hWnd                              LpType  ?
 uID                               dd  ?
 uFlags                            dd  ?
 uCallbackMessage                  dd  ?
 hIcon                             LpType  ?
 szTip                             du  64 dup(?)

ends


struct MODIFITER
 db  VK_SHIFT
 db  VK_CONTROL
 db  VK_MENU
 db  VK_LSHIFT
 db  VK_RSHIFT
 db  VK_LCONTROL
 db  VK_RCONTROL
 db  VK_LMENU
 db  VK_RMENU
ends


  struct  VectoredExceptionHandler
    MessageEng               du  'Version: ',equVersionApp,13,10
                             du  'Save the screenshot and send it with a description of the actions that led to the error on the:',equMailSupport,13,10,13,10

                             du  'Exception Address: 0x'
    ExceptionAddress         du  8 dup(?),13,10
                             du  'Exception Record: 0x'
    ExceptionRecord          du  8 dup(?),13,10
                             du  'Exception Flags: 0x'
    ExceptionFlags           du  8 dup(?),13,10
                             du  'Exception Code: 0x'
    ExceptionCode            du  8 dup(?),13,10
                             du  'Base of module: 0x'
    BaseOfModule             du  8 dup('0'),13,10
                             du  'Base of app: 0x'
    BaseOfApp                du  8 dup(?),13,10,13,10

                             du  'Eax=0x'
    MessageEax               du  8 dup(?),' Ecx=0x'
    MessageEcx               du  8 dup(?),' Edx=0x'
    MessageEdx               du  8 dup(?),13,10,'Ebx=0x'
    MessageEbx               du  8 dup(?),' Esi=0x'
    MessageEsi               du  8 dup(?),' Edi=0x'
    MessageEdi               du  8 dup(?),13,10,'Ebp=0x'
    MessageEbp               du  8 dup(?),' Esp=0x'
    MessageEsp               du  8 dup(?),13,10,13,10

  ends



struct !LARGE_INTEGER
   LowPart                      dd ?
   HighPart                     dd ?
ends


macro arraybVCPCode  name,[id,str]
{ common
   local max,loc
   max  = 0

  forward
   if  id > max
    max = id
   end if

  common
   align 2
   name:
   dw (max+1)*2 dup(?)

  forward
   loc = $
   du str
   store word loc - name at name + id*2*2
   store word $ - loc at name + id*2*2 + 2
   du 0}


macro arrayChangeTab  name,[id,label]
{ common
   local max
   count = 1
   align 4
   name:
  forward
   count = count + 1

  common
   repeat count
  forward
   if (id+1) = %
     dd  label
   end if

  common
  end repeat }


macro  arrayKey name,[id,str]
{ common
   local max,any,maxi
   maxi = 0
   max = 0
  forward
   if id > max
     max = id
   end if
  common
   align 4
   name dd szEmptyItem
   repeat max
  forward
    if id = %
     local loc
     if  name+ 4 + %*4 > $
      dd  (name+  %*4 - $)/4 dup any
     end if
     dd loc
    end if

  common
   end repeat
   repeat max
  forward
   if id = %
     loc du str,0
     if ($ - loc - 2)/2 > maxi
       maxi = ($ - loc - 2)/2
     end if
   end if
  common
   end repeat
   any  du 'Unknown',0
   siz = $ - any - 2
   name#lengthMax uniq maxi*3+3*2+1+8 };maxi*3 = три надписи может быть;3*2 = ' + ';+1 = на 0;+8 - так про запас



;----------------------------------------------
macro langtableDU  name,[id,namelng,pathlng]
{ common
   local count
   count = 0
   align 4
   name:
  forward
   local any
   id uniq count
   dd any
   count = count + 1

  forward
   lp#id  du namelng,0
   any  du pathlng,0  }


;----------------------------------------------
macro  enumMenuJmp num,last,[id,arg]
{ common
   local count
   count = num
   align 2
   Jmp#num:

  forward
   id uniq count
   count  = count + 1
   dw  arg-Start
  common
   last uniq count }


;----------------------------------------------
macro  arrayDU  name,[lab,val]
{ common
  align 2
  name:

  forward
  if lab eq Anonymous
    duA val
   else
    lab duA val
  end if


  common
  sizeof.#name uniq $ - name}
