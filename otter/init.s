# RISC-V baremetal init.s
# This code is executed first.

# use the script in image_assemblr to create this array from any .jpg, .jpeg, or .png
# don't forget to delete this array before adding your own
.data
	img:    .byte # pixel array goes here

.section .text.init
entry:
    la sp, __sp-32      # set up the stack pointer, using a constant defined in the linker script

    # set up interrupts
    la t0, isr
    csrw mtvec, t0
    li t0, 1
    csrw mie, t0

    # load image address in saved register
    la s0, img
    # see 'main.s'
    call _start

end:
    j end

isr:
    # moved image pointer and dimension into argument registers
    mv a0, s0
    # see 'interrupt.c'
    call interrupt
    mret
