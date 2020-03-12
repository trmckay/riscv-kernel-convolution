# LEDS: 0x11080000
# SWITCHES: 0x11000000

.globl main
.type main, @function
main:
    addi sp, sp, -4
    sw a0, 4(sp)
    # move image pointer and dimension into argument registers
    mv a0, s0
    # see 'draw.c'
    call draw_image
    lw a0, 4(sp)
    addi sp, sp, 4
    ret
