.data
_ch: .space 96
.text
bbb:
# prologue sequence
	sw  $ra, 0($sp)
	sw  $fp, -4($sp)
	add $fp, $sp, -4
	add $sp, $sp, -8
	lw  $v0, _framesize_of_bbb
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
_begin_bbb:
	li.s  $f4, 4.000000
	li.s  $f6, 3.000000
	li.s  $f8, 2.000000
	li   $v0, 6
	syscall
	mov.s $f10, $f0
	add.s  $f26, $f8, $f10
	s.s	$f26, -4($fp)
	l.s	$f30, -4($fp)
	add.s  $f8, $f6, $f30
	add.s  $f6, $f4, $f8
	mov.s	$f0, $f6
_end_bbb:
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
	_framesize_of_bbb: .word 72

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
	jal  bbb
	mov.s $f4, $f0
	s.s  $f4, -8($fp)
	li  $8, 7
	li  $9, 0
	li  $10, 1
	add $9, $9, $10
	li  $25, 3
	mul $9, $9, $25
	li  $11, 0
	add $9, $9, $11
	li  $25, 4
	mul $9, $9, $25
	li  $12, 2
	add $9, $9, $12
	mul $9, $9, 4
	sw  $8, _ch+0($9)
	li  $8,  0
	li  $9, 1
	add $8, $8, $9
	li  $9, 3
	mul $8, $8, $9
	li  $9, 0
	add $8, $8, $9
	li  $9, 4
	mul $8, $8, $9
	li  $9, 2
	add $8, $8, $9
	mul $8, $8, 4
	lw  $9, _ch+0($8)
	li    $v0, 1
	move  $a0, $9
	syscall
	l.s  $f4, -8($fp)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_1: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_1
	syscall
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
	_framesize_of_main: .word 76

.data
