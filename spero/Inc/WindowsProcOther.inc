;Для получения доступа к классу диалог - сразу же удаляеться
;--------------------------------------
 proc DlgProcEmpty !HwndDlg,!Wmsg,!Wparam,!Lparam
                 xor  eax,eax
                 ret
 endp



;Диалог отвечает за получение клавиш
;--------------------------------------
 proc DlgProcAddKeys [esi edi ebp ebx],!HwndDlg,!Wmsg,!Wparam,!Lparam
  locals
      !aTestCmpSet           dd  ?
      !aTestCmpTo            dd  ?
      !aRectAddAction        RECT
      !aAddItem              LVITEMW
      !aChangeItem           LVITEMW
      !aBufferString         du  tbKeylengthMax dup(?)
                             du  tbKeylengthMax dup(?)
  endl
                 mov  ebp,[!HwndDlg];ebp = Хендл диолога
                 mov  eax,[!Wmsg];eax = сообщение
                 mov  ecx,[!Wparam];ecx = !Wparam
                 mov  edx,[!Lparam];edx = !Lparam
                 xor  ebx,ebx;ebx = ноль


;ebp = Хендл диолога ;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------------------------------------------------------
                 cmp  eax,WM_COMMAND
                  je  .WM_COMMAND
                 cmp  eax,WM_MENUSELECT
                  je  .WM_MENUSELECT
                 cmp  eax,WM_INITDIALOG
                  je  .WM_INITDIALOG
                 cmp  eax,WM_DESTROY
                  je  .WM_DESTROY


             .False:
                 xor  eax,eax
                 ret

         ;Осводим ресурсы требуещие этого
         ;ebx = ноль
         ;--------------------------------------------------------------------------------------
        .WM_DESTROY:
                 ;Подготовим
                 mov  esi,aMenuState;esi = aMenuState

              ;Освободим ресурс не нужный
              ;ebx = ноль;esi = aMenuState
              ;-------------------------------------------
              invoke  DestroyIcon,[hiconAdd]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

               ;Освободим ресурс не нужный
               ;ebx = ноль;esi = aMenuState
               ;-------------------------------------------
               invoke  DestroyMenu,[esi+MENU_STATE.hmenuAddAction];esi = aMenuState
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 ;Обнулим что бы не было колизей при следующем запуски диалога
                 ;ebx = ноль;esi = aMenuState
                 mov  [esi+MENU_STATE.SubAddActionModif_1],ebx;ebx = ноль;esi = aMenuState
                 mov  [esi+MENU_STATE.SubAddActionModif_2],ebx;ebx = ноль;esi = aMenuState

                 jmp  .False


;Отвечает за подсказки к меню
;ebp = Хендл диолога;ebx = ноль;ecx = !Wparam;edx = !Lparam
;--------------------------------------------------------------------------------------
.WM_MENUSELECT:
       ;Подготовим
       mov  esi,aMenuState;esi = aMenuState

       ;Проверим выходим ли с меню в котором показывали в статике подсказки, если так то очистим текст
       cmp  ecx,[esi+MENU_STATE.iForStaticMenu];esi = aMenuState
        je  .FreeStaticText;ebx = ноль;esi = aMenuState

      ;Проверяем может это системное меню - тогда ничего неделаем
      test  ecx,MF_SYSMENU shl 16
    cmovnz  ebx,esp;ebx = ноль

      ;Проверяем может это обьекты типа separator который не выделяеться не имеет текста и соответственно нам не интересен - тогда ничего неделаем
      test  ecx,MF_HILITE shl 16
     cmovz  ebx,esp;ebx = ноль

      ;Если не равно нулю не наш случай
      test  ebx,ebx;ebx = ноль
       jnz  .True

      ;Проверяем может это подменю тогда по другому обрабатываем
      test  ecx,MF_POPUP  shl 16
       jnz  .MF_POPUP;esi = aMenuState;ebx = ноль;ecx = !Wparam;edx = !Lparam

       ;Если id menu равен нулю - не наш случай, так как все используемые меню имеют не нулевой индифактор
       and  ecx,0xFFFF;eсx = id menu
     cmovz  ebx,esp;ebx = ноль

       ;Если id menu больше или равно данного числа значит это меню не входит в список меню имеющих подсказки
       cmp  ecx,MENU_HELP_LAST_Id;eсx = id menu
    cmovae  ebx,esp;ebx = ноль

      ;Если не равно нулю не наш случай;Но еще проверим может мы находимся на на пункте меню не имеющего подсказку тогда если текст отображаеться в статике - стирем его
      test  ebx,ebx;ebx = ноль;esi = aMenuState
       jnz  .FreeStaticTextTest

       ;Так как id menu совпадает с id STR подсказки - получаем текст строки подсказки
       ;esi = aMenuState;ebx = ноль;eсx = id menu
       ;-------------------------------------------
   stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR_MENU,ecx;eсx = id menu

    ;Установим текст подсказку
    ;ebx = ноль;esi = aMenuState
    invoke  SendMessageW,[esi+MENU_STATE.hwndAddAddKeysStatic],WM_SETTEXT,ebx,eax;ebx = ноль;esi = aMenuState
            _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

       ;Установим
       mov  [esi+MENU_STATE.iForStaticMenu],0xFFFF0000;esi = aMenuState

       jmp  .True

             ;Проверим что бы при переходе на меню не отображающего подсказки подсказка стерта предыдущая была;Если не равно нулю - стираем предыдущий текст
             ;esi = aMenuState
             ;-------------------------------------------
            .FreeStaticTextTest:
                             xor  ebx,ebx;ebx = ноль
                             cmp  [esi+MENU_STATE.iForStaticMenu],ebx;esi = aMenuState;ebx = ноль
                              je  .True

                 ;Очистим статик что бы не остался текст связаный с помощью меню
                 ;esi = aMenuState;ebx = ноль
                 ;-------------------------------------------
                .FreeStaticText:
                         stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_EMPTY

                          invoke  SendMessageW,[esi+MENU_STATE.hwndAddAddKeysStatic],WM_SETTEXT,ebx,eax;ebx = ноль;esi = aMenuState
                                  _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                             ;Перед выходом сбросим в ноль
                             mov  [esi+MENU_STATE.iForStaticMenu],ebx;ebx = ноль;esi = aMenuState

                             jmp  .True

                       ;Вывидим если надо подсказку для подменю
                       ;esi = aMenuState;ebx = ноль;ecx = !Wparam;edx = !Lparam
                       ;-------------------------------------------
                      .MF_POPUP:
                             ;Подготовим
                             and  ecx,0xFFFF;ecx = index

                             cmp  edx,[esi+MENU_STATE.hmenuSubAddAction0Lvl]
                              je  .hmenuSubAddAction0Lvl;esi = aMenuState;ebx = ноль;ecx = index
                             cmp  edx,[esi+MENU_STATE.hmenuSubAddManual1Lvl]
                              je  .hmenuSubAddAction1Lvl;esi = aMenuState;ebx = ноль;ecx = index
                             jmp  .True

                             ;Подменю первого уровня
                             ;esi = aMenuState;ebx = ноль;ecx = index
                            .hmenuSubAddAction1Lvl:
                                                mov  eax,STR_HELP_Modifiers
                                                cmp  ecx,MENU_ADD_2LVL_MODIF_Index;ecx = index
                                              cmove  ebx,eax;ebx = ноль

                                                mov  eax,STR_HELP_Mouse
                                                cmp  ecx,MENU_ADD_2LVL_MOUSE_Index;ecx = index
                                              cmove  ebx,eax;ebx = ноль
                                                jmp  .TestHelp

                             ;Подменю нулевого уровня
                             ;esi = aMenuState;ebx = ноль;ecx = index
                            .hmenuSubAddAction0Lvl:
                                                mov  eax,STR_HELP_Manual
                                                cmp  ecx,MENU_ADD_1LVL_MANUAL_Index;ecx = index
                                              cmove  ebx,eax;ebx = ноль

                                                mov  eax,STR_HELP_Monitor
                                                cmp  ecx,MENU_ADD_1LVL_MONITOR_Index;ecx = index
                                              cmove  ebx,eax;ebx = ноль

                                          ;Проверим нужно ли подсказку выводить или нет;Если равно нулю то ничего не делаем
                                          ;esi = aMenuState;ebx = STR_XXX_xxx
                                         .TestHelp:
                                               test  ebx,ebx
                                                 jz  .True

                                            ;Отобразим подсказку
                                            stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,ebx;ebx = STR_XXX_xxx
                                             invoke  SendMessageW,[esi+MENU_STATE.hwndAddAddKeysStatic],WM_SETTEXT,0,eax;esi = aMenuState
                                                     _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                                                ;Установим
                                                mov  [esi+MENU_STATE.iForStaticMenu],0xFFFF0000;esi = aMenuState

                                                jmp  .True



;Обрабатываем сообщения от Control, Accelerator, Menu
;ebp = Хендл диолога ;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------------------------------------------------------
        .WM_COMMAND:
                 cmp  ecx,IDCANCEL
                  je  .Exit;ebp = Хендл диолога ;ebx = ноль
                 cmp  ecx,IDOK
                  je  .Ok;ebp = Хендл диолога ;ebx = ноль
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_KEYMAP_BUTTON_ADD_Id;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .DIALOG_KEYMAP_BUTTON_ADD_Id;ebp = Хендл диолога ;ebx = ноль

                 jmp  .True


;Будем показывать меню доп действий при нажатии кнопки
;ebp = Хендл диолога ;ebx = ноль;edx = хенд окна контрола
;--------------------------------------------------------------------------------------
.DIALOG_KEYMAP_BUTTON_ADD_Id:

           ;Подготовим
           lea  esi,[!aRectAddAction];esi = !aRectAddAction
           mov  edi,aMenuState;edi = aMenuState

        invoke  GetWindowRect,edx,esi;edx = хенд окна контрола;esi = !aRectAddAction
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

        invoke  TrackPopupMenuEx,[edi+MENU_STATE.hmenuSubAddAction0Lvl],TPM_NONOTIFY or TPM_RETURNCMD,[esi+RECT.left],[esi+RECT.top],\
                                 ebp,ebx;ebp = Хендл диолога;esi = !aRectAddAction;ebx = ноль;edi = aMenuState
           ;Если равно нулю - пункт меню не выбран - ничего не делаем
          test  eax,eax
            jz  .True


           ;Проверим может это пункты меню отвечающие за  получение vkCode клавиш в при отсутствии их физически на клавиатуре;Если больше или равно MENU_TABLE_KEY_Id значит так и есть
           cmp  eax,MENU_TABLE_KEY_Id
           jae  .MENU_TABLE_KEY_Id;ebp = Хендл диолога;ebx = ноль;edi = aMenuState

           ;Проверим может это пункт меню входящий в таблицу для прижков
           cmp  eax,MENU_TABLE_ADD_Id
           jae  .MENU_TABLE_ADD_Id;ebp = Хендл диолога;ebx = ноль;edi = aMenuState;eax = id menu

           ;Действия различные
           ;ebp = Хендл диолога;ebx = ноль;edi = aMenuState;eax = id menu
           ;-------------------------------------------
           cmp  eax,MENU_LAUNCH_FILE_Id
            je  .MENU_LAUNCH_FILE_Id;ebp = Хендл диолога;ebx = ноль
           cmp  eax,MENU_LOCK_KEY_Id
            je  .MENU_LOCK_KEY_Id;ebx = ноль
           cmp  eax,MENU_ADD_2LVL_MONITOR_OffUniv
            je  .MENU_ADD_2LVL_MONITOR_OffUniv;ebx = ноль

           jmp  .True


 ;Пригнем в соотвентсвии с таблицей
 ;ebp = Хендл диолога;ebx = ноль;edi = aMenuState;eax = id menu
 ;-------------------------------------------
