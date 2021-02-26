//
//  main.c
//  svmp
//
//  Created by John Brown on 12/08/2018.
//  Copyright © 2018 staticocean. All rights reserved.
//

#ifndef __SVMP__
#define __SVMP__

//------------------------------------------------------------------------------
// Synchronised Virtual Memory Protocol
//------------------------------------------------------------------------------

#include <stdint.h>

//------------------------------------------------------------------------------

#define SVMP_TS_MASK                0x01

#define SVMP_ID_MASK_MSG            0x780
#define SVMP_ID_MASK_SRC            0x07F

#define SVMP_ADDR_HOLD              0x0800
#define SVMP_ADDR_DATA              0x07FF
#define SVMP_ADDR_OFFSET            0xF000

#define SVMP_ID_TSR                 0x000       /* Time Sync Request            */
#define SVMP_ID_TSI                 0x080       /* Time Sync Initiate           */
#define SVMP_ID_TSP                 0x100       /* Time Sync Process            */
#define SVMP_ID_TSE                 0x180       /* Time Sync End                */

#define SVMP_ID_RXS                 0x200       /* Reception Single             */
#define SVMP_ID_RXF                 0x280       /* Reception File               */

#define SVMP_ID_TXS                 0x300       /* Transmission Single          */
#define SVMP_ID_TXF                 0x380       /* Transmission File            */
#define SVMP_ID_TXR                 0x400       /* Transmission Request         */

#define SVMP_ID_STI                 0x480       /* State Transmission Initiate  */
#define SVMP_ID_STR                 0x500       /* State Transmission Request   */

#define SVMP_ID_CSR                 0x580       /* Checksum request */       
#define SVMP_ID_CST                 0x600       /* Checksum transmission */

#define SVMP_ID_R68                 0x680
#define SVMP_ID_R70                 0x700

#define SVMP_ID_ISF                 0x780       /* Internal Service Frame */

#define SVMP_USART_SLIP_FEND        0xC0
#define SVMP_USART_SLIP_TFEND       0xDC
#define SVMP_USART_SLIP_FESC        0xDB
#define SVMP_USART_SLIP_TFESC       0xDD

#define SVMP_USART_RX_STATE_BOM     0x01
#define SVMP_USART_RX_STATE_FESC    0x02

#define SVMP_RDWR_RD                0x01
#define SVMP_RDWR_WR                0x02
//#define SVMP_RDWR_WR                0x04

//------------------------------------------------------------------------------

// fscs - Frame Sequence CheckSum

typedef struct __attribute__((packed)) svmp_tsr
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint8_t         res[6];
    
}   svmp_tsr_t;

typedef struct __attribute__((packed)) svmp_tsi
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint8_t         server_48H[6];
    
}   svmp_tsi_t;

typedef struct __attribute__((packed)) svmp_tsp
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint8_t         c_tx[3]; // Mask 0x0FFFFF ~ 1 sec
    uint8_t         res[3];
    
}   svmp_tsp_t;

/*
    e = 0.5 * [(srx+stx+g1-sr) - (ctx+crx+g0-cr)]
*/

typedef struct __attribute__((packed)) svmp_tse
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint8_t         c_tx[3]; // Mask 0x0FFFFF ~ 1 sec
    uint8_t         s_cts[3]; // corrected time stamp
    
}   svmp_tse_t;

typedef struct __attribute__((packed)) svmp_rxs
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint16_t        addr;
    uint8_t         data[4];
    
}   svmp_rxs_t;

typedef struct __attribute__((packed)) svmp_rxf
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint16_t        addr;
    uint16_t        offset;
    uint8_t         data[2];
    
}   svmp_rxf_t;

typedef struct __attribute__((packed)) svmp_txs
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint16_t        addr;
    uint8_t         data[4];
    
}   svmp_txs_t;

typedef struct __attribute__((packed)) svmp_txf
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint16_t        addr;
    uint16_t        offset;
    uint8_t         data[2];
    
}   svmp_txf_t;

typedef struct __attribute__((packed)) svmp_txr
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint16_t        addr[2];
    
}   svmp_txr_t;

typedef struct __attribute__((packed)) svmp_sti
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint8_t         ts_32L[3];
    uint8_t         group_delay[3];
    
}   svmp_sti_t;

typedef struct __attribute__((packed)) svmp_str
{
    uint8_t         dest;
    uint8_t         frcs;
    uint16_t        addr;
    
}   svmp_str_t;

typedef struct __attribute__((packed)) svmp_csr
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint16_t        addr;
    
}   svmp_csr_t;

typedef struct __attribute__((packed)) svmp_cst
{
    uint8_t         dest;
    uint8_t         frcs;
    
    uint16_t        addr;
    uint32_t        checksum;
    
}   svmp_cst_t;

//------------------------------------------------------------------------------

