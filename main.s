.globl main
.type main, @function
main:
    mv a0, s0
    li a1, 80
    call drawImage
    ret
