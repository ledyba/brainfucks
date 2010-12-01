/*
 * mem.c
 *
 *  Created on: 2010/11/28
 *      Author: psi
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mem.h"

MemController* MemController_new(size_t factor, size_t size){
	MemController* self = malloc(sizeof(MemController));
	if(!self){
		fprintf(stderr, "failed to allocate MemController\n");
		return (MemController*)0;
	}
	self->Size = size;
	self->Factor = factor;
	size_t mem_size = (unsigned int)factor * size;
	self->Mem = malloc(mem_size);
	memset(self->Mem,0,(size_t)mem_size);
	if(!self->Mem){
		fprintf(stderr, "failed to allocate MemController\n");
		free(self);
		return (MemController*)0;
	}
	return self;
}
unsigned short MemController_readShort(MemController* self, size_t idx){
	return MemController_readByte(self,idx) +
			(MemController_readByte(self, idx+1) << 8);
}
unsigned char MemController_readByte(MemController* self, size_t idx){
	if(idx >= 0  && idx < self->Size){
#ifdef DEBUG
		printf("Read from %04d = %01x\n", idx, self->Mem[idx * self->Factor]);
#endif
		return self->Mem[idx * self->Factor];
	}
	fprintf(stderr, __FILE__"(%d):invalid idx:%d < 0 || %d > %d\n",__LINE__,idx,idx,self->Size);
	return 0;
}
void MemController_writeShort(MemController* self, size_t idx, unsigned short data){
	MemController_writeByte(self,idx, (unsigned char)(data & 0xff));
	MemController_writeByte(self, idx+1, (unsigned char)(data >> 8));
}
void MemController_writeByte(MemController* self, size_t idx, unsigned char data){
	if(idx >= 0  && idx < self->Size){
#ifdef DEBUG
		printf("Write to %04d \"%01x\".\n", idx, data);
#endif
		self->Mem[idx * self->Factor] = data;
	}else{
		fprintf(stderr, __FILE__"(%d):invalid idx:%d < 0 || %d > %d\n",__LINE__,idx,idx,self->Size);
	}
}
void MemController_clear(MemController* self, size_t idx, size_t size){
	if(size == 0){
		size = self->Size - idx;
	}
	int i;
	for(i=0;i<size;i++){
		MemController_writeByte(self, idx+i, 0);
	}
}

void MemController_free(MemController* self){
	free(self->Mem);
	free(self);
}
