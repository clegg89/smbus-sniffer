#ifndef BUFFER_H_
#define BUFFER_H_

// ******************************************************
// * A circular buffer for the data received by I2C     *
// *                                                    *
// * Instead of printing the I2C data as soon as it is  *
// * received, we store them in the memory and print    *
// * them when the processor has time to spare.         *
// *                                                    *
// * This buffer is circular, meaning that whenever we  *
// * run out of space, we go back to the start, erasing *
// * any previous data.                                 *
// ******************************************************

/* Each word is encoded to represent a byte on the i2c line:
 * Bit 0-7 Data // On START transactions bit 0 is R/W
 * Bit 8 ACk/NACK
 * Bit 9 == STOP
 * Bit 10 == START
 * Bit 11-15 Reserved
 */
#define I2C_BUFFER_SIZE 1000
extern uint16_t buffer[I2C_BUFFER_SIZE];
extern uint16_t bufferPos;   // the current writing position inside the buffer
extern uint16_t bufferStart; // the current reading position inside the buffer

#endif /* BUFFER_H_ */
