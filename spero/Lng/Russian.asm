format  binary as "lng"

include  '..\Include\Win32W.inc'
include  '..\Inc\ConstEquates.inc'

virtual at 0
 Start:
 ;������������ ���� ���������� ��� ������� DlgProcAddKeys ��� ������� �������� �������� id �������� ������ ���� ����� MENU_TABLE_ADD_Id, ��� ����� ������ ������� ������� �������� 0
 enumMenuJmp  MENU_TABLE_ADD_Id,MENU_TABLE_KEY_Id,\;MENU_TABLE_KEY_Id-��������� ������� ���� ������� � ���� ��������� ������� ��� vkCode � ��� ��������� ��� ��������� ���� ����� ���
              MENU_ADD_2LVL_MODIF_ALT_Id,0,\
              MENU_ADD_2LVL_MODIF_CTRL_Id,0,\
              MENU_ADD_2LVL_MODIF_SHIFT_Id,0,\
              MENU_ADD_2LVL_MONITOR_ScrSav,0
end virtual


;������� ����
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




 dialogIndirect dialog_notlicense,<equNameApp,' ',equVersionApp,' - �� ����������������'>,0,0,235,100,WS_CAPTION or WS_POPUP or WS_SYSMENU,WS_EX_TOPMOST,0,'Segoe UI',12
  dialogitemIndirect 'STATIC',equCopyright,DIALOG_NOTLICENSE_COPYRIGHT,2,1,233,13,WS_VISIBLE or SS_CENTER
  dialogitemIndirect 'STATIC','�� ������ ����� ����������� ��� �������-���������� ��������� � ������� 30 ����, ����� ���� �� ������ ���� ����������������, ���� ������� ��.',\
                               NULL,2,14,233,41,WS_VISIBLE  or SS_LEFT
  dialogitemIndirect 'BUTTON','��������',DIALOG_NOTLICENSE_LICENSE,10,56,90,17,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','���������� � �����������',DIALOG_NOTLICENSE_REGINFO,135,56,90,17,WS_VISIBLE or BS_COMMANDLINK
  dialogitemIndirect 'BUTTON','',DIALOG_NOTLICENSE_RANDOM,2,73,231,26,WS_VISIBLE or BS_GROUPBOX or BS_CENTER
  dialogitemIndirect 'BUTTON','1',DIALOG_NOTLICENSE_ONE,10,84,50,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','2',DIALOG_NOTLICENSE_TWO,65,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','3',DIALOG_NOTLICENSE_THREE,120,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','4',DIALOG_NOTLICENSE_FOUR,175,84,50,13,WS_VISIBLE  or BS_PUSHBUTTON
 enddialogIndirect


 dialogIndirect dialog_options,'�����',40,40,200,100,WS_CAPTION or WS_POPUP or WS_SYSMENU or WS_CLIPCHILDREN,0,0,'Segoe UI',12;�����
  dialogitemIndirect 'BUTTON','����� �������� � Windows',DIALOG_OPTIONS_BUTTON_AVTO_START_Id,2,24,180,15,WS_VISIBLE  or BS_AUTOCHECKBOX
  dialogitemIndirect 'BUTTON','Ok',IDOK,90,80,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON
 enddialogIndirect

 dialogIndirect dialog_add_keys,'�������� ����� ��������',0,0,400,100,WS_CAPTION or WS_POPUP or WS_SYSMENU or WS_CLIPCHILDREN,0,0,'Segoe UI',12
  dialogitemIndirect 'STATIC','',DIALOG_KEYMAP_STATIC_HELP_Id,1,88,318,12,WS_VISIBLE or SS_SUNKEN or SS_NOPREFIX or SS_CENTER
  dialogitemIndirect 'BUTTON','Ok',IDOK,320,87,40,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON or WS_GROUP
  dialogitemIndirect 'BUTTON','������',IDCANCEL,360,87,40,13,WS_VISIBLE  or BS_PUSHBUTTON
  dialogitemIndirect 'BUTTON','',DIALOG_KEYMAP_BUTTON_ADD_Id,380,70,20,13,WS_VISIBLE or WS_TABSTOP or BS_PUSHBUTTON  or BS_CENTER or BS_ICON
  dialogitemIndirect 'EDIT','',DIALOG_KEYMAP_EDIT_SET_Id,1,70,188,13,WS_VISIBLE or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'EDIT','',DIALOG_KEYMAP_EDIT_TO_Id,192,70,188,13,WS_VISIBLE or WS_BORDER  or ES_CENTER   or ES_AUTOHSCROLL or ES_AUTOVSCROLL
  dialogitemIndirect 'STATIC',<'������� ���������� �������� � ���������� Alt + Ctrl + Del �� ����� ���� �������������.',13,10,\
                               '��� �� ��������� ����� ������ �� ��� ��������� - �� ������� ��������� � ����������������� �������.',13,10,\
                               '���������� ����� �������� �� ����� ��� ���� ������ ������������� ���� ���������� �������.'>,NULL,1,1,398,55,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','�������, ������ ���� ��� ����������:',NULL,1,57,188,13,WS_VISIBLE or SS_LEFT
  dialogitemIndirect 'STATIC','�������, ������ ����, ���������� ��� ��������:',NULL,192,57,188,13,WS_VISIBLE  or SS_LEFT
 enddialogIndirect

 menuIndirect menus_add_action
