.data
len: .word 4
wid: .word 5

.text
lw		$t0, len
lw		$t1, wid
j		perimeter
addi	$a0, $zero, 0
addi	$v0, $zero, 1	#returning integer
syscall

perimeter:
add		$t2, $t0, $t0
add		$t3, $t1, $t1
add		$a0, $t2, $t3

addi	$v0, $zero, 1	#returning integer
syscall
addi	$v0, $zero, 10	#exit
syscall

# Expected outcome:
# 18
# if fail:
# 018
