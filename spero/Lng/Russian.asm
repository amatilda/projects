format  binary as "lng"

include  '..\Include\Win32W.inc'
include  '..\Inc\ConstEquates.inc'

virtual at 0
 Start:
 ;Перечесление меню специально для диалога DlgProcAddKeys для прижков смещения получаем id нажатого пункта меню минус MENU_TABLE_ADD_Id, тем самым первый элемент получит смещение 0
 enumMenuJmp  MENU_TABLE_ADD_Id,MENU_TABLE_KEY_Id,\;MENU_TABLE_KEY_Id-последний элемент этой таблицы с него начнеться таблица для vkCode и она последняя все остальные надо перед ней
              MENU_ADD_2LVL_MODIF_ALT_Id,0,\
              MENU_ADD_2LVL_MODIF_CTRL_Id,0,\
              MENU_ADD_2LVL_MODIF_SHIFT_Id,0,\
              MENU_ADD_2LVL_MONITOR_ScrSav,0
end virtual


;Русский язык
;----------------------------------------------
dirIndirect tbMultiLanguage,\
            RT_STR,StringTable,\
            RT_STR_MENU,StringTableMenu,\
            RT_MENUS,Menus,\
            RT_DLG,Dialog




resIndirect Dialog,\
            DIALOG_OPTIONS,dialog_options,\
            DIALOG_KEY_MAP,dialog_add_keys,\
            DIALOG_NOTLICENSE,dialog_notlicense


resIndirect Menus,\
            MENUS_POPUP_Notify,menus_popup_notify,\
            MENUS_ADD_ACTION,menus_add_action




 dialogIndirect dialog_notlicense,<equNameApp,' ',equVersionApp,' - Не зарегистрировано'>,0,0,235,100,WS_CAPTION or WS_POPUP or WS_SYSMENU,WS_EX_TOPMOST,0,'Segoe UI',12
  dialogitemIndirect 'STATIC',equCopyright,DIALOG_NOTLICENSE_COPYRIGHT,2,1,233,13,WS_VISIBLE or SS_CENTER
  dialogitemIndirect 'STATIC','Вы имеете право попробовать эту условно-бесплатную программу в течение 30 дней, после чего вы должны либо зарегистрировать, либо удалить ее.',\
                               NULL,2,14,233,41,WS_VISIBLE  or SS_LEFT
  dialogitemIndirect 'BUTTON','Лицензия',DIALOG_NOTLICENSE_LICENSE,10,56,90,17,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','Информация о регистрации',DIALOG_NOTLICENSE_REGINFO,135,56,90,17,WS_VISIBLE or BS_COMMANDLINK
  dialogitemIndirect 'BUTTON','',DIALOG_NOTLICENSE_RANDOM,2,73,231,26,WS_VISIBLE or BS_GROUPBOX or BS_CENTER
  dialogitemIndirect 'BUTTON','1',DIALOG_NOTLICENSE_ONE,10,84,50,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','2',DIALOG_NOTLICENSE_TWO,65,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','3',DIALOG_NOTLICENSE_THREE,120,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','4',DIALOG_NOTLICENSE_FOUR,175,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
 enddialogIndirect


 dialogIndirect dialog_options,'Опции',40,40,200,100,WS_CAPTION or WS_POPUP or WS_SYSMENU or WS_CLIPCHILDREN,0,0,'Segoe UI',12;Опции
  dialogitemIndirect 'BUTTON','Будет запущена с Windows',DIALOG_OPTIONS_BUTTON_AVTO_START_Id,2,24,180,15,WS_VISIBLE  or BS_AUTOCHECKBOX
  dialogitemIndirect 'BUTTON','Ok',IDOK,90,80,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
 enddialogIndirect

 dialogIndirect dialog_add_keys,'Добавить новое действие',0,0,400,100,WS_CAPTION or WS_POPUP or WS_SYSMENU or WS_CLIPCHILDREN,0,0,'Segoe UI',12
  dialogitemIndirect 'STATIC','',DIALOG_KEYMAP_STATIC_HELP_Id,1,88,318,12,WS_VISIBLE or SS_SUNKEN or SS_NOPREFIX or SS_CENTER
  dialogitemIndirect 'BUTTON','Ok',IDOK,320,87,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON or WS_GROUP
  dialogitemIndirect 'BUTTON','Отмена',IDCANCEL,360,87,40,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','',DIALOG_KEYMAP_BUTTON_ADD_Id,380,70,20,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON  or BS_CENTER or BS_ICON
  dialogitemIndirect 'EDIT','',DIALOG_KEYMAP_EDIT_SET_Id,1,70,188,13,WS_VISIBLE or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'EDIT','',DIALOG_KEYMAP_EDIT_TO_Id,192,70,188,13,WS_VISIBLE or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'STATIC',<'Клавиши управления питанием и комбинация Alt + Ctrl + Del не могут быть переназначены.',13,10,\
                               'Что бы программа могла влиять на все программы - ее следует запускать с административными правами.',13,10,\
                               'Комбинация может состоять из одной или двух клавиш модификаторов плюс нормальная клавиша.'>,NULL,1,1,398,55,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','Клавиша, кнопка мыши или комбинация:',NULL,1,57,188,13,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','Клавиша, кнопка мыши, комбинация или действие:',NULL,192,57,188,13,WS_VISIBLE  or SS_LEFT
 enddialogIndirect

 menuIndirect menus_add_action