typedef union svmp_data
{
    svmp_tsr_t      tsr;
    svmp_tsi_t      tsi;
    svmp_tsp_t      tsp;
    svmp_tse_t      tse;
    svmp_rxs_t      rxs;
    svmp_rxf_t      rxf;
    svmp_txr_t      txr;
    svmp_txs_t      txs;
    svmp_txf_t      txf;
    svmp_str_t      str;
    svmp_sti_t      sti;
    svmp_csr_t      csr;
    svmp_cst_t      cst;
    
    uint8_t         bytes[8];
    
}   svmp_data_t;

typedef struct svmp_frame
{
    uint8_t         __rx_state__;
    uint16_t        __rx_crc__;
    
    uint16_t        id;
    uint8_t         dlc;
    
    svmp_data_t     data;
    
    uint64_t        time_stamp;

}   svmp_frame_t;

typedef struct svmp_inst
{
    uint8_t         device_id;
    uint8_t         broadcast_enable;
    
    uint8_t         frcs_table[SVMP_ID_MASK_SRC + 0x01];
    uint16_t        cben_table[SVMP_ID_MASK_SRC + 0x01]; // callback enable table

    uint8_t         ts_filter;
    uint16_t        ts_latency      [SVMP_ID_MASK_SRC + 0x01];
    uint16_t        ts_group_delay  [SVMP_ID_MASK_SRC + 0x01];
    
    void            (*transmit_callback) (svmp_frame_t *);

    void            (*tsr_callback) (struct svmp_inst*, svmp_frame_t*);
    void            (*tsi_callback) (struct svmp_inst*, svmp_frame_t*);
    void            (*tsp_callback) (struct svmp_inst*, svmp_frame_t*);
    void            (*tse_callback) (struct svmp_inst*, svmp_frame_t*);
    
    void            (*rxs_callback) (struct svmp_inst*, svmp_frame_t*);
    void            (*rxf_callback) (struct svmp_inst*, svmp_frame_t*);
    
    void            (*txr_callback) (struct svmp_inst*, svmp_frame_t*);
    void            (*txs_callback) (struct svmp_inst*, svmp_frame_t*);
    void            (*txf_callback) (struct svmp_inst*, svmp_frame_t*);
    
    void            (*str_callback) (struct svmp_inst*, svmp_frame_t*);
    void            (*sti_callback) (struct svmp_inst*, svmp_frame_t*);
    
    void            (*csr_callback) (struct svmp_inst*, svmp_frame_t*);
    void            (*cst_callback) (struct svmp_inst*, svmp_frame_t*);

    void            (*isf_callback) (struct svmp_inst*, svmp_frame_t*);
    
}   svmp_inst_t;

typedef struct svmp_config
{
    uint8_t         device_id;

    void            (*transmit_callback) (svmp_frame_t *);
    
    void            (*tsr_callback) (svmp_inst_t*, svmp_frame_t*);
    void            (*tsi_callback) (svmp_inst_t*, svmp_frame_t*);
    void            (*tsp_callback) (svmp_inst_t*, svmp_frame_t*);
    void            (*tse_callback) (svmp_inst_t*, svmp_frame_t*);
    
    void            (*rxs_callback) (svmp_inst_t*, svmp_frame_t*);
    void            (*rxf_callback) (svmp_inst_t*, svmp_frame_t*);
    
    void            (*txr_callback) (svmp_inst_t*, svmp_frame_t*);
    void            (*txs_callback) (svmp_inst_t*, svmp_frame_t*);
    void            (*txf_callback) (svmp_inst_t*, svmp_frame_t*);
    
    void            (*str_callback) (svmp_inst_t*, svmp_frame_t*);
    void            (*sti_callback) (svmp_inst_t*, svmp_frame_t*);
    
    void            (*csr_callback) (struct svmp_inst*, svmp_frame_t*);
    void            (*cst_callback) (struct svmp_inst*, svmp_frame_t*);

    void            (*isf_callback) (struct svmp_inst*, svmp_frame_t*);

}   svmp_config_t;

//------------------------------------------------------------------------------

#define __svmp_crc16_template__(bit_pos, bit_mask)                              \
                                                                                \
    switch (((crc_init >> (8 + bit_pos)) ^ ((uint8_t*) data)[i]) & bit_mask)    \
    {                                                                           \
        case 0x00: crc_init = crc_init << 1; break;                             \
        default: crc_init = (crc_init << 1) ^ 0x1021; break;                    \
    }

uint16_t __svmp_crc16__ (uint16_t crc_init, uint32_t data_size, void *data)
{
    uint32_t i;
    
    for (i = 0; i < data_size; ++i)
    {
        __svmp_crc16_template__(0x00, 0x80);
        __svmp_crc16_template__(0x01, 0x40);
        __svmp_crc16_template__(0x02, 0x20);
        __svmp_crc16_template__(0x03, 0x10);
        __svmp_crc16_template__(0x04, 0x08);
        __svmp_crc16_template__(0x05, 0x04);
        __svmp_crc16_template__(0x06, 0x02);
        __svmp_crc16_template__(0x07, 0x01);
    }
    
    return crc_init;
}

