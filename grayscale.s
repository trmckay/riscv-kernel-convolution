# all pixel values in memory <- grayscale equivalent
# used registers: a0, a1, a2 (color values, not saved),
#                 t0 (current address, not saved), t1 (RGB value, not saved),
#                 t2 (bit-shifted intensity, not saved)
grayscale:
    # initialize address at beginning of image
    mv t0, s5

    next_rgb:
    lb t1, 0(t0)
    # bit mask for each color
    andi a0, t1, 0xE0
    srli a0, a0, 5
    andi a1, t1, 0x1C
    srli a1, a1, 2
    andi a2, t1, 0x03

    # a0 <- intensity
    # intensity uses only argument registers
    # i.e. temp registers do not need to be saved
    call intensity

    # overwrite all colors with intensity
    li t1, 0
    # TODO: convert 3 bit average to two bit before adding it to blue
    srl t2, a0, 1
    # add blue value
    add t1, t1, t2
    # shift bits to correct position
    slli t2, a0 3
    # add green value
    add t1, t1, t2
    # shift bits
    slli t2, a0, 5
    # add red value
    add t1, t1, t2
    # overwrite pixel
    sb t1, 0(t0)
    addi t0, t0, 1
    bne t0, s2, next_rgb
    ret

# a0, a1, a2: values to be sampled
# registers used: a0, a1, a2
# a0 <- intensity
intensity:
    # a0 <- a0 + a1 + 2(a2)
    add a0, a0, a1
    # blue must be weighted double to account for its smaller range
    add a0, a0, a2
    add a0, a0, a2
    li a1, 3
    # a0 <- a0/3
    call divide
    ret
