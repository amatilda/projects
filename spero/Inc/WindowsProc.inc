 proc WindowProc [esi edi ebp ebx],!Hwnd,!Wmsg,!Wparam,!Lparam
          locals
           !NumberOfBytesWritten      dd  ?;Заодно большой буфер не менее duMAX_PATH и надо быть акуратным при использовании других структур локальных и пригать акуратно, что бы не затереть
           !aTrackPopupPozition       POINT
           !aTrackPopupAddItem        POINT
           !aVkAppsRect               RECT
           !aIntersectRect            RECT
           !aPaintStruct              PAINTSTRUCT
           !aStateItemImage           LVHITTESTINFO
           !aStateItem                LVITEMW
           !aAddLVGROUP               LVGROUPW
           !aChangeLVGROUP            LVGROUPW
           !aAddItemCommmon           LVITEMW
           !aKeyPressedMap            dd  constLengthKeyPressedMap  dup(?);здесь отмечаются клавиши которые удерживаються, если = 1 значит удерживаеться доступ по индексу vk,
           !ddKeyMapScankode          dw VK_LAST+1 dup(?)  ;+1 на нулевой индекс
          endl
                 mov  ebp,[!Hwnd];ebp = Хендл окна
                 mov  eax,[!Wmsg];eax = сообщение
                 mov  ecx,[!Wparam];ecx = !Wparam
                 mov  edx,[!Lparam];edx = !Lparam
                 xor  ebx,ebx;ebx = ноль


;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------
                 cmp  eax,WM_MOUSEMOVE
                  je  .WM_MOUSEMOVE
                 cmp  eax,WM_COMMAND
                  je  .WM_COMMAND
                 cmp  eax,WM_NOTIFY
                  je  .WM_NOTIFY
                 cmp  eax,WM_PAINT
                  je  .WM_PAINT
                 cmp  eax,WM_LBUTTONDOWN
                  je  .WM_LBUTTONDOWN
                 cmp  eax,WM_LBUTTONUP
                  je  .WM_LBUTTONUP
                 cmp  eax,WM_MENUSELECT
                  je  .WM_MENUSELECT
                 cmp  eax,WM_ACTIVATE
                  je  .WM_ACTIVATE
                 cmp  eax,WM_APP_NOTIFY_ICON
                  je  .WM_APP_NOTIFY_ICON
                 cmp  eax,WM_WINDOWPOSCHANGED
                  je  .WM_WINDOWPOSCHANGED
                 cmp  eax,WM_WINDOWPOSCHANGING
                  je  .WM_WINDOWPOSCHANGING
                 cmp  eax,WM_SYSCOMMAND
                  je  .WM_SYSCOMMAND
                 cmp  eax,WM_DESTROY
                  je  .WM_DESTROY
                 cmp  eax,[msgTaskCreat]
                  je  .msgTaskCreat
                 cmp  eax,WM_APP_ACTION
                  je  .WM_APP_ACTION
                 cmp  eax,WM_APP_LICENSE
                  je  .WM_APP_LICENSE
                 cmp  eax,WM_SYSCOLORCHANGE
                  je  .WM_SYSCOLORCHANGE
                 cmp  eax,WM_QUERYENDSESSION
                  je  .WM_DESTROY
                 cmp  eax,WM_ENDSESSION
                  je  .proc_null
;--------------------------------------
           .freeJmp:
             freeJmp
                 jmp  [DefWindowProcW]

;Здесь будем проверять купленна программа или нет и добавления надписи владельца, если нет то вывод окна задержки и добавление надписи что не куплена
;ebx = ноль
;--------------------------------------------------------------------------------------
.WM_APP_LICENSE:
       ;Если = 0 тогда нет лицензии
        cld;установим на всякий про всякий направление строкавых
        or  ebx,[lpBuyNameUser];ebx = ноль
        jz  .NotLicense

;Проверим адекватность серийника посчитав RSA
;ebx = [lpBuyNameUser]

       ;Подготовим для нижней части
       mov  esi,aKeySerial
       mov  edi,constNumberN
       mov  ecx,[esi+KEYFILEW.iDigitalSignatureLow]
       mov  esi,[esi+KEYFILEW.iCRC32ForKey]
       mov  eax,ecx
       mov  ebp,constNumberE-1;учитываем что при возвидении нужно уменьшить на 1, при умнажении для цикла

       @@:
       mul  ecx
       div  edi
       dec  ebp
       mov  eax,edx
       jnz  @b

       cmp  si,dx
       jne  .NotLicense

       ;Подготовим для верхней части
       ;ebx = [lpBuyNameUser]
       mov  ecx,[aKeySerial.iDigitalSignatureHiw]
       shr  esi,16
       mov  ebp,constNumberE-1;учитываем что при возвидении нужно уменьшить на 1, при умнажении для цикла
       mov  eax,ecx

       @@:
       mul  ecx
       div  edi
       dec  ebp
       mov  eax,edx
       jnz  @b

       cmp  esi,edx
       jne  .NotLicense

;Программа купленна отобразим теперь имя пользователя купившего в загаловке главного окна
;ebx = [lpBuyNameUser]

       ;Подготовим строку для загаловка
       ;ebx = [lpBuyNameUser]
       lea  edi,[!NumberOfBytesWritten]
       mov  esi,sNameMainWin
       mov  ecx,sizeof.sNameMainWin/2
       mov  ebp,edi;ebp = caption hwnd
       rep  movsw

       ;Подготовим строку для загаловка
       ;ebx = [lpBuyNameUser];ebp = caption hwnd
       mov  ecx,[aKeySerial.lengthszNameUser]
      xchg  esi,ebx
       inc  ecx;что бы и ноль скопировать
       rep  movsw;ecx = 0

       ;Установим текс в основное окно
       ;ecx = 0;ebp = caption hwnd
    invoke  SendMessageW,[!Hwnd],WM_SETTEXT,ecx,ebp;ecx = 0;ebp = caption hwnd
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       ;Idтеста для отображения в статике или пустая строка для стирания
       mov  [iIdForStatic],STR_EMPTY

       jmp  .Shell_NotifyIcon

                ;На программу у пользователя нет лицензии
                .NotLicense:
                        ;Idтеста для отображения в статике или пустая строка для стирания
                         mov  [iIdForStatic],STR_WINDOW_NOT_LICENSE

                         ;Отобразим в загаловке что не зарегестрированна
                         ;Подготовим строку для загаловка
                     stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_WINDOW_NOT_LICENSE;eax = указатель на конечные данные;ecx - длина строки в символах без нуля
                         lea  edi,[!NumberOfBytesWritten]
                        xchg  eax,esi
                         mov  ebp,edi;ebp = caption windows
                         rep  movsw;ecx - длина строки в символах без нуля
                         mov  [edi],cx;ecx = 0

                      ;Установим текс в основное окно
                      ;ecx = 0;ebp = caption windows
                         mov  ebx,[!Hwnd];ebx = [!Hwnd]
                      invoke  SendMessageW,ebx,WM_SETTEXT,ecx,ebp;ecx = 0;ebp = caption windows;ebx = [!Hwnd]
                              _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                         ;Запускаем диалог выбора кнопак
                     stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_DLG,DIALOG_NOTLICENSE
                      invoke  DialogBoxIndirectParamW,[hInstance],eax,ebx,DlgProcNotLicense,ecx;ebx = [!Hwnd];ecx - роли не играет
                        test  eax,eax;Если ноль значит пользователь нажал не ту клнопку - выходим
                         jnz  .Shell_NotifyIcon

                      invoke  DestroyWindow,ebx;ebx = [!Hwnd]
                              _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                         jmp  .proc_null

          .Shell_NotifyIcon:
                      ;регистрируем и будем получать сообщение на случай если Exploer будет перезапущен
                      invoke  RegisterWindowMessageW,szMsgTaskCreat
                              _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                         mov  [msgTaskCreat],eax

                         ;Подготовим
                         mov  ebp,[iIdForStatic];ebp = Id STR

                     ;добавляем значок в трей
                     ;Добавляет иконку или изменяенти подсказку к ней в зависимости от языка и активности главного хука
                     ;На входе:;!dwMessage = NIM_ADD или NIM_MODIFY для Shell_NotifyIcon
                     stdcall  ChangeNotifyIcon,NIM_ADD

                         ;Если программа не куплена то отобразим в статике это
                         ;ebp = Id STR
                         cmp  ebp,STR_WINDOW_NOT_LICENSE;ebp = Id STR
                         jne  .proc_null

                     stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,ebp;ebp = Id STR

                      ;Установим что программа не купленна
                      invoke  SendMessageW,[aArrayHwndFont.hwndStatic],WM_SETTEXT,0,eax
                              _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                         .lpEmptyGuardianEnd:;Конец участка для проверки приманки
                         jmp  .proc_null


;Отвечает за активации окна
;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------------------------------------------------------
.WM_ACTIVATE:
       mov  [aFlag.Activate],ecx;ecx = !Wparam
       jmp  [aTabCurrent.Activate];ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam

;Отвечает за перемещения курсора мыши
;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------------------------------------------------------
.WM_MOUSEMOVE:
       jmp  [aTabCurrent.MouseMove];ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam

;Отвечает за нажатия левой кнопки мыши
;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------------------------------------------------------
.WM_LBUTTONDOWN:
       jmp  [aTabCurrent.MouseLeftUpDown];ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam

;Отвечает за отжатие левой кнопки мыши
;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------------------------------------------------------
.WM_LBUTTONUP:
       jmp  [aTabCurrent.MouseLeftUpDown];ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam

;Отвечает за рисование
;ebp = Хендл окна;ebx = ноль lpPaintTabCurrent
;--------------------------------------------------------------------------------------
.WM_PAINT:
       jmp  [aTabCurrent.Paint];ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam

;Отвечает за смену используемого цвета
;--------------------------------------------------------------------------------------
.WM_SYSCOLORCHANGE:
       mov  esi,[GetSysColor];esi = [GetSysColor]
       mov  edi,aSysStock;edi = SYS_STOCK

   stdcall  esi,COLOR_BTNFACE;esi = [GetSysColor]
       mov  [edi+SYS_STOCK.rgbBTNFACE],eax;edi = SYS_STOCK

   stdcall  esi,COLOR_BTNTEXT;esi = [GetSysColor]
       mov  [edi+SYS_STOCK.rgbBTNTEXT],eax;edi = SYS_STOCK

   stdcall  esi,COLOR_HOTLIGHT;esi = [GetSysColor]
       mov  [edi+SYS_STOCK.rgbHOTLIGHT],eax;edi = SYS_STOCK

   stdcall  esi,COLOR_3DLIGHT;esi = [GetSysColor]
       mov  [edi+SYS_STOCK.rgb3DLIGHT],eax;edi = SYS_STOCK

       jmp  .freeJmp;Пусть прешлет дочерним окнам


;Отвечает за подсказки к меню
;ebx = ноль;ecx = !Wparam
;--------------------------------------------------------------------------------------
.WM_MENUSELECT:
       ;Проверим выходим ли с меню в котором показывали в статике подсказки, если так то очистим текст
       cmp  ecx,[aMenuState.iForStaticMenu]
        je  .FreeStaticText;ebx = ноль

      ;Проверяем может это под меню или системное меню - тогда ничего неделаем
      test  ecx,(MF_POPUP or MF_SYSMENU) shl 16
    cmovnz  ebx,esp;ebx = ноль

       ;Если id menu равен нулю - не наш случай, так как все используемые меню имеют не нулевой индифактор
       and  ecx,0xFFFF;eсx = id menu
     cmovz  ebx,esp;ebx = ноль

       ;Если id menu больше или равно данного числа значит это меню не входит в список меню имеющих подсказки
       cmp  ecx,MENU_HELP_LAST_Id;eсx = id menu
    cmovae  ebx,esp;ebx = ноль

      ;Если не равно нулю не наш случай;Так как все пункты меню имеею подсказки иначе надо добавить доп проверки как в меню с диалогам DlgProcAddKeys
      test  ebx,ebx;ebx = ноль
       jnz  .proc_null

       ;Так как id menu совпадает с id STR подсказки - получаем текст строки подсказки
       ;ebx = ноль;eсx = id menu
   stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR_MENU,ecx;eсx = id menu

    ;Установим текст подсказку
    ;ebx = ноль
    invoke  SendMessageW,[aArrayHwndFont.hwndStatic],WM_SETTEXT,ebx,eax;ebx = ноль
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       ;Установим
       mov  [aMenuState.iForStaticMenu],0xFFFF0000

       jmp  .proc_null

                 ;Очистим статик что бы не остался текст связаный с помощью меню
                 ;ebx = ноль
                 ;-------------------------------------------
                .FreeStaticText:
                         stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,[iIdForStatic]

                          invoke  SendMessageW,[aArrayHwndFont.hwndStatic],WM_SETTEXT,ebx,eax;ebx = ноль
                                  _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                             ;Установим
                             mov  [aMenuState.iForStaticMenu],ebx;ebx = ноль

                             jmp  .proc_null



;Контролируем что бы  при потери фокуса не оставался нажатый прямоугольник и нарисованая рамка
;ebx = ноль;ecx = !Wparam
.AboutActivate:
;--------------------------------------------------------------------------------------
      ;Проверим активируеться или деактивируеться окно;Если активируеться нечего не делаем
      test  ecx,ecx;ecx = !Wparam;WA_INACTIVE = 0
       jnz  .proc_null

       ;Подготовим
       mov  ebp,aLpAboutZonaButton;ebp = aLpAboutZonaButton
       mov  esi,aSysStock;esi = aSysStock
       mov  edx,[ebp+LP_ABOUT_ARRAY_ZONA.UpDown];edx = [LP_ABOUT_ARRAY_ZONA.UpDown];ebp = aLpAboutZonaButton
       mov  edi,[ebp+LP_ABOUT_ARRAY_ZONA.Point];edi = [LP_ABOUT_ARRAY_ZONA.Point];ebp = aLpAboutZonaButton

       ;Проверим может прямоугольник еще нажат хоть и фокус теряем
       ;edi = [LP_ABOUT_ARRAY_ZONA.Point];edx = [LP_ABOUT_ARRAY_ZONA.UpDown]
      test  edx,edx
        jz  .NotUpDown;edi = [LP_ABOUT_ARRAY_ZONA.Point];ebp = aLpAboutZonaButton;ebx = ноль;esi = aSysStock

       ;Обнулим
       ;edx = [LP_ABOUT_ARRAY_ZONA.UpDown];ebp = aLpAboutZonaButton;ebx = ноль;esi = aSysStock
       mov  [ebp+LP_ABOUT_ARRAY_ZONA.UpDown],ebx;ebp = aLpAboutZonaButton;ebx = ноль

     ;Рисует текс и фон нажатай клавиши или же востановит исходное состояние не нажатой, в зависимости от цветов в аргументах
    ;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbBrush - цвет для кисти;!rgbText - цвет для текста
    ;edx = [LP_ABOUT_ARRAY_ZONA.UpDown];esi = aSysStock
    stdcall  PaintBottomUpDown,edx,[esi+SYS_STOCK.rgbBTNFACE],[esi+SYS_STOCK.rgbHOTLIGHT];edx = [LP_ABOUT_ARRAY_ZONA.UpDown];esi = aSysStock

