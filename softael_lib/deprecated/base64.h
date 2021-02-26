
#ifndef __BASE64__
#define __BASE64__

//------------------------------------------------------------------------------

#include "vl.h"

//------------------------------------------------------------------------------

static const uint8_t base64_encode_table[64] =
{
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
    0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
    0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
    0x77, 0x78, 0x79, 0x7A, 0x30, 0x31, 0x32, 0x33,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2B, 0x2F,
};

static const uint8_t base64_decode_table[256] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x00
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x10
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x20
    0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, // 0x30
    0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // 0x40
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, // 0x50
    0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, // 0x60
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, // 0x70
    0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x80
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x90
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xA0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xB0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xC0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xD0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xE0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0xF0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//------------------------------------------------------------------------------

inline void base64_encode(uint32_t data_size, uint8_t *data, uint32_t *base64_size, uint8_t *base64)
{
	uint32_t i;

	uint8_t frame_uint8[3];
	uint8_t frame_base64[4];

    uint32_t size_in_frames = data_uint8_size / 3;
//    uint32_t size_in_bytes = (data_uint8_size+1) / 3;
    uint32_t remainder = data_uint8_size % 3;

    uint32_t data_base64_offset;

    data_base64_offset = 0;

    for (i = 0; i < size_in_frames; ++i)
    {
        frame_uint8[0] = data_uint8[i*3+0];
        frame_uint8[1] = data_uint8[i*3+1];
        frame_uint8[2] = data_uint8[i*3+2];

        frame_base64[0] =  (frame_uint8[0] & 0xFC) >> 2;
        frame_base64[1] = ((frame_uint8[0] & 0x03) << 4) + ((frame_uint8[1] & 0xF0) >> 4);
        frame_base64[2] = ((frame_uint8[1] & 0x0F) << 2) + ((frame_uint8[2] & 0xC0) >> 6);
        frame_base64[3] =   frame_uint8[2] & 0x3F;
        
        data_base64[data_base64_offset  ] = base64_encode_table[frame_base64[0]];
        data_base64[data_base64_offset+1] = base64_encode_table[frame_base64[1]];
        data_base64[data_base64_offset+2] = base64_encode_table[frame_base64[2]];
        data_base64[data_base64_offset+3] = base64_encode_table[frame_base64[3]];

        data_base64_offset += 4;
    }

    if (remainder > 0)
    {
        frame_uint8[0] = 0x00;
        frame_uint8[1] = 0x00;
        frame_uint8[2] = 0x00;

        for (i = 0; i < remainder; ++i)
        {
            frame_uint8[i] = data_uint8[size_in_frames*3+i];
        }

        frame_base64[0] =  (frame_uint8[0] & 0xFC) >> 2;
        frame_base64[1] = ((frame_uint8[0] & 0x03) << 4) + ((frame_uint8[1] & 0xF0) >> 4);
        frame_base64[2] = ((frame_uint8[1] & 0x0F) << 2) + ((frame_uint8[2] & 0xC0) >> 6);
        frame_base64[3] =   frame_uint8[2] & 0x3F; // maybe remove
        
        for (i = 0; i < remainder+1; ++i)
        {
            data_base64[data_base64_offset] = base64_encode_table[frame_base64[i]];
            data_base64_offset++;
        }

        for (i = 0; i < 3-remainder; ++i)
        {
            data_base64[data_base64_offset] = 0x3D;
            data_base64_offset++;
        }
    }
    
    *data_base64_size = data_base64_offset;

	return;
}

//------------------------------------------------------------------------------

inline void base64_decode(uint32_t base64_size, uint8_t *base64, uint32_t *data_size, uint8_t *data)
{
    uint32_t 	i;
    uint8_t 	frame_uint8[3];
    uint8_t 	frame_base64[4];
    uint32_t 	size_in_frames = data_base64_size / 4;
    uint32_t 	size_in_bytes = size_in_frames * 3;
    uint32_t 	padding = 0;
    uint32_t 	data_uint8_offset;

    if (data_base64[data_base64_size-2] == 0x3D)
    {
        size_in_bytes -= 2;
        padding = 2;

        data_base64[data_base64_size-1] = 0x41;
        data_base64[data_base64_size-2] = 0x41;
    }

    else if (data_base64[data_base64_size-1] == 0x3D)
    {
        size_in_bytes -= 1;
        padding = 1;

        data_base64[data_base64_size-1] = 0x41;
    }

    data_uint8_offset = 0;

    for (i = 0; i < size_in_frames-1; ++i)
    {
        frame_base64[0] = data_base64[i*4+0];
        frame_base64[1] = data_base64[i*4+1];
        frame_base64[2] = data_base64[i*4+2];
        frame_base64[3] = data_base64[i*4+3];

        frame_base64[0] = base64_decode_table[frame_base64[0]];
        frame_base64[1] = base64_decode_table[frame_base64[1]];
        frame_base64[2] = base64_decode_table[frame_base64[2]];
        frame_base64[3] = base64_decode_table[frame_base64[3]];

        frame_uint8[0] = (frame_base64[0] << 2) + ((frame_base64[1] & 0x30) >> 4);
        frame_uint8[1] = ((frame_base64[1] & 0xf) << 4) + ((frame_base64[2] & 0x3c) >> 2);
        frame_uint8[2] = ((frame_base64[2] & 0x3) << 6) + frame_base64[3];
        
        data_uint8[data_uint8_offset+0] = frame_uint8[0];
        data_uint8[data_uint8_offset+1] = frame_uint8[1];
        data_uint8[data_uint8_offset+2] = frame_uint8[2];

        data_uint8_offset += 3;
    }

    if (padding > 0)
    {
        frame_base64[0] = data_base64[data_base64_size-4];
        frame_base64[1] = data_base64[data_base64_size-3];
        frame_base64[2] = data_base64[data_base64_size-2];
        frame_base64[3] = data_base64[data_base64_size-1];

        frame_base64[0] = base64_decode_table[frame_base64[0]];
        frame_base64[1] = base64_decode_table[frame_base64[1]];
        frame_base64[2] = base64_decode_table[frame_base64[2]];
        frame_base64[3] = base64_decode_table[frame_base64[3]];

        frame_uint8[0] = (frame_base64[0] << 2) + ((frame_base64[1] & 0x30) >> 4);
        frame_uint8[1] = ((frame_base64[1] & 0xf) << 4) + ((frame_base64[2] & 0x3c) >> 2);
        frame_uint8[2] = ((frame_base64[2] & 0x3) << 6) + frame_base64[3];
        
        for (i = 0; i < 3-padding; ++i)
        {
            data_uint8[data_uint8_offset+i] = frame_uint8[i];
        }

        data_uint8_offset += 3-padding;
    }
    
    *data_uint8_size = data_uint8_offset;
    
    return;
}

//------------------------------------------------------------------------------

#endif /* __BASE64__ */
