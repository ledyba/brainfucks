/*
 * vm.h
 *
 *  Created on: 2008/10/09
 *      Author: Account01
 */

#ifndef AST_H_
#define AST_H_

#define AST_TYPE_NOP			0 /* ?? */
#define AST_TYPE_PT_INC		1 /* > */
#define AST_TYPE_PT_DEC		2 /* < */
#define AST_TYPE_VAL_INC		4 /* + */
#define AST_TYPE_VAL_DEC		8 /* - */
#define AST_TYPE_OUT			16 /* . */
#define AST_TYPE_INPUT		32 /* , */
#define AST_TYPE_LOOP_START	64 /* [ */
#define AST_TYPE_LOOP_END	128 /* ] */

typedef struct ASTNode {
	unsigned char type;
	int jump_index;	/* LOOPで使う */
}ASTNode;

typedef struct AST {
	unsigned int opcodes_size;
	ASTNode* opcodes;
}AST;

AST* AST_new(const char* filename);
void AST_free(AST* self);
void AST_show(AST* self);
#define AST_length(self) ((self)->opcodes_size)
#define AST_readNode(self,index) ( &((self)->opcodes[(index)]) )
#define ASTNode_type(self) ((self)->type)
#define ASTNode_jmpIdx(self) ((self)->jump_index)

/*
 *  実行関係
 */

typedef struct ASTMachine {
	AST* ast;
	unsigned char memory[32*1024];
	unsigned int eip;
	unsigned char* ptr;
} ASTMachine;

ASTMachine* ASTMachine_new(AST* ast);
void ASTMachine_exec(ASTMachine* self);
void ASTMachine_free(ASTMachine* self);

#endif /* AST_H_ */

