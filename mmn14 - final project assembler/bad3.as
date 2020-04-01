; 1. this line is ok
int:     .data 0

; 2. this line is ok
start:   jsr  *r1

; 3. this line is ok
         add  *r3,*r2

; 4. this line is ok
         cmp  r4,*r6

; 5. invalid source operand (register indirect)
         lea  *r1,r1

; 6. invlid characters (int)
         cmp  #48,*int

; 7. invlid characters (42)
         cmp  r1,*42

; 8. invalid characters (*r4)
         stop *r4

; 9. invalid operand (incomplete indirect *)
         dec  * r1
