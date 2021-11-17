

 ;Перечесление меню специально для диалога DlgProcAddKeys для прижков смещения получаем id нажатого пункта меню минус MENU_TABLE_ADD_Id, тем самым первый элемент получит смещение 0
 enumMenuJmp  MENU_TABLE_ADD_Id,MENU_TABLE_KEY_Id,\;MENU_TABLE_KEY_Id-последний элемент этой таблицы с него начнеться таблица для vkCode и она последняя все остальные надо перед ней
              MENU_ADD_2LVL_MODIF_ALT_Id,DlgProcAddKeys.MENU_ADD_2LVL_MODIF_ALT_Id,\
              MENU_ADD_2LVL_MODIF_CTRL_Id,DlgProcAddKeys.MENU_ADD_2LVL_MODIF_CTRL_Id,\
              MENU_ADD_2LVL_MODIF_SHIFT_Id,DlgProcAddKeys.MENU_ADD_2LVL_MODIF_SHIFT_Id,\
              MENU_ADD_2LVL_MONITOR_ScrSav,DlgProcAddKeys.MENU_ADD_2LVL_MONITOR_ScrSav

;Англиский язык
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




 dialogIndirect dialog_notlicense,<equNameApp,' ',equVersionApp,' - Not registered'>,0,0,235,100,WS_CAPTION or WS_POPUP or WS_SYSMENU,WS_EX_TOPMOST,0,'Segoe UI',12;Опции
  dialogitemIndirect 'STATIC',equCopyright,DIALOG_NOTLICENSE_COPYRIGHT,2,1,233,13,WS_VISIBLE or SS_CENTER
  dialogitemIndirect 'STATIC','You have the right to try this shareware program for 30 days, after which you must either register or delete it.',NULL,2,14,233,41,WS_VISIBLE  or SS_LEFT
  dialogitemIndirect 'BUTTON','License',DIALOG_NOTLICENSE_LICENSE,10,56,90,17,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','Registration information',DIALOG_NOTLICENSE_REGINFO,135,56,90,17,WS_VISIBLE or BS_COMMANDLINK
  dialogitemIndirect 'BUTTON','',DIALOG_NOTLICENSE_RANDOM,2,73,231,26,WS_VISIBLE or BS_GROUPBOX or BS_CENTER
  dialogitemIndirect 'BUTTON','1',DIALOG_NOTLICENSE_ONE,10,84,50,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','2',DIALOG_NOTLICENSE_TWO,65,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','3',DIALOG_NOTLICENSE_THREE,120,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','4',DIALOG_NOTLICENSE_FOUR,175,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
 enddialogIndirect


 dialogIndirect dialog_options,'Options',40,40,200,100,WS_CAPTION or WS_POPUP or WS_SYSMENU or WS_CLIPCHILDREN,0,0,'Segoe UI',12;Опции
  dialogitemIndirect 'BUTTON','Will be run with Windows',DIALOG_OPTIONS_BUTTON_AVTO_START_Id,2,24,180,15,WS_VISIBLE  or BS_AUTOCHECKBOX
  dialogitemIndirect 'BUTTON','Ok',IDOK,90,80,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
 enddialogIndirect

 dialogIndirect dialog_add_keys,'Add new action',0,0,400,100,WS_CAPTION or WS_POPUP or WS_SYSMENU or WS_CLIPCHILDREN,0,0,'Segoe UI',12
  dialogitemIndirect 'STATIC','',DIALOG_KEYMAP_STATIC_HELP_Id,1,88,318,12,WS_VISIBLE or SS_SUNKEN or SS_NOPREFIX or SS_CENTER
  dialogitemIndirect 'BUTTON','Ok',IDOK,320,87,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON or WS_GROUP
  dialogitemIndirect 'BUTTON','Cancel',IDCANCEL,360,87,40,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','',DIALOG_KEYMAP_BUTTON_ADD_Id,380,70,20,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON  or BS_CENTER or BS_ICON
  dialogitemIndirect 'EDIT','',DIALOG_KEYMAP_EDIT_SET_Id,1,70,188,13,WS_VISIBLE or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'EDIT','',DIALOG_KEYMAP_EDIT_TO_Id,192,70,188,13,WS_VISIBLE or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'STATIC',<'The keys for power management and the combination Alt+Ctrl+Del can not be reassigned.',13,10,\
                               'That the program could influence all programs - it should be started with administrative rights.',13,10,\
                               'The combination can consist of one or two modifier keys plus a normal key.'>,NULL,1,1,398,55,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','Key, mouse button or combination:',NULL,1,57,188,13,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','Key, mouse button, combination or action:',NULL,192,57,188,13,WS_VISIBLE  or SS_LEFT
 enddialogIndirect

 menuIndirect menus_add_action
