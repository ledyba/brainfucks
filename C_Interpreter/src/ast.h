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
#define AST_TYPE_VAL_INC		3 /* + */
#define AST_TYPE_VAL_DEC		4 /* - */
#define AST_TYPE_OUT			5 /* . */
#define AST_TYPE_INPUT		6 /* , */
#define AST_TYPE_LOOP_START	7 /* [ */
#define AST_TYPE_LOOP_END	8 /* ] */

typedef struct ASTNode {
	unsigned int type;
	int jump_index;	/* LOOPで使う */
}ASTNode;

typedef struct AST {
	unsigned int opcodes_count;
	ASTNode* opcodes;
}AST;

AST* AST_new(const char* filename);
void AST_free(AST* self);
#define AST_length(self) ((self)->opcodes_count)
#define AST_readNode(self,index) ( &((self)->opcodes[(index)]) )
#define ASTNode_type(self) ((self)->type)
#define ASTNode_jmpIdx(self) ((self)->jump_index)

#endif /* AST_H_ */

