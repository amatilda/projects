macro alignM alin,num
 { common
   local loc

   virtual at $
   align alin
   loc = $ - $$
   end virtual

   if ~ num eq
     db loc dup(num)
    else
     db loc dup(?)
   end if  }

macro NumToStrM num,args
 { common
   local loc,mods,count,Error
   count = args
   loc	 = num
   if num < 0xA & args = 0xA
     display num + 0x30
    else if num < 0x10 & args = 0x10
     if num >= 0xA
       display 0x41 + num - 0xA
      else
       display num + 0x30
     end if
    else if args = 0xA | args = 0x10
     repeat 64
      loc = loc / args
      if loc < args
	break
      end if
      count = count * args
     end repeat

     loc = num
     repeat 64
      mods = loc mod count
      loc  = loc / count
       if loc >= 0xA & args = 0x10
	 display 0x41 + loc - 0xA
	else
	 display loc + 0x30
       end if

      if count = 1
	break
      end if
      count = count / args
      loc   = mods
     end repeat
    else
     display 'NumToStrM,num,?',0xD,0xA
     error@
   end if
 }


macro displayM [text,bool]
 { forward
    if ~ text eq  & ~ bool eq
      display text,'0x'
      NumToStrM bool,0x10
      display ' '
      NumToStrM bool,0xA
      display  0xD,0xA
    end if  }

struc maxM addon,[args]
 { common
   local  max
   max	= 0

   forward
   if args > max
     max  = args
   end if

   common
   .  = max + addon  }

macro ctlsizeM num,[args]
 { forward
   if args > num
     display `args
     error@
   end if  }

if ~ equDebug@Driver in <dbg@print,0x353465475657>
macro DbgPrintM message,[args]
 { common
   local  dbgprint,str,jmpstr,subesp
   dbgprint    = 0
   subesp      = 4

   match arg0, equDebug@Driver \{ irps arg1, arg0  \\{ if arg1 eq dbg@print
							 dbgprint = 1
							end if	\\} \}
  forward
  subesp = subesp+4

  common
  if dbgprint <> 0
    jmp jmpstr
    str = $
    db	message,0xD,0xA,0
    jmpstr = $
    if args eq
      subesp = 4
      stdcall  DbgPrintL,str
     else
      stdcall  DbgPrintL,str,args
    end if
    add  esp,subesp

  common
  end if  }
end if

macro int3M
 { common
   push  @f
   push  Int3ExpertL
   push  dword[fs:0]
    mov  [fs:0],esp
    int  3
    @@:
    pop  dword[fs:0]
    add  esp,8
 }

struc CTL_CODE !DeviceType,!Function,!Method,!Access
 { common
   if !DeviceType > 0xFFFF
     display 'DeviceType not higher 0xFFFF'
     error@
   end if

   if !Function < 0x800 | !Function > 0xFFF
     display 'FunctionCode 0x800 ... 0xFFF'
     error@
   end if

   if !Method > 0x4
     display 'TransferType 0x0 ... 0x4'
     error@
   end if

   if !Access > 0x4
     display 'RequiredAccess 0x0 ... 0x4'
     error@
   end if
  . = (!DeviceType shl 16) or (!Access shl 14) or (!Function shl 2) or !Method	}