.MENU_TABLE_ADD_Id:
         movzx   ecx,word[JmpMENU_TABLE_ADD_Id+eax*2-MENU_TABLE_ADD_Id*2];на основании таблицы получим смещение относительно Start к нашей цели
           add  ecx,Start;Каректируем
           jmp  ecx;ebp = Хендл диолога;ebx = ноль;edi = aMenuState;eax = id menu

 ;Если меньше или равно значит это не id меню содержащее vkCode в себе - тогда нечего неделаем
 ;ebx = ноль;edi = aMenuState
 ;-------------------------------------------
.MENU_TABLE_KEY_Id:
           sub   eax,MENU_TABLE_KEY_Id;eax = vkCode

           ;Подготовим
           mov  esi,aTo;esi = aTo
           mov  ebp,eax;eax = vkCode
           mov  [esi+KEYMAP.vkCodeOne],eax;eax = vkCode;esi = aTo
           mov  [esi+KEYMAP.scanCodeSubOne],ebx;ebx = ноль;esi = aTo

           ;Проверим может vkCode от мыши - тогда ссканкод не нужна получать
           ;ebx = ноль;esi = aTo;edi = aMenuState;ebp = vkCode;eax = vkCode
           cmp  eax,VK_MOUSEWHEEL_UP;eax = vkCode
        cmovae  eax,esp;eax = vkCode
           cmp  eax,VK_XBUTTON2
        cmovbe  eax,esp;eax = vkCode
           cmp  eax,VK_CANCEL
         cmove  eax,ebp;ebp = vkCode;eax = vkCode

           ;Если eax неравно ebp значит это от мыши и не надо получать сканкоде
           cmp  eax,ebp;ebp = vkCode;eax = vkCode
           jne  .YesMouseVkCode;ebx = ноль;esi = aTo;edi = aMenuState;ebp = vkCode

        ;Получим сканкод vkCode
        ;ebx = ноль;esi = aTo;edi = aMenuState;ebp = vkCode
        invoke  MapVirtualKeyExW,ebp,MAPVK_VK_TO_VSC_EX,ebx;ebp = vkCode;ebx = ноль
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

           ;Проверим есть префикс расширеной раскладки в сканкоде и с каректируем в соответствии с выбраной  мной позицеей
           ;Что старший бит если установлен в один значит это расширеный префикс иначе если равно нулю то нет
          test ah,ah;Если не равно нулю значит установим бит префикса
         setnz ah
           shl ah,7;Перенесем в старший бит ax

           ;Установим
           mov  [esi+KEYMAP.scanCodeSubOne],eax;esi = aTo

 ;Проверим первый модификатор установлен он или нет если да - тогда получим сканкод
 ;ebx = ноль;esi = aTo;edi = aMenuState;ebp = vkCode
.YesMouseVkCode:
           mov  ebx,[edi+MENU_STATE.SubAddActionModif_1];edi = aMenuState
           mov  edi,[edi+MENU_STATE.SubAddActionModif_2];edi = aMenuState
           shl  ebx,16
           mov  bx,di
           and  edi,0xFFFF
            jz  .TwoTestModifeter;ebx = vkCodeTwoThree;edi = scanCodeSubTwoThree;esi = aTo;ebp = vkCode

         ;Получим сканкод vkCodeTwoThree
         ;ebx = vkCodeTwoThree;edi = scanCodeSubTwoThree;esi = aTo;ebp = vkCode
         movzx  eax,bx;ebx = vkCodeTwoThree
        invoke  MapVirtualKeyExW,eax,MAPVK_VK_TO_VSC_EX,0;ebp = vkCode
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

           ;Проверим есть префикс расширеной раскладки в сканкоде и с каректируем в соответствии с выбраной  мной позицеей
           ;Что старший бит если установлен в один значит это расширеный префикс иначе если равно нулю то нет
          test  ah,ah;Если не равно нулю значит установим бит префикса
         setnz  ah
           shl  ah,7;Перенесем в старший бит ax
           mov  edi,eax;edi = scanCodeSubTwoThree

 ;Проверим второй модификатор установлен он или нет если да - тогда получим сканкод
 ;ebx = vkCodeTwoThree;edi = scanCodeSubTwoThree;esi = aTo;ebp = vkCode
.TwoTestModifeter:
           ;Подготовим
           rol  ebx,16;ebx = vkCodeTwoThree
           shl  edi,16;edi = scanCodeSubTwoThree
          test  bx,bx
            jz  .NotTwoModifeter;ebx = vkCodeTwoThree;edi = scanCodeSubTwoThree;esi = aTo;ebp = vkCode

         ;Получим сканкод vkCodeTwoThree
         ;ebx = vkCodeTwoThree;edi = scanCodeSubTwoThree;esi = aTo;ebp = vkCode
         movzx  eax,bx;ebx = vkCodeTwoThree
        invoke  MapVirtualKeyExW,eax,MAPVK_VK_TO_VSC_EX,0;ebp = vkCode
                _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

           ;Проверим есть префикс расширеной раскладки в сканкоде и с каректируем в соответствии с выбраной  мной позицеей
           ;Что старший бит если установлен в один значит это расширеный префикс иначе если равно нулю то нет
          test  ah,ah;Если не равно нулю значит установим бит префикса
         setnz  ah
           shl  ah,7;Перенесем в старший бит ax
           mov  di,ax;edi = scanCodeSubTwoThree

 ;Сохраним и подготовим
 ;ebx = vkCodeTwoThree;edi = scanCodeSubTwoThree;esi = aTo;ebp = vkCode
