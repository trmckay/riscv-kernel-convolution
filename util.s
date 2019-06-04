# HELPER FUNCTIONS FOR KERNEL CONVOLUTION

# a0, a1: vector components
magnitude:
    # t0 <- (a0)^2
    call square
    li t0, 0
    add t0, t0, a0
    # t0 <- t0 + (a1)^2
    mv a1, a0
    call square
    add a0, a0, t0
    call sqrt
    ret

# a0 <- a0^2
square:
    mv a1, a0
    call multiply
    ret

# a0 <- sqrt(a0)
sqrt:
    ret

# a0 <- a0 * a1
# no temporary registers used!
multiply:
    bge a1, a0, add_m
    # swap registers if a1 < a0
    xor a0, a0, a1
    xor a1, a0, a1
    xor a0, a0, a1
    add_m:
    # a0 < a1 is true
    addi a0, a0, -1
    add a1, a1, a1
    bge a0, x0, add_m
    ret

# a0 <- a0 / a1
divide:
    ret