;Проверим может рамко еще не убрана вокруг строки - тогда уберем
;edi = [LP_ABOUT_ARRAY_ZONA.Point];ebp = aLpAboutZonaButton;ebx = ноль;esi = aSysStock
.NotUpDown:
      test  edi,edi;edi = [LP_ABOUT_ARRAY_ZONA.Point]
        jz  .proc_null;edi = [LP_ABOUT_ARRAY_ZONA.Point]

      ;Обнулим
       mov  [ebp+LP_ABOUT_ARRAY_ZONA.Point],ebx;ebp = aLpAboutZonaButton;ebx = ноль

   ;Раз рамка была нарисована  значит и текст подсказки был вывидин на статик - очищаем
   stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,[iIdForStatic]
    invoke  SendMessageW,[aArrayHwndFont.hwndStatic],WM_SETTEXT,ebx,eax;ebx = ноль
           _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

    ;Рисует рамку для прямоугольника на основании  5 точек с помощью стандартного pen и цвета полученого от пользователя.
    ;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbPen - цвет для pen который будет использован для рисования рамки
    ;edi = [LP_ABOUT_ARRAY_ZONA.Point];esi = aSysStock
    stdcall  PaintBottomFrame,edi,[esi+SYS_STOCK.rgbBTNFACE];edi = [LP_ABOUT_ARRAY_ZONA.Point]

       jmp  .proc_null


;Кантралируем отжатия левой кнопки мыши вкладки About
;edx = !Lparam
;--------------------------------------------------------------------------------------
.AboutMouseLeftUpDown:
      ;Проверяет есть ли  пересечение точки с  RECT из aAboutZonaButton
      ;На входе:;edx = !Lparam - содержит координаты курсора мыши в клиентских
      ;На выходе:;ebx = aAboutZonaButton.RECT если найдено пересечение иначе равно нулю если непересекаються
      ;Изменяемые реистры:eax,ecx,edx,ebx,esi,edi
      ;--------------------------------------------------------------------------------------
      call .AboutZonaButtonPtInRect
      test  ebx,ebx
        jz  .proc_null

                  ;Мышь находиться в нашем прямоугольниики отобразим в зависимости от того кнопка нажата или отжата
                  ;ebx = aAboutZonaButton.RECT
                  ;-------------------------------------------
                  .CrossPointLeftUpDown:
                                 ;Подготовим
                                 mov  edx,aSysStock;edx = aSysStock
                                 xor  edi,edi;edi = 0
                                 mov  eax,[edx+SYS_STOCK.rgbBTNFACE];edx = aSysStock;eax = !rgbBrush
                                 mov  ecx,[edx+SYS_STOCK.rgbHOTLIGHT];edx = aSysStock;ecx = !rgbText
                                 mov  edx,aLpAboutZonaButton.UpDown;edx = aLpAboutZonaButton.UpDown

                                 ;Проверим кнопка левой мыши нажимаеться или отжимаеться
                                 cmp  [!Wmsg],WM_LBUTTONUP
                                  je  .AboutMouseLeftUp;ebx = aAboutZonaButton.RECT;eax = !rgbBrush;ecx = !rgbText;edi = 0;edx = aLpAboutZonaButton.UpDown

                               ;Установим что нажата
                               ;ebx = aAboutZonaButton.RECT;eax = !rgbBrush;ecx = !rgbText;edi = 0;edx = aLpAboutZonaButton.UpDown
                                 mov  [edx],ebx;ebx = aAboutZonaButton.RECT;edx = aLpAboutZonaButton.UpDown

                               ;Подготовим
                                xchg  esi,ebx

                               ;Подготовим цвета;При уменьшении значения rgb цвет тимнее становиться и делаем проверку на перенос что бы значение не перешло в "меньше нуля"
                               ;Цвет фона уменьшаем на 30, а цвет текста на 90
                               ;esi = aAboutZonaButton.RECT;eax = !rgbBrush;ecx = !rgbText;edi = 0
                                 rol  eax,16;eax = !rgbBrush
                                 rol  ecx,16;ecx = !rgbText
                               movzx  edx,al
                               movzx  ebx,cl
                                 sub  edx,30
                               cmovc  edx,edi;edi = 0
                                 sub  ebx,90
                               cmovc  ebx,edi;edi = 0
                                 shl  edx,16
                                 shl  ebx,16
                                 rol  eax,8;eax = !rgbBrush
                                 rol  ecx,8;ecx = !rgbText
                                 mov  dl,al
                                 mov  bl,cl
                                 sub  dx,30
                               cmovc  dx,di;edi = 0
                                 sub  bx,90
                               cmovc  bx,di;edi = 0
                                 shl  dx,8
                                 shl  bx,8
                                 shr  eax,24;eax = !rgbBrush
                                 shr  ecx,24;ecx = !rgbText
                                 sub  eax,30
                               cmovc  eax,edi;edi = 0
                                 sub  ecx,90
                               cmovc  ecx,edi;edi = 0
                                  or  eax,edx;eax = !rgbBrush
                                  or  ecx,ebx;ecx = !rgbText

                             ;Рисует текс и фон нажатай клавиши или же востановит исходное состояние не нажатой, в зависимости от цветов в аргументах
                             ;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbBrush - цвет для кисти;!rgbText - цвет для текста
                             ;esi = aAboutZonaButton.RECT;eax = !rgbBrush;ecx = !rgbText
                             stdcall  PaintBottomUpDown,esi,eax,ecx;esi = aAboutZonaButton.RECT;eax = !rgbBrush;ecx = !rgbText
                                 jmp  .proc_null

                  ;Востановим текст и фон который должен быть когда кнопка не нажата
                  ;ebx = aAboutZonaButton.RECT;eax = !rgbBrush;ecx = !rgbText;edi = 0;edx = aLpAboutZonaButton.UpDown
                  .AboutMouseLeftUp:
                                 ;Проверим может хоть отжимаем на одном прямоугольники но нажат был другой - его тогда и отожмем
                                 mov  ebp,[edx];edx = aLpAboutZonaButton.UpDown;ebp = prevaAboutZonaButton.RECT
                                 mov  esi,ebx
                                test  ebp,ebp
                              cmovnz  ebx,ebp


                                 ;Обнулим раз отжимаеться
                                 mov  [edx],edi;edi = 0;edx = aLpAboutZonaButton.UpDown

                             ;Рисует текс и фон нажатай клавиши или же востановит исходное состояние не нажатой, в зависимости от цветов в аргументах
                             ;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbBrush - цвет для кисти;!rgbText - цвет для текста
                             stdcall  PaintBottomUpDown,ebx,eax,ecx;ebx = aAboutZonaButton.RECT;eax = !rgbBrush;ecx = !rgbText

                                 ;Проверим если предыдущий нажатый и теперешний прямоугольник на котором мы нажодимся совпадают значит - предприним действие
                                 ;esi = aAboutZonaButton.RECT;ebp = prevaAboutZonaButton.RECT
                                 cmp  esi,ebp
                                 jne  .proc_null;ebx = aAboutZonaButton.RECT;ebp = prevaAboutZonaButton.RECT

                                 ;Действие
                                 ;ebx = aAboutZonaButton.RECT
                                 mov  eax,[esi+ABOUT_ARRAY_ZONA.lpText];esi = aAboutZonaButton.RECT
                                 mov  ecx,szInfoEmailTo
                                 mov  edx,aShellExecute;edx = aShellExecute
                                 cmp  eax,szInfoEmail
                               cmove  eax,ecx
                                 mov  [edx+SHELLEXECUTEINFOW.lpFile],eax;edx = aShellExecute
                              invoke  ShellExecuteExW,edx;edx = aShellExecute

                                 jmp  .proc_null

;Кантралируем перемещения курсора мыши вкладку About
;ebp = Хендл окна;ebx = ноль;edx = !Lparam
;--------------------------------------------------------------------------------------
.AboutMouseMove:
      ;Проверим активно акно или нет, если равно нулю значит не  активно - ничего неделаем
       cmp  [aFlag.Activate],ebx;ebx = ноль
        je  .proc_null

      ;Проверяет есть ли  пересечение точки с  RECT из aAboutZonaButton
      ;На входе:;edx = !Lparam - содержит координаты курсора мыши в клиентских
      ;На выходе:;ebx = aAboutZonaButton.RECT если найдено пересечение иначе равно нулю если непересекаються
      ;Изменяемые реистры:eax,ecx,edx,ebx,esi,edi
      ;--------------------------------------------------------------------------------------
      call .AboutZonaButtonPtInRect

       ;Подготовим
       ;ebp = Хендл окна;ebx = aAboutZonaButton.RECT
       mov  edi,aLpAboutZonaButton;edi = aLpAboutZonaButton
       mov  esi,aSysStock;esi = aSysStock
       mov  eax,[edi+LP_ABOUT_ARRAY_ZONA.UpDown];edi = aLpAboutZonaButton;eax = [LP_ABOUT_ARRAY_ZONA.UpDown]
       xor  edx,edx;edx = 0

       ;Проверим может прямоугольник нарисован отжатым, а мы вышли за его пределы
       ;ebp = Хендл окна;ebx = aAboutZonaButton.RECT;edi = aLpAboutZonaButton;esi = aSysStock;eax = [LP_ABOUT_ARRAY_ZONA.UpDown];edx = 0
      test  eax,eax;eax = [LP_ABOUT_ARRAY_ZONA.UpDown]
     cmovz  edx,esp;edx = 0
       cmp  eax,ebx;eax = [LP_ABOUT_ARRAY_ZONA.UpDown];ebx = aAboutZonaButton.RECT
     cmove  edx,esp;edx = 0

      ;Если не равно нулю значит нет нажатага прямоугольника требуещего востановления
      test  edx,edx;edx = 0
       jnz  .NextTestMouseMove;ebp = Хендл окна;ebx = aAboutZonaButton.RECT;edi = aLpAboutZonaButton;esi = aSysStock

       ;Обнулим
       ;ebp = Хендл окна;ebx = aAboutZonaButton.RECT;edi = aLpAboutZonaButton;esi = aSysStock;eax = [LP_ABOUT_ARRAY_ZONA.UpDown];edx = 0
       mov  [edi+LP_ABOUT_ARRAY_ZONA.UpDown],edx;edx = 0;edi = aLpAboutZonaButton

   ;Рисует текс и фон нажатай клавиши или же востановит исходное состояние не нажатой, в зависимости от цветов в аргументах
   ;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbBrush - цвет для кисти;!rgbText - цвет для текста
   ;ebp = Хендл окна;ebx = aAboutZonaButton.RECT;edi = aLpAboutZonaButton;esi = aSysStock;eax = [LP_ABOUT_ARRAY_ZONA.UpDown]
   stdcall  PaintBottomUpDown,eax,[esi+SYS_STOCK.rgbBTNFACE],[esi+SYS_STOCK.rgbHOTLIGHT];eax = [LP_ABOUT_ARRAY_ZONA.UpDown];esi = aSysStock

      ;Если не равно нулю значит есть пересечение
      ;ebp = Хендл окна;ebx = aAboutZonaButton.RECT;edi = aLpAboutZonaButton;esi = aSysStock
.NextTestMouseMove:
      test  ebx,ebx;ebx = aAboutZonaButton.RECT
       jnz  .CrossPoint;ebp = Хендл окна;ebx = aAboutZonaButton.RECT;edi = aLpAboutZonaButton

       ;Раз тут значит курсор не над нашими рисоваными прямоугольниками
       ;ebp = Хендл окна;ebx = 0;edi = aLpAboutZonaButton;esi = aSysStock
       ;-------------------------------------------
        ;Если ноль значит рамки не на рисованы -   нечего не делаем
        or  ebx,[edi+LP_ABOUT_ARRAY_ZONA.Point];edi = aLpAboutZonaButton;ebx = 0
        jz  .proc_null

   ;Раз рамка была нарисована значит и текст подсказки был вывидин на статик - очищаем
   stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,[iIdForStatic]
    invoke  SendMessageW,[aArrayHwndFont.hwndStatic],WM_SETTEXT,0,eax
           _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       ;Подготовим
       mov  [edi+LP_ABOUT_ARRAY_ZONA.Point],0;edi = aLpAboutZonaButton

       ;Подготовим
       ;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA;esi = aSysStock