index 0
      indexM  MENU_ADD_0LVL_ACTION_Index
      menuitemIndirect '',NULL,MFR_POPUP or MFR_END
         index 0
               indexM  MENU_ADD_1LVL_MANUAL_Index
               menuitemIndirect '�������',NULL,MFR_POPUP

                 index 0
                       indexM  MENU_ADD_2LVL_MODIF_Index
                       menuitemIndirect "������������",NULL,MFR_POPUP
                               menuitemIndirect "Alt",MENU_ADD_2LVL_MODIF_ALT_Id
                               menuitemIndirect "Ctrl",MENU_ADD_2LVL_MODIF_CTRL_Id
                               menuitemIndirect "Shift",MENU_ADD_2LVL_MODIF_SHIFT_Id,MFR_END

                       indexM  MENU_ADD_2LVL_MOUSE_Index
                       menuitemIndirect "����",NULL,MFR_POPUP  or MFR_END
                               menuitemIndirect "����� ������",MENU_ADD_2LVL_MOUSE_1Id
                               menuitemIndirect "������ ������",MENU_ADD_2LVL_MOUSE_2Id
                               menuitemIndirect "������� ������",MENU_ADD_2LVL_MOUSE_3Id
                               menuitemIndirect "X1 ������",MENU_ADD_2LVL_MOUSE_4Id
                               menuitemIndirect "X2 ������",MENU_ADD_2LVL_MOUSE_5Id
                               menuitemIndirect "������ �����",MENU_ADD_2LVL_MOUSE_6Id
                               menuitemIndirect "������ ����",MENU_ADD_2LVL_MOUSE_7Id
                               menuitemIndirect "������ ������",MENU_ADD_2LVL_MOUSE_8Id
                               menuitemIndirect "������ �����",MENU_ADD_2LVL_MOUSE_9Id,MFR_END
                 endi


               indexM  MENU_ADD_1LVL_MONITOR_Index
               menuitemIndirect '�������',NULL,MFR_POPUP
                       menuitemIndirect "��������� - �������������",MENU_ADD_2LVL_MONITOR_OffUniv
                       menuitemIndirect "��������� ������",MENU_ADD_2LVL_MONITOR_ScrSav,MFR_END

              menuitemIndirect '�����������',MENU_LOCK_KEY_Id
              menuitemIndirect '��������� ����',MENU_LAUNCH_FILE_Id,MFR_END
         endi
endi
 endmenuIndirect




 menuIndirect menus_popup_notify
 index 0
     indexM  MENU_NOTIFU_Id;������ ��� ��� ����
     menuitemIndirect '',NULL,MFR_POPUP
             menuitemIndirect "��������",MENU_SHOW_HIDE_Id
             menuitemIndirect "�������������",MENU_ACTIVATE_Id
             menuseparatorIndirect
             menuitemIndirect '�����',MENU_EXIT_Id,MFR_END

    indexM  MENU_ADDITEM_Id;������ ��� ��� ����;��� ��� ��� ������ ���� ����� ��������� ����� ���� �������� ��� �������� ��� � ���� � �������� DlgProcAddKeys
    menuitemIndirect '',NULL,MFR_POPUP or MFR_END
             menuitemIndirect "�������� ������",MENU_ADD_GROUP_Id
             menuitemIndirect "�������� ������",MENU_CHANGED_GROUP_Id
             menuitemIndirect "������� ������",MENU_DELETE_GROUP_Id
             menuseparatorIndirect
             menuitemIndirect "��������",MENU_ADD_ITEM_Id
             menuitemIndirect "��������",MENU_CHANGED_ITEM_Id
             menuitemIndirect '�������',MENU_DELETE_ITEM_Id,MFR_END
 endi
 endmenuIndirect


