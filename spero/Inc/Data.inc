constMySizeofRoaming   uniq  sizeof.szRoamingAppData - sizeof.tbRoaming - 0x2;0x2 - учитываем ноль


struct  ABOUT_ARRAY_ZONA
  Rect                           RECT ABOUT_BORDER+ABOUT_BMP_Width_Height-ABOUT_INDENT*2;Тоже что и массив точек но в  виде RECT
                                 ;Массив точек для того что бы нарисовать пером прямоугольник идет следом не перемещать
  Point                          POINT ABOUT_BORDER+ABOUT_BMP_Width_Height-ABOUT_INDENT*2-1
  Any2                           POINT;Уменьшим на одну единицу так как некоторые функции в частности ExtTextOutW при заливки фона залазят на верхние линии прямойгольника
  Any3                           POINT
  Any4                           POINT ABOUT_BORDER+ABOUT_BMP_Width_Height-ABOUT_INDENT*2-1
  Any5                           POINT ABOUT_BORDER+ABOUT_BMP_Width_Height-ABOUT_INDENT*2-1
  lpRectText                     dd  aAboutRectPaint.Email
  lpText                         dd  szInfoEmail
  iLeght                         dd  (sizeof.szInfoEmail-2)/2
  IdHelp                         dd  STR_HELP_Emeil;Отвечает за айди строки помощи
  Color                          dd  ?;Цвет для пера с  помощью каторого рисуем рамку в таймере
  Adjustment                     dd  ?;То что добавляем в цикле рисования к Color - для осветления
  Indicator                      dd  ?;Сообщает об том есть установлен ли таймер для этого RECT и указывает когда надо удалять таймер и использовать перо цвета фона окна
  StringLeft                     dd  ?;Обновленая координата начала строки по x с учетам стрелки
  Arrow                          POINT ?,ABOUT_BORDER+ABOUT_BMP_Width_Height
                                 db sizeof.POINT*9 dup(?)
ends


struct  ABOUT_RECT_ZONA
 Email                           ABOUT_ARRAY_ZONA;Отвечает за строку емейла
 Registry                        ABOUT_ARRAY_ZONA <?,?,?,?>,<?,?>,<?,?>,<?,?>,<?,?>,<?,?>,aAboutRectPaint.Registry,szInfoRegUrl,\
                                                  (sizeof.szInfoRegUrl-2)/2,STR_HELP_Registry,?,?;Отвечает за строку сайта

ends


struct  ABOUT_RECT_PAINT
 ;Координаты расположения NameVer в вкладки About
 NameVer                        RECT  ABOUT_BORDER+ABOUT_BMP_Width_Height
 ;Координаты расположения Copyright в вкладки About
 Copyright                      RECT ABOUT_BORDER+ABOUT_BMP_Width_Height,ABOUT_BORDER+ABOUT_BMP_Width_Height+SYSTAB_CONTROL_Height
 ;Координаты расположения Email в вкладки About
 Email                          RECT ABOUT_BORDER+ABOUT_BMP_Width_Height
 ;Координаты расположения Registry в вкладки About
 Registry                       RECT
ends


;Структура ini файла
;----------------------------------------------
  struct  INITILIZDATA
    aWindPlac                WINDOWPLACEMENT sizeof.WINDOWPLACEMENT,0,SW_SHOWNORMAL,<-1,-1>,<-1,-1>,<0,0,800,700>
    Language                 dd  COMBOBOX_ENGLISH
    idTabCurent              dd  SYSTAB_CONTROL_ABOUT_Id;idTabCurent  = Index item SysTab
    iRun                     dd  BST_UNCHECKED
    iSuspendPlayHook         dd  0;Если ноль то image = Play и хуки не влючены
  ends


  struct  INIFILEDATA ;структура ini файла - сохранненые настройки
    aIni                     INITILIZDATA;не должно смещаться должно быть первым
    szOpenExeBufer           du  MAX_PATH dup(?)
    szLaunchfileBufer        du  MAX_PATH dup(?)
    CRC32                    dd  ?;не должно смещаться должно быть последним
  ends




;----------------------------------------------
section '.data' data readable writeable
_DebugDate; для отладки в релезе не сохраниться

aRWKeyMap                               RTL_SRWLOCK              RTL_SRWLOCK_INIT;контролирует доступ к данным основноо потока с потом содержащим основные хуки

