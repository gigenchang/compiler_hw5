#include<sdlib.h>
#include<stdio.h>
#include "header.h"

extern FILE *output;

//TODO: 處理reg不夠的問題
//TODO ARRAY ref offset問題


int float_compare_label_no = 0;
int int_compare_label_no = 0;
const char FLOAT_COMPARE_LABEL[] = "Float_Compare_Label_";
const char INT_COMPARE_LABEL[] = "Int_Compare_Label_";


int get_float_compare_label_no()
{
	float_compare_label_no += 1;
	return float_compare_label_no;
}

int get_int_compare_label_no()
{
	int_compare_label_no += 1;
	return int_compare_label_no;
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
	return reg_id;
}


void visit_const(AST_NODE* const_value)
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
				fprintf(output, "li.s  $f%d, %f\n", reg_id, val->const_u.fval);
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
	SymbolTableEntry* entry = id_node->semantic_value.identifierSemanticValue.symbolTableEntry;
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
			{
				ArrayProperties arrayProperty = array_entry->attribute->attr.typeDescriptor->properties.arrayProperties;

				int current_dimension = 0;
				AST_NODE* current_dimension_node = id_node->child;
				int total_offset_reg = get_reg();
				if (total_offset_reg != -1) {
					fpritnf(output, "li  $%d,  0\n", total_offset_reg); //先把total offset初始化成0
					while(current_dimension_node != NULL){
						gen_expr(current_dimension_node);
						
						int reg_id = get_reg();
						if (reg_id != -1){
							fprintf(output, "li  $%d, %d\n", reg_id, arrayProperty.dimension[current_dimension]);
							fprintf(output, "mul $%d, $%d, 4\n", reg_id, reg_id);  //乘上size=4
							fprintf(output, "mul $%d, $%d, $%d\n", reg_id, reg_id, current_dimension_node->place);  //乘上[expr]的大小

							fprintf(output, "add $%d, $%d, $%d\n", total_offset_reg, total_offset_reg, reg_id);  //最後把結果加到total offset reg內
							free_reg(reg_id);
						}

						current_dimension += 1;
						current_dimension_node = current_dimension_node->rightSibling;
					}
					switch (id_node->dataType) {
						case INT_TYPE:
							int reg_id = get_reg();
							if (reg_id != -1){
								id_node->place = reg_id;
								if (is_global) {
									fprintf(output, "lw  $%d, _%s+%d($%d)\n", reg_id, var_name, entry->offset, total_offset_reg);
								} else {
									fprintf(output, "add $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
									fprintf(output, "add $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
									fprintf(output, "lw  $%d, ($%d)\n",  reg_id, total_offset_reg);
								}
							}
							break;
						case FLOAT_TYPE:
							int reg_id = get_freg();
							if (reg_id != -1){
								id_node->place = reg_id;
								if (is_global) {
									fprintf(output, "l.s   $f%d, _%s+%d($%d)\n", reg_id, var_name, entry->offset, total_offset_reg);
								} else {
									fprintf(output, "add  $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
									fprintf(output, "add  $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
									fprintf(output, "l.s  $f%d, ($%d)\n",  reg_id, total_offset_reg);
								}
							}
							break;
						default:
							printf("visit_var_ref出現不是INT也不是FLOAT的normal id\n");	
					}
					free_reg(total_offset_reg);
				}
				break;
			}
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
	//最後記得要free掉右邊的reg
}

void visit_expr(AST_NODE* expr_node)
{
	if (expr_node->dataType == INT_TYPE){
		switch(expr_node->semantic_value.exprSemanticValue.kind){
			case(BINARY_OPERATION):
				{
					AST_NODE* left_child = expr_node->child;
					AST_NODE* right_child = left_child->rightSibling;
					gen_expr(left_child);
					gen_expr(right_child);

					int reg_id = get_reg();
					if (reg_id != -1){
						expr_node->place = reg_id;
					
						switch(expr_node->semantic_value.exprSemanticValue.op.binaryOp){
							case(BINARY_OP_ADD):
								fprintf(output, "add  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_SUB):
								fprintf(output, "sub  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_MUL):
								fprintf(output, "mul  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_DIV):
								fprintf(output, "div  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_EQ):
								fprintf(output, "seq  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_GE):
								fprintf(output, "sge  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_LE):
								fprintf(output, "sle  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_NE):
								fprintf(output, "sne  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_GT):
								fprintf(output, "sgt  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_LT):
								fprintf(output, "slt  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_AND):
								{
									//如果左邊或右邊其中一個 == 0. 那就是0, 反之為1
									int label_no = get_int_compare_label_no();
									fprintf(output, "li  $%d, 0\n", reg_id, left_child->place);
									fprintf(output, "beqz $%d, %s%d\n", left_child->place, INT_COMPARE_LABEL, label_no);
									fprintf(output, "beqz $%d, %s%d\n", right_child->place, INT_COMPARE_LABEL, label_no);
									fprintf(output, "li  $%d, 1\n", reg_id);
									fprintf(output, "%s%d:\n", INT_COMPARE_LABEL, label_no);
									break;
								}
							case(BINARY_OP_OR):
								{
									//如果左邊或右邊其中一個 != 0. 那就是1, 反之為0
									int label_no = get_int_compare_label_no();
									fprintf(output, "li  $%d, 1\n", reg_id, left_child->place);
									fprintf(output, "bnez $%d, %s%d\n", left_child->place, INT_COMPARE_LABEL, label_no);
									fprintf(output, "bnez $%d, %s%d\n", right_child->place, INT_COMPARE_LABEL, label_no);
									fprintf(output, "li  $%d, 0\n", reg_id);
									fprintf(output, "%s%d:\n", INT_COMPARE_LABEL, label_no);
									break;
								}
								break;
							default:
								printf("visit_expr出現無法判斷的binary operator\n");
						}
					}
					free_reg(left_child->place);
					free_reg(right_child->place);
					break;
				}
			case(UNARY_OPERATION):
				{
					AST_NODE* child = expr_node->child;
					gen_expr(child);

					int reg_id = get_reg();
					if (reg_id != -1){
						expr_node->place = reg_id;
					
						switch(expr_node->semantic_value.exprSemanticValue.op.unaryOp){
							case(UNARY_OP_POSITIVE):
								fprintf(output, "move  $%d, $%d\n", reg_id, child->place);
								break;
							case(UNARY_OP_NEGATIVE):
								fprintf(output, "neg  $%d, $%d\n", reg_id, child->place);
								break;
							case(UNARY_OP_LOGICAL_NEGATION):
								//如果  是0就變成1
								//    不是0就變成0
								fprintf(output, "seq  $%d, $%d, 0\n", reg_id, child->place);
								break;
							default:
								printf("visit_expr出現無法判斷的unary operator\n");
						}
					}
					free_reg(child->place);
					break;
				}
			default:
				printf("visit_expr出現無法判斷是 BINARY 或 UNIARY 的operation\n");
		}
	} else if (expr_node->dataType == FLOAT_TYPE){
		switch(expr_node->semantic_value.exprSemanticValue.kind){
			case(BINARY_OPERATION):
				{
					AST_NODE* left_child = expr_node->child;
					AST_NODE* right_child = left_child->rightSibling;
					gen_expr(left_child);
					gen_expr(right_child);

					int reg_id = get_freg();
					if (reg_id != -1){
						expr_node->place = reg_id;
					
						switch(expr_node->semantic_value.exprSemanticValue.op.binaryOp){
							case(BINARY_OP_ADD):
								fprintf(output, "add.s  $f%d, $f%d, $f%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_SUB):
								fprintf(output, "sub.s  $f%d, $f%d, $f%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_MUL):
								fprintf(output, "mul.s  $f%d, $f%d, $f%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_DIV):
								fprintf(output, "div.s  $f%d, $f%d, $f%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_EQ):
								{
									free_freg(reg_id); //把float reg還回去
									reg_id = get_reg(); //重新要一個一般的reg

									if (reg_id != -1){
										expr_node->place = reg_id;
										int label_no = get_float_compare_label_no();
										fprintf(output, "c.eq.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "li  %d, 1\n", reg_id);
										fprintf(output, "bc1t %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "li  %d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									}
									break;
								}
							case(BINARY_OP_GE):
								{
									//>= is not <
									free_freg(reg_id);
									reg_id = get_reg();

									if (reg_id != -1) {
										expr_node->place = reg_id;
										int label_no = get_float_compare_label_no();
										fprintf(output, "c.lt.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "li  %d, 1\n", reg_id);
										fprintf(output, "bc1f %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "li  %d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									}
									break;
								}
							case(BINARY_OP_LE):
								{
									free_freg(reg_id);
									reg_id = get_reg();
									if (reg_id != -1) {
										expr_node->place = reg_id;
										int label_no = get_float_compare_label_no();
										fprintf(output, "c.le.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "li  %d, 1\n", reg_id);
										fprintf(output, "bc1t %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "li  %d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									}
									break;
								}
							case(BINARY_OP_NE):
								{
									//!= is not ==
									free_freg(reg_id);
									reg_id = get_reg();
									if (reg_id != -1) {
										expr_node->place = reg_id;
										int label_no = get_float_compare_label_no();
										fprintf(output, "c.eq.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "li  %d, 1\n", reg_id);
										fprintf(output, "bc1f %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "li  %d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									}
									break;
								}
							case(BINARY_OP_GT):
								{
									//> is !<=
									free_freg(reg_id);
									reg_id = get_reg();
									if (reg_id != -1) {
										expr_node->place = reg_id;
										int label_no = get_float_compare_label_no();
										fprintf(output, "c.le.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "li  %d, 1\n", reg_id);
										fprintf(output, "bc1f %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "li  %d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									}
									break;
								}
							case(BINARY_OP_LT):
								{
									free_freg(reg_id);
									reg_id = get_reg();

									if (reg_id != -1) {
										expr_node->place = reg_id;
										int label_no = get_float_compare_label_no();
										fprintf(output, "c.lt.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "li  %d, 1\n", reg_id);
										fprintf(output, "bc1t %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "li  %d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									}
									break;
								}
							case(BINARY_OP_AND):
								{
									//如果左邊或右邊其中一個 == 0.0 那就是0, 反之為1
									int int_reg_id = get_reg();
									if (int_reg_id != -1) {
										expr_node->place = int_reg_id; 
										int label_no = get_float_compare_label_no();
										fprintf(output, "li  $%d, 0\n", int_reg_id);
										fprintf(output, "li.s $f%d, 0.0", reg_id);
										fprintf(output, "c.eq.s $f%d, $f%d\n", reg_id, left_child->place);
										fprintf(output, "bc1f %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, (有child==0.0) 就直接執行label後的code
										fprintf(output, "c.eq.s $f%d, $f%d\n", reg_id, right_child->place);
										fprintf(output, "bc1f %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, (有child==0.0) 就直接執行label後的code
										fprintf(output, "li  $%d, 1\n", int_reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									}
									break;
								}
							case(BINARY_OP_OR): 
								{
									int int_reg_id = get_reg();
									if (int_reg_id != -1) {
										expr_node->place = int_reg_id; 
										int label_no = get_float_compare_label_no();
										fprintf(output, "li  $%d, 1\n", int_reg_id);
										fprintf(output, "li.s $f%d, 0.0", reg_id);
										fprintf(output, "c.eq.s $f%d, $f%d\n", reg_id, left_child->place);
										fprintf(output, "bc1f %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是false, (有child!=0.0) 就直接執行label後的code
										fprintf(output, "c.eq.s $f%d, $f%d\n", reg_id, right_child->place);
										fprintf(output, "bc1f %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是false, (有child!=0.0) 就直接執行label後的code
										fprintf(output, "li  $%d, 0\n", int_reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									}
									break; 
								}
							default: 
								printf("visit_expr出現無法判斷的binary operator\n");
						}
					}
					free_freg(left_child->place);
					free_freg(right_child->place);
					break;
				}
			case(UNARY_OPERATION):
				{
					AST_NODE* child = expr_node->child;
					gen_expr(child);

					int reg_id = get_freg();
					if (reg_id != -1){
						expr_node->place = reg_id;
					
						switch(expr_node->semantic_value.exprSemanticValue.op.unaryOp){
							case(UNARY_OP_POSITIVE):
								fprintf(output, "mov.s  $f%d, $f%d\n", reg_id, child->place);
								break;
							case(UNARY_OP_NEGATIVE):
								fprintf(output, "neg.s  $f%d, $f%d\n", reg_id, child->place);
								break;
							case(UNARY_OP_LOGICAL_NEGATION):
								{
									//如果  是0.0就變成1
									//    不是0.0就變成0
									fprintf(output, "li.s $f%d, 0.0", reg_id);
								
									fprintf(output, "c.eq.s  $f%d, $f%d\n", reg_id, child->place);
									free_freg(reg_id);
									reg_id = get_reg();
									if (reg_id != -1){
										expr_node->place = reg_id;
										int label_no = get_float_compare_label_no();
										fprintf(output, "li  %d, 1\n", reg_id);
										fprintf(output, "bc1t %s%d", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "li  %d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									}
									break;
								}
							default:
								printf("visit_expr出現無法判斷的unary operator\n");
						}
					}
					free_freg(child->place);
					break;
				}
			default:
				printf("visit_expr出現無法判斷是 BINARY 或 UNIARY 的operation\n");
		}
		
	} else {
		printf("visit_expr中出現不支援的dataType\n");
	}
}




void gen_expr(AST_NODE* expr_node)
{
	/*
	 * 本function的功能為幫每個expr node(var, func_call, const)配置運算的reg, 
	 * 還有生成以reg進行運算的mips code
	 * */
	switch (expr_node->nodeType) {
		case(EXPR_NODE):
			visit_expr(expr_node);
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