// Fletcher32 src: https://en.wikipedia.org/wiki/Fletcher%27s_checksum
uint16_t __svmp_fletcher16__(uint32_t data_size, uint8_t *data)
{
    uint32_t c0, c1;
    uint32_t i;

    for (c0 = c1 = 0; data_size >= 5802; data_size -= 5802)
    {
        for (i = 0; i < 5802; ++i)
        {
            c0 = c0 + *data++;
            c1 = c1 + c0;
        }

        c0 = c0 % 255;
        c1 = c1 % 255;
    }

    for (i = 0; i < data_size; ++i)
    {
        c0 = c0 + *data++;
        c1 = c1 + c0;
    }

    c0 = c0 % 255;
    c1 = c1 % 255;

    return (c1 << 8 | c0);
}

uint32_t __svmp_cs32__(uint32_t data_size, uint8_t *data)
{
    uint32_t cs32_data = 0x00;

    cs32_data |= __svmp_fletcher16__ (data_size / 2, &data[0]);
    cs32_data |= __svmp_fletcher16__ (data_size - data_size / 2, &data[data_size - data_size / 2 - 1]) << 16;

    return cs32_data;
}

//------------------------------------------------------------------------------

uint32_t __svmp_usart_slip__ (uint32_t data_size, void *dest, void *src)
{
    uint32_t i;
    uint32_t dest_offset = 0x00;
    
    for (i = 0; i < data_size; ++i)
    {
        if (((uint8_t*) src)[i] == SVMP_USART_SLIP_FEND)
        {
            ((uint8_t*) dest)[dest_offset+0] = SVMP_USART_SLIP_FESC;
            ((uint8_t*) dest)[dest_offset+1] = SVMP_USART_SLIP_TFEND;
            
            dest_offset += 2;
        }
        
        else if (((uint8_t*) src)[i] == SVMP_USART_SLIP_FESC)
        {
            ((uint8_t*) dest)[dest_offset+0] = SVMP_USART_SLIP_FESC;
            ((uint8_t*) dest)[dest_offset+1] = SVMP_USART_SLIP_TFESC;
            
            dest_offset += 2;
        }
        
        else
        {
            ((uint8_t*) dest)[dest_offset] = ((uint8_t*) src)[i];
            dest_offset++;
        }
    }
    
    return dest_offset;
}

//------------------------------------------------------------------------------

void svmp_init (svmp_inst_t *inst, svmp_config_t *config)
{
    uint32_t i;
    
    inst->__smph_handle__ = xSemaphoreCreateBinary();
    xSemaphoreGive(inst->__smph_handle__);
    
    inst->device_id = config->device_id;
    
    inst->ts_filter = 0x00;
    
    for (i = 0; i < SVMP_ID_MASK_SRC; ++i)
    {
        inst->ts_latency[i] = 0x00;
        inst->ts_group_delay[i] = 0x00;
    }
    
    inst->transmit_callback = config->transmit_callback;
    
    inst->tsr_callback = config->tsr_callback;
    inst->tsi_callback = config->tsi_callback;
    inst->tsp_callback = config->tsp_callback;
    inst->tse_callback = config->tse_callback;

    inst->rxs_callback = config->rxs_callback;
    inst->rxf_callback = config->rxf_callback;

    inst->txr_callback = config->txr_callback;
    inst->txs_callback = config->txs_callback;
    inst->txf_callback = config->txf_callback;

    inst->str_callback = config->str_callback;
    inst->sti_callback = config->sti_callback;

    inst->csr_callback = config->csr_callback;
    inst->cst_callback = config->cst_callback;

    for (i = 0; i < (SVMP_ID_MASK_SRC+1); ++i)
    {
        inst->frcs_table[i] = 0x00;
        inst->cben_table[i] = 0x00;
    }

    svmp_broadcast_enable(inst);
    
    return;
}

//------------------------------------------------------------------------------

void svmp_broadcast_enable(svmp_inst_t *inst)
{
    inst->broadcast_enable = 0x01;
    return;
}

void svmp_broadcast_disable(svmp_inst_t *inst)
{
    inst->broadcast_enable = 0x00;
    return;
}

//------------------------------------------------------------------------------

void svmp_set_cben (svmp_inst_t *inst, uint8_t device_id, uint8_t cben_tags)
{
    inst->cben_table[device_id] = cben_tags;

    return;
}

//------------------------------------------------------------------------------

void svmp_frcs_write(svmp_inst_t *inst, svmp_frame_t *frame)
{
    uint8_t src = frame->id & SVMP_ID_MASK_SRC;

    frame->data.bytes[1] = inst->frcs_table[src];
    
//    if (frame->data.bytes[1] == 0x00)
//    {
//        frame->data.bytes[1] = 0xFF;
//    }

    return;
}

//------------------------------------------------------------------------------

