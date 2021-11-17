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




 dialogIndirect dialog_notlicense,<equNameApp,' ',equVersionApp,' - Не зарэгістравана'>,0,0,235,100,WS_CAPTION or WS_POPUP or WS_SYSMENU,WS_EX_TOPMOST,0,'Segoe UI',12
  dialogitemIndirect 'STATIC',equCopyright,DIALOG_NOTLICENSE_COPYRIGHT,2,1,233,13,WS_VISIBLE or SS_CENTER
  dialogitemIndirect 'STATIC','Вы маеце права паспрабаваць гэтую ўмоўна-бясплатную праграму на працягу 30 дзён, пасля чаго вы павінны альбо зарэгістраваць, альбо выдаліць яе.',\
                               NULL,2,14,233,41,WS_VISIBLE  or SS_LEFT
  dialogitemIndirect 'BUTTON','Ліцэнзія',DIALOG_NOTLICENSE_LICENSE,10,56,90,17,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','Інфармацыя аб рэгістрацыі',DIALOG_NOTLICENSE_REGINFO,135,56,90,17,WS_VISIBLE or BS_COMMANDLINK
  dialogitemIndirect 'BUTTON','',DIALOG_NOTLICENSE_RANDOM,2,73,231,26,WS_VISIBLE or BS_GROUPBOX or BS_CENTER
  dialogitemIndirect 'BUTTON','1',DIALOG_NOTLICENSE_ONE,10,84,50,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','2',DIALOG_NOTLICENSE_TWO,65,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','3',DIALOG_NOTLICENSE_THREE,120,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','4',DIALOG_NOTLICENSE_FOUR,175,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
 enddialogIndirect


 dialogIndirect dialog_options,'Опцыі',40,40,200,100,WS_CAPTION or WS_POPUP or WS_SYSMENU or WS_CLIPCHILDREN,0,0,'Segoe UI',12;Опции
  dialogitemIndirect 'BUTTON','Будзе запушчана з Windows',DIALOG_OPTIONS_BUTTON_AVTO_START_Id,2,24,180,15,WS_VISIBLE  or BS_AUTOCHECKBOX
  dialogitemIndirect 'BUTTON','Ok',IDOK,90,80,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
 enddialogIndirect

 dialogIndirect dialog_add_keys,'Дадаць новае дзеянне',0,0,400,100,WS_CAPTION or WS_POPUP or WS_SYSMENU or WS_CLIPCHILDREN,0,0,'Segoe UI',12
  dialogitemIndirect 'STATIC','',DIALOG_KEYMAP_STATIC_HELP_Id,1,88,318,12,WS_VISIBLE or SS_SUNKEN or SS_NOPREFIX or SS_CENTER
  dialogitemIndirect 'BUTTON','Ok',IDOK,320,87,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON or WS_GROUP
  dialogitemIndirect 'BUTTON','Адмена',IDCANCEL,360,87,40,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','',DIALOG_KEYMAP_BUTTON_ADD_Id,380,70,20,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON  or BS_CENTER or BS_ICON
  dialogitemIndirect 'EDIT','',DIALOG_KEYMAP_EDIT_SET_Id,1,70,188,13,WS_VISIBLE or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'EDIT','',DIALOG_KEYMAP_EDIT_TO_Id,192,70,188,13,WS_VISIBLE or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'STATIC',<'Клавішы кіравання сілкаваннем і камбінацыя Alt + Ctrl + Del не могуць быць пераназначаны.',13,10,\
                               'Што б праграма магла ўплываць на ўсе праграмы - яе варта запускаць з адміністрацыйнымі правамі.',13,10,\
                               'Камбінацыя можа складацца з адной або двух клавіш мадыфікатараў плюс нармальная клавіша.'>,NULL,1,1,398,55,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','Клавіша, кнопка мышы або камбінацыя:',NULL,1,57,188,13,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','Клавіша, кнопка мышы, камбінацыя або дзеянне:',NULL,192,57,188,13,WS_VISIBLE  or SS_LEFT
 enddialogIndirect

 menuIndirect menus_add_action
