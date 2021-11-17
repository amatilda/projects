;----------------------------------------------
 proc LowLevelMouseProc [ebp esi edi ebx],!nCode,!wParam,!lParam
  locals;Локальные переменые и сохраненые регистры должны быть одикавовы у: LowLevelKeyboardProc и LowLevelMouseProc, что бы можно было использовать .CmpGroup в двух этих функциях
   !lenghNameModule        dd  ?;Если = 0 значит еще активное окно не находили и буфере строки нет
   !iProcessId             dd  ?
   ;sizeof.INIFILEDATA.szOpenExeBufer/2 - что бы было одинаково с по длине с файлом открываем для групп, +1 учет на то что GetWindowModuleFileNameW возвращает длину без нуля
   !aBufferString          du  sizeof.INIFILEDATA.szOpenExeBufer/2 + 1 dup(?)
  endl;Локальные переменые и сохраненые регистры должны быть одикавовы у: LowLevelKeyboardProc и LowLevelMouseProc, что бы можно было использовать .CmpGroup в двух этих функциях

      mov  eax,[!nCode];если не равно нулю должны пропустить
      mov  esi,[!wParam];esi = [!wParam]
      mov  ebx,[!lParam];ebx = [!lParam]

      ;Просто движение мыши пропускаем однозначно;Меньше проверяем так как был один баг на переходах от мыши может сообщение отрицательное или еще что
      cmp  esi,WM_MOUSEMOVE;esi = [!wParam]
   cmovle  eax,esp
      ;Провераем KBDLLHOOKSTRUCT.dwExtraInfo если равно constNumberN, значит эта клавиша симулирована нами - пропускаем ее
      cmp  [ebx+MSLLHOOKSTRUCT.dwExtraInfo],constNumberN;ebx = [!lParam]
    cmove  eax,esp

     ;Если не равно нулю значит пропускаем
     test  eax,eax
      jnz  LowLevelKeyboardProc.Skip

      ;Подготовим даные
      ;esi = [!wParam];ebx = [!lParam]
      sub  esi,WM_LBUTTONDOWN
      mov  ebp,[ebx+MSLLHOOKSTRUCT.mouseData];ebp = [MSLLHOOKSTRUCT.mouseData];ebx = [!lParam]

      ;Проверим если сообщение больше последнего обрабатываемого или меньше первого обрабатываемого - пропускаем
      cmp  esi,WM_MOUSEHWHEEL - WM_LBUTTONDOWN
       ja  LowLevelKeyboardProc.Skip

   invoke  AcquireSRWLockShared,aRWKeyMap;Так как эти даные может изменять основной поток

      ;Пригаем на нужное сообщение  и получаем vkCode кнопки и узнаем кнопка отжимаеться или нажимаеться - если eax = 0 значит отжимаеться
      xor  eax,eax
      mov  ebx,aHook;ebx = aHook
      inc  eax
      jmp  dword[.JmpTable+esi*4];ebp = [MSLLHOOKSTRUCT.mouseData];eax = 1;ebx = aHook

   ;Левая кнопка
   ;ebp = [MSLLHOOKSTRUCT.mouseData];eax = 1;ebx = aHook
     .WM_LBUTTONUP:
                dec  eax

   .WM_LBUTTONDOWN:
                mov  esi,VK_LBUTTON
                jmp  .TestPressKey;esi = [vkCodeItemOne];eax = 0 значит отжимаеться;ebx = aHook

   ;Правая кнопка
   ;ebp = [MSLLHOOKSTRUCT.mouseData];eax = 1;ebx = aHook
     .WM_RBUTTONUP:
                dec  eax

   .WM_RBUTTONDOWN:
                mov  esi,VK_RBUTTON
                jmp  .TestPressKey;esi = [vkCodeItemOne];eax = 0 значит отжимаеться;ebx = aHook

   ;Средняя кнопка
   ;ebp = [MSLLHOOKSTRUCT.mouseData];eax = 1;ebx = aHook
     .WM_MBUTTONUP:
                dec  eax

   .WM_MBUTTONDOWN:
                mov  esi,VK_MBUTTON
                jmp  .TestPressKey;esi = [vkCodeItemOne];eax = 0 значит отжимаеться;ebx = aHook

    ;Колесо вперед или назад
    ;ebp = [MSLLHOOKSTRUCT.mouseData];eax = 1;ebx = aHook
    .WM_MOUSEWHEEL:
                shr  ebp,31;ebp = [MSLLHOOKSTRUCT.mouseData]
                lea  esi,[VK_MOUSEWHEEL_UP+ebp]
                jmp  .TestPressKey;esi = [vkCodeItemOne];eax = 0 значит отжимаеться;ebx = aHook

   ;X1 или X2 кнопка нажимаеться
   ;ebp = [MSLLHOOKSTRUCT.mouseData];eax = 1;ebx = aHook
     .WM_XBUTTONUP:
                dec  eax

   .WM_XBUTTONDOWN:
                mov  esi,VK_XBUTTON1
                mov  ecx,VK_XBUTTON2
                shr  ebp,17;ebp = [MSLLHOOKSTRUCT.mouseData]
             cmovnc  esi,ecx
                jmp  .TestPressKey;esi = [vkCodeItemOne];eax = 0 значит отжимаеться;ebx = aHook

   ;Колесо влево или вправо
   ;ebp = [MSLLHOOKSTRUCT.mouseData];eax = 1;ebx = aHook
   .WM_MOUSEHWHEEL:
                shr  ebp,31;ebp = [MSLLHOOKSTRUCT.mouseData]
                lea  esi,[VK_MOUSEWHEEL_RIGHT+ebp]


;Определим кнопка отжимаеться или нажимаеться
;esi = [vkCodeItemOne];eax = 0 значит отжимаеться;ebx = aHook
;---------------------------------
.TestPressKey:
      ;Подготовим даные
      mov  edi,eax;edi = 0 значит отжимаеться
      xor  edx,edx;edx = 0
      mov  ebp,aListViewItem;ebp = aListViewItem
      mov  [!lenghNameModule],edx;edx = 0;Если = 0 значит еще активное окно не находили и буфере строки нет

      ;Подсчитываем количество удерживаемых кнопок и устанавливаем те кнопки которорые нажаты
      cmp  esi,VK_XBUTTON2;esi = [vkCodeItemOne]
       ja  @f;Значит это колесо крутиться пропускаем - так как неучитываеться в общем количестве
      mov  cl,byte[ebx+HOOK.aKeyPressedMap+esi];ebx = aHook;esi = [vkCodeItemOne]
      sub  cl,al;eax = 0 значит отжимаеться
      neg  cl
      mov  byte[ebx+HOOK.aKeyPressedMap+esi],al;ebx = aHook;esi = [vkCodeItemOne];eax = 0 значит отжимаеться
      add  byte[ebx+HOOK.iCountKeyPress],cl;ebx = aHook
      @@:

    ;Проверка удерживаеться клавиша или нет; если 1 в таблице значит удерживаеться
     ;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = 0 значит отжимаеться;edx = 0;ebx = aHook
     test  edi,edi;edi = 0 значит отжимаеться
      jnz  .KeyPressed;ebp = aListViewItem;esi = [vkCodeItemOne];ebx = aHook;edx = 0


      ;Если edi = 0 значит клавиша не модификатор а у нас как раз и есть этот случай
      jmp  LowLevelKeyboardProc.UnPressedMouse;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif;edx = 0;ebx = aHook

   ;Кнопка нажата
   ;ebp = aListViewItem;esi = [vkCodeItemOne];ebx = aHook;edx = 0
   ;----------------------------------------------
  .KeyPressed:
           ;Подготовим даные
           mov  eax,[ebx+HOOK.iCountKeyPressDD];ebx = aHook;eax = [iCountKeyPressDD]
           lea  edi,[eax+1]
           cmp  esi,VK_XBUTTON2;esi = [vkCodeItemOne]
         cmova  eax,edi


          ;Одна клавиша нажата(или много)  - обычная(обычных)
           mov  ecx,LowLevelKeyboardProc.NotCombination
          test  ah,ah;ah = iCountKeyPressModif
         cmovz  edx,ecx;edx = 0

           ;Одна клавиша нажата обычная и один модификатор
           mov  ecx,LowLevelKeyboardProc.OneCombination
           cmp  eax,0x102;eax = [iCountKeyPressDD]
         cmove  edx,ecx;edx = 0

           ;Одна клавиша нажата обычная и два модификатора
           mov  ecx,LowLevelKeyboardProc.TwoCombination
           cmp  eax,0x203;eax = [iCountKeyPressDD]
         cmovz  edx,ecx;edx = 0

          test  edx,edx
            jz  LowLevelKeyboardProc.SkipUlock;Не одно с условий не выполнина - пропускаем
           jmp  LowLevelKeyboardProc.KeyPressedMouse;ebp = aListViewItem;esi = [vkCodeItemOne];edx = jmp


 .JmpTable ddA  .WM_LBUTTONDOWN;WM_LBUTTONDOWN
           ddA  .WM_LBUTTONUP;WM_LBUTTONUP
           ddA  LowLevelKeyboardProc.SkipUlock;WM_LBUTTONDBLCLK
           ddA  .WM_RBUTTONDOWN;WM_RBUTTONDOWN
           ddA  .WM_RBUTTONUP;WM_RBUTTONUP
           ddA  LowLevelKeyboardProc.SkipUlock;WM_RBUTTONDBLCLK
           ddA  .WM_MBUTTONDOWN;WM_MBUTTONDOWN
           ddA  .WM_MBUTTONUP;WM_MBUTTONUP
           ddA  LowLevelKeyboardProc.SkipUlock;WM_MBUTTONDBLCLK
           ddA  .WM_MOUSEWHEEL;WM_MOUSEWHEEL
           ddA  .WM_XBUTTONDOWN;WM_XBUTTONDOWN
           ddA  .WM_XBUTTONUP;WM_XBUTTONUP
           ddA  LowLevelKeyboardProc.SkipUlock;WM_XBUTTONDBLCLK
           ddA  .WM_MOUSEHWHEEL;WM_MOUSEHWHEEL

 endp