index 0
      indexM  MENU_ADD_0LVL_ACTION_Index
      menuitemIndirect '',NULL,MFR_POPUP or MFR_END
         index 0
               indexM  MENU_ADD_1LVL_MANUAL_Index
               menuitemIndirect 'Manually',NULL,MFR_POPUP

                 index 0
                       indexM  MENU_ADD_2LVL_MODIF_Index
                       menuitemIndirect "Modifiers",NULL,MFR_POPUP
                               menuitemIndirect "Alt",MENU_ADD_2LVL_MODIF_ALT_Id
                               menuitemIndirect "Ctrl",MENU_ADD_2LVL_MODIF_CTRL_Id
                               menuitemIndirect "Shift",MENU_ADD_2LVL_MODIF_SHIFT_Id,MFR_END

                       indexM  MENU_ADD_2LVL_MOUSE_Index
                       menuitemIndirect "Mouse",NULL,MFR_POPUP  or MFR_END
                               menuitemIndirect "Left button",MENU_ADD_2LVL_MOUSE_1Id
                               menuitemIndirect "Right button",MENU_ADD_2LVL_MOUSE_2Id
                               menuitemIndirect "Middle  button",MENU_ADD_2LVL_MOUSE_3Id
                               menuitemIndirect "X1 button",MENU_ADD_2LVL_MOUSE_4Id
                               menuitemIndirect "X2 button",MENU_ADD_2LVL_MOUSE_5Id
                               menuitemIndirect "Wheel Up",MENU_ADD_2LVL_MOUSE_6Id
                               menuitemIndirect "Wheel Down",MENU_ADD_2LVL_MOUSE_7Id
                               menuitemIndirect "Wheel Right",MENU_ADD_2LVL_MOUSE_8Id
                               menuitemIndirect "Wheel Left",MENU_ADD_2LVL_MOUSE_9Id,MFR_END
                 endi


               indexM  MENU_ADD_1LVL_MONITOR_Index
               menuitemIndirect 'Monitor',NULL,MFR_POPUP
                       menuitemIndirect "Turn off - universal",MENU_ADD_2LVL_MONITOR_OffUniv
                       menuitemIndirect "Screensaver",MENU_ADD_2LVL_MONITOR_ScrSav,MFR_END

              menuitemIndirect 'Lock',MENU_LOCK_KEY_Id
              menuitemIndirect 'Launch file',MENU_LAUNCH_FILE_Id,MFR_END
         endi
endi
 endmenuIndirect




 menuIndirect menus_popup_notify
 index 0
     indexM  MENU_NOTIFU_Id;Индекс для суб меню
     menuitemIndirect '',NULL,MFR_POPUP
             menuitemIndirect "Show",MENU_SHOW_HIDE_Id
             menuitemIndirect "Suspend",MENU_ACTIVATE_Id
             menuseparatorIndirect
             menuitemIndirect 'Exit',MENU_EXIT_Id,MFR_END

    indexM  MENU_ADDITEM_Id;Индекс для суб меню;Так как все пункты меню имеею подсказки иначе надо добавить доп проверки как в меню с диалогам DlgProcAddKeys
    menuitemIndirect '',NULL,MFR_POPUP or MFR_END
             menuitemIndirect "Add Group",MENU_ADD_GROUP_Id
             menuitemIndirect "Edit group",MENU_CHANGED_GROUP_Id
             menuitemIndirect "Delete Group",MENU_DELETE_GROUP_Id
             menuseparatorIndirect
             menuitemIndirect "Add",MENU_ADD_ITEM_Id
             menuitemIndirect "Edit",MENU_CHANGED_ITEM_Id
             menuitemIndirect 'Delete',MENU_DELETE_ITEM_Id,MFR_END
 endi
 endmenuIndirect


