/******************************************************************************
 *
 * Module: TWI (I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI (I2C) AVR driver
 *
 * Author: Mohamed Tarek (modified and documented by Eng. Kerolous Labib)
 *
 *******************************************************************************/

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

/*
 * Description:
 * Type used to represent the device address on the I2C bus.
 * - Used when this microcontroller operates as a slave.
 * - 7-bit addressing is typical, but stored in an 8-bit variable.
 */
typedef uint8 TWI_AddressType;


/*
 * Description:
 * enum for different standard I2C bit rates (SCL frequencies).
 * The selected mode determines the communication speed.
 *
 * Note:
 * SCL frequency is determined from the formula:
 *
 *      SCL frequency = (CPU Clock frequency) / (16 + 2 * TWBR * 4^TWPS)
 *
 */
typedef enum {
	TWI_STANDARD_MODE     = 100000UL,   /* 100 kbps  */
	TWI_FAST_MODE         = 400000UL,   /* 400 kbps  */
	TWI_FAST_PLUS_MODE    = 1000000UL,  /* 1 Mbps    */
	TWI_HIGH_SPEED_MODE   = 3400000UL,  /* 3.4 Mbps  */
	TWI_ULTRA_FAST_MODE   = 5000000UL   /* 5.0 Mbps  */
} TWI_BaudRateType;


/*
 * Description:
 * Configuration structure for TWI initialization.
 *
 * Members:
 *   - address  : The MCU’s address when used as a slave.
 *   - bit_rate : Desired SCL clock frequency (communication speed).
 */
typedef struct {
	TWI_AddressType address;
	TWI_BaudRateType bit_rate;
} TWI_ConfigType;

extern TWI_ConfigType TWI_Config;

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/*
 * Description:
 * These macros define common I2C status codes
 * that can be read from the TWSR register (after masking with 0xF8).
 * Each represents a specific event or state in the I2C communication process.
 */

#define TWI_START         0x08  /* START condition transmitted */
#define TWI_REP_START     0x10  /* Repeated START condition transmitted */
#define TWI_MT_SLA_W_ACK  0x18  /* Master transmitted (SLA+W), ACK received */
#define TWI_MT_SLA_R_ACK  0x40  /* Master transmitted (SLA+R), ACK received */
#define TWI_MT_DATA_ACK   0x28  /* Master transmitted data, ACK received */
#define TWI_MR_DATA_ACK   0x50  /* Master received data, sent ACK */
#define TWI_MR_DATA_NACK  0x58  /* Master received data, sent NACK */


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Initialize the TWI (I2C) module.
 * - Sets the bit rate (SCL frequency) based on the provided configuration.
 * - Sets the device address if acting as a slave.
 * - Enables the TWI hardware module.
 *
 */
void TWI_init(const TWI_ConfigType * Config_Ptr);


/*
 * Description:
 * Send a START condition on the I2C bus.
 * Used to initiate communication as a master.
 */
void TWI_start(void);


/*
 * Description:
 * Send a STOP condition on the I2C bus.
 * Used to release the bus after communication is complete.
 */
void TWI_stop(void);


/*
 * Description:
 * Send a single byte of data over the I2C bus.
 *
 * Inputs:
 *   - data: Byte to be transmitted.
 */
void TWI_writeByte(uint8 data);


/*
 * Description:
 * Read a byte of data from the I2C bus and send ACK after reception.
 *
 * Returns:
 *   - Received data byte.
 */
uint8 TWI_readByteWithACK(void);


/*
 * Description:
 * Read a byte of data from the I2C bus and send NACK after reception.
 * Used when this is the last byte to read.
 *
 * Returns:
 *   - Received data byte.
 */
uint8 TWI_readByteWithNACK(void);


/*
 * Description:
 * Read and return the current TWI status code from TWSR register.
 * (Masked to keep only the status bits: bits 7–3).
 *
 * Returns:
 *   - 5-bit TWI status code (upper 5 bits of TWSR).
 */
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