;----------------------------------------------
 proc LowLevelKeyboardProc [ebp esi edi ebx],!nCode,!wParam,!lParam
  locals;Локальные переменые и сохраненые регистры должны быть одикавовы у: LowLevelKeyboardProc и LowLevelMouseProc, что бы можно было использовать .CmpGroup в двух этих функциях
   !lenghNameModule        dd  ?;Если = 0 значит еще активное окно не находили и буфере строки нет
   !iProcessId             dd  ?
   ;sizeof.INIFILEDATA.szOpenExeBufer/2 - что бы было одинаково с по длине с файлом открываем для групп, +1 учет на то что GetWindowModuleFileNameW возвращает длину без нуля
   !aBufferString          du  sizeof.INIFILEDATA.szOpenExeBufer/2 + 1 dup(?)
  endl;Локальные переменые и сохраненые регистры должны быть одикавовы у: LowLevelKeyboardProc и LowLevelMouseProc, что бы можно было использовать .CmpGroup в двух этих функциях


                        ;Подготавливаем
                        ;vkCode- должен байт занимать и таблица у нас исходит из этого поэтому и используем movzx,что бы не было проблем
                        mov  eax,[!nCode];если не равно нулю должны пропустить
                        mov  ebx,[!lParam];ebx = [!lParam]
                      movzx  esi,byte[ebx+KBDLLHOOKSTRUCT.vkCode];ebx = [!lParam]

                        ;Провераем KBDLLHOOKSTRUCT.dwExtraInfo если равно constNumberN, значит эта клавиша симулирована нами - пропускаем ее
                        cmp  [ebx+KBDLLHOOKSTRUCT.dwExtraInfo],constNumberN;ebx = [!lParam]
                      cmove  eax,esp

                        ;Подготавливаем
                        mov  ebp,aHook;ebp = aHook

                      ;Если не равно нулю значит пропускаем
                       test  eax,eax
                        jnz  .Skip

                       ;Определяем vkCode  клавиша модификатор или нет
                       ;ebx = [!lParam];ebp = aHook;esi = byte[KBDLLHOOKSTRUCT.vkCode]
                        xor  edi,edi;edi = 0

                        cmp  esi,VK_SHIFT ;esi = [KBDLLHOOKSTRUCT.vkCode]
                      cmove  edi,esp
                        cmp  esi,VK_CONTROL;esi = [KBDLLHOOKSTRUCT.vkCode]
                      cmove  edi,esp
                        cmp  esi,VK_MENU;esi = [KBDLLHOOKSTRUCT.vkCode]
                      cmove  edi,esp
                        cmp  esi,VK_LSHIFT;esi = [KBDLLHOOKSTRUCT.vkCode]
                      cmove  edi,esp
                        cmp  esi,VK_RSHIFT;esi = [KBDLLHOOKSTRUCT.vkCode]
                      cmove  edi,esp
                        cmp  esi,VK_LCONTROL;esi = [KBDLLHOOKSTRUCT.vkCode]
                      cmove  edi,esp
                        cmp  esi,VK_RCONTROL;esi = [KBDLLHOOKSTRUCT.vkCode]
                      cmove  edi,esp
                        cmp  esi,VK_LMENU;esi =[KBDLLHOOKSTRUCT.vkCode]
                      cmove  edi,esp
                        cmp  esi,VK_RMENU;esi = [KBDLLHOOKSTRUCT.vkCode]
                      cmove  edi,esp

                     invoke  AcquireSRWLockShared,aRWKeyMap;Так как эти даные может изменять основной поток

                        ;Подготавливаем
                        mov  cl,byte[ebp+HOOK.aKeyPressedMap+esi];cl = prev flags;ebp = aHook;esi = byte[KBDLLHOOKSTRUCT.vkCode]
                        mov  eax,[ebp+HOOK.iCountKeyPressDD];ebp = aHook;al = [HOOK.iCountKeyPress];ah =[HOOK.iCountKeyPressModif]

                       ;Проверка удерживаеться клавиша или нет; если 1 в таблице значит удерживаеться
                       ;ebx = [!lParam];ebp = aHook;esi = byte[KBDLLHOOKSTRUCT.vkCode]];al = [HOOK.iCountKeyPress];ah =[HOOK.iCountKeyPressModif;cl = prev flags;edi = modif
                       test  [ebx+KBDLLHOOKSTRUCT.flags],0x80;ebx = [!lParam]
                       setz  ch;ch = current aHook.aKeyPressedMap+Key
                       setz  byte[ebp+HOOK.aKeyPressedMap+esi];ebp = aHook;esi = byte[KBDLLHOOKSTRUCT.vkCode]

;Контролируем количество клавиш удерживаемых на клавиатуре
;ebp = aHook;esi = byte[KBDLLHOOKSTRUCT.vkCode]];al = [HOOK.iCountKeyPress];ah =[HOOK.iCountKeyPressModif;cl = prev flags;ch = current aHook.aKeyPressedMap+Key;edi = modif
                        sub  cl,ch;cl = prev flags;ch = current aHook.aKeyPressedMap+Key
                        neg  cl;cl = add or sub (+1 or -1 or 0)
                      movzx  edx,cl;edx = add or sub (+1 or -1 or 0);cl = add or sub (+1 or -1 or 0)
                        add  [ebp+HOOK.iCountKeyPress],cl;ebp = aHook;cl = add or sub (+1 or -1 or 0)

                       ;Определяем текущее количество клавиш модификаторов нажатых
                       test  edi,edi;edi = modif
                      cmovz  edx,edi;edi = modif;edx = add or sub (+1 or -1 or 0)
                        add  [ebp+HOOK.iCountKeyPressModif],dl;ebp = aHook;edx = add or sub (+1 or -1 or 0)

                        ;Подготавливаем
                        xor  edx,edx;edx = 0
                        mov  ebp,aListViewItem;ebp = aListViewItem
                        mov  [!lenghNameModule],edx;edx = 0;Если = 0 значит еще активное окно не находили и буфере строки нет

                       ;Проверка удерживаеться клавиша или нет; если 1 в таблице значит удерживаеться
                       test  ch,ch;ch = current aHook.aKeyPressedMap+Key
                        jnz  .KeyPressed;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];al = [HOOK.iCountKeyPress];ah =[HOOK.iCountKeyPressModif;edi = modif;edx = 0

           ;Кнопка отжимаеться
           ;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif;edx = 0
           ;----------------------------------------------
           ;Подготовим
           mov  ebx,aHook;ebx = aHook
.UnPressedMouse:;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif;edx = 0;ebx = aHook
           mov  eax,[ebx+HOOK.aCombi.vkCodeItemOne];eax = [HOOK.vkCodeItemOne];ebx = aHook
           mov  ecx,[ebx+HOOK.aCombi.vkCodeItemTwoThree];ecx = [HOOK.vkCodeItemTwoThree];ebx = aHook

          ;Проверим если не равно нулю значит у нас комбинация - если есть комбинация ничего другого быть не может,но может нажаты быть и другие кнопки потом и теперь освобождаться
          test  eax,eax;eax = [HOOK.vkCodeItemOne]
            jz  .NotCombinationReleased;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif;ebx = aHook;edx = 0

         ;Проверим может освобождаеться кнопка не входящая в комбинацию, а заодно проверяем есть ли совпадающие модификаторы с отжимающейся клавише в случае совпадения обнуляем
         ;ebp = aListViewItem;ebx = aHook;esi = byte[KBDLLHOOKSTRUCT.vkCode];eax = [HOOK.vkCodeItemOne];ecx = [HOOK.vkCodeItemTwoThree];edx = 0
         movzx  edi,cx;ecx = [HOOK.vkCodeItemTwoThree]
           shr  ecx,16;ecx = [HOOK.vkCodeItemTwoThree]
           cmp  edi,esi;esi = byte[KBDLLHOOKSTRUCT.vkCode];ecx = [HOOK.vkCodeItemTwoThree]
         cmove  ebp,edx;edx = 0
           cmp  eax,esi;esi = byte[KBDLLHOOKSTRUCT.vkCode];eax = [HOOK.vkCodeItemOne]
         cmove  ebp,edx;edx = 0
           cmp  ecx,esi;esi = byte[KBDLLHOOKSTRUCT.vkCode];ecx = [HOOK.vkCodeItemTwoThree]
         cmove  ebp,edx;edx = 0
          ;Если не  равно нулю значит не одного совпадения не было - пропускаем
          test  ebp,ebp
           jnz  .SkipUlock

          ;Получим vkCode отвечающее за действие каторое надо выполнить
          ;ebx = aHook;esi = byte[KBDLLHOOKSTRUCT.vkCode];eax = [HOOK.vkCodeItemOne];edx = 0;ebp = 0
           mov  dl,byte[ebx+HOOK.aKeyPressedMapColizion+eax];ebx = aHook;eax = [HOOK.vkCodeItemOne];edx = 0

           ;Обнулим так как все комбинация станет недействительной
           mov  [ebx+HOOK.aCombi.vkCodeItemOne],ebp;ebx = aHook;ebp = 0

           ;Проверим может действий при отпусканием клавиши производить и не надо
           cmp  edx,0xFF;edx = action
            je  .CombionsTestKeys;esi = byte[KBDLLHOOKSTRUCT.vkCode];eax = [HOOK.vkCodeItemOne]

          ;Сохраняем тот факт равна ли пришедшая обычная клавиша(если пришла обычная) с сохраненай и обнулим, если пришла обычная иначе установим 0xFF
           ;ebx = aHook;esi = byte[KBDLLHOOKSTRUCT.vkCode];eax = [HOOK.vkCodeItemOne];edx = action = [ITEMLISTVIEW.vkCodeSubOne]
          xchg  esi,eax;esi = byte[KBDLLHOOKSTRUCT.vkCode];eax = [HOOK.vkCodeItemOne]
           mov  ebp,0xFF
           xor  eax,esi
        cmovnz  eax,ebp
           mov  ebp,[ebx+HOOK.aCombi.scanCodeSubOne];ebx = aHook;ebp = [ITEMLISTVIEW.scanCodeSubOne]
           mov  byte[ebx+HOOK.aKeyPressedMapColizion+esi],al;ebx = aHook;esi = [HOOK.vkCodeItemOne]
         cmovz  ebx,eax;ebx = bool

        ;Отправим сообщение в тикущий паток
        ;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = [ITEMLISTVIEW.scanCodeSubOne]
        invoke  PostThreadMessageW,[iThreadIdHook],WM_APP_HOOK_COMB_ONE_KEY_RELEASED,edx,ebp;ebp = [ITEMLISTVIEW.scanCodeSubOne];edx = [ITEMLISTVIEW.vkCodeSubOne]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

           ;Если равно нулю тогда блокируем  клавишу
          test  ebx,ebx;ebx = bool
            jz  .InterlockKeyUnRelease;Клавишу блокируем но захват aRWKeyMap на чтение не освобождаем так как не знаем когда получим доступ и поэтому что бы основной поток не изменил данные

           jmp  .Skip;Клавишу не блокируем но захват aRWKeyMap на чтение не освобождаем так как не знаем когда получим доступ и поэтому что бы основной поток не изменил данные


;Проверим если обычная клавиша отпускаеться, тогда хоть действий и не надо, но блокируем ее
;esi = byte[KBDLLHOOKSTRUCT.vkCode];eax = [HOOK.vkCodeItemOne]
.CombionsTestKeys:
           cmp  eax,esi
            je  .InterlockKey;Блокируем клавишу и освобождаем захват aRWKeyMap

           jmp  .SkipUlock


        ;Освобождаемаеться одна клавиша а не комбинация
        ;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif;ebx = aHook;edx = 0
        .NotCombinationReleased:
                             ;Проверяем клавишу надо освобождать или нет
                              or  dl,byte[ebx+HOOK.aKeyPressedMapColizion+esi];esi = byte[KBDLLHOOKSTRUCT.vkCode];ebx = aHook;edx = 0
                              jz  .SkipUlock;Не надо освобождать

                             ;Очишаем так как  - освободили
                             mov  byte[ebx+HOOK.aKeyPressedMapColizion+esi],0;esi = byte[KBDLLHOOKSTRUCT.vkCode];ebx = aHook

                             ;Если клавиша модификатор отпускаеться  - востановим количество сохраненых модификаторов каторое должно быть
                            test  edi,edi;edi = modif
                              jz  @f
                             inc  byte[ebx+HOOK.iCountKeyPressModif];ebx = aHook
                             @@:

                             ;0xFF - значит просто клавишу при отпускании заблокируем - действий не требуеться
                             cmp  edx,0xFF
                              je  .InterlockKey;Блокируем клавишу и освобождаем захват aRWKeyMap

                             ;Подготавливаем
                             mov  ecx,ebp;ecx = aListViewItem

                             ;Найдем сканкод освабаждаемой клавиши
                             ;ebp = aListViewItem;edx = [ITEMLISTVIEW.vkCodeSubOne];ecx = ITEMLISTVIEW
                             @@:
                             mov  ebp,[ebp+ITEMLISTVIEW.lpNextItem];ebp = ITEMLISTVIEW
                             cmp  ebp,ecx;ecx = aListViewItem
                              je   .SkipUlockBag;Походу баг...???

                             cmp  [ebp+ITEMLISTVIEW.vkCodeSubOne],edx;ebp = ITEMLISTVIEW;edx = [ITEMLISTVIEW.vkCodeSubOne]
                             jne  @b

                          ;Отправим сообщение в тикущий паток
                          ;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW
                          invoke  PostThreadMessageW,[iThreadIdHook],WM_APP_HOOK_ONE_KEY_RELEASED,edx,ebp;ebp = ITEMLISTVIEW;edx = [ITEMLISTVIEW.vkCodeSubOne]
                                  _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                             ;Клавишу блокируем но захват aRWKeyMap на чтение не освобождаем так как не знаем когда получим доступ и поэтому что бы основной поток не изменил данные
                             jmp  .InterlockKeyUnRelease


   ;Кнопка нажата
   ;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];al = [HOOK.iCountKeyPress];ah =[HOOK.iCountKeyPressModif;edi = modif;edx = 0
   ;----------------------------------------------
  .KeyPressed:
           mov  ebx,.NotCombinationModif
           mov  ecx,.NotCombination

          ;Одна клавиша нажата - модификатор
          test  edi,edi;edi = modif
        cmovnz  ecx,ebx

          ;Одна клавиша нажата(или много)  - обычная(обычных)
          test  ah,ah;ah =[HOOK.iCountKeyPressModif
         cmovz  edx,ecx

            or  edi,[aHook.iCountKeyPressDD]
           mov  ecx,.TwoCombination

           ;Одна клавиша нажата обычная и два модификатора
           cmp  edi,0x203
         cmove  edx,ecx

           mov  ecx,.OneCombination

           ;Одна клавиша нажата обычная и один модификатор
           cmp  edi,0x102
         cmove  edx,ecx

          test  edx,edx
            jz  .SkipUlock;Не одно с условий не выполнина - пропускаем

.KeyPressedMouse:
          xchg  edi,edx;edi = jmp


;Переберем все итемы что бы определить есть ли совпадения с нажатай клавишай
;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp
.LoopSearchKey:
           mov  ebp,[ebp+ITEMLISTVIEW.lpNextItem];ebp = ITEMLISTVIEW
           xor  eax,eax;eax = 0
           cmp  ebp,aListViewItem
            je  .SkipUlock

           ;(IMAGE_LIST_STATE_CHECK_Id+1) shl 12; + 1 - так как тут идет индекс  не от нуля а от 1 в отличи от List Image
           cmp  [ebp+ITEMLISTVIEW.stateItem],(IMAGE_LIST_STATE_CHECK_Id+1) shl 12;ebp = ITEMLISTVIEW
        cmovne  eax,esp
           cmp  [ebp+ITEMLISTVIEW.vkCodeItemOne],esi;esi = [KBDLLHOOKSTRUCT.vkCode];ebp = ITEMLISTVIEW
        cmovne  eax,esp
          test  eax,eax
           jnz  .LoopSearchKey

           ;Сделаем прижок
           jmp  edi;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp;eax = 0


              ;На лучай бага, тут мы нидолжны появляться ну ни как
             .SkipUlockBag:;???
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                        ud2

                     ;К клавиши нет привязки - пропускаем
                .SkipUlock:
                     invoke  ReleaseSRWLockShared,aRWKeyMap;Так как эти даные может изменять основной поток

                     ;если меньше нуля должны пропустить
                     .Skip:
                     invoke  CallNextHookEx,0,[!nCode],[!wParam],[!lParam]
                        ret



;Пользователем была использована комбинация два модификатара и клавиша проверяем есть ли такая комбинация в текущем итеме
;в aKeyPressedMap храняться текущия нажатия всех клавиш, если например в vkCodeItemTwoThree  одной клавиш не будет - т.е. там будет ноль
;то засчет то в таблице aKeyPressedMap ноль всегда не нажат так что бутет все работать, vk кода с  нулем нету, эти кодыы начинаються с 1
;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp;eax = 0
;----------------------------------------------
.TwoCombination:
           mov  ebx,aHook;ebx = aHook
         movzx  ecx,byte[ebp+ITEMLISTVIEW.vkCodeItemTwoThree];ebp = ITEMLISTVIEW
         movzx  edx,byte[ebp+ITEMLISTVIEW.vkCodeItemTwoThree+2];ebp = ITEMLISTVIEW
           add  al,byte[ebx+HOOK.aKeyPressedMap+ecx];ebx = aHook;eax = 0
           add  al,byte[ebx+HOOK.aKeyPressedMap+edx];ebx = aHook;eax = 0
           cmp  al,2
           jne  .LoopSearchKey;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp

          ;Проверяем совпадает ли путь к активному процессу и группа или это глобальная группа
          ;Нужные данные: ;ebp = ITEMLISTVIEW
          ;Изменяемые регистры: eax,ecx,edx,ebx
          ;На выходе: Активный процесс и группа одинаковы либо итем с глобальной группы, если eax не равно нулю
          call  .CmpGroup
          test  eax,eax
            jz  .LoopSearchKey;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp

           jmp  .SaveCombination;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode]


;Пользователем была использована комбинация один модификатар и клавиша проверяем есть ли такая комбинация в текущем итеме
;в aKeyPressedMap храняться текущия нажатия всех клавиш, если например в vkCodeItemTwoThree  одной клавиш не будет - т.е. там будет ноль
;то засчет то в таблице aKeyPressedMap ноль всегда не нажат так что бутет все работать, vk кода с  нулем нету, эти кодыы начинаються с 1
;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp;eax = 0
;----------------------------------------------
.OneCombination:
         movzx  ecx,byte[ebp+ITEMLISTVIEW.vkCodeItemTwoThree];ebp = ITEMLISTVIEW
           mov  ah,byte[ebp+ITEMLISTVIEW.vkCodeItemTwoThree+2];ebp = ITEMLISTVIEW;eax = 0
           mov  al,byte[aHook.aKeyPressedMap+ecx];eax = 0
           dec  eax
           jnz  .LoopSearchKey;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif

          ;Проверяем совпадает ли путь к активному процессу и группа или это глобальная группа
          ;Нужные данные: ;ebp = ITEMLISTVIEW
          ;Изменяемые регистры: eax,ecx,edx,ebx
          ;На выходе: Активный процесс и группа одинаковы либо итем с глобальной группы, если eax не равно нулю
          call  .CmpGroup
          test  eax,eax
            jz  .LoopSearchKey;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp

           ;Сохраняем для последующего использования,когда отжимаеться клавиша связаная с комбинацией
           ;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode]
