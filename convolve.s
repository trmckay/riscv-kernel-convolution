# PROGRAM ARGUMENTS
# a0: starting address of image
# a1: num rows
# a2: num columns
# a3, a4: addresses in memory where 9x9 kernel matrices are located;
#         each component of the matrix is a byte
# a5: address for 76.8kB to be allocated
#     76.8kB of CONTINUOUS memory must be available!
#     the program will DESTROY any data in this range!
# using registers: s0-s8 (saved), a0 (not saved)

process:
# push saved registers to stack
addi sp, sp, -32
sw s0, 0(sp)
sw s1, 4(sp)
sw s2, 8(sp)
sw s3, 12(sp)
sw s4, 16(sp)
sw s5, 20(sp)
sw s6, 24(sp)
sw s7, 28(sp)
sw s8, 32(sp)

# store arguments in saved registers and initialize flags
mv s0, a0
mv s5, a0
mv s1, a1
mv s2, a2
mv s3, a3
mv s4, a4
mv s6, a5
mv s7, a5
li s8, 0

##############################################################################
# IMPORTANT INFO
# DO NOT OVERWRITE THESE REGISTERS
# s0: address of current pixel
# s1: num rows
# s2: num columns, then length of image after calc_len
# s3, s4: addresses in memory where 9x9 kernel matrices are located;
#         each component of the matrix is a byte
# the program will convolve with s3 and s4
# set any of the matrix address registers to <0 if they are to be ignored
# s5: starting address of image
# s6: convolution results destination address (starting)
# s7: convolution results destination address (current)
#############################################################################

    # get length of the pixel array
    call calc_len
    add s6, s5, s2
    mv s7, s6

    # convolution kernel
    mv a0, s3
    next_pixel:
    call convolve
    # store results of convolution
    sb a0, 0(s7)
    addi s0, s0, 1
    addi s7, s7, 1
    bne s0, s2, next_pixel

    done:
    addi sp, sp, 32
    lw s0, -32(sp)
    lw s1, -28(sp)
    lw s2, -24(sp)
    lw s3, -20(sp)
    lw s4, -16(sp)
    lw s5, -12(sp)
    lw s6, -8(sp)
    lw s7, -4(sp)
    lw s8, -0(sp)
    ret

# s2 <- rows * columns
# used registers: a0, a1 (not saved)
calc_len:
    mv a0, s1
    mv a1, s2
    call multiply
    mv s2, a0
    ret

# s0: address of pixel
# s2: number of columns
# registers used: none
# a0-a7 <- neighbors
# with CP as current pixel, neighbors are returned as follows:
# [ a0, a1, a2 ]
# | a3, CP, a4 |
# [ a5, a6, a7 ]
# TODO: add out of bounds detection
get_neighbors:
    # a0: top left neighbor
    sub a0, s0, s2
    addi a0, a0, -1
    # a1: top neighbor
    sub a1, s0, s2
    # a2: top right neighbor
    sub a2, s0, s2
    addi a2, a2, 1
    # a3: left neighbor
    addi a3, s0, -1
    # a4: right neighbor
    addi a4, s0, 1
    # a5: bottom left neighbor
    add a5, s0, s2
    addi a5, a5, -1
    # a6: bottom neighbor
    add a6, s0, s2
    # a7: bottom right neighbor
    add a7, s0, s2
    addi a7, a7, 1
    ret
