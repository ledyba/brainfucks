/*
 * middle.c
 *
 *  Created on: 2010/12/04
 *      Author: psi
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "middle.h"
#include "vector.h"


int _consumeAST(AST* ast, unsigned int idx, unsigned char* type, int* cnt){
	if((idx+*cnt) >= AST_length(ast)){
		*cnt = 0;
		*type = AST_TYPE_NOP;
		return 0;
	}
	*type = AST_readNode(ast,idx)->type;
	*cnt = 1;
	if(((*type) & (AST_TYPE_OUT | AST_TYPE_INPUT | AST_TYPE_LOOP_START | AST_TYPE_LOOP_END)) != 0 ){
		return (idx+*cnt) < AST_length(ast);
	}
	while((idx+*cnt) < AST_length(ast) && AST_readNode(ast,idx+*cnt)->type == *type){
		(*cnt)++;
	}
	return (idx+*cnt) < AST_length(ast);
}

MiddleLoop* _MiddleLoop_new(AST* ast, unsigned int* idx){
	MiddleLoop* self = malloc(sizeof(MiddleLoop));
	Vector* vector = Vector_new(sizeof(MiddleInst),NULL);
	if (!self) {
		fprintf(stderr, "[%s][%d] Failed to malloc.\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}

	int running = 1;
	//ASTとMiddleの変換
	while(running){
		int ast_cnt = 0;
		unsigned char ast_type = AST_TYPE_NOP;
		//ASTを消費する
		running = _consumeAST(ast,*idx,&ast_type,&ast_cnt);
		*idx += ast_cnt;
		//消費した結果を元に中間形式を作成
		MiddleInst* inst;
		switch(ast_type){
		case AST_TYPE_NOP:
			continue;
		case AST_TYPE_PT_INC:
			inst = Vector_add(vector,NULL);
			inst->type = MIDDLE_INST_PTR;
			inst->operand.value=ast_cnt;
			break;
		case AST_TYPE_PT_DEC:
			inst = Vector_add(vector,NULL);
			inst->type = MIDDLE_INST_PTR;
			inst->operand.value=-ast_cnt;
			break;
		case AST_TYPE_VAL_INC:
			inst = Vector_add(vector,NULL);
			inst->type = MIDDLE_INST_VAL;
			inst->operand.value=ast_cnt;
			break;
		case AST_TYPE_VAL_DEC:
			inst = Vector_add(vector,NULL);
			inst->type = MIDDLE_INST_VAL;
			inst->operand.value=-ast_cnt;
			break;
		case AST_TYPE_OUT:
			inst = Vector_add(vector,NULL);
			inst->type = MIDDLE_INST_OUTPUT;
			break;
		case AST_TYPE_INPUT:
			inst = Vector_add(vector,NULL);
			inst->type = MIDDLE_INST_INPUT;
			break;
		case AST_TYPE_LOOP_START:
			inst = Vector_add(vector,NULL);
			inst->type = MIDDLE_INST_LOOP;
			inst->operand.loop = _MiddleLoop_new(ast,idx);
			self->has_loop++;
			break;
		case AST_TYPE_LOOP_END:
			running = 0;
			break;
		default:
			fprintf(stderr,"Invalid operator: #%06d: %08x",*idx,ast_type);
			break;
		}
	}
	self->inst = Vector_toArray(vector);
	self->inst_size = Vector_length(vector);
	Vector_free(vector,1);
	return self;
}

Middle* Middle_new(AST* ast){
	Middle* self = malloc(sizeof(Middle));
	if (!self) {
		fprintf(stderr, "[%s][%d] Failed to malloc.\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	unsigned int idx = 0;
	self->main_loop = _MiddleLoop_new(ast,&idx);
	if (idx < AST_length(ast)) {
		fprintf(stderr, "[Warning] Failed to convert AST to Middle.\n");
	}

	return self;
}

void _MiddleLoop_free(MiddleLoop* self){
	unsigned int i;
	for(i=0;i<self->inst_size;i++){
		if(MIDDLE_INST_LOOP == self->inst[i].type){
			_MiddleLoop_free(self->inst[i].operand.loop);
		}
	}
	free(self);
}
void Middle_free(Middle* self){
	_MiddleLoop_free(self->main_loop);
	free(self);
}

void _MiddleLoop_show(MiddleLoop* self, int depth){
	int i;
	int j;
	for(j=0;j<depth;j++){
		printf("\t");
	}
	printf("** depth: %d size: %d **\n",depth,self->inst_size);
	for(i=0;i<self->inst_size;i++){
		MiddleInst* inst = &self->inst[i];
		for(j=0;j<depth;j++){
			printf("\t");
		}
		switch (inst->type) {
		case MIDDLE_INST_VAL:
			printf("#%06d VAL %d\n",i,inst->operand.value);
			break;
		case MIDDLE_INST_PTR:
			printf("#%06d PTR %d\n",i,inst->operand.value);
			break;
		case MIDDLE_INST_INPUT:
			printf("#%06d INPUT\n",i);
			break;
		case MIDDLE_INST_OUTPUT:
			printf("#%06d OUTPUT\n",i);
			break;
		case MIDDLE_INST_LOOP:
			printf("#%06d LOOP\n",i);
			_MiddleLoop_show(inst->operand.loop,depth+1);
			break;
		default:
			printf("#%06d INVALID: %d\n",i,inst->type);
			break;
		}
	}
	for(j=0;j<depth;j++){
		printf("\t");
	}
	printf("** end **\n");
}
void Middle_show(Middle* self){
	_MiddleLoop_show(self->main_loop,0);
}

//-------------------------------------------------------------------------------

MiddleMachine* MiddleMachine_new(Middle* middle){
	MiddleMachine* self = malloc(sizeof(MiddleMachine));
	if (!self) {
		fprintf(stderr, "[%s][%d] Failed to malloc.\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	self->src = middle;
	return self;
}
void MiddleMachine_free(MiddleMachine* self){
	free(self);
}

void _MiddleMachine_run(MiddleMachine* self, MiddleLoop* loop){
	int i;
	for(i=0;i<loop->inst_size;i++){
		MiddleInst* inst = &loop->inst[i];
		switch (inst->type) {
		case MIDDLE_INST_VAL:
			self->mem[self->mem_ptr] +=inst->operand.value;
			break;
		case MIDDLE_INST_PTR:
			self->mem_ptr +=inst->operand.value;
			break;
		case MIDDLE_INST_INPUT:
			self->mem[self->mem_ptr] = getchar();
			break;
		case MIDDLE_INST_OUTPUT:
			putchar(self->mem[self->mem_ptr]);
			break;
		case MIDDLE_INST_LOOP:
			while(self->mem[self->mem_ptr]){
				_MiddleMachine_run(self, inst->operand.loop);
			}
			break;
		default:
			break;
		}
	}
}

void MiddleMachine_run(MiddleMachine* self){
	self->loop_stack_depth=0;
	self->mem_ptr=0;
	memset(self->mem,0,MIDDLE_MEM_MAX);
	_MiddleMachine_run(self,self->src->main_loop);
}
