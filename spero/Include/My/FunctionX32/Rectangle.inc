;��������� ������������� �� RECT � !lprcSrc1 � �������� RECT � !lprcSrc2;!nCount - ���������� � RECT �������
;���� eax = 0 ����� �������������
;--------------------------------------
proc RectInRectHL [esi edi ebp],!lprcSrc1,!lprcSrc2,!nCount;!lprcSrc1- ��������� ��  RECT,!lprcSrc2 - ��������� ��  ������ ������������ RECT;!nCount - ���������� � RECT �������

          mov  esi,[!lprcSrc1]
          mov  edi,[!lprcSrc2]
          mov  ebp,[!nCount]
          xor  eax,eax

          @@:
          mov  ecx,[esi+RECT.left]
          mov  edx,[esi+RECT.right]

          cmp  ecx,[edi+RECT.right]
        cmovg  eax,esp
          cmp  edx,[edi+RECT.left]
        cmovl  eax,esp

          mov  ecx,[esi+RECT.top]
          mov  edx,[esi+RECT.bottom]

          cmp  ecx,[edi+RECT.bottom]
        cmovg  eax,esp
          cmp  edx,[edi+RECT.top]
        cmovl  eax,esp

          xor  ecx,ecx
          dec  ebp
        cmovz  ecx,esp
          add  esi,sizeof.RECT
           or  ecx,eax
           jz  @b

          ret
endp

;��������� ���������� �� ����� � ������� RECT
;���� eax = 0 ����� �������������
;--------------------------------------
proc PtInRectHL [esi ebp ebx],!x,!y,!lprcSrc,!nCount;!x,!y - ���������� �����;!lprcSrc - ��������� ��  ������ ������������ RECT;!nCount - ���������� � RECT �������

          mov  ecx,[!x]
          mov  edx,[!y]
          mov  esi,[!lprcSrc]
          mov  ebp,[!nCount]
          xor  eax,eax

          @@:
          cmp  ecx,[esi+RECT.left]
        cmovl  eax,esp
          cmp  ecx,[esi+RECT.right]
        cmovg  eax,esp

          cmp  edx,[esi+RECT.top]
        cmovl  eax,esp
          cmp  edx,[esi+RECT.bottom]
        cmovg  eax,esp

          xor  ebx,ebx
          dec  ebp
        cmovz  ebx,esp
          add  esi,sizeof.RECT
           or  ebx,eax
           jz  @b

          ret
endp


;���������� ���� RECT � �������� RECT ��� ����� � ���������� �����
;!lprcDst - ��������� �� RECT ���������� ������������;!lprcSrc1 - ��������� �� RECT;!lprcSrc2 - ��������� ��  ������ RECT;!nCount - ���������� � RECT �������
proc UnionRectHL [edi ebx esi ebp],!lprcDst,!lprcSrc1,!lprcSrc2,!nCount
          mov  edi,[!lprcSrc1]
          mov  esi,[!lprcSrc2]

          @@:
          mov  eax,[edi+RECT.left]
          mov  ecx,[edi+RECT.top]
          mov  edx,[esi+RECT.left]
          mov  ebx,[esi+RECT.top]

          cmp  eax,edx
        cmovg  eax,edx
          cmp  ecx,ebx
        cmovg  ecx,ebx

          mov  edx,[edi+RECT.right]
          mov  ebx,[edi+RECT.bottom]
          mov  ebp,[esi+RECT.right]
          mov  edi,[esi+RECT.bottom]

          cmp  edx,ebp
        cmovl  edx,ebp
          cmp  ebx,edi
        cmovl  ebx,edi

          mov  edi,[!lprcDst]
          add  esi,sizeof.RECT
          mov  [edi+RECT.left],eax
          mov  [edi+RECT.top],ecx
          mov  [edi+RECT.right],edx
          mov  [edi+RECT.bottom],ebx

          dec  [!nCount]
          jnz  @b

          ret
endp