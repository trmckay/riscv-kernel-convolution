# RISC-V baremetal init.s
# This code is executed first.

.data
	img:    .byte # place 80x80 pixel array here!

.section .text.init
entry:

    la sp, __sp-32      # set up the stack pointer, using a constant defined in the linker script
    mv s0, x0
    call _start #main
    la t0, isr
    csrw mtvec, t0
    li t0, 1
    csrw mie, t0

end:
    j end               # loop when finished if there is no environment to return to.

isr:
    mv a0, s0
    call interrupt
    mret
