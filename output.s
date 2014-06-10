.data
_a: .word 0
_b: .word 0
_c: .word 0
_d: .word 0
_e: .word 0
_f: .word 0
_g: .word 0
_h: .word 0
_i: .word 0
_j: .word 0
_k: .word 0
_l: .word 0
_m: .word 0
_n: .word 0
_o: .word 0
_p: .word 0
_q: .word 0
_r: .word 0
_s: .word 0
_t: .word 0
_u: .word 0
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
	sw  $8, _a
	li  $8, 2
	sw  $8, _b
	li  $8, 3
	sw  $8, _c
	li  $8, 4
	sw  $8, _d
	li  $8, 5
	sw  $8, _e
	li  $8, 6
	sw  $8, _f
	li  $8, 7
	sw  $8, _g
	li  $8, 8
	sw  $8, _h
	li  $8, 9
	sw  $8, _i
	li  $8, 10
	sw  $8, _j
	li  $8, 11
	sw  $8, _k
	li  $8, 12
	sw  $8, _l
	li  $8, 13
	sw  $8, _m
	li  $8, 14
	sw  $8, _n
	li  $8, 15
	sw  $8, _o
	li  $8, 16
	sw  $8, _p
	li  $8, 17
	sw  $8, _q
	li  $8, 18
	sw  $8, _r
	li  $8, 19
	sw  $8, _s
	li  $8, 20
	sw  $8, _t
	li  $8, 21
	sw  $8, _u
	lw  $8, _a
	lw  $9, _b
	lw  $10, _c
	lw  $11, _d
	lw  $12, _e
	lw  $13, _f
	lw  $14, _g
	lw  $15, _h
	lw  $16, _i
	lw  $17, _j
	lw  $18, _k
	lw  $19, _l
	lw  $20, _m
	lw  $21, _n
	lw  $22, _o
	lw  $23, _p
	lw  $24, _q
	sw	$24, -4($fp)
	lw  $24, _r
	sw	$24, -8($fp)
	lw  $24, _s
	sw	$24, -12($fp)
	lw  $24, _t
	sw	$24, -16($fp)
	add  $24, $-12, $-16
	sw	$24, -20($fp)
	add  $24, $-8, $-20
	sw	$24, -24($fp)
	add  $24, $-4, $-24
	sw	$24, -28($fp)
	add  $24, $23, $-28
	sw	$24, -32($fp)
	add  $23, $22, $-32
	add  $22, $21, $23
	add  $21, $20, $22
	add  $20, $19, $21
	add  $19, $18, $20
	add  $18, $17, $19
	add  $17, $16, $18
	add  $16, $15, $17
	add  $15, $14, $16
	add  $14, $13, $15
	add  $13, $12, $14
	add  $12, $11, $13
	add  $11, $10, $12
	add  $10, $9, $11
	add  $9, $8, $10
	sw  $9, _u
	lw  $8, _u
	li    $v0, 1
	move  $a0, $8
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
	_framesize_of_main: .word 100

.data
