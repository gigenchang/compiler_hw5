.data
_g1: .word 0
_g2: .float 0.0
_g3: .word 0
_g4: .float 0.0
_g5: .space 8
_g6: .space 8
_g7: .space 8
_g8: .space 8
_g9: .float 0.0
_g10: .float 0.0
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
	li  $8, 1
	sw  $8, _g1
	li.s  $f4, 2.000000
	s.s  $f4, _g2
	li  $8, 3
	sw  $8, _g3
	li.s  $f4, 4.000000
	s.s  $f4, _g4
	li  $8, 5
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	sw  $8, _g5+0($8)
	li  $8, 55
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	sw  $8, _g5+0($8)
	li.s  $f4, 6.000000
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	s.s   $f4, _g6+0($8)
	li.s  $f4, 66.000000
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	s.s   $f4, _g6+0($8)
	li  $8, 7
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	sw  $8, _g7+0($8)
	li  $8, 77
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	sw  $8, _g7+0($8)
	li.s  $f4, 8.000000
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	s.s   $f4, _g8+0($8)
	li.s  $f4, 88.000000
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	s.s   $f4, _g8+0($8)
	li  $8, 9
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	sw  $8, _g9+0($8)
	li  $8, 99
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	sw  $8, _g9+0($8)
	li.s  $f4, 10.000000
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	s.s   $f4, _g10+0($8)
	li.s  $f4, 100.000000
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	s.s   $f4, _g10+0($8)
	li  $8, 1
	sw  $8, -4($fp)
	li.s  $f4, 2.000000
	s.s  $f4, -8($fp)	li  $8, 3
	sw  $8, -12($fp)
	li.s  $f4, 4.000000
	s.s  $f4, -16($fp)	li  $8, 5
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -20
	sw  $8, ($8)
	li  $8, 55
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -20
	sw  $8, ($8)
	li.s  $f4, 6.000000
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -28
	s.s  $f4, ($8)
	li.s  $f4, 66.000000
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -28
	s.s  $f4, ($8)
	li  $8, 7
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -36
	sw  $8, ($8)
	li  $8, 77
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -36
	sw  $8, ($8)
	li.s  $f4, 8.000000
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -44
	s.s  $f4, ($8)
	li.s  $f4, 88.000000
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -44
	s.s  $f4, ($8)
	li  $8, 9
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -52
	sw  $8, ($8)
	li  $8, 99
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -52
	sw  $8, ($8)
	li.s  $f4, 10.000000
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -56
	s.s  $f4, ($8)
	li.s  $f4, 100.000000
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -56
	s.s  $f4, ($8)
	lw  $8, _g1
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_1: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_1
	syscall
	lw  $f4, _g2
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_2: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_2
	syscall
	lw  $8, _g3
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_3: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_3
	syscall
	lw  $f4, _g4
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_4: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_4
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	lw  $8, _g5+0($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_5: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_5
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	lw  $8, _g5+0($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_6: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_6
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	l.s   $f4, _g6+0($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_7: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_7
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	l.s   $f4, _g6+0($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_8: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_8
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	lw  $8, _g7+0($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_9: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_9
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	lw  $8, _g7+0($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_10: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_10
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	l.s   $f4, _g8+0($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_11: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_11
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	l.s   $f4, _g8+0($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_12: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_12
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	lw  $8, _g9+0($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_13: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_13
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	lw  $8, _g9+0($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_14: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_14
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	l.s   $f4, _g10+0($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_15: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_15
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	l.s   $f4, _g10+0($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_16: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_16
	syscall
	lw  $8, -4($fp)	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_17: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_17
	syscall
	lw  $f4, -8($fp)	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_18: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_18
	syscall
	lw  $8, -12($fp)	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_19: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_19
	syscall
	lw  $f4, -16($fp)	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_20: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_20
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -20
	lw  $8, ($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_21: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_21
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -20
	lw  $8, ($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_22: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_22
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -28
	l.s  $f4, ($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_23: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_23
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -28
	l.s  $f4, ($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_24: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_24
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -36
	lw  $8, ($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_25: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_25
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -36
	lw  $8, ($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_26: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_26
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -44
	l.s  $f4, ($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_27: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_27
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -44
	l.s  $f4, ($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_28: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_28
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -52
	lw  $8, ($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_29: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_29
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add $8, $8, $fp
	add $8, $8, -52
	lw  $8, ($8)
	li    $v0, 1
	move  $a0, $8
	syscall
.data
str_30: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_30
	syscall
	li  $8,  0
	li  $8, 0
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -56
	l.s  $f4, ($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_31: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_31
	syscall
	li  $8,  0
	li  $8, 1
	li  $8, 2
	mul $8, $8, 4
	mul $8, $8, $8
	add $8, $8, $8
	add  $8, $8, $fp
	add  $8, $8, -56
	l.s  $f4, ($8)
	li    $v0, 2
	mov.s $f12, $f4
	syscall
.data
str_32: .asciiz "\n"
.text
	li    $v0, 4
	la    $a0, str_32
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
	_framesize_of_main: .word 124

.data
