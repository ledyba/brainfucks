/*
 * vector.c
 *
 *  Created on: 2011/01/31
 *      Author: psi
 */

#include "vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

Vector* Vector_new(size_t obj_size, VectorCopyFunc copy_func){
	Vector* self = malloc(sizeof(Vector));
	if(!self){
		fprintf(stderr, "Failed to malloc.\n");
		exit(EXIT_FAILURE);
	}
	self->first = self->last = NULL;
	self->copy_func = copy_func;
	self->obj_size=obj_size;
	self->size=0;
	return self;
}

VectorItem* _Vector_createVectorItem(Vector* self, void* item){
	VectorItem* vec_item = NULL;
	if(!item){
		vec_item = (VectorItem*)malloc(sizeof(VectorItem) + self->obj_size);
		if(!vec_item){
			fprintf(stderr, "[%s][%d] Failed to malloc.\n",__FILE__,__LINE__);
			exit(EXIT_FAILURE);
		}
		vec_item->item = &(((char*)vec_item)[sizeof(VectorItem)]);
		vec_item->self_allocated = 1;
	}else{
		vec_item = (VectorItem*)malloc(sizeof(VectorItem));
		if(!vec_item){
			fprintf(stderr, "[%s][%d] Failed to malloc.\n",__FILE__,__LINE__);
			exit(EXIT_FAILURE);
		}
		vec_item->self_allocated = 0;
		vec_item->item = item;
	}
	vec_item->parent = self;
	vec_item->prev = NULL;
	vec_item->next = NULL;
	return vec_item;
}
void* _VectorItem_insert(VectorItem* self, VectorItem* item){
	if(!self){
		return NULL;
	}
	item->parent = self->parent;
	item->prev = self;
	VectorItem* next = self->next;
	if(next){
		item->next=next;
		next->prev = item;
	}else{
		item->next=NULL;
		self->parent->last = item;
	}
	self->next=item;
	self->parent->size++;
	return item->item;
}
void _VectorItem_free(VectorItem* self, int need_free){
	if(!self->self_allocated && need_free){
		free(self->item);
	}
	free(self);
}

void* Vector_add(Vector* self, void* item){
	VectorItem* vec_item = _Vector_createVectorItem(self,item);
	if(self->size > 0){
		VectorItem* last = self->last;
		return _VectorItem_insert(last,vec_item);
	}else{
		self->first = self->last = vec_item;
		self->size=1;
		return vec_item->item;
	}
}
void* Vector_insert(Vector* self, int index, void* item){
	if(self->size <= 0 && index==0){
		return Vector_add(self,item);
	}
	VectorItem* last = Vector_getItem(self,index);
	if(!last){
		return NULL;
	}
	VectorItem* vec_item = _Vector_createVectorItem(self,item);
	return _VectorItem_insert(last,vec_item);
}
void* Vector_delete(Vector* self, int index){
	VectorItem* item = Vector_getItem(self,index);
	if(!item){
		return NULL;
	}
	if(item->self_allocated){
		_VectorItem_free(item,0);
		return NULL;
	}
	void* ret = VectorItem_item(item);
	_VectorItem_free(item,0);
	return ret;
}
VectorItem* Vector_getItem(Vector* self, int index){
	int i;
	VectorItem* item = self->first;
	if(index >= self->size){
		return NULL;
	}
	for(i=0;i<index;i++){
		if(!VectorItem_isEnd(item)){
			break;
		}
		item = VectorItem_next(item);
	}
	if(i == index){
		return item;

	}
	return NULL;
}
void* Vector_toArray(Vector* self){
	char* mem = malloc(self->obj_size * self->size);
	if(!mem){
		fprintf(stderr, "[%s][%d] Failed to malloc.\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	int idx = 0;
	VectorItem* item;
	for(item = self->first;item != NULL;item=VectorItem_next(item)){
		if(self->copy_func){
			self->copy_func(&mem[idx],VectorItem_item(item));
		}else{
			memcpy(&mem[idx],VectorItem_item(item),self->obj_size);
		}
		idx += self->obj_size;
	}
	return mem;
}
void Vector_free(Vector* self, int need_free){
	VectorItem* item;
	for(item = self->first;item != NULL;){
		VectorItem* next = VectorItem_next(item);
		_VectorItem_free(item,need_free);
		item = next;
	}
	free(self);
}