.CleanTimer:
       mov  eax,[esi+SYS_STOCK.rgbBTNFACE];esi = aSysStock
       mov  ecx,[esi+SYS_STOCK.rgb3DLIGHT];esi = aSysStock


      ;Проверим что бы цвет фона был больше чем цвет рамки - иначе не сможем осветлить рамку и тем самым таймер не нужно запускать
      ;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA;eax = [rgbBTNFACE];ecx = [rgb3DLIGHT]
       cmp  eax,ecx;eax = [rgbBTNFACE];ecx = [rgb3DLIGHT] ;esi = aSysStock
        jb  .Clean;ebx = ABOUT_ARRAY_ZONA;eax = [rgbBTNFACE]

       ;Подготовим
       ;Разницу между цветам фона и цветам рамки делим на 4 части по байтно все это делаем и делаем проверку на перенос что бы значение не перешло в "больше байта"
       ;Белый цвет 0xFFFFFF - подругому говоря прибовление к цвету осветляеи тем самым имитирует медленное исчезновение рамки
       rol  eax,16;eax = [rgbBTNFACE]
       rol  ecx,16;ecx = [rgb3DLIGHT]
       mov  dl,al
       mov  dh,cl
       ror  eax,16;eax = [rgbBTNFACE]
       ror  ecx,16;ecx = [rgb3DLIGHT]
       sub  dl,dh
       jnc  @f
       xor  dl,dl
       @@:
       sub  al,cl
       jnc  @f
       xor  al,al
       @@:
       sub  ah,ch
       jnc  @f
       xor  ah,ah
       @@:
       xor  dh,dh
       shr  dl,2
       shr  al,2
       shr  ah,2
       shl  edx,16
       and  eax,0xFFFF
        or  edx,eax

       ;Установим в нужные значеия
       mov  [ebx+ABOUT_ARRAY_ZONA.Color],ecx;ebx = ABOUT_ARRAY_ZONA
       mov  [ebx+ABOUT_ARRAY_ZONA.Adjustment],edx;ebx = ABOUT_ARRAY_ZONA
       mov  [ebx+ABOUT_ARRAY_ZONA.Indicator],4;ebx = ABOUT_ARRAY_ZONA

    ;Установим таймер для того что бы рамка изчезала постепена;ebx = ABOUT_ARRAY_ZONA - используем ссылку на RECT для уникальности и для получения инфы о таймере
    ;ebp = Хендл окна;ebx = aAboutZonaButton.RECT
    invoke  SetTimer,ebp,ebx,800,TimerBottomClean;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       jmp  .proc_null

   ;Рисует рамку для прямоугольника на основании  5 точек с помощью стандартного pen и цвета полученого от пользователя.
   ;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbPen - цвет для pen который будет использован для рисования рамки
   ;ebx = ABOUT_ARRAY_ZONA;eax = [rgbBTNFACE]
   .Clean:
   stdcall  PaintBottomFrame,ebx,eax;eax = [rgbBTNFACE];ebx = ABOUT_ARRAY_ZONA
       jmp  .proc_null

           ;Мышь пришла на  один из наших прямоугольников
           ;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA;edi = aLpAboutZonaButton
           ;-------------------------------------------
       .CrossPoint:
                ;Подготовим
                mov  esi,[edi+LP_ABOUT_ARRAY_ZONA.Point];edi = aLpAboutZonaButton;esi = prevABOUT_ARRAY_ZONA

                ;Если предыдущее совпадает с теперерешним тогда небыло изменений никаких поэтому нечего не делаем
                ;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA;esi = prevABOUT_ARRAY_ZONA;edi = aLpAboutZonaButton
                cmp  esi,ebx;ebx = ABOUT_ARRAY_ZONA;esi = prevABOUT_ARRAY_ZONA
                 je  .proc_null

            ;Раз есть пересечения и придыдущее с теперешним не совпадает, значит в этом прямоугольники первый раз - вывиди текст подсказки
            stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,[ebx+ABOUT_ARRAY_ZONA.IdHelp];ebx = ABOUT_ARRAY_ZONA
             invoke  SendMessageW,[aArrayHwndFont.hwndStatic],WM_SETTEXT,0,eax
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Установим - тем самым будем знать был ли где прочертен бордюр и где именно
                mov  [edi+LP_ABOUT_ARRAY_ZONA.Point],ebx;ebx = ABOUT_ARRAY_ZONA;edi = aLpAboutZonaButton

            ;Рисует рамку для прямоугольника на основании  5 точек с помощью стандартного pen и цвета полученого от пользователя.
            ;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbPen - цвет для pen который будет использован для рисования рамки
            ;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA;esi = prevABOUT_ARRAY_ZONA
            stdcall  PaintBottomFrame,ebx,[aSysStock.rgb3DLIGHT];ebx = ABOUT_ARRAY_ZONA

               ;Проверит может мы возратились на тот же прямоугольник на котором были не давно и тем самым еще таймер даже не исчез;Если равно нулю значит таймера нет
               ;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA;esi = prevABOUT_ARRAY_ZONA
               xor  eax,eax;eax = 0
               cmp  [ebx+ABOUT_ARRAY_ZONA.Indicator],eax;ebx = ABOUT_ARRAY_ZONA;eax = 0
                je  .NotTimer;ebp = Хендл окна;esi = prevABOUT_ARRAY_ZONA

               ;Таймер есть удалим его и очистим Indicator что бы рамка не начала исчезать покуда курсор в прямоугольники
               ;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA;esi = prevABOUT_ARRAY_ZONA ;eax = 0
               mov  [ebx+ABOUT_ARRAY_ZONA.Indicator],eax;ebx = ABOUT_ARRAY_ZONA;eax = 0

            invoke  KillTimer,ebp,ebx;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA и посавместительству idEvent
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

         ;Проверим может иза того что вовремя сообщение не пришло так как на низкам приорите, то может проскочили пустое пространство и тем самым рамка предыдущего прямоугольника не убрана
         ;ebp = Хендл окна;esi = prevABOUT_ARRAY_ZONA
         .NotTimer:
               test  esi,esi;esi = prevABOUT_ARRAY_ZONA
                 jz  .proc_null

               ;Раз остался предыдущий прямоугольник то удалим просто рамку или с таймерм в зависимости от цветов используемых
                mov  ebx,esi;esi = prevABOUT_ARRAY_ZONA
                mov  esi,aSysStock;esi = aSysStock

                jmp  .CleanTimer;ebp = Хендл окна;ebx = ABOUT_ARRAY_ZONA;esi = aSysStock


;Проверяет есть ли  пересечение точки с  RECT из aAboutZonaButton
;На входе:;edx = !Lparam - содержит координаты курсора мыши в клиентских
;На выходе:;ebx = aAboutZonaButton.RECT если найдено пересечение иначе равно нулю если непересекаються
;Изменяемые реистры:eax,ecx,edx,ebx,esi,edi
;--------------------------------------------------------------------------------------
procM AboutZonaButtonPtInRect
       ;Подготовим
       ;edx = !Lparam
     movzx  esi,dx;esi = x
       shr  edx,16
       mov  ebx,aAboutZonaButton.Email.Rect;ebx = aAboutZonaButton.Email.Rect
       mov  edi,edx;edi = y

   ;ebx = aAboutRectPaint.Email;esi = x;edi = y
   ;Проверяем находиться ли точка в массиве RECT;Если eax = 0 тогда пересекаються
   stdcall  PtInRectHL,esi,edi,ebx,1;esi = x;edi = y;ebx = aAboutZonaButton.Email.Rect
      test  eax,eax
        jz  .AboutZonaButtonPtInRectCross;ebx = aAboutZonaButton.RECT

       ;Подготовим
       mov  ebx,aAboutZonaButton.Registry.Rect;ebx = aAboutZonaButton.Registry.Rect

   ;ebx = aAboutRectPaint.Registry;esi = x;edi = y
   ;Проверяем находиться ли точка в массиве RECT;Если eax = 0 тогда пересекаються
   stdcall  PtInRectHL,esi,edi,ebx,1;esi = x;edi = y;ebx = aAboutZonaButton.Registry.Rect

.AboutZonaButtonPtInRectCross:
       xor  edx,edx
      test  eax,eax
    cmovnz  ebx,edx
      retd
endpM




;Отрысоваем вкладку About
;ebp = Хендл окна
;--------------------------------------------------------------------------------------
.AboutPaint:
       lea  esi,[!aPaintStruct];esi = PAINTSTRUCT
    invoke  BeginPaint,ebp,esi;ebp = Хендл окна;esi = PAINTSTRUCT
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       ;Подготовим
       mov  ebx,eax;ebx = hdc current
       add  esi,PAINTSTRUCT.rcPaint;esi = PAINTSTRUCT.rcPaint
       lea  edi,[!aIntersectRect];edi = !aInsertRect

     ;Проверим общий RECTb и тем самым узнаем может не нужно вообще  рисовать текст или  bmp
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint
     ;Проверяем пересекаеться ли RECT в !lprcSrc1 с массивам RECT в !lprcSrc2;!nCount - количество в RECT массиве;Если eax = 0 тогда пересекаються
   stdcall  RectInRectHL,aAboutRectPaintUnion,esi,1;esi = PAINTSTRUCT.rcPaint
      test  eax,eax
       jnz  .PaintAboutEndPaint;ebx = hdc current;esi = PAINTSTRUCT.rcPaint

       ;Подготовим
       lea  edi,[!aIntersectRect];edi = !aInsertRect

     ;Установим режим что бы фон когда рисуеться текст не изменялся
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = !aInsertRect
    invoke  SetBkMode,ebx,TRANSPARENT;ebx = hdc current
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

     ;Установим цвет текста
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = !aInsertRect
    invoke  SetTextColor,ebx,[aSysStock.rgbBTNTEXT];ebx = hdc current
            _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

     ;Установим шрифт
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = !aInsertRect
    invoke  SelectObject,ebx,[aAboutFree.hfont30SEMIBOLD];ebx = hdc current
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

     ;Проверим нужно ли рисовать bmp about? если нужно то какую часть
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = !aInsertRect
     ;-------------------------------------------
    invoke  IntersectRect,edi,esi,aAboutRectBmp;edi = !aIntersectRect;esi = PAINTSTRUCT.rcPaint
      test  eax,eax
        jz  .PaintAboutDrawNameVer;esi = PAINTSTRUCT.rcPaint;ebx = hdc current;COLOR_BTNTEXT;[hfont30SEMIBOLD]

     ;Создадим DC совмистимое с текущим для  нашего bmp
    invoke  CreateCompatibleDC,ebx;ebx = hdc current
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
       mov  ebp,eax;ebp = hdc bmp

    invoke  SelectObject,ebp,[aAboutFree.hbmpAbout];ebp = hdc bmp
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       ;Подготовим участок который нужно скапировать
       mov  eax,[edi+RECT.left];edi = !aIntersectRect
       mov  ecx,[edi+RECT.top];edi = !aIntersectRect
       mov  edx,[edi+RECT.right];edi = !aIntersectRect
       mov  edi,[edi+RECT.bottom];edi = !aIntersectRect
       sub  edx,eax;Ширина участка который копируем
       sub  edi,ecx;Высота участка который копируем
       sub  eax,ABOUT_BORDER;Координаты на bmp начала откуда будем капировать
       sub  ecx,SYSTAB_CONTROL_Height+ABOUT_BORDER;Координаты на bmp начала откуда будем капировать

    ;Для того что бы фон не был виден на окне используем эту функцию
    ;0xFFFFFF = RGB - белый цвет фон нашего bmp, тем самым его делоем пррозрачным
    invoke  TransparentBlt,ebx,[!aIntersectRect.left],[!aIntersectRect.top],edx,edi,ebp,eax,ecx,edx,edi,0xFFFFFF;ebx = hdc current;ebp = hdc bmp
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

    invoke  DeleteDC,ebp;ebp = hdc bmp
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Рисуем текст
;ebx = hdc current;esi = PAINTSTRUCT.rcPaint
;-------------------------------------------
.PaintAboutDrawNameVer:
       ;Подготовим
       mov  edi,aAboutRectPaint.NameVer;edi = aAboutRectPaint.NameVer

     ;Проверим нужно ли рисовать текст  sNameMainWin
     ;Проверяем пересекаеться ли RECT в !lprcSrc1 с массивам RECT в !lprcSrc2;!nCount - количество в RECT массиве;Если eax = 0 тогда пересекаються
   stdcall  RectInRectHL,edi,esi,1;edi = aAboutRectPaint.NameVert;esi = PAINTSTRUCT.rcPaint
      test  eax,eax
       jnz  .PaintAboutDrawCopyright;esi = PAINTSTRUCT.rcPaint;ebx = hdc current;COLOR_BTNTEXT;[hfont30SEMIBOLD]

    ;Рисуем текст
    ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = aAboutRectPaint.NameVer
    invoke  ExtTextOutW,ebx,ABOUT_BORDER+ABOUT_BMP_Width_Height,[edi+RECT.top],0,0,sNameMainWin,(sizeof.sNameMainWin-4)/2,0;ebx = hdc current;edi = aAboutRectPaint.NameVer
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Рисуем текст
;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;COLOR_BTNTEXT;[hfont30SEMIBOLD]
;-------------------------------------------
.PaintAboutDrawCopyright:

     ;Проверим нужно ли рисовать текст  sCopyright
     ;Проверяем пересекаеться ли RECT в !lprcSrc1 с массивам RECT в !lprcSrc2;!nCount - количество в RECT массиве;Если eax = 0 тогда пересекаються
   stdcall  RectInRectHL,aAboutRectPaint.Copyright,esi,1;esi = PAINTSTRUCT.rcPaint
      test  eax,eax
       jnz  .PaintAboutDrawEmail;esi = PAINTSTRUCT.rcPaint;ebx = hdc current;COLOR_BTNTEXT;[hfont30SEMIBOLD]

    ;Рисуем текст
    ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint
    invoke  ExtTextOutW,ebx,ABOUT_BORDER+ABOUT_BMP_Width_Height,ABOUT_BORDER+ABOUT_BMP_Width_Height+SYSTAB_CONTROL_Height,0,0,sCopyright,sizeof.sCopyright/2,0;ebx = hdc current
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


;Рисуем текст
;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;COLOR_BTNTEXT;[hfont30SEMIBOLD]
;-------------------------------------------
.PaintAboutDrawEmail:
     ;Подготовим
       mov  ebp,[SelectObject];ebp = [SelectObject]
       mov  edi,aSysStock;edi = SYS_STOCK

     ;Установим шрифт
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;ebp = [SelectObject];edi = SYS_STOCK
   stdcall  ebp,ebx,[edi+SYS_STOCK.hfont];ebx = hdc current;ebp = [SelectObject];edi = SYS_STOCK
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

    ;Выбирим стандартную кисть
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;ebp = [SelectObject];edi = SYS_STOCK
   stdcall  ebp,ebx,[edi+SYS_STOCK.Brush];ebx = hdc current;ebp = [SelectObject];edi = SYS_STOCK
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

     ;Установим цвет текста
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = SYS_STOCK
    invoke  SetTextColor,ebx,[edi+SYS_STOCK.rgbHOTLIGHT];ebx = hdc current;edi = SYS_STOCK
            _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

     ;Установим цвет кисть
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = SYS_STOCK
    invoke  SetDCBrushColor,ebx,[edi+SYS_STOCK.rgbHOTLIGHT];ebx = hdc current;edi = SYS_STOCK
            _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

       ;Подготовим
       mov  edi,aAboutRectPaint.Email;edi = aAboutRectPaint.Email

     ;Проверим нужно ли рисовать текст  sNameMainWin
     ;Проверяем пересекаеться ли RECT в !lprcSrc1 с массивам RECT в !lprcSrc2;!nCount - количество в RECT массиве;Если eax = 0 тогда пересекаються
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = aAboutRectPaint.Email
   stdcall  RectInRectHL,edi,esi,1;edi = aAboutRectPaint.Email;esi = PAINTSTRUCT.rcPaint
      test  eax,eax
       jnz  .PaintAboutDrawRegistry;esi = PAINTSTRUCT.rcPaint;ebx = hdc current;COLOR_HOTLIGHT;[hfont];COLOR_HOTLIGHT brush

       ;Подготовим
       mov  ebp,aAboutZonaButton.Email;ebp = ABOUT_ARRAY_ZONA

    ;Рисуем текст
    ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = aAboutRectPaint.Email;ebp = ABOUT_ARRAY_ZONA
    invoke  ExtTextOutW,ebx,[ebp+ABOUT_ARRAY_ZONA.StringLeft],[edi+RECT.top],0,0,szInfoEmail,(sizeof.szInfoEmail-2)/2,0;ebx = hdc current;edi = aAboutRectPaint.Email;ebp = ABOUT_ARRAY_ZONA
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

    ;Рисуем стрелочку по координатам в массиве POINT
    ;На входе:;!hDc - контекст устройства где рисуем;!lpRect - указатель на RECT и массив из точек следующий за ним
    ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;ebp = ABOUT_ARRAY_ZONA
   stdcall  PaintArrow,ebx,ebp;ebx = hdc current;ebp = ABOUT_ARRAY_ZONA

