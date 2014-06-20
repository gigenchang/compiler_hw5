#include<stdlib.h>
#include<stdio.h>
#include "header.h"
#include "codeGenHeader.h"
#include "symbolTable.h"

//TODO: 處理reg不夠的問題
//TODO ARRAY ref offset問題

	
int float_compare_label_no = 0;
int int_compare_label_no = 0;
const char FLOAT_COMPARE_LABEL[] = "Float_Compare_Label_";
const char INT_COMPARE_LABEL[] = "Int_Compare_Label_";

int str_label_no = 0;
const char STR_LABEL[] = "str_";

int get_float_compare_label_no()
{
	printf("[get_float_compare_label_no]\n");
	float_compare_label_no += 1;
	return float_compare_label_no;
}

int get_int_compare_label_no()
{
	printf("[get_int_compare_label_no]\n");
	int_compare_label_no += 1;
	return int_compare_label_no;
}

int get_str_label_no()
{
	printf("[get_str_label_no]\n");
	str_label_no += 1;
	return str_label_no;
}

void gen_write_call(AST_NODE* expr)
{
	//expr是要被印出的expression node
	printf("[get_write_call]\n");
	gen_expr(expr);
	int type = expr->dataType;
	switch(type){
		case INT_TYPE:
			fprintf(output, "\tli    $v0, 1\n");
			if(expr->place > 0 && expr->place_type != -1)
				fprintf(output, "\tmove  $a0, $%d\n", expr->place); 
			else{
				gen_reg_buffer_code(expr->place, 24);
				fprintf(output, "\tmove  $a0, $24\n"); 
			}
			fprintf(output, "\tsyscall\n");
			free_reg(expr->place);
			break;
		case FLOAT_TYPE:
			fprintf(output, "\tli    $v0, 2\n");
			if(expr->place > 0 && expr->place_type != -1)
				fprintf(output, "\tmov.s $f12, $f%d\n", expr->place);
			else{
				gen_reg_buffer_code(expr->place, 28);
				fprintf(output, "\tmov.s $f12, $f28\n"); 
			}
			fprintf(output, "\tsyscall\n");
			free_freg(expr->place);
			break;
		case CONST_STRING_TYPE:
			fprintf(output, "\tli    $v0, 4\n");
			fprintf(output, "\tla    $a0, %s%d\n", STR_LABEL, expr->place); //str的label no是紀錄在expr->place中
			fprintf(output, "\tsyscall\n");
			free_reg(expr->place);
			break;
		default:
			printf("未知的write call type\n");
	}	
}


void gen_read_call(AST_NODE* func_node)
{
	printf("[get_read_call]\n");
	switch(func_node->dataType){
		case INT_TYPE:
		{
			int reg_id;
			fprintf(output, "\tli   $v0, 5\n");
			fprintf(output, "\tsyscall\n");
			reg_id = get_reg(); //TODO not implement
			if(reg_id > 0){
				func_node->place = reg_id;
				fprintf(output, "\tmove $%d, $v0\n", reg_id);
			}
			else{
				func_node->place = ARoffset;
				func_node->place_type = -1;
				ARoffset -= 4;
				fprintf(output, "\tmove $25, $v0\n");
				save_value_to_fp(25, func_node->place);
			}
		}	
			break;
		case FLOAT_TYPE:
		{
			int freg_id;
			fprintf(output, "\tli   $v0, 6\n");
			fprintf(output, "\tsyscall\n");
			freg_id = get_freg(); //TODO not implement
			if(freg_id > 0){
				func_node->place = freg_id;
				fprintf(output, "\tmov.s $f%d, $f0\n", freg_id);
			}
			else{
				func_node->place = ARoffset;
				func_node->place_type = -1;
				ARoffset -= 4;
				fprintf(output, "\tmov.s $f28, $f0\n");
				save_value_to_fp(28, func_node->place);
			}
			break;
		}
		case CONST_STRING_TYPE:
			printf("未實作read in string\n");
			break;
		default:
			printf("未知的read call type\n");
	}
}


void visit_const(AST_NODE* const_value)
{
	printf("[visit_const]\n");
	CON_Type *val = const_value->semantic_value.const1;
	
   	switch(val->const_type){
		case INTEGERC:
			{
				int reg_id = get_reg();
				if (reg_id != -1) {
					const_value->place = reg_id;
					fprintf(output, "\tli  $%d, %d\n", reg_id, val->const_u.intval);
				}
				else{
					const_value->place = ARoffset;
					const_value->place_type = -1;
					ARoffset -= 4;
					fprintf(output, "\tli $24, %d\n", val->const_u.intval);
					save_value_to_fp(24, const_value->place);
				}
				break;
			}
		case FLOATC:
			{
				int reg_id = get_freg();
				if (reg_id != -1) {
					const_value->place = reg_id;
					fprintf(output, "\tli.s  $f%d, %f\n", reg_id, val->const_u.fval);
				}
				else{
					const_value->place = ARoffset;
					const_value->place_type = -1;
					ARoffset -= 4;
					fprintf(output, "\tli.s $f28, %f\n", val->const_u.fval);
					save_value_to_fp(28, const_value->place);
				}
				break;
			}
		case STRINGC:
			const_value->place = get_str_label_no();  //!!!注意, str const value我們並不在place內存reg_id, 而是存str_label_no, 方便未來可以透過label取出
			fprintf(output, ".data\n");
			fprintf(output, "%s%d: .asciiz %s\n", STR_LABEL, const_value->place, val->const_u.sc);
			fprintf(output, ".text\n");
			break;
		default:
			printf("未知的gen_load_imm const value type\n");
	} 
}

