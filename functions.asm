section .data
c1 dq 4.0
c2 dq -8.0
c3 dq 3.0
c4 dq 2.0
c5 dq -1.0
    

section .text

; 1 + 4 / (x^2 + 1)
global f1
f1:
    push ebp
    mov ebp, esp
    ;
    finit
    fld qword[c1]
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fld1
    faddp
    fdivp
    fld1
    faddp
    ;
    leave
    ret

; x^3
global f2
f2:
    push ebp
    mov ebp, esp
    ;
    finit
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fld qword[ebp + 8]
    fmulp
    ;
    leave
    ret
 
; 2 ^ (-x)
global f3
f3:
    push ebp
    mov ebp, esp
    ;
    finit
    fld1 
    fld qword[ebp + 8]
    fld st0
    frndint
    fsub st1, st0
    fxch st1
    f2xm1
    fld1
    fadd
    fscale
    fstp st1
    fdivp
    ;
    mov esp, ebp
    pop ebp
    ret

; -8x / ((x^2 + 1)^2)
global f1_
f1_:
    push ebp
    mov ebp, esp
    ;
    finit
    fld qword[c2]
    fld qword[ebp + 8]
    fmulp
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fld1
    faddp
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fld1
    faddp
    fmulp
    fdivp
    ;
    leave
    ret
 
; 3x^2
global f2_
f2_:
    push ebp
    mov ebp, esp
    ;
    finit
    fld qword[ebp + 8]
    fld qword[ebp + 8]
    fmulp
    fld qword[c3]
    fmulp
    ;
    leave
    ret

; -(2^(-x) * ln(2))
global f3_
f3_:
    push ebp
    mov ebp, esp
    ;
    finit
    fld1 
    fld qword[ebp + 8]
    fld st0
    frndint
    fsub st1, st0
    fxch st1
    f2xm1
    fld1
    fadd
    fscale
    fstp st1
    fdivp
    fld1
    fldl2e
    fdivp
    fmulp
    fld qword[c5]
    fmulp
    ;
    mov esp, ebp
    pop ebp
    ret
