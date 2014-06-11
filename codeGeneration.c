#include <stdio.h>
#include <string.h>
#include "header.h"
#include "codeGenHeader.h"
#include "symbolTable.h"

FILE *output;

int reg[32] = {0};
int freg[32] = {0};
int ARoffset = -4;
// Buffer
int buffer_counter = 0;
char label_buffer[BUFFER_SIZE][128];
// Global counter
int float_counter = 0;
int for_counter = 0;
int while_counter = 0;
int if_counter = 0;


void dump_buff()
{
	printf("IN FUNCTION [dump_buff]\n");
	int count = 0;
	fprintf(output, ".data\n");
	for(count = 0; count < buffer_counter; count++){
		fprintf(output, "%s\n", label_buffer[count]);
	}
	printf("buffer:%d\n", buffer_counter);
	buffer_counter = 0;
}

int get_reg()
{
	printf("IN FUNCTION [get_reg]\n");
	int i;
	for (i = 8; i < 24 ; i++) {
		if (reg[i] == 0) {
			reg[i] = 1;
			return i;
		}
	}
	return -1;
}


int get_freg()
{
	printf("IN FUNCTION [get_freg]\n");
	int i;
	for (i = 4; i < 26 ; i+=2) {
		if (i == 12 || i == 14) {
			break;  // $f12, $14 preserved for parameter passing
		} else {
			if (freg[i] == 0) {
				freg[i] = 1;
				return i;
			}
		}
	}
	return -1;
}

int get_offset(SymbolTableEntry* entry)
{
	printf("IN FUNCTION [get_offset]\n");
	return entry->offset;
}

void free_reg(int free_id)
{
	printf("IN FUNCTION [free_reg]\n");
	if(free_id > 0){
		reg[free_id] = 0;
	}
}

void free_freg(int free_id)
{
	printf("IN FUNCTION [free_freg]\n");
	freg[free_id] = 0;
}

void gen_code(AST_NODE *proj)
{
	printf("IN FUNCTION [gen_code]\n");
	AST_NODE *nodePtr;
	nodePtr = proj->child;
	output = fopen("output.s", "w");


	while(nodePtr != NULL){
		gen_global_decl(nodePtr);
		nodePtr = nodePtr->rightSibling;
	}
	dump_buff();	
	fclose(output);
	printf("Code generated!!\n");
}

void gen_global_decl(AST_NODE *global_decl)
{
	printf("IN FUNCTION [gen_global_decl]\n");
	if(global_decl->nodeType == VARIABLE_DECL_LIST_NODE){
		fprintf(output, ".data\n");
		gen_decl_list(global_decl->child);
	}
	else if(global_decl->semantic_value.declSemanticValue.kind == FUNCTION_DECL){
		gen_func_type_empty(global_decl);
	}
	else{
		printf("TYPE: %d\n", global_decl->nodeType);
		printf("Unhandled function type. \n");
	}
	
}

void gen_decl_list(AST_NODE *decl_list)
{
	printf("IN FUNCTION [gen_decl_list]\n");
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
	printf("IN FUNCTION [gen_var_decl]\n");
	if(nodePtr->semantic_value.declSemanticValue.kind == VARIABLE_DECL){
		AST_NODE *temp = nodePtr->child->rightSibling;
		while(temp != NULL){
			if(temp->nodeType != IDENTIFIER_NODE)
				printf("Expected to be ID_NODE\n");
			switch(temp->semantic_value.identifierSemanticValue.kind){
				case NORMAL_ID:
					{
						AST_NODE* type_node = temp->leftmostSibling;
						SymbolTableEntry* type_entry = type_node->semantic_value.identifierSemanticValue.symbolTableEntry;
						if(type_entry->attribute->attr.typeDescriptor->kind == ARRAY_TYPE_DESCRIPTOR){
							gen_array_decl(temp);
						} else {
							gen_decl(temp);
						}
						break;
					}
				case ARRAY_ID:
					gen_array_decl(temp);
					break;
				case WITH_INIT_ID:
					gen_init_decl(temp);
					break;
				default: 
					printf("IS TYPE: %d\n", temp->dataType);
					printf("Unexpected dataType\n");
			}
		temp = temp->rightSibling;
		}
	}
	else
		printf("Should be VARIABLE_DECL type\n");
}

