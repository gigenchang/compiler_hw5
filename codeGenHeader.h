#ifndef LINSHANITY
#define LINSHANITY
#include "symbolTable.h"
#define REG_SIZE 32
#define FRAME_SIZE 64
#define SIZE 4
#define BUFFER_SIZE 128

extern FILE *output;
extern int ARoffset;

void save_value_to_fp(int reg, int offset);
void gen_reg_buffer_code(int offset, int reg);
void dump_buff();
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
void gen_param(AST_NODE* param_node);
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

// stmt generation
void gen_while_stmt(AST_NODE* node);
void gen_if_stmt(AST_NODE* node);
void gen_for_stmt(AST_NODE* node);
void gen_return_stmt(AST_NODE* node);
void gen_relop_expr_list(AST_NODE* node);
void gen_for_relop_expr_list(AST_NODE* node, int exit_number);
void gen_assign_stmt_list(AST_NODE* node);
void gen_stmt(AST_NODE* stmtNode);
void gen_assign_expr_list(AST_NODE* node);

// added by yenchen
void gen_assign_stmt(AST_NODE* assign_stmt_node);
void visit_expr(AST_NODE* expr_node);
void visit_function_call(AST_NODE* func_call_stmt_node);
void visit_var_ref(AST_NODE* id_node);
void visit_const(AST_NODE* const_value);
void gen_read_call(AST_NODE* func_node);
void gen_write_call(AST_NODE* expr);
int get_int_compare_label_no();
int get_float_compare_label_no();
void gen_expr(AST_NODE* expr_node);
void convert_int_to_float(AST_NODE* expr_node);
#endif
