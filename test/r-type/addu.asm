.text

load:
  addi $t0, $zero, 3
  addi $t1, $zero, 5
  addi $t2, $zero, -4 #65531
  addi $t3, $zero, -10 #65525
  addi $t4, $zero, 65535
compute:
  addi $v0 ,$zero ,1
  addu $a0, $t0, $t1
  syscall
  addu $a0, $t1, $t2
  syscall
  addu $a0, $t0, $t2
  syscall
  addu $a0, $t2, $t3
  syscall
  addu $a0, $t0, $t4
  syscall
  addiu $v0, $zero, 10
  syscall

# Expected outputs:
# 8
# 65536
# 65534
# 131056
# 65538