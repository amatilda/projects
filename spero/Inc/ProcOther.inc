
;Обрабатываем таймер отвечающий за приложения от взлома  constTimeGuardianMin
;--------------------------------------------------------------------------------------
proc TimerProcGuardian [ebx],!hwnd,!uMsg,!idEvent,!dwTime

                 ;Получаем  адрес LoadLibraryExW
                 mov  eax,Start.GetProcessHeap
                 mov  ecx,[constLoadLibraryExW+eax]
                 xor  ebx,ebx;ebx = 0
                 add  ecx,2;ecx = [LoadLibraryExW];Каректируем что бы пропустить mov  edi,edi - ну авдруг усложнит
                 ;Если определили что программа взломанна - не проверяем больше;Если равно нулю - взломана
                 cmp  [iPlague],ebx;ebx = 0
                  je  .Exit

             ;Попробуем загрузить нашу Guardian.dll
             ;ebx = 0
             stdcall  ecx,szRoamingAppData,ebx,ebx;ecx = [LoadLibraryExW];ebx = 0
              ;Если возврати возвратила ноль - отладчик подключен - будем предпринимать меры
                test  eax,eax
                  jz  .TakeAction

                 ;Получаем  адрес FreeLibrary
                 mov  edx,Start.GetProcessHeap
                 mov  ecx,[constFreeLibrary+edx]
                 add  ecx,2;ecx = [FreeLibrary];Каректируем что бы пропустить mov  edi,edi - ну авдруг усложнит

             ;Выгрузим dll
             stdcall  ecx,eax;ecx = [FreeLibrary]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

             ;Начнем считать CRC32c  защищеных участков
             stdcall  CRC32cHL,Start,lpGuardianEnd - Start,-1
                 cmp  eax,[iconstCRC32cGuardian]
                 jne  .TakeAction

               ;Выходим нечего не делая
              .Exit:
                 ret

        ;Примем меры так как либо под отладчикам или программа изменнена
       .TakeAction:
                dec  [iPlague]
                ret
endp

