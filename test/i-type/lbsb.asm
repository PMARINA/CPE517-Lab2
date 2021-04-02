.data
val: 0xfdfcfbfa
.text
lb $t0, val
addi $t0, $t0, 1
sb $t0, val
addi $a0, $zero, 0
lb $a0, val
addi $v0, $zero, 1
syscall
addi $v0, $zero, 10
syscall

# We are seeing output 0xfffffffb, no idea why
# Stuff to try
# Make it 0x000000fb
# Make it 0x0000000a
# If that produces similar output, then we can safely assume registers are initialized to F, not 0.