.NotTwoModifeter:
           lea  edx,[!aBufferString];edx = !aBufferString
           mov  [esi+KEYMAP.vkCodeTwoThree],ebx;ebx = vkCodeTwoThree;esi = aTo
           mov  [esi+KEYMAP.scanCodeSubTwoThree],edi;edi = scanCodeSubTwoThree;esi = aTo

       ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
       ;ebx = vkCodeTwoThree;esi = aTo;ebp = vkCode;edx = !aBufferString
       stdcall  VkCodeInString,ebp,ebx,edx;ebx = vkCodeTwoThree;ebp = vkCode;edx = !aBufferString

        ;Установим текст
        ;esi = aTo;edx = !aBufferString
        invoke  SendMessageW,[esi+KEYMAP.hwnd],WM_SETTEXT,0,edx;edx = !aBufferString;esi = aTo
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

           jmp  .True


       ;Общий модификатор ALT - установим или снимим флажок и сохраним состояние флажка
       ;ebx = ноль;edi = aMenuState
       ;-------------------------------------------
      .MENU_ADD_2LVL_MODIF_ALT_Id:
                              mov  ebp,(MENU_ADD_2LVL_MODIF_ALT_Id shl 16) or VK_MENU
                              jmp  .TestModifCheck;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState

       ;Общий модификатор CTRL - установим или снимим флажок и сохраним состояние флажка
       ;ebx = ноль;edi = aMenuState
       ;-------------------------------------------
      .MENU_ADD_2LVL_MODIF_CTRL_Id:
                              mov  ebp,(MENU_ADD_2LVL_MODIF_CTRL_Id shl 16) or VK_CONTROL
                              jmp  .TestModifCheck;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState

       ;Общий модификатор SHIFT - установим или снимим флажок и сохраним состояние флажка
       ;ebx = ноль;edi = aMenuState
       ;-------------------------------------------
      .MENU_ADD_2LVL_MODIF_SHIFT_Id:
                              mov  ebp,(MENU_ADD_2LVL_MODIF_SHIFT_Id shl 16) or VK_SHIFT


                  ;Определим надо отметить пунк меню галочкой или снять галочку
                  ;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState
                  ;-------------------------------------------
                 .TestModifCheck:
                              ;Подготовим
                              mov  esi,aMenuStateChange;esi = aMenuStateChange
                              mov  eax,[edi+MENU_STATE.SubAddActionModif_1];edi = aMenuState;eax = [edi+MENU_STATE.SubAddActionModif_1]
                              mov  ecx,[edi+MENU_STATE.SubAddActionModif_2];edi = aMenuState;ecx = [edi+MENU_STATE.SubAddActionModif_2]
                              mov  [esi+MENUITEMINFOW.fMask],MIIM_STATE;esi = aMenuStateChange

                              ;Если совпадают - снимаем галочку
                              cmp  eax,ebp;ebp = id menu + vkCode
                               je   .ModifUnCheck_1;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange;eax = [edi+MENU_STATE.SubAddActionModif_1]
                              cmp  ecx,ebp;ebp = id menu + vkCode
                               je   .ModifUnCheck_2;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange;ecx = [edi+MENU_STATE.SubAddActionModif_2]

                             ;Если равно нулю значит свободно можно - ставить галочку
                             test  eax,eax
                               jz   .Check_1;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange
                             test  ecx,ecx
                               jz   .ModifCheck_2;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange

                              ;Раз тут оказались значит двое слотов заняты будем снимать с одного галочку а а на тольчто поступивший установим галочку
                              ;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange;eax = id menu UnCheck
                              ;Подготовим
                              shr  eax,16;eax = id menu UnCheck
                              mov  [esi+MENUITEMINFOW.fState],ebx;ebx = ноль;esi = aMenuStateChange;MFS_UNCHECKED = 0

                           ;Сбросим галочку
                           ;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange;eax = id menu UnCheck
                           invoke  SetMenuItemInfoW,[edi+MENU_STATE.hmenuAddAction],eax,ebx,esi;ebx = ноль;edi = aMenuState;esi = aMenuStateChange;eax = id menu UnCheck
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                          ;Подготовим
                          ;ebx = state;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange
                         .Check_1:
                              mov  [edi+MENU_STATE.SubAddActionModif_1],ebp;ebp = id menu + vkCode;edi = aMenuState
                         .Check_2:
                              mov  ebx,MFS_CHECKED

                    ;Подготовим
                    ;ebx = state;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange
                   .CheckUnCheck:
                              shr  ebp,16;ebp = id menu + vkCode
                              mov  [esi+MENUITEMINFOW.fState],ebx;ebx = state;esi = aMenuStateChange

                           ;Сбросим галочку или установим
                           ;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange
                           invoke  SetMenuItemInfoW,[edi+MENU_STATE.hmenuAddAction],ebp,0,esi;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                              jmp  .True

                                                      ;Снимемем галочку ;MFS_UNCHECKED = 0
                                                      ;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange;eax = [edi+MENU_STATE.SubAddActionModif_1]
                                                     .ModifUnCheck_1:
                                                                 xchg  ebp,eax;eax = [edi+MENU_STATE.SubAddActionModif_1]
                                                                  mov  [edi+MENU_STATE.SubAddActionModif_1],ebx;ebx = ноль;edi = aMenuState
                                                                  jmp  .CheckUnCheck;ebx = state;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange

                                                      ;Снимемем галочку ;MFS_UNCHECKED = 0
                                                      ;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange;ecx = [edi+MENU_STATE.SubAddActionModif_2]
                                                     .ModifUnCheck_2:
                                                                 xchg  ebp,ecx;ecx = [edi+MENU_STATE.SubAddActionModif_2]
                                                                  mov  [edi+MENU_STATE.SubAddActionModif_2],ebx;ebx = ноль;edi = aMenuState
                                                                  jmp  .CheckUnCheck;ebx = state;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange

                                                        ;Ставим галочку раз свободно
                                                        ;ebx = ноль;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange
                                                       .ModifCheck_2:
                                                                  mov  [edi+MENU_STATE.SubAddActionModif_2],ebp;edi = aMenuState
                                                                  jmp  .Check_2;ebx = state;ebp = id menu + vkCode;edi = aMenuState;esi = aMenuStateChange


   ;Добавляет в структуру aTo действие - выключение экрана через SC_MONITORPOWER
   ;ebx = ноль
   ;-------------------------------------------
  .MENU_ADD_2LVL_MONITOR_OffUniv:
                              ;Установим
                              mov  eax,VK_OFFUNIV;eax = Action
                              mov  esi,szOffUniv;esi = lpData

                              ;длину строки в байтах  включая ноль так как другие данные не используються
                              mov  ecx,sizeof.szOffUniv+2;ecx = sizeof all Data
                              ;ebx = ноль;В случае с VK_OFFUNIV никакие доп даные не используються

                              jmp  .TextAction;eax = Action;esi = lpData;ebx = add Data;ecx = sizeof all Data


    ;Добавляет в структуру aTo действие - запуск хранителя экрана
    ;ebx = ноль
    ;-------------------------------------------
   .MENU_ADD_2LVL_MONITOR_ScrSav:
                              ;Установим
                              mov  eax,VK_SCRSAV;eax = Action
                              mov  esi,szScreensaver;esi = lpData

                              ;длину строки в байтах  включая ноль так как другие данные не используються
                              mov  ecx,sizeof.szScreensaver+2;ecx = sizeof all Data
                              ;ebx = ноль;В случае с VK_SCRSAV никакие доп даные не используються

                              jmp  .TextAction;eax = Action;esi = lpData;ebx = add Data;ecx = sizeof all Data

                ;Добавляет в структуру aTo действие - блокировку комбинации или клавиши
                ;ebx = ноль
                ;-------------------------------------------
               .MENU_LOCK_KEY_Id:
                              ;Установим
                              mov  eax,VK_LOCK_KEY;eax = Action
                              mov  esi,szLockKey;esi = lpData

                              ;длину строки в байтах  включая ноль так как другие данные не используються
                              mov  ecx,sizeof.szLockKey+2;ecx = sizeof all Data
                              ;ebx = ноль;В случае с VK_LOCK_KEY никакие доп даные не используються

                              ;УстановимСохраняем и отобразим
                              ;eax = Action;esi = lpData;ebx = add Data;ecx = sizeof all Data
                     .TextAction:
                              mov  edx,aTo;edx = aTo.KEYMAP
                              mov  [edx+KEYMAP.vkCodeOne],eax;eax = Action
                              mov  [edx+KEYMAP.vkCodeTwoThree],esi;esi = lpData
                              mov  [edx+KEYMAP.scanCodeSubOne],ebx;ebx = add Data

                              ;каректируем добовляем размер доп данных для получения указателя на строку
                              add  esi,ebx

                              mov  [edx+KEYMAP.scanCodeSubTwoThree],ecx;ecx = sizeof all Data

                           invoke  SendMessageW,[edx+KEYMAP.hwnd],WM_SETTEXT,0,esi;edx = aTo.KEYMAP;esi = lpText
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                              jmp  .True


             ;Добавляет в структуру aTo действие - запуск файла
             ;ebp = Хендл диолога ;ebx = ноль
             ;-------------------------------------------
            .MENU_LAUNCH_FILE_Id:
                              ;Вызываем стандартный диалог для открытия что бы пользователь выбрал нужное приложение
                              ;ebp = Хендл диолога ;ebx = ноль
                              mov  esi,aOpenFileExe;esi = aOpenFileExe
                          stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_LAUNCH_FILE_Filtr

                              mov  edi,aIniFileData.szLaunchfileBufer;edi = aIniFileData.szLaunchfileBufer
                              mov  ebx,aTo;ebx = aTo.KEYMAP
                              mov  [esi+OPENFILENAMEW.hwndOwner],ebp;ebp = Хендл диолога
                              mov  [esi+OPENFILENAMEW.lpstrFilter],eax
                              mov  [esi+OPENFILENAMEW.lpstrFile],edi;edi = aIniFileData.szLaunchfileBufer

                              cmp  [ebx+KEYMAP.vkCodeOne],VK_FILE_OPEN
                              jne  .NotChangeOpenFileDir;edi = aIniFileData.szLaunchfileBufer;ebx = aTo.KEYMAP;ebp = Хендл диолога

                              mov  eax,[ebx+KEYMAP.vkCodeTwoThree];lp  struct в данном случае это еще и ;lp  string
                              xor  ecx,ecx

                              @@:; Так как мы изменяем предытущее действие то каректтируем путь открытия на тот который изменяем
                              mov  dx,[eax+ecx*2]
                              mov  [edi+ecx*2],dx
                              inc  ecx
                             test  dx,dx
                              jnz  @b


            ;edi = aIniFileData.szLaunchfileBufer;ebx = aTo.KEYMAP;ebp = Хендл диолога;esi = aOpenFileExe
           .NotChangeOpenFileDir:
                           invoke  GetOpenFileNameW,esi;esi = aOpenFileExe
                             test  eax,eax
                               jz  .NotOpenFile;ebp = Хендл диолога

                              ;Считаем длину в байтах в символах включая ноль
                            movzx  ecx,[esi+OPENFILENAMEW.nFileExtension]

                              @@:
                              mov  ax,[edi+ecx*2];edi = aIniFileData.szLaunchfileBufer
                              inc  ecx;длина строки в символах
                             test  ax,ax
                              jnz  @b


                              ;Сохраняем
                              shl  ecx,1
                              xor  eax,eax;;eax = 0
                              mov  [ebx+KEYMAP.vkCodeOne],VK_FILE_OPEN;ebx = aTo.KEYMAP
                              mov  [ebx+KEYMAP.vkCodeTwoThree],edi;edi = aIniFileData.szLaunchfileBufer  ;lp  string
                              mov  [ebx+KEYMAP.scanCodeSubOne],eax;eax = 0;В случае с VK_FILE_OPEN никакие доп даные не используються
                              mov  [ebx+KEYMAP.scanCodeSubTwoThree],ecx;длину строки в байтах  включая ноль

                           invoke  SendMessageW,[ebx+KEYMAP.hwnd],WM_SETTEXT,eax,edi;ebx = aTo.KEYMAP;edi = aIniFileData.szLaunchfileBufer;eax = 0
                                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                              jmp  .True;Устанавливаем хуки отвечающие за Edit


                     ;Проверяем отменил пользователь диалог открыть файл или ошибка была; любую ошибку считаемкак слишком длинный путь о чем и сообщаем пользователю
                     ;ebp = Хендл диолога
                    .NotOpenFile:
                           invoke  CommDlgExtendedError
                             test  eax,eax;значит отменил
                               jz  .True;Устанавливаем хуки отвечающие за Edit

                              xor  ebx,ebx;ebx = ноль
                          stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_OPEN_EXE_LONG_Patch
                           invoke  MessageBoxExW,ebp,eax,ebx,MB_OK or MB_ICONINFORMATION,ebx;ebp = Хендл диолога ;ebx = ноль
                              mov  word[edi],bx;ebx = 0;edi = aIniFileData.szLaunchfileBufer;в случае маленького буфера указываеться нужный размер - затираем
                              jmp  .True;Устанавливаем хуки отвечающие за Edit


;Иницилизируем диалог
;ebp = Хендл диолога ;ebx = ноль ;edx = !Lparam
;--------------------------------------------------------------------------------------
     .WM_INITDIALOG:

;!Lparam = указатель на структуру ITEMLISTVIEW которую будем изменять сейчас или на основании которой добавим другой итем
                 mov  edi,edx;edi = выбраная структура item
                 mov  [lpChangeCurrentListViewItem],edx


;Иконку для кнопки загружаем хендл сохраняем для дальнейшего освобождения
;edi = выбраная структура item;ebp = Хендл диолога ;ebx = ноль
               invoke  LoadImageW,[hInstance],IDR_ICON_GROUP_ADD,IMAGE_ICON,ebx,ebx,ebx;;ebx = ноль LR_DEFAULTCOLOR = 0
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                  mov  [hiconAdd],eax
                  mov  ebx,eax;ebx = [hiconAdd]

;Устанавливаем иконку на кнопки доп действий
;edi = выбраная структура item;ebp = Хендл диолога
                  mov  esi,[SendDlgItemMessageW];esi = [SendDlgItemMessageW]
              stdcall  esi,ebp,DIALOG_KEYMAP_BUTTON_ADD_Id,BM_SETIMAGE,IMAGE_ICON,ebx;ebp = Хендл диолога;ebx = [hiconAdd];esi = [SendDlgItemMessageW]

                 ;Подготовим
                 mov  esi,[GetDlgItem];esi = [GetDlgItem]

;Получим хендл статика для подсказак
;edi = выбраная структура item;ebp = Хендл диолога;esi = [GetDlgItem]
             stdcall  esi,ebp,DIALOG_KEYMAP_STATIC_HELP_Id;esi = [GetDlgItem];ebp = Хендл диолога
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 mov  [aMenuState.hwndAddAddKeysStatic],eax