;На основании vkCode с помощью поиска по таблице находит текст и копирую в буфер, все регистры сохраняет
;--------------------------------------------------------------------------------------
proc VkCodeInString [eax ecx edx],!vkCodeOne,!vkCodeTwoThree,!Buffer
                   movzx  ecx,word[!vkCodeTwoThree+2]
                     mov  eax,[!Buffer]
                    test  ecx,ecx
                      jz  .TestTwo

                     mov  ecx,[tbKey+ecx*4]

                     @@:
                     mov  dx,[ecx]
                     add  ecx,2
                     mov  [eax],dx
                     add  eax,2
                    test  dx,dx
                     jnz  @b

                     mov  word[eax-2],' '
                     mov  word[eax],'+'
                     mov  word[eax+2],' '
                     add  eax,4

               .TestTwo:
                   movzx  ecx,word[!vkCodeTwoThree]
                    test  ecx,ecx
                      jz  .One

                     mov  ecx,[tbKey+ecx*4]

                     @@:
                     mov  dx,[ecx]
                     add  ecx,2
                     mov  [eax],dx
                     add  eax,2
                    test  dx,dx
                     jnz  @b

                     mov  word[eax-2],' '
                     mov  word[eax],'+'
                     mov  word[eax+2],' '
                     add  eax,4

                   .One:
                     mov  ecx,[!vkCodeOne]
                     mov  ecx,[tbKey+ecx*4]
                     @@:
                     mov  dx,[ecx]
                     add  ecx,2
                     mov  [eax],dx
                     add  eax,2
                    test  dx,dx
                     jnz  @b

                     ret
 endp

 ;--------------------------------------------------------------------------------------
 proc LowLevelMouseProcInValue [ebx esi edi ebp],!nCode,!wParam,!lParam
  locals
      !aBufferString         du  tbKeylengthMax dup(?)
  endl
                        mov  ebx,[!nCode]
                        mov  edi,[!wParam]
                        mov  esi,[!lParam]

                        ;ebx = [!nCode];edi = [!wParam];esi = [!lParam]
                        xor  eax,eax
                       test  ebx,ebx;если меньше нуля должны пропустить
                     cmovnz  eax,esp
                        cmp  edi,WM_MOUSEMOVE;Если просто движение мыши тоже пропускаем
                      cmove  eax,esp
                        cmp  edi,WM_LBUTTONUP;Пропускаем чтобы  можно было нажимать спокойно на другие  окна - кнопки
                      cmove  eax,esp
                       test  eax,eax
                        jnz  .Skip;ebx = [!nCode];edi = [!wParam];esi = [!lParam]

                     invoke  WindowFromPhysicalPoint,[esi+MSLLHOOKSTRUCT.pt.x],[esi+MSLLHOOKSTRUCT.pt.y]

                        mov  edx,aSet
                        mov  ebp,aTo
                        xor  ecx,ecx

                        cmp  eax,[edx+KEYMAP.hwnd]
                      cmove  ecx,edx
                        cmp  eax,[ebp+KEYMAP.hwnd]
                      cmove  ecx,ebp

                       test  ecx,ecx;Пропускаем так как не на наших окнах нажатие было
                         jz  .Skip;ebx = [!nCode];edi = [!wParam];esi = [!lParam]

                        cmp  edi,WM_LBUTTONDOWN;Пропускаем чтобы  можно было нажимать спокойно на другие  окна - кнопки  но так же и текст отображаем и сохраняем данные
                         je  .SkipText;ebx = [!nCode];edi = [!wParam];esi = [!lParam];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP

                        mov  ebp,[esi+MSLLHOOKSTRUCT.mouseData];ebp = [esi+MSLLHOOKSTRUCT.mouseData]

                        ;edi = [!wParam];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP;ebp = [esi+MSLLHOOKSTRUCT.mouseData]
                        cmp  edi,WM_MOUSEWHEEL;Колесо вперед и назад
                         je  .WM_MOUSEWHEEL
                        cmp  edi,WM_MOUSEHWHEEL;Колесо вправо и в лево
                         je  .WM_MOUSEHWHEEL
                        cmp  edi,WM_RBUTTONDOWN;Правая кнопка мыши
                         je  .WM_RBUTTONDOWN
                        cmp  edi,WM_MBUTTONDOWN;Щелчек колесом
                         je  .WM_MBUTTONDOWN
                        cmp  edi,WM_XBUTTONDOWN;Первая и вторая доп кнопка мыши
                         je  .WM_XBUTTONDOWN

                        jmp  .NoText

            ;edi = [!wParam];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP;ebp = [esi+MSLLHOOKSTRUCT.mouseData]
            .WM_MOUSEWHEEL:
                        shr  ebp,31
                        lea  edi,[VK_MOUSEWHEEL_UP+ebp]
                        jmp  .Text;edi = [vkCode];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP

            ;edi = [!wParam];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP;ebp = [esi+MSLLHOOKSTRUCT.mouseData]
            .WM_MOUSEHWHEEL:
                        shr  ebp,31
                        lea  edi,[VK_MOUSEWHEEL_RIGHT+ebp]
                        jmp  .Text;edi = [vkCode];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP


             ;edi = [!wParam];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP;ebp = [esi+MSLLHOOKSTRUCT.mouseData]
             .WM_RBUTTONDOWN:
                        mov  edi,VK_RBUTTON
                        jmp  .Text;edi = [vkCode];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP


             ;edi = [!wParam];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP;ebp = [esi+MSLLHOOKSTRUCT.mouseData]
             .WM_MBUTTONDOWN:
                        mov  edi,VK_MBUTTON
                        jmp  .Text;edi = [vkCode];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP


             ;edi = [!wParam];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP;ebp = [esi+MSLLHOOKSTRUCT.mouseData]
             .WM_XBUTTONDOWN:
                        mov  edi,VK_XBUTTON1
                        mov  edx,VK_XBUTTON2
                        shr  ebp,17
                     cmovnc  edi,edx

                      ;Все кнопки мыши кроме левой
                      ;edi = [vkCode];eax = [KEYMAP.hwnd];ecx = KEYMAP
                     .Text:
                        ;Подготовим
                        mov  edx,[tbKey+edi*4];edx = [lpName];edi = [vkCode]

                        ;Установим
                        mov  [ecx+KEYMAP.vkCodeOne],edi;edi = [vkCode];ecx = KEYMAP

                        ;Проверяем может нажат моификатор - тогда это комбинация отобразим по другому;Равны могут быть только если два равны нулю
                        mov  esi,[aHookMinor.iKeyModif]
                        mov  ebx,[aHookMinor.iKeyModif2]
                        cmp  ebx,esi;ebx = [HOOK_MINOR.iKeyModif];esi = [HOOK_MINOR.iKeyModif2]
                        jne  .Combi;edi = [vkCode];eax = [edx+KEYMAP.hwnd];ecx = KEYMAP;ebx = [HOOK_MINOR.iKeyModif2];esi = [HOOK_MINOR.iKeyModif]

                        ;Установим;Нужно для для проверки на схожесть клавиш в других итемах, чтобы не было савпадений - замкнутага цикла
                        mov  [ecx+KEYMAP.vkCodeTwoThree],ebx;ebx = 0;ecx = KEYMAP

                 ;Отобразим текст
                 ;ebx = 0;eax = [KEYMAP.hwnd];ecx = KEYMAP;edx = [lpName]
                .CombiText:
                        mov  [ecx+KEYMAP.scanCodeSubOne],ebx;ebx = 0;ecx = KEYMAP

                     invoke  SendMessageW,eax,WM_SETTEXT,ebx,edx;edx = [lpName];eax = [KEYMAP.hwnd];ebx = 0
                            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                   .NoText:
                        mov  eax,1
                        ret

                     ;Обработаем комбинацию
                     ;edi = [vkCode];eax = [KEYMAP.hwnd];ecx = KEYMAP;ebx = [HOOK_MINOR.iKeyModif2];esi = [HOOK_MINOR.iKeyModif]
                    .Combi:
                        mov  ebp,ebx;ebx = [HOOK_MINOR.iKeyModif2]
                        mov  edx,esi;esi = [HOOK_MINOR.iKeyModif]
                        shl  ebp,16
                        shr  esi,16;esi = [HOOK_MINOR.iKeyModif]
                        mov  bp,dx;ebp = vkCodeTwoThree
                        mov  bx,si;ebx = scanCodeSubTwoThree

                        ;Подготовим
                        lea  edx,[!aBufferString];edx = !aBufferString

                        ;Сохраним
                        ;edi = [vkCode];eax = [KEYMAP.hwnd];ecx = KEYMAP;ebp = vkCodeTwoThree;ebx = scanCodeSubTwoThree;edx = !aBufferString
                        mov  [ecx+KEYMAP.vkCodeTwoThree],ebp;ebp = vkCodeTwoThree;ecx = KEYMAP
                        mov  [ecx+KEYMAP.scanCodeSubTwoThree],ebx;ebx = scanCodeSubTwoThree;ecx = KEYMAP

                    ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
                    ;edi = [vkCode];eax = [KEYMAP.hwnd];ecx = KEYMAP;ebp = vkCodeTwoThree;edx = !aBufferString
                    stdcall  VkCodeInString,edi,ebp,edx;edi = [vkCode];ebp = vkCodeTwoThree;edx = !aBufferString

                        ;Подготовим
                        ;eax = [KEYMAP.hwnd];ecx = KEYMAP;edx = !aBufferString;edx = [lpName]
                        xor  ebx,ebx;ebx = 0

                        ;Вывидим текст
                        jmp  .CombiText;ebx = 0;eax = [KEYMAP.hwnd];ecx = KEYMAP;edx = [lpName]



                  ;Левая кнопка мыши
                  ;ebx = [!nCode];edi = [!wParam];esi = [!lParam];eax = [KEYMAP.hwnd];ecx = KEYMAP
                 .SkipText:
                        ;Установим
                        mov  [ecx+KEYMAP.vkCodeOne],VK_LBUTTON;ecx = KEYMAP

                        ;Проверяем может нажат моификатор - тогда это комбинация отобразим по другому;Равны могут быть только если два равны нулю
                        mov  edx,[aHookMinor.iKeyModif]
                        mov  ebp,[aHookMinor.iKeyModif2]
                        cmp  ebp,edx;ebp = [HOOK_MINOR.iKeyModif2];edx = [HOOK_MINOR.iKeyModif]
                        jne  .SkipCombi;ebx = [!nCode];edi = [!wParam];esi = [!lParam];ecx = KEYMAP;ebp = [HOOK_MINOR.iKeyModif2];edx = [HOOK_MINOR.iKeyModif]

                        ;Подготовим
                        mov  edx,[tbKey+VK_LBUTTON*4];edx = lpText

                        ;Установим;Нужно для для проверки на схожесть клавиш в других итемах, чтобы не было савпадений - замкнутага цикла
                        mov  [ecx+KEYMAP.vkCodeTwoThree],ebp;ebp = 0;ecx = KEYMAP

             ;Отобразим текст
             ;ebx = [!nCode];edi = [!wParam];esi = [!lParam];ebp = 0;eax = [KEYMAP.hwnd];edx = lpText;ecx = KEYMAP
            .SkipCombiText:
                        mov  [ecx+KEYMAP.scanCodeSubOne],ebp;ebp = 0 ;Нужно для для проверки на схожесть клавиш в других итемах, чтобы не было савпадений - замкнутага цикла

                     invoke  SendMessageW,eax,WM_SETTEXT,ebp,edx;eax = [KEYMAP.hwnd];ebp = 0;edx = lpText
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                     ;ebx = [!nCode];edi = [!wParam];esi = [!lParam]
                     .Skip:
                     invoke  CallNextHookEx,0,ebx,edi,esi;ebx = [!nCode];edi = [!wParam];esi = [!lParam]
                        ret

                 ;Обработаем комбинацию
                 ;ebx = [!nCode];edi = [!wParam];esi = [!lParam];ecx = KEYMAP;ebp = [HOOK_MINOR.iKeyModif2];edx = [HOOK_MINOR.iKeyModif]
                .SkipCombi:
                        ;Получим составной vkCodeTwoThree и scanCodeSubTwoThree
                        rol  ebp,16;ebp = [HOOK_MINOR.iKeyModif2]
                        rol  edx,16;edx = [HOOK_MINOR.iKeyModif]
                        mov  ax,bp
                        shl  eax,16
                        mov  ax,dx;eax = scanCodeSubTwoThree
                        rol  edx,16;edx = [HOOK_MINOR.iKeyModif]
                        mov  bp,dx;ebp = vkCodeTwoThree

                        ;Подготовим
                        lea  edx,[!aBufferString];edx = !aBufferString

                        ;Сохраним
                        ;ebx = [!nCode];edi = [!wParam];esi = [!lParam];ecx = KEYMAP;ebp = vkCodeTwoThree;eax = scanCodeSubTwoThree;edx = !aBufferString
                        mov  [ecx+KEYMAP.vkCodeTwoThree],ebp;ebp = vkCodeTwoThree;ecx = KEYMAP
                        mov  [ecx+KEYMAP.scanCodeSubTwoThree],eax;eax = scanCodeSubTwoThree;ecx = KEYMAP

                    ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
                    ;ebx = [!nCode];edi = [!wParam];esi = [!lParam];ecx = KEYMAP;ebp = vkCodeTwoThree;edx = !aBufferString
                    stdcall  VkCodeInString,VK_LBUTTON,ebp,edx;ebp = vkCodeTwoThree;edx = !aBufferString

                        ;Подготовим
                        ;ebx = [!nCode];edi = [!wParam];esi = [!lParam];ecx = KEYMAP;edx = !aBufferString
                        xor  ebp,ebp;ebp = 0
                        mov  eax,[ecx+KEYMAP.hwnd];ecx = KEYMAP;eax = [KEYMAP.hwnd]

                        ;Вывидим текст
                        jmp  .SkipCombiText;ebx = [!nCode];edi = [!wParam];esi = [!lParam];ebp = 0;eax = [KEYMAP.hwnd];edx = lpText;ecx = KEYMAP
 endp

 ;--------------------------------------------------------------------------------------
 proc LowLevelKeyboardProcInValue [ebx esi edi ebp],!nCode,!wParam,!lParam
  locals
      !aBufferString         du  tbKeylengthMax dup(?)
  endl
                        mov  ebx,[!nCode];ebx = [!nCode]
                        mov  ebp,[!lParam];ebp = [!lParam]

                       test  ebx,ebx;если меньше нуля должны пропустить
                        jnz .Skip;ebx = [!nCode];ebp = [!lParam]

                     ;ebx = [!nCode];ebp = [!lParam]
                     ;Получаем окно которое имеет клавиатурный фокус если совпадет Edit нашыми значит можно перехватывать клавиши
                     invoke  GetFocus

                        mov  esi,aSet
                        mov  edi,aTo
                        xor  edx,edx

                        cmp  eax,[esi+KEYMAP.hwnd]
                      cmove  edx,esi
                        cmp  eax,[edi+KEYMAP.hwnd]
                      cmove  edx,edi

                        ;ebx = [!nCode];ebp = [!lParam]-KBDLLHOOKSTRUCT;edx = KEYMAP
                      movzx  ecx,byte[ebp+KBDLLHOOKSTRUCT.flags];ecx =[flags]; флаги занимают только один байт поэтому используем movzx что бы навсяки случай не взять не нужные данные
                      movzx  esi,byte[ebp+KBDLLHOOKSTRUCT.vkCode];esi = [vkCode];vkCode-байт занимать и таблица у нас исходит из этого поэтому и используем movzx,что бы не было проблем
                        mov  edi,[lpaKeyPressedMapIn];edi = [lpaKeyPressedMapIn]
                      movzx  eax,byte[edi+esi];edi = [lpaKeyPressedMapIn];esi = [vkCode];eax = prev value key press

                       ;Проверка удерживаеться клавиша или нет - если 1 в таблице значит удерживаеться
                       ;ebx = [!nCode];esi = [vkCode];edx = KEYMAP;edi = [lpaKeyPressedMapIn];ecx =[flags];eax = prev value key press
                       test  ecx,0x80;ecx =[flags]
                       setz  byte[edi+esi];edi = [lpaKeyPressedMapIn];esi = [vkCode]

                       ;Регулируем то какая кнопка мыши в текущий момент нажата,когда любая клавиша клавиатуры меняет свое значение с нажата на освободена или обратна-обнуляем iMousePressed
                       ;Это позволяет правильно отображать сочитания мыши и клавиатуры
                       ;ebx = [!nCode];esi = [vkCode];edx = KEYMAP;edi = [lpaKeyPressedMapIn];ecx =[flags];eax = prev value key press
                        ;cmp  al,byte[edi+esi];esi = [vkCode];edi = [lpaKeyPressedMapIn];eax = prev value key press
                        ;mov  eax,0
                      ;cmove  eax,[aHookMinor.iMouse]
                        ;mov  [aHookMinor.iMouse],eax

                       ;Если ноль значит фокус не на одном из двух наших Edit, поэтому пропускаем
                       ;ebx = [!nCode];esi = [vkCode];edx = KEYMAP;ecx =[flags]
                       test  edx,edx;edx = KEYMAP
                         jz  .Skip;ebx = [!nCode];ebp = [!lParam]

                       ;Клавиша отпущена значит пропускаем так как мы все делаем что нужно во время когда клавиша нажата; тоестть сообщение имеет приставку "UP"
                       ;esi = [vkCode];edx = KEYMAP;ecx =[flags]
                       test  ecx,0x80;ecx =[flags]
                        jnz  .NoTextTest;esi = [vkCode]

                       ;Подготавливаем и соеденяем сканкод клавиши с битом расширения в страшем бите  word
                       ;esi = [vkCode];edx = KEYMAP;ecx =[flags]
                       xchg  ebp,esi;esi = [vkCode];ebp = [vkCode]
                        shl  ecx,15;работаем с  dword так как ранее с помощью movzx разобрались с вазможными не нужными данными
                        mov  esi,[esi+KBDLLHOOKSTRUCT.scanCode];esi = [scanCode]
                         or  esi,ecx

                       ;Создаем в регистре карту нажатык клавиш модификаторов и тем самым смещение их в структуре, если 1 значит нажата
                       ;ebp = [vkCode];edx = KEYMAP;edi = [lpaKeyPressedMapIn] ;esi = [scanCode]
                        xor  eax,eax
                        mov  cl,byte[edi+VK_SHIFT];edi = [lpaKeyPressedMapIn]
                        mov  ch,byte[edi+VK_CONTROL];edi = [lpaKeyPressedMapIn]
                        mov  al,cl
                        ror  eax,1
                        mov  al,ch
                        ror  eax,1

                        mov  bl,byte[edi+VK_MENU];edi = [lpaKeyPressedMapIn]
                        mov  bh,byte[edi+VK_LSHIFT];edi = [lpaKeyPressedMapIn]
                        mov  al,bl
                        add  cl,bl
                        ror  eax,1
                        mov  al,bh
                        add  ch,bh
                        ror  eax,1

                        mov  bl,byte[edi+VK_RSHIFT];edi = [lpaKeyPressedMapIn]
                        mov  bh,byte[edi+VK_LCONTROL];edi = [lpaKeyPressedMapIn]
                        mov  al,bl
                        add  cl,bl
                        ror  eax,1
                        mov  al,bh
                        add  ch,bh
                        ror  eax,1

                        mov  bl,byte[edi+VK_RCONTROL];edi = [lpaKeyPressedMapIn]
                        mov  bh,byte[edi+VK_LMENU];edi = [lpaKeyPressedMapIn]
                        mov  al,bl
                        add  cl,bl
                        ror  eax,1
                        add  ch,bh
                        mov  al,bh
                        ror  eax,1
                        add  cl,ch

                        mov  bl,byte[edi+VK_RMENU];edi = [lpaKeyPressedMapIn]
                        mov  al,bl
                        add  cl,bl
                        ror  eax,1

                        and  ecx,0xF
                        shr  eax,23;eax = карту нажатык клавиш модификаторов

                        cmp  ecx,2
                         ja  .NoText

                        xor  ebx,ebx
                        mov  edi,2;Так как сообщения ловим только UP иначе бы пришлось использовать 3

                        ;Проверяем текущая нажатая клавиша являетьли клавишей модификаторам
                        cmp  ebp,VK_SHIFT ;ebp = [vkCode]
                      cmove  ebx,edi
                        cmp  ebp,VK_CONTROL;ebp = [vkCode]
                      cmove  ebx,edi
                        cmp  ebp,VK_MENU;ebp = [vkCode]
                      cmove  ebx,edi
                        cmp  ebp,VK_LSHIFT;ebp = [vkCode]
                      cmove  ebx,edi
                        cmp  ebp,VK_RSHIFT;ebp = [vkCode]
                      cmove  ebx,edi
                        cmp  ebp,VK_LCONTROL;ebp = [vkCode]
                      cmove  ebx,edi
                        cmp  ebp,VK_RCONTROL;ebp = [vkCode]
                      cmove  ebx,edi
                        cmp  ebp,VK_LMENU;ebp = [vkCode]
                      cmove  ebx,edi
                        cmp  ebp,VK_RMENU;ebp = [vkCode]
                      cmove  ebx,edi

                        lea  edi,[!aBufferString];edi = [!aBufferString]
                        add  ecx,ebx

                        ;Сохраняем сканкод с битом расширанай клавиатуры(если расширина) в таблицу, что бы потом можно было извлечь
                        mov  ebx,[lpBufferDlgKeyMapScankode];ebx = [lpBufferDlgKeyMapScankode]
                        mov  word[ebx+ebp*2],si;esi = [scanCode];ebx = [lpBufferDlgKeyMapScankode]

                        jmp  [.JmpTable+ecx*4];ebp = [vkCode];edx = KEYMAP;esi = [scanCode];edi = [!aBufferString];ebx = [lpBufferDlgKeyMapScankode];eax = карту нажатык клавиш модификаторов

              ;Чисто для отображения при на жатие в Edit два модификатора
              ;ebp = [vkCode];edx = KEYMAP;esi = [scanCode];edi = [!aBufferString];ebx = [lpBufferDlgKeyMapScankode];eax = карту нажатык клавиш модификаторов
             .TwoModifiter:
                        ;Проверим если ни равно нулю значит ничего не деклаем  что бы ни сбить клавишу которая не умеет подовлять клавиши  модификаторы
                        cmp  [aHookMinor.iKeyModif2],0
                        jne  .NoText

                        ;Раз тут значит это вторая клавиша модификатор в первый раз нажатая сохраним ее для контроля мультимидийных не подовляющих клавиш модификаторов
                        mov  word[aHookMinor.iKeyModif2],bp;ebp = [vkCode]
                        mov  word[aHookMinor.iKeyModif2+2],si;esi = [scanCode]

                        ;Находим пазицию клавиши мадификатора занимаемаю в tbMODIFITER
                        bsr  ecx,eax;eax = карту нажатык клавиш модификаторов

                        mov  [edx+KEYMAP.vkCodeOne],0;edx = KEYMAP;только точки можно отменять

                        ;Получаем vkCode  первай клавиши мадификатара
                      movzx  ecx,byte[tbMODIFITER+ecx]

                        ;Находим пазицию клавиши мадификатора занимаемаю в tbMODIFITER
                        bsf  eax,eax;eax = карту нажатык клавиш модификаторов

                        shl  ecx,16

                        ;Получаем vkCode второй клавиши мадификатара
                      movzx  cx,byte[tbMODIFITER+eax]

                    ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
                    stdcall  VkCodeInString,0,ecx,edi;edi = [!aBufferString];ecx = [vkCode]

                        jmp  .Text;edx = KEYMAP;edi = техт


       ;Отображает и сохраняет  два модификатара + клавиша
       ;ebp = [vkCode];edx = KEYMAP;esi = [scanCode];edi = [!aBufferString];ebx = [lpBufferDlgKeyMapScankode];eax = карту нажатык клавиш модификаторов
       .TwoModifiterOneKey:
                        ;Находим пазицию клавиши мадификатора занимаемаю в tbMODIFITER
                        bsr  ecx,eax;eax = карту нажатык клавиш модификаторов

                        ;Сохраняем
                        mov  [edx+KEYMAP.vkCodeOne],ebp;edx = KEYMAP;ebp = [vkCode]
                        mov  [edx+KEYMAP.scanCodeSubOne],esi;edx = KEYMAP;esi = [scanCode]

                      ;Получаем vkCode  первай клавиши мадификатара
                      movzx  ecx,byte[tbMODIFITER+ecx]

                        ;Получаем по vkCode как индекс от нуля из времменой таблицы клавиш - scanCodeOne и флаг состояния расширеная или нет клавиша
                        mov  si,word[ebx+ecx*2];ebx = [lpBufferDlgKeyMapScankode];ecx = [vkCode]

                        ;Находим пазицию клавиши мадификатора занимаемаю в tbMODIFITER
                        bsf  eax,eax;eax = карту нажатык клавиш модификаторов

                        shl  ecx,16

                      ;Получаем vkCode  второй клавиши мадификатара
                      movzx  eax,byte[tbMODIFITER+eax]

                        shl  esi,16
                         or  ecx,eax

                        ;Получаем по vkCode как индекс от нуля из времменой таблицы клавиш - scanCodeOne и флаг состояния расширеная или нет клавиша
                        mov  si,word[ebx+eax*2];ebx = [lpBufferDlgKeyMapScankode];eax = [vkCode]

                    ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
                    stdcall  VkCodeInString,ebp,ecx,edi;edi = [!aBufferString];ebp = [vkCode];ecx = [vkCode]

                        ;Сохраняем
                        mov  [edx+KEYMAP.vkCodeTwoThree],ecx;edx = KEYMAP;ecx = [vkCode]
                        mov  [edx+KEYMAP.scanCodeSubTwoThree],esi;edx = KEYMAP;esi = [scanCode]

                        jmp  .Text;edx = KEYMAP;edi = техт


       ;Отображает и сохраняет модификатар + клавиша
       ;ebp = [vkCode];edx = KEYMAP;esi = [scanCode];edi = [!aBufferString];ebx = [lpBufferDlgKeyMapScankode];eax = карту нажатык клавиш модификаторов
       .OneModifiterOneKey:

                        ;Находим пазицию клавиши мадификатора занимаемаю в tbMODIFITER
                        bsf  ecx,eax;eax = карту нажатык клавиш модификаторов

                        ;Сохраняем
                        mov  [edx+KEYMAP.vkCodeOne],ebp;edx = KEYMAP;ebp = [vkCode]
                        mov  [edx+KEYMAP.scanCodeSubOne],esi;edx = KEYMAP;esi = [scanCode]

                      ;Получаем vkCode  первай клавиши мадификатара
                      movzx  ecx,byte[tbMODIFITER+ecx]

                        ;Получаем по vkCode как индекс от нуля из времменой таблицы клавиш - scanCodeOne и флаг состояния расширеная или нет клавиша
                      movzx esi,word[ebx+ecx*2];ebx = [lpBufferDlgKeyMapScankode];ecx = [vkCode]

                    ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
                    stdcall  VkCodeInString,ebp,ecx,edi;edi = [!aBufferString];ebp = [vkCode];ecx = [vkCode]

                        ;Сохраняем
                        mov  [edx+KEYMAP.vkCodeTwoThree],ecx;edx = KEYMAP;ecx = [vkCode]
                        mov  [edx+KEYMAP.scanCodeSubTwoThree],esi;edx = KEYMAP;esi = [scanCode]

                        jmp  .Text;edx = KEYMAP;edi = техт

     ;Обрабатываем один  модификатор
     ;ebp = [vkCode];edx = KEYMAP;esi = [scanCode];edi = [!aBufferString]
    .OneModifiterOneMouse:
                        ;Подготовим
                        mov  ecx,aHookMinor;ecx = HOOK_MINOR

                        cmp  [ecx+HOOK_MINOR.iKeyModif],0;ecx = HOOK_MINOR
                         je  .NotCombinationMulti;ebp = [vkCode];edx = KEYMAP;esi = [scanCode];ecx = HOOK_MINOR

                        jmp  .NoText


      ;Раз тут значит это первая клавиша модификатор сохраним ее для контроля мультимидийных не подовляющих клавиш модификаторов
      ;ebp = [vkCode];edx = KEYMAP;esi = [scanCode];ecx = HOOK_MINOR
      .NotCombinationMulti:
                        mov  word[ecx+HOOK_MINOR.iKeyModif],bp;ebp = [vkCode];ecx = HOOK_MINOR
                        mov  word[ecx+HOOK_MINOR.iKeyModif+2],si;esi = [scanCode];ecx = HOOK_MINOR

            ;Обрабатываем одну клавишу
            ;ebp = [vkCode];edx = KEYMAP;esi = [scanCode]
           .NotCombination:
                        mov  edi,[tbKey+ebp*4];ebp = [vkCode]
                        xor  eax,eax;eax = 0
                        mov  [edx+KEYMAP.vkCodeOne],ebp;ebp = [vkCode];edx = KEYMAP
                        mov  [edx+KEYMAP.vkCodeTwoThree],eax;eax = 0;Нужно для для проверки на схожесть клавиш в других итемах, чтобы не было савпадений - замкнутага цикла
                        mov  [edx+KEYMAP.scanCodeSubOne],esi;edx = KEYMAP;esi = [scanCode]

                    ;edx = KEYMAP;eax = техт
                    .Text:
                     invoke  SendMessageW,[edx+KEYMAP.hwnd],WM_SETTEXT,0,edi;edx = KEYMAP;edi = техт
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                   .NoText:
                        mov  eax,1
                        ret


                     ;ebx = [!nCode];ebp = [!lParam]
                     .Skip:
                     invoke  CallNextHookEx,0,ebx,[!wParam],ebp;ebx = [!nCode];ebp = [!lParam]
                        ret

                ;Конролируем отжатие сохранненых клавиш модификаторов
                ;esi = [vkCode]
               .NoTextTest:
                        mov  edx,aHookMinor;edx = HOOK_MINOR
                        xor  eax,eax;eax = 0

                        cmp  word[edx+HOOK_MINOR.iKeyModif],si;edx = HOOK_MINOR
                        jne  @f

                        mov  [edx+HOOK_MINOR.iKeyModif],eax;eax = 0;edx = HOOK_MINOR

                        @@:
                        cmp  word[edx+HOOK_MINOR.iKeyModif2],si;edx = HOOK_MINOR
                        jne  .NoText

                        mov  [edx+HOOK_MINOR.iKeyModif2],eax;eax = 0;edx = HOOK_MINOR

                        jmp  .NoText



 .JmpTable ddA  .NotCombination;0
           ddA  .OneModifiterOneKey;1
           ddA  .TwoModifiterOneKey;2
           ddA  .OneModifiterOneMouse;3
           ddA  .TwoModifiter;4

 endp