;��� ��������� ��������� WM_MENUSELECT � ������ id ��� ����
strIndirect StringTableMenu,\
            MENU_EMPTY,<''>,\;������������ id ����;���� � ����� ���� �� ������������� �� �������������, �� � ���� � �� � �������� ����� ������� � �������� � +1
            MENU_ADD_GROUP_Id,<'��������� ���������� �������������� ������ ����� �����������'>,\
            MENU_CHANGED_GROUP_Id,<'��������� �������� ����������� ������'>,\
            MENU_DELETE_GROUP_Id,<'��������� ������� ������'>,\
            MENU_ADD_ITEM_Id,<'��������� �������� ��������������: � �������, ������ ���� ��� ����������'>,\
            MENU_CHANGED_ITEM_Id,<'�������� ��������������: � �������, ������ ���� ��� ����������'>,\
            MENU_DELETE_ITEM_Id,<'������� ��� ��������������'>,\
            MENU_LAUNCH_FILE_Id,<'���������  ������� ��� ���������� ������ ���������� ��� �������� �����'>,\
            MENU_LOCK_KEY_Id,<'��������� ������� ��� ����������'>,\
            MENU_ADD_2LVL_MONITOR_OffUniv,<'��� ����� ������������� ���������� ��� ���� ������� ����������'>,\
            MENU_HELP_LAST_Id,<''>;��������� id ���� � ����������� �� ������� ��� ������ - ������������ ��� �� �� �������� ����� ��� ����


strIndirect StringTable,\
            STR_EMPTY,<''>,\
            STR_BUTTON_Exit,<'�����'>,\
            STR_BUTTON_Options,<'�����'>,\
            STR_COMBOBOX_LANG_Error,<'��������� ���� ����������.'>,\
            STR_SYSTAB_CONTROL_Key�hange,<'�������������� ������'>,\
            STR_SYSTAB_CONTROL_About,<'� ���������'>,\
            STR_LIST_VIEW_COLUM_ORIGINAL,<'������������'>,\
            STR_LIST_VIEW_COLUM_NEW,<'����������'>,\
            STR_MESAGE_KEY_NotCorect,<'��� ���������� �� ��������.'>,\
            STR_MESAGE_KEY_Already,<'��� ������� ��� ������������.'>,\
            STR_MESAGE_KEY_Collision,<'������� ������ �� �����������.'>,\
            STR_WINDOW_NOT_LICENSE,<equNameApp,' ',equVersionApp,' - �� ����������������'>,\
            STR_DIALOG_ADD_KEY_Name,<'�������� ���������� ��������'>,\
            STR_DIALOG_NOTLICENSE_RANDOM,<'������� ������ � 0, ����� ��������� ���������!'>,\
            STR_OPEN_EXE_Filtr,<'����������',0,'*.exe',0>,\
            STR_OPEN_EXE_LONG_Patch,<'���� ������� �������'>,\
            STR_OPEN_EXE_Already,<'��� ������ ��� ����������'>,\
            STR_MENU_NOTIFY_Show,<'��������'>,\
            STR_MENU_NOTIFY_Hide,<'������'>,\
            STR_MENU_NOTIFY_Activ,<'������������'>,\
            STR_MENU_NOTIFY_Susp,<'�������������'>,\
            STR_HELP_Emeil,<'�������� ������ � ������ ����������� ���������'>,\
            STR_HELP_Registry,<'������� �� ���-���� ���������'>,\
            STR_HELP_Manual,<'��������� ��������� ��������� ������������� �������'>,\
            STR_HELP_Modifiers,<'����� ��������� ����������, �������� ���� ��� ��� �������-������������'>,\
            STR_HELP_Mouse,<'�������� ��������� � �����'>,\
            STR_HELP_Monitor,<'�������� ��������� � ���������'>,\
            STR_NOTIFY_ICON_Activate,<equNameApp,' ',equVersionApp,' ',13,10,'������������'>,\;������ ���� �� ������ 64 �������� ������� ����
            STR_NOTIFY_ICON_Suspend,<equNameApp,' ',equVersionApp,' ',13,10,'��������������'>,\;������ ���� �� ������ 64 �������� ������� ����
            STR_LAUNCH_FILE_Filtr,<'����� ����',0,'*.*',0>,\
            STR_HACKING,<'��������� ��������!!!'>





align 4
       dd equVersionLang;������ ��������  ��� �������� �� �������������
CRC32c dd CRC32cLang

CRC32cInMacro  CRC32cLang,$$,CRC32c-$$,0xFFFFFFFF