index 0
      indexM  MENU_ADD_0LVL_ACTION_Index
      menuitemIndirect '',NULL,MFR_POPUP or MFR_END
         index 0
               indexM  MENU_ADD_1LVL_MANUAL_Index
               menuitemIndirect 'Вручную',NULL,MFR_POPUP

                 index 0
                       indexM  MENU_ADD_2LVL_MODIF_Index
                       menuitemIndirect "Модификаторы",NULL,MFR_POPUP
                               menuitemIndirect "Alt",MENU_ADD_2LVL_MODIF_ALT_Id
                               menuitemIndirect "Ctrl",MENU_ADD_2LVL_MODIF_CTRL_Id
                               menuitemIndirect "Shift",MENU_ADD_2LVL_MODIF_SHIFT_Id,MFR_END

                       indexM  MENU_ADD_2LVL_MOUSE_Index
                       menuitemIndirect "Мышь",NULL,MFR_POPUP  or MFR_END
                               menuitemIndirect "Левая кнопка",MENU_ADD_2LVL_MOUSE_1Id
                               menuitemIndirect "Правая кнопка",MENU_ADD_2LVL_MOUSE_2Id
                               menuitemIndirect "Средняя кнопка",MENU_ADD_2LVL_MOUSE_3Id
                               menuitemIndirect "X1 кнопка",MENU_ADD_2LVL_MOUSE_4Id
                               menuitemIndirect "X2 кнопка",MENU_ADD_2LVL_MOUSE_5Id
                               menuitemIndirect "Колесо вверх",MENU_ADD_2LVL_MOUSE_6Id
                               menuitemIndirect "Колесо вниз",MENU_ADD_2LVL_MOUSE_7Id
                               menuitemIndirect "Колесо вправо",MENU_ADD_2LVL_MOUSE_8Id
                               menuitemIndirect "Колесо влево",MENU_ADD_2LVL_MOUSE_9Id,MFR_END
                 endi


               indexM  MENU_ADD_1LVL_MONITOR_Index
               menuitemIndirect 'Монитор',NULL,MFR_POPUP
                       menuitemIndirect "Выключить - универсальный",MENU_ADD_2LVL_MONITOR_OffUniv
                       menuitemIndirect "Хранитель экрана",MENU_ADD_2LVL_MONITOR_ScrSav,MFR_END

              menuitemIndirect 'Блокировать',MENU_LOCK_KEY_Id
              menuitemIndirect 'Запустить файл',MENU_LAUNCH_FILE_Id,MFR_END
         endi
endi
 endmenuIndirect




 menuIndirect menus_popup_notify
 index 0
     indexM  MENU_NOTIFU_Id;Индекс для суб меню
     menuitemIndirect '',NULL,MFR_POPUP
             menuitemIndirect "Показать",MENU_SHOW_HIDE_Id
             menuitemIndirect "Приостановить",MENU_ACTIVATE_Id
             menuseparatorIndirect
             menuitemIndirect 'Выход',MENU_EXIT_Id,MFR_END

    indexM  MENU_ADDITEM_Id;Индекс для суб меню;Так как все пункты меню имеею подсказки иначе надо добавить доп проверки как в меню с диалогам DlgProcAddKeys
    menuitemIndirect '',NULL,MFR_POPUP or MFR_END
             menuitemIndirect "Добавить группу",MENU_ADD_GROUP_Id
             menuitemIndirect "Изменить группу",MENU_CHANGED_GROUP_Id
             menuitemIndirect "Удалить группу",MENU_DELETE_GROUP_Id
             menuseparatorIndirect
             menuitemIndirect "Добавить",MENU_ADD_ITEM_Id
             menuitemIndirect "Изменить",MENU_CHANGED_ITEM_Id
             menuitemIndirect 'Удалить',MENU_DELETE_ITEM_Id,MFR_END
 endi
 endmenuIndirect