;Контролируем видимость или невидимость вкладки SYSTAB_CONTROL_KEYCHANGE_Id и ее устанавливаем функцию отвечающую за перемещения этой вкладки
;--------------------------------------------------------------------------------------
 proc ControlVisibleKeyChange  [edi],!nCmdShow

                   ;Если равно значит скрываем вкладку поэтому не меняем текущие функции отвечающие за перемещения и видимость
                  xor  edi,edi
                   or  edi,[!nCmdShow];edi = [!nCmdShow]
                   jz  .NextShowWindow;SW_HIDE = 0

                  ;Установим текущую видимую вкладку
                  mov  [aIniFileData.aIni.idTabCurent],SYSTAB_CONTROL_KEYCHANGE_Id

                  ;Подготовим
                  mov  eax,MoveTabKeyChange;eax = MoveTabKeyChange
                  mov  ecx,WindowProc.freeJmp;ecx = WindowProc.freeJmp
                  mov  edx,aTabCurrent;edx = TAB_CURRENT

                  ;Установим функцию перемещения отвечающую за текущую вкладку
                  mov  [edx+TAB_CURRENT.Move],eax;eax = MoveTabKeyChange;edx = TAB_CURRENT
                  mov  [edx+TAB_CURRENT.Paint],ecx;ecx = WindowProc.freeJmp;edx = TAB_CURRENT
                  mov  [edx+TAB_CURRENT.MouseMove],ecx;ecx = WindowProc.freeJmp;edx = TAB_CURRENT
                  mov  [edx+TAB_CURRENT.MouseLeftUpDown],ecx;ecx = WindowProc.freeJmp;edx = TAB_CURRENT
                  mov  [edx+TAB_CURRENT.Activate],ecx;ecx = WindowProc.freeJmp;edx = TAB_CURRENT

                 ;С помощью функции с кариктируем текущее положение вкладки перед ее отображением
              stdcall  eax;eax = MoveTabKeyChange

     .NextShowWindow:
               invoke  ShowWindow,[aArrayHwndFont.hwndListView],edi;edi = [!nCmdShow]
                  ret
 endp


