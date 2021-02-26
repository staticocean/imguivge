
#ifndef __CHECKSUM_H__
#define __CHECKSUM_H__

//------------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

//------------------------------------------------------------------------------

#define CRC16_mask 		0x1021
#define CRC16_polinom 	0x8000

#define maskCRC16 		0x1021
#define polinomCRC16 	0x8000

//------------------------------------------------------------------------------

typedef struct checksum8
{
	uint8_t data[1];

} checksum8_t;

typedef struct checksum16
{
	uint8_t data[2];

} checksum16_t;

typedef struct checksum32
{
	uint8_t data[4];

} checksum32_t;

typedef struct checksum64
{
	uint8_t data[8];

} checksum64_t;

typedef struct checksum128
{
	uint8_t data[16];

} checksum128_t;

typedef struct checksum256
{
	uint8_t data[32];

} checksum256_t;

typedef struct checksum512
{
	uint8_t data[64];

} checksum512_t;

typedef struct checksum1024
{
	uint8_t data[128];

} checksum1024_t;

//------------------------------------------------------------------------------

void __checksum_get__(uint32_t cs_size, uint8_t cs[],
                        uint8_t data_size, uint8_t data[])
{
    uint32_t i;

    memset(cs, 0, cs_size);

    for (i = 0; i < data_size; ++i)
    {
        cs[i % cs_size] ^= data[i];
    }
}

//------------------------------------------------------------------------------

uint8_t __checksum_comp__(uint32_t cs_size, uint8_t cs_0[],
                        uint8_t cs_1[])
{
    uint8_t return_code = 0;

    uint32_t i;

    uint32_t *cs_word_ptr_0;
    uint32_t *cs_word_ptr_1;
    uint32_t cs_word_size;

    if (cs_size > 4)
    {
        cs_word_size = cs_size / 4;

        cs_word_ptr_0 = (uint32_t*) cs_0;
        cs_word_ptr_1 = (uint32_t*) cs_1;

        for (i = 0; i < cs_word_size; ++i)
            if (cs_word_ptr_0[i] != cs_word_ptr_1[i])
                return_code = 1;
    }

    else
    {
        for (i = 0; i < cs_size; ++i)
            if (cs_0[i] != cs_1[i])
                return_code = 1;
    }

    return return_code;
}

//------------------------------------------------------------------------------

void checksum8_get(checksum8_t *cs, uint32_t data_size, uint8_t data[])
{
    __checksum_get__(1, cs->data, data_size, data);
}

//------------------------------------------------------------------------------

uint8_t checksum8_comp(checksum8_t *cs_0, checksum8_t *cs_1)
{
    return __checksum_comp__(1, cs_0->data, cs_1->data);
}

//------------------------------------------------------------------------------

void checksum16_get(checksum16_t *cs, uint32_t data_size,
                    uint8_t data[])
{
    __checksum_get__(2, cs->data, data_size, data);
}

//------------------------------------------------------------------------------

uint8_t checksum16_comp(checksum16_t *cs_0, checksum16_t *cs_1)
{
    return __checksum_comp__(2, cs_0->data, cs_1->data);
}

//------------------------------------------------------------------------------

void checksum32_get(checksum32_t *cs, uint32_t data_size,
                    uint8_t data[])
{
    __checksum_get__(4, cs->data, data_size, data);
}

//------------------------------------------------------------------------------

uint8_t checksum32_comp(checksum32_t *cs_0, checksum32_t *cs_1)
{
    return __checksum_comp__(4, cs_0->data, cs_1->data);
}

//------------------------------------------------------------------------------

void checksum64_get(checksum64_t *cs, uint32_t data_size,
                    uint8_t data[])
{
    __checksum_get__(8, cs->data, data_size, data);
}

//------------------------------------------------------------------------------

uint8_t checksum64_comp(checksum64_t *cs_0, checksum64_t *cs_1)
{
    return __checksum_comp__(8, cs_0->data, cs_1->data);
}

//------------------------------------------------------------------------------

void checksum128_get(checksum128_t *cs, uint32_t data_size,
                    uint8_t data[])
{
    __checksum_get__(16, cs->data, data_size, data);
}

//------------------------------------------------------------------------------

uint8_t checksum128_comp(checksum128_t *cs_0, checksum128_t *cs_1)
{
    return __checksum_comp__(16, cs_0->data, cs_1->data);
}

//------------------------------------------------------------------------------

void checksum256_get(checksum256_t *cs, uint32_t data_size,
                    uint8_t data[])
{
    __checksum_get__(32, cs->data, data_size, data);
}

//------------------------------------------------------------------------------

uint8_t checksum256_comp(checksum256_t *cs_0, checksum256_t *cs_1)
{
    return __checksum_comp__(32, cs_0->data, cs_1->data);
}

//------------------------------------------------------------------------------

void checksum512_get(checksum512_t *cs, uint32_t data_size,
                    uint8_t data[])
{
    __checksum_get__(64, cs->data, data_size, data);
}

//------------------------------------------------------------------------------

uint8_t checksum512_comp(checksum512_t *cs_0, checksum512_t *cs_1)
{
    return __checksum_comp__(64, cs_0->data, cs_1->data);
}

//------------------------------------------------------------------------------

void checksum1024_get(checksum1024_t *cs, uint32_t data_size,
                    uint8_t data[])
{
    __checksum_get__(128, cs->data, data_size, data);
}

//------------------------------------------------------------------------------

uint8_t checksum1024_comp(checksum1024_t *cs_0, checksum1024_t *cs_1)
{
    return __checksum_comp__(128, cs_0->data, cs_1->data);
}

//------------------------------------------------------------------------------

uint16_t checksum_crc16_add(uint16_t crc_prev, uint32_t msg_size, uint8_t *msg)
{
    uint32_t i;
    uint32_t j;

    uint16_t CRC;
    uint16_t byte;

    CRC = crc_prev;
    
    for (i = 0; i < msg_size; ++i)
    {
        byte = msg[i] << 8;
        
        for (j = 0; j < 8; ++j)
        {
            if ((CRC ^ byte) & 0x8000)
            {
                CRC = (CRC << 1) ^ 0x1021;
            }

            else
            {
                CRC = CRC << 1;
            }
            
            byte = byte << 1;
        }
    }
    
    return CRC;
}

inline uint16_t checksum_crc16(uint32_t msg_size, uint8_t *msg)
{
    return checksum_crc16_add(0xFFFF, msg_size, msg);
}

//------------------------------------------------------------------------------

#endif /* __CHECKSUM_H__ */