.SaveCombination:
           mov  ebx,aHook;ebx = aHook
           mov  eax,[ebp+ITEMLISTVIEW.vkCodeItemTwoThree];ebp = ITEMLISTVIEW
           mov  ecx,[ebp+ITEMLISTVIEW.scanCodeSubOne];ebp = ITEMLISTVIEW
           mov  edx,[ebp+ITEMLISTVIEW.scanCodeItemTwoThree];ebp = ITEMLISTVIEW
           mov  [ebx+HOOK.aCombi.vkCodeItemOne],esi;ebx = aHook;esi = byte[KBDLLHOOKSTRUCT.vkCode]
           mov  [ebx+HOOK.aCombi.vkCodeItemTwoThree],eax;ebx = aHook
           mov  [ebx+HOOK.aCombi.scanCodeSubOne],ecx;ebx = aHook
           mov  [ebx+HOOK.aCombi.scanCodeItemTwoThree],edx;ebx = aHook

           jmp  .NextNotCombination;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode]

;Раз попали сюда значит одна обычная клавиша(или клавиша модификатор заменяемая на обычную) или более нажата и  последняя модификированая совпала
;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp;eax = 0
;----------------------------------------------
.NotCombinationModif:
           ;Значит в структуре храниться комбинация - ищим дальше совпадения
           cmp  [ebp+ITEMLISTVIEW.vkCodeItemTwoThree],eax;ebp = ITEMLISTVIEW
           jne  .LoopSearchKey;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp

          ;Проверяем совпадает ли путь к активному процессу и группа или это глобальная группа
          ;Нужные данные: ;ebp = ITEMLISTVIEW
          ;Изменяемые регистры: eax,ecx,edx,ebx
          ;На выходе: Активный процесс и группа одинаковы либо итем с глобальной группы, если eax не равно нулю
          call  .CmpGroup
          test  eax,eax
            jz  .LoopSearchKey;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp

           ;Подготавливаем
           mov  edx,aHook;edx = aHook

           ;Если клавиша была уже нажата то отнимать не надо так как уже это учли
           cmp  byte[edx+HOOK.aKeyPressedMapColizion+esi],0;edx = aHook
           jne  .NextNotCombination

           ;Отнимим что бы не считалось что клавиша модификатор нажата
           dec  byte[edx+HOOK.iCountKeyPressModif];edx = aHook

           jmp  .NextNotCombination;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode]


