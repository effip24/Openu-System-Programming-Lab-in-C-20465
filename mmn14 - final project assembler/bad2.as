; This file is intended to check the bad-path of the assembler.

; Each line (except a few) contains an error in the assembly language code.
; A comment preceding each line explains the error.

; All errors should be detectable in the first pass of the assembler.

; Run the assembler on this file, and verify that it catches all the errors.
; Your assembler messages need not be identical to the comments in this file.

; Disclaimer: this list of errors is not exhaustive;
;             you are encouraged to identify additional errors.

Start:  dec DATA4

        sub #5
        sub #5,
        red
       .extern DATA4
        add  #5,#6
Next:   cmp  #5,#6
        inc  #50
        and  r1,r2
        jSr  Start
        cmp  r1,,r2
        add  #5,r1,r3
        prn  r1 blabla
        prn  48
        prn  # 48
        cmp  r1,#r3
Next:   clr  r2
     label1:   sub  r1,r7
        lea  r3,r1
        lea  #3,r5
        stop Start
        .data 400
        .string "maman14"
Z1:     .data   200 400
DATA1:  .data   100,    	200 ,  -500,300, 500
        cmp     r1  ,      DATA1
        add     #1,r2
        Z2:     .data   1, ,3
Z3:     .data   4,
Z4:     .data   #123
Z5:     .data   12.4
Z6:     .data   --5
mov:    .data   5
r6:     .data   200
DATA1:  .data   300
Data1:  .data   100, +200, -300
1DATA:  .data   300
X:      .entry  DATA1
        .extern DATA4
        .extern Start
Y:      .extern DATA8
        .entry  STOP
        .entry  DATA4
DATA2   .data   4
        .DATA   5
        .entry  DATA1
Z7:     .string blabla
Z8:     .string "abcdefg" blabla
STR1:   .string "abc, ,defg"
SuperCalifragilisticExpiAlidocious: .data 4
          Z9:     .data
        .entry
Z10:    .invalid 85, 90
        rts
        rts Next
STOP:   stop
