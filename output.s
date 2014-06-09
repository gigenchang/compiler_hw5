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
.data
str_1: .asciiz "input:"
.text
	li    $v0, 4
	la    $a0, str_1
	syscall
	li   $v0, 5
	syscall
	move $8, $v0
	sw  $0, -4($fp)
.data
str_2: .asciiz "input:"
.text
	li    $v0, 4
	la    $a0, str_2
	syscall
	li   $v0, 5
	syscall
	move $9, $v0
	sw  $0, -8($fp)
.data
str_3: .asciiz "input:"
.text
	li    $v0, 4
	la    $a0, str_3
	syscall
	li   $v0, 6
	syscall
	mov.s $f4, $f0
	s.s  $f0, -12($fp)
.data
str_4: .asciiz "input:"
.text
	li    $v0, 4
	la    $a0, str_4
	syscall
	li   $v0, 6
	syscall
	mov.s $f6, $f0
	s.s  $f0, -16($fp)
	lw  $10, -4($fp)
	li    $v0, 1
	move  $a0, $10
	syscall
.data
str_5: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_5
	syscall
	lw  $10, -8($fp)
	li    $v0, 1
	move  $a0, $10
	syscall
.data
str_6: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_6
	syscall
	l.s  $f8, -12($fp)
	li    $v0, 2
	mov.s $f12, $f8
	syscall
.data
str_7: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_7
	syscall
	l.s  $f8, -16($fp)
	li    $v0, 2
	mov.s $f12, $f8
	syscall
.data
str_8: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_8
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
	_framesize_of_main: .word 84

.data
