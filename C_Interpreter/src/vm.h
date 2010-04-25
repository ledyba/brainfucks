/*
 * vm.h
 *
 *  Created on: 2008/10/09
 *      Author: Account01
 */

#ifndef VM_H_
#define VM_H_

#define OP_TYPE_NOP			0 /* ?? */
#define OP_TYPE_PT_INC		1 /* > */
#define OP_TYPE_PT_DEC		2 /* < */
#define OP_TYPE_VAL_INC		3 /* + */
#define OP_TYPE_VAL_DEC		4 /* - */
#define OP_TYPE_OUT			5 /* . */
#define OP_TYPE_INPUT		6 /* , */
#define OP_TYPE_LOOP_START	7 /* [ */
#define OP_TYPE_LOOP_END	8 /* ] */

typedef struct OPCODE {
	unsigned int type;
	int jump_index;	/* LOOPで使う */
}OPCODE;

#define MEM_SIZE (0x10000)

typedef struct VM {
	unsigned char mem[MEM_SIZE];
	unsigned char* ptr;
	unsigned int opcodes_count;
	unsigned int eip;
	OPCODE* opcodes;
}VM;

int execVM(VM* vm);
VM* makeVM(const char* filename);
void freeVM(VM* vm);

#endif /* VM_H_ */