;Получаем хендлы EDIT что бы хук мог определить когда делать перехват нажатий и посылать информацию какая клавиша нажата
;edi = выбраная структура item;ebp = Хендл диолога;esi = [GetDlgItem]

             stdcall  esi,ebp,DIALOG_KEYMAP_EDIT_SET_Id;esi = [GetDlgItem];ebp = Хендл диолога
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


                 mov  edx,aSet;edx = aSet
                 lea  ebx,[!aBufferString]; ebx = [!aBufferString]
                 mov  [edx+KEYMAP.hwnd],eax;edx = aSet

                 mov  eax,[edi+ITEMLISTVIEW.scanCodeItemOne];edi = выбраная структура item
                 mov  ecx,[edi+ITEMLISTVIEW.scanCodeItemTwoThree];edi = выбраная структура item
                 mov  [edx+KEYMAP.scanCodeSubOne],eax;edx = aSet
                 mov  [edx+KEYMAP.scanCodeSubTwoThree],ecx;edx = aSet

                 mov  eax,[edi+ITEMLISTVIEW.vkCodeItemOne];edi = выбраная структура item
                 mov  ecx,[edi+ITEMLISTVIEW.vkCodeItemTwoThree];edi = выбраная структура item
                 mov  [edx+KEYMAP.vkCodeOne],eax;edx = aSet
                 mov  [edx+KEYMAP.vkCodeTwoThree],ecx;edx = aSet

             ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
             stdcall  VkCodeInString,eax,ecx,ebx ; ebx = [!aBufferString]

               invoke  SendMessageW,[edx+KEYMAP.hwnd],EM_SETCUEBANNER,0,ebx;ebx = [!aBufferString];edx = aSet
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

             stdcall  esi,ebp,DIALOG_KEYMAP_EDIT_TO_Id;esi = [GetDlgItem];ebp = Хендл диолога
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 mov  edx,aTo;edx = aTo
                 mov  esi,[edi+ITEMLISTVIEW.scanCodeSubOne];edi = выбраная структура item;esi = [scanCodeSubOne]
                 mov  ecx,[edi+ITEMLISTVIEW.scanCodeSubTwoThree];edi = выбраная структура item;edx = aTo
                 mov  [edx+KEYMAP.hwnd],eax;edx = aTo
                 mov  [edx+KEYMAP.scanCodeSubOne],esi;edx = aTo
                 mov  [edx+KEYMAP.scanCodeSubTwoThree],ecx;edx = aTo
                 mov  eax,[edi+ITEMLISTVIEW.vkCodeSubOne];edi = выбраная структура item
                 mov  ecx,[edi+ITEMLISTVIEW.vkCodeSubTwoThree];edi = выбраная структура item
                 mov  [edx+KEYMAP.vkCodeOne],eax;edx = aTo
                 mov  [edx+KEYMAP.vkCodeTwoThree],ecx;edi = выбраная структура item;edx = aTo;ecx = [vkCodeTwoThree]


                 ;Проверяем если не просто клавиша, а действие тогда коректируем что бы оттобразился  нужный текст
                 lea  esi,[esi+ecx];esi = [scanCodeSubOne];ecx = [vkCodeTwoThree]
                 cmp  eax,VK_LAST
               cmova  ebx,esi
                  ja  .Next

             ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
             stdcall  VkCodeInString,eax,ecx,ebx; ebx = [!aBufferString]

               .Next:
                  mov  esi,[SendMessageW];esi = [SendMessageW]
              stdcall  esi,[edx+KEYMAP.hwnd],EM_SETCUEBANNER,0,ebx;ebx = [!aBufferString];esi = [SendMessageW];edx = aTo
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


;Изменяем название диалога если вместо добавления итема будем его изменять
;edi = выбраная структура item;ebp = Хендл диолога;esi = [SendMessageW]
                 xor  ebx,ebx;ebx = 0
                 cmp  [edi+ITEMLISTVIEW.vkCodeItemOne],ebx;edi = выбраная структура item;ebx = 0
                  je  .NotChagedNameDialog

              stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_DIALOG_ADD_KEY_Name
              stdcall  esi,ebp,WM_SETTEXT,ebx,eax;ebp = Хендл диолога;esi = [SendMessageW];ebx = 0


;Загрузим меню и получим суб меню и еще пару суб меню для вывода подсказак
;ebx = 0
;-------------------------------------------
.NotChagedNameDialog:
              ;Подготовим
              mov  esi,aMenuState;esi = aMenuState
              mov  edi,[GetSubMenu];edi = GetSubMenu

          ;Загрузим с ресурса наше меню для диалога DlgProcAddKeys
          ;edi = GetSubMenu;esi = aMenuState;ebx = 0
          stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_MENUS,MENUS_ADD_ACTION
           invoke  LoadMenuIndirectW,eax
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              mov  [esi+MENU_STATE.hmenuAddAction],eax;esi = aMenuState

          ;Получим суб меню 0lvl от меню для подсказок
          ;edi = GetSubMenu;esi = aMenuState;ebx = 0
          stdcall  edi,eax,MENU_ADD_0LVL_ACTION_Index;edi = GetSubMenu
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              mov  [esi+MENU_STATE.hmenuSubAddAction0Lvl],eax;esi = aMenuState
              mov  ebp,eax;ebp = [hmenuSubAddAction0Lvl]

          ;Получим суб меню 1lvl от субменю 0lvl для подсказок
          ;edi = GetSubMenu;esi = aMenuState;ebx = 0;ebp = [hmenuSubAddAction0Lvl]
          stdcall  edi,ebp,MENU_ADD_1LVL_MANUAL_Index;edi = GetSubMenu;ebp = [hmenuSubAddAction0Lvl]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              mov  [esi+MENU_STATE.hmenuSubAddManual1Lvl],eax;esi = aMenuState

          ;Получим суб меню 1lvl от субменю 0lvl для подсказок
          ;edi = GetSubMenu;esi = aMenuState;ebx = 0;ebp = [hmenuSubAddAction0Lvl]
          stdcall  edi,ebp,MENU_ADD_1LVL_MONITOR_Index;edi = GetSubMenu;ebp = [hmenuSubAddAction0Lvl]
                   _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              mov  [esi+MENU_STATE.hmenuSubAddMonitor1Lvl],eax;esi = aMenuState

;Устанавливаем хук на клавиатуру
;-------------------------------------------
          ;.SetHookIn:

                  xor  ebx,ebx;ebx = 0
                  mov  esi,[SetWindowsHookExW];esi = [SetWindowsHookExW]
              stdcall  esi,WH_KEYBOARD_LL,LowLevelKeyboardProcInValue,ebx,ebx;ebx = ноль;esi = [SetWindowsHookExW]
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 mov  [hhookInValue],eax

;Устанавливаем хук на мышь
              stdcall  esi,WH_MOUSE_LL,LowLevelMouseProcInValue,ebx,ebx;ebx = ноль;esi = [SetWindowsHookExW]
                       _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 mov  [hhookInValueMouse],eax
                 jmp  .True



