/*
 * vector.h
 *
 *  Created on: 2011/01/31
 *      Author: psi
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <stddef.h>

typedef struct VectorItem VectorItem;
typedef struct Vector Vector;
typedef void (*VectorCopyFunc)(void* dst,void* src);

struct VectorItem {
	Vector* parent;
	VectorItem* next;
	VectorItem* prev;
	int self_allocated;
	void* item;
};

struct Vector {
	int size;
	size_t obj_size;
	VectorCopyFunc copy_func;
	VectorItem* first;
	VectorItem* last;
};


Vector* Vector_new(size_t obj_size, VectorCopyFunc copy_func);
void* Vector_add(Vector* self, void* item);
void* Vector_insert(Vector* self, int index, void* item);
void* Vector_delete(Vector* self, int index);
VectorItem* Vector_getItem(Vector* self, int index);
#define Vector_get(self, index) (VectorItem_item(Vector_getItem((self),(index))))
void* Vector_toArray(Vector* self);
void Vector_free(Vector* self, int need_free);
#define Vector_size(self) ((self)->size)
#define Vector_length(self) Vector_size(self)
#define Vector_isEmpty (Vector_size(self) <= 0)

#define VectorItem_parent(self) ((self)->parent)
#define VectorItem_item(self) ((self)->item)
#define VectorItem_next(self) ((self)->next)
#define VectorItem_prev(self) ((self)->prev)
#define VectorItem_isFirst(self) (((self)->prev) == NULL)
#define VectorItem_isEnd(self) (((self)->next) == NULL)

#endif /* VECTOR_H_ */
