

;Загружает авторские ресурсы
;----------------------------------------------
proc LoadResoursIndirectHL !BaseRes,!TypeRes,IdRes;!BaseRes = указатель на корневой каталог,!TypeRes = Id типа ресурса,IdRes - Id конечных требуемых данных
          mov  eax,[!BaseRes]
          mov  ecx,[!TypeRes]
          mov  edx,[eax+ecx*4]
          mov  ecx,[IdRes]
          add  eax,edx
          mov  edx,[eax+ecx*8+4]
          mov  ecx,[eax+ecx*8]
          add  eax,edx
          ret; возвращает eax = указатель на конечные данные, ecx = Размер или что еще в зависимости от типа ресурса.
endp