;Ok
;ebp = Хендл диолога;ebx = ноль
;--------------------------------------
                .Ok:
                 ;Подготавливаем
                 mov  edi,aSet;edi = aSet
                 mov  edx,aTo;edx =aTo
                 mov  esi,[edi+KEYMAP.vkCodeOne];edi = aSet
                 mov  ecx,[edx+KEYMAP.vkCodeOne];edx =aTo;ecx = [aTo.vkCodeOne]
                 mov  eax,esi;eax = [aSet.vkCodeOne]

                  ;Проверяем адекватность выбора клавиш или действий пользователем
                  ;ebx = ноль
                  or  esi,ecx;клавиши не какие не выберались остались только точки - действуй не требуеться = отмена
                  jz  .Exit ;ebx = ноль
                test  eax,eax;eax = [aSet.vkCodeOne]
                  jz  .ErrorEnterKey;Одна клавиша не выбрана - точки вместо ее
                test  ecx,ecx;ecx = [aTo.vkCodeOne]
                  jz  .ErrorEnterKey;Одна клавиша не выбрана - точки вместо ее


                 ;Подготавливаем
                 ;edi = aSet;edx =aTo
                 mov  ebx,[lpChangeCurrentListViewItem];ebx =  lpChangeCurrentListViewItem
                 mov  ebp,[edi+KEYMAP.vkCodeTwoThree];ebp = [aSet.vkCodeTwoThree]
                 mov  edx,[edx+KEYMAP.vkCodeTwoThree];edx = [aTo.vkCodeTwoThree]
                 mov  edi,[ebx+ITEMLISTVIEW.idGroup];edi = [ITEMLISTVIEW.idGroup];ebx =  lpChangeCurrentListViewItem

                 ;Проверяем совпадают ли выбраные клавиши в окнах Edit между собой
                 xor  esi,esi
                 cmp  eax,ecx;eax = [aSet.vkCodeOne];ecx = [aTo.vkCodeOne]
              cmovne  esi,ebx
                 cmp  ebp,edx;ebp = [aSet.vkCodeTwoThree];edx = [aTo.vkCodeTwoThree]
              cmovne  esi,ebx
                test  esi,esi
                  jz  .ErrorEnterKey;Совпадают выбраные клавиши

                 xor  esi,esi

                 ;Проверяет совпадения такого типа: LCrtl + LAlt + X = X  в окнах Edit
                 ;Проверяет совпадения такого типа: LCrtl + LAlt + X = LAlt  в окнах Edit
                test  edx,edx;edx = [aTo.vkCodeTwoThree]
                 jnz  @f
                ; cmp  eax,ecx;eax = [aSet.vkCodeOne];ecx = [aTo.vkCodeOne]
             ; cmove  esi,esp
                 cmp  bp,cx;ecx = [aTo.vkCodeOne];ebp = [aSet.vkCodeTwoThree]
               cmove  esi,esp
                 ror  ebp,16;ebp = [aSet.vkCodeTwoThree]
                 cmp  bp,cx;ecx = [aTo.vkCodeOne];ebp = [aSet.vkCodeTwoThree]
               cmove  esi,esp
                 ror  ebp,16;ebp = [aSet.vkCodeTwoThree]
                test  esi,esi
                 jnz  .ErrorEnterKey;Совпадают выбраные клавиши

                 @@:
                 ;Проверяет совпадения такого типа: X = LCrtl + LAlt + X в окнах Edit
                 ;Проверяет совпадения такого типа: LAlt = LCrtl + LAlt + X в окнах Edit
                test  ebp,ebp;ebp = [aSet.vkCodeTwoThree]
                 jnz  .LoopCmpItem
               ;  cmp  eax,ecx;eax = [aSet.vkCodeOne];ecx = [aTo.vkCodeOne]
             ;  cmove  esi,esp
                 cmp  dx,ax;eax = [aSet.vkCodeOne];edx = [aTo.vkCodeTwoThree]
               cmove  esi,esp
                 ror  edx,16;edx = [aTo.vkCodeTwoThree]
                 cmp  dx,ax;eax = [aSet.vkCodeOne];edx = [aTo.vkCodeTwoThree]
               cmove  esi,esp
                 ror  edx,16;edx = [aTo.vkCodeTwoThree]
                test  esi,esi
                 jnz  .ErrorEnterKey;Совпадают выбраные клавиши

                 ;Проверяем есть ли такие же клавиши в двухсвязнам списке итамов
                 ;ebx =  lpChangeCurrentListViewItem;eax = [aSet.vkCodeOne];ebp = [aSet.vkCodeTwoThree];ecx = [aTo.vkCodeOne];edx = [aTo.vkCodeTwoThree];edi = [ITEMLISTVIEW.idGroup]
       .LoopCmpItem:
                 ;Как только попадем опять на текущий выбраный итем прикращаем проверать
                 mov  ebx,[ebx+ITEMLISTVIEW.lpNextItem];ebx =  ITEMLISTVIEW
                 xor  esi,esi
                 cmp  ebx,[lpChangeCurrentListViewItem]
                  je  .EndLoopCmpItem

                 ;Проверяем может это итем пустышка с ....
                 cmp  [ebx+ITEMLISTVIEW.vkCodeItemOne],esi
                  je  .LoopCmpItem

                 ;Если итем в списке из глобальной группы значит дальше проверяем, если итем и наш текущий выбраный из одной группы значит дальше проверяем на сходство
                 cmp  [ebx+ITEMLISTVIEW.idGroup],edi;edi = [ITEMLISTVIEW.idGroup]
               cmove  esi,esp
                 cmp  [ebx+ITEMLISTVIEW.idGroup],LIST_VIEW_GROUP_GLOBAL_Id;LIST_VIEW_GROUP_GLOBAL_Id = 0
               cmove  esi,esp
                test  edi,edi;LIST_VIEW_GROUP_GLOBAL_Id = 0
               cmovz  esi,esp
                test  esi,esi
                  jz  .LoopCmpItem

                 ;Если совпадают vkCodeItemOne и vkCodeItemTwoThree выбраного с текущим в списке значит - недопустимая комбинация ключей
                 cmp  [ebx+ITEMLISTVIEW.vkCodeItemOne],eax;eax = [aSet.vkCodeOne]
              cmovne  esi,ebx
                 cmp  [ebx+ITEMLISTVIEW.vkCodeItemTwoThree],ebp;ebp = [aSet.vkCodeTwoThree]
              cmovne  esi,ebx
                 cmp  esi,esp
                  je  .AlreadyKey;eax = [aSet.vkCodeOne];ebp = [aSet.vkCodeTwoThree]

                 ;Проверяем на цепочку из ключей - т.е. key1 = key2 key2 = key1 такое не должно быть, цепочки из ключей или комбинаций не должно быть
                 cmp  [ebx+ITEMLISTVIEW.vkCodeItemOne],ecx;ecx = [aTo.vkCodeOne]
              cmovne  esi,esp
                 cmp  [ebx+ITEMLISTVIEW.vkCodeItemTwoThree],edx;edx = [aTo.vkCodeTwoThree]
              cmovne  esi,esp
                 cmp  esi,ebx
                  je  .CollisionKey;ecx = [aTo.vkCodeOne];edx = [aTo.vkCodeTwoThree]

                 ;Проверяем на цепочку из ключей - т.е. key1 = key2 key2 = key1 такое не должно быть, цепочки из ключей или комбинаций не должно быть
                 cmp  [ebx+ITEMLISTVIEW.vkCodeSubOne],eax;eax = [aSet.vkCodeOne]
              cmovne  esi,ebx
                 cmp  [ebx+ITEMLISTVIEW.vkCodeSubTwoThree],ebp;ebp = [aSet.vkCodeTwoThree]
              cmovne  esi,ebx
                 cmp  esi,esp
                  je  .CollisionKey2;eax = [aSet.vkCodeOne];ebp = [aSet.vkCodeTwoThree]

                 xor  esi,esi;esi = 0

                 ;Проверяем что бы не было так: в итеме  LCrtl + LAlt + X - добавляемое  LCrtl
                test  ebp,ebp;ebp = [aSet.vkCodeTwoThree]
                 jnz  @f

                 cmp  word[ebx+ITEMLISTVIEW.vkCodeItemTwoThree],ax;eax = [aSet.vkCodeOne]
               cmove  esi,esp
                 cmp  word[ebx+ITEMLISTVIEW.vkCodeItemTwoThree+2],ax;eax = [aSet.vkCodeOne]
               cmove  esi,esp
                test  esi,esi
                 jnz  .AlreadyKey;eax = [aSet.vkCodeOne];ebp = [aSet.vkCodeTwoThree]


                 ;Проверяем что бы не было так: в итеме - LCrtl  добавляемое LCrtl + LAlt + X
                 @@:
                 cmp  [ebx+ITEMLISTVIEW.vkCodeItemTwoThree],esi;esi = 0
                 jne  @f

                 cmp  word[ebx+ITEMLISTVIEW.vkCodeItemOne],bp;ebp = [aSet.vkCodeTwoThree]
               cmove  esi,esp
                 ror  ebp,16;ebp = [aSet.vkCodeTwoThree]
                 cmp  word[ebx+ITEMLISTVIEW.vkCodeItemOne],bp;ebp = [aSet.vkCodeTwoThree]
               cmove  esi,esp
                 ror  ebp,16;ebp = [aSet.vkCodeTwoThree]
                test  esi,esi
                 jnz  .AlreadyKey;eax = [aSet.vkCodeOne];ebp = [aSet.vkCodeTwoThree]

                 ;Проверяем что бы не было так: в субитеме  LCrtl + LAlt + X - добавляемое  LCrtl
                 @@:
                test  edx,edx;edx = [aTo.vkCodeTwoThree]
                 jnz  @f

                 cmp  word[ebx+ITEMLISTVIEW.vkCodeItemTwoThree],cx;ecx = [aTo.vkCodeOne]
               cmove  esi,esp
                 cmp  word[ebx+ITEMLISTVIEW.vkCodeItemTwoThree+2],cx;ecx = [aTo.vkCodeOne]
               cmove  esi,esp
                test  esi,esi
                 jnz  .AlreadyKey2;ecx = [aTo.vkCodeOne];edx = [aTo.vkCodeTwoThree]

                 ;Проверяем что бы не было так: в субитеме - LCrtl  добавляемое LCrtl + LAlt + X
                 @@:
                 cmp  [ebx+ITEMLISTVIEW.vkCodeItemTwoThree],esi;esi = 0
                 jne  .LoopCmpItem

                 cmp  word[ebx+ITEMLISTVIEW.vkCodeItemOne],dx;edx = [aTo.vkCodeTwoThree]
               cmove  esi,esp
                 ror  edx,16;ebp = [aSet.vkCodeTwoThree]
                 cmp  word[ebx+ITEMLISTVIEW.vkCodeItemOne],dx;edx = [aTo.vkCodeTwoThree]
               cmove  esi,esp
                 ror  edx,16;edx = [aTo.vkCodeTwoThree]
                test  esi,esi
                 jnz  .AlreadyKey2;ecx = [aTo.vkCodeOne];edx = [aTo.vkCodeTwoThree]

                 jmp  .LoopCmpItem

   .EndLoopCmpItem:

                 ;Проверяем были ли произведены пользователям изменения в выюоре клавиш или действий по сравнению с изаеняемым итемом, если небыло изминений считаем как отмену это
                 ;ebx =  lpChangeCurrentListViewItem;eax = [aSet.vkCodeOne];ecx = [aTo.vkCodeOne];ebp = [aSet.vkCodeTwoThree];edx = [aTo.vkCodeTwoThree]
                 xor  esi,esi
                 xor  edi,edi

                 cmp  eax,[ebx+ITEMLISTVIEW.vkCodeItemOne];eax = [aSet.vkCodeOne];ebx =  lpChangeCurrentListViewItem
              cmovne  esi,esp
                 cmp  ebp,[ebx+ITEMLISTVIEW.vkCodeItemTwoThree];ebp = [aSet.vkCodeTwoThree];ebx =  lpChangeCurrentListViewItem
              cmovne  esi,esp

                 mov  ebp,[ebx+ITEMLISTVIEW.vkCodeSubOne];ebp = [ITEMLISTVIEW.vkCodeSubOne]
                 mov  [!aTestCmpSet],esi;Если = 0 итем  не был изменен

                 cmp  ecx,ebp;ecx = [aTo.vkCodeOne];ebx =  lpChangeCurrentListViewItem
              cmovne  edi,esp
                 cmp  edx,[ebx+ITEMLISTVIEW.vkCodeSubTwoThree];edx = [aTo.vkCodeTwoThree] ;ebx =  lpChangeCurrentListViewItem
              cmovne  edi,esp

                  or  esi,edi
               cmovz  ebx,edi
                  jz  .Exit;Одинаковы не итем не субитем неизменены = Отмена

                 mov  [!aTestCmpTo],edi;Если = 0 итем  не был изменен, если = -1 изменен но на камбинацию- доп память не нада, = положительное число - размер новой структуры в байтах
                test  edi,edi
                  jz  .Exit

                 xor  edi,edi
                 mov  eax,VK_LAST
                 xor  esi,esi
                 dec  edi

                 cmp  ecx,eax
               cmova  esi,eax
                 cmp  ebp,eax
               cmova  esi,eax
                 mov  [!aTestCmpTo],edi;Если = 0 итем  не был изменен, если = -1 изменен но на камбинацию- доп память не нада, = положительное число - размер новой структуры в байтах
                test  esi,esi
                  jz  .Exit

                 ;Определяем в зависимости от vkCodeOne в aTo сколько надо выделить памяти под структуру
                 mov  edx,aTo
                 xor  edi,edi
                 cmp  ecx,eax
               cmova  edi,[edx+KEYMAP.scanCodeSubTwoThree];Размер на все доп даные ключая строку с нулем
                 add  edi,sizeof.ITEMLISTVIEW
                 mov  [!aTestCmpTo],edi;Если = 0 итем  не был изменен, если = -1 изменен но на камбинацию- доп память не нада, = положительное число - размер новой структуры в байтах


;Закрываем диолог
;ebx = ноль  или ebx =  lpChangeCurrentListViewItem
              .Exit:
                ;Удаляем хуки отвечающие за Edit
              invoke  UnhookWindowsHookEx,[hhookInValue]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              invoke  UnhookWindowsHookEx,[hhookInValueMouse]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 mov  [hhookInValueMouse],0;что бы знать хук работает или нет и при выходе из приложения знать удалять его или нет

              invoke  EndDialog,[!HwndDlg],ebx ;ebx = отменил или нет пользователь свой выбор если  null = отменил
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет



;ebx = отменил или нет пользователь свой выбор если  null = отменил иначе = указатель натекущую изменяемую структуру items
;ebx = ноль  или ebx =  lpChangeCurrentListViewItem
                test  ebx,ebx
                  jz  .True

                 ;Добавить или изменить итем - не важно эти значение что установим тут нужны и впервом и во втором случае
                 lea  edi,[!aAddItem];edi =  !aAddItem
                 lea  edx,[!aBufferString];так как буфер для текста во всех случаях кроме когда действие - одинаковый  поэтому в самом начале заносим в структуру
                 mov  esi,[SendMessageW];esi = [SendMessageW]
                 mov  ebp,[aArrayHwndFont.hwndListView];ebp = [aArrayHwndFont.hwndListView]
                 mov  [edi+LVITEMW.iSubItem],0;edi =  !aAddItem; - индекс того столбика в которрый Item вставляем в нашем случае он всегда = 0 так как в первый вставляем чей индекс = 0
                 mov  [edi+LVITEMW.pszText],edx;edi =  !aAddItem

                 ;Проверяем если = 0 добавляем иначе изменяем итем
                 cmp  [ebx+ITEMLISTVIEW.vkCodeItemOne],0;ebx =  lpChangeCurrentListViewItem
                  je  .AddNewList;ebx =  lpChangeCurrentListViewItem;esi = [SendMessageW];edi =  !aAddItem;ebp = [aArrayHwndFont.hwndListView]


;Будем изменять текущий итем
;ebx =  lpChangeCurrentListViewItem;esi = [SendMessageW];edi =  !aAddItem;ebp = [aArrayHwndFont.hwndListView]

                 mov  ecx,[!aTestCmpTo];ecx = [!aTestCmpTo]

                 ;Общее для изменения итема
                 mov  eax,[iChangeCurrentListViewItemIndex];Индекс итема который будем изменять
                 mov  [edi+LVITEMW.mask],LVIF_TEXT;edi =  !aAddItem
                 mov  [edi+LVITEMW.iItem],eax;edi =  !aAddItem


                 ;Проверяем надо ли изменять обьем используемой памяти в структуре итема
                 cmp  ecx,0;Если = 0 итем  не был изменен, если = -1 изменен но на камбинацию- доп память не нада, = положительное число - размер новой структуры в байтах
                 jle  .NotAddMemory;ebx =  lpChangeCurrentListViewItem;esi = [SendMessageW];edi =  !aAddItem;ebp = [aArrayHwndFont.hwndListView]



