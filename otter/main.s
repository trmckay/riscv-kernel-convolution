# LEDS: 0x11080000
# SWITCHES: 0x11000000

.globl main
.type main, @function
main:
    mv a0, s0
    li a1, 200
    call drawImage

    ret

.globl sleep
.type sleep, @function
sleep:
    addi a0, a0, -1
    bgez a0, sleep
    ret
