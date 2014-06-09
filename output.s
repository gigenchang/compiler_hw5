.text
main:
# prologue sequence
	sw  $ra, 0($sp)
	sw  $fp, -4($sp)
	add $fp, $sp, -4
	add $sp, $sp, -8
	lw  $v0, _framesize_of_main
	sub $sp, $sp, $v0
	sw  $8,  64($sp)
	sw  $9,  60($sp)
	sw  $10, 56($sp)
	sw  $11, 52($sp)
	sw  $12, 48($sp)
	sw  $13, 44($sp)
	sw  $14, 40($sp)
	sw  $15, 36($sp)
	sw  $24, 32($sp)
	sw  $25, 28($sp)
	s.s  $f4, 24($sp)
	s.s  $f6, 20($sp)
	s.s  $f8, 16($sp)
	s.s  $f10, 12($sp)
	s.s  $f16, 8($sp)
	s.s  $f18, 4($sp)
_begin_main:
	li  $8, 1
	sw  $8, -4($fp)
	li  $8, 0
	sw  $8, -8($fp)
	lw  $8, -4($fp)
	lw  $9, -8($fp)
	sgt  $10, $8, $9
	beqz	$10, Lelse_0
.data
str_1: .asciiz "correct\n"
.text
	li    $v0, 4
	la    $a0, str_1
	syscall
	j Lexit_0
Lelse_0:
.data
str_2: .asciiz "wrong\n"
.text
	li    $v0, 4
	la    $a0, str_2
	syscall
Lexit_0:
	lw  $8, -4($fp)
	lw  $9, -8($fp)
	slt  $10, $8, $9
	beqz	$10, Lelse_1
.data
str_3: .asciiz "wrong\n"
.text
	li    $v0, 4
	la    $a0, str_3
	syscall
	j Lexit_1
Lelse_1:
.data
str_4: .asciiz "correct\n"
.text
	li    $v0, 4
	la    $a0, str_4
	syscall
Lexit_1:
	lw  $8, -4($fp)
	lw  $9, -8($fp)
	seq  $10, $8, $9
	beqz	$10, Lelse_2
.data
str_5: .asciiz "wrong\n"
.text
	li    $v0, 4
	la    $a0, str_5
	syscall
	j Lexit_2
Lelse_2:
.data
str_6: .asciiz "correct\n"
.text
	li    $v0, 4
	la    $a0, str_6
	syscall
Lexit_2:
	lw  $8, -4($fp)
	lw  $9, -8($fp)
	sne  $10, $8, $9
	beqz	$10, Lelse_3
.data
str_7: .asciiz "correct\n"
.text
	li    $v0, 4
	la    $a0, str_7
	syscall
	j Lexit_3
Lelse_3:
.data
str_8: .asciiz "wrong\n"
.text
	li    $v0, 4
	la    $a0, str_8
	syscall
Lexit_3:
	li.s  $f4, 1.000000
	s.s  $f4, -12($fp)
	li.s  $f4, 0.000000
	s.s  $f4, -16($fp)
	l.s  $f4, -12($fp)
	l.s  $f6, -16($fp)
	c.le.s  $f4, $f6
	li  $8, 1
	bc1f Float_Compare_Label_1
	li  $8, 0
Float_Compare_Label_1:
	beqz	$8, Lelse_4
.data
str_9: .asciiz "correct\n"
.text
	li    $v0, 4
	la    $a0, str_9
	syscall
	j Lexit_4
Lelse_4:
.data
str_10: .asciiz "wrong\n"
.text
	li    $v0, 4
	la    $a0, str_10
	syscall
Lexit_4:
	l.s  $f4, -12($fp)
	l.s  $f6, -16($fp)
	c.lt.s  $f4, $f6
	li  $8, 1
	bc1t Float_Compare_Label_2
	li  $8, 0
Float_Compare_Label_2:
	beqz	$8, Lelse_5
.data
str_11: .asciiz "wrong\n"
.text
	li    $v0, 4
	la    $a0, str_11
	syscall
	j Lexit_5
Lelse_5:
.data
str_12: .asciiz "correct\n"
.text
	li    $v0, 4
	la    $a0, str_12
	syscall
Lexit_5:
	l.s  $f4, -12($fp)
	l.s  $f6, -16($fp)
	c.eq.s  $f4, $f6
	li  $8, 1
	bc1f Float_Compare_Label_3
	li  $8, 0
Float_Compare_Label_3:
	beqz	$8, Lelse_6
.data
str_13: .asciiz "correct\n"
.text
	li    $v0, 4
	la    $a0, str_13
	syscall
	j Lexit_6
Lelse_6:
.data
str_14: .asciiz "wrong\n"
.text
	li    $v0, 4
	la    $a0, str_14
	syscall
Lexit_6:
	lw  $8, -4($fp)
	lw  $9, -8($fp)
	li  $10, 0
	beqz $8, Int_Compare_Label_1
	beqz $9, Int_Compare_Label_1
	li  $10, 1
Int_Compare_Label_1:
	beqz	$10, Lelse_7
.data
str_15: .asciiz "wrong\n"
.text
	li    $v0, 4
	la    $a0, str_15
	syscall
	j Lexit_7
Lelse_7:
Lexit_7:
	lw  $8, -4($fp)
	lw  $9, -8($fp)
	li  $10, 1
	bnez $8, Int_Compare_Label_2
	bnez $9, Int_Compare_Label_2
	li  $10, 0
Int_Compare_Label_2:
	beqz	$10, Lelse_8
.data
str_16: .asciiz "correct\n"
.text
	li    $v0, 4
	la    $a0, str_16
	syscall
	j Lexit_8
Lelse_8:
Lexit_8:
	li  $8, 0
	sw  $8, -4($fp)
Ltrue_0:
	lw  $8, -4($fp)
	li  $9, 10
	slt  $10, $8, $9
	beqz	$10, Lexit_9
.data
str_17: .asciiz "correct: "
.text
	li    $v0, 4
	la    $a0, str_17
	syscall
	lw  $8, -4($fp)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_18: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_18
	syscall
	lw  $8, -4($fp)
	li  $9, 1
	add  $11, $8, $9
	sw  $11, -4($fp)
	j Ltrue_0
Lexit_9:
	lw  $8, -4($fp)
	li  $9, 10
	seq  $11, $8, $9
	beqz	$11, Lelse_9
.data
str_19: .asciiz "correct\n"
.text
	li    $v0, 4
	la    $a0, str_19
	syscall
	j Lexit_10
Lelse_9:
Lexit_10:
	li  $8, 0
	move	$v0, $8
_end_main:
# epilogue sequence
	lw  $8,  64($sp)
	lw  $9,  60($sp)
	lw  $10, 56($sp)
	lw  $11, 52($sp)
	lw  $12, 48($sp)
	lw  $13, 44($sp)
	lw  $14, 40($sp)
	lw  $15, 36($sp)
	lw  $24, 32($sp)
	lw  $25, 28($sp)
	l.s  $f4, 24($sp)
	l.s  $f6, 20($sp)
	l.s  $f8, 16($sp)
	l.s  $f10, 12($sp)
	l.s  $f16, 8($sp)
	l.s  $f18, 4($sp)
	lw   $ra, 4($fp)
	add  $sp, $fp, 4
	lw   $fp, 0($fp)
	li  $v0, 10
	syscall

.data
	_framesize_of_main: .word 84

.data