;Рисуем текст
;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;COLOR_HOTLIGHT;[hfont];COLOR_HOTLIGHT brush
;-------------------------------------------
.PaintAboutDrawRegistry:
       ;Подготовим
       mov  edi,aAboutRectPaint.Registry;edi = aAboutRectPaint.Registry

     ;Проверим нужно ли рисовать текст  sNameMainWin
     ;Проверяем пересекаеться ли RECT в !lprcSrc1 с массивам RECT в !lprcSrc2;!nCount - количество в RECT массиве;Если eax = 0 тогда пересекаються
     ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = aAboutRectPaint.Registry
   stdcall  RectInRectHL,edi,esi,1;edi = aAboutRectPaint.Registry;esi = PAINTSTRUCT.rcPaint
      test  eax,eax
       jnz  .PaintAboutEndPaint;esi = PAINTSTRUCT.rcPaint

       ;Подготовим
       mov  ebp,aAboutZonaButton.Registry;ebp = ABOUT_ARRAY_ZONA

    ;Рисуем текст
    ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;edi = aAboutRectPaint.Registry;ebp = ABOUT_ARRAY_ZONA
    invoke  ExtTextOutW,ebx,[ebp+ABOUT_ARRAY_ZONA.StringLeft],[edi+RECT.top],0,0,szInfoRegUrl,(sizeof.szInfoRegUrl-2)/2,0;ebx = hdc current;edi=aAboutRectPaint.Registry;ebp=ABOUT_ARRAY_ZONA
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

    ;Рисуем стрелочку по координатам в массиве POINT
    ;На входе:;!hDc - контекст устройства где рисуем;!lpRect - указатель на RECT и массив из точек следующий за ним
    ;ebx = hdc current;esi = PAINTSTRUCT.rcPaint;ebp = ABOUT_ARRAY_ZONA
   stdcall  PaintArrow,ebx,ebp;ebx = hdc current;ebp = ABOUT_ARRAY_ZONA

;Завершаем
;esi = PAINTSTRUCT.rcPaint
.PaintAboutEndPaint:
;-------------------------------------------
       ;Подготовим
       sub  esi,PAINTSTRUCT.rcPaint;esi = PAINTSTRUCT

    invoke  EndPaint,[!Hwnd],esi;esi = PAINTSTRUCT
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       jmp  .proc_null


;Сдесь будем обрабатывать все действия привязанные к клавишам и переданые для выполнения вторым потоком
;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = lp Action Date - освободить потом надо память
;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------------------------------------------------------
.WM_APP_ACTION:
       mov  esi,edx;edx = !Lparam

       cmp  ecx,VK_FILE_OPEN;ecx = !Wparam
        je  .VK_FILE_OPEN;ebp = Хендл окна;ebx = ноль;esi = lp Action Date - освободить потом надо память

       cmp  ecx,VK_OFFUNIV;ecx = !Wparam
        je  .VK_OFFUNIV;ebp = Хендл окна;ebx = ноль

       cmp  ecx,VK_TESTCRC32;ecx = !Wparam
        je  .VK_TESTCRC32;ebp = Хендл окна;ebx = ноль

       jmp  .proc_null

          ;Будем запускать файл с помощью оболочки и с действием по умолчанию
          ;ebx = ноль;esi = lp Action Date - освободить потом надо память
          ;----------------------------------------------
         .VK_FILE_OPEN:
                    ;Так как тут только одна строка указывающая файл для запуска то esi = lp Action Date указывает иммено на ее
                    ;Подготовим
                    mov  edx,aShellExecute;edx = aShellExecute
                    mov  [edx+SHELLEXECUTEINFOW.lpFile],esi;edx = aShellExecute;esi = lp Action Date - освободить потом надо память
                 invoke  ShellExecuteExW,edx;edx = aShellExecute

                 invoke  HeapFree,[hheap],ebx,esi;esi = lp Action Date - освободить потом надо память;ebx = ноль
                         _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                    jmp  .proc_null

            ;С помощью WM_SYSCOMMAND выключил экран
            ;ebp = Хендл окна
            ;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam - не используеться
            ;----------------------------------------------
           .VK_OFFUNIV:
                 invoke  PostMessageW,ebp,WM_SYSCOMMAND,SC_MONITORPOWER,2;ebp = Хендл окна
                         _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                    jmp  .proc_null

          ;Проверим целостность куска программы проверкой приманкой
          ;----------------------------------------------
          ;ebp = Хендл окна;ebx = ноль
         .VK_TESTCRC32:
                ;Начнем считать CRC32c  защищеных участков
                stdcall  CRC32cHLEmpty,Start.lpEmptyGuardianStart,WindowProc.lpEmptyGuardianEnd - Start.lpEmptyGuardianStart,-1
                    cmp  eax,[iconstCRC32cGuardianEmpty]
                     je  .proc_null

                    inc  [iBoolMessageReg]
                    jmp  .Exit

;Проверяем сообщения посылаемые контролами через WM_NOTIFY
;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------------------------------------------------------
     .WM_NOTIFY:
             mov  ebp,[edx+NMHDR.hwndFrom];ebp = NMHDR.hwndFrom
             mov  eax,[edx+NMHDR.idFrom];eax = NMHDR.idFrom
             mov  ecx,[edx+NMHDR.code];ecx = NMHDR.code

             cmp  eax,SYSTAB_CONTROL_Id
              je  .SYSTAB_CONTROL_Id;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam ;ecx = NMHDR.code
             cmp  eax,LIST_VIEW_Id
              je  .LIST_VIEW_Id;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam ;ecx = NMHDR.code
             jmp  .proc_null





                ;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam;ecx = NMHDR.code
                  .LIST_VIEW_Id:
                             cmp  ecx,NM_DBLCLK
                              je  .NM_DBLCLK_Listview
                             cmp  ecx,NM_RCLICK
                              je  .NM_RCLICK_Listview
                             cmp  ecx,NM_CLICK
                              je  .NM_CLICK_Listview
                             cmp  ecx,LVN_KEYDOWN
                              je  .LVN_KEYDOWN
                             jmp  .proc_null


                    ;Отлавливаем нажатия клавиш на итемах
                    ;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam
                    .LVN_KEYDOWN:
                            movzx  edi,[edx+LVKEYDOWN.wVKey]
                              mov  eax,.VK_DELETE;кнопка Del
                              mov  ecx,.VK_RETURN;кнопка Enter
                              mov  edx,.VK_APPS;кнопка Menu
                              xor  esi,esi

                             ;проверяем была нажата нужная клавиша или нет
                              cmp  edi,VK_DELETE;кнопка Del
                            cmove  esi,eax
                              cmp  edi,VK_DECIMAL;кнопка Del
                            cmove  esi,eax
                              cmp  edi,VK_RETURN;кнопка Enter
                            cmove  esi,ecx
                              cmp  edi,VK_APPS;кнопка Menu
                            cmove  esi,edx


                             test  esi,esi;esi = jmp
                               jz  .proc_null

                           ;определяем итем с фокусам если такой есть вообще
                           invoke  SendMessageW,ebp,LVM_GETNEXTITEM,-1,LVNI_FOCUSED;ebp = NMHDR.hwndFrom
                             test  eax,eax
                               js  .proc_null;проверяем если -1 значит щелчек не по итему - так как итемов так много не будет то проверяем просто на отрицательное число

                              mov  ebx,eax;ebx = iItem

                              ;находим адрес структуры выбранного итема
                              mov  edi,aListViewItem

                              @@:
                              dec  eax
                               js  @f

                              mov  edi,[edi+ITEMLISTVIEW.lpNextItem]
                              jmp  @b
                              @@:;edi = выбраная структура item

                              jmp  esi;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item


                ;При нажатии левай клавиши мыши по значку итема делаем этот итем активным или не активным что отабражаеться с помощью image
                ;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam
              .NM_CLICK_Listview:
                               or  ebx,[edx+NMITEMACTIVATE.iItem];ebx = iItem
                               js  .proc_null;проверяем если -1 значит щелчек не по итему - так как итемов так много не будет то проверяем просто на отрицательное число

                              ;находим адрес структуры выбранного итема
                              mov  ecx,ebx
                              mov  edi,aListViewItem

                              @@:
                              dec  ecx
                               js  @f

                              mov  edi,[edi+ITEMLISTVIEW.lpNextItem]
                              jmp  @b
                              @@:;edi = выбраная структура item

                              lea  esi,[!aStateItemImage];esi = !aStateItemImage POINT идет первым в этой структуре поэтому все работает как надо
                              cmp  [edi+ITEMLISTVIEW.vkCodeItemOne],0;проверяем если "пустой" итем то нечего не делаем
                               je  .proc_null

                           invoke  GetCursorPos,esi;esi = !aStateItemImage POINT идет первым в этой структуре поэтому все работает как надо
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                           invoke  ScreenToClient,ebp,esi;ebp = NMHDR.hwndFrom;esi = !aStateItemImage POINT идет первым в этой структуре поэтому все работает как надо
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                           ;проверяем щелчек произведеный на итеме где был именно сделан
                           invoke  SendMessageW,ebp,LVM_SUBITEMHITTEST,0,esi;ebp = NMHDR.hwndFrom;esi = !aStateItemImage POINT идет первым в этой структуре поэтому все работает как надо
                                   _LastErrorNotSigned;Если не минус возращает функция то ошибки нет

                             test  [esi+LVHITTESTINFO.flags],LVHT_ONITEMSTATEICON
                               jz  .proc_null;если не на image то не чего не делаем

                              ;(IMAGE_LIST_STATE_QUES_Id+1) shl 12 + 1 - так как тут идет индекс  не от нуля а от 1 в отличи от List Image
                              mov  eax,(IMAGE_LIST_STATE_NOTCHECK_Id+1) shl 12
                              mov  ecx,(IMAGE_LIST_STATE_CHECK_Id+1) shl 12

                              lea  edx,[!aStateItem];edx = LVITEMW
                              cmp  ecx,[edi+ITEMLISTVIEW.stateItem];edi = выбраная структура item
                           cmovne  eax,ecx

                              mov  [edx+LVITEMW.mask],LVIF_STATE
                              mov  [edx+LVITEMW.iItem],ebx;ebx = iItem
                              mov  [edx+LVITEMW.iSubItem],0
                              mov  [edx+LVITEMW.state],eax
                              mov  [edx+LVITEMW.stateMask],-1

                             ;с помощью lock гарантируме что  будет атомарный доступ и другой поток в это время эту переменную не прочитает
                             lock  xchg [edi+ITEMLISTVIEW.stateItem],eax;edi = выбраная структура item
                           ;изменяем image итема
                           invoke  SendMessageW,ebp,LVM_SETITEMW,0,edx;ebp = NMHDR.hwndFrom;edx = LVITEMW
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                              jmp  .proc_null





                         ;Обрабатываем нажатие клавиши "Menu"
                         ;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item
                        .VK_APPS:;кнопка Menu

                              ;Определяем расположение и размер итема на который имеет сейчас фокус
                              lea  esi,[!aVkAppsRect];esi = !aVkAppsRect
                              mov  [esi+RECT.left],LVIR_LABEL
                           invoke  SendMessageW,ebp,LVM_GETITEMRECT,ebx,esi;ebp = NMHDR.hwndFrom;ebx = iItem ;esi = !aVkAppsRect
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                           invoke  ClientToScreen,ebp,esi;ebp = NMHDR.hwndFrom;esi = !aVkAppsRect
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                              jmp  .next_VK_APPS;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item;esi = POINT

                ;Запускаем меню для запуска действий с List View
                ;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam
             .NM_RCLICK_Listview:
                               or  ebx,[edx+NMITEMACTIVATE.iItem];ebx = iItem
                               js  .proc_null;проверяем если -1 значит щелчек не по итему - так как итемов так много не будет то проверяем просто на отрицательное число


                              xor  esi,esi;для определения сюда попали с помощью мыши или кнопки "Menu" если = 0 то с помощью мыши


                   ;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item ;esi = POINT
                   ;находим адрес структуры выбранного итема
                   .next_VK_APPS:
                              mov  ecx,ebx
                              mov  edi,aListViewItem

                              @@:
                              dec  ecx
                               js  @f

                              mov  edi,[edi+ITEMLISTVIEW.lpNextItem]
                              jmp  @b
                              @@:;edi = выбраная структура item


                              ;Проверяем когда нужно активировать а когда нет пункт меню для итемав
                              ;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item ;esi = POINT или определитель как сюда попали от клавы или от мыши
                              mov  edx,aMenuStateChange;edx = aMenuStateChange
                              xor  eax,eax;eax = 0
                              mov  ecx,MFS_DISABLED;ecx = fState

                              cmp  [edi+ITEMLISTVIEW.vkCodeItemOne],eax;eax = 0
                           cmovne  ecx,eax;ecx = fState;eax = 0 MFS_ENABLED = 0

                              mov  [edx+MENUITEMINFOW.fMask],MIIM_STATE;edx = aMenuStateChange

                              cmp  [aMenuState.Item],ecx;ecx = fState
                               je  .NotChangeStateMenuItem



                           ;Активирруем или деактивируем пункт меню в зависимости от состояния сохраненого в переменной iStateMenuItem
                              mov  [aMenuState.Item],ecx;ecx = fState
                              mov  [edx+MENUITEMINFOW.fState],ecx;ecx = fState;edx = aMenuStateChange
                           invoke  SetMenuItemInfoW,[hmenu],MENU_DELETE_ITEM_Id,eax,edx;edx = !aMenuStateChange;eax = 0
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                           invoke  SetMenuItemInfoW,[hmenu],MENU_CHANGED_ITEM_Id,0,aMenuStateChange
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


           ;Проверяем когда нужно активировать а когда нет пункт меню для групп
           ;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item ;esi = POINT или определитель как сюда попали от клавы или от мыши
          .NotChangeStateMenuItem:
                              mov  edx,aMenuStateChange;edx = aMenuStateChange
                              xor  eax,eax;eax = 0
                              mov  ecx,MFS_DISABLED;ecx = fState

                              cmp  [edi+ITEMLISTVIEW.idGroup],eax;eax = 0
                           cmovne  ecx,eax;ecx = fState;eax = 0 MFS_ENABLED = 0

                              cmp  [aMenuState.Group],ecx;ecx = fState
                               je  .NotChangeStateMenuGroup

                           ;Активирруем или деактивируем пункт меню в зависимости от состояния сохраненого в переменной iStateMenuGroup
                              mov  [aMenuState.Group],ecx;ecx = fState
                              mov  [edx+MENUITEMINFOW.fState],ecx;ecx = fState;edx = !aMenuStateChange
                           invoke  SetMenuItemInfoW,[hmenu],MENU_DELETE_GROUP_Id,eax,edx;edx = !aMenuStateChange;eax = 0
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                           invoke  SetMenuItemInfoW,[hmenu],MENU_CHANGED_GROUP_Id,0,aMenuStateChange
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

         ;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item ;esi = POINT или определитель как сюда попали от клавы или от мыши
        .NotChangeStateMenuGroup:
                             test  esi,esi;для определения сюда попали с помощью мыши или кнопки "Menu" если = 0 то с помощью мыши
                              jnz  .NotMouse

                              lea  esi,[!aTrackPopupAddItem];esi =[!aTrackPopupAddItem]
                           invoke  GetCursorPos,esi;esi =[!aTrackPopupAddItem]
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                       .NotMouse:
                           invoke  TrackPopupMenuEx,[hmenuSubAddItem],TPM_NONOTIFY or TPM_RETURNCMD,[esi+POINT.x],[esi+POINT.y],[hwnd],0;esi =[!aTrackPopupAddItem]
                              cmp  eax,MENU_DELETE_ITEM_Id
                               je  .MENU_DELETE_ITEM_Id;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item
                              cmp  eax,MENU_ADD_ITEM_Id
                               je .MENU_ADD_ITEM_Id;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item
                              cmp  eax,MENU_CHANGED_ITEM_Id
                               je .MENU_CHANGED_ITEM_Id;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item
                              cmp  eax,MENU_ADD_GROUP_Id
                               je  .MENU_ADD_GROUP_Id;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item
                              cmp  eax,MENU_DELETE_GROUP_Id
                               je  .MENU_DELETE_GROUP_Id;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item
                              cmp  eax,MENU_CHANGED_GROUP_Id
                               je  .MENU_CHANGED_GROUP_Id

                              jmp  .proc_null

