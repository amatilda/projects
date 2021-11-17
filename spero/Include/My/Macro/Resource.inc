
; Macroinstructions for making resource section
;----------------------------------------------
macro PolinomCRC32v countTabl,val;val = полином для таблицы
{ common
  local loc,arg,locmini,temp,big
  big = 0

 while  big  < countTabl
  loc = 0 ; Расчитать значения для всех 256 байт
  big = big + 1
  while  loc < 256;большой цикл для crc32 = 8 * 32
   locmini = 0
   arg     = loc

   while  locmini < 8 * big; малый цикл
    temp = arg
    arg  = arg shr 1
    if (temp and 1) <> 0; Проверить четность байта
      arg = arg xor val; XOR выполняется только если байт нечетный
    end if
    locmini = locmini + 1
   end while ; Следующий бит

   dd arg  ; Записать полученный dword в таблицу
   loc = loc + 1
  end while;Следующий байт
 end while }


macro CRC32cInMacro  name,!Source,!Length,!FirstStep
{ common
  local x,y,z,count
   count = 0
   y = !FirstStep

  repeat  !Length
   load x byte from !Source + count
    x = (x xor y) and 0xFF
    y = y shr 8
   virtual at 0
    PolinomCRC32v 1,0x82F63B78
    load z dword from x*4
   end virtual
    y = y xor z
    count = count + 1
  end repeat
 name uniq not y }


; Авторские ресурсы
;----------------------------------------------
;Структура
;База - типы ресурсов
;ipResId dd - указатель на id ресурса относительно базы, пишуться попорядку так что база на множеная Id типа - так находиться ресурсы
;Ресурсы идут
;iData dd - размер данных или еще что то в зависимости от типа ресурса
;ipResData dd - указатель на сами данные относительно ресурса
;Id типа ресурса устанавливаеться само по схеме количество типа ресурсав множенное на 4 - начиная с нуля в случае, id ресурса еще множиться на 2 дополнительно.

macro dirIndirect cat,[id,label];Корнеевой каталог
 { common
    local count
    count = 0
    align 4
   dirIndirect@M:;- база
    cat:;- база
   forward
    id uniq count
    count = count + 1
    dd label - cat  }


; dd - размер данных или еще что зависит от данных
; dd - указатель на массив данных
 macro resIndirect  dir,[id,label];Сохраняет массивы данных
 { common
    local count,loc
    count = 0
    align 4
    dir:;- начала ресурса
    if dirIndirect@M > dir
      display 'Resources hosted incorrectly',0xD,0xA
      err@directoryM;Следующие все даные должны быть ниже корневой папки,а не выше
    end if
   forward
     dd  size.#label
     dd  label - dir
     id uniq count
    count = count + 1 }



; dd - длина строки в символах  без завершающего нуля
; dd - указатель на строку относительно данно ресурса
 macro strIndirect  dir,[id,sting];Сохраняет строки Юникода с оканчательным нулем но в размер ноль не идет
 { common
    local count,loc
    count = 0
    align 4
    dir:;- начала ресурса
    if dirIndirect@M > dir
      display 'Resources hosted incorrectly',0xD,0xA
      err@directoryM;Следующие все даные должны быть ниже корневой папки,а не выше
    end if
   forward
    count = count + 1
   common
    dd count*2 dup(0)
    count = 0
   forward
    loc = $
    id uniq count
    store dword loc - dir at dir + 4 + id*4*2; - указатель на сами данные относительно ресурса
    du sting;строка Юникод
    store dword ($ - loc)/2 at dir +  id*4*2;Размер строки в байтак без нуля
    du 0;Завершающий ноль
    count = count + 1}



