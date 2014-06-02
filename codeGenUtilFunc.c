#include<sdlib.h>
#include<stdio.h>
#include "header.h"

extern FILE *output;

struct Reg {
	int val;
	int used;
};

struct FReg {
	float val;
	int used;
};

Reg reg[32];
FReg freg[32];

int init_reg()
{
	int i;
	for (i=0; i<32; i++){
		reg[i].used = 0;
	}
}

int get_temp_reg()
{
	int reg_id[13] = {8, 9, 10, 11, 12, 13, 14, 15, 24, 25};
	int i, id;
	for (i=0; i<13; i++){
		id = reg_id[i];
		if (reg[id].used == 0) {
			reg[id].used = 1;
			return id;
		}
	}
	return -1;
}

int get_saved_reg()
{
	int reg_id[8] = {16, 17, 18, 19, 20 ,21 ,22, 23};
	int i, id;
	for (i=0; i<8; i++){
		id = reg_id[i];
		if (reg[id].used == 0) {
			reg[id].used = 1;
			return id;
		}
	}
	return -1;
}

int get_temp_freg()
{
	int freg_id[6] = {4, 6, 8, 10, 16, 18}; //f12不能拿去用 那是給system call用的
	int i, id;
	for (i=0; i<6; i++){
		id = freg_id[i];
		if (freg[id].used == 0) {
			freg[id].used = 1;
			return id;
		}
	}
	return -1;
}

void free_reg(int reg_id)
{
	reg[reg_id].used = 0;
}

void free_freg(int reg_id)
{
	freg[reg_id].used = 0;
}


void gen_write_call(AST_NODE* expr)
{
	int type = //想辦法get type;
	switch(type){
		case INT_TYPE:
			fprintf(output, "li    $v0, 1\n");
			fprintf(output, "move  $a0, $%d\n", XXX Reg); //TODO
			fprintf(output, "syscall\n");
			freeReg(XXX Reg); //TODO not implement
			break;
		case FLOAT_TYPE:
			fprintf(output, "li    $v0, 1\n");
			fprintf(output, "mov.s $f12, $f%d\n", XXX fReg); //TODO
			fprintf(output, "syscall\n");
			freeFReg(XXX Reg); //TODO not implement
			break;
		case CONST_STRING_TYPE:
			break;
		default:
			printf("未知的write call type\n");
	}	
}


int gen_read_call(int read_in_type)
{
	int reg_id;
	switch(read_in_type){
		case INT_TYPE:
			fprintf(output, "li   $v0, 5\n");
			fprintf(output, "syscall\n");
			reg_id = get_reg(); //TODO not implement
			fprintf(output, "move $%d, $v0\n", reg_id);
			break;
		case FLOAT_TYPE:
			fprintf(output, "li   $v0, 6\n");
			fprintf(output, "syscall\n");
			reg_id = get_freg(); //TODO not implement
			fprintf(output, "move $%d, $f0\n", reg_id);
			break;
		case CONST_STRING_TYPE:
			printf("未實作read in string\n");
			break;
		default:
			printf("未知的read call type\n");
	}
	return reg_id
}


int gen_load_imm(AST_NODE* const_value)
{
	//TODO:不保證該function完整
	CON_TYPE *val = const_value->semantic_value.const1;
	
   	switch(val->const_type){
		case INTEGERC:
			int reg_id = get_reg(); //TODO not implement
			fprintf(output, "li  $%d, %d\n", reg, val->const_u.intval);
			break;
		case FLOATC:
			int reg_id = get_freg(); //TODO not implement
			fprintf(output, "li  $%d, $%f\n", reg, val->const_u.fval);
			break;
		case STRINGC:
			printf("未實作gen load imm string\n");
			break;
		default:
			printf("未知的gen_load_imm const value type\n");
	} 
}
