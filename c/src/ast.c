/*
 * AST.c
 *
 *  Created on: 2008/10/09
 *      Author: PSI
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "ast.h"

void compileSctipt(AST* self,const char* filename) {
	FILE* src = fopen(filename, "r");
	if (!src) {
		fprintf(stderr, "Failed to open: %s\n", filename);
		exit(EXIT_FAILURE);
	}
	unsigned int str;

	unsigned int loop_stack[0x100];
	ASTNode* loop_ptr_stack[0x100];
	unsigned int loop_stack_index = 0x100 / 2;

	Vector* vec = Vector_new(sizeof(ASTNode),NULL);
	while ((str = getc(src)) != EOF) {
		ASTNode* opcode;
		switch (str) {
		case '>':
			opcode = Vector_add(vec,NULL);
			opcode->type = AST_TYPE_PT_INC;
			opcode->jump_index = 0;
			break;
		case '<':
			opcode = Vector_add(vec,NULL);
			opcode->type = AST_TYPE_PT_DEC;
			opcode->jump_index = 0;
			break;
		case '+':
			opcode = Vector_add(vec,NULL);
			opcode->type = AST_TYPE_VAL_INC;
			opcode->jump_index = 0;
			break;
		case '-':
			opcode = Vector_add(vec,NULL);
			opcode->type = AST_TYPE_VAL_DEC;
			opcode->jump_index = 0;
			break;
		case '.':
			opcode = Vector_add(vec,NULL);
			opcode->type = AST_TYPE_OUT;
			opcode->jump_index = 0;
			break;
		case ',':
			opcode = Vector_add(vec,NULL);
			opcode->type = AST_TYPE_INPUT;
			opcode->jump_index = 0;
			break;
		case '[':
			opcode = Vector_add(vec,NULL);
			opcode->type = AST_TYPE_LOOP_START;
			loop_stack[loop_stack_index] = Vector_size(vec);
			loop_ptr_stack[loop_stack_index] = opcode;
			loop_stack_index++;
			break;
		case ']':
			opcode = Vector_add(vec,NULL);
			loop_stack_index--;
			opcode->type = AST_TYPE_LOOP_END;
			opcode->jump_index = loop_stack[loop_stack_index]-1;
			loop_ptr_stack[loop_stack_index]->jump_index = Vector_size(vec)-1;
			break;
		default:
			continue;
			break;
		}
	}
	fclose(src);
	self->opcodes_size = Vector_size(vec);
	self->opcodes = Vector_toArray(vec);
	Vector_free(vec,1);
}

AST* AST_new(const char* filename) {
	AST* self = malloc(sizeof(AST));
	compileSctipt(self,filename);
	return self;
}

void AST_free(AST* self) {
	free(self->opcodes);
	free(self);
}

void AST_show(AST* self){
	int i;
	for(i=0;i<self->opcodes_size;i++){
		ASTNode* node = &self->opcodes[i];
		switch(node->type){
		case AST_TYPE_PT_INC:
			printf("#%06d PT_INC\n",i);
			break;
		case AST_TYPE_PT_DEC:
			printf("#%06d PT_DEC\n",i);
			break;
		case AST_TYPE_VAL_INC:
			printf("#%06d VAL_INC\n",i);
			break;
		case AST_TYPE_VAL_DEC:
			printf("#%06d VAL_DEC\n",i);
			break;
		case AST_TYPE_INPUT:
			printf("#%06d INPUT\n",i);
			break;
		case AST_TYPE_OUT:
			printf("#%06d OUTPUT\n",i);
			break;
		case AST_TYPE_LOOP_START:
			printf("#%06d LOOP_START (-> #%06d)\n",i,node->jump_index);
			break;
		case AST_TYPE_LOOP_END:
			printf("#%06d LOOP_END (-> #%06d)\n",i,node->jump_index);
			break;
		case AST_TYPE_NOP:
			printf("#%06d NOP\n",i);
			break;
		default:
			printf("#%06d INVALID_OPERATOR (0x%08x,%08x)\n",i,node->type,node->jump_index);
		}
	}
}

/*
 *  実行関係
 */

void op_nop(ASTMachine* self,ASTNode* node){
}
void op_pt_inc(ASTMachine* self,ASTNode* node){
	self->ptr++;
}
void op_pt_dec(ASTMachine* self,ASTNode* node){
	self->ptr--;
}
void op_val_inc(ASTMachine* self,ASTNode* node){
	(*self->ptr)++;
}
void op_val_dec(ASTMachine* self,ASTNode* node){
	(*self->ptr)--;
}
void op_out(ASTMachine* self,ASTNode* node){
	putchar(*self->ptr);
}
void op_input(ASTMachine* self,ASTNode* node){
	*self->ptr = getchar();
}
void op_loop_start(ASTMachine* self,ASTNode* node){
	if(!*self->ptr){
		self->eip =node->jump_index;
	}
}
void op_loop_end(ASTMachine* self,ASTNode* node){
	if(*self->ptr){
		self->eip =node->jump_index;
	}
}

void (*op_funcs[])(ASTMachine* self,ASTNode* node) = {
		op_nop,
		op_pt_inc,
		op_pt_dec,
		op_val_inc,
		op_val_dec,
		op_out,
		op_input,
		op_loop_start,
		op_loop_end,
};

ASTMachine* ASTMachine_new(AST* ast){
	ASTMachine* self = malloc(sizeof(ASTMachine));
	if(!self){
		fprintf(stderr, "failed to allocate mem for ASTMachine.");
		return 0;
	}
	memset(self,0,sizeof(ASTMachine));
	self->ast = ast;
	self->eip = 0;
	self->ptr = self->memory;
	return self;
}

int _getPowerOfTwo(int code){
	int cnt = 0;
	while(code > 0){
		code >>= 1;
		cnt ++;
	}
	return cnt;
}

void ASTMachine_exec(ASTMachine* self){
	ASTNode* node;
	self->eip = 0;
	//printf("max_opcode = %d\n",vm->opcodes_count);
	for(;self->eip < self->ast->opcodes_size;self->eip++){
		node = &self->ast->opcodes[self->eip];
		//printf("eip:%d opcode:%d jumpidx: %d\n",vm->eip,opcode->type,opcode->jump_index);
		op_funcs[_getPowerOfTwo(node->type)](self,node);
	}
}

void ASTMachine_free(ASTMachine* self){
	free(self);
}
