#include "fifo.h"

/* Functional Description: Generic FIFO library for deeply
 embedded system. See the unit tests for usage examples. */

/****************************************************************************
 * DESCRIPTION: Returns the number of elements in the ring buffer
 * RETURN:      Number of elements in the ring buffer
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
uint16_t FIFO_Count(FIFO_BUFFER const *b) {
	return (b ? (b->head - b->tail) : 0);
}

/****************************************************************************
 * DESCRIPTION: Returns the empty/full status of the ring buffer
 * RETURN:      true if the ring buffer is full, false if it is not.
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
static bool FIFO_Full(FIFO_BUFFER const *b) {
	return (b ? (FIFO_Count(b) == b->buffer_len) : true);
}

/****************************************************************************
 * DESCRIPTION: Returns the empty/full status of the ring buffer
 * RETURN:      true if the ring buffer is empty, false if it is not.
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
bool FIFO_Empty(FIFO_BUFFER const *b) {
	return (b ? (FIFO_Count(b) == 0) : true);
}

/****************************************************************************
 * DESCRIPTION: Looks at the data from the head of the list without removing it
 * RETURN:      byte of data, or zero if nothing in the list
 * ALGORITHM:   none
 * NOTES:       Use Empty function to see if there is data to retrieve
 *****************************************************************************/
uint8_t FIFO_Peek(FIFO_BUFFER const *b) {
	if (b) {
		return (b->buffer[b->tail % b->buffer_len]);
	}

	return 0;
}

/****************************************************************************
 * DESCRIPTION: Gets the data from the front of the list, and removes it
 * RETURN:      the data, or zero if nothing in the list
 * ALGORITHM:   none
 * NOTES:       Use Empty function to see if there is data to retrieve
 *****************************************************************************/
bool FIFO_Get(FIFO_BUFFER *b, uint8_t *data_byte) {
	if (!FIFO_Empty(b)) {
		*data_byte = b->buffer[b->tail % b->buffer_len];
		b->tail++;
	} else {
		return false;
	}
	return true;
}

/****************************************************************************
 * DESCRIPTION: Adds an element of data to the FIFO
 * RETURN:      true on successful add, false if not added
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
bool FIFO_Put(FIFO_BUFFER *b, uint8_t data_byte) {
	bool status = false; /* return value */

	if (b) {
		/* limit the ring to prevent overwriting */
		if (!FIFO_Full(b)) {
			b->buffer[b->head % b->buffer_len] = data_byte;
			b->head++;
			status = true;
		}
	}

	return status;
}

/****************************************************************************
 * DESCRIPTION: Adds string to the FIFO
 * RETURN:      true on successful add, false if not added
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
bool FIFO_BulkPut(FIFO_BUFFER *b, uint8_t *data_byte, uint32_t len) {
	bool status = false; /* return value */
	uint32_t i;

	if (!b) {
		return false;
	}
	for (i = 0; i < len; ++i) {
		/* limit the ring to prevent overwriting */
		if (!FIFO_Full(b)) {
			b->buffer[b->head % b->buffer_len] = data_byte[i];
			b->head++;
			status = true;
		} else {
			status = false;
		}
	}

	return status;
}
/****************************************************************************
 * DESCRIPTION: Gets string from the FIFO
 * RETURN:      true on successful add, false if not added
 * ALGORITHM:   none
 * NOTES:       none
 *****************************************************************************/
bool FIFO_BulkGet(FIFO_BUFFER *b, uint8_t *data_byte, uint8_t len) {
	bool status = false; /* return value */
	uint8_t i;

	if (!b) {
		return false;
	}
	for (i = 0; i < len; ++i) {
		/* see if there is data to retrieve */
		if (!FIFO_Empty(b)) {
			data_byte[i] = b->buffer[b->tail % b->buffer_len];
			b->tail++;
			status = true;
		} else {
			status = false;
		}
	}

	return status;
}

/****************************************************************************
 * DESCRIPTION: Configures the ring buffer
 * RETURN:      none
 * ALGORITHM:   none
 * NOTES:       buffer_len must be a power of two
 *****************************************************************************/
void FIFO_Init(FIFO_BUFFER *b, volatile uint8_t *buffer, unsigned buffer_len) {
	if (b) {
		b->head = 0;
		b->tail = 0;
		b->buffer = buffer;
		b->buffer_len = buffer_len;
	}

	return;
}