;Нужно изменить обьем памяти
;ebx =  lpChangeCurrentListViewItem;esi = [SendMessageW];edi =  !aAddItem;ebp = [aArrayHwndFont.hwndListView]
              ;Выделяем память под измененый размер итема
              invoke  HeapAlloc,[hheap],0,ecx;ecx = [!aTestCmpTo]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              ;Наполняем новую структуру
                 mov  ebx,[ebx+ITEMLISTVIEW.idGroup]; переносим айди группы
                 mov  edx,aSet
                xchg  ebx,eax;ebx = New list item struct ITEMLISTVIEW; eax = айди группы

                 mov  ecx,[edx+KEYMAP.scanCodeSubOne];edx = aSet
                 mov  [ebx+ITEMLISTVIEW.idGroup],eax; eax = айди группы
                 mov  [ebx+ITEMLISTVIEW.stateItem],(IMAGE_LIST_STATE_CHECK_Id+1) shl 12;+1 - так как тут идет индекс  не от нуля а от 1 в отличи от List Image
                 mov  [ebx+ITEMLISTVIEW.scanCodeItemOne],ecx;ebx = New list item struct ITEMLISTVIEW

                 mov  eax,[edx+KEYMAP.scanCodeSubTwoThree];edx = aSet
                 mov  ecx,[edx+KEYMAP.vkCodeOne];edx = aSet
                 mov  edx,[edx+KEYMAP.vkCodeTwoThree];edx = aSet
                 mov  [ebx+ITEMLISTVIEW.scanCodeItemTwoThree],eax ;ebx = New list item struct ITEMLISTVIEW
                 mov  [ebx+ITEMLISTVIEW.vkCodeItemOne],ecx
                 mov  [ebx+ITEMLISTVIEW.vkCodeItemTwoThree],edx ;ebx = New list item struct ITEMLISTVIEW

                 ;Проверяем надо ли менять текст итема
                 cmp  [!aTestCmpSet],0;Если = 0 итем  не был изменен
                  jz  .NotChengeMemoryItem;Текст итема не изменился


             ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
                lea  eax,[!aBufferString]
             stdcall  VkCodeInString,ecx,edx,eax;eax = [!aBufferString];ecx = [vkCode];edx = [vkCode]

             stdcall  esi,ebp,LVM_SETITEMW,0,edi;esi = [SendMessageW];ebp = [aArrayHwndFont.hwndListView];edi = !aChangeItem
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

.NotChengeMemoryItem:
              ;Наполняем новую структуру
              ;ebx = New list item struct ITEMLISTVIEW;ebp = [aArrayHwndFont.hwndListView];edi =  !aAddItem
                 mov  edx,aTo
                 mov  eax,[edx+KEYMAP.vkCodeOne];edx = aTo
                 mov  ecx,[edx+KEYMAP.vkCodeTwoThree];edx = aTo
                 mov  esi,[edx+KEYMAP.scanCodeSubOne];edx = aTo
                 mov  edx,[edx+KEYMAP.scanCodeSubTwoThree];edx = aTo
                 mov  [ebx+ITEMLISTVIEW.vkCodeSubOne],eax;ebx = New list item struct ITEMLISTVIEW
                 mov  [ebx+ITEMLISTVIEW.vkCodeSubTwoThree],ecx;ebx = New list item struct ITEMLISTVIEW
                 mov  [ebx+ITEMLISTVIEW.scanCodeSubOne],esi;ebx = New list item struct ITEMLISTVIEW
                 mov  [ebx+ITEMLISTVIEW.scanCodeSubTwoThree],edx ;ebx = New list item struct ITEMLISTVIEW

                 cmp  eax,VK_LAST
                  ja  .CopyDataChange

              ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
                 lea  esi,[!aBufferString]
             stdcall  VkCodeInString,eax,ecx,esi;esi = [!aBufferString];eax = [vkCode]  ;ecx = [vkCode]
                 jmp  .SetSubItem

          ;Копируем доп даные и ссылку на строку отправляем в названия субитема
    .CopyDataChange: ;ebx = New list item struct ITEMLISTVIEW
                 lea  eax,[ebx+sizeof.ITEMLISTVIEW]
                 lea  esi,[ebx+esi+sizeof.ITEMLISTVIEW];имя субитема
                 mov  [ebx+ITEMLISTVIEW.vkCodeSubTwoThree],eax
             stdcall  MoveMemoryHL,eax,ecx,edx
                 mov  [edi+LVITEMW.pszText],esi

        .SetSubItem:
                 inc  [edi+LVITEMW.iSubItem];LIST_VIEW_COLUM_NEW_Id  - индекс того столбика в которрый SubItem вставляем в нашем случае он всегда = 1 так как 2 только столбика есть
              invoke  SendMessageW,ebp,LVM_SETITEMW,0,edi;ebp = [aArrayHwndFont.hwndListView];edi = !aChangeItem
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


                 ;Вместо старой структуры вставим новую
                 ;ebx = New list item struct ITEMLISTVIEW
                 mov  esi,aRWKeyMap;esi = aRWKeyMap
                 mov  edi,[lpChangeCurrentListViewItem];edi = [lpChangeCurrentListViewItem]
              invoke  AcquireSRWLockExclusive,esi;esi = aRWKeyMap;Так как эти даные могут читаться вторым потоком

                 mov  eax,[edi+ITEMLISTVIEW.lpNextItem];edi = [lpChangeCurrentListViewItem]
                 mov  ecx,[edi+ITEMLISTVIEW.lpPrevItem];edi = [lpChangeCurrentListViewItem]
                 mov  [ebx+ITEMLISTVIEW.lpNextItem],eax;ebx = New list item struct ITEMLISTVIEW
                 mov  [ebx+ITEMLISTVIEW.lpPrevItem],ecx;ebx = New list item struct ITEMLISTVIEW
                 mov  [ecx+ITEMLISTVIEW.lpNextItem],ebx;ebx = New list item struct ITEMLISTVIEW
                 mov  [eax+ITEMLISTVIEW.lpPrevItem],ebx;ebx = New list item struct ITEMLISTVIEW

              invoke  ReleaseSRWLockExclusive,esi;esi = aRWKeyMap;Так как эти даные могут читаться вторым потоком


              ;Освобождаем пямять от ненужной структуры итема которую мы заменили другой
              invoke  HeapFree,[hheap],0,edi;edi = [lpChangeCurrentListViewItem] ,
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 jmp  .True


;Изменять обьем занимаемой памяти не надо
;ebx =  lpChangeCurrentListViewItem;esi = [SendMessageW];edi =  !aAddItem;ebp = [aArrayHwndFont.hwndListView]
      .NotAddMemory:
                 ;Проверяем надо ли менять текст итема
                 cmp  [!aTestCmpSet],0;Если = 0 итем  не был изменен
                  jz  .NotChengeItem;Текст итема не изменился

             ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
                 lea  eax,[!aBufferString]
                 mov  edx,aSet;edx = aSet
             stdcall  VkCodeInString,[edx+KEYMAP.vkCodeOne],[edx+KEYMAP.vkCodeTwoThree],eax;eax = [!aBufferString];edx = aSet

             stdcall  esi,ebp,LVM_SETITEMW,0,edi;esi = [SendMessageW];ebp = [aArrayHwndFont.hwndListView];edi = !aChangeItem
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

     .NotChengeItem:
                 ;Проверяем надо ли менять текст субитема
                 cmp  [!aTestCmpTo],0;Если = 0 итем  не был изменен, если = -1 изменен но на камбинацию- доп память не нада, = положительное число - размер новой структуры в байтах
                  jz  .NotChengeSubItem;Текст субитема не изменился

                 mov  edx,aTo;edx = aTo
                 lea  eax,[!aBufferString]
                 inc  [edi+LVITEMW.iSubItem];LIST_VIEW_COLUM_NEW_Id  - индекс того столбика в которрый SubItem вставляем в нашем случае он всегда = 1 так как 2 только столбика есть

             ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
             stdcall  VkCodeInString,[edx+KEYMAP.vkCodeOne],[edx+KEYMAP.vkCodeTwoThree],eax;eax = [!aBufferString];edx = aTo

             stdcall  esi,ebp,LVM_SETITEMW,0,edi;esi = [SendMessageW];ebp = [aArrayHwndFont.hwndListView];edi = !aChangeItem
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет


  .NotChengeSubItem:
                 ;Так как заменяемые данные в структуре итема не большие поэтому даже не проверяем изменил их пользователь или нет - так во много раз эфективней
                 ;ebx =  lpChangeCurrentListViewItem
                 mov  esi,aRWKeyMap;esi = aRWKeyMap
              invoke  AcquireSRWLockExclusive,esi;esi = aRWKeyMap;Так как эти даные могут читаться вторым потоком

                 ;Изменяем структуру итема
                 mov  edx,aSet;edx = aSet

                 mov  eax,[edx+KEYMAP.scanCodeSubOne];edx = aSet
                 mov  ecx,[edx+KEYMAP.scanCodeSubTwoThree];edx = aSet
                 mov  [ebx+ITEMLISTVIEW.scanCodeItemOne],eax;ebx = New list item struct ITEMLISTVIEW
                 mov  [ebx+ITEMLISTVIEW.scanCodeItemTwoThree],ecx ;ebx = New list item struct ITEMLISTVIEW

                 mov  eax,[edx+KEYMAP.vkCodeOne];edx = aSet
                 mov  ecx,[edx+KEYMAP.vkCodeTwoThree];edx = aSet
                 mov  [ebx+ITEMLISTVIEW.vkCodeItemOne],eax;ebx =  lpChangeCurrentListViewItem
                 mov  [ebx+ITEMLISTVIEW.vkCodeItemTwoThree],ecx;ebx =  lpChangeCurrentListViewItem

                 ;Изменяем структуру итема
                 mov  edx,aTo
                 mov  eax,[edx+KEYMAP.vkCodeOne];edx = aTo
                 mov  ecx,[edx+KEYMAP.vkCodeTwoThree];edx = aTo
                 mov  [ebx+ITEMLISTVIEW.vkCodeSubOne],eax;ebx =  lpChangeCurrentListViewItem
                 mov  [ebx+ITEMLISTVIEW.vkCodeSubTwoThree],ecx;ebx =  lpChangeCurrentListViewItem
                 mov  eax,[edx+KEYMAP.scanCodeSubOne];edx = aTo
                 mov  ecx,[edx+KEYMAP.scanCodeSubTwoThree];edx = aTo
                 mov  [ebx+ITEMLISTVIEW.scanCodeSubOne],eax;ebx =  lpChangeCurrentListViewItem
                 mov  [ebx+ITEMLISTVIEW.scanCodeSubTwoThree],ecx;ebx =  lpChangeCurrentListViewItem

              invoke  ReleaseSRWLockExclusive,esi;esi = aRWKeyMap;Так как эти даные могут читаться вторым потоком

                 jmp  .True