;Для обработки сообщения WM_MENUSELECT и заадно id для меню
strIndirect StringTableMenu,\
            MENU_EMPTY,<''>,\;Перечесление id меню;Ноль и минус один не используеться не акселератарах, не в меню и не в дочерних окнах поэтому и начинаем с +1
            MENU_ADD_GROUP_Id,<'Allows you to limit the reassignment of keys by one application'>,\
            MENU_CHANGED_GROUP_Id,<'Allows you to change the key limit'>,\
            MENU_DELETE_GROUP_Id,<'Completely removes the group'>,\
            MENU_ADD_ITEM_Id,<'Adds a reassignment to: a key, a mouse button, or a combination'>,\
            MENU_CHANGED_ITEM_Id,<'Changes the reassignment: to a key, mouse button, or combination'>,\
            MENU_DELETE_ITEM_Id,<'Removes this reassignment'>,\
            MENU_LAUNCH_FILE_Id,<'Assigns a key or combination to launch an application or open a file'>,\
            MENU_LOCK_KEY_Id,<'Blocks a key or combination'>,\
            MENU_ADD_2LVL_MONITOR_OffUniv,<'With any use of the keyboard or mouse, the monitor turns on'>,\
            MENU_HELP_LAST_Id,<''>;Последний id меню с подсказками не включая его самого - кантралирует что бы не пригнуть альше чем надо



strIndirect StringTable,\
            STR_EMPTY,<''>,\
            STR_BUTTON_Exit,<'Exit'>,\
            STR_BUTTON_Options,<'Options'>,\
            STR_COMBOBOX_LANG_Error,<'The selected language is not available.'>,\
            STR_SYSTAB_CONTROL_KeyСhange,<'Key reassignment'>,\
            STR_SYSTAB_CONTROL_About,<'About'>,\
            STR_LIST_VIEW_COLUM_ORIGINAL,<'Original to'>,\
            STR_LIST_VIEW_COLUM_NEW,<'Changed to'>,\
            STR_MESAGE_KEY_NotCorect,<'This combination is not available.'>,\
            STR_MESAGE_KEY_Already,<'This key is already in use.'>,\
            STR_MESAGE_KEY_Collision,<'The keys chains are not allowed.'>,\
            STR_WINDOW_NOT_LICENSE,<equNameApp,' ',equVersionApp,' - Not registered'>,\
            STR_DIALOG_ADD_KEY_Name,<'Edit assigned action'>,\
            STR_DIALOG_NOTLICENSE_RANDOM,<'Press button № 0 to start the programm!'>,\
            STR_OPEN_EXE_Filtr,<'Applications',0,'*.exe',0>,\
            STR_OPEN_EXE_LONG_Patch,<'Path is too long'>,\
            STR_OPEN_EXE_Already,<'This group already exists'>,\
            STR_MENU_NOTIFY_Show,<'Show'>,\
            STR_MENU_NOTIFY_Hide,<'Hide'>,\
            STR_MENU_NOTIFY_Activ,<'Activate'>,\
            STR_MENU_NOTIFY_Susp,<'Suspend'>,\
            STR_HELP_Emeil,<'Write a letter to technical support'>,\
            STR_HELP_Registry,<'Go to the program website'>,\
            STR_HELP_Manual,<'Allows you to assign physically missing keys'>,\
            STR_HELP_Modifiers,<'To assign a combination, select one or two modifier keys'>,\
            STR_HELP_Mouse,<'Actions associated with the mouse'>,\
            STR_HELP_Monitor,<'Actions related to monitor'>,\
            STR_NOTIFY_ICON_Activate,<equNameApp,' ',equVersionApp,' ',13,10,'Activated'>,\;ДОЛЖНО БЫТЬ НЕ длинее 64 символов включая ноль
            STR_NOTIFY_ICON_Suspend,<equNameApp,' ',equVersionApp,' ',13,10,'Suspended'>,\;ДОЛЖНО БЫТЬ НЕ длинее 64 символов включая ноль
            STR_LAUNCH_FILE_Filtr,<'Any file',0,'*.*',0>,\
            STR_HACKING,<'The program was hacked !!!'>