void gen_func_type_empty(AST_NODE *func_head)
{	
	printf("IN FUNCTION [gen_func_type_empty]\n");
	int ARoffset = -4;
	AST_NODE *nodePtr = func_head->child->rightSibling;
	char* func_name = nodePtr->semantic_value.identifierSemanticValue.identifierName;
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
	gen_block(nodePtr->rightSibling->rightSibling);
	fprintf(output, "_end_%s:\n", func_name);
	gen_epilogue(func_name);
}

void gen_block(AST_NODE *block)
{
	printf("IN FUNCTION [gen_block]\n");
	AST_NODE* block_child = block->child;
	switch(block_child->nodeType){
		case VARIABLE_DECL_LIST_NODE:
			gen_decl_list(block_child->child);
			if(block_child->rightSibling != NULL){
				gen_stmt_list(block_child->rightSibling->child);
			}
			break;
		case STMT_LIST_NODE:
			gen_stmt_list(block_child->child);
			break;
		default:
			printf("Unexperted block type\n");
	}
}

void gen_stmt_list(AST_NODE *stmt_ptr)
{
	printf("IN FUNCTION [gen_stmt_list]\n");
	while(stmt_ptr != NULL){
		gen_stmt(stmt_ptr);
		stmt_ptr = stmt_ptr->rightSibling;
	}
	//gen_stmt(stmt_ptr->child);
}


void gen_head(char* name)
{
	printf("IN FUNCTION [gen_head]\n");
	fprintf(output, ".text\n");
	fprintf(output, "%s:\n", name);
}


void gen_stmt(AST_NODE* stmtNode)
{
	printf("IN FUNCTION [gen_stmt]\n");
	if(stmtNode->nodeType == NUL_NODE){
		return;
	}
	else if(stmtNode->nodeType == BLOCK_NODE){
		gen_block(stmtNode);
	}
	else{
		switch(stmtNode->semantic_value.stmtSemanticValue.kind){
			case WHILE_STMT:
				gen_while_stmt(stmtNode);
				break;
			case IF_STMT:
				gen_if_stmt(stmtNode);
				break;
			case FOR_STMT:
				gen_for_stmt(stmtNode);
				break;
			case ASSIGN_STMT:
				gen_assign_stmt(stmtNode);
				break;
			case FUNCTION_CALL_STMT:
				visit_function_call(stmtNode);
				break;
			case RETURN_STMT:
				gen_return_stmt(stmtNode);
				break;
			default:
				printf("Unhandled STMT type\n");
		}
	}
}

void gen_decl(AST_NODE* ID_node)
{
	printf("IN FUNCTION [gen_decl]\n");
	//for normal id的宣告, 請確保傳入的一定是normal id
	SymbolTableEntry* entry = ID_node->semantic_value.identifierSemanticValue.symbolTableEntry;
	int size = SIZE;
	
	if(entry->nestingLevel == 0){ // global variable
		if(entry->attribute->attr.typeDescriptor->properties.dataType == INT_TYPE){
			fprintf(output, "_%s: .word 0\n", entry->name);
		}
		if(entry->attribute->attr.typeDescriptor->properties.dataType == FLOAT_TYPE){
			fprintf(output, "_%s: .float 0.0\n", entry->name);	
		}
	}
	else{						// local variable
		printf("ARoffset:%d\n", ARoffset);
		ARoffset -= (size - 4);	
		entry->offset = ARoffset;
		ARoffset -= 4;
	}
}

void gen_array_decl(AST_NODE* ID_node)
{	
	printf("IN FUNCTION [gen_array_decl]\n");

	SymbolTableEntry* array_entry = ID_node->semantic_value.identifierSemanticValue.symbolTableEntry;
	int size = SIZE;
	int count;
	ArrayProperties arrayProperty = array_entry->attribute->attr.typeDescriptor->properties.arrayProperties;
	for (count = 0; count < arrayProperty.dimension; count++) {
		size *= arrayProperty.sizeInEachDimension[count];
	}
	if(array_entry->nestingLevel == 0){
		fprintf(output, "_%s: .space %d\n", ID_node->semantic_value.identifierSemanticValue.identifierName, size);
	}
	else{
		ARoffset -= (size - 4);
		array_entry->offset = ARoffset;
		ARoffset -= 4;
	}
}

