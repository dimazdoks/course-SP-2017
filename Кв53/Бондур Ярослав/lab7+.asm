.386

scale   macro   p1
    fld max_&p1
    fsub    min_&p1
    fild    p1&_res
    fdivp   st(1), st(0)
    fstp    scale_&p1
endm

x_res_  equ 320
y_res_  equ 200

_data   segment use16
    min_x   dq  -8.0
    max_x   dq  8.0
    mnx     dq  -8.0
    x_res   dw  x_res_
    crt_x   dw  ?
    scale_x dq  ?

    min_y   dq  -3.0
    max_y   dq  3.1
    y_res   dw  y_res_
    crt_y   dw  ?
    scale_y dq  ?
    adr1    dw ?
    step    dq  0.0001
    tmp     dw  ?
    tmp2    dq  ?
    tmp3    dq  ?
    res     dd ?
    min     dq -1.0
    con1    dq 1.0
    
    ryad dq 1.0
    temp dq 1.0
    n   dw 10
    n_cos dw 5
    i dw 0    
    sum dq 0
_data   ends

_code   segment use16
    assume  cs:_code, ds:_data
@main:
    mov ax, _data
    mov ds, ax

    mov ax, 13h
    int 10h

    finit
    scale   x
    scale   y

    call    draw_axis

    push    15
    push    offset func
    call    draw_gra
    add     sp, 4
    push    12

    push    14
    push    offset func1
    call    draw_gra1
    add     sp, 4
    push    12

    mov ah, 8
    int 21h
    mov ax, 3
    int 10h
    mov ax, 4C00h
    int 21h

func    proc 
            
            cmp n,0
                 jz ex
            
    fst tmp2
    fld1
    fst temp
    fldz
    fst sum
    mov cx,n
    
     lable : 
    fld temp
     fmul tmp2
     fst  temp
     fsin
     fadd sum
     fst sum
    loop lable
    
    fld sum
     fld st(0)
            ffree st(2)
            ffree st(3)
            ffree st(4)
            ffree st(5)
            ffree st(6)
            ffree st(7) 
   
    ex:
    ret
    
    
    
func    endp


func1    proc 
    
              cmp n_cos,0
              jz ex1
            
    fst tmp2
    fld1
    fst temp
    fldz
    fst sum
    mov cx,n_cos
    
    lable1 : 
    fld temp
     fmul tmp2
     fst  temp
     fcos
     fadd sum
     fst sum
     loop lable1
    
    fld sum
     fld st(0)
            ffree st(2)
            ffree st(3)
            ffree st(4)
            ffree st(5)
            ffree st(6)
            ffree st(7) 
   
            ex1:
    ret
    
    
func1    endp

draw_gra    proc
    push    bp
    mov bp, sp

    fld min_x
@1:
    fld st(0)
    fld st(0)
    
    call    get_x
    call    func   
    call    get_y
    push    [bp+6]
    call    draw_point
    add     sp, 2
    
    fld     step
     fld tmp2
    faddp   st(1), st(0)
    fcom    max_x
    fstsw   ax
    sahf
    jna @1
    
    ffree   st(0)
    pop bp
    ret
draw_gra    endp

draw_gra1 proc
   push    bp
    mov bp, sp

    fld min_x
@3228:
    fld st(0)
    fld st(0)
    
    call    get_x
    call    word ptr [bp+4] 
    call    get_y
    push    [bp+6]
    call    draw_point1
    add     sp, 2
    
    fld     step
    fld tmp2
    faddp   st(1), st(0)
    fcom    max_x
    fstsw   ax
    sahf
    jna @3228
    
    ffree   st(0)
    pop bp
    ret
draw_gra1 endp
   
get_x   proc
    fsub    min_x
    fdiv    scale_x
    frndint
    fistp   crt_x
    ret
get_x   endp

get_y   proc
    fsub    min_y
    fdiv    scale_y
    frndint
    fistp   crt_y
    mov ax, y_res
    sub ax, crt_y
    mov crt_y, ax
    ret
get_y   endp

draw_point  proc
    push    bp
    mov bp, sp
    mov ax, 0A000h
    mov es, ax
    mov si, crt_y
    mov di, crt_x
    cmp si, y_res_
    jae @nd
    cmp di, x_res_
    jae @nd
    mov ax, x_res_
    mul si
    add ax, di
    mov bx, ax
    mov dx, [bp+4]
    mov byte ptr es:[bx], dl
@nd:
    pop bp
    ret
draw_point  endp


draw_point1  proc
    push    bp
    mov bp, sp
    mov ax, 0A000h
    mov es, ax
    mov si, crt_y
    mov di, crt_x
    cmp si, y_res_
    jae @nd11
    cmp di, x_res_
    jae @nd11
    mov ax, x_res_
    mul si
    add ax, di
    mov bx, ax
    mov dx, [bp+4]
    and  dl, 1101b
    mov byte ptr es:[bx], dl
@nd11:
    pop bp
    ret
draw_point1  endp

draw_axis   proc
    fldz
    call    get_y
    mov crt_x, 0
    mov cx, x_res_
@x_c:
    push    15
    call    draw_point
    add sp, 2
    inc crt_x
    loop    @x_c

    fld max_x
    fsub    min_x
    frndint
    fistp   tmp
    mov cx, tmp

    fld min_x
    frndint
    dec crt_y
@lx:
    fld st(0)
    call    get_x
    push    15
    call    draw_point
    add sp, 2
    
    fld1
    faddp   st(1), st(0)
    loop    @lx
    ffree   st(0)

    fldz
    call    get_x
    mov crt_y, 0
    mov cx, y_res_
@y_c:
    push    15
    call    draw_point
    add sp, 2
    inc crt_y
    loop    @y_c

    fld max_y
    fsub    min_y
    frndint
    fistp   tmp
    mov cx, tmp

    fld min_y
    frndint
    dec crt_x
@ly:
    fst st(1)
    call    get_y
    push    15
    call    draw_point
    add sp, 2   

    fld1
    faddp   st(1), st(0)
    fcom    max_y
    loop    @ly
    ffree   st(0)
    ret
draw_axis   endp

_code   ends
    end @main