#include <stdio.h>
#include <string.h>
#include "header.h"

char *function_name;
FILE *output;


void gen_code(AST_NODE *proj)
{
	AST_Node *nodePtr;
	nodePtr = proj->child;
	output = fopen("output.s", "w");
	
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


