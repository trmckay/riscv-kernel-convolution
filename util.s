.global draw_dot
.type draw_dot, @function
# a0: col
# a1: row
draw_dot:
    andi t0, a0, 0x1FF    # select bottom 7 bits (col), 9bits
    andi t1, a1, 0xFF     # select bottom 6 bits  (row), 8bits
    slli t1, t1, 9        # {xcord[7:0],ycord[8:0]}
    or t0, t0, t1         # 17-bit address
    sw t0, 0(s4)          # write 13 address bits to register
    sw a3, 0(s5)          # write color data to frame buffer
    ret


.global sleep
.type sleep, @function
# a0: cycles to sleep
sleep:
    bge a0, x0, sleep
    ret
