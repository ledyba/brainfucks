/*
 * middle.h
 * 中間形式。ここで最適化等を行う。
 *
 *  Created on: 2010/12/04
 *      Author: psi
 */

#ifndef MIDDLE_H_
#define MIDDLE_H_
#include "ast.h"

typedef struct Middle Middle;
typedef struct MiddleLoop MiddleLoop;
typedef struct MiddleInst MiddleInst;

///

#define MIDDLE_INST_NOP (0)
#define MIDDLE_INST_PTR (1)
#define MIDDLE_INST_VAL (2)
#define MIDDLE_INST_INPUT (4)
#define MIDDLE_INST_OUTPUT (8)
#define MIDDLE_INST_LOOP (16)

struct MiddleInst{
	unsigned char type;
	union operand{
		int value;
		MiddleLoop* loop;
	} operand;
};

struct MiddleLoop{
	unsigned int inst_size;
	MiddleInst* inst;
	//以下、最適化のための情報
	int has_loop;
};

struct Middle{
	MiddleLoop* main_loop;
};

Middle* Middle_new(AST* ast);
void Middle_free(Middle* self);
void Middle_show(Middle* self);

///実行用

#define MIDDLE_LOOP_STACK_MAX (128)
#define MIDDLE_MEM_MAX (128*1024)
typedef struct{
	Middle* src;
	int loop_stack[MIDDLE_LOOP_STACK_MAX];
	int loop_stack_depth;
	int mem_ptr;
	char mem[MIDDLE_MEM_MAX];
} MiddleMachine;

MiddleMachine* MiddleMachine_new(Middle* middle);
void MiddleMachine_free(MiddleMachine* self);
void MiddleMachine_run(MiddleMachine* self);

#endif /* MIDDLE_H_ */