;Раз попали сюда значит одна обычная клавиша(или клавиша модификатор заменяемая на обычную) или более нажата и  последняя обычная клавиша совпала
;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp;eax = 0
;----------------------------------------------
.NotCombination:
           ;Значит в структуре храниться комбинация - ищим дальше совпадения
           cmp  [ebp+ITEMLISTVIEW.vkCodeItemTwoThree],eax;ebp = ITEMLISTVIEW
           jne  .LoopSearchKey;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp

          ;Проверяем совпадает ли путь к активному процессу и группа или это глобальная группа
          ;Нужные данные: ;ebp = ITEMLISTVIEW
          ;Изменяемые регистры: eax,ecx,edx,ebx
          ;На выходе: Активный процесс и группа одинаковы либо итем с глобальной группы, если eax не равно нулю
          call  .CmpGroup
          test  eax,eax
            jz  .LoopSearchKey;ebp = aListViewItem;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = jmp

           ;Подготавливаем
           ;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode]
.NextNotCombination:
           mov  ebx,[ebp+ITEMLISTVIEW.vkCodeSubOne];ebp = ITEMLISTVIEW
           xor  edi,edi;edi = DOWN(0)
          xchg  esi,ebx;esi = [ITEMLISTVIEW.vkCodeSubOne];ebx = byte[KBDLLHOOKSTRUCT.vkCode]

