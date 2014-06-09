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
	_framsize_main: .word 68

.data
