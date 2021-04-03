.text

addi $t0, $zero, 0xfa
sb $t0, 0x10010000
add $t0, $zero, $zero

lb $t0, 0x10010000
addi $t0, $t0, 1
sb $t0, 0x10010000
addi $a0, $zero, 0
lb $a0, 0x10010000
addi $v0, $zero, 1
syscall
addi $v0, $zero, 10
syscall

# We are seeing output 0xfffffffb, no idea why
# Stuff to try
# Make it 0x000000fb
# Make it 0x0000000a
# If that produces similar output, then we can safely assume registers are initialized to F, not 0.