;Контролируем видимость или невидимость вкладки SYSTAB_CONTROL_ABOUT_Id и ее устанавливаем функцию отвечающую за перемещения этой вкладки
;--------------------------------------
 proc ControlVisibleAbout  [edi ebp esi ebx],!nCmdShow

                   ;Если равно значит скрываем вкладку поэтому не меняем текущие функции отвечающие за перемещения и видимость
                  xor  edi,edi
                  xor  eax,eax;eax = 0
                   or  edi,[!nCmdShow];edi = [!nCmdShow]
                   jz  .NextShowWindow;SW_HIDE = 0

                  ;Загрузим bitmap для вкладки
               invoke  LoadImageW,[hInstance],IDR_BITMAP_ABOUT,eax,eax,eax,eax;eax = 0;IMAGE_BITMAP = 0;LR_DEFAULTCOLOR = 0
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                  mov  ebp,[hwnd];ebp = [hwnd]
                  mov  [aAboutFree.hbmpAbout],eax

               ;Загрузим нужные шрифты и подсчитаем сколько текст займет с ними места
               ;ebp = [hwnd]
               ;--------------------------------------
               invoke  GetDC,ebp;ebp = [hwnd]
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                  ;Подготовим
                  mov  edx,aFont;edx = LOGFONTW
                  mov  edi,eax;edi = hDc current
                  mov  [edx+LOGFONTW.lfHeight],30;edx =  LOGFONTW
                  mov  [edx+LOGFONTW.lfWeight],FW_SEMIBOLD;edx =  LOGFONTW

               ;Создадим нужный шрифт
               invoke  CreateFontIndirectW,edx;edx = LOGFONTW
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                  mov  [aAboutFree.hfont30SEMIBOLD],eax

               ;Освободим не нужный DC
               ;ebp = [hwnd];edi = hDc current
               invoke  ReleaseDC,ebp,edi;ebp = [hwnd];edi = hDc current
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                  ;Подготовим
                  mov  edx,aTabCurrent;edx = TAB_CURRENT

                  ;Установим текущую видимую вкладку
                  mov  [aIniFileData.aIni.idTabCurent],SYSTAB_CONTROL_ABOUT_Id

                  ;Установим функцию перемещения отвечающую за текущую вкладку
                  mov  [edx+TAB_CURRENT.Move],MoveTabAbout;edx = TAB_CURRENT
                  mov  [edx+TAB_CURRENT.Paint],WindowProc.AboutPaint;edx = TAB_CURRENT
                  mov  [edx+TAB_CURRENT.MouseMove],WindowProc.AboutMouseMove;edx = TAB_CURRENT
                  mov  [edx+TAB_CURRENT.MouseLeftUpDown],WindowProc.AboutMouseLeftUpDown;edx = TAB_CURRENT
                  mov  [edx+TAB_CURRENT.Activate],WindowProc.AboutActivate;edx = TAB_CURRENT
                  jmp  .Exit


     .NextShowWindow:
                 ;Подготовим
                 mov  esi,aAboutFree;esi = ABOUT_FREE
                 mov  ebp,[DeleteObject];ebp = [DeleteObject]

               ;Освобождаем ненужные ресурсы
              stdcall  ebp,[esi+ABOUT_FREE.hbmpAbout];ebp = [DeleteObject];esi = ABOUT_FREE
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              stdcall  ebp,[esi+ABOUT_FREE.hfont30SEMIBOLD];ebp = [DeleteObject];esi = ABOUT_FREE
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

               .Exit:
                  ret
 endp

;Рисует текс и фон нажатай клавиши или же востановит исходное состояние не нажатой, в зависимости от цветов в аргументах
;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbBrush - цвет для кисти;!rgbText - цвет для текста
;--------------------------------------
proc PaintBottomUpDown [ebp edi esi ebx],!lpRect,!rgbBrush,!rgbText
                ;Получим Dc
                mov  ebp,[hwnd];ebp = Хендл окна
             invoke  GetDC,ebp;ebp = Хендл окна
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Подготовим
                mov  esi,aSysStock;esi = SYS_STOCK
                mov  edi,eax;edi = current Dc
                mov  ebx,[SelectObject];ebx = [SelectObject]

             ;Выбирим стандартную кисть
             ;ebp = Хендл окна;edi = current Dc;ebx = [SelectObject];esi = SYS_STOCK
            stdcall  ebx,edi,[esi+SYS_STOCK.Brush];edi = current Dc;ebx = [SelectObject];esi = SYS_STOCK
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

             ;Установим шрифт
             ;ebp = Хендл окна;edi = current Dc;ebx = [SelectObject];esi = SYS_STOCK
            stdcall  ebx,edi,[esi+SYS_STOCK.hfont];edi = current Dc;ebx = [SelectObject];esi = SYS_STOCK
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Подготовим
                mov  ebx,[!rgbText];ebx = [!rgbText]
                mov  esi,[!lpRect];esi = ABOUT_ARRAY_ZONA

             ;Установим цвет кисть
             ;ebp = Хендл окна;edi = current Dc;ebx = [!rgbText];esi = ABOUT_ARRAY_ZONA
             invoke  SetDCBrushColor,edi,ebx;edi = current Dc;ebx = [!rgbText]
                     _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

             ;Установим цвет текста
             ;ebp = Хендл окна;edi = current Dc;ebx = [!rgbText];esi = ABOUT_ARRAY_ZONA
             invoke  SetTextColor,edi,ebx;edi = current Dc;ebx = [!rgbText]
                     _LastErrorNotSigned;;Если не минус возращает функция то ошибки нет

             ;Установим цвет фона
             ;ebp = Хендл окна;edi = current Dc;esi = ABOUT_ARRAY_ZONA
             invoke  SetBkColor,edi,[!rgbBrush];edi = current Dc
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Подготовим
                mov  ecx,[esi+ABOUT_ARRAY_ZONA.lpRectText];ecx = ABOUT_RECT_PAINT.xxx;esi = ABOUT_ARRAY_ZONA

             ;Рисует текс и фон
             ;ebp = Хендл окна;edi = current Dc;esi = ABOUT_ARRAY_ZONA
             invoke  ExtTextOutW,edi,[esi+ABOUT_ARRAY_ZONA.StringLeft],[ecx+RECT.top],ETO_OPAQUE,esi,[esi+ABOUT_ARRAY_ZONA.lpText],\
                                [esi+ABOUT_ARRAY_ZONA.iLeght],0;edi = current Dc;ecx = ABOUT_RECT_PAINT.xxx;esi = ABOUT_ARRAY_ZONA.Rect
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            ;Рисуем стрелочку по координатам в массиве POINT
            ;На входе:;!hDc - контекст устройства где рисуем;!lpRect - указатель на RECT и массив из точек следующий за ним
            ;ebp = Хендл окна;edi = current Dc;esi = ABOUT_ARRAY_ZONA
            stdcall  PaintArrow,edi,esi;edi = current Dc;esi = ABOUT_ARRAY_ZONA

             ;Освобождаем Dc
             ;ebp = Хендл окна;edi = hDc current
             invoke  ReleaseDC,ebp,edi;ebp = Хендл окна;edi = hDc current
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                ret
endp

