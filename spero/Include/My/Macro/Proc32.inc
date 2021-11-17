; Macroinstructions for defining and calling procedures

macro stdcall proc,[args]                ; directly call STDCALL procedure
 { common
   local count
   count = 0
    if ~ args eq
      if procM@list@all eq & procM@list@local eq
        reverse
        count = count + 1
        pushd args
    common
       else
        procM@callM count,args
      end if
    end if
    call proc
    if defined proc#.import@parameter@count & proc#.import@parameter@count <> count
      display "Invalid count of parameters for ",`proc,".",0Dh,0Ah
      invoke@Error
    end if  }

macro invoke proc,[args]                 ; indirectly call STDCALL procedure
 { common
   local count
   count = 0
    if ~ args eq
      if procM@list@all eq & procM@list@local eq
        reverse
        count = count + 1
        pushd args
    common
       else
        procM@callM count,args
      end if
    end if
    call [proc]
    if defined proc#.import@parameter@count & proc#.import@parameter@count <> count
      display 'Invalid count of parameters'
      invoke@Error
    end if  }

macro procM@callM outparameter,[args]
 { common
   local count,bool
   count = -4
   bool  =  0
   reverse
   count = count + 4

     match arg0, procM@list@all
     \{ match [arg1], args
        \\{ match [arg3+0], args \\\{ irp arg2, arg0 \\\\{ if arg3 eq arg2
                                                             bool = 1
                                                             pushd [arg1 + count]
                                                           end if  \\\\}  \\\}

            match [arg3-0], args \\\{ irp arg2, arg0 \\\\{ if arg3 eq arg2
                                                             bool = 1
                                                             pushd [arg1 + count]
                                                             end if  \\\\}  \\\}
            if bool = 0
              irp arg2, arg0 \\\{ if arg1 eq arg2
                                    bool = 1
                                    pushd [arg1 + count]
                                  end if  \\\}
            end if  \\}  \}

     match arg0, procM@list@local
     \{ match [arg1], args
        \\{ match [arg3+0], args \\\{ irp arg2, arg0 \\\\{ if arg3 eq arg2
                                                             bool = 1
                                                             pushd [arg1 + count]
                                                           end if  \\\\}  \\\}

            match [arg3-0], args \\\{ irp arg2, arg0 \\\\{ if arg3 eq arg2
                                                             bool = 1
                                                             pushd [arg1 + count]
                                                             end if  \\\\}  \\\}
            if bool = 0
              irp arg2, arg0 \\\{ if arg1 eq arg2
                                    bool = 1
                                    pushd [arg1 + count]
                                  end if  \\\}
            end if  \\}  \}

   if bool = 0
     pushd args
    else
     bool = 0
   end if
   common
   outparameter = (count + 4) / 4  }


macro proc [args]; define procedure
 { common
   match arg0 arg1, args> \{ procM@defineM arg0,<arg1  \}  }

macro procM@defineM name,[args]
 { common

   if used name
     local CountUsesLocals,UsesLocals,CountUsesPushd,UsesPushd,CountUsesParametr
     CountUsesLocals   = 0
     CountUsesPushd    = 0
     CountUsesParametr = 0

     procM@list@all          equ
     procM@list@param        equ
     procM@list@local        equ
     procM@list@local@sizeof equ
     procM@list@local@label  equ

     match <arg0>, args
     \{ match [arg1], arg0 \\{ procM@list@param equ arg1 \\}
        match [arg1] =,arg2, arg0 \\{ procM@list@param equ arg1
                                      procM@list@all equ  arg2 \\}
        match  , procM@list@param \\{ match  , procM@list@all \\\{ procM@list@all equ  arg0 \\\}  \\} \}

    ;Искажаем диасамблирование мусаром
    if equGuardian  eq Guardian
     db %t and 0xF

     if  ( $ - $$) and (4 - 1) <> 0
        if 4 - ( $ - $$) and (4 - 1) = 3
          db %t and 0xF
          db %t and 0xF
          db %t and 0xF
         else if 4 - ( $ - $$) and (4 - 1) = 2
          db %t and 0xF
          db %t and 0xF
         else if 4 - ( $ - $$) and (4 - 1) = 1
          db %t and 0xF
        end if
     end if

     else
     align 4
    end if

     name:

     match arg0, procM@list@all
     \{ virtual at esp + 4 + UsesPushd + UsesLocals

        irp arg1, arg0
        \\{ local ..arg2
              arg1 equ ..arg2
              ..arg2 dd ?
              CountUsesParametr = CountUsesParametr + 1\\}

        end virtual \}

     match arg0, procM@list@param
     \{ irps arg1, arg0
        \\{ pushd arg1
            CountUsesPushd = CountUsesPushd + 4\\} \}


     if UsesLocals <> 0
        if UsesLocals > 0xFF0
          display 'Too much memory is allocated'
          procM@Error
        end if
        sub esp,UsesLocals
     end if


     macro locals
     \{ common
        virtual at esp
        macro label def \\{ match . type,def> \\\{ ProcM@deflocal@proc@label  .,label,<type \\\} \\}
        struc db [val] \\{ \common ProcM@deflocal@proc .,db,1,val \\}
        struc dw [val] \\{ \common ProcM@deflocal@proc .,dw,2,val \\}
        struc du [val] \\{ \common ProcM@deflocal@proc .,du,2,val \\}
        struc dd [val] \\{ \common ProcM@deflocal@proc .,dd,4,val \\}

        struc dq [val]
        \\{ common
        display 'Not suport'
        procM@Error \\} \}


     macro endl
     \{ common
        restruc db,dw,du,dd,dq
        purge   label
        CountUsesLocals = $ - esp
        end virtual \}

     macro ret
     \{ common
        if UsesLocals <> 0
          add esp,UsesLocals
        end if


        match arg0, procM@list@param
        \\{ irps arg1, arg0
            \\\{ reverse
                 popd arg1 \\\}  \\}

        ret  CountUsesParametr * 4

       ;Искажаем диасамблирование мусаром
       if equGuardian  eq Guardian
        db %t and 0xF
       end if  \}

     macro freeJmp
     \{ common
        if UsesLocals <> 0
          add esp,UsesLocals
        end if


        match arg0, procM@list@param
        \\{ irps arg1, arg0
            \\\{ reverse
                 popd arg1 \\\}  \\} \}


     macro  procM names
     \{ common
         ProcM@deflocal@proc@NamesM  names

         match arg0, procM@list@all
         \\{ irp  arg1, arg0
             \\\{ arg1 equ arg1 + 4  \\\} \\}
         match arg0, procM@list@local
         \\{ irp  arg1, arg0
             \\\{ arg1 equ arg1 + 4  \\\} \\} \}


     macro  endpM
     \{ common
        match arg0, procM@list@all
        \\{ irp  arg1, arg0
            \\\{restore arg1 \\\} \\}
        match arg0, procM@list@local
        \\{ irp  arg1, arg0
            \\\{restore arg1 \\\} \\} \}


     macro procM@finishM
     \{ common
        UsesPushd  = CountUsesPushd
        UsesLocals = (CountUsesLocals + 7) and ( not 7)

        name#.import@parameter@count =  CountUsesParametr

        match arg0, procM@list@all
        \\{ restore procM@list@all
            irp arg1, arg0
            \\\{ restore arg1 \\\} \\}

        match arg0, procM@list@local
        \\{ irp arg1, arg0
            \\\{ restore arg1
                 restore procM@list@local \\\} \\}

        match arg0, procM@list@local@sizeof
        \\{ irp arg1, arg0
            \\\{ restore arg1
                 restore procM@list@local@sizeof \\\} \\}

        match arg0, procM@list@local@label
        \\{ irp arg1, arg0
            \\\{ restore arg1
                 restore procM@list@local@label  \\\} \\}

        match arg0, procM@list@param
        \\{ restore procM@list@param\\}

        restore procM@list@all,procM@list@local,procM@list@local@sizeof,procM@list@local@label,procM@list@param
        end if  \}  }


macro endp
 { common
   procM@finishM
   purge   procM@finishM,locals,endl,ret,freeJmp,procM,endpM }



macro ProcM@deflocal@proc name,def,alin,[val]
 { common
   local ..var,loc
   match arg0, procM@list@local \{ procM@list@local equ procM@list@local,name \}
   match arg0, procM@list@local@sizeof \{ procM@list@local@sizeof equ procM@list@local@sizeof,sizeof.#name\}

   match , procM@list@local \{ procM@list@local equ name \}
   match , procM@list@local@sizeof \{ procM@list@local@sizeof equ sizeof.#name\}

   name equ ..var
   align alin
   ..var def val
   sizeof.#name equ loc
   loc = $ - ..var }


macro ProcM@deflocal@proc@label  name,type,val
{ common
   local ..var
    match , procM@list@local@label \{ procM@list@local@label equ name \}
    match arg0, procM@list@local@label \{ procM@list@local@label equ procM@list@local@label,name \}
   name equ ..var
   type  ..var  val }


macro ProcM@deflocal@proc@NamesM names
{ .#names: }












