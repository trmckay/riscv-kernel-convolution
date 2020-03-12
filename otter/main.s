# LEDS: 0x11080000
# SWITCHES: 0x11000000

.globl main
.type main, @function
main:
    # move image pointer and dimension into argument registers
    mv a0, s0
    # see 'draw.c'
    call draw_image
    ret