void svmp_frcs_update(svmp_inst_t *inst, svmp_frame_t *frame)
{
    uint8_t src = frame->id & SVMP_ID_MASK_SRC;
    uint8_t dest = frame->data.bytes[0] & SVMP_ID_MASK_SRC;

//    inst->frcs_table[src] =
//        ((inst->frcs_table[src] + 0x10) & 0xF0) |
//        ((inst->frcs_table[src] + 0x00) & 0x0F);
//
//    inst->frcs_table[dest] =
//        ((inst->frcs_table[dest] + 0x00) & 0xF0) |
//        ((inst->frcs_table[dest] + 0x01) & 0x0F);

    inst->frcs_table[src ] = (inst->frcs_table[src ] + 1) & 0x7F;
    inst->frcs_table[dest] = (inst->frcs_table[dest] + 1) & 0x7F;
    
    return;
}

//------------------------------------------------------------------------------

void svmp_usart_reset (svmp_frame_t *frame)
{
    frame->__rx_state__ = 0x00;
    frame->__rx_crc__   = 0x00;
    
    return;
}

//------------------------------------------------------------------------------

void svmp_transmit_frame (svmp_inst_t *inst, svmp_frame_t *frame)
{
    svmp_frcs_write (inst, frame);
    inst->transmit_callback(frame);
    svmp_frcs_update (inst, frame);

    return;
}

//------------------------------------------------------------------------------

void svmp_usart_write (svmp_frame_t *frame, uint32_t *data_size, uint8_t *data)
{
    uint16_t cs_crc16 = 0x0000;
    
    frame->id |= (((uint16_t) frame->dlc) << 11);

    cs_crc16 = __svmp_crc16__(cs_crc16, 2, &frame->id);
    cs_crc16 = __svmp_crc16__(cs_crc16, frame->dlc, &frame->data);
    
    data[0] = SVMP_USART_SLIP_FEND;
    
    *data_size = 0x01;
    
    *data_size += __svmp_usart_slip__(2, &data[*data_size], &cs_crc16);
    *data_size += __svmp_usart_slip__(2, &data[*data_size], &frame->id);
    *data_size += __svmp_usart_slip__(frame->dlc, &data[*data_size], (uint8_t*) &frame->data);
    
    data[*data_size] = SVMP_USART_SLIP_FEND;
    
    *data_size = *data_size + 1;
    
    frame->id = frame->id & (SVMP_ID_MASK_MSG | SVMP_ID_MASK_SRC);

    return;
}

//------------------------------------------------------------------------------

uint8_t svmp_usart_read (svmp_frame_t *frame, uint32_t data_size, uint8_t *data)
{
    uint8_t return_code = 0x00;
    uint32_t i;
    
    for (i = 0; i < data_size; ++i)
    {
        if      ((frame->__rx_state__ & SVMP_USART_RX_STATE_BOM)
                 && (data[i] == SVMP_USART_SLIP_FEND)
                 && (frame->dlc >= 4))
        {
            frame->__rx_state__ = 0x00;
            frame->dlc = frame->dlc-4;
            
            uint16_t cs_crc16 = 0x0000;
            
            cs_crc16 = __svmp_crc16__(cs_crc16, 2, (uint8_t*) &frame->id);
            cs_crc16 = __svmp_crc16__(cs_crc16, frame->dlc, frame->data.bytes);
            
            if (frame->__rx_crc__ == cs_crc16)
            {
                frame->dlc = frame->id >> 11;
                frame->id  = frame->id & (SVMP_ID_MASK_MSG | SVMP_ID_MASK_SRC);

                return_code = 0x01;
            }
        }
        
        else if (data[i] == SVMP_USART_SLIP_FEND)
        {
            frame->__rx_state__ = SVMP_USART_RX_STATE_BOM;
            frame->__rx_crc__ = 0x00;
            frame->dlc = 0x00;
        }
        
        // Handle FESC
        
        else if (( frame->__rx_state__ & SVMP_USART_RX_STATE_BOM )
                 && (~frame->__rx_state__ & SVMP_USART_RX_STATE_FESC)
                 && (data[i] == SVMP_USART_SLIP_FESC))
        {
            frame->__rx_state__ |= SVMP_USART_RX_STATE_FESC;
        }
        
        // Handle TFEND in data field
        
        else if ((frame->__rx_state__ & SVMP_USART_RX_STATE_BOM )
                 && (frame->__rx_state__ & SVMP_USART_RX_STATE_FESC)
                 && (data[i] == SVMP_USART_SLIP_TFEND))
        {
            switch (frame->dlc)
            {
                case 0x00: frame->__rx_crc__ = SVMP_USART_SLIP_FEND; break;
                case 0x01: frame->__rx_crc__ |= ((uint16_t) SVMP_USART_SLIP_FEND) << 8; break;
                case 0x02: frame->id = SVMP_USART_SLIP_FEND; break;
                case 0x03: frame->id |= ((uint16_t) SVMP_USART_SLIP_FEND) << 8; break;
                default  : frame->data.bytes[frame->dlc-4] = SVMP_USART_SLIP_FEND; break;
            }
            
            frame->__rx_state__ = SVMP_USART_RX_STATE_BOM;
            frame->dlc++;
        }
        
        // Handle TFESC in data field
        
        else if ((frame->__rx_state__ & SVMP_USART_RX_STATE_BOM )
                 && (frame->__rx_state__ & SVMP_USART_RX_STATE_FESC)
                 && (data[i] == SVMP_USART_SLIP_TFESC))
        {
            switch (frame->dlc)
            {
                case 0x00: frame->__rx_crc__ = SVMP_USART_SLIP_FESC; break;
                case 0x01: frame->__rx_crc__ |= ((uint16_t) SVMP_USART_SLIP_FESC) << 8; break;
                case 0x02: frame->id = SVMP_USART_SLIP_FESC; break;
                case 0x03: frame->id |= ((uint16_t) SVMP_USART_SLIP_FESC) << 8; break;
                default  : frame->data.bytes[frame->dlc-4] = SVMP_USART_SLIP_FESC; break;
            }
            
            frame->__rx_state__ = SVMP_USART_RX_STATE_BOM;
            frame->dlc++;
        }
        
        else if ((frame->__rx_state__ & SVMP_USART_RX_STATE_BOM )
                 && (~frame->__rx_state__ & SVMP_USART_RX_STATE_FESC)
                 && (data[i] != SVMP_USART_SLIP_FEND)
                 && (data[i] != SVMP_USART_SLIP_FESC))
        {
            switch (frame->dlc)
            {
                case 0x00: frame->__rx_crc__ = data[i]; break;
                case 0x01: frame->__rx_crc__ |= ((uint16_t) data[i]) << 8; break;
                case 0x02: frame->id = data[i]; break;
                case 0x03: frame->id |= ((uint16_t) data[i]) << 8; break;
                default  : frame->data.bytes[frame->dlc-4] = data[i]; break;
            }
            
            frame->dlc++;
        }
        
        else
        {
            frame->__rx_state__ = 0x00;
            frame->__rx_crc__ = 0x00;
            frame->dlc = 0x00;
        }
    }
    
    return return_code;
}

