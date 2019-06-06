.equ VG_ADDR,  0x11100000
.equ VG_COLOR, 0x11140000

# a0: unsigned int col
# a1: unsigned int row
# a3: unsigned char RGB
.globl draw_dot
.type draw_dot, @function
draw_dot:
    li t3, VG_ADDR
    li t4, VG_COLOR
    andi t0, a0, 0x1FF    # select bottom 7 bits (col), 9bits
    andi t1, a1, 0xFF     # select bottom 6 bits  (row), 8bits
    slli t1, t1, 9        # {xcord[7:0],ycord[8:0]}
    or t0, t0, t1         # 17-bit address
    sw t0, 0(t3)          # write 13 address bits to register
    sw a3, 0(t4)          # write color data to frame buffer
    ret

# a0: cycles to sleep
.globl sleep
.type sleep, @function
sleep:
    addi a0, a0, -1
    bge a0, x0, sleep
    ret