void visit_var_ref(AST_NODE* id_node)
{
	printf("[visit_var_ref]\n");
	//var可能是global或local
	char* var_name = id_node->semantic_value.identifierSemanticValue.identifierName;
	SymbolTableEntry* entry = id_node->semantic_value.identifierSemanticValue.symbolTableEntry;
	int is_global = (entry->nestingLevel == 0);

	switch(id_node->semantic_value.identifierSemanticValue.kind){
		case(NORMAL_ID):
			switch (id_node->dataType) {
				case INT_TYPE:
					{
						int reg_id = get_reg();
						if (reg_id != -1){
							id_node->place = reg_id;
							if (is_global) {
								fprintf(output, "\tlw  $%d, _%s\n", reg_id, var_name);
							} else {
								fprintf(output, "\tlw  $%d, %d($fp)\n",  reg_id, entry->offset);
							}
						}
						else{
							id_node->place = ARoffset;
							id_node->place_type = -1;
							ARoffset -= 4;
							if (is_global) {
								fprintf(output, "\tlw  $24, _%s\n", var_name);
								save_value_to_fp(24, id_node->place);
							} else {
								fprintf(output, "\tlw  $24, %d($fp)\n", entry->offset);
								save_value_to_fp(24, id_node->place);
							}
						}
						break;
					}
			    case FLOAT_TYPE:
					{
						int reg_id = get_freg();
						if (reg_id != -1){
							id_node->place = reg_id;
							if (is_global) {
								fprintf(output, "\tl.s  $f%d, _%s\n", reg_id, var_name);
							} else {
								fprintf(output, "\tl.s  $f%d, %d($fp)\n",  reg_id, entry->offset);
							}
						}
						else{
							id_node->place = ARoffset;
							id_node->place_type = -1;
							ARoffset -= 4;
							if (is_global) {
								fprintf(output, "\tl.s  $f28, _%s\n", var_name);
								save_value_to_fp(28, id_node->place);
							} else {
								fprintf(output, "\tl.s  $f28, %d($fp)\n", entry->offset);
								save_value_to_fp(28, id_node->place);
							}
						}
						break;
					}
				default:
					printf("visit_var_ref出現不是INT也不是FLOAT的normal id\n");	
			}
			break;
		case(ARRAY_ID):
			{
				ArrayProperties arrayProperty = entry->attribute->attr.typeDescriptor->properties.arrayProperties;

				int current_dimension = 0;
				AST_NODE* current_dimension_node = id_node->child;
				int total_offset_reg = get_reg();
				if (total_offset_reg != -1) {
				} else {
					total_offset_reg = 24;
				}

				fprintf(output, "\tli  $%d,  0\n", total_offset_reg); //先把total offset初始化成0
				while(current_dimension_node != NULL){
					//TODO 存下total offset reg(if total_offset_reg == 24)
					gen_expr(current_dimension_node);
					//TODO load 回 total offset reg(if total_offset_reg == 24)
					//先加上目前維度的值
					if(current_dimension_node->place > 0 && current_dimension_node->place_type !=-1)
						fprintf(output, "\tadd $%d, $%d, $%d\n", total_offset_reg, total_offset_reg, current_dimension_node->place);
					else{
						gen_reg_buffer_code(current_dimension_node->place, 25);
						fprintf(output, "\tadd $%d, $%d, $25\n", total_offset_reg, total_offset_reg);
					}
					free_reg(current_dimension_node->place);
					if (current_dimension_node->rightSibling != NULL) {
						//如果右邊還有維度的話
						int reg_id = get_reg();
						if (reg_id == -1){
							reg_id = 25;
						}
							//load出下一個維度的大小，並乘上去
						fprintf(output, "\tli  $%d, %d\n", reg_id, arrayProperty.sizeInEachDimension[current_dimension+1]);
						fprintf(output, "\tmul $%d, $%d, $%d\n", total_offset_reg, total_offset_reg, reg_id);
						free_reg(reg_id);

					} else {
						//沒有的話就乘以四
						fprintf(output, "\tmul $%d, $%d, 4\n", total_offset_reg, total_offset_reg);
					}
					current_dimension += 1;
					current_dimension_node = current_dimension_node->rightSibling;
				}
				
				switch (id_node->dataType) {
					case INT_TYPE:
						{
							int reg_id = get_reg();
							if (reg_id != -1){
								id_node->place = reg_id;
								if (is_global) {
									fprintf(output, "\tlw  $%d, _%s+%d($%d)\n", reg_id, var_name, entry->offset, total_offset_reg);
								} else {
									fprintf(output, "\tadd $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
									fprintf(output, "\tadd $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
									fprintf(output, "\tlw  $%d, ($%d)\n",  reg_id, total_offset_reg);
								}
							}
							else{
								id_node->place = ARoffset;
								id_node->place_type = -1;
								ARoffset -= 4;
								if (is_global) {
									fprintf(output, "\tlw  $25, _%s+%d($%d)\n", var_name, entry->offset, total_offset_reg);
									save_value_to_fp(25, id_node->place);
								} else {
									fprintf(output, "\tadd $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
									fprintf(output, "\tadd $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
									fprintf(output, "\tlw  $25, ($%d)\n", total_offset_reg);
									save_value_to_fp(25, id_node->place);
								}
							}
							break;
						}
					case FLOAT_TYPE:
						{
							int reg_id = get_freg();
							if (reg_id != -1){
								id_node->place = reg_id;
								if (is_global) {
									fprintf(output, "\tl.s   $f%d, _%s+%d($%d)\n", reg_id, var_name, entry->offset, total_offset_reg);
								} else {
									fprintf(output, "\tadd  $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
									fprintf(output, "\tadd  $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
									fprintf(output, "\tl.s  $f%d, ($%d)\n",  reg_id, total_offset_reg);
								}
							}
							else{
								id_node->place = ARoffset;
								id_node->place_type = -1;
								ARoffset -= 4;
								if (is_global) {
									fprintf(output, "\tl.s   $f30, _%s+%d($%d)\n", var_name, entry->offset, total_offset_reg);
									save_value_to_fp(30, id_node->place);
								} else {
									fprintf(output, "\tadd  $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
									fprintf(output, "\tadd  $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
									fprintf(output, "\tl.s  $f30, ($%d)\n",  total_offset_reg);
									save_value_to_fp(30, id_node->place);
								}
							}
							break;
						}
					default:
						printf("visit_var_ref出現不是INT也不是FLOAT的normal id\n");	
				}
				free_reg(total_offset_reg);
				break;
			}
		default:
			printf("visit_var_ref出現無法判斷為normal或array的id_node\n");
	}
}

void visit_function_call(AST_NODE* func_call_stmt_node)
{
	printf("[visit_function_call]\n");
	AST_NODE* func_name_node = func_call_stmt_node->child;
	AST_NODE* func_para_node = func_name_node->rightSibling;

	char* func_name = func_name_node->semantic_value.identifierSemanticValue.identifierName;
	if(strcmp(func_name, "read") == 0){
		gen_read_call(func_call_stmt_node);
		return;
	}
	else if(strcmp(func_name, "fread") == 0){
		gen_read_call(func_call_stmt_node);
		return;
	}
	switch (func_para_node->nodeType) {
		case(NUL_NODE):
				//無參數的function call
			if(func_call_stmt_node->dataType == INT_TYPE){
				int reg_id = get_reg();
				if(reg_id != -1) {
					func_call_stmt_node->place = reg_id;	
					fprintf(output, "\tjal  %s\n", func_name);
					fprintf(output, "\tmove $%d, $v0\n", reg_id);
				}
				else{
					func_call_stmt_node->place = ARoffset;
					func_call_stmt_node->place_type = -1;
					ARoffset -= 4;
					fprintf(output, "\tjal  %s\n", func_name);
					fprintf(output, "\tmove $24, $v0\n");
					save_value_to_fp(24, func_call_stmt_node->place);
				}
			}
			else if(func_call_stmt_node->dataType == FLOAT_TYPE){
				printf("LLLL\n");
				int freg_id = get_freg();
				if(freg_id != -1) {
					func_call_stmt_node->place = freg_id;	
					fprintf(output, "\tjal  %s\n", func_name);
					fprintf(output, "\tmov.s $f%d, $f0\n", freg_id);
				}
				else{
					func_call_stmt_node->place = ARoffset;
					func_call_stmt_node->place_type = -1;
					ARoffset -= 4;
					fprintf(output, "\tjal  %s\n", func_name);
					fprintf(output, "\tmov.s $f28, $f0\n");
					save_value_to_fp(28, func_call_stmt_node->place);
				}
			}
			else{
				fprintf(output, "\tjal  %s\n", func_name);
				printf("Unexpected function dataType\n");
			}
			break;
		case(NONEMPTY_RELOP_EXPR_LIST_NODE):
			if (!strcmp(func_name, "write")){
				gen_write_call(func_para_node->child); //把參數的expr node傳進去
			}
			else {
				SymbolTableEntry* functionEntry = retrieveSymbol(func_name);
				Parameter* currentParamter = functionEntry->attribute->attr.functionSignature->parameterList; //para list point to first paramter
				
				AST_NODE* func_param = func_para_node->child;
				int param_offset = 0;
				int param_num = func_name_node->semantic_value.identifierSemanticValue.symbolTableEntry->attribute->attr.functionSignature->parametersCount;
				while(func_param != NULL){
					gen_expr(func_param);
					param_offset += 4;
					if(currentParamter->type->properties.dataType == INT_TYPE){
						if(func_param->place >0 && func_param->place_type != -1)
							fprintf(output, "\tsw\t$%d, %d($fp)\n", func_param->place, ARoffset - FRAME_SIZE - param_num * 4 + param_offset - 4);
						else{
							gen_reg_buffer_code(func_param->place, 25);
							fprintf(output, "\tsw\t$25, %d($fp)\n", ARoffset - FRAME_SIZE - param_num * 4 + param_offset - 4);
						}
					}
					else if(currentParamter->type->properties.dataType == FLOAT_TYPE){
						convert_int_to_float(func_param);
						if(func_param->place >0 && func_param->place_type != -1)
							fprintf(output, "\ts.s\t$f%d, %d($fp)\n", func_param->place, ARoffset - FRAME_SIZE - param_num * 4 + param_offset - 4);
						else{
							gen_reg_buffer_code(func_param->place, 25);
							fprintf(output, "\ts.s\t$f25, %d($fp)\n", ARoffset - FRAME_SIZE - param_num * 4 + param_offset - 4);
						}
					}
					else{
						printf("func_param: %d\n", func_param->dataType);
						printf("Unexpected data type\n");
					}
					free_reg(func_param->place);
					func_param = func_param->rightSibling;	
					currentParamter = currentParamter->next;
				}
				fprintf(output,	"\tli\t$24, %d\n", param_offset);
				fprintf(output, "\tsub\t$sp, $sp, $24\n");
				if(func_call_stmt_node->dataType == INT_TYPE){
					int reg_id = get_reg();
					if(reg_id != -1) {
						func_call_stmt_node->place = reg_id;	
						fprintf(output, "\tjal  %s\n", func_name);
						fprintf(output, "\tmove $%d, $v0\n", reg_id);
					}
					else{
						func_call_stmt_node->place = ARoffset;
						func_call_stmt_node->place_type = -1;
						ARoffset -= 4;
						fprintf(output, "\tjal  %s\n", func_name);
						fprintf(output, "\tmove $24, $v0\n");
						save_value_to_fp(24, func_call_stmt_node->place);
					}
				}
				else if(func_call_stmt_node->dataType == FLOAT_TYPE){
					printf("LLLL\n");
					int freg_id = get_freg();
					if(freg_id != -1) {
						func_call_stmt_node->place = freg_id;	
						fprintf(output, "\tjal  %s\n", func_name);
						fprintf(output, "\tmov.s $f%d, $f0\n", freg_id);
					}
					else{
						func_call_stmt_node->place = ARoffset;
						func_call_stmt_node->place_type = -1;
						ARoffset -= 4;
						fprintf(output, "\tjal  %s\n", func_name);
						fprintf(output, "\tmov.s $f28, $f0\n");
						save_value_to_fp(28, func_call_stmt_node->place);
					}
				}
				else{
					fprintf(output, "\tjal  %s\n", func_name);
					printf("Unexpected function dataType\n");
				}
				fprintf(output,	"\tli\t$24, %d\n", param_offset);
				fprintf(output, "\tadd\t$sp, $sp, $24\n");
				break;
			}
			//有參數的function call
			break;
		default:
			printf("visit_function_call: 無法判斷的的function_call參數AST_NODE\n");
	}
}

void turn_offset_into_reg(AST_NODE* expr_node, int reg)
{
	if (expr_node->place < 0) {
		gen_reg_buffer_code(expr_node->place, reg);
		expr_node->place = reg;
	}
}

void convert_int_to_float(AST_NODE* expr_node)
{
	if (expr_node->dataType == INT_TYPE) {
		turn_offset_into_reg(expr_node, 24);

		int freg = get_freg();
		if (freg != -1) {
			fprintf(output, "\tmtc1  $%d, $f%d\n", expr_node->place, freg); //copy reg to freg verbatimly
			fprintf(output, "\tcvt.s.w  $f%d, $f%d\n", freg, freg);          //so need convert
			free_reg(expr_node->place);	
			expr_node->place = freg;
		} else {		
			fprintf(output, "\tmtc1  $%d, $f28\n", expr_node->place); //copy left to freg verbatimly
			fprintf(output, "\tcvt.s.w  $f28, $f28\n");          //so need convert
			free_reg(expr_node->place);	
			expr_node->place = ARoffset;
			expr_node->place_type = -1;
			fprintf(output, "\ts.s  $f28, %d($fp)\n", expr_node->place);
			ARoffset -= 4;
		}
	}
}


void gen_assign_stmt(AST_NODE* assign_stmt_node)
{
	printf("[gen_assign_stmt]\n");
	AST_NODE* left_node = assign_stmt_node->child;
	AST_NODE* right_node = left_node->rightSibling;
	gen_expr(right_node);
	if(left_node->dataType == FLOAT_TYPE){
		convert_int_to_float(right_node);
	}
	
	char* var_name = left_node->semantic_value.identifierSemanticValue.identifierName;
	SymbolTableEntry* entry = left_node->semantic_value.identifierSemanticValue.symbolTableEntry;
	int is_global = (entry->nestingLevel == 0);

	switch(left_node->semantic_value.identifierSemanticValue.kind){
		case(NORMAL_ID):
			switch (left_node->dataType) {
				case INT_TYPE:
					if (is_global) {
						if(right_node->place > 0 && right_node->place_type != -1)
							fprintf(output, "\tsw  $%d, _%s\n", right_node->place, var_name);
						else{
							gen_reg_buffer_code(right_node->place, 24);
							fprintf(output, "\tsw  $24, _%s\n", var_name);
						}
					} else {
						if(right_node->place > 0 && right_node->place_type != -1)
							fprintf(output, "\tsw  $%d, %d($fp)\n",  right_node->place, entry->offset);
						else{
							gen_reg_buffer_code(right_node->place, 24);
							fprintf(output, "\tsw  $24, %d($fp)\n",  entry->offset);
						}
					}
					free_reg(right_node->place);
					break;
			    case FLOAT_TYPE:
					if (is_global) {
						if(right_node->place > 0 && right_node->place_type != -1)
							fprintf(output, "\ts.s  $f%d, _%s\n", right_node->place, var_name);
						else{
							gen_reg_buffer_code(right_node->place, 28);
							fprintf(output, "\ts.s  $f28, _%s\n", var_name);
						}
					} else {
						if(right_node->place > 0 && right_node->place_type != -1)
							fprintf(output, "\ts.s  $f%d, %d($fp)\n",  right_node->place, entry->offset);
						else{
							gen_reg_buffer_code(right_node->place, 28);
							fprintf(output, "\ts.s  $f28, %d($fp)\n", entry->offset);
						}
					}
					free_freg(right_node->place);
					break;
				default:
					printf("visit_var_ref出現不是INT也不是FLOAT的normal id\n");	
			}
			break;
		case(ARRAY_ID):
			{
				ArrayProperties arrayProperty = entry->attribute->attr.typeDescriptor->properties.arrayProperties;

				int current_dimension = 0;
				AST_NODE* current_dimension_node = left_node->child;
				int total_offset_reg = get_reg();
				if (total_offset_reg != -1) {
				} else {
					total_offset_reg = 24;
				}

				fprintf(output, "\tli  $%d, 0\n", total_offset_reg); //先把total offset初始化成0
				while(current_dimension_node != NULL){
					//TODO 如果total_offset = 24,那麼必須先把它存起來
					gen_expr(current_dimension_node);
					//TODO 然後在這裡把它load回去
					
					//先加上目前維度的值
					if(current_dimension_node->place > 0 && current_dimension_node->place_type != -1)
						fprintf(output, "\tadd $%d, $%d, $%d\n", total_offset_reg, total_offset_reg, current_dimension_node->place);
					else{
						gen_reg_buffer_code(current_dimension_node->place, 25);
						fprintf(output, "\tadd $%d, $%d, $25\n", total_offset_reg, total_offset_reg);
					}
					free_reg(current_dimension_node->place);
					if (current_dimension_node->rightSibling != NULL) {
						//如果右邊還有維度的話
						int reg_id = get_reg();
						if (reg_id != -1){
							reg_id = 25;
						}
							//load出下一個維度的大小，並乘上去
							fprintf(output, "\tli  $%d, %d\n", reg_id, arrayProperty.sizeInEachDimension[current_dimension+1]);
							fprintf(output, "\tmul $%d, $%d, $%d\n", total_offset_reg, total_offset_reg, reg_id);
							free_reg(reg_id);
					} else {
						//沒有的話就乘以四
						fprintf(output, "\tmul $%d, $%d, 4\n", total_offset_reg, total_offset_reg);
					}
					current_dimension += 1;
					current_dimension_node = current_dimension_node->rightSibling;
				}
				switch (left_node->dataType) {
					case INT_TYPE:
						if (is_global) {
							if(right_node->place > 0 && right_node->place_type != -1)
								fprintf(output, "\tsw  $%d, _%s+%d($%d)\n", right_node->place, var_name, entry->offset, total_offset_reg);
							else{
								gen_reg_buffer_code(right_node->place, 25);
								fprintf(output, "\tsw  $25, _%s+%d($%d)\n", var_name, entry->offset, total_offset_reg);
							}
						} else {
							if(right_node->place > 0 && right_node->place_type != -1){
								fprintf(output, "\tadd $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
								fprintf(output, "\tadd $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
								fprintf(output, "\tsw  $%d, ($%d)\n",  right_node->place, total_offset_reg);
							}
							else{
								fprintf(output, "\tadd $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
								fprintf(output, "\tadd $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
								gen_reg_buffer_code(right_node->place, 25);
								fprintf(output, "\tsw  $25, ($%d)\n", total_offset_reg);
									
							}
						}
						free_reg(right_node->place);
						break;
					case FLOAT_TYPE:
						if (is_global) {
							if(right_node->place > 0 && right_node->place_type != -1)
								fprintf(output, "\ts.s   $f%d, _%s+%d($%d)\n", right_node->place, var_name, entry->offset, total_offset_reg);
							else{
								gen_reg_buffer_code(right_node->place, 30);
								fprintf(output, "\ts.s   $f30, _%s+%d($%d)\n", var_name, entry->offset, total_offset_reg);
							}
						} else {
							if(right_node->place > 0 && right_node->place_type != -1){
								fprintf(output, "\tadd  $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
								fprintf(output, "\tadd  $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
								fprintf(output, "\ts.s  $f%d, ($%d)\n",  right_node->place, total_offset_reg);
							}
							else{
								fprintf(output, "\tadd  $%d, $%d, $fp\n",  total_offset_reg, total_offset_reg);
								fprintf(output, "\tadd  $%d, $%d, %d\n",  total_offset_reg, total_offset_reg, entry->offset);
								gen_reg_buffer_code(right_node->place, 30);
								fprintf(output, "\tsw  $f30, ($%d)\n", total_offset_reg);	
							}
						}
						free_freg(right_node->place);
						break;
					default:
						printf("visit_var_ref出現不是INT也不是FLOAT的normal id\n");	
				}
				free_reg(total_offset_reg);
				break;
			}
		default:
			printf("visit_var_ref出現無法判斷為normal或array的id_node\n");
	}
}

void visit_expr(AST_NODE* expr_node)
{
	printf("[visit_expr]\n");
	if (expr_node->dataType == INT_TYPE){
		switch(expr_node->semantic_value.exprSemanticValue.kind){
			case(BINARY_OPERATION):
				{
					AST_NODE* left_child = expr_node->child;
					AST_NODE* right_child = left_child->rightSibling;

					int reg_id = get_reg();
					if (reg_id != -1){
						expr_node->place = reg_id;
					} else {
						reg_id = 24;    //reg不夠用，先暫時使用$24來做運算
					}
						switch(expr_node->semantic_value.exprSemanticValue.op.binaryOp){
							case(BINARY_OP_ADD):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tadd  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_SUB):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tsub  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_MUL):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tmul  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_DIV):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tdiv  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_EQ):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tseq  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_GE):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tsge  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_LE):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tsle  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_NE):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tsne  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_GT):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tsgt  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_LT):
								gen_expr(left_child);
								gen_expr(right_child);
								turn_offset_into_reg(left_child, 25);
								turn_offset_into_reg(right_child, 26);
								fprintf(output, "\tslt  $%d, $%d, $%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_AND):
								{
									gen_expr(left_child);
									turn_offset_into_reg(left_child, 25);
									//如果左邊或右邊其中一個 == 0. 那就是0, 反之為1
									int label_no = get_int_compare_label_no();
									fprintf(output, "\tli  $%d, 0\n", reg_id);
									fprintf(output, "\tbeqz $%d, %s%d\n", left_child->place, INT_COMPARE_LABEL, label_no);
									gen_expr(right_child);
									turn_offset_into_reg(right_child, 26);
									fprintf(output, "\tbeqz $%d, %s%d\n", right_child->place, INT_COMPARE_LABEL, label_no);
									fprintf(output, "\tli  $%d, 1\n", reg_id);
									fprintf(output, "%s%d:\n", INT_COMPARE_LABEL, label_no);
									break;
								}
							case(BINARY_OP_OR):
								{
									gen_expr(left_child);
									turn_offset_into_reg(left_child, 25);
									//如果左邊或右邊其中一個 != 0. 那就是1, 反之為0
									int label_no = get_int_compare_label_no();
									fprintf(output, "\tli  $%d, 1\n", reg_id);
									fprintf(output, "\tbnez $%d, %s%d\n", left_child->place, INT_COMPARE_LABEL, label_no);
									gen_expr(right_child);
									turn_offset_into_reg(right_child, 26);
									fprintf(output, "\tbnez $%d, %s%d\n", right_child->place, INT_COMPARE_LABEL, label_no);
									fprintf(output, "\tli  $%d, 0\n", reg_id);
									fprintf(output, "%s%d:\n", INT_COMPARE_LABEL, label_no);
									break;
								}
								break;
							default:
								printf("visit_expr出現無法判斷的binary operator\n");
						}
					//}
					if (reg_id == 24) {
						expr_node->place = ARoffset;
						expr_node->place_type = -1;
						save_value_to_fp(24, expr_node->place);
						ARoffset -= 4;
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
					} else {
						reg_id = 24;	
					}
						switch(expr_node->semantic_value.exprSemanticValue.op.unaryOp){
							case(UNARY_OP_POSITIVE):
								turn_offset_into_reg(child, 25);
								fprintf(output, "\tmove  $%d, $%d\n", reg_id, child->place);
								break;
							case(UNARY_OP_NEGATIVE):
								turn_offset_into_reg(child, 25);
								fprintf(output, "\tneg  $%d, $%d\n", reg_id, child->place);
								break;
							case(UNARY_OP_LOGICAL_NEGATION):
								turn_offset_into_reg(child, 25);
								//如果  是0就變成1
								//    不是0就變成0
								fprintf(output, "\tseq  $%d, $%d, 0\n", reg_id, child->place);
								break;
							default:
								printf("visit_expr出現無法判斷的unary operator\n");
						}
					//}
					if (reg_id == 24) {
						expr_node->place = ARoffset;
						expr_node->place_type = -1;
						save_value_to_fp(24, expr_node->place);
						ARoffset -= 4;
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

					int reg_id = get_freg();
					if (reg_id != -1){
						expr_node->place = reg_id;
					} else {
						reg_id = 26;  //先暫時利用f26來做運算
					}	
					
						switch(expr_node->semantic_value.exprSemanticValue.op.binaryOp){
							case(BINARY_OP_ADD):
								gen_expr(left_child);
								gen_expr(right_child);
								convert_int_to_float(left_child);
								convert_int_to_float(right_child);
								turn_offset_into_reg(left_child, 28);
								turn_offset_into_reg(right_child, 30);
								fprintf(output, "\tadd.s  $f%d, $f%d, $f%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_SUB):
								gen_expr(left_child);
								gen_expr(right_child);
								convert_int_to_float(left_child);
								convert_int_to_float(right_child);
								turn_offset_into_reg(left_child, 28);
								turn_offset_into_reg(right_child, 30);
								fprintf(output, "\tsub.s  $f%d, $f%d, $f%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_MUL):
								gen_expr(left_child);
								gen_expr(right_child);
								convert_int_to_float(left_child);
								convert_int_to_float(right_child);
								turn_offset_into_reg(left_child, 28);
								turn_offset_into_reg(right_child, 30);
								fprintf(output, "\tmul.s  $f%d, $f%d, $f%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_DIV):
								gen_expr(left_child);
								gen_expr(right_child);
								convert_int_to_float(left_child);
								convert_int_to_float(right_child);
								turn_offset_into_reg(left_child, 28);
								turn_offset_into_reg(right_child, 30);
								fprintf(output, "\tdiv.s  $f%d, $f%d, $f%d\n", reg_id, left_child->place, right_child->place);
								break;
							case(BINARY_OP_EQ):
								{
									gen_expr(left_child);
									gen_expr(right_child);
									convert_int_to_float(left_child);
									convert_int_to_float(right_child);
									free_freg(reg_id); //把float reg還回去
									reg_id = get_reg(); //重新要一個一般的reg

									if (reg_id != -1){
										expr_node->place = reg_id;
									} else {
										reg_id = 24;  //先暫時用$24來做運算
									}
										turn_offset_into_reg(left_child, 28);
										turn_offset_into_reg(right_child, 30);
										int label_no = get_float_compare_label_no();
										fprintf(output, "\tc.eq.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "\tli  $%d, 1\n", reg_id);
										fprintf(output, "\tbc1t %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "\tli  $%d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									//}
									if (reg_id == 24) {
										expr_node->place = ARoffset;
										expr_node->place_type = -1;
										save_value_to_fp(24, expr_node->place);
										ARoffset -= 4;
									}
									break;
								}
							case(BINARY_OP_GE):
								{
									gen_expr(left_child);
									gen_expr(right_child);
									convert_int_to_float(left_child);
									convert_int_to_float(right_child);
									//>= is not <
									free_freg(reg_id);
									reg_id = get_reg();

									if (reg_id != -1) {
										expr_node->place = reg_id;
									} else {
										reg_id = 24;  //先暫時用$24來做運算
									}
										turn_offset_into_reg(left_child, 28);
										turn_offset_into_reg(right_child, 30);
										int label_no = get_float_compare_label_no();
										fprintf(output, "\tc.lt.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "\tli  $%d, 1\n", reg_id);
										fprintf(output, "\tbc1f %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "\tli  $%d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									//}
									if (reg_id == 24) {
										expr_node->place = ARoffset;
										expr_node->place_type = -1;
										save_value_to_fp(24, expr_node->place);
										ARoffset -= 4;
									}
									break;
								}
							case(BINARY_OP_LE):
								{
									gen_expr(left_child);
									gen_expr(right_child);
									convert_int_to_float(left_child);
									convert_int_to_float(right_child);
									free_freg(reg_id);
									reg_id = get_reg();
									if (reg_id != -1) {
										expr_node->place = reg_id;
									} else {
										reg_id = 24;  //先暫時用$24來做運算
									}
										turn_offset_into_reg(left_child, 28);
										turn_offset_into_reg(right_child, 30);
										int label_no = get_float_compare_label_no();
										fprintf(output, "\tc.le.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "\tli  $%d, 1\n", reg_id);
										fprintf(output, "\tbc1t %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "\tli  $%d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									//}
									if (reg_id == 24) {
										expr_node->place = ARoffset;
										expr_node->place_type = -1;
										save_value_to_fp(24, expr_node->place);
										ARoffset -= 4;
									}
									break;
								}
							case(BINARY_OP_NE):
								{
									gen_expr(left_child);
									gen_expr(right_child);
									convert_int_to_float(left_child);
									convert_int_to_float(right_child);
									//!= is not ==
									free_freg(reg_id);
									reg_id = get_reg();
									if (reg_id != -1) {
										expr_node->place = reg_id;
									} else {
										reg_id = 24;
									}
										turn_offset_into_reg(left_child, 28);
										turn_offset_into_reg(right_child, 30);
										int label_no = get_float_compare_label_no();
										fprintf(output, "\tc.eq.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "\tli  $%d, 1\n", reg_id);
										fprintf(output, "\tbc1f %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "\tli  $%d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									//}
									if (reg_id == 24) {
										expr_node->place = ARoffset;
										expr_node->place_type = -1;
										save_value_to_fp(24, expr_node->place);
										ARoffset -= 4;
									}
									break;
								}
							case(BINARY_OP_GT):
								{
									gen_expr(left_child);
									gen_expr(right_child);
									convert_int_to_float(left_child);
									convert_int_to_float(right_child);
									//> is !<=
									free_freg(reg_id);
									reg_id = get_reg();
									if (reg_id != -1) {
										expr_node->place = reg_id;
									} else {
										reg_id = 24;
									}
										turn_offset_into_reg(left_child, 28);
										turn_offset_into_reg(right_child, 30);
										int label_no = get_float_compare_label_no();
										fprintf(output, "\tc.le.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "\tli  $%d, 1\n", reg_id);
										fprintf(output, "\tbc1f %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "\tli  $%d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									//}
									if (reg_id == 24) {
										expr_node->place = ARoffset;
										expr_node->place_type = -1;
										save_value_to_fp(24, expr_node->place);
										ARoffset -= 4;
									}
									break;
								}
							case(BINARY_OP_LT):
								{
									gen_expr(left_child);
									gen_expr(right_child);
									convert_int_to_float(left_child);
									convert_int_to_float(right_child);
									free_freg(reg_id);
									reg_id = get_reg();

									if (reg_id != -1) {
										expr_node->place = reg_id;
									} else {
										reg_id = 24;
									}
										turn_offset_into_reg(left_child, 28);
										turn_offset_into_reg(right_child, 30);
										int label_no = get_float_compare_label_no();
										fprintf(output, "\tc.lt.s  $f%d, $f%d\n", left_child->place, right_child->place);
										fprintf(output, "\tli  $%d, 1\n", reg_id);
										fprintf(output, "\tbc1t %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "\tli  $%d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									//}
									if (reg_id == 24) {
										expr_node->place = ARoffset;
										expr_node->place_type = -1;
										save_value_to_fp(24, expr_node->place);
										ARoffset -= 4;
									}
									break;
								}
							case(BINARY_OP_AND):
								{
									//如果左邊或右邊其中一個 == 0.0 那就是0, 反之為1
									int int_reg_id = get_reg();
									if (int_reg_id != -1) {
										expr_node->place = int_reg_id; 
									} else {
										int_reg_id = 24;
									}
										gen_expr(left_child);
										convert_int_to_float(left_child);
										turn_offset_into_reg(left_child, 28);
										int label_no = get_float_compare_label_no();
										fprintf(output, "\tli  $%d, 0\n", int_reg_id);
										fprintf(output, "\tli.s $f%d, 0.0\n", reg_id);
										fprintf(output, "\tc.eq.s $f%d, $f%d\n", reg_id, left_child->place);
										fprintf(output, "\tbc1f %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, (有child==0.0) 就直接執行label後的code
										gen_expr(right_child);
										convert_int_to_float(right_child);
										turn_offset_into_reg(right_child, 30);
										fprintf(output, "\tc.eq.s $f%d, $f%d\n", reg_id, right_child->place);
										fprintf(output, "\tbc1f %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, (有child==0.0) 就直接執行label後的code
										fprintf(output, "\tli  $%d, 1\n", int_reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									//}
									if (int_reg_id == 24) {
										expr_node->place = ARoffset;
										expr_node->place_type = -1;
										save_value_to_fp(24, expr_node->place);
										ARoffset -= 4;
									}
									break;
								}
							case(BINARY_OP_OR): 
								{
									int int_reg_id = get_reg();
									if (int_reg_id != -1) {
										expr_node->place = int_reg_id; 
									} else {
										int_reg_id = 24;
									}
										gen_expr(left_child);
										convert_int_to_float(left_child);
										turn_offset_into_reg(left_child, 28);
										int label_no = get_float_compare_label_no();
										fprintf(output, "\tli  $%d, 1\n", int_reg_id);
										fprintf(output, "\tli.s $f%d, 0.0\n", reg_id);
										fprintf(output, "\tc.eq.s $f%d, $f%d\n", reg_id, left_child->place);
										fprintf(output, "\tbc1f %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是false, (有child!=0.0) 就直接執行label後的code
										gen_expr(right_child);
										convert_int_to_float(right_child);
										turn_offset_into_reg(right_child, 30);
										fprintf(output, "\tc.eq.s $f%d, $f%d\n", reg_id, right_child->place);
										fprintf(output, "\tbc1f %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是false, (有child!=0.0) 就直接執行label後的code
										fprintf(output, "\tli  $%d, 0\n", int_reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									//}
									if (int_reg_id == 24) {
										expr_node->place = ARoffset;
										expr_node->place_type = -1;
										save_value_to_fp(24, expr_node->place);
										ARoffset -= 4;
									}
									break; 
								}
							default: 
								printf("visit_expr出現無法判斷的binary operator\n");
						}
					//}
					if (reg_id == 26) {
						expr_node->place = ARoffset;
						expr_node->place_type = -1;
						save_value_to_fp(26, expr_node->place);
						ARoffset -= 4;
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
					} else {
						reg_id = 26;  //先暫時利用f30來做運算
					}	
					
						switch(expr_node->semantic_value.exprSemanticValue.op.unaryOp){
							case(UNARY_OP_POSITIVE):
								turn_offset_into_reg(child, 28);
								fprintf(output, "\tmov.s  $f%d, $f%d\n", reg_id, child->place);
								break;
							case(UNARY_OP_NEGATIVE):
								turn_offset_into_reg(child, 28);
								fprintf(output, "\tneg.s  $f%d, $f%d\n", reg_id, child->place);
								break;
							case(UNARY_OP_LOGICAL_NEGATION):
								{
									//如果  是0.0就變成1
									//    不是0.0就變成0
									fprintf(output, "\tli.s $f%d, 0.0\n", reg_id);
								
									turn_offset_into_reg(child, 28);
									fprintf(output, "\tc.eq.s  $f%d, $f%d\n", reg_id, child->place);
									free_freg(reg_id);
									reg_id = get_reg();
									if (reg_id != -1){
										expr_node->place = reg_id;
									} else {
										reg_id = 24;
									}
										int label_no = get_float_compare_label_no();
										fprintf(output, "\tli  %d, 1\n", reg_id);
										fprintf(output, "\tbc1t %s%d\n", FLOAT_COMPARE_LABEL, label_no); //如果結果是true, 就直接執行label後的code
										fprintf(output, "\tli  %d, 0\n", reg_id);
										fprintf(output, "%s%d:\n", FLOAT_COMPARE_LABEL, label_no);
									//}
									if (reg_id == 24) {
										expr_node->place = ARoffset;
										expr_node->place_type = -1;
										save_value_to_fp(24, expr_node->place);
										ARoffset -= 4;
									}
									
									break;
								}
							default:
								printf("visit_expr出現無法判斷的unary operator\n");
						}
					//}
					if (reg_id == 26) {
						expr_node->place = ARoffset;
						expr_node->place_type = -1;
						save_value_to_fp(26, expr_node->place);
						ARoffset -= 4;
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
	printf("[gen_expr]\n");
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
				printf("gen_expr中出現無法判斷的STMT_NODE type\n");
			}
			break;
		default:
			printf("gen_expr出現無法判斷的node type\n");
	}
}


