.text
floatfunc:
# prologue sequence
	sw  $ra, 0($sp)
	sw  $fp, -4($sp)
	add $fp, $sp, -4
	add $sp, $sp, -8
	lw  $v0, _framesize_of_floatfunc
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
_begin_floatfunc:
	li  $8, 6
	li  $9, 0
	li  $10, 2
	add $9, $9, $10
	li  $25, 4
	mul $9, $9, $25
	li  $11, 1
	add $9, $9, $11
	mul $9, $9, 4
	add $9, $9, $fp
	add $9, $9, -48
	sw  $8, ($9)
	lw  $8, 8($fp)
	l.s  $f6, 12($fp)
	mtc1  $8, $f8
	cvt.s.w  $f8, $f8
	add.s  $f4, $f8, $f6
	li  $8, 0
	li  $9, 1
	add $8, $8, $9
	li  $25, 3
	mul $8, $8, $25
	li  $11, 2
	add $8, $8, $11
	mul $8, $8, 4
	add  $8, $8, $fp
	add  $8, $8, -84
	s.s  $f4, ($8)
	li  $8,  0
	li  $11, 2
	add $8, $8, $11
	li  $11, 4
	mul $8, $8, $11
	li  $11, 1
	add $8, $8, $11
	mul $8, $8, 4
	add $8, $8, $fp
	add $8, $8, -48
	lw  $11, ($8)
	li  $8,  0
	li  $12, 1
	add $8, $8, $12
	li  $12, 3
	mul $8, $8, $12
	li  $12, 2
	add $8, $8, $12
	mul $8, $8, 4
	add  $8, $8, $fp
	add  $8, $8, -84
	l.s  $f6, ($8)
	mtc1  $11, $f8
	cvt.s.w  $f8, $f8
	add.s  $f4, $f8, $f6
	mov.s	$f0, $f4
_end_floatfunc:
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
	jr  $ra

.data
	_framesize_of_floatfunc: .word 152

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
	li  $8, 4
	sw	$8, -4($sp)
	li  $8, 3
	sw	$8, -4($sp)
	li  $8, 6
	mtc1  $8, $f4
	cvt.s.w  $f4, $f4
	s.s	$f4, 0($sp)
	li	$24, 8
	sub	$sp, $sp, $24
	jal  floatfunc
	mov.s $f6, $f0
	li	$24, 8
	add	$sp, $sp, $24
	s.s	$f6, 0($sp)
	li	$24, 8
	sub	$sp, $sp, $24
	jal  floatfunc
	mov.s $f8, $f0
	li	$24, 8
	add	$sp, $sp, $24
	li    $v0, 2
	mov.s $f12, $f8
	syscall
.data
str_1: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_1
	syscall
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
	_framesize_of_main: .word 68

.data
