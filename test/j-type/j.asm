.text
addi	$t0, $zero, 4
addi	$t1, $zero, 5
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