void gen_init_decl(AST_NODE* ID_node)
{
	printf("IN FUNCTION [gen_init_decl]\n");
	SymbolTableEntry* entry = ID_node->semantic_value.identifierSemanticValue.symbolTableEntry;
	AST_NODE* const_value = ID_node->child;
	CON_Type* value = const_value->semantic_value.const1;
	if(entry->attribute->attr.typeDescriptor->properties.dataType == INT_TYPE){
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
	else if(entry->attribute->attr.typeDescriptor->properties.dataType == FLOAT_TYPE){
		printf("lalalalalalallala\n");
		if(entry->nestingLevel == 0){
			fprintf(output, "_%s: .float %f\n", ID_node->semantic_value.identifierSemanticValue.identifierName, value->const_u.fval);
		}
		else{
			int freg = get_freg();
			if(freg == -1){
				printf("Register Deficiency\n");
			}
			if(buffer_counter == BUFFER_SIZE){
				printf("Buffer Boundary Reached\n");
			}
			sprintf(label_buffer[float_counter], "_fp%d: .float %f", float_counter, value->const_u.fval);
			fprintf(output, "\tl.s\t$f%d, _fp%d\n", freg, float_counter);
			entry->offset = ARoffset;
			ARoffset -= 4;
			fprintf(output, "\ts.s\t$f%d, %d($fp)\n", freg, entry->offset);
			free_freg(freg);
			buffer_counter++;
		}
	}
	else{
		printf("OAQQ:%d\n", ID_node->dataType);
		printf("Unexpected const_type\n");
	}
}

void gen_prologue(char* func_name)
{
	printf("IN FUNCTION [gen_prologue]\n");
	fprintf(output, "# prologue sequence\n");
	fprintf(output, "\tsw  $ra, 0($sp)\n");    //存return address
	fprintf(output, "\tsw  $fp, -4($sp)\n");   //存現在的fp
	fprintf(output, "\tadd $fp, $sp, -4\n");   //改fp
	fprintf(output, "\tadd $sp, $sp, -8\n");   //改sp
	fprintf(output, "\tlw  $v0, _framesize_of_%s\n", func_name);
	fprintf(output, "\tsub $sp, $sp, $v0\n");  //移動sp,騰出空間用作push
	fprintf(output, "\tsw  $8,  64($sp)\n");   //$t0   
	fprintf(output, "\tsw  $9,  60($sp)\n"); 
	fprintf(output, "\tsw  $10, 56($sp)\n"); 
	fprintf(output, "\tsw  $11, 52($sp)\n"); 
	fprintf(output, "\tsw  $12, 48($sp)\n"); 
	fprintf(output, "\tsw  $13, 44($sp)\n"); 
	fprintf(output, "\tsw  $14, 40($sp)\n"); 
	fprintf(output, "\tsw  $15, 36($sp)\n");   //$t7
	fprintf(output, "\tsw  $24, 32($sp)\n");   //$t8
	fprintf(output, "\tsw  $25, 28($sp)\n");   //$t9

	//使用f4,6,8,10,16,18作為float temporary reg, 原因見http://www.cs.iit.edu/~virgil/cs470/Labs/Lab4.pdf
	//假設此次作業全部使用single float, 因此使用s.s
	fprintf(output, "\ts.s  $f4, 24($sp)\n");   //$f4
	fprintf(output, "\ts.s  $f6, 20($sp)\n");   //$f6
	fprintf(output, "\ts.s  $f8, 16($sp)\n");   //$f8
	fprintf(output, "\ts.s  $f10, 12($sp)\n");   //$f10
	fprintf(output, "\ts.s  $f16, 8($sp)\n");   //$f16
	fprintf(output, "\ts.s  $f18, 4($sp)\n");   //$f18
}


void gen_epilogue(char* name)
{
	printf("IN FUNCTION [gen_epilogue]\n");
	fprintf(output, "# epilogue sequence\n");
	fprintf(output, "\tlw  $8,  64($sp)\n");   //$t0   
	fprintf(output, "\tlw  $9,  60($sp)\n"); 
	fprintf(output, "\tlw  $10, 56($sp)\n"); 
	fprintf(output, "\tlw  $11, 52($sp)\n"); 
	fprintf(output, "\tlw  $12, 48($sp)\n"); 
	fprintf(output, "\tlw  $13, 44($sp)\n"); 
	fprintf(output, "\tlw  $14, 40($sp)\n"); 
	fprintf(output, "\tlw  $15, 36($sp)\n");   //$t7
	fprintf(output, "\tlw  $24, 32($sp)\n");   //$t8
	fprintf(output, "\tlw  $25, 28($sp)\n");   //$t9

	fprintf(output, "\tl.s  $f4, 24($sp)\n");   //$f4
	fprintf(output, "\tl.s  $f6, 20($sp)\n");   //$f6
	fprintf(output, "\tl.s  $f8, 16($sp)\n");   //$f8
	fprintf(output, "\tl.s  $f10, 12($sp)\n");   //$f10
	fprintf(output, "\tl.s  $f16, 8($sp)\n");   //$f16
	fprintf(output, "\tl.s  $f18, 4($sp)\n");   //$f18
	
	fprintf(output, "\tlw   $ra, 4($fp)\n");   //load return address
	fprintf(output, "\tadd  $sp, $fp, 4\n");   //讓sp pop掉執行完的frame(也就是回到$fp+4)
	fprintf(output, "\tlw   $fp, 0($fp)\n");   //load return pointer

	if (strcmp (name, "main") == 0) { 
		fprintf(output, "\tli  $v0, 10\n");    //system call code 10 means exit
		fprintf(output, "\tsyscall\n"); 
	} 
	else{ 
		fprintf(output, "\tjr  $ra\n"); 
	}
	
	fprintf(output, "\n.data\n");
	fprintf(output, "\t_framesize_of_%s: .word %d\n\n", name, FRAME_SIZE - ARoffset);

}

void gen_while_stmt(AST_NODE* node)
{

	printf("IN FUNCTION [gen_while_stmt]\n");
	AST_NODE* test_expr = node->child;
	AST_NODE* while_stmt = test_expr->rightSibling;
	node->semantic_value.stmtSemanticValue.test_counter = while_counter;
	while_counter++;
	fprintf(output, "Wtrue_%d:\n", node->semantic_value.stmtSemanticValue.test_counter);
	gen_expr(test_expr);
	fprintf(output, "\tbeqz\t$%d, Wexit_%d\n", test_expr->place, node->semantic_value.stmtSemanticValue.test_counter);
	gen_stmt(while_stmt);
	fprintf(output, "\tj Wtrue_%d\n", node->semantic_value.stmtSemanticValue.test_counter);
	fprintf(output, "Wexit_%d:\n", node->semantic_value.stmtSemanticValue.test_counter);
}

void gen_if_stmt(AST_NODE* node)
{
	printf("IN FUNCTION [gen_if_stmt]\n");
	AST_NODE* test_expr = node->child;
	AST_NODE* if_stmt = test_expr->rightSibling;
	AST_NODE* else_stmt = if_stmt->rightSibling;
	node->semantic_value.stmtSemanticValue.test_counter = if_counter;
	if_counter++;	
	gen_expr(test_expr);
	if(else_stmt != NULL){ // if-else case
		fprintf(output, "\tbeqz\t$%d, Lelse_%d\n", test_expr->place, node->semantic_value.stmtSemanticValue.test_counter);
		gen_stmt(if_stmt);
		fprintf(output, "\tj Lexit_%d\n", node->semantic_value.stmtSemanticValue.test_counter);

		fprintf(output, "Lelse_%d:\n", node->semantic_value.stmtSemanticValue.test_counter);
		gen_stmt(else_stmt);

		fprintf(output, "Lexit_%d:\n", node->semantic_value.stmtSemanticValue.test_counter);
	}
	else{
		fprintf(output, "\tbeqz\t$%d, Lexit_%d", test_expr->place, node->semantic_value.stmtSemanticValue.test_counter);
		gen_stmt(if_stmt);

		fprintf(output, "Lexit_%d:\n", node->semantic_value.stmtSemanticValue.test_counter);
	}
	free_reg(test_expr->place);
}

void gen_for_stmt(AST_NODE* node)
{
	printf("IN FUNCTION [gen_for_stmt]\n");
	AST_NODE* assign_expr_1 = node->child;
	AST_NODE* relop_expr = assign_expr_1->rightSibling;
	AST_NODE* assign_expr_2 = relop_expr->rightSibling;
	AST_NODE* statement = assign_expr_2->rightSibling;
	node->semantic_value.stmtSemanticValue.test_counter = for_counter;
	for_counter++;	

	gen_assign_expr_list(assign_expr_1);

	fprintf(output, "FTest_%d:\n", node->semantic_value.stmtSemanticValue.test_counter);
	gen_for_relop_expr_list(relop_expr, node->semantic_value.stmtSemanticValue.test_counter);

	fprintf(output, "\tj Body_%d\n", node->semantic_value.stmtSemanticValue.test_counter);

	fprintf(output, "Inc_%d:\n", node->semantic_value.stmtSemanticValue.test_counter);
	gen_assign_expr_list(assign_expr_2);
	fprintf(output, "\tj FTest_%d\n", node->semantic_value.stmtSemanticValue.test_counter);

	fprintf(output, "Body_%d:\n", node->semantic_value.stmtSemanticValue.test_counter);
	gen_stmt(statement);
	fprintf(output, "\tj Inc_%d\n", node->semantic_value.stmtSemanticValue.test_counter);

	fprintf(output, "Fexit_%d:\n", node->semantic_value.stmtSemanticValue.test_counter);

	free_reg(relop_expr->place);
}

void gen_return_stmt(AST_NODE* node)
{
	printf("IN FUNCTION [gen_return_stmt]\n");
	AST_NODE* relop_expr = node->child;
	gen_expr(relop_expr);
	int type = relop_expr->dataType;
	int reg;
	
	if(type == INT_TYPE){
		if (relop_expr->place > 0) {
			reg = relop_expr->place;
		} else {
			gen_reg_buffer_code(relop_expr->place, 24);
			reg = 24;
		}
		fprintf(output, "\tmove\t$v0, $%d\n", reg);
		free_reg(reg);
	}
	else if(type == FLOAT_TYPE){
		if (relop_expr->place > 0) {
			reg = relop_expr->place;
		} else {
			gen_reg_buffer_code(relop_expr->place, 30);
			reg = 30;
	}
		fprintf(output, "\tmov.s\t$f0, $f%d\n", reg);
		free_freg(reg);
	}
	else{
		printf("TYPE: %d\n", type);
		printf("Unhandled return type\n");
	}
}

void gen_reg_buffer_code(int offset, int reg)
{
	if(reg == 24 || reg == 25 || reg == 26){
		fprintf(output, "\tlw\t$%d, %d($fp)\n", reg, offset);
	}
	else{
		fprintf(output, "\tl.s\t$f%d, %d($fp)\n", reg, offset);
	}
}

void save_value_to_fp(int reg, int offset)
{
	if(reg == 24 || reg == 25){
		fprintf(output, "\tsw\t$%d, %d($fp)\n", reg, offset);
	}
	else{
		fprintf(output, "\ts.s\t$f%d, %d($fp)\n", reg, offset);
	}	
}

void gen_relop_expr_list(AST_NODE* node)
{
	printf("IN FUNCTION [gen_relop_expr_list]\n");
	AST_NODE* relop_expr_node = node->child;
	while(relop_expr_node != NULL){
		gen_expr(relop_expr_node);
		relop_expr_node = relop_expr_node->rightSibling;
	}
}

void gen_for_relop_expr_list(AST_NODE* node, int exit_number)
{
	printf("IN FUNCTION [gen_for_relop_expr_list]\n");
	AST_NODE* relop_expr_node = node->child;
	while(relop_expr_node != NULL){
		gen_expr(relop_expr_node);
		fprintf(output, "\tbeqz\t$%d, Fexit_%d\n", relop_expr_node->place, exit_number);

		relop_expr_node = relop_expr_node->rightSibling;
	}
}

void gen_assign_stmt_list(AST_NODE* node)
{
	printf("IN FUNCTION [gen_assign_stmt_list]\n");
}
void gen_assign_expr_list(AST_NODE* node)
{
	printf("IN FUNCTION [gen_assign_expr_list]\n");
	AST_NODE* assign_list_node = node->child;
	while(assign_list_node != NULL){
		gen_assign_stmt(assign_list_node);
		assign_list_node = assign_list_node->rightSibling;
	}
}
