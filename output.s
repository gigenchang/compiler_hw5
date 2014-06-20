.text
FA:
# prologue sequence
	sw  $ra, 0($sp)
	sw  $fp, -4($sp)
	add $fp, $sp, -4
	add $sp, $sp, -8
	lw  $v0, _framesize_of_FA
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
_begin_FA:
	lw  $8, 8($fp)
	mtc1  $8, $f4
	cvt.s.w  $f4, $f4
	s.s  $f4, -4($fp)
	l.s  $f6, -4($fp)
	l.s  $f8, -4($fp)
	mul.s  $f4, $f6, $f8
	sw  $4, -808($fp)
	lw  $10, 8($fp)
	lw  $11, -808($fp)
	sgt  $9, $10, $11
	beqz	$9, Lelse_0
	li.s  $f6, 0.000000
	s.s  $f6, -4($fp)
	j Lexit_0
Lelse_0:
	li.s  $f6, 1.000000
	s.s  $f6, -4($fp)
Lexit_0:
	li  $9, 0
	mtc1  $9, $f6
	cvt.s.w  $f6, $f6
	sw  $6, -812($fp)
FTest_0:
	lw  $11, -812($fp)
	lw  $12, 8($fp)
	slt  $10, $11, $12
	beqz	$10, Fexit_0
	j Body_0
Inc_0:
	lw  $11, -812($fp)
	li  $12, 1
	add  $10, $11, $12
	mtc1  $10, $f8
	cvt.s.w  $f8, $f8
	sw  $8, -812($fp)
	j FTest_0
Body_0:
	l.s  $f28, -4($fp)
	s.s	$f28, -816($fp)
	li  $8, 10
	mtc1  $8, $f28
	cvt.s.w  $f28, $f28
	s.s  $f28, -820($fp)
	l.s	$f28, -816($fp)
	l.s	$f30, -820($fp)
	add.s  $f10, $f28, $f30
	li  $11, 0
	lw  $12, -812($fp)
	add $11, $11, $12
	li  $25, 20
	mul $11, $11, $25
	lw  $13, -812($fp)
	add $11, $11, $13
	mul $11, $11, 4
	add  $11, $11, $fp
	add  $11, $11, -804
	s.s  $f10, ($11)
	j Inc_0
Fexit_0:
	lw  $11, -808($fp)
	l.s  $f28, 12($fp)
	s.s	$f28, -824($fp)
	mtc1  $11, $f28
	cvt.s.w  $f28, $f28
	s.s  $f28, -828($fp)
	l.s	$f28, -828($fp)
	l.s	$f30, -824($fp)
	add.s  $f10, $f28, $f30
	move	$v0, $10
_end_FA:
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
	_framesize_of_FA: .word 896

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
	li  $10, 2
	sw	$10, -904($fp)
	li.s $f28, 0.400000
	s.s	$f28, -832($fp)
	lw	$25, -832($fp)
	s.s	$f25, -904($sp)
	li	$24, 8
	sub	$sp, $sp, $24
	jal  FA
	move $13, $v0
	li	$24, 8
	add	$sp, $sp, $24
	li    $v0, 1
	move  $a0, $13
	syscall
	li  $13, 0
	move	$v0, $13
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
	_framesize_of_main: .word 900

.data
