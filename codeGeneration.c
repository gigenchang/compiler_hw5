#include <stdio.h>
#include <string.h>
#include "header.h"

#define FRAMESIZE 64;
#define SIZE 4;

char *function_name;
FILE *output;

int reg[32] = {0};
int freg[32] = {0};
int ARoffset = -4;
// Buffer
char label_buffer[32][128];
// Global counter
int float_counter = 0;
int exit_counter = 0;
int else_counter = 0;
int	test_counter = 0;
int	exit_counter = 0;
int	body_counter = 0;
int	Inc_counter = 0;	

// Register related function
int get_reg();
int get_freg();
int get_offset(SymbolTableEntry* entry);
void free_reg(int free_id);
void free_freg(int free_id);

void gen_code(AST_NODE *proj);
void gen_global_decl(AST_NODE *global_decl);
void gen_decl_list(AST_NODE *decl_list);
void gen_var_decl(AST_NODE *nodePtr);
void gen_func_type_empty(AST_NODE *func_head);
void gen_block(AST_NODE *block);
void gen_stmt_list(AST_NODE *stmt_ptr);
void gen_head(char* name);

// variable declaration
void gen_decl(AST_NODE* ID_node);
void gen_array_decl(AST_NODE* ID_node);
void gen_init_decl(AST_NODE* ID_node);

// function declaration
void gen_prologue(char* func_name);
void gen_epilogue(char* name);
void fill_local_var_offset(int offset, SymbolTableEntry* entry);

// stmt generation
void gen_assign_stmt();
void gen_if_stmt(AST_NODE* node);
void gen_for_stmt(AST_NODE* node);
void gen_return_stmt(AST_NODE* node);
void gen_func_call_stmt(AST_NODE* node);

void fill_place(int place, AST_NODE* node);

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

