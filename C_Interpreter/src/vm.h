/*
 * vm.h
 *
 *  Created on: 2010/11/28
 *      Author: psi
 */

#ifndef VM_H_
#define VM_H_
#include "mem.h"
#include "ast.h"

#define VM_MEM (32*1024)

#define VM_OP_MASK (0x7)
#define VM_OP_BIT (3)

#define VM_OP_NOP (0)
#define VM_OP_ADD (1)
#define VM_OP_FORWARD (2)
#define VM_OP_JMP (3)
#define VM_OP_JMPEND (4)
#define VM_OP_INPUT (5)
#define VM_OP_OUTPUT (6)

#define VM_OP_LENGTH (sizeof(unsigned short))

typedef struct VM{
	MemController* Mem;
	unsigned int Ptr;
	unsigned int PC;
	unsigned int EndPC;
}VM;

VM* VM_new(AST* ast,size_t factor);
void VM_free(VM* self);
void VM_run(VM* self);
void VM_show(VM* self);


#endif /* VM_H_ */