;Будем добавлять новый итем в группу итемав
;ebx =  lpChangeCurrentListViewItem;esi = [SendMessageW];edi =  !aAddItem;ebp = [aArrayHwndFont.hwndListView]
        .AddNewList:
                 ;Определяем в зависимости от vkCodeOne в aTo сколько надо выделить памяти под структуру
                 mov  edx,aTo
                 xor  eax,eax
                 xor  ecx,ecx
                 cmp  [edx+KEYMAP.vkCodeOne],VK_LAST
               cmova  eax,[edx+KEYMAP.scanCodeSubOne];Размер на доп даные требуемые для действия например VK_FILE_OPEN
               cmova  ecx,[edx+KEYMAP.scanCodeSubTwoThree];Размер на все доп даные ключая строку с нулем
                 lea  eax,[eax+ecx+sizeof.ITEMLISTVIEW]

              ;Выделяем память под новый итем
              invoke  HeapAlloc,[hheap],0,eax
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              ;Наполняем новую структуру
                 mov  ebx,[ebx+ITEMLISTVIEW.idGroup]; определяем в какую группу добовлять
                 mov  edx,aSet
                xchg  ebx,eax;ebx = New list item struct ITEMLISTVIEW

                 mov  ecx,[edx+KEYMAP.scanCodeSubOne];edx = aSet
                 mov  [ebx+ITEMLISTVIEW.stateItem],(IMAGE_LIST_STATE_CHECK_Id+1) shl 12;+1 - так как тут идет индекс  не от нуля а от 1 в отличи от List Image
                 mov  [ebx+ITEMLISTVIEW.scanCodeItemOne],ecx;ebx = New list item struct ITEMLISTVIEW
                 mov  ecx,[edx+KEYMAP.scanCodeSubTwoThree];edx = aSet
                 mov  [ebx+ITEMLISTVIEW.idGroup],eax
                 mov  [ebx+ITEMLISTVIEW.scanCodeItemTwoThree],ecx ;ebx = New list item struct ITEMLISTVIEW

                 mov  ecx,[edx+KEYMAP.vkCodeOne]
                 mov  edx,[edx+KEYMAP.vkCodeTwoThree]
                 mov  [ebx+ITEMLISTVIEW.vkCodeItemOne],ecx
                 mov  [ebx+ITEMLISTVIEW.vkCodeItemTwoThree],edx ;ebx = New list item struct ITEMLISTVIEW


                 ;добавляем итем в List View
                 ;edi =  !aAddItem
                 mov  [edi+LVITEMW.iGroupId],eax
                 mov  eax,[iCountIndexItem]
                 mov  [edi+LVITEMW.mask],LVIF_TEXT or LVIF_STATE or LVIF_GROUPID
                 mov  [edi+LVITEMW.iItem],eax
                 mov  [edi+LVITEMW.state],(IMAGE_LIST_STATE_CHECK_Id+1) shl 12;+1 - так как тут идет индекс  не от нуля а от 1 в отличи от List Image
                 mov  [edi+LVITEMW.stateMask],-1
                 lea  eax,[!aBufferString]
                 inc  [iCountIndexItem];контролируем

             ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
             stdcall  VkCodeInString,ecx,edx,eax;eax = [!aBufferString];ecx = [vkCode];edx = [vkCode]

             stdcall  esi,ebp,LVM_INSERTITEMW,0,edi;esi = [SendMessageW] ;ebp = [aArrayHwndFont.hwndListView];edi =  !aAddItem
                      _LastErrorNotSigned;Если не минус возращает функция то ошибки нет


              ;Наполняем новую структуру
              ;ebx = New list item struct ITEMLISTVIEW;ebp = [aArrayHwndFont.hwndListView];edi =  !aAddItem
                 mov  edx,aTo
                 mov  eax,[edx+KEYMAP.vkCodeOne]
                 mov  ecx,[edx+KEYMAP.vkCodeTwoThree]
                 mov  esi,[edx+KEYMAP.scanCodeSubOne]
                 mov  edx,[edx+KEYMAP.scanCodeSubTwoThree]
                 mov  [ebx+ITEMLISTVIEW.vkCodeSubOne],eax
                 mov  [ebx+ITEMLISTVIEW.vkCodeSubTwoThree],ecx
                 mov  [ebx+ITEMLISTVIEW.scanCodeSubOne],esi
                 mov  [ebx+ITEMLISTVIEW.scanCodeSubTwoThree],edx ;ebx = New list item struct ITEMLISTVIEW

                 cmp  eax,VK_LAST
                  ja  .CopyData

              ;Сохраняет все регистры которые использует иподготавливают строку на основании vkCode
                 lea  esi,[!aBufferString]
             stdcall  VkCodeInString,eax,ecx,esi;esi = [!aBufferString];ecx = [vkCode]
                 jmp  .AddSubItem

          ;Копируем доп даные и ссылку на строку отправляем в названия субитема
          .CopyData: ;ebx = New list item struct ITEMLISTVIEW
                 lea  eax,[ebx+sizeof.ITEMLISTVIEW]
                 lea  esi,[ebx+esi+sizeof.ITEMLISTVIEW];имя субитема
                 mov  [ebx+ITEMLISTVIEW.vkCodeSubTwoThree],eax
             stdcall  MoveMemoryHL,eax,ecx,edx
                 mov  [edi+LVITEMW.pszText],esi

        .AddSubItem:
                 ;добавляем субитем в List View
                 mov  [edi+LVITEMW.mask],LVIF_TEXT; or LVIF_STATE
                 inc  [edi+LVITEMW.iSubItem];LIST_VIEW_COLUM_NEW_Id  - индекс того столбика в которрый SubItem вставляем в нашем случае он всегда = 1 так как 2 только столбика есть
              invoke  SendMessageW,ebp,LVM_SETITEMW,0,edi;ebp = [aArrayHwndFont.hwndListView];edi =  !aAddItem
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет



                 mov  esi,aRWKeyMap;esi = aRWKeyMap
                 mov  edi,aListViewItem;edi= aListViewItem
              invoke  AcquireSRWLockExclusive,esi;esi = aRWKeyMap ;Так как эти даные могут читаться вторым потоком

              ;Каректируем структуры итемов с учетам нового итема
              ;ebx = New list item struct ITEMLISTVIEW
                  mov  eax,[edi+ITEMLISTVIEW.lpPrevItem];edi= aListViewItem
                 mov  [ebx+ITEMLISTVIEW.lpNextItem],edi;ebx = New list item struct ITEMLISTVIEW
                 mov  [ebx+ITEMLISTVIEW.lpPrevItem],eax;ebx = New list item struct ITEMLISTVIEW
                 mov  [edi+ITEMLISTVIEW.lpPrevItem],ebx;ebx = New list item struct ITEMLISTVIEW
                 mov  [eax+ITEMLISTVIEW.lpNextItem],ebx;ebx = New list item struct ITEMLISTVIEW

              invoke  ReleaseSRWLockExclusive,esi;esi = aRWKeyMap ;Так как эти даные могут читаться вторым потоком

              .True:
                 xor  eax,eax
                 inc  eax
                 ret


     ;eax = [aSet.vkCodeOne];ebp = [aSet.vkCodeTwoThree]
     .CollisionKey2:
                 mov  ecx,eax
                 mov  edx,ebp

       ;ecx = [aTo.vkCodeOne];edx = [aTo.vkCodeTwoThree]
       .CollisionKey:;key1 = key2 - key2 = key1 такие цепочки не допустимы - иначе возникнет колизия
                 lea  esi,[!aBufferString]
             stdcall  VkCodeInString,ecx,edx,esi;esi = [!aBufferString];ecx = [aSet.vkCodeOne];edx = [aSet.vkCodeTwoThree]

             stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_MESAGE_KEY_Collision
                 jmp  .MessageErrorKey


       ;ecx = [aTo.vkCodeOne];edx = [aTo.vkCodeTwoThree]
       .AlreadyKey2:;Така клавиша уже есть в базе
                 lea  esi,[!aBufferString]
             stdcall  VkCodeInString,ecx,edx,esi;esi = [!aBufferString];eax = [aSet.vkCodeOne];ebp = [aSet.vkCodeTwoThree]

             stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_MESAGE_KEY_Already
                 jmp  .MessageErrorKey


        ;eax = [aSet.vkCodeOne];ebp = [aSet.vkCodeTwoThree]
        .AlreadyKey:;Така клавиша уже есть в базе
                 lea  esi,[!aBufferString]
             stdcall  VkCodeInString,eax,ebp,esi;esi = [!aBufferString];eax = [aSet.vkCodeOne];ebp = [aSet.vkCodeTwoThree]

             stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_MESAGE_KEY_Already
                 jmp  .MessageErrorKey



     .ErrorEnterKey: ;сообщаем пользователю о не допустимой комбинации
                 xor  esi,esi
             stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_MESAGE_KEY_NotCorect


    ;eax = string esi = 0 or esi = lp string
   .MessageErrorKey:
              invoke  MessageBoxExW,[!HwndDlg],eax,esi,MB_OK or MB_ICONINFORMATION,0 ;esi = 0 or esi = lp string
                 jmp  .True
 endp




;Диалог отвечает за все опции в приложении
;--------------------------------------
 proc DlgProcOptions [esi edi ebp ebx],!HwndDlg,!Wmsg,!Wparam,!Lparam
     locals
      !hkey          dd  ?
      !ModuleName    du  MAX_PATH dup(?)
     endl
                 mov  ebp,[!HwndDlg];ebp = Хендл диолога
                 mov  eax,[!Wmsg];eax = сообщение
                 mov  ecx,[!Wparam];ecx = !Wparam
                 mov  edx,[!Lparam];edx = !Lparam
                 xor  ebx,ebx;ebx = ноль


;ebp = Хендл диолога ;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------
                 cmp  eax,WM_COMMAND
                  je  .WM_COMMAND
                 cmp  eax,WM_INITDIALOG
                  je  .WM_INITDIALOG



             .False:
                 xor  eax,eax
                 ret



;Обрабатываем сообщения от Control, Accelerator, Menu
;ebp = Хендл диолога ;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------
        .WM_COMMAND:
                 cmp  ecx,IDOK
                  je  .Exit;ebp = Хендл диолога ;ebx = ноль
                 cmp  ecx,IDCANCEL
                  je  .Exit;ebp = Хендл диолога ;ebx = ноль

                 jmp  .True


;Иницилизируем диалог
;ebp = Хендл диолога ;ebx = ноль
;--------------------------------------
     .WM_INITDIALOG:
              ;Устанавливаем check кнопка или нет
              invoke  CheckDlgButton,ebp,DIALOG_OPTIONS_BUTTON_AVTO_START_Id,[aIniFileData.aIni.iRun];ebp = Хендл диолога
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 jmp  .True


;Закрываем диолог
;ebp = Хендл диолога ;ebx = ноль
;--------------------------------------
              .Exit:
              invoke  EndDialog,ebp,ebx;ebp = Хендл диолога ;ebx = ноль -Код вазврата роли не играет
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              invoke  IsDlgButtonChecked,ebp,DIALOG_OPTIONS_BUTTON_AVTO_START_Id;ebp = Хендл диолога
                 cmp  eax,[aIniFileData.aIni.iRun];Если состояние автозапуска пользователь изменил - тогда каректируем результат
                 jne  .ChengeAvtoStart;ebx = ноль


              .Next:
              .True:
                 xor  eax,eax
                 inc  eax
                 ret