;Рисуем стрелочку по координатам в массиве POINT
;На входе:;!hDc - контекст устройства где рисуем;!lpRect - указатель на RECT и массив из точек следующий за ним
;--------------------------------------
proc PaintArrow [ebx],!hDc,!lpRect
       ;Подготовим
       mov  ebx,[!hDc];ebx = [!hDc]

    ;Начинаем путь
    invoke  BeginPath,ebx;ebx = [!hDc]
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       ;Подготовим
       mov  edx,[!lpRect]
       add  edx,ABOUT_ARRAY_ZONA.Arrow

    ;Рисуем стрелку
    invoke  Polyline,ebx,edx,10;ebx = [!hDc]
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

    ;Закончим путь
    invoke  EndPath,ebx;ebx = [!hDc]
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

    ;Зайлем путь
    invoke  FillPath,ebx;ebx = [!hDc]
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
       ret
endp







;Рисует рамку для прямоугольника на основании  5 точек с помощью стандартного pen и цвета полученого от пользователя.
;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbPen - цвет для pen который будет использован для рисования рамки
;--------------------------------------
proc PaintBottomFrame [ebp edi],!lpRect,!rgbPen
                ;Получим Dc
                mov  ebp,[hwnd];ebp = Хендл окна
             invoke  GetDC,ebp;ebp = Хендл окна
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;Подготовим
                mov  edi,eax;edi = current Dc

             ;Выбирим в Dc стандартный Pen
             ;ebp = Хендл окна;edi = current Dc
             invoke  SelectObject,edi,[aSysStock.Pen];edi = current Dc
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

             ;Установим нужный нам цвет
             ;ebp = Хендл окна;edi = current Dc
             invoke  SetDCPenColor,edi,[!rgbPen];edi = current Dc
                     _LastErrorNotSigned;Если не минус возращает функция то ошибки нет

                ;Каректируем что бы указывало на нужный массив точек
                mov  edx,[!lpRect]
                add  edx,sizeof.RECT;edx = ABOUT_ARRAY_ZONA

             ;Рисуем рамку вокруг строки
             ;ebp = Хендл окна;edi = current Dc;edx = ABOUT_ARRAY_ZONA.POINT
             invoke  Polyline,edi,edx,5;edi = current Dc;edx = aAboutZonaButton.POINT
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

             ;Освобождаем Dc
             ;ebp = Хендл окна;edi = hDc current
             invoke  ReleaseDC,ebp,edi;ebp = Хендл окна;edi = hDc current
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                ret
endp

;Обрабатываем таймер отвечающий за медленное исчезновение рамки вокруг текста для вкладки About
;--------------------------------------------------------------------------------------
proc TimerBottomClean !hwnd,!uMsg,!idEvent,!dwTime

     ;Подготовим
     mov  edx,[!idEvent];edx = ABOUT_ARRAY_ZONA и idEvent
     mov  eax,[edx+ABOUT_ARRAY_ZONA.Color];edx = ABOUT_ARRAY_ZONA и idEvent
     mov  ecx,[edx+ABOUT_ARRAY_ZONA.Adjustment];edx = ABOUT_ARRAY_ZONA и idEvent

      ;Если равно отрицателльному числу значит уже таймер удален, а это сообщение просто уже попало в очередь сообщение - поэтому нечего неделая выходим
      ;Если равно нулю значит это наше последнее обрабатываемое сообщение так что удаляем таймер и востанавливаем фон
      ;eax = [ABOUT_ARRAY_ZONA.Color];ecx = [ABOUT_ARRAY_ZONA.Adjustment];edx = ABOUT_ARRAY_ZONA и idEvent;edx = 0
     dec  [edx+ABOUT_ARRAY_ZONA.Indicator];edx = ABOUT_ARRAY_ZONA и idEvent
      js  .Exit;edx = ABOUT_ARRAY_ZONA и idEvent
      jz  .KillTimer;edx = ABOUT_ARRAY_ZONA и idEvent


     ;Подготовим
     add  eax,ecx;eax = [ABOUT_ARRAY_ZONA.Color];ecx = [ABOUT_ARRAY_ZONA.Adjustment]
     add  [edx+ABOUT_ARRAY_ZONA.Color],ecx;edx = ABOUT_ARRAY_ZONA и idEvent;ecx = [ABOUT_ARRAY_ZONA.Adjustment]

 ;Рисует рамку для прямоугольника на основании  5 точек с помощью стандартного pen и цвета полученого от пользователя.
 ;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbPen - цвет для pen который будет использован для рисования рамки
 ;edx = ABOUT_ARRAY_ZONA;eax = [ABOUT_ARRAY_ZONA.Color]
 stdcall  PaintBottomFrame,edx,eax;eax = [rgbBTNFACE];edx = ABOUT_ARRAY_ZONA и idEvent
     ret

 ;Последний раз рисуем рамку поэтому также удаляем таймер
 ;edx = ABOUT_ARRAY_ZONA и idEvent
.KillTimer:
 ;Рисует рамку для прямоугольника на основании  5 точек с помощью стандартного pen и цвета полученого от пользователя.
 ;На входе:!lpRect - указатель на RECT и массив из точек следующий за ним;!rgbPen - цвет для pen который будет использован для рисования рамки
 ;edx = ABOUT_ARRAY_ZONA
 stdcall  PaintBottomFrame,edx,[aSysStock.rgbBTNFACE];edx = ABOUT_ARRAY_ZONA и idEvent

  invoke  KillTimer,[!hwnd],[!idEvent]
          _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
     ret

  ;Востановим до нуля
  ;edx = ABOUT_ARRAY_ZONA и idEvent
  .Exit:
     inc  [edx+ABOUT_ARRAY_ZONA.Indicator]
     ret
endp


;Контролируем перемещение вкладки SYSTAB_CONTROL_KEYCHANGE_Id
;--------------------------------------
proc MoveTabKeyChange  [edi esi ebx ebp]

                ;Подготовим
                mov  edx,aClientRect;edx = RECT
                mov  ebx,[aArrayHwndFont.hwndListView];ebx = [aArrayHwndFont.hwndListView]
                mov  ebp,[SendMessageW];ebp = [SendMessageW]
                mov  edi,[edx+RECT.right];edi = Width client;edx = RECT
                mov  esi,[edx+RECT.bottom];esi = Height client;edx = RECT


;List View
;ebx = [aArrayHwndFont.hwndListView];ebp = [SendMessageW] ;edi = Width client;esi = Height client
;--------------------------------------
                ;Узнаем новые координаты дочернего окна
                lea  ecx,[esi-BUTTON_EXIT_Height-SYSTAB_CONTROL_Height];esi = Height client
             invoke  SetWindowPos,ebx,0,0,SYSTAB_CONTROL_Height,edi,ecx,SWP_NOZORDER;ebx = [aArrayHwndFont.hwndListView]
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                ;List View  Colum
                shr  edi,1
             stdcall  ebp,ebx,LVM_SETCOLUMNWIDTH,LIST_VIEW_COLUM_ORIGINAL_Id,edi;ebx = [aArrayHwndFont.hwndListView];ebp = [SendMessageW]
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

             stdcall  ebp,ebx,LVM_SETCOLUMNWIDTH,LIST_VIEW_COLUM_NEW_Id,edi;ebx = [aArrayHwndFont.hwndListView];ebp = [SendMessageW]
                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

      ret
endp


;Контролируем перемещение вкладки SYSTAB_CONTROL_ABOUT_Id и ее отрисовку частичную
;--------------------------------------
proc MoveTabAbout



      ret
endp


;Так как окно может быть видимо но закрыто поэтому перебираем все окна с помощью
;EnumWindows и если RECT окна нашего и получененых пересикаються значит только выводим на передний план
;если же таких окон нет и доходим в поиски до нашего главного значит скрываем наше главное окно
;--------------------------------------
 proc EnumWindowsProc  [ebp edi ebx],!Hwnd,!Lparam
          locals
           !aEnumSeash                  RECT
          endl

                  mov  ebx,[hwnd];ebx = Хендл окна основного окна
                  mov  ebp,[!Hwnd];ebp = [!Hwnd]
                  lea  edi,[!aEnumSeash];edi = [!aEnumRect]

                  ;может наше акно основное - тогда выходим из цикла и скрываем окно
                  ;ebp = [!Hwnd];edi = [!aEnumRect] ;ebx = Хендл окна основного окна
                  cmp  ebp,ebx ;ebx = Хендл окна основного окна
                   je  .Hide;ebx = Хендл окна основного окна

                  ;Если окно от панели задач - продолжаем поиск
                  ;ebp = [!Hwnd];ebx = Хендл окна основного окна
                  cmp  ebp,[hwndShell_Tray]
                   je  .True

               ;Если окно не видимо значит ищим дальше
               ;ebp = [!Hwnd];edi = [!aEnumRect];ebx = Хендл окна основного окна
               invoke  IsWindowVisible,ebp;ebp = [!Hwnd]
                 test  eax,eax
                   jz  .True

               ;Если у окна стиль WS_EX_TOPMOST то ищем дальше
               ;ebp = [!Hwnd];edi = [!aEnumRect];ebx = Хендл окна основного окна
               invoke  GetWindowLongW,ebp,GWL_EXSTYLE
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 test  eax,WS_EX_TOPMOST
                  jnz  .True

               invoke  GetWindowRect,ebp,edi;ebp = [!Hwnd];edi = [!aEnumSeash]
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              ;Проверяем пересекаеться ли RECT в !lprcSrc1 с массивам RECT в !lprcSrc2;!nCount - количество в RECT массиве;Если eax = 0 тогда пересекаються
              ;ebp = [!Hwnd];edi = [!aEnumRect];ebx = Хендл окна основного окна
              stdcall  RectInRectHL,aWindowRect,edi,1;edi = [!aEnumSeash]
                 ;Если не пересекаються то дальше ищем
                 test  eax,eax
                  jnz  .True

               ;выходим из цикла и выводим окно на передний план
               invoke  SetForegroundWindow,ebx;ebx = Хендл окна основного окна

                  jmp  .ExitLoop

               ;продолжаем поиск окон
               .True:
                  mov  eax,1
                  jmp  .Exit

                ;скрываем окно
                ;ebx = Хендл окна основного окна
               .Hide:
               invoke  ShowWindow,ebx,SW_HIDE;ebx = Хендл окна основного окна;SW_HIDE = 0

            ;выходим из цикла
           .ExitLoop:
                  xor  eax,eax

               .Exit:
                  ret
 endp






