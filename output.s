.text
test:
# prologue sequence
	sw  $ra, 0($sp)
	sw  $fp, -4($sp)
	add $fp, $sp, -4
	add $sp, $sp, -8
	lw  $v0, _framesize_of_test
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
_begin_test:
	l.s	$f4, _fp0
	s.s	$f4, -4($fp)
	lw  $9, 8($fp)
	mtc1  $9, $f6
	cvt.s.w  $f6, $f6
	li  $8, 1
	li.s $f4, 0.0
	c.eq.s $f4, $f6
	bc1f Float_Compare_Label_1
	l.s  $f8, 12($fp)
	c.eq.s $f4, $f8
	bc1f Float_Compare_Label_1
	li  $8, 0
Float_Compare_Label_1:
	s.s  $f8, -8($fp)
	l.s  $f6, -4($fp)
	li    $v0, 2
	mov.s $f12, $f6
	syscall
	l.s  $f6, -8($fp)
	mov.s	$f0, $f6
_end_test:
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
	_framesize_of_test: .word 76

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
	li	$10, 3
	sw	$10, -12($fp)
	l.s	$f6, _fp0
	s.s	$f6, -16($fp)
	li  $10, 6
	sw	$10, -92($fp)
	li.s  $f6, 3.000000
	s.s	$f6, -88($fp)
	li	$24, 8
	sub	$sp, $sp, $24
	jal  test
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
	li  $11, 6
	sw	$11, -92($fp)
	li  $12, 3
	mtc1  $12, $f8
	cvt.s.w  $f8, $f8
	s.s	$f8, -88($fp)
	li	$24, 8
	sub	$sp, $sp, $24
	jal  test
	mov.s $f10, $f0
	li	$24, 8
	add	$sp, $sp, $24
	li    $v0, 2
	mov.s $f12, $f10
	syscall
.data
str_2: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_2
	syscall
	li  $13, 6
	sw	$13, -92($fp)
	lw  $14, -12($fp)
	mtc1  $14, $f10
	cvt.s.w  $f10, $f10
	s.s	$f10, -88($fp)
	li	$24, 8
	sub	$sp, $sp, $24
	jal  test
	mov.s $f28, $f0
	s.s	$f28, -20($fp)
	li	$24, 8
	add	$sp, $sp, $24
	li    $v0, 2
	l.s	$f28, -20($fp)
	mov.s  $a0, $f28
	syscall
.data
str_3: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_3
	syscall
	li  $12, 6
	sw	$12, -96($fp)
	lw  $15, -12($fp)
	l.s  $f28, -16($fp)
	s.s	$f28, -24($fp)
	mtc1  $15, $f28
	cvt.s.w  $f28, $f28
	s.s  $f28, -28($fp)
	l.s	$f28, -28($fp)
	l.s	$f30, -24($fp)
	add.s  $f26, $f28, $f30
	s.s	$f26, -32($fp)
	lw	$25, -32($fp)
	s.s	$f25, -104($fp)
	li	$24, 8
	sub	$sp, $sp, $24
	jal  test
	mov.s $f28, $f0
	s.s	$f28, -36($fp)
	li	$24, 8
	add	$sp, $sp, $24
	li    $v0, 2
	l.s	$f28, -36($fp)
	mov.s  $a0, $f28
	syscall
.data
str_4: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_4
	syscall
	li  $16, 0
	move	$v0, $16
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
	_framesize_of_main: .word 104

.data
_fp0: .float 0.000000

