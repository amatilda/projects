

;��������� ��������� �������
;----------------------------------------------
proc LoadResoursIndirectHL !BaseRes,!TypeRes,IdRes;!BaseRes = ��������� �� �������� �������,!TypeRes = Id ���� �������,IdRes - Id �������� ��������� ������
          mov  eax,[!BaseRes]
          mov  ecx,[!TypeRes]
          mov  edx,[eax+ecx*4]
          mov  ecx,[IdRes]
          add  eax,edx
          mov  edx,[eax+ecx*8+4]
          mov  ecx,[eax+ecx*8]
          add  eax,edx
          ret; ���������� eax = ��������� �� �������� ������, ecx = ������ ��� ��� ��� � ����������� �� ���� �������.
endp