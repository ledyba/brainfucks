/*
 * vm_exec.c
 *
 *  Created on: 2008/10/09
 *      Author: PSI
 */


#include <stdio.h>
#include "vm.h"

void op_nop(VM* vm,OPCODE* opcode){
}
void op_pt_inc(VM* vm,OPCODE* opcode){
	vm->ptr++;
}
void op_pt_dec(VM* vm,OPCODE* opcode){
	vm->ptr--;
}
void op_val_inc(VM* vm,OPCODE* opcode){
	(*vm->ptr)++;
}
void op_val_dec(VM* vm,OPCODE* opcode){
	(*vm->ptr)--;
}
void op_out(VM* vm,OPCODE* opcode){
	putchar(*vm->ptr);
}
void op_input(VM* vm,OPCODE* opcode){
	*vm->ptr = getchar();
}
void op_loop_start(VM* vm,OPCODE* opcode){
	if(!*vm->ptr){
		vm->eip =opcode->jump_index;
	}
}
void op_loop_end(VM* vm,OPCODE* opcode){
	if(*vm->ptr){
		vm->eip =opcode->jump_index;
	}
}

void (*op_funcs[])(VM* vm,OPCODE* opcode) = {
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

int execVM(VM* vm){
	OPCODE* opcode;
	vm->eip = 0;
	//printf("max_opcode = %d\n",vm->opcodes_count);
	for(;vm->eip < vm->opcodes_count;vm->eip++){
		opcode = &vm->opcodes[vm->eip];
		//printf("eip:%d opcode:%d jumpidx: %d\n",vm->eip,opcode->type,opcode->jump_index);
		op_funcs[opcode->type](vm,opcode);
	}
	return 0;
}