;Кантролируем минимальный размер окна и устанавливаем таймер на проверку взлома
;ebp = Хендл окна;ebx = ноль;edx = !Lparam
.WM_WINDOWPOSCHANGING:
       ;проверяем изменился ли размер основного окна если нет  - проверять на минимальный размер не надо
       ;ebp = Хендл окна;ebx = ноль;edx = !Lparam
       test  [edx+WINDOWPOS.flags],SWP_NOSIZE;edx = !Lparam
       jnz  .proc_null

       ;Проконтролируем если меньше установленой минимальной высоты и ширины то - каректируем
       ;ebp = Хендл окна;ebx = ноль;edx = !Lparam
       mov  eax,[edx+WINDOWPOS.cx];edx = !Lparam
       mov  ecx,[edx+WINDOWPOS.cy];edx = !Lparam
       mov  esi,BUTTON_OPTIONS_Width+BUTTON_EXIT_Width+BUTTON_ACTIVSUSP_Width+COMBOBOX_LANG_Width+18
       mov  edi,300
       cmp  eax,esi
     cmovb  eax,esi
       cmp  ecx,edi
     cmovb  ecx,edi
       mov  [edx+WINDOWPOS.cx],eax;edx = !Lparam
       mov  [edx+WINDOWPOS.cy],ecx;edx = !Lparam

       ;проверяем надо ли установить таймер, не равно нулю - уже установлен
       ;ebp = Хендл окна;ebx = ноль
       cmp  [iTimerActivation],ebx;ebx = ноль
       jne  .proc_null

       ;Что бы явно не было видно при диасемблери и отладчики что устанавливаем таймер
       ;ebp = Хендл окна
       mov  edx,Start.GetProcessHeap
       mov  [iTimerActivation],esp
       mov  eax,[constSetTimer+edx]
       add  eax,2;eax = [SetTimer];Каректируем что бы пропустить mov  edi,edi - ну авдруг усложнит

   ;Установим таймер
   ;ebp = Хендл окна
   stdcall  eax,ebp,TimerProcGuardian,constTimeGuardianMin,TimerProcGuardian;eax = [SetTimer];ebp = Хендл окна
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       jmp  .proc_null


            ;Будем удалять выбранную группу и все итемы входящие в нее
            ;edi = выбраная структура item
           .MENU_DELETE_GROUP_Id:
                              ;Найдем с помощью итема изменяемую группу
                              mov  ebx,[edi+ITEMLISTVIEW.idGroup];ebx = [idGroup]
                              mov  ebp,aListViewGroup;ebp = aListViewGroup

                              @@:
                              cmp  [ebp+GROUPLISTVIEW.idGroup],ebx;ebx = [idGroup]
                               je  @f

                              mov  ebp,[ebp+GROUPLISTVIEW.lpNextGroup]
                              jmp  @b

                              @@:;ebp = GROUPLISTVIEW- изменяемая группа

                           invoke  AcquireSRWLockExclusive,aRWKeyMap;Так как эти даные могут читаться вторым потоком

                           ;Уберем из структуры групп удаляемую группу, в этой структуре сохраним данные:
                           ;первый dd - указатель на следующую структуру от которой надо буде освободить память
                           ;ebx = [idGroup];ebp = GROUPLISTVIEW- изменяемая группа
                              mov  eax,[ebp+GROUPLISTVIEW.lpPrevGroup];ebp = GROUPLISTVIEW- изменяемая группа
                              mov  ecx,[ebp+GROUPLISTVIEW.lpNextGroup];ebp = GROUPLISTVIEW- изменяемая группа
                              mov  [eax+GROUPLISTVIEW.lpNextGroup],ecx
                              mov  [ecx+GROUPLISTVIEW.lpPrevGroup],eax

                           ;Ищем структуры итемав с id удаляемай группы и исключаем эти структуры итемав, в этой структуре сохраним данные:
                           ;+0 первый dd - указатель на следующую структуру от которой надо буде освободить память
                           ;+4 второй dd - индекс итема который надо удалить
                           ;ebx = [idGroup];ebp = GROUPLISTVIEW- изменяемая группа
                              mov  edi,aListViewItem;edi = ITEMLISTVIEW
                              xor  edx,edx;index item
                              mov  esi,ebp;ebp = GROUPLISTVIEW

                              @@:
                              mov  edi,[edi+ITEMLISTVIEW.lpNextItem];edi = ITEMLISTVIEW
                              inc  edx
                              cmp  edi,aListViewItem
                               je  @f

                              cmp  [edi+ITEMLISTVIEW.idGroup],ebx;ebx = [idGroup];edi = ITEMLISTVIEW
                              jne  @b


                              mov  [esi],edi;сохраняем указатель на следующую структуру от которой надо буде освободить память
                              mov  [esi+4],edx;index item;сохраняем  итема для удаления

                              ;изменяем связи структур с учетам удаленнай
                              mov  eax,[edi+ITEMLISTVIEW.lpPrevItem]
                              mov  ecx,[edi+ITEMLISTVIEW.lpNextItem]
                              mov  esi,edi;благодаря этому всегда esi = указатель на последнюю структуру удаляемый итемов
                              mov  [eax+ITEMLISTVIEW.lpNextItem],ecx
                              mov  [ecx+ITEMLISTVIEW.lpPrevItem],eax
                              jmp  @b


                              @@:;все нужные структуры успешно изменены

                           invoke  ReleaseSRWLockExclusive,aRWKeyMap;Так как эти даные могут читаться вторым потоком

                           ; Удаляем из List Vieww группу
                           ;ebx = [idGroup];ebp = GROUPLISTVIEW- изменяемая группа;esi = указатель на последнюю структуру удаляемый итемов
                              mov  edi,[aArrayHwndFont.hwndListView];edi = [aArrayHwndFont.hwndListView]
                           invoke  SendMessageW,edi,LVM_REMOVEGROUP,ebx,0;ebx = [idGroup];edi = [aArrayHwndFont.hwndListView]
                                   _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

                           ; Удаляем из List Vieww итемы и освобождаем память
                           ;edi = [aArrayHwndFont.hwndListView];esi = указатель на последнюю структуру удаляемый итемов;ebp = удаляемые структуры
                              xor  ebx,ebx;ebx = для карактировки индексов итемав, будем считать сколько удалили и каректировать, уменьшая на это число ранее сохраненый индекс

               .LoopGroupDelItem:
                              mov  eax,ebx;ebx = для карактировки индексов итемав
                              dec  ebx;ebx = для карактировки индексов итемав
                              add  eax,[ebp+4];+4 второй dd - индекс итема который надо удалить

                           invoke  SendMessageW,edi,LVM_DELETEITEM,eax,0;eax = index item;ebx = [idGroup];edi = [aArrayHwndFont.hwndListView]
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                              mov  eax,ebp
                              mov  ebp,[ebp];+0 первый dd - указатель на следующую структуру от которой надо буде освободить память

                           invoke  HeapFree,[hheap],0,eax
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                              dec  [iCountIndexItem];коректируем для создание будущих итемов

                              cmp  ebp,esi;esi = указатель на последнюю структуру удаляемый итемов
                              jne  .LoopGroupDelItem

                           ;Так как в первай структуре(группе) храниться индекс итема то во время освобождения памяти с под группы
                           ;в то же время удаляем из List Vieww итем и таким образом получаеться что последняя структура с памяти не освобождаеться
                           ;Поэтому тут и освобождаем
                           invoke  HeapFree,[hheap],0,esi
                              jmp  .proc_null




           ;Изменяем путь в выбранай группе
           ;edi = выбраная структура item
          .MENU_CHANGED_GROUP_Id:
                              ;Найдем с помощью итема изменяемую группу
                              mov  eax,[edi+ITEMLISTVIEW.idGroup];eax = [idGroup]
                              mov  ebp,aListViewGroup;ebp = aListViewGroup

                              @@:
                              cmp  [ebp+GROUPLISTVIEW.idGroup],eax;eax = [idGroup]
                               je  @f

                              mov  ebp,[ebp+GROUPLISTVIEW.lpNextGroup]
                              jmp  @b

                              @@:;ebp = GROUPLISTVIEW- изменяемая группа

                              ;Копируем строку для GetOpenFileNameW, что бы диалог появился в том месте куда указывает путь изменяемой группы
                              mov  edi,aIniFileData.szOpenExeBufer
                              mov  esi,[ebp+GROUPLISTVIEW.lpszNameGroup]
                              mov  ecx,[ebp+GROUPLISTVIEW.lengthNameGroup]
                              cld
                              rep  movsw


                              ;Вызываем стандартный диалог для открытия что бы пользователь выбрал нужное приложение
                              mov  esi,aOpenFileExe;esi = aOpenFileExe
                          stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_OPEN_EXE_Filtr

                              mov  ecx,[hwnd]
                              mov  [esi+OPENFILENAMEW.lpstrFile],aIniFileData.szOpenExeBufer
                              mov  [esi+OPENFILENAMEW.hwndOwner],ecx
                              mov  [esi+OPENFILENAMEW.lpstrFilter],eax
                           invoke  GetOpenFileNameW,esi;esi = aOpenFileExe
                             test  eax,eax
                               jz  .NotOpenFile

                            ;Проверяем изменился путь или остался таким же
                            movzx  ebx,word[esi+OPENFILENAMEW.nFileExtension];esi = aOpenFileEx
                              add  ebx,4;учитываем 0 = +1 и расширение а это + 3 - в любом случае это все в символах, а не байтах;ebx = размер в символах пути к файлу включая ноль

                              cmp  ebx,[ebp+GROUPLISTVIEW.lengthNameGroup]
                              jne  .NextTestChangedGroup;ebp = GROUPLISTVIEW- изменяемая группа;ebx = размер в символах пути к файлу включая ноль

                              mov  edi,[ebp+GROUPLISTVIEW.lpszNameGroup]
                              mov  esi,aIniFileData.szOpenExeBufer
                              mov  ecx,ebx;ebx = размер в символах пути к файлу включая ноль
                             cld
                             repe  cmpsw
                               je  .proc_null;путь не был изменен поэтому нечего не делаем


           ;Проверяем есть у нас группа с таким же путем к испольняемому файлу  или нет
           ;ebp = GROUPLISTVIEW- изменяемая группа;ebx = размер в символах пути к файлу включая ноль
           .NextTestChangedGroup:
                              mov  edx,aListViewGroup

                              @@:
                              cmp  [edx+GROUPLISTVIEW.lengthNameGroup],ebx;ebx = размер в символах пути к файлу включая ноль
                               je  .CompareChangeNameGroup;совпадает длина путей к файлу проверим может одинаковые

                  .ProceedChange:
                              mov  edx,[edx+GROUPLISTVIEW.lpNextGroup]
                              cmp  edx,aListViewGroup
                              jne  @b
                              jmp  .AllocMemoryChangedGroup;Одинаковых путей не нашли продолжим добавление группы

         .CompareChangeNameGroup:
                              mov  esi,[edx+GROUPLISTVIEW.lpszNameGroup]
                              mov  edi,aIniFileData.szOpenExeBufer
                              mov  ecx,ebx
                             cld
                             repe  cmpsw
                              jne  .ProceedChange

                          ;Уже такая группа есть говорим об этом пользователю
                          stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_OPEN_EXE_Already
                           invoke  MessageBoxExW,[hwnd],eax,0,MB_OK or MB_ICONINFORMATION,0
                              jmp  .proc_null

        ;Выделяем с кучи память под изменненую группу
        ;ebp = GROUPLISTVIEW- изменяемая группа;ebx = размер в символах пути к файлу включая ноль
        .AllocMemoryChangedGroup:
                              lea  eax,[ebx*2 + sizeof.GROUPLISTVIEW];ebx = размер в символах пути к файлу включая ноль
                           invoke  HeapAlloc,[hheap],0,eax
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                                    ;eax = GROUPLISTVIEW

                              ;Заполняем структуры группы
                              mov  edx,[ebp+GROUPLISTVIEW.idGroup];ebp = GROUPLISTVIEW- изменяемая группа
                              lea  edi,[eax+sizeof.GROUPLISTVIEW];edi = [lpszNameGroup]
                              mov  esi,aIniFileData.szOpenExeBufer
                              mov  ecx,ebx;ebx = размер в символах пути к файлу включая ноль
                              mov  ebx,eax;ebx = GROUPLISTVIEW
                              lea  eax,[!aChangeLVGROUP];eax = LVGROUPW

                              mov  [ebx+GROUPLISTVIEW.idGroup],edx;edx = [idGroup]
                              mov  [ebx+GROUPLISTVIEW.lengthNameGroup],ecx
                              mov  [ebx+GROUPLISTVIEW.lpszNameGroup],edi;edi = [lpszNameGroup]
                              mov  [eax+LVGROUPW.cbSize],sizeof.LVGROUPW
                              mov  [eax+LVGROUPW.mask],LVGF_HEADER
                              mov  [eax+LVGROUPW.pszHeader],edi;edi = [lpszNameGroup]

                              cld
                              rep  movsw;копируем szNameGroup на каторый будет указывать lpszNameGroup в GROUPLISTVIEW
                              mov  esi,aRWKeyMap;esi = aRWKeyMap

                           ;Устанавливаем в List View новую информацию о пути
                           invoke  SendMessageW,[aArrayHwndFont.hwndListView],LVM_SETGROUPINFO,edx,eax;edx = [idGroup];eax = LVGROUPW
                                   _LastErrorNotSigned;Если не минус возращает функция то ошибки нет

                           ;ebp = GROUPLISTVIEW- изменяемая группа;ebx = GROUPLISTVIEW;esi = aRWKeyMap
                           invoke  AcquireSRWLockExclusive,esi;esi = aRWKeyMap ;Так как эти даные могут читаться вторым потоком

                             ;Каректируем структуры групп с учетам новой группы

                              mov  eax,[ebp+GROUPLISTVIEW.lpPrevGroup];ebp = GROUPLISTVIEW- изменяемая группа
                              mov  ecx,[ebp+GROUPLISTVIEW.lpNextGroup];ebp = GROUPLISTVIEW- изменяемая группа
                              mov  [ebx+GROUPLISTVIEW.lpPrevGroup],eax;ebx = GROUPLISTVIEW
                              mov  [ebx+GROUPLISTVIEW.lpNextGroup],ecx;ebx = GROUPLISTVIEW
                              mov  [eax+GROUPLISTVIEW.lpNextGroup],ebx;ebx = GROUPLISTVIEW
                              mov  [ecx+GROUPLISTVIEW.lpPrevGroup],ebx;ebx = GROUPLISTVIEW

                           invoke  ReleaseSRWLockExclusive,esi;esi = aRWKeyMap ;Так как эти даные могут читаться вторым потоком

                           ;Освобождаем изменямой группой занемаемую память так как ее уже заменили новой группой
                           invoke  HeapFree,[hheap],0,ebp;ebp = GROUPLISTVIEW- изменяемая группа
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                              jmp  .proc_null



              ;Добавляем новую группу - которая будет ограничивать действие клавиш на выбранное приложениее
              .MENU_ADD_GROUP_Id:

                              ;Вызываем стандартный диалог для открытия что бы пользователь выбрал нужное приложение
                              mov  esi,aOpenFileExe;esi = aOpenFileExe
                          stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_OPEN_EXE_Filtr

                              mov  ebp,aIniFileData.szOpenExeBufer;ebp = aIniFileData.szOpenExeBufer
                              mov  ecx,[hwnd]
                              mov  [esi+OPENFILENAMEW.lpstrFile],ebp;ebp = aIniFileData.szOpenExeBufer
                              mov  [esi+OPENFILENAMEW.hwndOwner],ecx
                              mov  [esi+OPENFILENAMEW.lpstrFilter],eax
                           invoke  GetOpenFileNameW,esi;esi = aOpenFileExe
                             test  eax,eax
                               jz  .NotOpenFile


                            ;Проверяем есть унас группа с таким же путем к испольняемому файлу  или нет
                            ;ebp = aIniFileData.szOpenExeBufer
                            movzx  ebx,word[esi+OPENFILENAMEW.nFileExtension];esi = aOpenFileEx
                              mov  edx,aListViewGroup
                              add  ebx,4;учитываем 0 = +1 и расширение а это + 3 - в любом случае это все в символах, а не байтах;ebx = размер в символах пути к файлу включая ноль
                              mov  eax,edx;edx = aListViewGroup;eax = aListViewGroup

                              @@:
                              cmp  [edx+GROUPLISTVIEW.lengthNameGroup],ebx;ebx = размер в символах пути к файлу включая ноль
                               je  .CompareNameGroup;совпадает длина путей к файлу проверим может одинаковые

                        .Proceed:
                              mov  edx,[edx+GROUPLISTVIEW.lpNextGroup]
                              cmp  eax,edx;edx = aListViewGroup;eax = aListViewGroup
                              jne  @b
                              jmp  .AllocMemoryGroup;Одинаковых путей не нашли продолжим добавление группы и итемов

               .CompareNameGroup:
                              mov  esi,[edx+GROUPLISTVIEW.lpszNameGroup]
                              mov  edi,ebp;ebp = aIniFileData.szOpenExeBufer
                              mov  ecx,ebx
                             cld
                             repe  cmpsw
                              jne  .Proceed

                          ;Уже такая группа есть говорим об этом пользователю
                          stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_OPEN_EXE_Already
                           invoke  MessageBoxExW,[hwnd],eax,0,MB_OK or MB_ICONINFORMATION,0
                              jmp  .proc_null


               .AllocMemoryGroup:
                            ;Выделяем с кучи память под новую группу и начальный стандартный итем группы
                              mov  edi,[hheap];edi = [hheap]
                              mov  esi,[HeapAlloc];esi = [HeapAlloc]
                          stdcall  esi,edi,0,sizeof.ITEMLISTVIEW;edi = [hheap];esi = [HeapAlloc]
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                              mov  ebp,eax;ebp = New list item struct ITEMLISTVIEW

                              lea  eax,[ebx*2 + sizeof.GROUPLISTVIEW];ebx = размер в символах пути к файлу включая ноль
                          stdcall  esi,edi,0,eax;edi = [hheap];esi = [HeapAlloc]
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                                  ;eax = GROUPLISTVIEW