;За гружает нужный язык по !idlang - id языка для загрузки например COMBOBOX_ENGLISH
;Если не удаеться загрузить то eax = 0
;--------------------------------------
 proc LoadLang [ebp esi edi ebx],!idlang;!idlang - id языка для загрузки например COMBOBOX_ENGLISH
  locals
   !aSizeFile                  !LARGE_INTEGER; сюда получаем размер файла
   !NumberOfBytesRead          dd  ?
  endl

               mov  ebx,[!idlang];ebx = [!idlang]
               mov  edi,tbMultiLanguage
               cmp  ebx,COMBOBOX_ENGLISH;Проверяем может это язык по умолчанию и соответственно его загружать не надо
                je  .DefaultLang;edi = ссылка на язык;ebx = [!idlang]

               ;получаем путь к нужному языку
               ;ebx = [!idlang]
               mov  eax,[ebx*4+tbLanguageTable];ebx = [!idlang]
            invoke  CreateFileW,eax,FILE_GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0
               mov  ebp,eax;ebp = хендл нашего файла
               inc  eax;INVALID_HANDLE_VALUE
                jz  .Skip;не удаеться открыть  файл

               ;Получим размер файла, что бы знать сколько выделить памяти
               ;ebp = хендл нашего  файла;ebx = [!idlang]
               lea  eax,[!aSizeFile]
            invoke  GetFileSizeEx,ebp,eax;ebp = хендл нашего  файла
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

               ;Подготовим
               mov  esi,[!aSizeFile.LowPart];esi = размер файла

            ;Выделим память под файл
            ;ebp = хендл нашего  файла;esi = размер файла;ebx = [!idlang]
            invoke  HeapAlloc,[hheap],0,esi;esi = размер файла
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

               ;Подготовим
               lea  edx,[!NumberOfBytesRead];edx = !NumberOfBytesRead
               mov  edi,eax;edi = ссылка на язык

            ;читаем файл в память
            ;esi= размер файла;ebp = хендл нашего;edi = ссылка на язык;ebx = [!idlang];edx = !NumberOfBytesRead
            invoke  ReadFile,ebp,edi,esi,edx,0;esi= размер файла;ebp = хендл нашего;edi = ссылка на язык;edx = !NumberOfBytesRead
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            ;Закроем хендл ненужного уже файла
            ;esi= размер файла;ebp = хендл нашего;edi = ссылка на язык;ebx = [!idlang]
            invoke  CloseHandle,ebp;ebp = хендл нашего файла
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

               ;учитываем то что в конце файла 4 байта занимает  CRC32c
               sub  esi,4;esi= размер файла

           ;CRC32c - 0x82F63B78 -полином  проверяем контрольную сумму файла
          ;esi= размер файла;edi = ссылка на язык;ebx = [!idlang]
           stdcall  CRC32cHL,edi,esi,-1;edi = ссылка на язык
               xor  ecx,ecx

               ;Проверим правельный ли CRC32c
               cmp  [edi+esi],eax;edi = ссылка на язык
            cmovne  ecx,esp
               ;Проверим может версия языка не совместима с текущей версийе приложения
               cmp  dword[edi+esi-4],equVersionLang;edi = ссылка на язык
            cmovne  ecx,esp

              test  ecx,ecx
                jz  .DefaultLang;edi = ссылка на язык;ebx = [!idlang]

            ;Освободим память
            ;edi = ссылка на язык
            invoke  HeapFree,[hheap],0,edi;edi = ссылка на язык
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            ;не удалось язык загрузить
            .Skip:
               xor  eax,eax
               jmp  .Exit


     ;язык по умолчанию
     ;edi = ссылка на язык;ebx = [!idlang]
     .DefaultLang:
               ;Подготовим
               mov  ecx,[lpMultiLanguage];ecx = [lpMultiLanguage]
               xor  eax,eax;eax = 0

               ;Если предыдущий язык это язык по умолчанию память не освобождаем
               cmp  ecx,tbMultiLanguage
                je  .NotFree;edi = ссылка на язык;ebx = [!idlang]

             ;Освободим память от предыдущего языка
            invoke  HeapFree,[hheap],eax,ecx;ecx = [lpMultiLanguage];eax = 0
                    _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          ;Установим выбраный язык
          ;edi = ссылка на язык;ebx = [!idlang]
         .NotFree:
               mov  [aIniFileData.aIni.Language],ebx;ebx = [!idlang]
               mov  [lpMultiLanguage],edi;edi = ссылка на язык

               ;все хорошо
               mov  eax,esp

            .Exit:
               ret
 endp


;Устанавливает текст в контролы и везде где надо
;--------------------------------------
 proc SetText [ebp esi edi ebx]
  locals
   !aSetTcItem            TCITEMW;set item for SysTab Control
   !aSetColum             LVCOLUMNW
  endl
              xor  eax,eax
              mov  esi,[SendMessageW];esi = [SendMessageW]
              mov  ebp,[lpMultiLanguage];ebp =[lpMultiLanguage]
              mov  edi,LoadResoursIndirectHL;edi = LoadResoursIndirectHL


               ;Проверяем меню надо удалять или нет;Тем же самым проверяем это первый запуск этой функции или нет - если первый то пропускаем эти действия
               or  eax,[hmenu]
               jz  .Next;esi = [SendMessageW];ebp =[lpMultiLanguage];edi = LoadResoursIndirectHL

;Перед загрузкой меню с другим языком уничтожаем старае
;esi = [SendMessageW];ebp =[lpMultiLanguage];edi = LoadResoursIndirectHL
;--------------------------------------
           invoke  DestroyMenu,eax;eax = [hmenu]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          ;обновляем текст значка в треи
          ;Добавляет иконку или изменяенти подсказку к ней в зависимости от языка и активности главного хука
          ;На входе:;!dwMessage = NIM_ADD или NIM_MODIFY для Shell_NotifyIcon
          stdcall  ChangeNotifyIcon,NIM_MODIFY

;Проверим может версия не зарегестрированая и тогда надо обновить  загаловак главного окна
;esi = [SendMessageW];ebp =[lpMultiLanguage];edi = LoadResoursIndirectHL
;--------------------------------------
              ;Если не равно ноль значит не надо изменять
              cmp  [lpBuyNameUser],0
              jne  .Next;esi = [SendMessageW];ebp =[lpMultiLanguage];edi = LoadResoursIndirectHL

          stdcall  edi,ebp,RT_STR,STR_WINDOW_NOT_LICENSE;edi = LoadResoursIndirectHL;ebp =[lpMultiLanguage]
              mov  ebx,eax;ebx = lpStr

          ;Изменим загаловак
          stdcall  esi,[hwnd],WM_SETTEXT,0,eax;esi = [SendMessageW]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          ;Изменим также текст статика раз не зарегестированна
          stdcall  esi,[aArrayHwndFont.hwndStatic],WM_SETTEXT,0,ebx;esi = [SendMessageW];ebx = lpStr
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет



;Обнавляем текст в кладках SysTab Control
;esi = [SendMessageW];ebp =[lpMultiLanguage];edi = LoadResoursIndirectHL
;--------------------------------------
           .Next:
          stdcall  edi,ebp,RT_STR,STR_SYSTAB_CONTROL_KeyСhange;edi = LoadResoursIndirectHL;ebp =[lpMultiLanguage]

              lea  edx,[!aSetTcItem];edx = [!aSetTcItem]
              mov  ebx,[aArrayHwndFont.hwndSysTab];ebx = [hwndSysTab]
              mov  [edx+TCITEMW.mask],TCIF_TEXT
              mov  [edx+TCITEMW.pszText],eax
          stdcall  esi,ebx,TCM_SETITEMW,SYSTAB_CONTROL_KEYCHANGE_Id,edx;esi = [SendMessageW];ebx = [hwndSysTab] ;edx = [!aSetTcItem]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          stdcall  edi,ebp,RT_STR,STR_SYSTAB_CONTROL_About;edi = LoadResoursIndirectHL;ebp =[lpMultiLanguage]

              lea  edx,[!aSetTcItem]
              mov  [edx+TCITEMW.pszText],eax
          stdcall  esi,ebx,TCM_SETITEMW,SYSTAB_CONTROL_ABOUT_Id,edx;esi = [SendMessageW];ebx = [hwndSysTab] ;edx = [!aSetTcItem]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет



;Обнавляем текст в ListView
;esi = [SendMessageW];ebp =[lpMultiLanguage];edi = LoadResoursIndirectHL
;--------------------------------------
          stdcall  edi,ebp,RT_STR,STR_LIST_VIEW_COLUM_ORIGINAL;edi = LoadResoursIndirectHL;ebp =[lpMultiLanguage]

              lea  edx,[!aSetColum];edx = [!aSetColum]
              mov  ebx,[aArrayHwndFont.hwndListView];ebx = [hwndListView]
              mov  [edx+LVCOLUMNW.mask],LVCF_TEXT
              mov  [edx+LVCOLUMNW.pszText],eax
          stdcall  esi,ebx,LVM_SETCOLUMNW,LIST_VIEW_COLUM_ORIGINAL_Id,edx;esi = [SendMessageW];ebx = [hwndListView];edx = [!aSetColum]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


          stdcall  edi,ebp,RT_STR,STR_LIST_VIEW_COLUM_NEW;edi = LoadResoursIndirectHL;ebp =[lpMultiLanguage]

              lea  edx,[!aSetColum];edx = [!aSetColum]
              mov  [edx+LVCOLUMNW.pszText],eax
          stdcall  esi,ebx,LVM_SETCOLUMNW,LIST_VIEW_COLUM_NEW_Id,edx;esi = [SendMessageW];ebx = [hwndListView];edx = [!aSetColum]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


