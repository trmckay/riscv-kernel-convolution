# s0: address of pixel
# a0: address of kernel
# a0 <- result
convolve:
    # load kernel
    # get neighbors
    # convolve
    ret

# a0: address of first kernel results
# a1: address of second kernel results
# mem[a0] <- array of all magnitude(mem[a0+i], mem[a1+i])
merge:
    ret
