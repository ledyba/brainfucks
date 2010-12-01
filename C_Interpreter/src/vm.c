/*
 * vm.c
 *
 *  Created on: 2010/11/28
 *      Author: psi
 */
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "mem.h"
#include "vm.h"

/**
 * ASTコンパイル後のサイズを返す
 */

int VM_comsumeAST(AST* ast, unsigned int* now_ptr,unsigned int *opcode, unsigned int* operand){
	if((*now_ptr) >= AST_length(ast)){
		return 0;
	}
	ASTNode* node = AST_readNode(ast,(*now_ptr)++ );
	int collect = 0;
	int cnt = 0;
	switch(ASTNode_type(node)){
	case AST_TYPE_INPUT:
		*opcode = VM_OP_INPUT;
		break;
	case AST_TYPE_OUT:
		*opcode = VM_OP_OUTPUT;
		break;
	case AST_TYPE_LOOP_START:
		*opcode = VM_OP_JMP;
		break;
	case AST_TYPE_LOOP_END:
		*opcode = VM_OP_JMPEND;
		break;
	case AST_TYPE_PT_INC:
		*opcode = VM_OP_FORWARD;
		collect = 1;
		cnt++;
		break;
	case AST_TYPE_PT_DEC:
		*opcode = VM_OP_FORWARD;
		collect = 1;
		cnt--;
		break;
	case AST_TYPE_VAL_INC:
		*opcode = VM_OP_ADD;
		collect = 1;
		cnt++;
		break;
	case AST_TYPE_VAL_DEC:
		*opcode = VM_OP_ADD;
		collect = 1;
		cnt--;
		break;
	default:
		fprintf(stderr, __FILE__": invalid op: %02x\n",ASTNode_type(node));
		*opcode = VM_OP_NOP;
		break;
	}
	while(*now_ptr < AST_length(ast) && collect){
		node = AST_readNode(ast,*now_ptr);
		int type = ASTNode_type(node);
		if(*opcode == VM_OP_ADD){
			switch(type){
			case AST_TYPE_VAL_INC:
				cnt++;
				break;
			case AST_TYPE_VAL_DEC:
				cnt--;
				break;
			default:
				goto end;
			}
		}else if(*opcode == VM_OP_FORWARD){
			switch(type){
			case AST_TYPE_PT_INC:
				cnt++;
				break;
			case AST_TYPE_PT_DEC:
				cnt--;
				break;
			default:
				goto end;
			}
		}else{
			goto end;
		}
		(*now_ptr)++;
	}
	end:
	*operand = cnt;
	return *now_ptr <= AST_length(ast);
}

size_t VM_convertASTtoByteCode(VM* self, AST* ast){
	size_t jmp_stack[1000];
	size_t jmp_stack_idx = 0;
	size_t opcode_length = 0;
	unsigned int now_ptr = 0;
	unsigned int opcode = 0;
	unsigned int operand = 0;
	while(VM_comsumeAST(ast,&now_ptr,&opcode,&operand)){
		if(opcode == VM_OP_JMP){
			jmp_stack[jmp_stack_idx] = opcode_length;
			jmp_stack_idx++;
		}else if(opcode == VM_OP_JMPEND){
			if(jmp_stack_idx <= 0){
				fprintf(stderr, __FILE__ " invalid source. ] is too much.\n");
			}
			jmp_stack_idx--;
			//JMP前
			MemController_writeShort(self->Mem,
					jmp_stack[jmp_stack_idx],
					((opcode_length) << VM_OP_BIT) | VM_OP_JMP
					);
			//JMP後
			MemController_writeShort(self->Mem,
					opcode_length,
					(jmp_stack[jmp_stack_idx] << VM_OP_BIT) | VM_OP_JMPEND
					);
		}else{
			short inst = (operand << VM_OP_BIT) | opcode;
			MemController_writeShort(self->Mem,opcode_length,inst);
		}
		opcode_length+=VM_OP_LENGTH;
	}
	return opcode_length;
}