;Напалняем новую структуру группы  и итема, а так же подготавливаем их структуру к вставки в List View
;ebp = New list item struct ITEMLISTVIEW;eax = GROUPLISTVIEW;ebx = размер в символах пути к файлу включая ноль

                              ;Найдем idGroup ближайший свободный
                               mov  esi,aListViewGroup;esi = aListViewGroup
                               xor  ecx,ecx


                .LoopSeashidGroup:; С помощью этого поиска ищем id которые уже освободились тоесть группы были удалены если нет то возращает следующий, - тем самым всегда id актуален
                               mov  edx,esi;edx = aListViewGroup;esi = aListViewGroup
                               inc  ecx

                               @@:
                               cmp  [edx+GROUPLISTVIEW.idGroup],ecx;edx = aListViewGroup
                                je  .LoopSeashidGroup

                               mov  edx,[edx+GROUPLISTVIEW.lpNextGroup];edx = aListViewGroup
                               cmp  edx,esi;edx = aListViewGroup;esi = aListViewGroup
                               jne  @b

                              ;ecx = idGroup
                              mov  [eax+GROUPLISTVIEW.lengthNameGroup],ebx;eax = GROUPLISTVIEW;ebx = размер в символах пути к файлу включая ноль
                              lea  edi,[eax + sizeof.GROUPLISTVIEW];eax = GROUPLISTVIEW
                              mov  esi,aIniFileData.szOpenExeBufer
                             xchg  ecx,ebx;ebx = размер в символах пути к файлу включая ноль;ecx = idGroup
                              mov  edx,edi;edx = [lpszNameGroup]
                              cld
                              rep  movsw;копируем szNameGroup на каторый будет указывать lpszNameGroup в GROUPLISTVIEW

                             xchg  ebx,eax;eax = GROUPLISTVIEW;ebx = idGroup
                              lea  ecx,[!aAddLVGROUP];ecx = !aAddLVGROUP
                              mov  [ebx+GROUPLISTVIEW.idGroup],eax;ebx = GROUPLISTVIEW;eax = idGroup
                              mov  [ebx+GROUPLISTVIEW.lpszNameGroup],edx;ebx = GROUPLISTVIEW ;edx = [lpszNameGroup]
                              mov  [ecx+LVGROUPW.cbSize],sizeof.LVGROUPW;ecx = !aAddLVGROUP
                              mov  [ecx+LVGROUPW.mask],LVGF_HEADER or LVGF_GROUPID;ecx = !aAddLVGROUP
                              mov  [ecx+LVGROUPW.pszHeader],edx ;ecx = !aAddLVGROUP
                              mov  [ecx+LVGROUPW.iGroupId],eax  ;ecx = !aAddLVGROUP
                              mov  [!aAddItemCommmon.iGroupId],eax
                              mov  [ebp+ITEMLISTVIEW.idGroup],eax;ebp = New list item struct ITEMLISTVIEW

                              ;Вставляем группу
                              mov  esi,[SendMessageW];esi = [SendMessageW]
                              mov  edi,[aArrayHwndFont.hwndListView];edi = [aArrayHwndFont.hwndListView]
                          stdcall  esi,edi,LVM_INSERTGROUP,-1,ecx;esi = [SendMessageW] ;edi = [aArrayHwndFont.hwndListView]
                                   _LastErrorNotSigned;Если не минус возращает функция то ошибки нет

                              lea  edx,[!aAddItemCommmon];edx = !aAddItemCommmon
                              xor  ecx,ecx;ecx = 0

                              mov  [ebp+ITEMLISTVIEW.stateItem],(IMAGE_LIST_STATE_QUES_Id+1) shl 12;ebp = ITEMLISTVIEW
                              mov  [ebp+ITEMLISTVIEW.vkCodeItemOne],ecx;ebp = ITEMLISTVIEW
                              mov  [ebp+ITEMLISTVIEW.vkCodeItemTwoThree],ecx;Нужно для для проверки на схожесть клавиш в других итемах, чтобы не было савпадений - замкнутага цикла
                              mov  [ebp+ITEMLISTVIEW.vkCodeSubOne],ecx;ebp = ITEMLISTVIEW
                              mov  [ebp+ITEMLISTVIEW.vkCodeSubTwoThree],ecx;Нужно для для проверки на схожесть клавиш в других итемах, чтобы не было савпадений - замкнутага цикла

                              mov  [edx+LVITEMW.pszText],szEmptyItem
                              mov  eax,[iCountIndexItem]
                              mov  [edx+LVITEMW.stateMask],-1
                              mov  [edx+LVITEMW.mask],LVIF_TEXT or LVIF_STATE or LVIF_GROUPID
                              mov  [edx+LVITEMW.iSubItem],ecx;  - индекс того столбика в которрый Item вставляем в нашем случае он всегда = 0 так как в первый вставляем чей индекс = 0
                              mov  [edx+LVITEMW.state],(IMAGE_LIST_STATE_QUES_Id+1) shl 12
                              mov  [edx+LVITEMW.iItem],eax
                              inc  [iCountIndexItem];контролируем

                              ;вставляем итем
                          stdcall  esi,edi,LVM_INSERTITEMW,ecx,edx;esi = [SendMessageW] ;edi = [aArrayHwndFont.hwndListView];edx = !aAddItemCommmon;ecx = 0
                                   _LastErrorNotSigned;Если не минус возращает функция то ошибки нет

                              lea  edx,[!aAddItemCommmon];edx = !aAddItemCommmon
                              mov  [edx+LVITEMW.mask],LVIF_TEXT; or LVIF_STATE
                              inc  [edx+LVITEMW.iSubItem];LIST_VIEW_COLUM_NEW_Id-индекс того столбика в которрый SubItem вставляем в нашем случае он всегда =1 так как 2 только столбика есть

                              ;вставляем субитем
                           stdcall  esi,edi,LVM_SETITEMW,0,edx;esi = [SendMessageW] ;edi = [aArrayHwndFont.hwndListView] ;edx = !aAddItemCommmon
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