;Устанавливаем техт в кнопки - контролы
;esi = [SendMessageW];ebp =[lpMultiLanguage];edi = LoadResoursIndirectHL
;--------------------------------------
              xor  ebx,ebx;ebx = 0

          stdcall  edi,ebp,RT_STR,STR_BUTTON_Exit;edi = LoadResoursIndirectHL;ebp =[lpMultiLanguage]
          stdcall  esi,[aArrayHwndFont.hwndButtonExit],WM_SETTEXT,ebx,eax;esi = [SendMessageW];ebx = 0
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

          stdcall  edi,ebp,RT_STR,STR_BUTTON_Options;edi = LoadResoursIndirectHL;ebp =[lpMultiLanguage]
          stdcall  esi,[aArrayHwndFont.hwndButtonOptions],WM_SETTEXT,ebx,eax;esi = [SendMessageW];ebx = 0
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


;Загружаем меню
;ebp =[lpMultiLanguage];edi = LoadResoursIndirectHL
;--------------------------------------
          stdcall  edi,ebp,RT_MENUS,MENUS_POPUP_Notify;edi = LoadResoursIndirectHL;ebp =[lpMultiLanguage]
           invoke  LoadMenuIndirectW,eax
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
              mov  [hmenu],eax
              mov  esi,[GetSubMenu];esi = [GetSubMenu]
              mov  ebx,eax;ebx = [hmenu]

;Получаем субменю иконки в трее
          stdcall  esi,ebx,MENU_NOTIFU_Id;esi = [GetSubMenu];ebx = [hmenu]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
              mov  [hmenuSubPopupNotify],eax

;Получаем субменю для работы с итеми в List View
          stdcall  esi,ebx,MENU_ADDITEM_Id;esi = [GetSubMenu];ebx = [hmenu]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              ;Подготовим
              mov  edx,aMenuState;edx = MENU_STATE
              xor  ecx,ecx;ecx = 0

              mov  [hmenuSubAddItem],eax

              ;обнуляем счетчик состояния пункта меню так как загружаем новое меню
              ;edx = FLAG;ecx = 0
              mov  [edx+MENU_STATE.Item],ecx;edx = MENU_STATE;ecx = 0
              mov  [edx+MENU_STATE.Group],ecx;edx = MENU_STATE;ecx = 0
              mov  [edx+MENU_STATE.NotifySnowHide],ecx;edx = MENU_STATE;ecx = 0

              ret
 endp

;Добавляет иконку или изменяенти подсказку к ней в зависимости от языка и активности главного хука
;На входе:;!dwMessage = NIM_ADD или NIM_MODIFY для Shell_NotifyIcon
;--------------------------------------------------------------------------------------
proc ChangeNotifyIcon !dwMessage

       ;Подготовим
       mov  edx,[aIniFileData.aIni.iSuspendPlayHook]
       mov  eax,STR_NOTIFY_ICON_Activate
       mov  ecx,STR_NOTIFY_ICON_Suspend

       ;ПРоверим сейчас активен главный хук или нет, что бы правильно отобразить подсказку над значком
       cmp  edx,[hiconPlay]
    cmovne  eax,ecx

   ;Загружает авторские ресурсы
   ;На выхоже:eax = указатель на конечные данные;ecx = длина строки в символах  без завершающего нуля
   stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,eax

       ;Подготовим
       xor  ecx,ecx;ecx = 0
       mov  edx,aNotifyIconData.szTip

       ;Скопируем текст подсказки
       @@:
       mov  cx,word[eax]
       add  eax,2
       mov  word[edx],cx
       add  edx,2
      test  ecx,ecx;ecx = 0
       jnz  @b

       ;Подготовим
       mov  eax,NIF_ICON or NIF_MESSAGE or NIF_TIP
       mov  edx,NIF_TIP

       ;Проверим какое действие над иконкой надо сделать и с учетам этого раставим флаги в структуре
        or  ecx,[!dwMessage];ecx = 0;NIM_ADD = 0
    cmovnz  eax,edx

       ;Установим флаги
       mov  [aNotifyIconData.uFlags],eax

    invoke  Shell_NotifyIconW,ecx,aNotifyIconData
           _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       ret
endp


;--------------------------------------------------------------------------------------
 proc UnhandledExceptionFilter  [esi edi ebp ebx],!ExceptionInfo
         locals
          !countLoop           dd  ?;Сколько цикл делать
          !NameModule          dw  MAX_PATH dup(?);Путь к модулю где исключение и загаловак
          !ExceptionMessage    VectoredExceptionHandler;Структура для  регистрав и т.д
          !StackHex            dw 0x333 dup(?);Для остального всего максимально сколько можно, так что при добавлении новых функций надо быть акурратным
          !BufferString        dd  MAX_PATH/2 dup(?);Временное для пути к модулю в стеке
         endl

         ;Подготовим
         mov  eax,[!ExceptionInfo]
         mov  ebp,HexToUnicodHL;ebp = HexToUnicodHL
         mov  esi,[eax+EXCEPTION_POINTERS.ContextRecord];esi = ContextRecord
         mov  edi,[eax+EXCEPTION_POINTERS.ExceptionRecord];edi = ExceptionRecord

         ;Проверим может анти отладочное исключени
         cmp  [esi+CONTEXTx86.Eip],ThreadProcForHook.lpExceptionGuardian;esi = ContextRecord
          je  .NotDbg;esi = ContextRecord

     ;Преобразуем значение всех обычных регистров в Юникод по 16 системе исчисления цифр сохраним в структуру отладки
     ;ebp = HexToUnicodHL;esi = ContextRecord;edi = ExceptionRecord
     stdcall  ebp,[esi+CONTEXTx86.Eax],aVectoredExceptionHandler.MessageEax;ebp = HexToUnicodHL;esi = ContextRecord
     stdcall  ebp,[esi+CONTEXTx86.Ecx],aVectoredExceptionHandler.MessageEcx;ebp = HexToUnicodHL;esi = ContextRecord
     stdcall  ebp,[esi+CONTEXTx86.Edx],aVectoredExceptionHandler.MessageEdx;ebp = HexToUnicodHL;esi = ContextRecord
     stdcall  ebp,[esi+CONTEXTx86.Ebx],aVectoredExceptionHandler.MessageEbx;ebp = HexToUnicodHL;esi = ContextRecord
     stdcall  ebp,[esi+CONTEXTx86.Esi],aVectoredExceptionHandler.MessageEsi;ebp = HexToUnicodHL;esi = ContextRecord
     stdcall  ebp,[esi+CONTEXTx86.Edi],aVectoredExceptionHandler.MessageEdi;ebp = HexToUnicodHL;esi = ContextRecord
     stdcall  ebp,[esi+CONTEXTx86.Ebp],aVectoredExceptionHandler.MessageEbp;ebp = HexToUnicodHL;esi = ContextRecord
         mov  esi,[esi+CONTEXTx86.Esp];esi = CONTEXTx86.Esp
     stdcall  ebp,esi,aVectoredExceptionHandler.MessageEsp;ebp = HexToUnicodHL;esi = CONTEXTx86.Esp

     ;Преобразуем остальные значения в Юникод по 16 системе исчисления цифр сохраним в структуру отладки
     ;esi = CONTEXTx86.Esp;ebp = HexToUnicodHL;edi = ExceptionRecord
     stdcall  ebp,[edi+EXCEPTION_RECORD.ExceptionCode],aVectoredExceptionHandler.ExceptionCode;ebp = HexToUnicodHL;edi = ExceptionRecord
     stdcall  ebp,[edi+EXCEPTION_RECORD.ExceptionFlags],aVectoredExceptionHandler.ExceptionFlags;ebp = HexToUnicodHL;edi = ExceptionRecord
     stdcall  ebp,[edi+EXCEPTION_RECORD.ExceptionRecord],aVectoredExceptionHandler.ExceptionRecord;ebp = HexToUnicodHL;edi = ExceptionRecord
     stdcall  ebp,[hInstance],aVectoredExceptionHandler.BaseOfApp;ebp = HexToUnicodHL

         ;Подготовим
         ;esi = CONTEXTx86.Esp;ebp = HexToUnicodHL;edi = ExceptionRecord
         mov  ebx,[edi+EXCEPTION_RECORD.ExceptionAddress];ebx = ExceptionAddress;edi = ExceptionRecord
         lea  edi,[!NameModule];[edi] = lpNameModule

     ;Преобразуем адрес исключени в Юникод по 16 системе исчисления цифр сохраним в структуру отладки
     ;esi = CONTEXTx86.Esp;ebp = HexToUnicodHL;ebx = ExceptionAddress;[edi] = lpNameModule
     stdcall  ebp,ebx,aVectoredExceptionHandler.ExceptionAddress;ebp = HexToUnicodHL;ebx = ExceptionAddress

      ;Получим базу модуля с ошибкой
      ;esi = CONTEXTx86.Esp;ebp = HexToUnicodHL;ebx = ExceptionAddress;[edi] = lpNameModule
      invoke  GetModuleHandleExW,GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS or GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,ebx,edi;ebx = ExceptionAddress;[edi] = lpNameModule
        test  eax,eax
          jz  .ErrorModuleException;esi = CONTEXTx86.Esp;ebp = HexToUnicodHL

         ;Подготовим
         ;esi = CONTEXTx86.Esp;ebp = HexToUnicodHL;[edi] = lpNameModule
         mov  ebx,[edi];ebx = BaseOfModule;[edi] = lpNameModule

     ;Преобразуем адрес модуля приведшего к исключению в Юникод по 16 системе исчисления цифр сохраним в структуру отладки
     ;esi = CONTEXTx86.Esp;ebp = HexToUnicodHL;[edi] = lpNameModule;ebx = BaseOfModule
     stdcall  ebp,ebx,aVectoredExceptionHandler.BaseOfModule;ebp = HexToUnicodHL;ebx = BaseOfModule

      ;Получим путь и имя модуля приведшего к исключению
      ;esi = CONTEXTx86.Esp;ebp = HexToUnicodHL;[edi] = lpNameModule;ebx = BaseOfModule
      invoke  GetModuleFileNameW,ebx,edi,sizeof.!NameModule/2;ebx = BaseOfModule;[edi] = lpNameModule
         ;Проверим если ошибкой завершилась  то обнулим начало загаловка MessageBoxExWчто бы  все правильно отобразилась
         mov  cx,word[edi];[edi] = lpNameModule
        test  eax,eax
       cmovz  ecx,eax
         mov  word[edi],cx;[edi] = lpNameModule


 ;Проведем раскрутку стека в 9 позиций
 ;esi = CONTEXTx86.Esp
 ;--------------------------------------
