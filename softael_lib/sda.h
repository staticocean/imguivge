
#ifndef __SDA__
#define __SDA__

//------------------------------------------------------------------------------
// SDA - SYNCHRONISED DATA ACCESS
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vl.h>

//------------------------------------------------------------------------------

#define SDA_DEPTH_2 	0x01
#define SDA_DEPTH_4 	0x03
#define SDA_DEPTH_8 	0x07
#define SDA_DEPTH_16 	0x0F
#define SDA_DEPTH_32  	0x1F
#define SDA_DEPTH_64  	0x3F
#define SDA_DEPTH_128 	0x7F
#define SDA_DEPTH_256 	0xFF

typedef struct sda
{
	uint8_t mask;
	uint32_t size;
	
	volatile uint8_t offset;
	void *data;

}	s_sda;

inline void sda_init(s_sda *sda, uint32_t size, void *data)
{
	sda->mask 		= SDA_DEPTH_8;
	sda->size 		= size;
	
	sda->offset 	= 0x00;
	sda->data 		= data;
	
	return;
}

inline void sda_fetch(s_sda *sda, void *data)
{
	uint8_t offset = __sync_add_and_fetch(&sda->offset, 0x00);
	void *addr = (void*) ((uintptr_t) sda->data + (offset & sda->mask) * sda->size);
	
	memcpy(data, addr, sda->size);
	
	return;
}

inline void sda_write(s_sda *sda, void *data)
{
	uint8_t offset = __sync_add_and_fetch(&sda->offset, 0x00);
	void *addr = (void*) ((uintptr_t) sda->data + (offset & sda->mask) * sda->size);
	
	memcpy(addr, data, sda->size);
	
	return;
}

//------------------------------------------------------------------------------


#endif /* __SCH__ */