//------------------------------------------------------------------------------

void svmp_tx_tsr (svmp_inst_t *inst, uint8_t dest)
{
    __svmp_lock__(inst);

    svmp_frame_t frame_tx;

    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_TSR;
    frame_tx.dlc = 8;
    
    frame_tx.data.tsr.dest = dest & SVMP_ID_MASK_SRC;

    frame_tx.data.tsr.res[0] = 0x00;
    frame_tx.data.tsr.res[1] = 0x00;
    frame_tx.data.tsr.res[2] = 0x00;
    frame_tx.data.tsr.res[3] = 0x00;
    frame_tx.data.tsr.res[4] = 0x00;
    frame_tx.data.tsr.res[5] = 0x00;
    
    __svmp_unlock__(inst);

    svmp_transmit_frame (inst, &frame_tx);

    return;
}

//------------------------------------------------------------------------------

void svmp_tx_tsi (svmp_inst_t *inst, uint8_t dest, uint64_t time_stamp)
{
    __svmp_lock__(inst);

    svmp_frame_t frame_tx;

    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_TSI;
    frame_tx.dlc = 8;
    
    frame_tx.data.tsi.dest = dest & SVMP_ID_MASK_SRC;
    
    time_stamp = time_stamp >> 16;

    frame_tx.data.tsi.server_48H[0] = (time_stamp >>  0) & 0xFF;
    frame_tx.data.tsi.server_48H[1] = (time_stamp >>  8) & 0xFF;
    frame_tx.data.tsi.server_48H[2] = (time_stamp >> 16) & 0xFF;
    frame_tx.data.tsi.server_48H[3] = (time_stamp >> 24) & 0xFF;
    frame_tx.data.tsi.server_48H[4] = (time_stamp >> 32) & 0xFF;
    frame_tx.data.tsi.server_48H[5] = (time_stamp >> 40) & 0xFF;
    
    __svmp_unlock__(inst);

    svmp_transmit_frame (inst, &frame_tx);
    
    return;
}

//------------------------------------------------------------------------------

void svmp_tx_tsp (svmp_inst_t *inst, uint8_t dest, uint32_t c_tx)
{
    __svmp_lock__(inst);

    svmp_frame_t frame_tx;

    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_TSP;
    frame_tx.dlc = 8;
    
    frame_tx.data.tsp.dest = dest & SVMP_ID_MASK_SRC;
    
    c_tx = c_tx & 0x0FFFFF;

    frame_tx.data.tsp.c_tx[0] = (c_tx >>  0) & 0xFF;
    frame_tx.data.tsp.c_tx[1] = (c_tx >>  8) & 0xFF;
    frame_tx.data.tsp.c_tx[2] = (c_tx >> 16) & 0xFF;
    
    frame_tx.data.tsp.res[0] = 0x00;
    frame_tx.data.tsp.res[1] = 0x00;
    frame_tx.data.tsp.res[2] = 0x00;
    
    __svmp_unlock__(inst);

    svmp_transmit_frame (inst, &frame_tx);
    
    return;
}

//------------------------------------------------------------------------------