aVectoredExceptionHandler               VectoredExceptionHandler ;структура для исключений
aWc                                     WNDCLASSEXW              sizeof.WNDCLASSEXW,CS_PARENTDC,WindowProc,0,0,?,?,0,COLOR_BTNFACE+1,0,szEventOneApp,?;структурра для главного окна
aNotifyIconData                         NOTIFYICONDATAW_1        sizeof.NOTIFYICONDATAW,?,NOTIFY_ICON_Id,?,WM_APP_NOTIFY_ICON,?,?;Изменяються: uFlags и szTip

aListViewGroup                          GROUPLISTVIEW            LIST_VIEW_GROUP_GLOBAL_Id,0,szNameGroup,aListViewGroup,aListViewGroup

;(IMAGE_LIST_STATE_QUES_Id+1) shl 12; + 1 - так как тут идет индекс  не от нуля а от 1 в отличи от List Image
aListViewItem                           ITEMLISTVIEW             (IMAGE_LIST_STATE_QUES_Id+1) shl 12,LIST_VIEW_GROUP_GLOBAL_Id,aListViewItem,aListViewItem

aOpenFileExe                            OPENFILENAMEW            sizeof.OPENFILENAMEW,?,0,?,0,0,0,?,sizeof.INIFILEDATA.szOpenExeBufer/2,0,0,0,0,\
                                                                 OFN_FILEMUSTEXIST or OFN_PATHMUSTEXIST or OFN_LONGNAMES or OFN_EXPLORER or OFN_HIDEREADONLY,0,0,0,0,0,0,0,0

aMenuStateChange                        MENUITEMINFOW        sizeof.MENUITEMINFOW;Для изменения меню основного потока;статичен только элемент - cbSize

aFont                                   LOGFONTW             30,0,0,0,FW_SEMIBOLD,0,0,0,DEFAULT_CHARSET,OUT_TT_ONLY_PRECIS,CLIP_DEFAULT_PRECIS,\
                                                              DEFAULT_QUALITY,DEFAULT_PITCH or FF_SWISS,<'Segoe UI',0>
;Координаты расположения bmp в вкладки About
aAboutRectBmp                  RECT  ABOUT_BORDER,SYSTAB_CONTROL_Height+ABOUT_BORDER,ABOUT_BORDER+ABOUT_BMP_Width_Height,SYSTAB_CONTROL_Height+ABOUT_BORDER+ABOUT_BMP_Width_Height
aAboutRectPaint                ABOUT_RECT_PAINT;для удобства в одной структуре - все RECT используемые в About для  текста
aAboutZonaButton               ABOUT_RECT_ZONA;для удобства в одной структуре - все RECT используемые в About для рисования рамок вокруг теста

lpMultiLanguage             ddA  tbMultiLanguage;для хранение ссылки на лакаль;Указана ссылка что бы не пыталось при запуски програмы без причины освободить память

iPlague                     ddA  1;Портит логику программы;  равно нулю когда программа взломана; и не равно нулю когда все нормально

iCountIndexItem             ddA  1;для контроля итемов  количество\индекса индекс от нуля включая количество но так как добавлен на один больше то по
                                  ;факту даное число соответствует фактическому количеству  нужно использовать при добавлении или удалении итемав
;-----неицилизированые данные  - пустые
aIniFileData               INIFILEDATA;сюда загружаем настройки c ini файла и в случае их не валидности загружаем дефолтные
aKeySerial                 KEYFILEW;Для серийника
aMsg                       MSG;структура для главного цикла получения сообщений.
aMsgForHook                MSG ;для сообщений в патоке с хуками
aShellExecute              SHELLEXECUTEINFOW;Для открывания файлав в основном потоке изменяеться только имя открываемого файла
aClientRect                RECT;Заполняеться один раз до WM_WINDOWPOSCHANGED после всегда в сообщении - всегда содержит в для основного потока размер клиетской части главного окна
aWindowRect                RECT;Заполняеться в сообщении WM_WINDOWPOSCHANGED - всегда содержит в для основного потока размер главного окна
aAboutRectPaintUnion       RECT;Общий RECT для About для предварительной проверки что бы каждый не провирять отдельно RECT
aAboutFree                 ABOUT_FREE;для удобства в одной структуре - то что освобождаем когда 'закрываем' вкладку About
aTabCurrent                TAB_CURRENT;для удобства в одной структуре - отвечает за разные действия в текущей вкладке
aSysStock                  SYS_STOCK;для удобства в одной структуре - нужные системные обьекты храняться
aLpAboutZonaButton         LP_ABOUT_ARRAY_ZONA;для удобства в одной структуре - ссылки на текущие рамки вокруг прямоугольника и на текущий нажатый прямоугольник в вкладке About
aFlag                      FLAG;для удобства в одной структуре - сборник разных состояний в рамках основного окна -потока
aMenuState                 MENU_STATE;для удобства в одной структуре - сборник состояний меню в рамках основного окна -потока
aHookMinor                 HOOK_MINOR;для удобства в одной структуре - сборник контроля правильной записи и отображения клавиш и комбинаций в в доп хуке вызываемом в диалоге