index 0
      indexM  MENU_ADD_0LVL_ACTION_Index
      menuitemIndirect '',NULL,MFR_POPUP or MFR_END
         index 0
               indexM  MENU_ADD_1LVL_MANUAL_Index
               menuitemIndirect 'Ўручную',NULL,MFR_POPUP

                 index 0
                       indexM  MENU_ADD_2LVL_MODIF_Index
                       menuitemIndirect "Мадыфікатары",NULL,MFR_POPUP
                               menuitemIndirect "Alt",MENU_ADD_2LVL_MODIF_ALT_Id
                               menuitemIndirect "Ctrl",MENU_ADD_2LVL_MODIF_CTRL_Id
                               menuitemIndirect "Shift",MENU_ADD_2LVL_MODIF_SHIFT_Id,MFR_END

                       indexM  MENU_ADD_2LVL_MOUSE_Index
                       menuitemIndirect "Мыш",NULL,MFR_POPUP  or MFR_END
                               menuitemIndirect "Левая кнопка",MENU_ADD_2LVL_MOUSE_1Id
                               menuitemIndirect "Правая кнопка",MENU_ADD_2LVL_MOUSE_2Id
                               menuitemIndirect "Сярэдняя кнопка",MENU_ADD_2LVL_MOUSE_3Id
                               menuitemIndirect "X1 кнопка",MENU_ADD_2LVL_MOUSE_4Id
                               menuitemIndirect "X2 кнопка",MENU_ADD_2LVL_MOUSE_5Id
                               menuitemIndirect "Кола ўверх",MENU_ADD_2LVL_MOUSE_6Id
                               menuitemIndirect "Кола ўніз",MENU_ADD_2LVL_MOUSE_7Id
                               menuitemIndirect "Кола направа",MENU_ADD_2LVL_MOUSE_8Id
                               menuitemIndirect "Кола налева",MENU_ADD_2LVL_MOUSE_9Id,MFR_END
                 endi


               indexM  MENU_ADD_1LVL_MONITOR_Index
               menuitemIndirect 'Манітор',NULL,MFR_POPUP
                       menuitemIndirect "Выключыць - універсальны",MENU_ADD_2LVL_MONITOR_OffUniv
                       menuitemIndirect "Захавальнік экрана",MENU_ADD_2LVL_MONITOR_ScrSav,MFR_END

              menuitemIndirect 'Блакаваць',MENU_LOCK_KEY_Id
              menuitemIndirect 'Запусціць файл',MENU_LAUNCH_FILE_Id,MFR_END
         endi
endi
 endmenuIndirect




 menuIndirect menus_popup_notify
 index 0
     indexM  MENU_NOTIFU_Id;Индекс для суб меню
     menuitemIndirect '',NULL,MFR_POPUP
             menuitemIndirect "Паказаць",MENU_SHOW_HIDE_Id
             menuitemIndirect "Прыпыніць",MENU_ACTIVATE_Id
             menuseparatorIndirect
             menuitemIndirect 'Выхад',MENU_EXIT_Id,MFR_END

    indexM  MENU_ADDITEM_Id;Индекс для суб меню;Так как все пункты меню имеею подсказки иначе надо добавить доп проверки как в меню с диалогам DlgProcAddKeys
    menuitemIndirect '',NULL,MFR_POPUP or MFR_END
             menuitemIndirect "Дадаць групу",MENU_ADD_GROUP_Id
             menuitemIndirect "Змяніць группу",MENU_CHANGED_GROUP_Id
             menuitemIndirect "Выдаліць группу",MENU_DELETE_GROUP_Id
             menuseparatorIndirect
             menuitemIndirect "Дадаць",MENU_ADD_ITEM_Id
             menuitemIndirect "Змяніць",MENU_CHANGED_ITEM_Id
             menuitemIndirect 'Выдаліць',MENU_DELETE_ITEM_Id,MFR_END
 endi
 endmenuIndirect