void svmp_tx_tse (svmp_inst_t *inst, uint8_t dest,
                  uint32_t c_tx, uint32_t s_rx, uint32_t s_tx)
{
    __svmp_lock__(inst);

    svmp_frame_t frame_tx;

    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_TSE;
    frame_tx.dlc = 8;
    
    frame_tx.data.tse.dest = dest & SVMP_ID_MASK_SRC;
    
    // allow negative overflow
    int64_t __c_tx__ = c_tx & 0x0FFFFF;
    int64_t __s_rx__ = s_rx & 0x0FFFFF;
    int64_t __s_tx__ = s_tx & 0x0FFFFF;

    if (__s_rx__ < __c_tx__)
    {
        __s_rx__ = __s_rx__ + 0x100000;
        __s_tx__ = __s_tx__ + 0x100000;
    }

    int64_t __s_cts__ = __s_tx__ + __s_rx__;// + inst->group_delay - inst->rx_delay;
    uint64_t s_cts = (__s_cts__ < 0x00) ? -__s_cts__ : +__s_cts__;

    s_cts |= (__s_cts__ < 0x00) ? 0x800000 : 0x000000;

    frame_tx.data.tse.c_tx[0] = (c_tx >>  0) & 0xFF;
    frame_tx.data.tse.c_tx[1] = (c_tx >>  8) & 0xFF;
    frame_tx.data.tse.c_tx[2] = (c_tx >> 16) & 0xFF;

    frame_tx.data.tse.s_cts[0] = (s_cts >>  0) & 0xFF;
    frame_tx.data.tse.s_cts[1] = (s_cts >>  8) & 0xFF;
    frame_tx.data.tse.s_cts[2] = (s_cts >> 16) & 0xFF;

    __svmp_unlock__(inst);
    
    svmp_transmit_frame (inst, &frame_tx);

    return;
}

//------------------------------------------------------------------------------

void svmp_tx_rxs (svmp_inst_t *inst, uint8_t dest,
                  uint16_t data_id, uint32_t data_size, uint8_t *data)
{
    uint32_t i;
    
    svmp_frame_t frame_tx;

    data_id = data_id & SVMP_ADDR_DATA;
    data_size = (data_size > 64) ? 64 : data_size;
    
    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_RXS;
    frame_tx.data.rxs.dest = dest & SVMP_ID_MASK_SRC;
    
    for (i = 0; i < data_size / 4; ++i)
    {
        frame_tx.dlc = 8;
        frame_tx.data.rxs.addr = data_id | (i << 12);
        
        frame_tx.data.rxs.data[0] = ((uint8_t*) data)[i*4 + 0];
        frame_tx.data.rxs.data[1] = ((uint8_t*) data)[i*4 + 1];
        frame_tx.data.rxs.data[2] = ((uint8_t*) data)[i*4 + 2];
        frame_tx.data.rxs.data[3] = ((uint8_t*) data)[i*4 + 3];
        
        if ((data_size % 4) != 0x00 ||
            (i + 1) != (data_size / 4))
        {
            frame_tx.data.rxs.addr |= SVMP_ADDR_HOLD;
        }
    
        svmp_transmit_frame (inst, &frame_tx);
    }

    if (data_size % 4 != 0x00)
    {
        frame_tx.dlc = 4 + (data_size % 4);
        frame_tx.data.rxs.addr = data_id | ((data_size / 4) << 12);
        
        for (i = 0; i < (data_size % 4); ++i)
        {
            frame_tx.data.rxs.data[i] =
            ((uint8_t*) data)[(data_size - data_size % 4) + i];
        }
        
        svmp_transmit_frame (inst, &frame_tx);
    }

    return;
}

//------------------------------------------------------------------------------

void svmp_tx_rxf (svmp_inst_t *inst, uint8_t dest,
                  uint16_t data_id, uint32_t data_size, void *data)
{
    uint32_t i;
    
    svmp_frame_t frame_tx;

    data_id = data_id & SVMP_ADDR_DATA;
    data_size = (data_size > 2097152) ? 2097152 : data_size;
    
    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_RXF;
    frame_tx.data.rxf.dest = dest & SVMP_ID_MASK_SRC;
    
    uint8_t high_4b;
    uint8_t low_16b;
    
    for (i = 0; i < data_size / 2; ++i)
    {
        high_4b = (i & 0xF0000) >> 4;
        low_16b = (i & 0x0FFFF) >> 0;
        
        frame_tx.dlc = 8;
        frame_tx.data.rxf.addr = data_id | high_4b;
        frame_tx.data.rxf.offset = low_16b;
        
        frame_tx.data.rxf.data[0] = ((uint8_t*) data)[i*2 + 0];
        frame_tx.data.rxf.data[1] = ((uint8_t*) data)[i*2 + 1];
        
        if ((data_size % 2) != 0x00 ||
            (i + 1) != (data_size / 2))
        {
            frame_tx.data.rxf.addr |= SVMP_ADDR_HOLD;
        }
    
        svmp_transmit_frame (inst, &frame_tx);
    }

    if (data_size % 2 != 0x00)
    {
        high_4b = ((data_size / 2) & 0xF0000) >> 4;
        low_16b = ((data_size / 2) & 0x0FFFF) >> 0;
        
        frame_tx.dlc = 6 + (data_size % 2);
        frame_tx.data.rxf.addr = data_id | high_4b;
        frame_tx.data.rxf.offset = low_16b;
        
        for (i = 0; i < (data_size % 2); ++i)
        {
            frame_tx.data.rxf.data[i] =
            ((uint8_t*) data)[(data_size - data_size % 2) + i];
        }
        
        svmp_transmit_frame (inst, &frame_tx);
    }

    // __svmp_unlock__(inst);

    return;
}

