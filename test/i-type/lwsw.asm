.text
addi $t0, $zero, 0xab
lui $t0, 0xce000000
lw $t0, val
addi $t0, $t0, 1
sw $t0, val
addi $a0, $zero, 0
lw $a0, val
addi $v0, $zero, 1
syscall

addi $t0, $zero, 0xab
lui $t0, 0x4e000000
lw $t0, val
addi $t0, $t0, 1
sw $t0, val
addi $a0, $zero, 0
lw $a0, val
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall

# This is a copy of lbsb with w instead of b...