;Для обработки сообщения WM_MENUSELECT и заадно id для меню
strIndirect StringTableMenu,\
            MENU_EMPTY,<''>,\;Перечесление id меню;Ноль и минус один не используеться не акселератарах, не в меню и не в дочерних окнах поэтому и начинаем с +1
            MENU_ADD_GROUP_Id,<'Дазваляе абмежаваць перапрызначэння клавіш адным прыкладаннем'>,\
            MENU_CHANGED_GROUP_Id,<'Дазваляе змяніць абмежаванне клавіш'>,\
            MENU_DELETE_GROUP_Id,<'Цалкам выдаліць групу'>,\
            MENU_ADD_ITEM_Id,<'Дазваляе дадаць перапрызначэння: да клавішы, кнопцы мышы або камбінацыі'>,\
            MENU_CHANGED_ITEM_Id,<'Змяняе перапрызначэння: да клавішы, кнопцы мышы або камбінацыі'>,\
            MENU_DELETE_ITEM_Id,<'Выдаляе гэта перапрызначэння'>,\
            MENU_LAUNCH_FILE_Id,<'Назначае клавішы або камбінацыі - запуск прыкладання або адкрыццё файла'>,\
            MENU_LOCK_KEY_Id,<'Блакуе клавішу або камбінацыю'>,\
            MENU_ADD_2LVL_MONITOR_OffUniv,<'Пры любым выкарыстанні клавіятуры або мышы манітор ўключыцца'>,\
            MENU_HELP_LAST_Id,<''>;Последний id меню с подсказками не включая его самого - кантралирует что бы не пригнуть альше чем надо


strIndirect StringTable,\
            STR_EMPTY,<''>,\
            STR_BUTTON_Exit,<'Выхад'>,\
            STR_BUTTON_Options,<'Опцыі'>,\
            STR_COMBOBOX_LANG_Error,<'Абраная мова недаступна.'>,\
            STR_SYSTAB_CONTROL_KeyСhange,<'Перапрызначэння клавіш'>,\
            STR_SYSTAB_CONTROL_About,<'Аб праграме'>,\
            STR_LIST_VIEW_COLUM_ORIGINAL,<'Арыгінальная'>,\
            STR_LIST_VIEW_COLUM_NEW,<'Змененая'>,\
            STR_MESAGE_KEY_NotCorect,<'Гэтая камбінацыя не даступная.'>,\
            STR_MESAGE_KEY_Already,<'Гэтая клавіша ўжо выкарыстоўваецца.'>,\
            STR_MESAGE_KEY_Collision,<'Ланцужкі клавіш не дапускаюцца.'>,\
            STR_WINDOW_NOT_LICENSE,<equNameApp,' ',equVersionApp,' - Не зарэгістравана'>,\
            STR_DIALOG_ADD_KEY_Name,<'Змяніць прызначэнае дзеянне'>,\
            STR_DIALOG_NOTLICENSE_RANDOM,<'Націсніце кнопку № 0, каб запусціць праграму!'>,\
            STR_OPEN_EXE_Filtr,<'Прыкладання',0,'*.exe',0>,\
            STR_OPEN_EXE_LONG_Patch,<'Шлях занадта доўгі'>,\
            STR_OPEN_EXE_Already,<'Гэтая група ўжо існуе'>,\
            STR_MENU_NOTIFY_Show,<'Паказаць'>,\
            STR_MENU_NOTIFY_Hide,<'Схаваць'>,\
            STR_MENU_NOTIFY_Activ,<'Актываваць'>,\
            STR_MENU_NOTIFY_Susp,<'Прыпыніць'>,\
            STR_HELP_Emeil,<'Напісаць ліст у службу тэхнічнай падтрымкі'>,\
            STR_HELP_Registry,<'Перайсці на вэб-сайт праграмы'>,\
            STR_HELP_Manual,<'Дазваляе прызначаць фізічна адсутныя клавішы'>,\
            STR_HELP_Modifiers,<'Каб прызначыць камбінацыю, выберыце адну ці дзве клавішы-мадыфікатары'>,\
            STR_HELP_Mouse,<'Дзеянні звязаныя з мышшу'>,\
            STR_HELP_Monitor,<'Дзеянні звязаныя з маніторам'>,\
            STR_NOTIFY_ICON_Activate,<equNameApp,' ',equVersionApp,' ',13,10,'Актывавана'>,\;ДОЛЖНО БЫТЬ НЕ длинее 64 символов включая ноль
            STR_NOTIFY_ICON_Suspend,<equNameApp,' ',equVersionApp,' ',13,10,'Прыпынена'>,\;ДОЛЖНО БЫТЬ НЕ длинее 64 символов включая ноль
            STR_LAUNCH_FILE_Filtr,<'Любы файл',0,'*.*',0>,\
            STR_HACKING,<'Праграма ўзламаная!!!'>





align 4
       dd equVersionLang;Версия перевода  для проверки на совместимость
CRC32c dd CRC32cLang

CRC32cInMacro  CRC32cLang,$$,CRC32c-$$,0xFFFFFFFF