//------------------------------------------------------------------------------

void svmp_tx_txr (svmp_inst_t *inst, uint8_t dest,
                  uint16_t data_id_low, uint16_t data_id_high)
{
    __svmp_lock__(inst);

    svmp_frame_t frame_tx;

    data_id_low  = data_id_low  & SVMP_ADDR_DATA;
    data_id_high = data_id_high & SVMP_ADDR_DATA;
    
    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_TXR;
    frame_tx.dlc = 4;
    
    frame_tx.data.txr.dest = dest & SVMP_ID_MASK_SRC;

    frame_tx.data.txr.addr[0] = data_id_low;
    frame_tx.data.txr.addr[1] = data_id_high;
    
    __svmp_unlock__(inst);

    svmp_transmit_frame (inst, &frame_tx);

    return;
}

//------------------------------------------------------------------------------

void svmp_tx_txs (svmp_inst_t *inst, uint8_t dest,
                  uint16_t data_id, uint32_t data_size, void *data)
{
    // __svmp_lock__(inst);

    uint32_t i;
    
    svmp_frame_t frame_tx;

    data_size = (data_size > 64) ? 64 : data_size;
    data_id = data_id & SVMP_ADDR_DATA;
    
    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_TXS;
    frame_tx.data.txs.dest = dest & SVMP_ID_MASK_SRC;
    
    for (i = 0; i < data_size / 4; ++i)
    {
        frame_tx.dlc = 8;
        frame_tx.data.txs.addr = data_id | (i << 12);
        
        frame_tx.data.txs.data[0] = ((uint8_t*) data)[i*4 + 0];
        frame_tx.data.txs.data[1] = ((uint8_t*) data)[i*4 + 1];
        frame_tx.data.txs.data[2] = ((uint8_t*) data)[i*4 + 2];
        frame_tx.data.txs.data[3] = ((uint8_t*) data)[i*4 + 3];

        if ((data_size % 4) != 0x00
            || (i + 1) != (data_size / 4))
        {
            frame_tx.data.txs.addr |= SVMP_ADDR_HOLD;
        }

        svmp_transmit_frame (inst, &frame_tx);
    }
    
    if ((data_size % 4) != 0)
    {
        frame_tx.dlc = 4 + (data_size % 4);
        frame_tx.data.txs.addr = data_id | ((data_size / 4) << 12);
        
        for (i = 0; i < (data_size % 4); ++i)
        {
            frame_tx.data.txs.data[i] =
                    ((uint8_t*) data)[(data_size - data_size % 4) + i];
        }

        svmp_transmit_frame (inst, &frame_tx);
    }
    
    // __svmp_unlock__(inst);

    return;
}

//------------------------------------------------------------------------------

void svmp_tx_txf (svmp_inst_t *inst, uint8_t dest,
                  uint16_t data_id, uint32_t data_size, void *data)
{
    // __svmp_lock__(inst);

    uint32_t i;
    
    svmp_frame_t frame_tx;

    data_size = (data_size > 2097152) ? 2097152 : data_size;
    data_id = data_id & SVMP_ADDR_DATA;
    
    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_TXF;
    frame_tx.data.txf.dest = dest & SVMP_ID_MASK_SRC;
    
    uint16_t high_4b;
    uint16_t low_16b;
    
    for (i = 0; i < data_size / 2; ++i)
    {
        high_4b = (i & 0xF0000) >> 4;
        low_16b = (i & 0x0FFFF) >> 0;
        
        frame_tx.dlc = 8;
        frame_tx.data.txf.addr = data_id | high_4b;
        frame_tx.data.txf.offset = low_16b;
        
        frame_tx.data.txf.data[0] = ((uint8_t*) data)[i*2 + 0];
        frame_tx.data.txf.data[1] = ((uint8_t*) data)[i*2 + 1];

        if ((data_size % 2) != 0x00
            || (i + 1) != (data_size / 2))
        {
            frame_tx.data.txf.addr |= SVMP_ADDR_HOLD;
        }

        svmp_transmit_frame (inst, &frame_tx);
    }
    
    if ((data_size % 2) != 0)
    {
        high_4b = ((data_size / 2) & 0xF0000) >> 4;
        low_16b = ((data_size / 2) & 0x0FFFF) >> 0;
        
        frame_tx.dlc = 6 + (data_size % 2);
        frame_tx.data.txf.addr = data_id | high_4b;
        frame_tx.data.txf.offset = low_16b;
        
        for (i = 0; i < (data_size % 2); ++i)
        {
            frame_tx.data.txf.data[i] =
                    ((uint8_t*) data)[(data_size - data_size % 2) + i];
        }

        svmp_transmit_frame (inst, &frame_tx);
    }
    
    return;
}