;Для обработки сообщения WM_MENUSELECT и заадно id для меню
strIndirect StringTableMenu,\
            MENU_EMPTY,<''>,\;Перечесление id меню;Ноль и минус один не используеться не акселератарах, не в меню и не в дочерних окнах поэтому и начинаем с +1
            MENU_ADD_GROUP_Id,<'Позволяет ограничить переназначение клавиш одним приложением'>,\
            MENU_CHANGED_GROUP_Id,<'Позволяет изменить ограничение клавиш'>,\
            MENU_DELETE_GROUP_Id,<'Полностью удалить группу'>,\
            MENU_ADD_ITEM_Id,<'Позволяет добавить переназначение: к клавише, кнопке мыши или комбинации'>,\
            MENU_CHANGED_ITEM_Id,<'Изменяет переназначение: к клавише, кнопке мыши или комбинации'>,\
            MENU_DELETE_ITEM_Id,<'Удаляет это переназначение'>,\
            MENU_LAUNCH_FILE_Id,<'Назначает  клавише или комбинации запуск приложения или открытие файла'>,\
            MENU_LOCK_KEY_Id,<'Блокирует клавишу или комбинацию'>,\
            MENU_ADD_2LVL_MONITOR_OffUniv,<'При любом использовании клавиатуры или мыши монитор включиться'>,\
            MENU_HELP_LAST_Id,<''>;Последний id меню с подсказками не включая его самого - кантралирует что бы не пригнуть альше чем надо


strIndirect StringTable,\
            STR_EMPTY,<''>,\
            STR_BUTTON_Exit,<'Выход'>,\
            STR_BUTTON_Options,<'Опции'>,\
            STR_COMBOBOX_LANG_Error,<'Выбранный язык недоступен.'>,\
            STR_SYSTAB_CONTROL_KeyСhange,<'Переназначение клавиш'>,\
            STR_SYSTAB_CONTROL_About,<'О программе'>,\
            STR_LIST_VIEW_COLUM_ORIGINAL,<'Оригинальная'>,\
            STR_LIST_VIEW_COLUM_NEW,<'Измененная'>,\
            STR_MESAGE_KEY_NotCorect,<'Эта комбинация не доступна.'>,\
            STR_MESAGE_KEY_Already,<'Эта клавиша уже используется.'>,\
            STR_MESAGE_KEY_Collision,<'Цепочки клавиш не допускаются.'>,\
            STR_WINDOW_NOT_LICENSE,<equNameApp,' ',equVersionApp,' - Не зарегистрировано'>,\
            STR_DIALOG_ADD_KEY_Name,<'Изменить назначеное действие'>,\
            STR_DIALOG_NOTLICENSE_RANDOM,<'Нажмите кнопку № 0, чтобы запустить программу!'>,\
            STR_OPEN_EXE_Filtr,<'Приложения',0,'*.exe',0>,\
            STR_OPEN_EXE_LONG_Patch,<'Путь слишком длинный'>,\
            STR_OPEN_EXE_Already,<'Эта группа уже существует'>,\
            STR_MENU_NOTIFY_Show,<'Показать'>,\
            STR_MENU_NOTIFY_Hide,<'Скрыть'>,\
            STR_MENU_NOTIFY_Activ,<'Активировать'>,\
            STR_MENU_NOTIFY_Susp,<'Приостановить'>,\
            STR_HELP_Emeil,<'Написать письмо в службу технической поддержки'>,\
            STR_HELP_Registry,<'Перейти на веб-сайт программы'>,\
            STR_HELP_Manual,<'Позволяет назначать физически отсутствующие клавиши'>,\
            STR_HELP_Modifiers,<'Чтобы назначить комбинацию, выберите одну или две клавиши-модификаторы'>,\
            STR_HELP_Mouse,<'Действия связанные с мышью'>,\
            STR_HELP_Monitor,<'Действия связанные с мониторам'>,\
            STR_NOTIFY_ICON_Activate,<equNameApp,' ',equVersionApp,' ',13,10,'Активирована'>,\;ДОЛЖНО БЫТЬ НЕ длинее 64 символов включая ноль
            STR_NOTIFY_ICON_Suspend,<equNameApp,' ',equVersionApp,' ',13,10,'Приостановлена'>,\;ДОЛЖНО БЫТЬ НЕ длинее 64 символов включая ноль
            STR_LAUNCH_FILE_Filtr,<'Любой файл',0,'*.*',0>,\
            STR_HACKING,<'Программа взломана!!!'>





align 4
       dd equVersionLang;Версия перевода  для проверки на совместимость
CRC32c dd CRC32cLang

CRC32cInMacro  CRC32cLang,$$,CRC32c-$$,0xFFFFFFFF

