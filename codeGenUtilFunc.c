#include<sdlib.h>
#include<stdio.h>
#include "header.h"

extern FILE *output;

//TODO: 處理reg不夠的問題
//TODO ARRAY ref offset問題
//TODO visit_expr

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


int visit_const(AST_NODE* const_value)
{
	CON_TYPE *val = const_value->semantic_value.const1;
	
   	switch(val->const_type){
		case INTEGERC:
			int reg_id = get_reg();
			if (reg_id != -1) {
				const_value->place = reg_id;
				fprintf(output, "li  $%d, %d\n", reg_id, val->const_u.intval);
			}
			break;
		case FLOATC:
			int reg_id = get_freg();
			if (reg_id != -1) {
				const_value->place = reg_id;
				fprintf(output, "li  $f%d, %f\n", reg_id, val->const_u.fval);
			}
			break;
		case STRINGC:
			printf("未實作gen load imm string\n");
			break;
		default:
			printf("未知的gen_load_imm const value type\n");
	} 
}

void visit_var_ref(AST_NODE* id_node)
{
	//var可能是global或local
	char* var_name = id_node->semantic_value.identifierSemanticValue->identifierName;
	SymbolTableEntry* entry = retrieveSymbol(var_name);
	int is_global = (entry->nestingLevel == 0);

	switch(id_node->semantic_value.identifierSemanticValue.kind){
		case(NORMAL_ID):
			switch (id_node->dataType) {
				case INT_TYPE:
					int reg_id = get_reg();
					if (reg_id != -1){
						id_node->place = reg_id;
						if (is_global) {
							fprintf(output, "lw  $%d, _%s\n", reg_id, var_name);
						} else {
							fprintf(output, "lw  $%d, %d($fp)",  reg_id, entry->offset);
						}
					}
					break;
			    case FLOAT_TYPE:
					int reg_id = get_freg();
					if (reg_id != -1){
						id_node->place = reg_id;
						if (is_global) {
							fprintf(output, "lw  $f%d, _%s\n", reg_id, var_name);
						} else {
							fprintf(output, "lw  $f%d, %d($fp)",  reg_id, entry->offset);
						}
					}
					break;
				default:
					printf("visit_var_ref出現不是INT也不是FLOAT的normal id\n");	
			}
			break;
		case(ARRAY_ID):
			//TODO
			break;
		default:
			printf("visit_var_ref出現無法判斷為normal或array的id_node\n");
	}
}

void visit_function_call(AST_NODE* func_call_stmt_node)
{
	AST_NODE* func_name_node = func_call_stmt_node->child;
	AST_NODE* func_para_node = func_name_node->rightSibling;

	char* func_name = func_name_node->semantic_value.identifierSemanticValue->identifierName;
	
	switch (func_para_node->nodeType) {
		case(NUL_NODE):
			//無參數的function call
			int reg_id = get_reg();
			if (reg_id != -1) {
				func_call_stmt_node->place = reg_id;	
				fprintf(output, "jal  %s\n", func_name);
				fprintf(output, "move $%d, $v0\n", reg_id);
			}
			break;
		case(NONEMPTY_RELOP_EXPR_LIST_NODE):
			//有參數的function call
			break;
		default:
			printf("visit_function_call: 無法判斷的的function_call參數AST_NODE\n");
	}
}




void gen_assign_stmt(AST_NODE* assign_stmt_node)
{
	AST_NODE* left_node = assign_stmt_node->child;
	AST_NODE* right_node = left_node->rightSibling;
	gen_expr(right_node);
}

void gen_expr(AST_NODE* expr_node)
{
	/*
	 * 本function的功能為幫每個expr node(var, func_call, const)配置運算的reg, 
	 * 還有生成以reg進行運算的mips code
	 * */
	switch (expr_node->nodeType) {
		case(EXPR_NODE):
			break;
		case(IDENTIFIER_NODE):
			visit_var_ref(expr_node);			
			break;
		case(CONST_VALUE_NODE):
			visit_const(expr_node);
			break;
		case(STMT_NODE):
			if (expr_node->semantic_value.stmtSemanticValue.kind == FUNCTION_CALL_STMT) {
				visit_function_call(expr_node);
			} else {
				printf("gen_expr中出現無法判斷的STMT_NODE type");
			}
			break;
		default:
			printf("gen_expr出現無法判斷的node type\n");
	}
}