//------------------------------------------------------------------------------

void svmp_tx_str (svmp_inst_t *inst, uint8_t dest)
{
    __svmp_lock__(inst);

    svmp_frame_t frame_tx;

    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_STR;
    frame_tx.dlc = 2;
    
    frame_tx.data.str.dest = dest & SVMP_ID_MASK_SRC;

    __svmp_unlock__(inst);
    
    svmp_transmit_frame (inst, &frame_tx);

    return;
}

//------------------------------------------------------------------------------

void svmp_tx_sti (svmp_inst_t *inst, uint8_t dest, uint32_t ts_32L)
{
    __svmp_lock__(inst);

    svmp_frame_t frame_tx;
    
    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_STI;
    frame_tx.dlc = 8;
    
    frame_tx.data.sti.dest = dest & SVMP_ID_MASK_SRC;
    
    frame_tx.data.sti.ts_32L[0] = (ts_32L >>  0) & 0xFF;
    frame_tx.data.sti.ts_32L[1] = (ts_32L >>  8) & 0xFF;
    frame_tx.data.sti.ts_32L[2] = (ts_32L >> 16) & 0xFF;

    frame_tx.data.sti.group_delay[0] = 0x00;
    frame_tx.data.sti.group_delay[1] = 0x00;
    frame_tx.data.sti.group_delay[2] = 0x00;

    __svmp_unlock__(inst);
    
    svmp_transmit_frame (inst, &frame_tx);

    return;
}

//------------------------------------------------------------------------------

void svmp_tx_csr (svmp_inst_t *inst, uint8_t dest, uint16_t addr)
{
    __svmp_lock__(inst);

    svmp_frame_t frame_tx;
    
    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_CSR;
    frame_tx.dlc = 4;
    
    frame_tx.data.csr.dest = dest & SVMP_ID_MASK_SRC;
    frame_tx.data.csr.addr = addr & SVMP_ADDR_DATA;
    
    __svmp_unlock__(inst);
    
    svmp_transmit_frame (inst, &frame_tx);
    
    return;
}

void svmp_tx_cst (svmp_inst_t *inst, uint8_t dest, uint16_t addr, uint32_t data_size, uint8_t *data)
{
    __svmp_lock__(inst);

    svmp_frame_t frame_tx;
    
    frame_tx.id = (inst->device_id & SVMP_ID_MASK_SRC) | SVMP_ID_CST;
    frame_tx.dlc = 8;
    
    frame_tx.data.cst.dest = dest & SVMP_ID_MASK_SRC;
    frame_tx.data.cst.addr = addr & SVMP_ADDR_DATA;

    frame_tx.data.cst.checksum = __svmp_cs32__(data_size, data);
    
    __svmp_unlock__(inst);
    
    svmp_transmit_frame (inst, &frame_tx);
    
    return;
}

//------------------------------------------------------------------------------

inline void svmp_rx (svmp_inst_t *inst, svmp_frame_t *frame)
{
    // __svmp_lock__(inst);

    uint8_t src  = frame->id & SVMP_ID_MASK_SRC;
    uint8_t dest = frame->data.bytes[0] & SVMP_ID_MASK_SRC;

    if (dest == (inst->device_id & SVMP_ID_MASK_SRC)
        || (inst->broadcast_enable && dest == 0x00))
    {
        svmp_frcs_update(inst, frame);

        switch (frame->id & SVMP_ID_MASK_MSG)
        {
            case SVMP_ID_TSR: inst->tsr_callback(inst, frame); break;
            case SVMP_ID_TSI: inst->tsi_callback(inst, frame); break;
            case SVMP_ID_TSP: inst->tsp_callback(inst, frame); break;
            case SVMP_ID_TSE: inst->tse_callback(inst, frame); break;
        
            case SVMP_ID_RXS: inst->rxs_callback(inst, frame); break;
            case SVMP_ID_RXF: inst->rxf_callback(inst, frame); break;
        
            case SVMP_ID_TXR: inst->txr_callback(inst, frame); break;
            case SVMP_ID_TXS: inst->txs_callback(inst, frame); break;
            case SVMP_ID_TXF: inst->txf_callback(inst, frame); break;
        
            case SVMP_ID_STR: inst->str_callback(inst, frame); break;
            case SVMP_ID_STI: inst->sti_callback(inst, frame); break;
        
            case SVMP_ID_CSR: inst->csr_callback(inst, frame); break;
            case SVMP_ID_CST: inst->cst_callback(inst, frame); break;
        }
    }

    // if ((frame->id & SVMP_ID_MASK_MSG) == SVMP_ID_STI)
    // {
    //     switch (frame->id & SVMP_ID_MASK_MSG)
    //     {
    //         case SVMP_ID_STI: inst->sti_callback(inst, frame); break;
    //         case SVMP_ID_ISF: inst->sti_callback(inst, frame); break;
    //     }
    // }

    // __svmp_unlock__(inst);
}

//------------------------------------------------------------------------------

#endif /* __SVMP__ */

