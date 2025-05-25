section .data
        eight dd 8.0
        minus_five dd -5.0
section .text

    ; float f1(float x) = exp(x) + 2
    global f1
    f1:
        push ebp
        mov ebp, esp
        finit
        
        fld dword[ebp + 8]; st0 = x
        fldl2e; st0 = log2(e), st1 = x
        fmul; st0 = x * log2(e)
        fld st0; st0 = x * log2(e), st1 = x * log2(e)
        frndint; st0 - целое, st1 - полное
        fsub    st1, st0; st0 - целое, st1 - остаток
        fxch    st1; st0 - отсаток, st1 - целое
        f2xm1; st0 = 2^(x * log2(e)) - 1
        fld1; st0 = 1, st1 = 2^(x * log2(e)) - 1 - остаток, st2 = x * log2(e) - целое 
        fadd; st0 = 2^(x * log2(e)) - остаток, st1 = x * log2(e) - целое 
        fscale; st0 = 2^(x * log2(e) = e^x, st1 = x * log2(e)
        fstp st1; st0 = e^x
        
        fld1; st0 = 1, st1 = e^x
        fadd st0, st0; st0 = 2, st1 = e^x
        fadd; st0 = e^x + 2
        
        mov esp, ebp
        pop ebp
        ret
    
    ; float f2(float x) = -2x + 8
    global f2
    f2:
        push ebp
        mov ebp, esp
        finit
        
        fld dword[ebp + 8]; st0 = x
        fadd st0, st0; st0 = 2x
        fchs; st0 = -2x
        fld dword[eight]; st0 = 8.0, st1 = -2x
        fadd; st0 = -2x + 8
        
        mov esp, ebp
        pop ebp
        ret
        
    ; float f3(float x) = -5 / x
    global f3
    f3:
        push ebp
        mov ebp, esp
        finit
        
        fld dword[minus_five]; st0 = -5.0
        fdiv dword[ebp + 8]; st0 = -5.0 / x
        
        mov esp, ebp
        pop ebp
        ret
