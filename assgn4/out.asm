.text
.globl main
	jal main
	li $v0, 10
	syscall
globVarOK:
	#allocating space on activation record for everything except local variables
	subi $sp, $sp, 44
	sw $fp, 36($sp)
	#saving registers $t0-$t7
	sw $t0, 32($sp)
	sw $t1, 28($sp)
	sw $t2, 24($sp)
	sw $t3, 20($sp)
	sw $t4, 16($sp)
	sw $t5, 12($sp)
	sw $t6, 8($sp)
	sw $t7, 4($sp)
	#make $fp point to return value in activation record
	addi $fp, $sp, 40
	#save return address into activation record
	sw $ra, 4($fp)

	#Begin Function Body
	lw $t0, i
	#loading registers $t0-$t7 back from activation record
	lw $t0, 32($sp)
	lw $t1, 28($sp)
	lw $t2, 24($sp)
	lw $t3, 20($sp)
	lw $t4, 16($sp)
	lw $t5, 12($sp)
	lw $t6, 8($sp)
	lw $t7, 4($sp)
	lw $ra, 4($fp)
	#reset $fp back to where it was before function call
	lw $fp, 36($sp)
	addi $sp, $sp, 44
	jr $ra
locVarOK:
	#allocating space on activation record for everything except local variables
	subi $sp, $sp, 44
	sw $fp, 36($sp)
	#saving registers $t0-$t7
	sw $t0, 32($sp)
	sw $t1, 28($sp)
	sw $t2, 24($sp)
	sw $t3, 20($sp)
	sw $t4, 16($sp)
	sw $t5, 12($sp)
	sw $t6, 8($sp)
	sw $t7, 4($sp)
	#make $fp point to return value in activation record
	addi $fp, $sp, 40
	#save return address into activation record
	sw $ra, 4($fp)

	#Begin Function Body
	#loading var c into activation record
	subi $sp, $sp, 4
	lw $t1, 0($sp)
	#loading registers $t0-$t7 back from activation record
	lw $t0, 32($sp)
	lw $t1, 28($sp)
	lw $t2, 24($sp)
	lw $t3, 20($sp)
	lw $t4, 16($sp)
	lw $t5, 12($sp)
	lw $t6, 8($sp)
	lw $t7, 4($sp)
	lw $ra, 4($fp)
	#reset $fp back to where it was before function call
	lw $fp, 36($sp)
	addi $sp, $sp, 44
	jr $ra
paramOK:
	#allocating space on activation record for everything except local variables
	subi $sp, $sp, 44
	sw $fp, 36($sp)
	#saving registers $t0-$t7
	sw $t0, 32($sp)
	sw $t1, 28($sp)
	sw $t2, 24($sp)
	sw $t3, 20($sp)
	sw $t4, 16($sp)
	sw $t5, 12($sp)
	sw $t6, 8($sp)
	sw $t7, 4($sp)
	#make $fp point to return value in activation record
	addi $fp, $sp, 40
	#save return address into activation record
	sw $ra, 4($fp)

	#Begin Function Body
	lw $t2, 8($fp)
	#loading registers $t0-$t7 back from activation record
	lw $t0, 32($sp)
	lw $t1, 28($sp)
	lw $t2, 24($sp)
	lw $t3, 20($sp)
	lw $t4, 16($sp)
	lw $t5, 12($sp)
	lw $t6, 8($sp)
	lw $t7, 4($sp)
	lw $ra, 4($fp)
	#reset $fp back to where it was before function call
	lw $fp, 36($sp)
	addi $sp, $sp, 44
	jr $ra
undecl:
	#allocating space on activation record for everything except local variables
	subi $sp, $sp, 44
	sw $fp, 36($sp)
	#saving registers $t0-$t7
	sw $t0, 32($sp)
	sw $t1, 28($sp)
	sw $t2, 24($sp)
	sw $t3, 20($sp)
	sw $t4, 16($sp)
	sw $t5, 12($sp)
	sw $t6, 8($sp)
	sw $t7, 4($sp)
	#make $fp point to return value in activation record
	addi $fp, $sp, 40
	#save return address into activation record
	sw $ra, 4($fp)

	#Begin Function Body
	#loading registers $t0-$t7 back from activation record
	lw $t0, 32($sp)
	lw $t1, 28($sp)
	lw $t2, 24($sp)
	lw $t3, 20($sp)
	lw $t4, 16($sp)
	lw $t5, 12($sp)
	lw $t6, 8($sp)
	lw $t7, 4($sp)
	lw $ra, 4($fp)
	#reset $fp back to where it was before function call
	lw $fp, 36($sp)
	addi $sp, $sp, 44
	jr $ra
.data
i:	.word