int get_offset(SymbolTableEntry* entry)
{
	return entry->offset;
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
	AST_NODE *nodePtr;
	nodePtr = proj->child;
	output = fopen("output.s", "w");


	while(nodePtr != NULL){
		gen_global_decl(nodePtr);
		nodePtr = nodePtr->rightSibling;
	}
	
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
	fprintf(output, ".data"\n);
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
					gen_decl(temp);
					break;
				case ARRAY_ID:
					gen_array_decl(temp);
					break;
				case WITH_INIT_ID:
					gen_init_decl(temp);
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
	int ARoffset = -4;
	AST_NODE *nodePtr = func_head->child->rightSibling;
	char* func_name = nodePtr->semantic_value.const1->sc;
//	Are these needed ?
	if(strcmp("read", func_name) == 0){
		// gen_read();
		return;
	}
	else if(strcmp("write", func_name) == 0){
		// gen_write();
		return;
	}
//	
	gen_head(func_name);
	gen_prologue(func_name);
	fprintf(output, "_begin_%s:\n", func_name);
	gen_block(nodePtr->rightSibling);
	fprintf(output, "_end_%s:\n", func_name);
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


void gen_head(char* name)
{
	fprintf(output, ".text\n");
	fprintf(output, "%s:\n", name);
}


void gen_stmt(AST_NODE* node)
{
	if(stmtNode->nodeType == NUL_NODE){
		return;
	}
	else if(stmtNode->nodeType == BLOCK_NODE){
		gen_block(node);
	}
	else{
		switch(node->semantic_value.stmtSemanticValue.kind){
			case IF_STMT:
				gen_if_stmt();
				break;
			case FOR_STMT:
				gen_for_stmt();
				break;
			case ASSIGN_STMT:
				gen_assign_stmt();
				break;
			case FUNCTION_CALL_STMT:
				gen_func_call_stmt();
				break;
			case RETURN_STMT:
				gen_return_stmt();
				break;
			default:
				printf("Unhandled STMT type\n");
		}
	}
}

void gen_decl(AST_NODE* ID_node)
{
	SymbolTableEntry* entry = ID_node->semantic_value.identifierSemanticValue.symbolTableEntry;
	if(entry->nestingLevel == 0){ // global variable
		if(entry->attribute->attr.typeDescriptor->properties.dataType == INT_TYPE){
			fprintf(output, "_%s: .word 0\n");
		}
		if(entry->attribute->attr.typeDescriptor->properties.dataType == FLOAT_TYPE){
			fprintf(output, "_%s: .float 0.0\n");	
		}
	}
	else{						// local variable
		entry->offset = ARoffset;
		ARoffset -= 4;	
	}
}

void gen_array_decl(AST_NODE* ID_node)
{	
	SymbolTableEntry* array_entry = ID_node->semantic_value.identifierSemanticValue.symbolTableEntry;
	int size = SIZE;
	int count = 0;
	int arrayProperty = array_entry->attribute->attr.typeDescriptor->properties.arrayProperties;
	for(count; count< arrayProperty.dimension; count++){
		size *= arrayProperty.sizeInEachDimension[count];
	}
	if(entry->nestingLevel == 0){
		fprintf(output, "_%s: .space %d", ID_node->semantic_value.identifierSemanticValue.identifierName, size);
	}
	else{
		array_entry->offset = ARoffset;
		ARoffset -= size;
	}
}

void gen_init_decl(AST_NODE* ID_node)
{
	SymbolTableEntry* entry = ID_node->semantic_value.identifierSemanticValue.symbolTableEntry;
	AST_NODE* const_value = ID_node->rightSibling;
	CON_Type* value = const_value->semantic_value.const1;
	if(value->const_type == INTEGERC){
		if(entry->nestingLevel == 0){
			fprintf(output, "_%s: .word %d\n", ID_node->semantic_value.identifierSemanticValue.identifierName, value->const_u.intval);
		}
		else{
			int reg = get_reg();
			if(reg == -1){
				printf("Register Deficiency\n");
			}
			entry->offset = ARoffset;
			ARoffset -= 4;
			fprintf(output, "\tli\t$%d, %d\n", reg, value->const_u.intval);
			fprintf(output, "\tsw\t$%d, %d($fp)\n", reg, entry->offset);
			free_reg(reg);
		}
	}
	else if(value->const_type == FLOATC){
		if(entry->nestingLevel == 0){
			fprintf(output, "_%s: .float %f\n", ID_node->semantic_value.identifierSemanticValue.identifierName, value->const_u.fval);
		}
		else{
			int freg = get_freg();
			if(freg == -1){
				printf("Register Deficiency\n");
			}
			sprintf(label_buffer[float_counter], "_fp%d: .float %f", float_counter, value->const_u.fval);
			fprintf(output, "\tl.s\t$f%d, _fp%d\n", freg, float_counter);
			fprintf(output, "\ts.s\t$f%d, %d($fp)\n", freg, entry->offset);
			free_freg(freg);
		}
	}
}

void gen_prologue(char* func_name)
{
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
	fprintf(output, "s.s  $f4, 24($sp)\n");   //$f4
	fprintf(output, "s.s  $f6, 20($sp)\n");   //$f6
	fprintf(output, "s.s  $f8, 16($sp)\n");   //$f8
	fprintf(output, "s.s  $f10, 12($sp)\n");   //$f10
	fprintf(output, "s.s  $f16, 8($sp)\n");   //$f16
	fprintf(output, "s.s  $f18, 4($sp)\n");   //$f18
}


void gen_epilogue(char* name)
{
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

	fprintf(output, "l.s  $f4, 24($sp)\n");   //$f4
	fprintf(output, "l.s  $f6, 20($sp)\n");   //$f6
	fprintf(output, "l.s  $f8, 16($sp)\n");   //$f8
	fprintf(output, "l.s  $f10, 12($sp)\n");   //$f10
	fprintf(output, "l.s  $f16, 8($sp)\n");   //$f16
	fprintf(output, "l.s  $f18, 4($sp)\n");   //$f18
	
	fprintf(output, "lw   $ra, 4($fp)\n");   //load return address
	fprintf(output, "add  $sp, $fp, 4\n");   //讓sp pop掉執行完的frame(也就是回到$fp+4)
	fprintf(output, "lw   $fp, 0($fp)\n");   //load return pointer

	if (strcmp (name, "main") == 0) { 
		fprintf(output, "li  $v0, 10\n");    //system call code 10 means exit
		fprintf(output, "syscall\n"); 
	} else { 
		fprintf(output, "jr  $ra\n"); 
	}
	
	fprintf(output, "\n.data\n");
	fprintf("\t_framsize_%s: .word %d\n\n", name, FRAMESIZE - ARoffset);
}

void fill_local_var_offset(int offset, SymbolTableEntry* entry)
{
	entry->offset = offset;
}

void gen_assign_stmt();
void gen_if_stmt(AST_NODE* node)
{
	AST_NODE* test_expr = node->child;
	AST_NODE* if_stmt = test_expr->rightSibling;
	AST_NODE* else_stmt = if_stmt->rightSibling;

	if(else_stmt != NULL){ // if-else case
		fprintf(output, "\tbeqz\t$%d, Lelse_%d\n", test_expr->place, else_counter);
		gen_stmt(if_stmt);
		fprintf(output, "\tj Lexit_%d\n", exit_counter);

		fprintf(output, "Lelse_%d\n:", else_counter);
		gen_stmt(else_stmt);

		fprintf(output, "Lexit_%d\n:", exit_counter);
	}
	else{
		fprintf(output, "\tbeqz\t$%d, Lexit_%d", test_expr->place, exit_counter);
		gen_stmt(if_stmt);

		fprintf(output, "Lexit_:%d\n", exit_counter);
	}
	free_reg(test_expr->place);
	else_counter++;
	exit_counter++;		
}
void gen_for_stmt(AST_NODE* node)
{
	AST_NODE* assign_expr_1 = node->child;
	AST_NODE* relop_expr = assign_expr->rightSibling;
	AST_NODE* assign_expr_2 = relop_expr->rightSibling;
	AST_NODE* statement = assign_expr2->rightSibling;

	gen_assign_expr_list(assign_expr);

	fprintf(output, "Test_%d:\n", test_counter);
	gen_relop_expr_list(relop_expr);
	fprintf(output, "\tbeqz\t$%d, Lexit_%d\n", relop_expr->place, exit_counter);
	fprintf(output, "\tj Body_%d\n", body_counter);

	fprintf(output, "Inc_%d:\n", inc_counter);
	gen_assign_expr_list(assign_expr_2);
	fprintf(output, "\tj TEST_%d\n", test_counter);

	fprintf(output, "Body_%d:\n", body_counter);
	gen_stmt(statement);
	fprintf(output, "\tj Inc\n", Inc_counter);

	fprintf(output, "Lexit_%d:\n", exit_counter);

	free_reg(relop_expr->place);
	test_counter++;
	exit_counter++;
	body_counter++;
	Inc_counter++;	
}
void gen_return_stmt(AST_NODE* node)
{
	AST_NODE* relop_expr = node->child;
	if();
}
void gen_func_call_stmt(AST_NODE* node)
{
	char* func_name = node->semantic_value.const1->sc;
	if(strcmp(func_name, "write") == 0){
		// gen_write;
	}	
	else if (strcmp(func_name, "read") == 0){
		// gen_read;
	}
	else{
		
	}
}
void fill_place(int place, AST_NODE* node)
{
	node->place = place;
}
