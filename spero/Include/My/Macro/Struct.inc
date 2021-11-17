macro index arg
{ common
   local count
   count = arg

   macro indexM  id
   \{ common
       id = count
       count = count + 1 \} }


macro endi
{ common
  purge indexM }


macro enum num,[arg]
{ common
   local count
   count = num
  forward
   arg uniq count
   count = count + 1  }


;«амена  =(равно) - при использовании uniq индефекаторы присваиваютьс€ только раз. “оесть такое не прокатит Argument uniq 5, а потом Argument uniq 6
;--------------------------------------
struc uniq arg;arg = любое число
{ common
   virtual at arg
    . :
   end virtual }


;«амена встроеному выравниванию так как при базе равной например esp - встроеный не работает
;--------------------------------------
macro align val;val = на сколько выравниванием подерживает от 2 до 16
{ common
  if val = 2 | val = 4 | val = 8 | val = 16
   if  ( $ - $$) and (val - 1) <> 0
     db val - ( $ - $$) and (val - 1) dup(0x90)
   end if
  else if val = 1
  else
   display 'Invalid parameter',0Dh,0Ah
   align@Error
  end if }

   struc dwA [args]
   { common
     align 2
    . dw  args}

   struc duA [args]
   { common
     align 2
    . du  args
    sizeof.#. uniq $ - .}

   struc ddA [args]
   { common
     align 4
    . dd  args}

   struc dqA [args]
   { common
     align 8
    . dq  args}


   macro ddA [args]
   { common
     align 4
     dd  args}

;—оздает таблицы дл€ CRC32 на оснавании полинома зеркальным(более удобным програмистам) способом
;--------------------------------------
macro PolinomCRC32 countTabl,val;val = полином дл€ таблицы;countTabl- глубина(количество) - создаваемых таблиц
{ common
 if ~ defined CRC32_#val | defined @f
  local loc,arg,locmini,temp,big
  big = 0
  align 4
  label CRC32_#val dword
  @@:


 while  big  < countTabl
  loc = 0 ; –асчитать значени€ дл€ всех 256 байт
  big = big + 1
  while  loc < 256;большой цикл дл€ crc32 = 8 * 32
   locmini = 0
   arg     = loc

   while  locmini < 8 * big; малый цикл
    temp = arg
    arg  = arg shr 1
    if (temp and 1) <> 0; ѕроверить четность байта
      arg = arg xor val; XOR выполн€етс€ только если байт нечетный
    end if
    locmini = locmini + 1
   end while ; —ледующий бит

   dd arg  ; «аписать полученный dword в таблицу
   loc = loc + 1
  end while;—ледующий байт
 end while
 end if }



;—труктуры с подержкой выравнивани€ и именованых структур с юнионами
;--------------------------------------
 macro struct name
  { common
    define structM@Main@Struc
    structM@List@Args equ name
    structM@Add@Sub   equ
    structM@Add@Name  equ

    virtual at 0

   struc db [args]
   \{ common
      match arg0, structM@Add@Name \\{ structM@AddM@SubstructM .,arg0,db,<args>,1,ordinary \\}
      match  , structM@Add@Name \\{ structM@List@Args equ structM@List@Args,.,db,<args>,1,ordinary \\}  \}

   struc dw [args]
   \{ common
      match arg0, structM@Add@Name \\{ structM@AddM@SubstructM .,arg0,dw,<args>,2,ordinary \\}
      match  , structM@Add@Name \\{ structM@List@Args equ structM@List@Args,.,dw,<args>,2,ordinary \\}  \}

   struc du [args]
   \{ common
      match arg0, structM@Add@Name \\{ structM@AddM@SubstructM .,arg0,du,<args>,2,ordinary \\}
      match  , structM@Add@Name \\{ structM@List@Args equ structM@List@Args,.,du,<args>,2,ordinary \\}  \}

   struc dd [args]
   \{ common
      match arg0, structM@Add@Name \\{ structM@AddM@SubstructM .,arg0,dd,<args>,4,ordinary \\}
      match  , structM@Add@Name \\{ structM@List@Args equ structM@List@Args,.,dd,<args>,4,ordinary \\}  \}

   struc dq [args]
   \{ common
      match arg0, structM@Add@Name \\{ structM@AddM@SubstructM .,arg0,dq,<args>,8,ordinary \\}
      match  , structM@Add@Name \\{ structM@List@Args equ structM@List@Args,.,dq,<args>,8,ordinary \\}  \}

   macro db [args]
   \{ common
      structM@List@Args@Anymousv3M db,<args>\}

   macro dw [args]
   \{ common
      structM@List@Args@Anymousv3M dw,<args>\}

   macro du [args]
   \{ common
      structM@List@Args@Anymousv3M du,<args>\}

   macro dd [args]
   \{ common
      structM@List@Args@Anymousv3M dd,<args>\}

   macro dq [args]
   \{ common
      structM@List@Args@Anymousv3M dq,<args>\}

   macro union nammed
   \{ common
      match arg0, nammed
      \\{ match  arg1, structM@Add@Name \\\{ structM@AddM@PrefM arg1,nammed \\\}
          match  , structM@Add@Name \\\{ structM@Add@Name  equ nammed \\\}  \\}

      match  , nammed \\{ structM@Add@Name  equ structM@Add@Name \\}

      structM@List@Args@AnymousM  union
      structM@Add@Sub equ union  \}

   macro struct nammed
   \{ common
      match arg0, nammed
      \\{ match  arg1, structM@Add@Name \\\{ structM@AddM@PrefM arg1,nammed \\\}
          match  , structM@Add@Name \\\{ structM@Add@Name  equ nammed \\\}  \\}

      match  , nammed \\{ structM@Add@Name  equ structM@Add@Name \\}

      structM@List@Args@AnymousM  substruct
      structM@Add@Sub equ substruct  \}  }


macro ends
 { match , structM@Add@Sub
   \{ restruc db,dw,du,dd,dq
      purge   union,struct,db,dw,du,dd,dq

      match name=,arg0, structM@List@Args
      \\{ if $ > 0
           display 'Error: definition of ',\\`name,' contains illegal instructions.',0xD,0xA
           structM@Error
          end if

          end virtual

          restore structM@Main@Struc
          structM@MainM name,arg0\\}

      irpv any, structM@List@Args  \\{ restore structM@List@Args \\} \}

   match =union, structM@Add@Sub
   \{ structM@List@Args@Anymousv2M \}

   match =substruct, structM@Add@Sub
   \{ structM@List@Args equ structM@List@Args>,substruct,substruct \}

   restore structM@Add@Sub,structM@Add@Name}


macro structM@MainM name,[subname,type,val,maxsi,mean]
 { common
   local alig
   align.#alig = 1

   virtual at 0
    db `name
    load structM@initial@struct byte from 0

    if structM@initial@struct = '.'
     display 'Error: name of structure should not begin with a dot.',0Dh,0Ah
     structM@Error
    end if
   end virtual

   virtual at 0
   forward

   match =union, mean
   \{ subname = 1
      structM@UnionM alings,,name,subname,val
      align subname
      if subname > align.#alig
       align.#alig = subname
      end if
      maxsi = 0
      structM@UnionM sizeofunion,maxsi,name,subname,val
      rb maxsi\}

   match =substruct, mean
   \{ subname = 1
      structM@UnionM alings,,name,subname,val
      align subname
      if subname > align.#alig
       align.#alig = subname
      end if
      structM@UnionM sizeofsubstruct,,name,subname,val
      align subname\}

   match =ordinary, mean
   \{ if maxsi > align.#alig
        align.#alig = maxsi
      end if
      align maxsi
      name#.#subname type val
      sizeof.#name#.#subname uniq $ - name#.#subname \}

   match =struc, mean
   \{ if maxsi > align.#alig
        align.#alig = maxsi
      end if
      name#.#subname type val \}

   common
    align align.#alig
    sizeof.#name uniq $
    end virtual
    local list
    list equ

   forward
    local arguments
    match any, list \{ list equ list, \}
    list equ list <arguments>

   common
   match values, list \{
   struc name arguments
   \\{ common
      match , structM@Main@Struc
      \\\{ match  arg0, structM@Add@Name \\\\{ structM@AddM@SubstructM .,arg0,name,<values>,align.#alig,struc \\\\}
          match  , structM@Add@Name \\\\{ structM@List@Args equ structM@List@Args,.,name,<values>,align.#alig,struc \\\\} \\\}
      match any, structM@Main@Struc
      \\\{ align align.#alig
          define structM@Pre@Args
          local count
          label . at $


          forward
          match , arguments
          \\\\{ match =ordinary, mean
               \\\\\{ align maxsi
                     .#subname type val \\\\\}

               match =substruct, mean
               \\\\\{ align subname
                     structM@UnionM writesubstruct,name,.,subname,val
                     align subname\\\\\}

               match =union, mean
               \\\\\{ align subname
                     structM@UnionM sizeofunion,maxsi,.,subname,val
                     rb maxsi \\\\\}

               match =struc, mean
               \\\\\{ .#subname type val \\\\\} \\\\}

          match arg0, arguments
          \\\\{ match =ordinary, mean
               \\\\\{ align maxsi
                     .#subname type arg0
                     rb sizeof.#name#.#subname - ($ - .#subname) \\\\\}

               match =substruct, mean
               \\\\\{ align subname
                     irp value, arguments
                     \\\\\\{ reverse
                            define structM@Pre@Args value\\\\\\}
                     structM@UnionM writesubstruct,name,.,subname,val
                     align subname\\\\\}

               match =union, mean
               \\\\\{ align subname
                     structM@UnionM sizeofunion,maxsi,.,subname,val
                     .#subname:
                     count = 0
                     irp value, arg0
                     \\\\\\{ forward
                            count = count + 1

                            match  =?, value
                            \\\\\\\{ count = count - 1 \\\\\\\}

                            match  arg1 arg2, value
                            \\\\\\\{ rb arg1 - $
                                    count = count - 1
                                    arg2 \\\\\\\}  \\\\\\}
                    if count <> 0
                     display 'Error: in parameters Union.',0xD,0xA
                     structM@Error
                    end if
                    rb maxsi - ($ - .#subname) \\\\\}


               match =struc, mean
               \\\\\{ align maxsi
                     .#subname type arg0
                     align maxsi\\\\\} \\\\}

          common
          align align.#alig
          restore structM@Pre@Args

          if sizeof.#name <> $ - .
           display 'Error: my macro.',0xD,0xA
           structM@Error
          end if \\\} \\} \} }

macro structM@List@Args@AnymousM mean
 { common
   local anonymous
   structM@List@Args equ structM@List@Args,anonymous,mean,<}

macro structM@List@Args@Anymousv2M
 { common
   local anonymous
   structM@List@Args equ structM@List@Args>,anonymous,union}

macro structM@List@Args@Anymousv3M val,args
 { common
   local anonymous
   anonymous val args}

macro structM@AddM@SubstructM nammed,subname,type,val,maxsi,mean
 { common
   structM@List@Args equ structM@List@Args,subname#.#nammed,type,<val>,maxsi,mean }

macro structM@AddM@PrefM namme,nammed
 { common
   structM@Add@Name  equ namme#.#nammed }

macro structM@Name@PrefM name,subname,type,val
{ common
  name#.#subname type val}

macro structM@Name@PrefM@Sizeof name,subname,type,val
{ common
  name#.#subname type val
  sizeof.#name#.#subname = $ - name#.#subname}

macro structM@Name@PrefM@Dup names,name,subname,type,val
{ common
  name#.#subname type val
  rb sizeof.#names#.#subname - ($ - name#.#subname)}


macro structM@EnableMUnionM
{ common
macro structM@UnionM trig,names,name,nammed,empty,[subname,type,val,maxsi,mean]
 \{ forward
    match =alings, trig
    \\{ match =substruct, mean
        \\\{ structM@EnableMUnionM
             subname = 1
             structM@UnionM alings,,name,subname,val
             if subname > nammed
               nammed = subname
             end if
             purge structM@UnionM  \\\}


        match =union, mean
        \\\{ structM@EnableMUnionM
             subname = 1
             structM@UnionM alings,,name,subname,val
             if subname > nammed
               nammed = subname
             end if
             purge structM@UnionM  \\\}

        match =ordinary, mean
        \\\{ if maxsi > nammed
              nammed = maxsi
             end if \\\}

        match =struc, mean
        \\\{ if maxsi > nammed
               nammed = maxsi
             end if  \\\} \\}

    match =sizeofunion, trig
    \\{ match =substruct, mean
        \\\{ structM@EnableMUnionM
             virtual at $
             structM@UnionM sizeofsubstruct,,name,subname,val
             align nammed
             if $ - $$ > names
               names = $ - $$
             end if
             purge structM@UnionM
             end virtual \\\}

        match =union, mean
        \\\{ structM@EnableMUnionM
             structM@UnionM sizeofunion,names,name,subname,val
             purge structM@UnionM \\\}

        match =ordinary, mean
        \\\{ virtual at $
             structM@Name@PrefM  name,subname,type,<val>
             if $ - $$ > names
               names = $ - $$
             end if
             end virtual\\\}

        match =struc, mean
        \\\{ virtual at $
             structM@Name@PrefM  name,subname,type,<val>
             if $ - $$ > names
               names = $ - $$
             end if
             end virtual \\\}  \\}


    match =sizeofsubstruct, trig
    \\{ match =substruct, mean
        \\\{ structM@EnableMUnionM
             align nammed
             structM@UnionM sizeofsubstruct,,name,subname,val
             align nammed
             purge structM@UnionM  \\\}

        match =union, mean
        \\\{ structM@EnableMUnionM
             maxsi = 0
             align subname
             structM@UnionM sizeofunion,maxsi,name,subname,val
             rb maxsi
             purge structM@UnionM \\\}

        match =ordinary, mean
        \\\{ align maxsi
             structM@Name@PrefM@Sizeof  name,subname,type,<val>  \\\}

        match =struc, mean
        \\\{ structM@Name@PrefM  name,subname,type,<val> \\\}  \\}

    match =writesubstruct, trig
    \\{ match =substruct, mean
        \\\{ structM@EnableMUnionM
             align nammed
             match arg0, structM@Pre@Args
             \\\\{ restore structM@Pre@Args
                   irp value, arg0
                   \\\\\{ reverse
                          define structM@Pre@Args value\\\\\}  \\\\}
             structM@UnionM writesubstruct,names,name,subname,val
             align nammed
             purge structM@UnionM  \\\}

        match =union, mean
        \\\{ local count
             align subname
             structM@EnableMUnionM
             structM@UnionM sizeofunion,maxsi,name,subname,val
             purge structM@UnionM
             match arg0, structM@Pre@Args
             \\\\{ restore structM@Pre@Args
                   .#subname:
                   count = 0
                   irp value, arg0
                   \\\\\{ forward
                          count = count + 1
                          match  =?, value
                          \\\\\\{ count = count - 1 \\\\\\}
                          match  arg1 arg2, value
                          \\\\\\{ rb arg1 - $
                                  count = count - 1
                                  arg2  \\\\\\}  \\\\\}
                     if count <> 0
                      display 'Error: in parameters Union.',0xD,0xA
                      structM@Error
                     end if \\\\}
                     rb maxsi - ($ - .#subname) \\\}


        match =ordinary, mean
        \\\{ align maxsi
             match , structM@Pre@Args \\\\{ structM@Name@PrefM  name,subname,type,<val> \\\\}
             match arg0, structM@Pre@Args
             \\\\{ structM@Name@PrefM@Dup  names,name,subname,type,<arg0>
                   restore structM@Pre@Args \\\\} \\\}

        match =struc, mean
        \\\{ match , structM@Pre@Args \\\\{ structM@Name@PrefM  name,subname,type,<val> \\\\}
             match arg0, structM@Pre@Args
             \\\\{ structM@Name@PrefM  name,subname,type,<arg0>
                   restore structM@Pre@Args \\\\} \\\} \\}

     \}
  common}

structM@EnableMUnionM

;ƒл€ задани€ гуид например дл€ такого - FOLDERID_RoamingAppData
;--------------------------------------
struc GUID [args]
{ common
   match val, args
   \{ . GUIDM val \}
   match , args
   \{ . GUIDM ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?  \} }

struct GUIDM
 sizeof.GUID uniq sizeof.GUIDM
 valdd          dd  ?
 valdw0         dw  ?
 valdw1         dw  ?
 valdb0         db  ?
 valdb1         db  ?
 valdb2         db  ?
 valdb3         db  ?
 valdb4         db  ?
 valdb5         db  ?
 valdb6         db  ?
 valdb7         db  ?
ends


   struc GUID3 args
   { common
      align 4
      label . dword

      match val, args
      \{ GUID@M val \} }


   macro GUID@M3 valdd, valdw0, valdw1, valdb0, valdb1, valdb2, valdb3, valdb4, valdb5, valdb6, valdb7
   { common
       dd  valdd
       dw  valdw0
       dw  valdw1
       db  valdb0
       db  valdb1
       db  valdb2
       db  valdb3
       db  valdb4
       db  valdb5
       db  valdb6
       db  valdb7  }


