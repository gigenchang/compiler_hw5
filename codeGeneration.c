#include <stdio.h>
#include <string.h>
#include "header.h"

char *function_name;
FILE *output;


int reg[32] = {0};
int freg[32] = {0};

int get_reg()
{
	int i;
	for (i = 8; i<=25 ; i++) {
		if (reg[i] == 0) {
			return i;
		}
	}
	return -1;
}


int get_freg()
{
	int i;
	for (i = 4; i<=30 ; i+=2) {
		if (i == 12 || i == 14) {
			break;  // $f12, $14 preserved for parameter passing
		} else {
			if (freg[i] == 0) {
				return i;
			}
		}
	}
	return -1;
}

void free_reg(int free_id)
{
	reg[free_id] = 0;
}

void free_freg(int free_id)
{
	freg[free_id] = 0;
}

void gen_code(AST_NODE *proj)
{
	AST_Node *nodePtr;
	nodePtr = proj->child;
	output = fopen("output.s", "w");

	//TODO: 下面兩行可以刪掉	
	if(nodePtr == NULL)
		return;
	fprintf(output, ".data"\n);

	while(nodePtr != NULL){
		gen_global_decl(nodePtr);
		nodePtr = nodePtr->rightSibling;
	}
	
	// dunp;
	fclose(output);
}

void gen_global_decl(AST_NODE *global_decl)
{
	
	if(global_decl->semantic_value.declSemanticValue.kind == VARIABLE_DECL_LIST_NODE){
		gen_decl_list(global_decl);
		node = node->rightSibling;	
	}
	if(global_decl->semantic_value.declSemanticValue->kind == FUNCTION_DECL){
		gen_func_type_empty(global_decl);
	}
	else{
		printf("Unhandled function type. \n");
	}
	
}

void gen_decl_list(AST_NODE *decl_list)
{
	while(decl_list != NULL){
		if(decl_list->semantic_value.declSemanticValue.kind == VARIABLE_DECL){
			gen_var_decl(decl_list);
		}
		else{
			printf("Declaration node type unhandled\n");
		}
		decl_list = decl_list->rightSibling;
	}
}

void gen_var_decl(AST_NODE *nodePtr)
{
	if(nodePtr->semantic_value.declSemanticValue->kind == VARIABLE_DECL){
		AST_NODE *temp = nodePtr->child->rightSibling;
		while(nodePtr != NULL){
			if(temp->nodeType != IDENTIFIER_NODE)
				printf("Expected to be ID_NODE\n");
			switch(temp->dataType){
				case NORMAL_ID:
					//gen_decl(temp);
					break;
				case ARRAY_ID:
					//gen_array_decl(temp);
					break;
				case WITH_INIT_ID:
					//gen_init_decl(temp);
					break;
			}
			temp = temp->rightSibling;
		}
	}
	else
		printf("Should be VARIABLE_DECL type\n");
}

void gen_func_type_empty(AST_NODE *func_head)
{	
	AST_NODE *nodePtr = func_head->child->rightSibling;
	char* func_name = nodePtr->semantic_value.const1->sc;
	if(strcmp("read", func_name) == 0){
		// gen_read();
		return;
	}
	else if(strcmp("write", func_name) == 0){
		// gen_write();
		return;
	}
	
	gen_text(func_name);
	gen_prologue(func_name);
	// ARoffset = -4;
	gen_block(nodePtr->rightSibling);
	gen_epilogue(func_name);
}

void gen_block(AST_NODE *block)
{
	switch(block->child->dataType){
		case VARIABLE_DECL_LIST_NODE:
			gen_decl_list(block->child)
			if(block->child->sibling != NULL){
				gen_stmt_list(block->child->sibling);
			}
			break;
		case STMT_LIST_NODE:
			gen_stmt_list(block->child);
			break;
		default:
			printf("Unexperted block type\n");
	}
}

void gen_stmt_list(AST_NODE *stmt_ptr)
{
	while(stmt_list != NULL){
		gen_stmt(stmt_ptr->child);
		stmt_ptr = stmt_ptr->rightSibling;
	}
	gen_stmt(stmt->child);
}


void gen_text(char* name)
{
	fprintf(output, ".text\n");
	fprintf(output, "%s:\n", name);
}


void gen_stmt()
{

}


