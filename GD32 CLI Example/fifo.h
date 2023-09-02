#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stdbool.h>


/* Functional Description: Generic FIFO library for deeply
   embedded system. See the unit tests for usage examples.
   This library only uses a byte sized chunk.
   This library is designed for use in Interrupt Service Routines
   and so is declared as "static inline" */


struct fifo_buffer_t {
    volatile uint16_t head;      /* first byte of data */
    volatile uint16_t tail;     /* last byte of data */
    volatile uint8_t *buffer; /* block of memory or array of data */
    uint16_t buffer_len;     /* length of the data */
};
typedef struct fifo_buffer_t FIFO_BUFFER;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint16_t FIFO_Count (FIFO_BUFFER const *b);

bool FIFO_Empty(
        FIFO_BUFFER const *b);

uint8_t FIFO_Peek(
        FIFO_BUFFER const *b);

bool FIFO_Get(
        FIFO_BUFFER * b,uint8_t *data_byte);

bool FIFO_Put(
        FIFO_BUFFER * b,
        uint8_t data_byte);

bool FIFO_BulkPut(
        FIFO_BUFFER * b,
        uint8_t* data_byte, uint32_t len);

bool FIFO_BulkGet(
        FIFO_BUFFER * b,
        uint8_t* data_byte, uint8_t len);

/* note: buffer_len must be a power of two */
void FIFO_Init(
        FIFO_BUFFER * b,
        volatile uint8_t *buffer,
        unsigned buffer_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* FIFO_H */

