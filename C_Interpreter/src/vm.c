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

int VM_comsumeAST(AST* ast, VmPtr* now_ptr,VmOp *opcode, VmOp* operand){
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
	VmPtr now_ptr = 0;
	VmOp opcode = 0;
	VmOp operand = 0;
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
			*((VmOp*)(&(self->Mem[jmp_stack[jmp_stack_idx]])))=((opcode_length) << VM_OP_BIT) | VM_OP_JMP;
			//JMP後
			*((VmOp*)(&(self->Mem[opcode_length])))=(jmp_stack[jmp_stack_idx] << VM_OP_BIT) | VM_OP_JMPEND;
		}else{
			VmOp inst = (operand << VM_OP_BIT) | opcode;
			*((VmOp*)(&(self->Mem[opcode_length])))=inst;
		}
		opcode_length+=VM_OP_LENGTH;
	}
	return opcode_length;
}

VM* VM_new(AST* ast){
	VM* self = malloc(sizeof(VM));
	self->Mem = malloc(VM_MEM);
	if(!self->Mem){
		fprintf(stderr, __FILE__"(in %d) failed to allocate mem.\n", __LINE__);
		return 0;
	}
	self->PC = 0;
	self->Ptr = self->EndPC = VM_convertASTtoByteCode(self,ast);
	return self;
}
void VM_free(VM* self){
	free(self->Mem);
	free(self);
}

void VM_show(VM* self){
	self->PC = 0;
	self->Ptr = self->EndPC;
	while(self->PC < self->EndPC){
		VmOp inst = *((unsigned int*)(&(self->Mem[self->PC])));
		switch(inst & VM_OP_MASK){
		case VM_OP_ADD:
			printf("%08x / ADD %04x\n", self->PC, (((VmOpSigned)inst) >> VM_OP_BIT));
			break;
		case VM_OP_FORWARD:
			printf("%08x / FORWARD %04x\n", self->PC, (((VmOpSigned)inst) >> VM_OP_BIT));
			break;
		case VM_OP_JMP:
			printf("%08x / JMP %04x\n", self->PC, (((VmOp)inst) >> VM_OP_BIT));
			break;
		case VM_OP_JMPEND:
			printf("%08x / JMPEND %04x\n",self->PC, (((VmOp)inst) >> VM_OP_BIT));
			break;
		case VM_OP_INPUT:
			printf("%08x / IN\n",self->PC);
			break;
		case VM_OP_OUTPUT:
			printf("%08x / OUT\n",self->PC);
			break;
		default:
			fprintf(stderr, __FILE__"(%d): invalid instruction:%02x (%dth)\n",__LINE__,inst & VM_OP_MASK,self->PC);
		}
		self->PC+=VM_OP_LENGTH;
	}
}

void VM_exec(VM* self){
	self->PC = 0;
	self->Ptr = self->EndPC;
	memset(&self->Mem[self->EndPC],0,VM_MEM-self->EndPC);
	while(self->PC < self->EndPC){
		VmOp inst = *((unsigned int*)(&(self->Mem[self->PC])));
		switch(inst & VM_OP_MASK){
		case VM_OP_ADD:
#ifdef TRACE
			printf("%08x / ADD %04x Ptr: %04x(=%02x)\n", self->PC, (((VmOpSigned)inst) >> VM_OP_BIT), self->Ptr, self->Mem[self->Ptr]);
#endif
			self->Mem[self->Ptr] = (unsigned char)(self->Mem[self->Ptr] + (((VmOpSigned)inst) >> VM_OP_BIT));
			break;
		case VM_OP_FORWARD:
#ifdef TRACE
			printf("%08x / FORWARD %04x Ptr: %04x(=%02x)\n", self->PC, (((VmOpSigned)inst) >> VM_OP_BIT), self->Ptr, (unsigned char)self->Mem[self->Ptr]);
#endif
			self->Ptr = self->Ptr + (((VmOpSigned)inst) >> VM_OP_BIT);
			break;
		case VM_OP_JMP:
#ifdef TRACE
			printf("%08x / JMP %04x Ptr: %04x(=%02x)\n", self->PC, (((VmOp)inst) >> VM_OP_BIT), self->Ptr, (unsigned char)self->Mem[self->Ptr]);
#endif
			if(!self->Mem[self->Ptr]){
				self->PC = (((VmOp)inst) >> VM_OP_BIT);
			}
			break;
		case VM_OP_JMPEND:
#ifdef TRACE
			printf("%08x / JMPEND %04x Ptr: %04x(=%02x)\n",self->PC, (((VmOpSigned)inst) >> VM_OP_BIT), self->Ptr, (unsigned char)self->Mem[self->Ptr]);
#endif
			if(self->Mem[self->Ptr]){
				self->PC = (((VmOp)inst) >> VM_OP_BIT);
			}
			break;
		case VM_OP_INPUT:
#ifdef TRACE
			printf("%08x / IN\n",self->PC);
#endif
			self->Mem[self->Ptr] = getchar();
			break;
		case VM_OP_OUTPUT:
#ifdef TRACE
			printf("%08x / OUT\n",self->PC);
#endif
			putchar(self->Mem[self->Ptr]);
			break;
		default:
			fprintf(stderr, __FILE__"(%d): invalid instruction:%02x (%dth)\n",__LINE__,inst & VM_OP_MASK,self->PC);
		}
		self->PC+=VM_OP_LENGTH;
	}
}