.ErrorModuleException:
         ;Подготовим
         mov  [!countLoop],9
         lea  edi,[!StackHex];edi = [!StackHex]

  ;Развертваем стек в цикле
  ;'0x',8 dup(?),' ','0x'8 dup(?),0xD,0xA;В конце еще надо добавить ,0xD,0xA
  ;esi = CONTEXTx86.Esp;edi = !StackHex
 .loopStack:
         ;Подготовим
         lea  ebx,[!BufferString];ebx = !BufferString

         ;Текст
         mov  word[edi],'0';edi = !StackHex
         mov  word[edi+2],'x';edi = !StackHex
         add  edi,4;edi = !StackHex
         mov  ebp,[esi];ebp = number stack

     ;Преобразуем сначала хранящиеся числа в стеке в Юникод по 16 системе исчисления цифр сохраним в структуру отладки
     ;esi = CONTEXTx86.Esp;edi = !StackHex;ebx = !BufferString;ebp = number stack
     stdcall  HexToUnicodHL,ebp,edi;ebp = number stack;edi = !StackHex

         ;Текст
         xor  eax,eax;eax = 0
         mov  word[edi+8*2],' ';edi = !StackHex
         mov  word[edi+8*2+2],'0';edi = !StackHex
         mov  word[edi+8*2+4],'x';edi = !StackHex
         add  edi,8*2+6;edi = !StackHex

         ;Проверим если число в стеке слишком маленькое то модуль нету смысла искать - обнулим
         cmp  ebp,0x10000;ebp = number stack
       cmovb  ebp,eax;eax = 0
          jb  .StackSmallNumber;esi = CONTEXTx86.Esp;edi = !StackHex;ebp = hInstance or null


      ;Получим базу модуля с ошибкой
      ;esi = CONTEXTx86.Esp;edi = !StackHex;ebx = !BufferString;ebp = number stack
      invoke  GetModuleHandleExW,GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS or GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,ebp,ebx;ebp = number stack;ebx = !BufferString
        ;Если завершилось ошибкой имя модуля нельзя искать  - обнулим
        test  eax,eax
       cmovz  ebp,eax;eax = 0
          jz  .StackSmallNumber;esi = CONTEXTx86.Esp;edi = !StackHex;ebp = [hInstance] or null

         ;Подготовим
         mov  ebp,[ebx];ebp = [hInstance];ebx = !BufferString

      ;Получим путь и имя модуля  в стеке
      ;esi = CONTEXTx86.Esp;edi = !StackHex;ebx = !BufferString;ebp = [hInstance]
      invoke  GetModuleFileNameW,ebp,ebx,sizeof.!BufferString/2;ebp = hInstance;ebx = !BufferString

         ;Подготовим
         lea  ebx,[ebx+eax*2];ebx = !BufferString

         ;Проверим  без ошибок и нет ли переполнения;Если есть чтото то строку по умолчанию
         dec  eax
         xor  ecx,ecx;ecx = 0
         cmp  eax,sizeof.!BufferString/2 - 1
       cmove  ebp,ecx;ecx = 0
       cmova  ebp,ecx;ecx = 0

 ;Преобразуем hInstance в Юникод по 16 системе исчисления цифр сохраним в структуру отладки
 ;esi = CONTEXTx86.Esp;edi = !StackHex;ebx = !BufferString;ebp = [hInstance] or null
.StackSmallNumber:
     stdcall  HexToUnicodHL,ebp,edi;ebp = [hInstance] or null;edi = !StackHex

         ;Текст
         mov  word[edi+8*2],0xD;edi = !StackHex
         mov  word[edi+8*2+2],0xA;edi = !StackHex
         add  edi,8*2+4;edi = !StackHex

         ;Проверим какие строки: по умолчанию или имя модуля в стеке;Если равно ноль то по умолчанию
         ;esi = CONTEXTx86.Esp;edi = !StackHex;ebx = !BufferString;ebp = [hInstance] or null
        test  ebp,ebp
          jz  .DefalString;esi = CONTEXTx86.Esp;edi = !StackHex;ebx = !BufferString

         ;Найдем начала имени модуля в стеке
         ;esi = CONTEXTx86.Esp;edi = !StackHex;ebx = !BufferString
         xor  eax,eax

         @@:
         sub  ebx,2
         inc  eax;eax = count
         cmp  word[ebx],'\';ebx = !BufferString
         jne  @b

         ;Копируем имя модуля с расширением
         @@:
         mov  cx,[ebx+2];ebx = !BufferString
         add  ebx,2
         mov  [edi],cx;edi = !StackHex
         add  edi,2
         dec  eax;eax = count
         jnz  @b

         ;Текст
         mov  word[edi-2],0xD;edi = !StackHex
         mov  word[edi],0xA;edi = !StackHex
         add  edi,2;edi = !StackHex

         jmp  .LoopTest;esi = CONTEXTx86.Esp;edi = !StackHex


 ;Строки по умолчанию
 ;esi = CONTEXTx86.Esp;edi = !StackHex;ebx = !BufferString
.DefalString:
         mov  word[edi],'.';edi = !StackHex
         mov  word[edi+2],'.';edi = !StackHex
         mov  word[edi+4],'.';edi = !StackHex
         mov  word[edi+6],0xD;edi = !StackHex
         mov  word[edi+8],0xA;edi = !StackHex
         add  edi,10;edi = !StackHex



   ;Проверяем надо ли продолжать цикл
   ;esi = CONTEXTx86.Esp;edi = !StackHex
  .LoopTest:
         ;Каректируем
         add  esi,4;esi = CONTEXTx86.Esp

         dec  [!countLoop]
         jnz  .loopStack;esi = CONTEXTx86.Esp;edi = !StackHex


         ;Каректируем
         mov  word[edi],0xD;edi = !StackHex
         mov  word[edi+2],0xA;edi = !StackHex
         mov  word[edi+4],0;edi = !StackHex


         ;Подготовим что бы отобразить сообщение об ошибки критичной
         ;--------------------------------------
         lea  edi,[!ExceptionMessage];edi = !ExceptionMessage

     ;Скопируем VectoredExceptionHandler  в стек для отображения с помощью MessageBoxExW
     ;edi = !ExceptionMessage
     ;На входе:;[!Destination] - Место назначения;[!Source] - Источник;[!Length] - Обьем копирования в байтах
     stdcall  MoveMemoryHL,edi,aVectoredExceptionHandler,sizeof.VectoredExceptionHandler;edi = !ExceptionMessage

         ;Отобразим
         ;edi = !ExceptionMessage
         lea  edx,[!NameModule];edx = [!NameModule]
      invoke  MessageBoxExW,[hwnd],edi,edx,MB_OK or MB_ICONSTOP or MB_APPLMODAL,0;edi = !ExceptionMessage;edx = [!NameModule]


      invoke  ExitProcess,eax

    ;Нет отладки исправим что бы работала и пометим что отладки нет но проверить на изменения код надо все таки
    ;esi = ContextRecord
   .NotDbg:
        xor  eax,eax
        mov  [esi+CONTEXTx86.Ebx],hEventThread;esi = ContextRecord
        dec  eax;EXCEPTION_CONTINUE_EXECUTION = 0xFFFFFFFF
        ret

 endp


;Что бы было видно использование  LoadLibraryExW и FreeLibrary
;--------------------------------------------------------------------------------------
proc TimerEmptyGuardian !hwnd,!uMsg,!idEvent,!dwTime

              ;Попробуем загрузить нашу Guardian.dll
              invoke  LoadLibraryExW,szRoamingAppData,0,0
              ;Если возврати возвратила ноль - отладчик подключен - будем предпринимать меры
                test  eax,eax
                  jz  .TakeAction

              ;Выгрузим dll
              invoke  FreeLibrary,eax
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              ;Выходим
             .Exit:
                mov  eax,1
                ret



        ;Примем меры
       .TakeAction:
                ret
endp

;Для  подставной проверки
;--------------------------------------------------------------------------------------
proc  CRC32cHLEmpty [esi edi ebx ],!Source,!Length,!FirstStep;0x82F63B78 таблица генерируеться макросом отдельно -  PolinomCRC32 8,0x82F63B78

            mov  esi,[!Source];Источник
            mov  ecx,[!Length];Обьем для которгого нужно посчитать CRC32
            mov  eax,[!FirstStep];Начало для CRC32 зачастую -1


           test  esi,3
           setz  dl
            cmp  ecx,11
          setae  dh
            shl  dl,1
            add  dl,dh
          movzx  ebx,dl
            jmp  dword[.Table + ebx*4]

 .PredBiglLoop:
            @@:
          movzx  edx,byte[esi]
            xor  dl,al
            inc  esi
            shr  eax,8
            dec  ecx
            xor  eax,[CRC32_0x82F63B78+edx*4]
           test  esi,3
            jnz  @b


     .BiglLoop:
            sub  ecx,8
          align  4
            @@:
            mov  ebx,eax
            mov  edx,[esi+4]
            xor  ebx,[esi]

          movzx  edi,dl
            mov  eax,[edi*4 + CRC32_0x82F63B78 + 1024*3]
          movzx  edi,dh
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*2]
            shr  edx,16
          movzx  edi,dl
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*1]
          movzx  edi,dh
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*0]

          movzx  edi,bl
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*7]
          movzx  edi,bh
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*6]
            shr  ebx,16
          movzx  edi,bl
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*5]
          movzx  edi,bh
            xor  eax,[edi*4 + CRC32_0x82F63B78 + 1024*4]


            add  esi,8
            sub  ecx,8
            jae  @b

            add  ecx,8
             jz  .Finish

    .SmallLoop:
            @@:
          movzx  edx,byte[esi]
            xor  dl,al
            inc  esi
            shr  eax,8
            xor  eax,[CRC32_0x82F63B78+edx*4]
            dec  ecx
            jnz  @b

       .Finish:
            not  eax
            ret

align 4
.Table:
       dd  .SmallLoop;0
       dd  .PredBiglLoop;1
       dd  .SmallLoop;2
       dd  .BiglLoop;3
endp
