/*
 * vm.c
 *
 *  Created on: 2008/10/09
 *      Author: PSI
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

/* ここでしか使わない構造体なので、ここで定義しちゃう。 */

typedef struct LINKED_ITEM LINKED_ITEM;
struct LINKED_ITEM{
	OPCODE opcode;
	LINKED_ITEM* next;
};

typedef struct LINKED_LIST {
	LINKED_ITEM* first;
	unsigned int cnt;
}LINKED_LIST;

/* 以下関数定義 */

void convertLinkedList(VM* vm,LINKED_LIST* list) {
	OPCODE* opcodes = malloc(list->cnt * sizeof(OPCODE));
	if (!opcodes) {
		fprintf(stderr, "Failed to malloc.\n");
		exit(EXIT_FAILURE);
	}
	int i;
	LINKED_ITEM* item = list->first;
	for(i=0;i<list->cnt;i++){
		memcpy(&opcodes[i],&item->opcode,sizeof(OPCODE));
		LINKED_ITEM* tmp = item;
		item = tmp->next;
		free(tmp);
	}
	vm->opcodes = opcodes;
	vm->opcodes_count = list->cnt;
}

void compileSctipt(VM* vm,const char* filename) {
	FILE* src = fopen(filename, "r");
	if (!src) {
		fprintf(stderr, "Failed to open: %s\n", filename);
		exit(EXIT_FAILURE);
	}
	unsigned int str;
	unsigned int loop_stack[0x100];
	OPCODE* loop_ptr_stack[0x100];
	unsigned int loop_stack_index = 0x100 / 2;
	LINKED_ITEM* item = malloc(sizeof(LINKED_ITEM));
	if (!item) {
		fprintf(stderr, "Failed to malloc.\n");
		exit(EXIT_FAILURE);
	}
	LINKED_LIST list;
	list.cnt = 0;
	list.first = item;
	while ((str = getc(src)) != EOF) {
		OPCODE* opcode = &item->opcode;
		switch (str) {
		case '>':
			opcode->type = OP_TYPE_PT_INC;
			opcode->jump_index = 0;
			break;
		case '<':
			opcode->type = OP_TYPE_PT_DEC;
			opcode->jump_index = 0;
			break;
		case '+':
			opcode->type = OP_TYPE_VAL_INC;
			opcode->jump_index = 0;
			break;
		case '-':
			opcode->type = OP_TYPE_VAL_DEC;
			opcode->jump_index = 0;
			break;
		case '.':
			opcode->type = OP_TYPE_OUT;
			opcode->jump_index = 0;
			break;
		case ',':
			opcode->type = OP_TYPE_INPUT;
			opcode->jump_index = 0;
			break;
		case '[':
			opcode->type = OP_TYPE_LOOP_START;
			loop_stack[loop_stack_index] = list.cnt;
			loop_ptr_stack[loop_stack_index] = opcode;
			loop_stack_index++;
			break;
		case ']':
			loop_stack_index--;
			opcode->type = OP_TYPE_LOOP_END;
			opcode->jump_index = loop_stack[loop_stack_index]-1;
			loop_ptr_stack[loop_stack_index]->jump_index = list.cnt;
			break;
		default:
			continue;
			break;
		}
		LINKED_ITEM* tmp = malloc(sizeof(LINKED_ITEM));
		if (!tmp) {
			fprintf(stderr, "Failed to malloc.\n");
			exit(EXIT_FAILURE);
		}
		item->next = tmp;
		item = tmp;
		item->next = 0;
		list.cnt++;
	}
	fclose(src);
	convertLinkedList(vm,&list);
}

VM* makeVM(const char* filename) {
	VM* vm = malloc(sizeof(VM));
	memset(vm->mem, 0x00, MEM_SIZE);
	vm->ptr = &vm->mem[MEM_SIZE / 2];
	vm->eip = 0;
	compileSctipt(vm,filename);
	return vm;
}

void freeVM(VM* vm) {
	free(vm->opcodes);
	free(vm);
}