VM* VM_new(AST* ast,size_t factor){
	VM* self = malloc(sizeof(VM));
	self->Mem = MemController_new(factor, VM_MEM);
	self->PC = 0;
	self->Ptr = self->EndPC = VM_convertASTtoByteCode(self,ast);
	return self;
}
void VM_free(VM* self){
	MemController_free(self->Mem);
	free(self);
}

void VM_show(VM* self){
	self->PC = 0;
	self->Ptr = self->EndPC;
	while(self->PC < self->EndPC){
		unsigned short inst = MemController_readShort(self->Mem, self->PC);
		unsigned short operand = inst >> VM_OP_BIT;
		switch(inst & VM_OP_MASK){
		case VM_OP_ADD:
			printf("%04d / ADD %02x\n", self->PC, operand);
			break;
		case VM_OP_FORWARD:
			printf("%04d / FORWARD %02x\n", self->PC, operand);
			break;
		case VM_OP_JMP:
			printf("%04d / JMP %d\n", self->PC, operand);
			break;
		case VM_OP_JMPEND:
			printf("%04d / JMPEND %d\n",self->PC, operand);
			break;
		case VM_OP_INPUT:
			printf("%04d / IN\n",self->PC);
			break;
		case VM_OP_OUTPUT:
			printf("%04d / OUT\n",self->PC);
			break;
		default:
			fprintf(stderr, __FILE__"(%d): invalid instruction:%02x (%dth)\n",__LINE__,inst & VM_OP_MASK,self->PC);
		}
		self->PC+=VM_OP_LENGTH;
	}
}

void VM_run(VM* self){
	self->PC = 0;
	self->Ptr = self->EndPC;
	MemController_clear(self->Mem, self->EndPC, 0);
	while(self->PC < self->EndPC){
		unsigned short inst = MemController_readShort(self->Mem, self->PC);
		unsigned int operand = inst >> VM_OP_BIT;
		switch(inst & VM_OP_MASK){
		case VM_OP_ADD:
		{
#ifdef DEBUG
			printf("%04d / ADD %02x Ptr: %04d\n", self->PC, operand, self->Ptr);
#endif
			unsigned short tmp = MemController_readByte(self->Mem, self->Ptr);
			if(operand & 0100){
				operand |= 0xfffffe00;
			}
			tmp = tmp + operand;
			MemController_writeByte(self->Mem,self->Ptr,(unsigned char)tmp);
		}
			break;
		case VM_OP_FORWARD:
#ifdef DEBUG
			printf("%04d / FORWARD %02x Ptr: %04d\n", self->PC, operand, self->Ptr);
#endif
			if(operand & 0100){
				operand |= 0xfffffe00;
			}
			self->Ptr = self->Ptr + operand;
			break;
		case VM_OP_JMP:
		{
#ifdef DEBUG
			printf("%04d / JMP %d\n", self->PC, operand);
#endif
			unsigned short tmp = MemController_readByte(self->Mem, self->Ptr);
			if(!tmp){
				self->PC = operand;
			}
		}
			break;
		case VM_OP_JMPEND:
		{
#ifdef DEBUG
			printf("%04d / JMPEND %d\n",self->PC, operand);
#endif
			unsigned short tmp = MemController_readByte(self->Mem, self->Ptr);
			if(tmp){
				self->PC = operand;
			}
		}
			break;
		case VM_OP_INPUT:
#ifdef DEBUG
			printf("%04d / IN\n",self->PC);
#endif
//			MemController_writeByte(self->Mem, self->Ptr, getchar());
			break;
		case VM_OP_OUTPUT:
#ifdef DEBUG
			printf("%04d / OUT\n",self->PC);
#endif
//			putchar(MemController_readByte(self->Mem, self->Ptr));
			break;
		default:
			fprintf(stderr, __FILE__"(%d): invalid instruction:%02x (%dth)\n",__LINE__,inst & VM_OP_MASK,self->PC);
		}
		self->PC+=VM_OP_LENGTH;
#ifdef DEBUG
		fflush(stdout);
#endif
	}
}