iInputCount                ddA  ?;Отвечает за количество структур INPUT отправляемых SendInput
lpaInputCurrent            ddA  ?;Указатель на готовые структуры  INPUT - aInput; используеться в паре с iInputCount
;массив из 9 структур INPUT, так возможно такое сочитание 2 modif UP + 2 modif DOWN и 1 key DOWN + 2 modif UP и 1 key UP и 1 key (на всяк провсяк)
aInput                     ddA  (sizeof.INPUT*9)/4 dup(?)

aSet                       KEYMAP;нужна для сохранения нажатых клавиш в диалоге добавить клавиши
aTo                        KEYMAP;нужна для сохранения нажатых клавиш в диалоге добавить клавиши

aHook                       HOOK;Все данные для хука второго потока требующие обнуления собраны в структуру, для удобства обнуления

aArrayHwndFont              HWNDFONT;структура для всех детских окон  которым меняеться шрифт

lpEndRoaming                ddA  ?;указатель на ноль в szRoamingAppData
lpBufferDlgKeyMapScankode   ddA  ?;Указатель на  временный буфер в стеке нужен для хукав с Edit в нем храняться сканкоды клавиш и значение расширеная или нет клавиша
lpaKeyPressedMapIn          ddA  ?;Указатель на буфер для Edit где отмечаются клавиши которые удерживаються, если = 1 значит удерживаеться доступ по индексу vk,
lpBuyNameUser               ddA  ?;Указатель на пользователя купившего программу

iChangeCurrentListViewItemIndex  ddA  ?;id итема выбранага для редактирования
lpChangeCurrentListViewItem      ddA  ?;ссылка на структуру итема выбранага для редактирования

iBoolMessageReg                  ddA  ?;Если не равно нулю перед выходом покажим сообщение что копия взломана

iNumberNotLicense                ddA  ?;Число в юникоде номер которого нажать надо в диалоге DlgProcNotLicense

iThreadIdHook                    ddA  ?;id второго потока в котором работают основные хуки

iIdForStatic               ddA  ?;ID строки с языкавых ресурсав;Если программа куплена то просто стирает текст с статика инааче помещает сообщение что нет лицензии

iTimerActivation           ddA  ?;Если равно нуль - таймер не установлен надо установить его

msgTaskCreat               ddA  ?;Если оболочка windows перезапуститься придет это сообщение

hmenu                      ddA  ?;Меню основное
hmenuSubPopupNotify        ddA  ?;суб меню для иконки в треи
hmenuSubAddItem            ddA  ?;суб меню для List View

hicon                      ddA  ?;Иконка для загаловка основного окна и всех диалогов
hiconPause                 ddA  ? ;для  list view иконка
hiconPlay                  ddA  ?;для  list view иконка
hiconAdd                   ddA  ?;Иконка для кнопки доп действий времменное сохранение

hfontNotLicense            ddA  ?;Временный шрифт для диалога DlgProcNotLicense - освобождаеться

hEventThread               ddA  ?;Событие которое не дает запуститься второй раз приложению и и используеться для проверки создал ли второй поток очередь сообщений
hInstance                  ddA  ?;Дескриптор главного модуля
hheap                      ddA  ?;Дескриптор общего хеапа создающего при создании процесса
hImageListState            ddA  ?;лист с иконками для list view

hwnd                       ddA  ?; Дескриптор главного окна
hwndDeskop                 ddA  ?;Дескриптор рабочего стола
hwndButtonActivSusp        ddA  ?;Кнопка активации фильтрации клавиш
hwndShell_Tray             ddA  ?;хендля окна содержащего иконку в треи

hhookMouse                 ddA  ?;Хендл хука мыши основной во втором потоке   времменное сохранение
hhookKeyboard              ddA  ?;Хендл хука клавиатурнага основной во втором потоке   времменное сохранение
hhookInValue               ddA  ?;Хендл хука для определения нажатой клавиши времменное сохранение
hhookInValueMouse          ddA  ?;Хендл хука для определения нажатой клавиши мыши времменное сохранение


;Путь к Roaming и к нашем файлам в нем;Насчет изменения нужно быть осторожно так как этот путь несколь раз используеться и ожидаеться что не будет изменено
szRoamingAppData           duA  MAX_PATH dup(?)