;Вставляем структуру групы и итема в конец двухсвязных списков
;ebp = New list item struct ITEMLISTVIEW;ebx = GROUPLISTVIEW

                              mov  esi,aRWKeyMap;esi = aRWKeyMap
                              mov  edi,aListViewGroup;edi= aListViewGroup
                           invoke  AcquireSRWLockExclusive,esi;esi = aRWKeyMap ;Так как эти даные могут читаться вторым потоком

                             ;Каректируем структуры групп с учетам новой группы
                             ;ebx = GROUPLISTVIEW
                              mov  eax,[edi+GROUPLISTVIEW.lpPrevGroup];edi= aListViewGroup
                              mov  [ebx+GROUPLISTVIEW.lpNextGroup],edi;ebx = GROUPLISTVIEWW
                              mov  [ebx+GROUPLISTVIEW.lpPrevGroup],eax;ebx = GROUPLISTVIEW
                              mov  [edi+GROUPLISTVIEW.lpPrevGroup],ebx;ebx = GROUPLISTVIEW
                              mov  [eax+GROUPLISTVIEW.lpNextGroup],ebx;ebx = GROUPLISTVIEW

                             ;Каректируем структуры итемов с учетам нового итема
                             ;ebp = New list item struct ITEMLISTVIEW
                              mov  edi,aListViewItem;edi= aListViewItem
                              mov  eax,[edi+ITEMLISTVIEW.lpPrevItem];edi= aListViewItem
                              mov  [ebp+ITEMLISTVIEW.lpNextItem],edi;ebp = New list item struct ITEMLISTVIEW
                              mov  [ebp+ITEMLISTVIEW.lpPrevItem],eax;ebp = New list item struct ITEMLISTVIEW
                              mov  [edi+ITEMLISTVIEW.lpPrevItem],ebp;ebp = New list item struct ITEMLISTVIEW
                              mov  [eax+ITEMLISTVIEW.lpNextItem],ebp;ebp = New list item struct ITEMLISTVIEW

                           invoke  ReleaseSRWLockExclusive,esi;esi = aRWKeyMap ;Так как эти даные могут читаться вторым потоком
                              jmp  .proc_null


                     ;Проверяем отменил пользователь диалог открыть файл или ошибка была; любую ошибку считаемкак слишком длинный путь о чем и сообщаем пользователю
                    .NotOpenFile:
                           invoke  CommDlgExtendedError
                             test  eax,eax;значит отменил
                               jz  .proc_null

                              xor  ebx,ebx;ebx = 0
                          stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_OPEN_EXE_LONG_Patch
                           invoke  MessageBoxExW,[hwnd],eax,ebx,MB_OK or MB_ICONINFORMATION,ebx;ebx = 0
                              mov  word[aIniFileData.szOpenExeBufer],bx;ebx = 0;в случае маленького буфера указываеться нужный размер - затираем
                              jmp  .proc_null




             ;Удаляем выбранный итем пользавателем
             ;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item
                      .VK_DELETE:;кнопка Del
                              cmp  [edi+ITEMLISTVIEW.vkCodeItemOne],0;проверяем если "пустой" итем то нечего не делаем
                               je  .proc_null

            .MENU_DELETE_ITEM_Id:

                           invoke  SendMessageW,ebp,LVM_DELETEITEM,ebx,0;ebp = NMHDR.hwndFrom;ebx = iItem
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                           invoke  AcquireSRWLockExclusive,aRWKeyMap;Так как эти даные могут читаться вторым потоком

                              ;изменяем связи структур с учетам удаленнай
                              mov  eax,[edi+ITEMLISTVIEW.lpPrevItem]
                              mov  ecx,[edi+ITEMLISTVIEW.lpNextItem]
                              mov  [eax+ITEMLISTVIEW.lpNextItem],ecx
                              mov  [ecx+ITEMLISTVIEW.lpPrevItem],eax

                           invoke  ReleaseSRWLockExclusive,aRWKeyMap;Так как эти даные могут читаться вторым потоком

                              dec  [iCountIndexItem];контролируем

                           invoke  HeapFree,[hheap],0,edi
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                              jmp  .proc_null



                 ;Обрабатываем пункт меню "Добавить итем"- обрабатываеться схоже с случаем двойного щелчка мыши, но нужно найти начальный итем в текущей группе
                 ;ebp = NMHDR.hwndFrom;ebx = iItem;edi = выбраная структура item
               .MENU_ADD_ITEM_Id:
                              xor  ebx,ebx
                              mov  eax,[edi+ITEMLISTVIEW.idGroup]
                              mov  edi,aListViewItem
                              dec  ebx

                              @@:
                              cmp  [edi+ITEMLISTVIEW.vkCodeItemOne],0
                             sete  cl
                              cmp  [edi+ITEMLISTVIEW.idGroup],eax
                             sete  ch

                              inc  ebx
                              and  cl,ch
                              jnz  .nextMENU_ADD_ITEM_Id;edi = выбраная структура item;ebx = iItem

                              mov  edi,[edi+ITEMLISTVIEW.lpNextItem]
                              jmp  @b



                ;Добавляем или редактируем итемы
                ;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam
             .NM_DBLCLK_Listview:
                               or  ebx,[edx+NMITEMACTIVATE.iItem];ebx = iItem
                               js  .proc_null;проверяем если -1 значит щелчек не по итему - так как итемов так много не будет то проверяем просто на отрицательное число

                              ;находим адрес структуры выбранного итема
                              mov  ecx,ebx
                              mov  edi,aListViewItem

                              @@:
                              dec  ecx
                               js  @f

                              mov  edi,[edi+ITEMLISTVIEW.lpNextItem]
                              jmp  @b
                              @@:;edi = выбраная структура item

           .nextMENU_ADD_ITEM_Id:;edi = выбраная структура item;ebx = iItem
                      .VK_RETURN:;edi = выбраная структура item ;ebx = iItem;кнопка Enter
           .MENU_CHANGED_ITEM_Id:;edi = выбраная структура item;ebx = iItem

                   ;Обнуляем буфер для удерживаемых клавиш, что бы не было неожиданастей а заодно обнулим и другие переменные
                              xor  eax,eax;eax = 0
                              cld
                              mov  [aHookMinor.iKeyModif],eax;eax = 0 ;обнулим
                              mov  [aHookMinor.iKeyModif2],eax;eax = 0 ;обнулим

                              mov  edx,edi
                              lea  edi,[!aKeyPressedMap]
                              mov  ecx,constLengthKeyPressedMap
                              mov  [lpaKeyPressedMapIn],edi
                              rep  stosd

                              mov  edi,edx
                              lea  ecx,[!ddKeyMapScankode]
                              mov  [iChangeCurrentListViewItemIndex],ebx;ebx = iItem
                              mov  [lpBufferDlgKeyMapScankode],ecx
                          stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_DLG,DIALOG_KEY_MAP
                           invoke  DialogBoxIndirectParamW,[hInstance],eax,[hwnd],DlgProcAddKeys,edi;edi = выбраная структура item
                                   _LastErrorNotSigned;Если не минус возращает функция то ошибки нет

                              jmp  .proc_null



                ;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam;ecx = NMHDR.code
              .SYSTAB_CONTROL_Id:
                              cmp  ecx,TCN_SELCHANGE
                               je  .TCN_SELCHANGE;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam

                              jmp  .proc_null


                  ;ebp = NMHDR.hwndFrom;ebx = ноль;edx = !Lparam
                  ;Отрабатываем смену вкладак
                  .TCN_SELCHANGE:

                           ;Делаем не видемыми те окна которые были связаны с предытущей вкладкой
                            mov  eax,[aIniFileData.aIni.idTabCurent]
                           push  ebx;ebx = ноль;SW_HIDE = 0
                           call  dword[tbChangeTab+eax*4]; - считаем по индексу * 4 - с макраса с переходами на нужные функции

                         invoke  SendMessageW,ebp,TCM_GETCURSEL,ebx,ebx;ebp = NMHDR.hwndFrom;ebx = ноль
                            mov  [aIniFileData.aIni.idTabCurent],eax

                           ;Делаем видимыми те окна которые связаны с текущей вкладкой  эти функции так же каректирую функции перемещения окан детских
                           push  SW_SHOW;ebx = ноль;SW_HIDE = 0
                           call  dword[tbChangeTab+eax*4]; - считаем по индексу * 4 - с макраса с переходами на нужные функции


                            jmp  .proc_null


;Обрабатываем системную кнопку закрыть а так же alt + F4 в отношении  окна
;ebp = Хендл окна;ebx = ноль;ecx = !Wparam
;----------------------------------------------
     .WM_SYSCOMMAND:
                 and  ecx,0xFFF0;ecx = !Wparam; Коректируем так как первые 4 бита имеют значение только для системы
                 cmp  ecx,SC_CLOSE
                 jne  .freeJmp;все остальное отправляем DefWindowProc - на обработку что бы область caption и другое работало

              invoke  ShowWindow,ebp,ebx;ebx = ноль ;ebp = Хендл окна;SW_HIDE = null;скрываем окно
                 jmp  .proc_null





;Shell_NotifyIcon - Обрабатываем события мыши и клавиатуры над значком в трее
;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;----------------------------------------------
.WM_APP_NOTIFY_ICON:;!Wparam -  не проверяем так у нас в трее висит только одна иконка - соответственно может только от одной приходить сообщения - NOTIFY_ICON_Id
                 cmp  edx,WM_LBUTTONDBLCLK
                  je  .WM_LBUTTONDBLCLK_NOTIFY_ICON
                 cmp  edx,WM_RBUTTONDOWN
                  je  .WM_RBUTTONUP_NOTIFY_ICON
                 jmp  .proc_null


;левая кнопка дважды нажата показываем окно
       .WM_LBUTTONDBLCLK_NOTIFY_ICON:;ebp = Хендл окна;ebx = ноль
                               invoke  IsWindowVisible,ebp;ebp = Хендл окна
                                 test  eax,eax
                                   jz  .WM_LBUTTONDBLCLK_NOTIFY_ICON_SHOW

                               ;Так как окно может быть видимо но закрыто поэтому перебираем все окна с помощью
                               ;EnumWindows и если RECT окна нашего и получененых пересикаються значит только выводим на передний план
                               ;если же таких окон нет значит скрываем наше главное окно
                               invoke  EnumWindows,EnumWindowsProc,ebx;ebx - роли не играет
                                  jmp  .proc_null


   ;раз окно не видимо - значит надо показать его и вывести на передний план
  .WM_LBUTTONDBLCLK_NOTIFY_ICON_SHOW:;ebp = Хендл окна;ebx = ноль
                               invoke  SetForegroundWindow,ebp;ebp = Хендл окна
                               invoke  ShowWindow,ebp,SW_SHOW;ebp = Хендл окна
                                  jmp  .proc_null

            ;правая кнопка показываем меню на иконке
            ;ebp = Хендл окна
            .WM_RBUTTONUP_NOTIFY_ICON:
                                   ;Подготовим
                                   ;ebp = Хендл окна
                                   mov  ebx,[hmenu];ebx = hmenu
                                   mov  esi,[SetMenuItemInfoW];esi = [SetMenuItemInfoW]
                                   mov  edi,[lpMultiLanguage];edi =[lpMultiLanguage]
                                   mov  [aMenuStateChange.fMask],MIIM_STRING;Установим - общий значения для всех последующих вызавов SetMenuItemInfoW

                                ;Проверим скрыто окно или показано и взависимости от этого установим в текст
                                ;ebp = Хендл окна;ebx = hmenu ;esi = [SetMenuItemInfoW];edi =[lpMultiLanguage]
                                invoke  IsWindowVisible,ebp;ebp = Хендл окна;esi = [SetMenuItemInfoW];edi =[lpMultiLanguage]

                                   ;Если равно значит менять потребнасти нету
                                   cmp  eax,[aMenuState.NotifySnowHide]
                                    je  .NotChangeMenuShowHide;ebp = Хендл окна;ebx = hmenu ;esi = [SetMenuItemInfoW];edi =[lpMultiLanguage]

                                   ;Установим новое значение
                                   mov  [aMenuState.NotifySnowHide],eax

                                   ;Определим нужный id строки
                                   mov  ecx,STR_MENU_NOTIFY_Show
                                   mov  edx,STR_MENU_NOTIFY_Hide
                                  test  eax,eax
                                cmovnz  ecx,edx

                               ;!BaseRes = указатель на корневой каталог,!TypeRes = Id типа ресурса,IdRes - Id конечных требуемых данных;возвращает eax = указатель на конечные данные
                               ;ebp = Хендл окна;ebx = hmenu;esi = [SetMenuItemInfoW];edi =[lpMultiLanguage]
                               stdcall  LoadResoursIndirectHL,edi,RT_STR,ecx;edi =[lpMultiLanguage]

                                   ;Установим
                                   mov  [aMenuStateChange.dwTypeData],eax

                               ;Установим новый текст
                               ;ebp = Хендл окна;ebx = hmenu ;esi = [SetMenuItemInfoW];edi =[lpMultiLanguage]
                               stdcall  esi,ebx,MENU_SHOW_HIDE_Id,0,aMenuStateChange;ebx = hmenu;esi = [SetMenuItemInfoW]
                                        _LastErrorNotNull;Если не ноль возращает функция то ошибки нет



     ;Установим текст в итеме меню MENU_ACTIVATE_Id если надо
     ;ebp = Хендл окна;ebx = hmenu;esi = [SetMenuItemInfoW];edi =[lpMultiLanguage]
    .NotChangeMenuShowHide:
                        ;Подготовим
                        mov  eax,[aIniFileData.aIni.iSuspendPlayHook]
                        mov  ecx,STR_MENU_NOTIFY_Activ
                        mov  edx,STR_MENU_NOTIFY_Susp

                        ;Если равно значит менять потребнасти нету
                        cmp  [aMenuState.ActivSusp],eax
                         je  .NotChangeMenuActivSusp;ebp = Хендл окна

                         ;Установим новое значение
                        mov  [aMenuState.ActivSusp],eax

                        ;Определим нужный id строки
                        cmp  [hiconPlay],eax
                      cmove  ecx,edx

                    ;!BaseRes = указатель на корневой каталог,!TypeRes = Id типа ресурса,IdRes - Id конечных требуемых данных;возвращает eax = указатель на конечные данные
                    ;ebp = Хендл окна;ebx = hmenu;esi = [SetMenuItemInfoW];edi =[lpMultiLanguage]
                    stdcall  LoadResoursIndirectHL,edi,RT_STR,ecx;edi =[lpMultiLanguage]

                        ;Установим
                        mov  [aMenuStateChange.dwTypeData],eax

                    ;Установим новый текст
                    ;ebp = Хендл окна;ebx = hmenu ;esi = [SetMenuItemInfoW];edi =[lpMultiLanguage]
                    stdcall  esi,ebx,MENU_ACTIVATE_Id,0,aMenuStateChange;ebx = hmenu;esi = [SetMenuItemInfoW]
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

        ;Выводим меню shellnotify
        ;ebp = Хендл окна
       .NotChangeMenuActivSusp:
                        xor  ebx,ebx;ebx = ноль
                        lea  esi,[!aTrackPopupPozition];esi =[!aTrackPopupPozition]
                     invoke  SetForegroundWindow,ebp;ebp = Хендл окна ;чтоб не глючило

                     invoke  GetCursorPos,esi;esi =[!aTrackPopupPozition]
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                     invoke  TrackPopupMenuEx,[hmenuSubPopupNotify],TPM_RETURNCMD or TPM_NONOTIFY,[esi+POINT.x],[esi+POINT.y],ebp,ebx;ebp = Хендл окна;ebx = ноль;esi =[!aTrackPopupPozition]
                        mov  esi,eax;esi = результат TrackPopupMenuEx

                     invoke  PostMessageW,ebp,ebx,ebx,ebx;ebp = Хендл окна;ebx = ноль;WM_NULL = 0;чтоб не глючило
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                        cmp  esi,MENU_EXIT_Id;esi = результат TrackPopupMenuEx
                         je  .Exit;ebp = Хендл окна;ebx = ноль
                        cmp  esi,MENU_SHOW_HIDE_Id;esi = результат TrackPopupMenuEx
                         je  .MENU_SHOW_HIDE_Id;ebp = Хендл окна;ebx = ноль
                        cmp  esi,MENU_ACTIVATE_Id;esi = результат TrackPopupMenuEx
                         je  .MENU_ACTIVATE_Id;ebp = Хендл окна;ebx = ноль

                        jmp  .proc_null