;Создаем или удаляем запись об программе в автозагрузки
   .ChengeAvtoStart: ;ebx = ноль
                 mov  [aIniFileData.aIni.iRun],eax
                test  eax,eax;BST_UNCHECKED = 0
                  jz  .ChengeAvtoStartDelete;не выбрана- удаляем

                 lea  eax,[!hkey]
              invoke  RegCreateKeyExW,HKEY_CURRENT_USER,szSubKeyRun,ebx,ebx,ebx,KEY_SET_VALUE,ebx,eax,ebx ;ebx = ноль
                      _LastErrorNull;Если ноль возращает функция то ошибки нет
                 mov  ebp,[!hkey];ebp = [!hkey]

                 lea  edi,[!ModuleName];edi = !ModuleName
              invoke  GetModuleFileNameW,ebx,edi,sizeof.!ModuleName/2;edi = !ModuleName;ebx = ноль

                 lea  eax,[eax*2+2];Так как получаем длину в символах плюс без учета нуля
              invoke  RegSetValueExW,ebp,szKeyRun,ebx,REG_SZ,edi,eax ;ebp = [!hkey] ;ebx = ноль;edi = !ModuleName
                      _LastErrorNull;Если ноль возращает функция то ошибки нет
                 jmp  .Next

             .ChengeAvtoStartDelete:
                              invoke  RegDeleteKeyValueW,HKEY_CURRENT_USER,szSubKeyRun,szKeyRun
                                      _LastErrorNull;Если ноль возращает функция то ошибки нет
                                 jmp  .Next
 endp


;Диалог отвечает случай отсутствия лицензии
;--------------------------------------
 proc DlgProcNotLicense [esi edi ebp ebx],!HwndDlg,!Wmsg,!Wparam,!Lparam
     locals
      !aRect               RECT
      !aFont               LOGFONTW
      !aBufferstring       du  MAX_PATH dup(?)
     endl

                 mov  ebp,[!HwndDlg];ebp = Хендл диолога
                 mov  eax,[!Wmsg];eax = сообщение
                 mov  ecx,[!Wparam];ecx = !Wparam
                 mov  edx,[!Lparam];edx = !Lparam
                 xor  ebx,ebx;ebx = ноль


;ebp = Хендл диолога ;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------
                 cmp  eax,WM_COMMAND
                  je  .WM_COMMAND
                 cmp  eax,WM_INITDIALOG
                  je  .WM_INITDIALOG
                 cmp  eax,WM_DESTROY
                  je  .WM_DESTROY


             .False:
                 xor  eax,eax
                 ret


        .WM_DESTROY:
              invoke  DeleteObject,[hfontNotLicense]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 jmp  .False


;Обрабатываем сообщения от Control, Accelerator, Menu
;ebp = Хендл диолога ;ebx = ноль;ecx = !Wparam ;edx = !Lparam
;--------------------------------------
        .WM_COMMAND:
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_NOTLICENSE_ONE;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .DIALOG_NOTLICENSE_ONE;ebp = Хендл диолога ;ebx = ноль
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_NOTLICENSE_TWO;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .DIALOG_NOTLICENSE_TWO;ebp = Хендл диолога ;ebx = ноль
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_NOTLICENSE_THREE;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .DIALOG_NOTLICENSE_THREE;ebp = Хендл диолога ;ebx = ноль
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_NOTLICENSE_FOUR;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .DIALOG_NOTLICENSE_FOUR;ebp = Хендл диолога ;ebx = ноль
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_NOTLICENSE_LICENSE;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .DIALOG_NOTLICENSE_LICENSE;ebp = Хендл диолога ;ebx = ноль
                 cmp  ecx,(BN_CLICKED shl 16) or DIALOG_NOTLICENSE_REGINFO;BN_CLICKED - HIWORD, DIALOG_OPTIONS_BUTTON_OK_Id - LOWORD
                  je  .DIALOG_NOTLICENSE_REGINFO;ebp = Хендл диолога ;ebx = ноль
                 jmp  .True


;Откроем сайт с информацие о регистрацией
;ebp = Хендл диолога ;ebx = ноль
.DIALOG_NOTLICENSE_REGINFO:
                 mov  edx,aShellExecute;edx = aShellExecute
                 mov  [edx+SHELLEXECUTEINFOW.lpFile],szInfoRegUrl;edx = aShellExecute
              invoke  ShellExecuteExW,edx;edx = aShellExecute
                 jmp  .True

;Откроем текстовый файл лицензии
;ebp = Хендл диолога ;ebx = ноль
.DIALOG_NOTLICENSE_LICENSE:
                 mov  edx,aShellExecute;edx = aShellExecute
                 mov  [edx+SHELLEXECUTEINFOW.lpFile],szFileLicense;edx = aShellExecute
              invoke  ShellExecuteExW,edx;edx = aShellExecute
                 jmp  .True

;Проверяем нажата ли кнопка в соответствии с номерам, если нет то в ноль код выхода с диалога
;ebp = Хендл диолога ;ebx = ноль
;--------------------------------------
.DIALOG_NOTLICENSE_ONE:
                 cmp  [iNumberNotLicense],0x31
               cmove  ebx,esp
                 jmp  .Exit;ebp = Хендл диолога ;ebx = код выход

;Проверяем нажата ли кнопка в соответствии с номерам, если нет то в ноль код выхода с диалога
;ebp = Хендл диолога ;ebx = ноль
;--------------------------------------
.DIALOG_NOTLICENSE_TWO:
                 cmp  [iNumberNotLicense],0x32
               cmove  ebx,esp
                 jmp  .Exit;ebp = Хендл диолога ;ebx = код выход

;Проверяем нажата ли кнопка в соответствии с номерам, если нет то в ноль код выхода с диалога
;ebp = Хендл диолога ;ebx = ноль
;--------------------------------------
.DIALOG_NOTLICENSE_THREE:
                 cmp  [iNumberNotLicense],0x33
               cmove  ebx,esp
                 jmp  .Exit;ebp = Хендл диолога ;ebx = код выход

;Проверяем нажата ли кнопка в соответствии с номерам, если нет то в ноль код выхода с диалога
;ebp = Хендл диолога ;ebx = ноль
;--------------------------------------
.DIALOG_NOTLICENSE_FOUR:
                 cmp  [iNumberNotLicense],0x34
               cmove  ebx,esp

              ;ebp = Хендл диолога ;ebx = код выхода
              .Exit:
              invoke  EndDialog,ebp,ebx;ebp = Хендл диолога;ebx = код выхода
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 jmp  .True



;Иницилизируем диалог
;ebp = Хендл диолога
;--------------------------------------
     .WM_INITDIALOG:
                 ;Выведем диалог по центру экрана
                 ;ebp = Хендл диолога
                 lea  esi,[!aRect];esi = [!aRect]
              invoke  GetWindowRect,[hwndDeskop],esi;esi = [!aRect]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 mov  ebx,[esi+RECT.right];esi = [!aRect]
                 mov  edi,[esi+RECT.bottom];esi = [!aRect]

              invoke  GetWindowRect,ebp,esi;ebp = Хендл диолога;esi = [!aRect]
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 ;Находим ширину и высоту диалога из егоных координат
                 mov  eax,[esi+RECT.right];esi = [!aRect]
                 mov  ecx,[esi+RECT.bottom];esi = [!aRect]
                 sub  eax,[esi+RECT.left];esi = [!aRect]
                 sub  ecx,[esi+RECT.top];esi = [!aRect]

                 ;Найдем коодинаты центра рабочего стола с учетам ширины и высоты диалога
                 sub  ebx,eax
                 sub  edi,ecx
                 shr  ebx,1
                 shr  edi,1
                 xor  eax,eax;eax = 0

                 ;Установим положение диалога
              invoke  SetWindowPos,ebp,eax,ebx,edi,eax,eax,SWP_NOZORDER or SWP_NOSIZE;ebp = Хендл диолога;eax = 0
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

               ;Получим число случайно от 1 до 4 которое будет указывать на кнопку для нажимания
               ;ebp = Хендл диолога
               rdtsc
                 mov  ebx,4
                 and  eax,3
              cmovnz  ebx,eax;edx = число

               ;Подготовим строку для отображения какую кнопку нажать
               ;ebp = Хендл диолога;ebx = число
             stdcall  LoadResoursIndirectHL,[lpMultiLanguage],RT_STR,STR_DIALOG_NOTLICENSE_RANDOM;eax = указатель на конечные данные

                 add  ebx,0x30;В число в символ юникода
                 lea  ecx,[!aBufferstring]
                 mov  [iNumberNotLicense],ebx

                 @@:
                 mov  dx,[eax]
                 add  eax,2
                 mov  [ecx],dx
                 add  ecx,2
                 cmp  dx,'0'
                 jne  @b

                 mov  [ecx-2],bx

                 @@:
                 mov  dx,[eax]
                 add  eax,2
                 mov  [ecx],dx
                 add  ecx,2
                test  dx,dx
                 jnz  @b

                 ;Подготовим
                 mov  esi,[SendMessageW];esi = [SendMessageW]

              ;Получим шрифт используемый в контролах диалога
              ;ebp = Хендл диолога;esi = [SendMessageW]
              invoke  GetDlgItem,ebp,DIALOG_NOTLICENSE_RANDOM;ebp = Хендл диолога
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

                 mov  edi,eax;edi = hButtomRANDOM

              ;Получим  хендл шрифта
              ;ebp = Хендл диолога;esi = [SendMessageW];edi = hButtomRANDOM
                 lea  ebx,[!aFont];ebx =  LOGFONTW
             stdcall  esi,edi,WM_GETFONT,0,0;esi = [SendMessageW];edi = hButtomRANDOM
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              ;Получим  структуру шрифта
              ;ebp = Хендл диолога;esi = [SendMessageW];edi = hButtomRANDOM;ebx =  LOGFONTW
              invoke  GetObjectW,eax,sizeof.LOGFONTW,ebx;ebx =  LOGFONTW
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              ;На основании полученого шрифта сделаем другой, но уже жирный
              ;ebp = Хендл диолога;esi = [SendMessageW];edi = hButtomRANDOM;ebx =  LOGFONTW
                 mov  [ebx+LOGFONTW.lfWeight],FW_SEMIBOLD;ebx =  LOGFONTW
              invoke  CreateFontIndirectW,ebx;ebx =  LOGFONTW
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет
                 mov  ebx,eax;ebx = [hfontNotLicense]
                 mov  [hfontNotLicense],eax

              ;Установим шрифт в нужные контролы
              ;ebp = Хендл диолога;esi = [SendMessageW];edi = hButtomRANDOM;ebx = [hfontNotLicense]
              invoke  SendDlgItemMessageW,ebp,DIALOG_NOTLICENSE_COPYRIGHT,WM_SETFONT,ebx,0;ebp = Хендл диолога;ebx = [hfontNotLicense]

             stdcall  esi,edi,WM_SETFONT,ebx,0;esi = [SendMessageW];edi = hButtomRANDOM;ebx = [hfontNotLicense]

              ;Установим указывающий какую кнопку нажать
              ;ebp = Хендл диолога;esi = [SendMessageW];edi = hButtomRANDOM
                 lea  edx,[!aBufferstring]
             stdcall  esi,edi,WM_SETTEXT,0,edx;esi = [SendMessageW];edi = hButtomRANDOM
                      _LastErrorNotNull;Если не ноль возращает функция то ошибки нет

              .True:
                 xor  eax,eax
                 inc  eax
                 ret


 endp