macro menuIndirect label
 { common
    align 4
    label dw 1,4,0,0

macro menuitemIndirect string,id,resinfo,status,type
 \{ dd MFT_STRING or type+0,status+0,id
   dw resinfo+0
   du string,0
   align 4
   if ~ resinfo eq
    if resinfo and MFR_POPUP
     dd 0
    end if
   end if  \}

macro menuseparatorIndirect resinfo
\{ dd MFT_SEPARATOR,0,0
   dw resinfo+0,0 \}

macro menuIndirect@Finish@M
\{ common
    size.#label  uniq  $ - label \} }


macro endmenuIndirect
 { common
    menuIndirect@Finish@M
    purge  menuitemIndirect,menuseparatorIndirect,menuIndirect@Finish@M  }


macro dialogIndirect label,title,x,y,cx,cy,style,exstyle,menu,fontname,fontsize
 { local items
  label dd style or DS_SETFONT,exstyle +0
   dw items,x,y,cx,cy
   if menu+0 <> 0
    dw 0FFFFh
   end if
   du menu+0,0,title,0
   if fontname eq
    du 8,'MS Sans Serif',0
   else
    du fontsize+0,fontname,0
   end if
   align 4
   dialog_items equ items = dialog_items_counter
   dialog_items_counter = 0

    macro dialogIndirect@Finish@M
    \{common
      size.#label  uniq  $ - label \}  }




macro dialogitemIndirect class,title,id,x,y,cx,cy,style,exstyle
 { dd style or WS_CHILD,exstyle +0
   dw x,y,cx,cy,id
   if class eq 'BUTTON'
    dw 0FFFFh,80h
   else if class eq 'EDIT'
    dw 0FFFFh,81h
   else if class eq 'STATIC'
    dw 0FFFFh,82h
   else if class eq 'LISTBOX'
    dw 0FFFFh,83h
   else if class eq 'SCROLLBAR'
    dw 0FFFFh,84h
   else if class eq 'COMBOBOX'
    dw 0FFFFh,85h
   else
    du class,0
   end if
   if title eqtype 0
    dw 0FFFFh,title
   else
    du title,0
   end if
   dw 0
   align 4
   dialog_items_counter = dialog_items_counter + 1 }

macro enddialogIndirect
 { dialog_items
   dialogIndirect@Finish@M
   purge dialogIndirect@Finish@M}


;Структура ресурсов
;----------------------------------------------
;Типы ресурсов - первый уровень например -RT_VERSION,RT_MANIFEST
;Такой вид структура имеет:
;Characteristics        dd - должно быть ноль
;Time/Date Stamp        dd - время создания ресурсав может быть ноль
;Major Version          dw - версия ресурсов по факту всегда ставиться в ноль
;Minor Version          dw - версия ресурсов по факту всегда ставиться в ноль
;Number of Name Entries dw - количество строк используемых для индефикации следующих каталогов - по факту в ноль ставиться так как используеться по всеместно id для индефикации ресурсов
;Number of ID Entries   dw   количество структур для индефикации следующих каталогов
;следом идут пара: id и указатель на следующие каталоги или даные в количестве указаном в Number of ID Entries
;Integer ID          dd  - тут указываеться тип ресурса т.е например  RT_VERSION
;Subdirectory Offset dd  - офсет следуещего каталога относительно самого начала нашай структуры ресурсов, если указатель не на даные то старший бит ставиться в 1 т.е. 80000000h  or офсет
;Второй уровень - тут идут id - всех используемых ресурсов одного типа
;Characteristics        dd - должно быть ноль
;Time/Date Stamp        dd - время создания ресурсав может быть ноль
;Major Version          dw - версия ресурсов по факту всегда ставиться в ноль
;Minor Version          dw - версия ресурсов по факту всегда ставиться в ноль
;Number of Name Entries dw - количество строк используемых для индефикации следующих каталогов - по факту в ноль ставиться так как используеться по всеместно id для индефикации ресурсов
;Number of ID Entries   dw   количество структур для индефикации следующих каталогов
;следом идут пара: id и указатель на следующие каталоги или даные в количестве указаном в Number of ID Entries
;Integer ID          dd  - тут указываеться id ресурса который сам уже пользователь придумывает  например  - IDR_ICON_GROUP
;Subdirectory Offset dd  - офсет следуещего каталога относительно самого начала нашай структуры ресурсов, если указатель не на даные то старший бит ставиться в 1 т.е. 80000000h  or офсет
;Третий уровень последний дальше идут данные  - тут идут языкавые индификаторы - все языка привязаные к одному id- обычно один языкавой индификатор, но можно много
;Characteristics        dd - должно быть ноль
;Time/Date Stamp        dd - время создания ресурсав может быть ноль
;Major Version          dw - версия ресурсов по факту всегда ставиться в ноль
;Minor Version          dw - версия ресурсов по факту всегда ставиться в ноль
;Number of Name Entries dw - количество строк используемых для индефикации следующих каталогов - по факту в ноль ставиться так как используеться по всеместно id для индефикации ресурсов
;Number of ID Entries   dw   количество структур для индефикации следующих каталогов
;следом идут пара: id и указатель на следующие каталоги или даные в количестве указаном в Number of ID Entries
;Integer ID          dd  - тут указываеться языкавой индефекатор например LANG_NEUTRAL
;Subdirectory Offset dd  - офсет данных относительно самого начала нашай структуры ресурсов
macro directory [type,label]
 { common
    local max,count
    count = 0
    max = 0
   forward
    if directory@res@M > label
      display 'Resources hosted incorrectly',0xD,0xA
      err@directoryM;Следующие все даные должны быть ниже корневой папки,а не выше
    end if
    count = count + 1
    if type > max
     max = type
    end if
   common
    align 4
    directory@res@M dd 0,%t;Главная корневая папка
                    dw 0,0,0,count;указываем количество типов ресурсов
    repeat max
   forward
    if % = type
     dd type,80000000h + label-directory@res@M;перечесляем типы с адресами относительно корневой папки
    end if
   common
    end repeat }

macro resource dir,[id,lang,label]
 { common
    local count
    count = 0
    directory@res@M@List equ
   forward
    directory@res@M@List equ directory@res@M@List,id <lang> label
    if dir > label
      display 'Resources hosted incorrectly',0xD,0xA
      err@directoryM;Следующие все даные должны быть ниже корневой папки,а не выше
    end if
    if ~ id eq Anonymous
      directory@res@M#id = -1
    end if
   forward
    if ~ id eq Anonymous
      directory@res@M#id = directory@res@M#id + 1
      count = count + 1 -  directory@res@M#id
      if directory@res@M#id = 0
        id uniq count
      end if
     else
      local anonym
      count = count + 1
      anonym uniq count
    end if
   common
    align 4
    dir dd 0,%t
        dw 0,0,0,count;количество id которые содержит каждый тип ресурсов
   forward;перечесляем id с адресами относительно корневой папки
    if id eq Anonymous;анонимный id для удобства макрос присваивает каждому свой числовой по порядку
      dd anonym,80000000h + directory@res@M#anonym - directory@res@M
     else if directory@res@M#id = 0;если у id только один язык то тут все обрабатываеться
       dd id,80000000h + directory@res@M@Table#id - directory@res@M
     else if directory@res@M#id > 0;если у id более чем один язык то тут все обрабатываеться
       directory@res@M#id = - directory@res@M#id
       dd id,80000000h  + directory@res@M@Table#id - directory@res@M
    end if
   forward
    if id eq Anonymous
directory@res@M#anonym dd 0,%t
                       dw 0,0,0,1
                       dd lang,label-directory@res@M;у анономных всегда один язык то  количество языка у одного только один
          label#.resid uniq anonym
     else if directory@res@M#id = 0; если один язык то тут обрабатываеться
directory@res@M@Table#id dd 0,%t
                      dw 0,0,0,1
                      dd lang,label-directory@res@M
          label#.resid uniq id
     else if directory@res@M#id < 0
       count =  (-directory@res@M#id) + 1
directory@res@M@Table#id dd 0,%t
                      dw 0,0,0,count; если много языков то тут обрабатываеться


       match =,arg0, directory@res@M@List
       \{ common
           directory@res@Massiv@M2   count,id,arg0 \}
       label#.resid uniq id
       directory@res@M#id = - directory@res@M#id
     end if

   forward
    restore directory@res@M@List
   common
    restore directory@res@M@List }

       ; displayM 'fhgj',count


macro directory@res@Massiv@M2 count,compare,[arg]
{ common
   local max,probe,loc
   loc = $
   max = 0
   probe = 0
  forward
   match id <lang> label, arg
   \{ common
      if compare eq id & lang > max
        max = lang
      end if
      if compare eq id & lang = 0; lang = LANG_NEUTRAL то тут обрабатываеться, так как при 0 repeat не работает
        dd lang,label-directory@res@M
        if probe = 1
          display 'In this ',\`id,' to match ',\`lang,0xD,0xA
          err@directory@res@Massiv@M2;если при одном id есть два одинаковых lang
        end if
        probe = 1
      end if \}

  common
   if max = 0
     display 'In this ',\`id,' to match ',\`lang,0xD,0xA
     err@directory@res@Massiv@M2;если при одном id есть два одинаковых lang
   end if
   probe = 0
   repeat max
    forward
   match id <lang> label, arg
   \{ common
      if compare eq id & % = lang
        if probe = %
          display 'In this ',\`id,' to match ',\`lang,0xD,0xA
          err@directory@res@Massiv@M2;если при одном id есть два одинаковых lang
        end if
        probe = %
        dd lang,label-directory@res@M
      end if \}

  common
   end repeat
   if count*8 <> $ - loc
     display 'Error: my macro.',0xD,0xA
     err@directory@res@Massiv@M2
   end if }

;----------------------------------------------
macro MessageTableM label,[args];нужно пересмотреть
 { common
   resdata label
   local  count@Blocks,lp@Blocks,min,max,count,bool,duplicate
   count        = 0
   min          = 4294967295
   max          = 0
   dd count@Blocks
   lp@Blocks = $
   dd count@Blocks*3 dup(0x30)

   irp arg0, args \{ if (arg0 eqtype 1)
                      if max < arg0
                        max = arg0
                      end if
                      if min > arg0
                        min = arg0
                      end if
                     end if  \}


  while lp@Blocks <> 0
   store dword min at lp@Blocks
   store dword $ - (lp@Blocks-4-count*12) at lp@Blocks + 8

   while lp@Blocks <> 0
    duplicate = 0
    bool      = 0
    irp arg0, args \{ if bool = 1
                        bool = $
                        dw 0,0
                        arg0,0
                        store word $ - bool at bool
                        match arg1 arg2, arg0 \\{ if (arg1 eq du)
                                                    store word 1 at bool+2
                                                  end if  \\}
                        bool = 2
                      end if
                      if (arg0 eqtype 1) & (arg0 = min)
                        duplicate = duplicate + 1
                        bool = 1
                      end if  \}
    if duplicate > 1
      displayM 'Duplicate id:',min
      error@
    end if

    if bool <> 2
      break
    end if

    min = min + 1
   end while

   store dword min-1 at lp@Blocks + 4
   count = count + 1
   lp@Blocks = lp@Blocks + 12

   if min > max
     break
    else
     bool = max
     irp arg0, args \{ if (arg0 eqtype 1) & (arg0 > min) & (bool > arg0)
                         bool = arg0
                       end if  \}
     min = bool
   end if
  end while

   count@Blocks = count
   endres
   }

 macro string label,[id,str];не доделанно идет пачками - то есть на один id  идет сколько то строк в пачке например при id 7  номер строк будуте уже начинаться на 100 и более ...
 {
  common
   local ids,labels,count,idss
   ids = 0
   count = 0
   resdata label

  forward
   idss = id mod 16
   if idss-ids > 1 & count > 0
     dw idss-ids-1 dup(0)
    else if idss > 0 & count = 0
     dw idss dup(0)
   end if
   ids = idss
   count = count+1
   dw 0
   labels = $
   du str
   store word ($-labels)/2 at labels-2

  common
   endres
   }


;FASM
;----------------------------------------------

macro bitmap label,bitmap_file
 { local data,size
   label dd RVA data,size,0,0
   data file bitmap_file:0Eh
   size = $ - data
   align 4 }

macro icon group,[label,icon_file]
 { common local count
    count = 0
   forward local data,size,position
    label dd RVA data,size,0,0
    virtual at 0
     file icon_file:6,16
     load size dword from 8
     load position dword from 12
    end virtual
    data file icon_file:position,size
    count = count+1
   common local header
    align 4
    group dd RVA header,6+count*14,0,0
    header dw 0,1,count
   forward
    file icon_file:6,12
    dw label#.resid
   common
    align 4 }

macro cursor group,[label,cursor_file]
 { common local count
    count = 0
   forward local data,width,height,size,position
    label dd RVA data,size+4,0,0
    virtual at 0
     file cursor_file:6,16
     load width byte from 0
     load height byte from 1
     load size dword from 8
     load position dword from 12
    end virtual
    data file cursor_file:10,4
         file cursor_file:position,size
    count = count+1
   common local header
    align 4
    group dd RVA header,6+count*14,0,0
    header dw 0,2,count
   forward
    dw width,height,1,0
    dd size+4
    dw label#.resid
   common
    align 4 }

macro menu label
 { local data,size
   label dd RVA data,size,0,0
   data dw 1,4,0,0
   menu_size equ size = $ - data
   menu_level = 1 }

macro menuitem string,id,resinfo,status,type
 { dd MFT_STRING or type+0,status+0,id
   dw resinfo+0
   du string,0
   align 4
   if ~ resinfo eq
    if resinfo and MFR_END
     menu_level = menu_level - 1
    end if
    if resinfo and MFR_POPUP
     menu_level = menu_level + 1
     dd 0
    end if
   end if
   if menu_level = 0
    menu_size
   end if }

macro menuseparator resinfo
 { dd MFT_SEPARATOR,0,0
   dw resinfo+0,0
   if ~ resinfo eq
    if resinfo and MFR_END
     menu_level = menu_level - 1
    end if
   end if
   if menu_level = 0
    menu_size
   end if }

macro dialog label,title,x,y,cx,cy,style,exstyle,menu,fontname,fontsize
 { local data,size,items
   label dd RVA data,size,0,0
   data dd style or DS_SETFONT,exstyle +0
   dw items,x,y,cx,cy
   if menu+0 <> 0
    dw 0FFFFh
   end if
   du menu+0,0,title,0
   if fontname eq
    du 8,'MS Sans Serif',0
   else
    du fontsize+0,fontname,0
   end if
   align 4
   dialog_size equ size = $ - data
   dialog_items equ items = dialog_items_counter
   dialog_items_counter = 0 }




macro dialogitem class,title,id,x,y,cx,cy,style,exstyle
 { dd style or WS_CHILD,exstyle +0
   dw x,y,cx,cy,id
   if class eq 'BUTTON'
    dw 0FFFFh,80h
   else if class eq 'EDIT'
    dw 0FFFFh,81h
   else if class eq 'STATIC'
    dw 0FFFFh,82h
   else if class eq 'LISTBOX'
    dw 0FFFFh,83h
   else if class eq 'SCROLLBAR'
    dw 0FFFFh,84h
   else if class eq 'COMBOBOX'
    dw 0FFFFh,85h
   else
    du class,0
   end if
   if title eqtype 0
    dw 0FFFFh,title
   else
    du title,0
   end if
   dw 0
   align 4
   dialog_items_counter = dialog_items_counter + 1 }

macro enddialog
 { dialog_items
   dialog_size }

macro accelerator label,[fvirt,key,cmd]
 { common
    local data,size
    label dd RVA data,size,0,0
    data:
    accel_count = 0
   forward
    accel_count = accel_count + 1
   common
    size = accel_count * 8
   forward
    accel_count = accel_count - 1
    if accel_count = 0
     dw fvirt or 80h,key
    else
     dw fvirt,key
    end if
    dd cmd }

macro versioninfo label,fileos,filetype,filesubtype,lang,cp,[name,value]
 { common
    local data,size,vivalue,visize
    label dd RVA data,size,0,0
    data dw size,visize,0
    du 'VS_VERSION_INFO',0,0
    vivalue dd 0FEEF04BDh,00010000h
    local version,count,shift,char,filever,productver
    filever = 0
    productver = 0
   forward
    if name eq 'FileVersion' | name eq 'ProductVersion'
     virtual at 0
      db value
      count = $
      version = 0
      shift = 16
      repeat count
       load char from %-1
       if char='.'
        if shift mod 32
         shift = shift-16
        else
         shift = shift+32+16
        end if
       else
        version = (version and not (0FFFFh shl shift)) or ((version shr shift and 0FFFFh)*10+char-'0') shl shift
       end if
      end repeat
     end virtual
     if name eq 'FileVersion'
      filever = version
     else if name eq 'ProductVersion'
      productver = version
     end if
    end if
   common
    dq filever,productver
    dd 0,0,fileos,filetype+0,filesubtype+0,0,0
    visize = $ - vivalue
    local sfi_data,sfi_size
    sfi_data dd sfi_size
    du 1,'StringFileInfo',0
    local str_data,str_size
    str_data dd str_size
    du 1,'040904E4',0
   forward
    local vs_data,vs_size,value_data,value_size
    align 4
    vs_data dw vs_size,value_size/2
    du 1,name,0
    align 4
    value_data du value,0
    value_size = $ - value_data
    vs_size = $ - vs_data
   common
    align 4
    str_size = $ - str_data
    sfi_size = $ - sfi_data
    local vfi_data,vfi_size,var_data,var_size
    vfi_data dd vfi_size
    du 1,'VarFileInfo',0,0
    var_data dw var_size,4
    du 0,'Translation',0,0
    dw lang,cp+0
    var_size = $ - var_data
    vfi_size = $ - vfi_data
    size = $ - data }

macro resdata label
{ local data,size
  label dd RVA data,size,0,0
  data = $
  ressize equ size = $ - data }

macro endres
{ ressize
  align 4 }