;Выполним нужные действия
;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];ebx = byte[KBDLLHOOKSTRUCT.vkCode];edi = DOWN(0);ecx = aHook
;----------------------------------------------
           ;Если ноль то только одну клавишу или мышь выполнить надо
           cmp  [ebp+ITEMLISTVIEW.vkCodeSubTwoThree],edi;ebp = aListViewItem;edi = DOWN(0)
            je  .ActionOne;ebp = aListViewItem;esi = [ITEMLISTVIEW.vkCodeSubOne];ebx = byte[KBDLLHOOKSTRUCT.vkCode];edi = DOWN(0);ecx = aHook

           ;В случаи комбинации и действия реагировать при отпускании клавиши кроме как блокировать не надо - помечаем это
           ;0xFF - значит просто клавишу при отпускании заблокируем - действий не требуеться
           mov  [aHook.aKeyPressedMapColizion+ebx],0xFF;ebx = byte[KBDLLHOOKSTRUCT.vkCode];ecx = aHook

           cmp  esi,VK_LAST
            ja  .Action;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = DOWN(0)

           ;Нужно симулировать комбинацию, но выполним уже после выхода с LowLevelKeyboardProc что бы не было глюков с мышьей если выполнить в LowLevelKeyboardProc сразу DOWN и UP для мыши
           ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
           ;----------------------------------------------
           ;Отправим сообщение в тикущий паток
           ;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW
        invoke  PostThreadMessageW,[iThreadIdHook],WM_APP_HOOK_COMBINATION_KEY,esi,ebp;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

           ;Клавишу блокируем но захват aRWKeyMap на чтение не освобождаем так как не знаем когда получим доступ и поэтому что бы основной поток не изменил данные
           jmp  .InterlockKeyUnRelease


                   ;Симулировать действие будет основной поток
                   ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = DOWN(0)
                   ;----------------------------------------------
                   .Action:
                        ;От этого действия требуеться только блокировать клавишу
                        cmp  esi,VK_LOCK_KEY
                         je  .InterlockKey

                        ;Запустим хранитель экрана по умолчанию
                        cmp  esi,VK_SCRSAV
                         je  .VK_SCRSAV;edi = DOWN(0)

                        ;Отправим главному окну сообщение о том чтобы с помощью WM_SYSCOMMAND выключил экран
                        cmp  esi,VK_OFFUNIV
                         je  .VK_OFFUNIV;edi = DOWN(0)

                        ;Получим размер нужный выделить для новой структуры ITEMLISTVIEW и выделим память под нее
                        mov  ebx,[ebp+ITEMLISTVIEW.scanCodeSubTwoThree]
                     invoke  HeapAlloc,[hheap],edi,ebx;edi = DOWN(0);ebx = full Action Date
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                        mov  edi,eax;edi = lp Action Date

                        ;Перенесем данные в новое место
                        add  ebp,sizeof.ITEMLISTVIEW;Каректируем до доп данных
                    stdcall  MoveMemoryHL,edi,ebp,ebx;edi = lp Action Date;ebp = ITEMLISTVIEW;ebx = full Action Date

                     ;Отправим сообщение окну основного потока для обработки действия
                     ;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = lp Action Date - освободить потом надо память
                     invoke  PostMessageW,[hwnd],WM_APP_ACTION,esi,edi;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = lp Action Date
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                        ;Блокируем клавишу и освобождаем захват aRWKeyMap
                        jmp  .InterlockKey

                 ;Запустим хранитель экрана по умолчанию
                 ;edi = DOWN(0)
                .VK_SCRSAV:
                     invoke  PostMessageW,[hwnd],WM_SYSCOMMAND,SC_SCREENSAVE,edi;edi = DOWN(0)
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                        ;Блокируем клавишу и освобождаем захват aRWKeyMap
                        jmp  .InterlockKey

                ;Отправим главному окну сообщение о том чтобы с помощью WM_SYSCOMMAND выключил экран
                ;edi = DOWN(0)
               .VK_OFFUNIV:
                     ;Отправим сообщение окну основного потока для обработки действия
                     ;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam - не используеться
                     invoke  PostMessageW,[hwnd],WM_APP_ACTION,esi,edi;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = DOWN(0)
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                        ;Блокируем клавишу и освобождаем захват aRWKeyMap
                        jmp  .InterlockKey


                ;Нужно симулировать одну клавишу или мышь но  выполним уже после выхода с LowLevelKeyboardProc что бы не было глюков с мышьей если выполнить в LowLevelKeyboardProc
                ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
                ;----------------------------------------------
                .ActionOne:
                     ;Отправим сообщение в тикущий паток
                     ;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW
                     invoke  PostThreadMessageW,[iThreadIdHook],WM_APP_HOOK_ONE_KEY,esi,ebp;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                        ;Клавишу блокируем но захват aRWKeyMap на чтение не освобождаем так как не знаем когда получим доступ и поэтому что бы основной поток не изменил данные
                        jmp  .InterlockKeyUnRelease

              ;Блокируем клавишу и освобождаем захват aRWKeyMap
             .InterlockKey:
                     invoke  ReleaseSRWLockShared,aRWKeyMap;Так как эти даные может изменять основной поток

    ;Клавишу блокируем но захват aRWKeyMap на чтение не освобождаем так как не знаем когда получим доступ и поэтому что бы основной поток не изменил данные
    .InterlockKeyUnRelease:
                        mov  eax,1
                        ret


             ;Проверяем совпадает ли путь к активному процессу и группа или это глобальная группа
             ;Нужные данные: ;ebp = ITEMLISTVIEW
             ;Изменяемые регистры: eax,ecx,edx,ebx
             ;На выходе: Активный процесс и группа одинаковы либо итем с глобальной группы, если eax не равно нулю
                  procM  CmpGroup
                        ;Если итем в общей группе значит проверять активное акно нету надобности, так как клавиша действует глобально
                        ;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif
                        xor  eax,eax;eax = 0
                        cmp  [ebp+ITEMLISTVIEW.idGroup],eax;LIST_VIEW_GROUP_GLOBAL_Id = 0;ebp = ITEMLISTVIEW;eax = 0
                         je  .GlobalGroup

                         ;Проверяем может уже находили путь к активному процессу в текущем цикле проверки
                         or  eax,[!lenghNameModule];Если = 0 значит еще активное окно не находили и в буфере строки нет
                        jnz  .CmpSring;eax = [!lenghNameModule]

                        ;Так как группа не глобальная, значит нужно проверить соответствует ли активный процесс группе
                        ;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif

                        ;Получаем активное акно
                     invoke  GetForegroundWindow
                       test  eax,eax;Если ноль - происходит смена активного окна
                         jz  .GroupNotEqual;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif

                        ;Получаем айди процесса к каторому принадлежит активное акно
                        lea  ecx,[!iProcessId]
                     invoke  GetWindowThreadProcessId,eax,ecx

                        ;Получаем хендл активного процесса
                     invoke  OpenProcess,PROCESS_QUERY_LIMITED_INFORMATION,0,[!iProcessId]
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                       test  eax,eax
                         jz  .GroupNotEqual;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif


                        ;Получаем путь файлу содержащий активный процесс
                        lea  ecx,[!lenghNameModule]
                        lea  edx,[!aBufferString]
                        mov  ebx,eax;ebx = hProcess
                        mov  dword[ecx],sizeof.!aBufferString/2

                     invoke  QueryFullProcessImageNameW,eax,0,edx,ecx
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                       test  eax,eax
                         jz  .ErrorGroupCloseHandle;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif;eax = 0;ebx = hProcess

                        ;Закрываем хендл процесса за не надобностью
                        inc  [!lenghNameModule]
                     invoke  CloseHandle,ebx;ebx = hProcess
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                        mov  eax,[!lenghNameModule]

                 ;eax = lengh current module
                 .CmpSring:
                        mov  edx,aListViewGroup;edx = aListViewGroup
                        mov  ecx,[ebp+ITEMLISTVIEW.idGroup];ecx = current idGroup


                        ;Найдем группу нашу в списке групп
                        ;eax = lengh current module;ecx = current idGroup;edx = aListViewGroup
                        @@:
                        mov  edx,[edx+GROUPLISTVIEW.lpNextGroup];edx = GROUPLISTVIEW
                        cmp  [edx+GROUPLISTVIEW.idGroup],ecx;ecx = current idGroup;edx = aListViewGroup
                        jne  @b

                        ;Подготавливаем
                        mov  ecx,[edx+GROUPLISTVIEW.lengthNameGroup];ecx = lengthNameGroup
                        mov  edx,[edx+GROUPLISTVIEW.lpszNameGroup];edx = lpszNameGroup

                        ;Сравним длину строк
                        ;eax = lengh current module;ecx = lengthNameGroup;edx = lpszNameGroup
                        sub  eax,ecx;eax = lengh current module;ecx = lengthNameGroup
                        jnz  .GroupNotEqual;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif

                        lea  ecx,[!aBufferString];ecx = string current module

                        ;Сравним строки
                        ;eax = 0;ecx = string current module;edx = lpszNameGroup
                        @@:
                        mov  ax,word[edx];edx = lpszNameGroup
                        add  edx,2
                        add  ecx,2
                        cmp  ax,word[ecx-2];ecx = string current module
                        jne  .GroupNotEqual;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif
                       test  eax,eax
                        jnz  @b

                       ;Строки одинаковы или глобальная группа
              .GlobalGroup:
                        mov  eax,[iPlague];Если есть взлом или попытка взлома программа перестанет работать
                       retd  ;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif

                 ;Произошла ошибка, закроем хендл процесса и обнулим [!lenghNameModule], что бы считалось, что буфер строки пустой
                 ;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif;eax = 0;ebx = hProcess
    .ErrorGroupCloseHandle:
                        mov  [!lenghNameModule],eax;eax = 0;Обнулим опять - раз неудачно все получилось
                     invoke  CloseHandle,ebx;ebx = hProcess
                             _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

            .GroupNotEqual:
                        xor  eax,eax
                       retd  ;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode];edi = modif
                  endpM

 endp



