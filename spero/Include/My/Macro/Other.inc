struc COLORREF red,green,blue
 { common
   .  uniq  red or (green shl 8) or (blue shl 16) }