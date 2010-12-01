/*
 * mem.h
 *
 *  Created on: 2010/11/28
 *      Author: psi
 */

#ifndef MEM_H_
#define MEM_H_

typedef struct MemController {
	size_t Size;
	size_t Factor;
	unsigned char* Mem;
}MemController;

MemController* MemController_new(size_t factor, size_t size);
unsigned short MemController_readShort(MemController* self, size_t idx);
unsigned char MemController_readByte(MemController* self, size_t idx);
void MemController_writeShort(MemController* self, size_t idx, unsigned short data);
void MemController_writeByte(MemController* self, size_t idx, unsigned char data);
void MemController_clear(MemController* self, size_t idx, size_t size);
#define MemController_size(self) ((self)->Size)
#define MemController_factor(self) ((self)->Factor)
void MemController_free(MemController* self);

#endif /* MEM_H_ */