;Используеться когда в итеме две клавиша модификатор одна или ноль, добавляет в структуру INPUT событие UP для нее, что бы программы не считали что эти клавиши до сих пор нажаты
;Нужные данные: ebp = ITEMLISTVIEW;edi = KEYEVENTF_KEYUP или DOWN
;Изменяемые регистры: eax, ecx, edx
;----------------------------------------------
proc  MakeModifeter
       xor  eax,eax
       mov  edx,[lpaInputCurrent]

        or  ax,word[ebp+ITEMLISTVIEW.vkCodeItemTwoThree]
        jz  @f
     movzx  ecx,word[ebp+ITEMLISTVIEW.scanCodeItemTwoThree]

       mov  [edx+INPUT.type],INPUT_KEYBOARD;edx = lp INPUT*6
       mov  [edx+INPUT.ki.wVk],ax;edx = lp INPUT*6

       btr  ecx,15;ebx = [ITEMLISTVIEW.scanCodeSubOne]
      setc  al

       mov  [edx+INPUT.ki.time],0;edx = lp INPUT*6;ebx = 0
       mov  [edx+INPUT.ki.dwExtraInfo],constNumberN;edx = lp INPUT*6;ebx = 0

        or  eax,edi
       mov  [edx+INPUT.ki.wScan],cx;edx = lp INPUT*6
       mov  [edx+INPUT.ki.dwFlags],eax;edx = lp INPUT*6

       xor  eax,eax
       add  edx,sizeof.INPUT
       inc  [iInputCount];Отвечает за количество структур INPUT отправляемых SendInput
       mov  [lpaInputCurrent],edx

       @@:
        or  ax,word[ebp+ITEMLISTVIEW.vkCodeItemTwoThree+2]
        jz  @f
     movzx  ecx,word[ebp+ITEMLISTVIEW.scanCodeItemTwoThree+2]

       mov  [edx+INPUT.type],INPUT_KEYBOARD;edx = lp INPUT*6
       mov  [edx+INPUT.ki.wVk],ax;edx = lp INPUT*6

       btr  ecx,15;ebx = [ITEMLISTVIEW.scanCodeSubOne]
      setc  al

       mov  [edx+INPUT.ki.time],0;edx = lp INPUT*6;ebx = 0
       mov  [edx+INPUT.ki.dwExtraInfo],constNumberN;edx = lp INPUT*6;ebx = 0

        or  eax,edi
       mov  [edx+INPUT.ki.wScan],cx;edx = lp INPUT*6
       mov  [edx+INPUT.ki.dwFlags],eax;edx = lp INPUT*6

       inc  [iInputCount];Отвечает за количество структур INPUT отправляемых SendInput
       add  [lpaInputCurrent],sizeof.INPUT
       @@:
      retd
endp


             ;Записывает в INPUT информацио о нажатой клавише или кнопке мыше
             ;Нужные данные: ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = KEYEVENTF_KEYUP или DOWN
             ;Изменяемые регистры: eax,ecx,edx,edi
             ;----------------------------------------------
         proc  MakeInputKeyMouse
                        xor  ecx,ecx
                        mov  edx,[lpaInputCurrent]
                       xchg  ecx,edi;edi = 0;ecx = KEYEVENTF_KEYUP или DOWN
                        inc  [iInputCount];Отвечает за количество структур INPUT отправляемых SendInput
                        add  [lpaInputCurrent],sizeof.INPUT

                        ;Определим симулируем мышь или клавиатуру
                         or  edi,[ebp+ITEMLISTVIEW.scanCodeSubOne];ebp = ITEMLISTVIEW;edi = 0
                        jnz  .ActionOneKey;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edx = lp INPUT*6;ecx = KEYEVENTF_KEYUP или DOWN;edi = [ITEMLISTVIEW.scanCodeSubOne]

             ;Симулируем мышь
             ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edx = lp INPUT*6;ecx = KEYEVENTF_KEYUP или DOWN;edi = 0
           .ActionOneMouse:
                        mov  [edx+INPUT.type],edi;edx = lp INPUT*6;edi = 0;INPUT_MOUSE = 0
                        mov  [edx+INPUT.mi.dx],edi;edx = lp INPUT*6;edi = 0
                        mov  [edx+INPUT.mi.dy],edi;edx = lp INPUT*6;edi = 0
                        mov  [edx+INPUT.mi.time],edi;edx = lp INPUT*6;edi = 0
                        mov  [edx+INPUT.mi.dwExtraInfo],constNumberN;edx = lp INPUT*6;edi = 0

                        ;Основано это на том что разница у мыши в сообщениях между DOWN и UP в один сдвиг, тоесть UP больше на один сдвиг
                        shr  ecx,1;ecx = modi UP_DOWN

                        ;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne] ;ecx = modi UP_DOWN
                        cmp  esi,VK_MOUSEWHEEL_UP
                         je  .VK_MOUSEWHEEL_UP;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne] ;ecx = modi UP_DOWN
                        cmp  esi,VK_MOUSEWHEEL_DOWN
                         je  .VK_MOUSEWHEEL_DOWN;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne] ;ecx = modi UP_DOWN
                        cmp  esi,VK_MOUSEWHEEL_RIGHT
                         je  .VK_MOUSEWHEEL_RIGHT;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne] ;ecx = modi UP_DOWN
                        cmp  esi,VK_MOUSEWHEEL_LEFT
                         je  .VK_MOUSEWHEEL_LEFT;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne] ;ecx = modi UP_DOWN
                        cmp  esi,VK_XBUTTON1
                        jae  .VK_XBUTTON;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne] ;ecx = modi UP_DOWN

                        ;Определяем левую, правую и центральную(щелчок колесо) кнопки мыши
                        ;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne];ecx = modi UP_DOWN
                        xor  edi,edi
                        mov  eax,MOUSEEVENTF_LEFTDOWN
                        cmp  esi,VK_LBUTTON
                      cmove  edi,eax
                        mov  eax,MOUSEEVENTF_RIGHTDOWN
                        cmp  esi,VK_RBUTTON
                      cmove  edi,eax
                        mov  eax,MOUSEEVENTF_MIDDLEDOWN
                        cmp  esi,VK_MBUTTON
                      cmove  edi,eax

                        shl  edi,cl;ecx = modi UP_DOWN
                        mov  [edx+INPUT.mi.mouseData],0;edx = lp INPUT*6
                        mov  [edx+INPUT.mi.dwFlags],edi;edx = lp INPUT*6
                       retd  ;Изменяемые регистры: eax,ecx,edx,edi


               ;Определяем кнопки X1 и  X2
               ;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne] ;ecx = modi UP_DOWN
               .VK_XBUTTON:
                        lea  eax,[esi-VK_XBUTTON1];esi = [ITEMLISTVIEW.vkCodeSubOne]
                        mov  edi,MOUSEEVENTF_XDOWN
                        inc  eax;XBUTTON1 = 1; XBUTTON2 = 2
                        shl  edi,cl;ecx = modi UP_DOWN
                        mov  [edx+INPUT.mi.mouseData],eax;edx = lp INPUT*6
                        mov  [edx+INPUT.mi.dwFlags],edi;edx = lp INPUT*6
                       retd  ;Изменяемые регистры: eax,ecx,edx,edi

          ;Колесо в перед - от пользователя
          ;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne]
         .VK_MOUSEWHEEL_UP:
                        mov  [edx+INPUT.mi.mouseData],WHEEL_DELTA;edx = lp INPUT*6
                        mov  [edx+INPUT.mi.dwFlags],MOUSEEVENTF_WHEEL;edx = lp INPUT*6
                       retd  ;Изменяемые регистры: eax,ecx,edx,edi

          ;Колесо в назад - к пользователю
          ;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne]
       .VK_MOUSEWHEEL_DOWN:
                        mov  [edx+INPUT.mi.mouseData],-WHEEL_DELTA;edx = lp INPUT*6
                        mov  [edx+INPUT.mi.dwFlags],MOUSEEVENTF_WHEEL;edx = lp INPUT*6
                       retd  ;Изменяемые регистры: eax,ecx,edx,edi

          ;Колесо вправо - от пользователя
          ;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne]
      .VK_MOUSEWHEEL_RIGHT:
                        mov  [edx+INPUT.mi.mouseData],WHEEL_DELTA;edx = lp INPUT*6
                        mov  [edx+INPUT.mi.dwFlags],MOUSEEVENTF_HWHEEL;edx = lp INPUT*6
                       retd  ;Изменяемые регистры: eax,ecx,edx,edi

          ;Колесо влево - от пользователя
          ;ebp = ITEMLISTVIEW;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne]
       .VK_MOUSEWHEEL_LEFT:
                        mov  [edx+INPUT.mi.mouseData],-WHEEL_DELTA;edx = lp INPUT*6
                        mov  [edx+INPUT.mi.dwFlags],MOUSEEVENTF_HWHEEL;edx = lp INPUT*6
                       retd  ;Изменяемые регистры: eax,ecx,edx,edi

             ;Симулируем клавиатуру
             ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edx = lp INPUT*6;ecx = KEYEVENTF_KEYUP или DOWN;edi = [ITEMLISTVIEW.scanCodeSubOne]
             .ActionOneKey:
                        ;В скан коде в самом старшом бите содержиться KEYEVENTF_EXTENDEDKEY, т.е. если он есть мы убираем его с сканкода и устанавливаем в dwFlags
                        xor  eax,eax
                        btr  edi,15;edi = [ITEMLISTVIEW.scanCodeSubOne]
                       setc  al
                         or  eax,ecx;ecx = KEYEVENTF_KEYUP или DOWN;eax = dwFlags

                        mov  [edx+INPUT.type],INPUT_KEYBOARD;edx = lp INPUT*6
                        mov  [edx+INPUT.ki.wVk],si;edx = lp INPUT*6;esi = [ITEMLISTVIEW.vkCodeSubOne]
                        mov  [edx+INPUT.ki.wScan],di;edx = lp INPUT*6;edi = [ITEMLISTVIEW.scanCodeSubOne]
                        mov  [edx+INPUT.ki.dwFlags],eax;edx = lp INPUT*6;eax = dwFlags
                        mov  [edx+INPUT.ki.time],0;edx = lp INPUT*6
                        mov  [edx+INPUT.ki.dwExtraInfo],constNumberN;edx = lp INPUT*6
                       retd  ;Изменяемые регистры: eax,ecx,edx,edi
         endp


