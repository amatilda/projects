;----------------------------------------------
macro  _DebugDate
{ common
   if equGlAssembly eq Debug
     struct  _DebugMessage
                               du  'Version: ',equVersionApp,13,10,'Value Eax:  0x'
      MessageEaxHex            du  8 dup(' '),13,10
                               du 'Last error:  0x'
      MessageLastErrHex        du  8 dup(' '),13,10
                               du 'Offset last function:  0x'
      MessageOffsetHex         du  8 dup(' '),0
     ends

     _aDebugMessage  _DebugMessage

     _VTuneAd         LARGE_INTEGER
     _VTuneStart      LARGE_INTEGER
     _VTunehProcess   dd ?
     _VTuneMiniS      du 'H 0x'
     _VTuneMiniH      du 8 dup(' '),13,10
                      du 'L 0x'
     _VTuneMiniL      du 8 dup(' '),0
    end if }





 macro _VTuneMini
 { common
   if equGlAssembly eq Debug


   pushad
   pushfd
   invoke  GetCurrentProcess
      mov  ebx,eax
      mov  [_VTunehProcess],eax

   invoke  SetPriorityClass,ebx,REALTIME_PRIORITY_CLASS
   invoke  SleepEx,0,0

    rdtsc
      mov  [_VTuneAd.LowPart],eax
      mov  [_VTuneAd.HighPart],edx
    rdtsc
      mov  [_VTuneStart.LowPart],eax
      mov  [_VTuneStart.HighPart],edx

    popfd
    popad
   end if }

 macro _EndVTuneMini
 { common
   if equGlAssembly eq Debug
   pushad
   pushfd
   rdtsc
      mov  ecx,[_VTuneStart.LowPart]
      mov  ebx,[_VTuneStart.HighPart]
      sub  eax,ecx
      sbb  edx,ebx
      sub  ecx,[_VTuneAd.LowPart]
      sbb  ebx,[_VTuneAd.HighPart]
      sub  eax,ecx
      sbb  edx,ebx
      mov  ebp,edx

  stdcall  HexToUnicodHL,eax,_VTuneMiniL

  stdcall  HexToUnicodHL,ebp,_VTuneMiniH

      xor  ebx,ebx
   invoke  MessageBoxExW,ebx,_VTuneMiniS,ebx,ebx,ebx;MB_OK = 0

    popfd
    popad
   end if }

;----------------------------------------------
  macro _LastErrorNotSigned
 { common
   if equGlAssembly eq Debug
    local loc
    loc =  $
        test eax,eax
         jns  @f
     stdcall  _DebugMessageProc,loc-$$
         @@:
   end if }


 macro _LastErrorNull
 { common
   if equGlAssembly eq Debug
    local loc
    loc =  $
         cmp eax,0
         je  @f
     stdcall  _DebugMessageProc,loc-$$
         @@:
   end if }

 macro _LastErrorNotNull
 { common
   if equGlAssembly eq Debug
    local loc
    loc =  $
         cmp eax,0
         jne  @f
     stdcall  _DebugMessageProc,loc-$$
         @@:
   end if }

 macro _Debug
 { common
   if equGlAssembly eq Debug  }

 macro _EndDebug
 { common
   end if  }
;----------------------------------------------