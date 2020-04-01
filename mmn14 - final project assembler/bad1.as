
X:     .extern DATA1
Y:     .entry START
START:  add  DATA1,r1
        add  Q,r2
        add  r3,Q
        add  X,r4
        add  r5,Y
       .entry Q
       .entry X
       .entry Y
        stop


; 4. undefined label as first operand
; 5. undefined label as second operand
; 6. undefined label as first operand (label X is ignored)
; 7. undefined label as second operand (label Y is ignored)
; 8. undefined label
;9. undefined label (label X is ignored)
; 10. undefined label (label Y is ignored)