;Дополнительный поток для работы хуков
;--------------------------------------
 proc ThreadProcForHook   !lpParameter;[esi edi ebp ebx] - так как этот поток будет работать до замого завершение программ то нету потребности регистры сохранять
  locals
   !aTempItemListView            ITEMLISTVIEW
  endl
           xor  ebx,ebx;ebx = 0

        ;Организуем для этого потока очердь сообщений
        ;ebx = 0
        invoke  PeekMessageW,edi,ebx,WM_USER,WM_USER,PM_NOREMOVE;ebx = 0

 ;Установим событие что очередь уже есть - можно отправлять сообщения сюда
 ;ebx = 0
.lpExceptionGuardian:;Сознательно используем исключение для проверки на отладку
        invoke  SetEvent,[ebx];Должно быть hEventThread  вместо ebx
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

     ;Вечный цикл выборки сообщений этого потока
     ;ebx = 0
     ;--------------------------------------
     .LoopMsg:
           xor  ebx,ebx;ebx = 0
           mov  esi,aMsgForHook;esi = aMsgForHook
        invoke  GetMessageW,esi,ebx,ebx,ebx;ebx = 0;esi = aMsgForHook
           mov  eax,[esi+MSG.message];esi = aMsgForHook

           cmp  eax,WM_APP_HOOK_Play
            je  .WM_APP_HOOK_Play;ebx = 0
           cmp  eax,WM_APP_HOOK_Stop
            je  .WM_APP_HOOK_Stop;ebx = 0

           ;Подготавливаем
           ;ebx = 0;esi = aMsgForHook
           mov  ebp,[esi+MSG.lParam];esi = aMsgForHook;ebp = ITEMLISTVIEW
           mov  esi,[esi+MSG.wParam];esi = aMsgForHook;esi = [ITEMLISTVIEW.vkCodeSubOne]
           mov  [iInputCount],ebx;ebx = 0;Отвечает за количество структур INPUT отправляемых SendInput
           mov  [lpaInputCurrent],aInput

           cmp  eax,WM_APP_HOOK_COMBINATION_KEY
            je  .WM_APP_HOOK_COMBINATION_KEY;ebx = 0;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
           cmp  eax,WM_APP_HOOK_ONE_KEY
            je  .WM_APP_HOOK_ONE_KEY;ebx = 0;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
           cmp  eax,WM_APP_HOOK_ONE_KEY_RELEASED
            je  .WM_APP_HOOK_ONE_KEY_RELEASED;ebx = 0;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
           cmp  eax,WM_APP_HOOK_COMB_ONE_KEY_RELEASED
            je  .WM_APP_HOOK_COMB_ONE_KEY_RELEASED;ebx = 0;ebp = [ITEMLISTVIEW.scanCodeSubOne];esi = [ITEMLISTVIEW.vkCodeSubOne]

           jmp  .LoopMsg


      ;Активируем хуки основные
      ;ebx = 0
     ;--------------------------------------
     .WM_APP_HOOK_Play:
                    mov  esi,[SetWindowsHookExW];esi = [SetWindowsHookExW]
                stdcall  esi,WH_MOUSE_LL ,LowLevelMouseProc,ebx,ebx;ebx = ноль;esi = [SetWindowsHookExW]
                        _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                   mov  [hhookMouse],eax

                stdcall  esi,WH_KEYBOARD_LL,LowLevelKeyboardProc,ebx,ebx;ebx = ноль;esi = [SetWindowsHookExW]
                        _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                   mov  [hhookKeyboard],eax

                    jmp   .LoopMsg

      ;Деактивируем хуки основные
      ;ebx = 0
     ;--------------------------------------
     .WM_APP_HOOK_Stop:
                    mov  ebp,[UnhookWindowsHookEx];ebp = [UnhookWindowsHookEx]
                stdcall  ebp,[hhookKeyboard];ebp = [UnhookWindowsHookEx]
                         _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                    mov  [hhookKeyboard],ebx;ebx = 0;что бы знать хук работает или нет и при выходе из приложения знать удалять его или нет

                stdcall  ebp,[hhookMouse];ebp = [UnhookWindowsHookEx]
                         _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                    xor  eax,eax
                    mov  edi,aHook
                    mov  ecx,sizeof.HOOK/4
                    cld
                    rep  stosd;Обнуляем буфер для удерживаемых клавиш, что бы не было неожиданастей

                    jmp  .LoopMsg

             ;Нужно симулировать отнажатие одной клавиши или мыши в комбинации
             ;ebx = 0;ebp = [ITEMLISTVIEW.scanCodeSubOne];esi = [ITEMLISTVIEW.vkCodeSubOne]
             ;--------------------------------------
           .WM_APP_HOOK_COMB_ONE_KEY_RELEASED:;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = [ITEMLISTVIEW.scanCodeSubOne]
                        ;Подготавливаем
                        mov  [!aTempItemListView.scanCodeSubOne],ebp
                        lea  ebp,[!aTempItemListView];ebp = ITEMLISTVIEW

             ;Нужно симулировать отнажатие одной клавиши или мыши
             ;ebx = 0;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
             ;--------------------------------------
           .WM_APP_HOOK_ONE_KEY_RELEASED:;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW
                        ;Подготавливаем
                        mov  edi,KEYEVENTF_KEYUP

                       ;Записывает в INPUT информацио о нажатой клавише или кнопке мыше
                       ;Нужные данные: ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = KEYEVENTF_KEYUP или DOWN
                       ;Изменяемые регистры: eax,ecx,edx,edi
                       call  MakeInputKeyMouse
                        jmp  .SendInput

             ;Нужно симулировать нажатие одной клавиши или мыши
             ;ebx = 0;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
             ;--------------------------------------
            .WM_APP_HOOK_ONE_KEY:;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW
                        ;Подготавливаем
                        mov  ecx,aHook;ecx = aHook
                        xor  edi,edi;edi = DOWN(0)
                        mov  ebx,[ebp+ITEMLISTVIEW.vkCodeItemOne];ebp = ITEMLISTVIEW;ebx = byte[KBDLLHOOKSTRUCT.vkCode]

                        ;Подготавливаем
                        ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];ebx = byte[KBDLLHOOKSTRUCT.vkCode];edi = DOWN(0);ecx = aHook
                        mov  eax,[ebp+ITEMLISTVIEW.scanCodeSubOne];ebp = ITEMLISTVIEW;eax = [ITEMLISTVIEW.scanCodeSubOne]

                         ;Получим предытущее действие выполняемое с текущей нажатой клавишей
                      movzx  edx,byte[ecx+HOOK.aKeyPressedMapColizion+ebx];ebx = byte[KBDLLHOOKSTRUCT.vkCode];ecx = aHook;edx = prev[ITEMLISTVIEW.vkCodeSubOne]

                       test  eax,eax;eax = [ITEMLISTVIEW.scanCodeSubOne]
                     cmovnz  edi,esp;edi = DOWN(0)
                        cmp  ebx,0xFF;ebx = byte[KBDLLHOOKSTRUCT.vkCode]
                      cmovb  edi,esp;edi = DOWN(0)
                        cmp  esi,0xFF;esi = [ITEMLISTVIEW.vkCodeSubOne]
                      cmova  edi,esp;edi = DOWN(0)
                       test  edi,edi
                         jz  .ActionOne_TwoMouse;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]

               ;Проверим может текущая кнопка которую нужно нажать - это мышь и она уже была ранее нажата - тогда просто блокируем кнопку сейчас нажимаемую
               ;Если сканкод равен нулю и предыдущий vkCodeSubOne совпадает с текущим тогда eax равен нулю и блокируем просто клавишу
               ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];ebx = byte[KBDLLHOOKSTRUCT.vkCode];ecx = aHook;edx = prev[ITEMLISTVIEW.vkCodeSubOne];eax = [ITEMLISTVIEW.scanCodeSubOne]
                        sub  eax,edx;eax = [ITEMLISTVIEW.scanCodeSubOne];edx = prev[ITEMLISTVIEW.vkCodeSubOne]
                        add  eax,esi;esi = [ITEMLISTVIEW.vkCodeSubOne]
                         jz  .ReleaseSRWLockShared

                        ;Сохраняем нажатия которые могут удерживаться в нажатом состоянии - в случае надобности для освобождения
                        ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];ebx = byte[KBDLLHOOKSTRUCT.vkCode];ecx = aHook;edx = prev[ITEMLISTVIEW.vkCodeSubOne]
                        mov  eax,0xFF;0xFF - значит просто клавишу при отпускании заблокируем - действий не требуеться - это для тех случаев когда калесо мыши крутиться
                        cmp  esi,eax
                      cmovb  eax,esi
                        xor  edi,edi;edi = DOWN(0)
                        mov  byte[ecx+HOOK.aKeyPressedMapColizion+ebx],al;ebx = byte[KBDLLHOOKSTRUCT.vkCode];ecx = aHook

                       ;Проверим может это клавиша в другой группе чем предытущая нажатая была раньше причем с другим действием если так - то ее освободим
                       ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = DOWN(0);edx = prev[ITEMLISTVIEW.vkCodeSubOne]
                       test  edx,edx
                      cmovz  edi,esp;edi = DOWN(0)
                        cmp  edx,esi;esi = [ITEMLISTVIEW.vkCodeSubOne]
                      cmove  edi,esp;edi = DOWN(0)
                        cmp  edx,0xFF;0xFF - значит просто клавишу при отпускании заблокируем - действий не требуеться
                      cmove  edi,esp;edi = DOWN(0)
                       test  edi,edi
                        jnz  .NotUpPrevKey;ebp = ITEMLISTVIEW;esi = byte[KBDLLHOOKSTRUCT.vkCode]

                        ;Сохраняем для того что бы потом востановить
                        mov  ebx,ebp;ebp = ITEMLISTVIEW;ebx = old ITEMLISTVIEW

                        ;Подготавливаем
                        mov  ecx,aListViewItem;ecx = aListViewItem
                        mov  esi,edx;esi = [ITEMLISTVIEW.vkCodeSubOne]
                        mov  ebp,ecx;ecx = aListViewItem
                        mov  edi,KEYEVENTF_KEYUP

                        ;Найдем сканкод  клавиши
                        ;ebp = aListViewItem;esi = [ITEMLISTVIEW.vkCodeSubOne];ecx = ITEMLISTVIEW
                        @@:
                        mov  ebp,[ebp+ITEMLISTVIEW.lpNextItem];ebp = ITEMLISTVIEW
                        cmp  ebp,ecx;ecx = aListViewItem
                         je  LowLevelKeyboardProc.SkipUlockBag;Походу баг...???

                        cmp  [ebp+ITEMLISTVIEW.vkCodeSubOne],esi;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
                        jne  @b

                        ;Записывает в INPUT информацио о нажатой клавише или кнопке мыше
                        ;Нужные данные: ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = KEYEVENTF_KEYUP или DOWN
                        ;Изменяемые регистры: eax,ecx,edx,edi
                        call  MakeInputKeyMouse

                        ;Востанавливаем сохраненые данные
                        mov  ebp,ebx;ebp = ITEMLISTVIEW;ebx = old ITEMLISTVIEW
                        mov  esi,[ebx+ITEMLISTVIEW.vkCodeSubOne];ebx = old ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]

             .NotUpPrevKey:
                        mov  edi,KEYEVENTF_KEYUP

                       ;Подготавливает события UP для тех случаев когда в итеме есть клавиши модификаторы, что бы не было глюков при выполнении клавиш в субитемах
                       ;Нужные данные: ebp = ITEMLISTVIEW;edi = KEYEVENTF_KEYUP или DOWN
                       ;Изменяемые регистры: eax, ecx, edx
                       call  MakeModifeter

                        xor  edi,edi;edi = DOWN(0)

                       ;Записывает в INPUT информацио о нажатой клавише или кнопке мыше
                       ;Нужные данные: ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = KEYEVENTF_KEYUP или DOWN
                       ;Изменяемые регистры: eax,ecx,edx,edi
                       call  MakeInputKeyMouse
                        jmp  .SendInput

  ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
 .ActionOne_TwoMouse:
           ;Подготавливаем
           ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
           mov  edi,KEYEVENTF_KEYUP

           ;Отвечает за один или два модификатара с итемав , что бы не было глюкав
           ;Подготавливает события UP для тех случаев когда в итеме есть клавиши модификаторы, что бы не было глюков при выполнении клавиш в субитемах
           ;Нужные данные: ebp = ITEMLISTVIEW;edi = KEYEVENTF_KEYUP или DOWN
           ;Изменяемые регистры: eax, ecx, edx
          call  MakeModifeter

           ;Подготавливаем
           ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
           xor  edi,edi;edi = DOWN(0)

          ;Нажимаем одну клавишу или кнопку мыши с суб итемав
          ;Записывает в INPUT информацио о нажатой клавише или кнопке мыше
          ;Нужные данные: ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = KEYEVENTF_KEYUP или DOWN
          ;Изменяемые регистры: eax,ecx,edx,edi
          call  MakeInputKeyMouse

           ;Подготавливаем
           ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
           mov  edi,KEYEVENTF_KEYUP

          ;Нажимаем одну клавишу или кнопку мыши с суб итемав
          ;Записывает в INPUT информацио о нажатой клавише или кнопке мыше
          ;Нужные данные: ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = KEYEVENTF_KEYUP или DOWN
          ;Изменяемые регистры: eax,ecx,edx,edi
          call  MakeInputKeyMouse
           jmp  .SendInput


     ;Выполним комбинацию, приходиться сдесь что бы не было глюков с мышьей если выполнить в LowLevelKeyboardProc сразу DOWN и UP для мыши
     ;ebx = 0;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
     ;--------------------------------------
     .WM_APP_HOOK_COMBINATION_KEY:;wParam = [ITEMLISTVIEW.vkCodeSubOne];lParam = ITEMLISTVIEW

           ;Подготавливаем
           ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne]
           mov  edi,KEYEVENTF_KEYUP

           ;Отвечает за один или два модификатара с итемав , что бы не было глюкав
           ;Подготавливает события UP для тех случаев когда в итеме есть клавиши модификаторы, что бы не было глюков при выполнении клавиш в субитемах
           ;Нужные данные: ebp = ITEMLISTVIEW;edi = KEYEVENTF_KEYUP или DOWN
           ;Изменяемые регистры: eax, ecx, edx
          call  MakeModifeter

           ;Подготавливаем
           xor  edi,edi;edi = DOWN(0)
           add  ebp,-(ITEMLISTVIEW.vkCodeItemOne-ITEMLISTVIEW.vkCodeSubOne);ebp = ITEMLISTVIEW;Коректируем что бы использовать одну и туже функцию

           ;Нажимаем один или два модификатара с суб итемав
           ;Подготавливает события UP для тех случаев когда в итеме есть клавиши модификаторы, что бы не было глюков при выполнении клавиш в субитемах
           ;Нужные данные: ebp = ITEMLISTVIEW;edi = KEYEVENTF_KEYUP или DOWN
           ;Изменяемые регистры: eax, ecx, edx
          call  MakeModifeter

           ;Подготавливаем
           sub  ebp,-(ITEMLISTVIEW.vkCodeItemOne-ITEMLISTVIEW.vkCodeSubOne);ebp = ITEMLISTVIEW;Востанавливаем

          ;Нажимаем одну клавишу или кнопку мыши с суб итемав
          ;Записывает в INPUT информацио о нажатой клавише или кнопке мыше
          ;Нужные данные: ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = KEYEVENTF_KEYUP или DOWN
          ;Изменяемые регистры: eax,ecx,edx,edi
          call  MakeInputKeyMouse

          ;Проверим может не нужно отжимать
           cmp  esi,VK_MOUSEWHEEL_UP;esi = [ITEMLISTVIEW.vkCodeSubOne]
           jae  .NotActionCombinations

           ;Подготавливаем
           mov  edi,KEYEVENTF_KEYUP

          ;Отжимаем одну клавишу или кнопку мыши с суб итемав
          ;Записывает в INPUT информацио о нажатой клавише или кнопке мыше
          ;Нужные данные: ;ebp = ITEMLISTVIEW;esi = [ITEMLISTVIEW.vkCodeSubOne];edi = KEYEVENTF_KEYUP или DOWN
          ;Изменяемые регистры: eax,ecx,edx,edi
          call  MakeInputKeyMouse

           ;Подготавливаем
.NotActionCombinations:
           mov  edi,KEYEVENTF_KEYUP
           add  ebp,-(ITEMLISTVIEW.vkCodeItemOne-ITEMLISTVIEW.vkCodeSubOne);ebp = ITEMLISTVIEW;Коректируем что бы использовать одну и туже функцию

           ;Отжимаем один или два модификатара с суб итемав
           ;Подготавливает события UP для тех случаев когда в итеме есть клавиши модификаторы, что бы не было глюков при выполнении клавиш в субитемах
           ;Нужные данные: ebp = ITEMLISTVIEW;edi = KEYEVENTF_KEYUP или DOWN
           ;Изменяемые регистры: eax, ecx, edx
          call  MakeModifeter

   .SendInput:
        invoke  SendInput,[iInputCount],aInput,sizeof.INPUT
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


        ;Освобождаем блокировку полученную из LowLevelKeyboardProc
.ReleaseSRWLockShared:
        invoke  ReleaseSRWLockShared,aRWKeyMap;Так как эти даные может изменять основной поток
           jmp   .LoopMsg

            ;ret - Выходить с потока не будем
 endp



include  'ProcOther.inc'

include  '..\Include\My\FunctionX32.inc'