void gen_global_decl(AST_NODE *nodePtr)
{
	if(nodePtr->semantic_value.declSemanticValue->kind == VARIABLE_DECL){
		fprintf(output, ".data\n");
		AST_NODE *temp = nodePtr->child->rightSibling;
		while(nodePtr != NULL){
			if(temp->nodeType != IDENTIFIER_NODE)
				printf("Expected to be ID_NODE\n");
			switch(temp->dataType){
				case NORMAL_ID:
					//gen_global_decl(temp);
					break;
				case ARRAY_ID:
					//gen_global_array_decl(temp);
					break;
				case WITH_INIT_ID:
					//gen_global_init_decl(temp);
					break;
			}
			temp = temp->rightSibling;
		}
	}
	else
		printf("Should be VARIABLE_DECL type\n");
}

void gen_prologue(char* func_name) {
	fprintf(output, "# prologue sequence\n");
	fprintf(output, "sw  $ra, 0($sp)\n");    //存return address
	fprintf(output, "sw  $fp, -4($sp)\n");   //存現在的fp
	fprintf(output, "add $fp, $sp, -4\n");   //改fp
	fprintf(output, "add $sp, $sp, -8\n");   //改sp
	fprintf(output, "lw  $v0, _framesize_of_%s\n", name);
	fprintf(output, "sub $sp, $sp, $v0\n");  //移動sp,騰出空間用作push
	fprintf(output, "sw  $8,  64($sp)\n");   //$t0   
	fprintf(output, "sw  $9,  60($sp)\n"); 
	fprintf(output, "sw  $10, 56($sp)\n"); 
	fprintf(output, "sw  $11, 52($sp)\n"); 
	fprintf(output, "sw  $12, 48($sp)\n"); 
	fprintf(output, "sw  $13, 44($sp)\n"); 
	fprintf(output, "sw  $14, 40($sp)\n"); 
	fprintf(output, "sw  $15, 36($sp)\n");   //$t7
	fprintf(output, "sw  $24, 32($sp)\n");   //$t8
	fprintf(output, "sw  $25, 28($sp)\n");   //$t9

	//使用f4,6,8,10,16,18作為float temporary reg, 原因見http://www.cs.iit.edu/~virgil/cs470/Labs/Lab4.pdf
	//假設此次作業全部使用single float, 因此使用s.s
	fprintf(output, "s.s  $25, 24($sp)\n");   //$f4
	fprintf(output, "s.s  $25, 20($sp)\n");   //$f6
	fprintf(output, "s.s  $25, 16($sp)\n");   //$f8
	fprintf(output, "s.s  $25, 12($sp)\n");   //$f10
	fprintf(output, "s.s  $25, 8($sp)\n");   //$f16
	fprintf(output, "s.s  $25, 4($sp)\n");   //$f18
}


void gen_epilogue(char* name) {
	fprintf(output, "# epilogue sequence\n");
	fprintf(output, "_end_%s:\n", name);
	fprintf(output, "lw  $8,  64($sp)\n");   //$t0   
	fprintf(output, "lw  $9,  60($sp)\n"); 
	fprintf(output, "lw  $10, 56($sp)\n"); 
	fprintf(output, "lw  $11, 52($sp)\n"); 
	fprintf(output, "lw  $12, 48($sp)\n"); 
	fprintf(output, "lw  $13, 44($sp)\n"); 
	fprintf(output, "lw  $14, 40($sp)\n"); 
	fprintf(output, "lw  $15, 36($sp)\n");   //$t7
	fprintf(output, "lw  $24, 32($sp)\n");   //$t8
	fprintf(output, "lw  $25, 28($sp)\n");   //$t9

	fprintf(output, "l.s  $25, 24($sp)\n");   //$f4
	fprintf(output, "l.s  $25, 20($sp)\n");   //$f6
	fprintf(output, "l.s  $25, 16($sp)\n");   //$f8
	fprintf(output, "l.s  $25, 12($sp)\n");   //$f10
	fprintf(output, "l.s  $25, 8($sp)\n");   //$f16
	fprintf(output, "l.s  $25, 4($sp)\n");   //$f18
	
	fprintf(output, "lw   $ra, 4($fp)\n");   //load return address
	fprintf(output, "add  $sp, $fp, 4\n");   //讓sp pop掉執行完的frame(也就是回到$fp+4)
	fprintf(output, "lw   $fp, 0($fp)\n");   //load return pointer

	if (strcmp (name, "main") == 0) { 
		fprintf(output, "li  $v0, 10\n");    //system call code 10 means exit
		fprintf(output, "syscall\n"); 
	} else { 
		fprintf(output, "jr  $ra\n"); 
	}
}