;Скроем или покажим главное окно в зависимости от потребности
;ebp = Хендл окна;ebx = ноль
;--------------------------------------------------------------------------------------
.MENU_SHOW_HIDE_Id:
       mov  eax,SW_SHOW
       cmp  ebx,[aMenuState.NotifySnowHide];ebx = ноль
    cmovne  eax,ebx;ebx = ноль
    invoke  ShowWindow,ebp,eax;ebp = Хендл окна;SW_HIDE = 0

       jmp  .proc_null


;активируем или приостановим работу хука основного
;ebp = Хендл окна;ebx = ноль
;--------------------------------------------------------------------------------------
.MENU_ACTIVATE_Id:

       ;Подготовим
       mov  edx,[hwndButtonActivSusp]

       jmp  .BUTTON_ACTIVSUSP_Id;ebx = ноль;edx = !Lparam ;ebp = Хендл окна



;Shell_NotifyIcon - Exploer перезагрузился нужно опять значок добаватить в трей
;ebx = 0
;----------------------------------------------
      .msgTaskCreat:
             ;добавляем значок в трей
             ;Добавляет иконку или изменяенти подсказку к ней в зависимости от языка и активности главного хука
             ;На входе:;!dwMessage = NIM_ADD или NIM_MODIFY для Shell_NotifyIcon
             stdcall  ChangeNotifyIcon,NIM_ADD

                 jmp  .proc_null


;Обрабатываем сообщения от Control, Accelerator, Menu
;--------------------------------------
        .WM_COMMAND:;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
                test  edx,edx;Проверяем - если !Lparam не равно нулю значит Control
                 jnz  .WM_COMMAND.Contol;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam

                 jmp  .proc_null


;Обрабатываем сообщения от Control
 .WM_COMMAND.Contol:;ebp = Хендл окна;ebx = ноль;ecx = !Wparam ;edx = !Lparam
                 cmp  ecx,(BN_CLICKED shl 16) or BUTTON_EXIT_Id;BN_CLICKED - HIWORD, BUTTON_EXIT_Id - LOWORD
                  je  .Exit;ebp = Хендл окна;ebx = ноль
                 cmp  ecx,(CBN_SELCHANGE shl 16) or COMBOBOX_LANG_Id
                  je  .ChangeLang;ebx = ноль;edx = !Lparam ;ebp = Хендл окна
                 cmp  ecx,(BN_CLICKED shl 16) or BUTTON_OPTIONS_Id;BN_CLICKED - HIWORD, BUTTON_OPTIONS_Id - LOWORD
                  je  .BUTTON_OPTIONS_Id;ebx = ноль;edx = !Lparam ;ebp = Хендл окна
                 cmp  ecx,(BN_CLICKED shl 16) or BUTTON_ACTIVSUSP_Id;BN_CLICKED - HIWORD, BUTTON_OPTIONS_Id - LOWORD
                  je  .BUTTON_ACTIVSUSP_Id;ebx = ноль;edx = !Lparam ;ebp = Хендл окна
                 jmp  .proc_null


;Приостановка действия хука
;ebx = ноль;edx = !Lparam ;ebp = Хендл окна
;--------------------------------------
 .BUTTON_ACTIVSUSP_Id:
                invoke  SendMessageW,edx,BM_SETIMAGE,IMAGE_ICON,[aIniFileData.aIni.iSuspendPlayHook];edx = !Lparam
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                   mov  [aIniFileData.aIni.iSuspendPlayHook],eax
                   mov  ecx,WM_APP_HOOK_Play
                   mov  edx,WM_APP_HOOK_Stop
                   cmp  [hiconPlay],eax
                cmovne  ecx,edx

                invoke  PostThreadMessageW,[iThreadIdHook],ecx,ebx,ebx;ebx = ноль
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

               ;обновляем текст значка в треи
               ;Добавляет иконку или изменяенти подсказку к ней в зависимости от языка и активности главного хука
               ;На входе:;!dwMessage = NIM_ADD или NIM_MODIFY для Shell_NotifyIcon
               stdcall  ChangeNotifyIcon,NIM_MODIFY

                   jmp  .proc_null

;Все настройки приложения
;ebx = ноль;edx = !Lparam ;ebp = Хендл окна
 .BUTTON_OPTIONS_Id:
             stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_DLG,DIALOG_OPTIONS
              invoke  DialogBoxIndirectParamW,[hInstance],eax,ebp,DlgProcOptions,ebx;ebx = ноль;ebp = Хендл окна
                 jmp  .proc_null

;Коректируем расположение дочерних окон
;ebp = Хендл окна;edx = !Lparam;ebx = ноль
;--------------------------------------
.WM_WINDOWPOSCHANGED:
                 ;Сохраняем размер и положение главного окна
                 ;ebp = Хендл окна;edx = !Lparam;ebx = ноль
                 mov  esi,aWindowRect;esi = aWindowRect
                 mov  eax,[edx+WINDOWPOS.x];edx = !Lparam
                 mov  ecx,[edx+WINDOWPOS.y];edx = !Lparam
                 mov  [esi+RECT.left],eax;esi = aWindowRect
                 mov  [esi+RECT.top],ecx;esi = aWindowRect
                 add  eax,[edx+WINDOWPOS.cx];edx = !Lparam
                 add  ecx,[edx+WINDOWPOS.cy];edx = !Lparam
                 mov  [esi+RECT.right],eax;esi = aWindowRect
                 mov  [esi+RECT.bottom],ecx;esi = aWindowRect

                ;проверяем изменился ли размер основного окна если нет  то перемещать дочерние нет нужды
                ;ebp = Хендл окна;edx = !Lparam;ebx = ноль
                test  [edx+WINDOWPOS.flags],SWP_NOSIZE
                 jnz  .proc_null; перемещаем окна так как  размер основного изменился

                 mov  esi,aClientRect
              invoke  GetClientRect,ebp,esi;ebp = Хендл окна;esi = aClientRect
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 mov  edi,[esi+RECT.right];edi = Width client
                 mov  esi,[esi+RECT.bottom];esi = Height client

                 mov  ebp,[SetWindowPos];ebp = [SetWindowPos]

                ;Button Exit
                ;Узнаем новые координаты дочернего окна
                lea  eax,[edi-BUTTON_EXIT_Width];edi = Width client
                lea  ecx,[esi-BUTTON_EXIT_Height];esi = Height client
            stdcall  ebp,[aArrayHwndFont.hwndButtonExit],ebx,eax,ecx,ebx,ebx,SWP_NOZORDER or SWP_NOSIZE;ebx = ноль ;ebp = [SetWindowPos]
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Button Options
                ;Узнаем новые координаты дочернего окна
                lea  ecx,[esi-BUTTON_OPTIONS_Height];esi = Height client
            stdcall  ebp,[aArrayHwndFont.hwndButtonOptions],ebx,ebx,ecx,ebx,ebx,SWP_NOZORDER or SWP_NOSIZE;ebx = ноль;ebp = [SetWindowPos]
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Button приостановить/активировать
                ;Узнаем новые координаты дочернего окна
                lea  eax,[edi-BUTTON_ACTIVSUSP_Width-BUTTON_EXIT_Width];edi = Width client
                lea  ecx,[esi-BUTTON_ACTIVSUSP_Height];esi = Height client
            stdcall  ebp,[hwndButtonActivSusp],ebx,eax,ecx,ebx,ebx,SWP_NOZORDER or SWP_NOSIZE;ebx = ноль ;ebp = [SetWindowPos]
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Static
                ;Узнаем новые координаты дочернего окна
                lea  ecx,[esi-STATIC_HELP_Height+1];esi = Height client
                lea  edx,[edi-BUTTON_EXIT_Width-BUTTON_OPTIONS_Width-BUTTON_ACTIVSUSP_Width-COMBOBOX_LANG_Width-2]
            stdcall  ebp,[aArrayHwndFont.hwndStatic],ebx,BUTTON_OPTIONS_Width+COMBOBOX_LANG_Width+2,ecx,edx,STATIC_HELP_Height-1,SWP_NOZORDER;ebx = ноль ;ebp = [SetWindowPos]
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;ComboBox
                ;Узнаем новые координаты дочернего окна
                lea  ecx,[esi-COMBOBOX_LANG_Height];esi = Height client
            stdcall  ebp,[aArrayHwndFont.hwndComboBoxLang],ebx,BUTTON_OPTIONS_Width+1,ecx,ebx,ebx,SWP_NOZORDER or SWP_NOSIZE;ebx = ноль ;ebp = [SetWindowPos]
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Sys Tab
                ;Узнаем новые координаты дочернего окна
            stdcall  ebp,[aArrayHwndFont.hwndSysTab],ebx,ebx,ebx,edi,SYSTAB_CONTROL_Height,SWP_NOZORDER or SWP_NOMOVE;ebx = ноль ;ebp = [SetWindowPos];edi = Width client
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Подготовим
                mov  ebp,[iIdForStatic];ebp = Id STR

             invoke  aTabCurrent.Move

                ;Если программа не куплена то нужно обновлять текст в статике, так как статик на изменения размера не риагирует
                cmp  ebp,STR_WINDOW_NOT_LICENSE;ebp = Id STR
                jne  .proc_null

            stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,ebp;ebp = Id STR

             ;Установим что программа не купленна
             invoke  SendMessageW,[aArrayHwndFont.hwndStatic],WM_SETTEXT,0,eax
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 jmp  .proc_null


;Обрабатываем пунткт выбраный в ComboBox
;ebx = ноль ;edx = !Lparam ;ebp = Хендл окна
;--------------------------------------
        .ChangeLang:
                 mov  edi,edx;edi = Хендл  ComboBox
              invoke  SendMessageW,edi,CB_GETCURSEL,ebx,ebx;edi = Хендл  ComboBox ;ebp = Хендл окна;ebx = ноль
                 mov  esi,eax;esi = Id  выбраного пользователям итема в ComboBox

                 ;Проверяем может проверяем может по факту выбора и не было сделано - тогда ничего не делаем
                 cmp  [aIniFileData.aIni.Language],eax
                  je  .proc_null

             ;За гружает нужный язык по !idlang - id языка для загрузки например COMBOBOX_ENGLISH он совпадаед с id выбраным в ComboBox
             ;Если не удаеться загрузить то eax = 0
             ;edi = Хендл  ComboBox;esi = Id  выбраного пользователям итема в ComboBox
             stdcall  LoadLang,esi;esi = Id  выбраного пользователям итема в ComboBox
                test  eax,eax
                  jz  @f;ebp = Хендл окна ;ebx = ноль;edi = Хендл  ComboBox

;Устанавливает текст
             stdcall  SetText
                 jmp  .proc_null


                 @@:;ebp = Хендл окна ;ebx = ноль;edi = Хендл  ComboBox
;Раз не удалось загрузить другой язык сообщим пользователь об этом
                 mov  esi,[esi*4*2+tbLanguageTable+4];получаем путь к нужному языку; esi = Id  выбраного пользователям итема в ComboBox
             stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_COMBOBOX_LANG_Error;eax = указатель на конечные данные,ecx = Размер или что еще в зависимости от типа
              invoke  MessageBoxExW,ebp,eax,esi,MB_OK or MB_ICONINFORMATION,ebx;ebp = Хендл окна;ebx = ноль
;Востановим предыдущий выбар пользователя в  ComboBox
              invoke  SendMessageW,edi,CB_SETCURSEL,[aIniFileData.aIni.Language],ebx;edi = Хендл ComboBox;ebx = ноль
                 jmp  .proc_null



;Выход из приложения
;ebp = Хендл окна;ebx = ноль
;--------------------------------------
              .Exit:

;Сначала проводим операции по закрытию в каторых нужен хендл окна
;ebp = Хендл окна;ebx = ноль
;--------------------------------------
              ;Получаем расположение окна для сохранения в файле
              ;ebp = Хендл окна;ebx = ноль
                 mov  esi,aIniFileData.aIni.aWindPlac;esi = WINDOWPLACEMENT
              invoke  GetWindowPlacement,ebp,esi;ebp = Хендл окна;esi = WINDOWPLACEMENT
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 mov  edi,[esi+WINDOWPLACEMENT.showCmd];esi = WINDOWPLACEMENT

              ;Проверим окно скрыто или нет
              ;ebp = Хендл окна;esi = WINDOWPLACEMENT;ebx = ноль;edi = [WINDOWPLACEMENT.showCmd]
              invoke  IsWindowVisible,ebp;ebp = Хендл окна

                ;Подготовим данные в зависимости от того: окно видимо или нет и окно максиризировано или нет
                test  eax,eax
               cmovz  edi,eax
                 mov  [esi+WINDOWPLACEMENT.showCmd],edi


              ;Shell_NotifyIcon - удаляем  значок с панели задач
              ;ebp = Хендл окна;ebx = ноль
              invoke  Shell_NotifyIconW,NIM_DELETE,aNotifyIconData
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


                 ;Обнулим так как уничтожаем окно и функции использущие хендл главного окна не смогли уже его использовать
                 ;ebp = Хендл окна;ebx = ноль
                 mov  [hwnd],ebp;ebx = ноль

              ;закрываем окно
              ;ebp = Хендл окна;ebx = ноль
              invoke  DestroyWindow,ebp;ebp = Хендл окна
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 jmp  .proc_null


;--------------------------------------
        .WM_DESTROY:
              invoke  PostQuitMessage,eax
         .proc_null:
                 xor  eax,eax
          .proc_end:
                 ret
 endp
